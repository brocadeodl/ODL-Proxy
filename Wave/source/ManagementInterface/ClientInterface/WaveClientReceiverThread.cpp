/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/WaveClientReceiverThread.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/WaveManagementServerInformation.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ClientInterface/WaveManagementServer.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include <typeinfo>

using namespace std;

namespace WaveNs
{

WaveClientReceiverThread::WaveClientReceiverThread (ServerStreamingSocket *pServerStreamingSocket)
    : PrismPosixThread         (),
      m_pServerStreamingSocket (pServerStreamingSocket), 
      m_peerServerPort (0),
      m_peerServerId (0),
      m_peerServerMessageVersion (""),
      m_peerServerSerializationType (SERIALIZE_WITH_UNKNOWN) 
{
}

WaveClientReceiverThread::~WaveClientReceiverThread ()
{
    if (NULL != m_pServerStreamingSocket)
    {
        m_peerServerMutex.lock ();

        (WaveClientTransportObjectManager::getInstance ())->disconnectFromServer (m_peerServerId, false);

        delete m_pServerStreamingSocket;
        m_pServerStreamingSocket = NULL;

        m_peerServerMutex.unlock ();

        // delete this thread from the cache of WaveClientReceiverObjectManager if those details were not already removed.

        if (0 != m_peerServerPort)
        {
            WaveClientReceiverObjectManager::removeWaveClientReceiverThreadFromCache (m_peerServerIpAddress, m_peerServerPort);
        }
    }
}

WaveThreadStatus WaveClientReceiverThread::start ()
{
    trace (TRACE_LEVEL_DEVEL, "WaveClientReceiverThread::start : Starting ...");

                 ResourceId     connectionStatus                            = FRAMEWORK_ERROR;
                 UI32           sizeOfConnectionInformation                 = 0;
                 string         peerIpAddress                               = m_pServerStreamingSocket->getPeerIpAddress ();
                 string         serverInformationString;
                 string         peerServerIpAddress;
                 UI32           peerServerPort                              = 0;
                 bool           isSuccessful                                = false;
				 UI32 			peerServerId							    = 0;
    static const PrismServiceId waveUserInterfaceObjectManagerServiceId     = WaveUserInterfaceObjectManager::getPrismServiceId ();
                 UI32           serverUniqueIdentifierFromServerPerspective = 0;
                 string         messageVersion                              = "";

    // Validate the client first

    isSuccessful = authorizeClient ();

    // If the client is authorized, then read the peer's Connection Information (ipaddress and server port).

    if (true == isSuccessful)
    {
        isSuccessful = (*m_pServerStreamingSocket) >> sizeOfConnectionInformation;

        if (true == isSuccessful)
        {
            if (0 == sizeOfConnectionInformation)
            {
                trace (TRACE_LEVEL_FATAL, "WaveThreadStatus WaveClientReceiverThread::start : If we have read the size successfully, it cannot be zero.");
                prismAssert (0 != sizeOfConnectionInformation, __FILE__, __LINE__);
            }
        }

        if (0 == sizeOfConnectionInformation)
        {
            // Indicate that the size could not be read

            isSuccessful = false;
        }
    }

    // By the time we reach here the sizeOfConnectionInformation should not be 0.  If it is still zero, set it to a min value so that the fixed size buffer will be created without assert.
    // In any case the client will not be accepted.

    if (0 == sizeOfConnectionInformation)
    {
        sizeOfConnectionInformation = 1;
    }

    FixedSizeBuffer WaveManagementServerInformationBuffer (sizeOfConnectionInformation);

    if (true == isSuccessful)
    {
        isSuccessful = (*m_pServerStreamingSocket) >> WaveManagementServerInformationBuffer;
    }

    if (true == isSuccessful)
    {
        if (0 == (WaveManagementServerInformationBuffer.getCurrentSize ()))
        {
            isSuccessful = false;
        }
    }

    WaveManagementServerInformation serverInformation;

    if (true == isSuccessful)
    {
        WaveManagementServerInformationBuffer.toString (serverInformationString);
        serverInformation.loadFromSerializedData2 (serverInformationString, SERIALIZE_WITH_ATTRIBUTE_NAME);

        peerServerIpAddress                         = serverInformation.getIpAddress                                   ();
        peerServerPort                              = serverInformation.getPort                                        ();
        serverUniqueIdentifierFromServerPerspective = serverInformation.getServerUniqueIdentifierFromServerPerspective ();
        messageVersion                              = serverInformation.getMessageVersion                               (); 

        trace (TRACE_LEVEL_DEBUG, string ("                           MessageVersion                  : \"") + messageVersion + "\"");
        if (true == messageVersion.empty())
        {
            messageVersion = string("0.0.0"); 
        }

        trace (TRACE_LEVEL_DEBUG, string ("Management Server Location Ip Address                      : \"") + peerServerIpAddress + "\"");
        trace (TRACE_LEVEL_DEBUG, string ("                           Port                            : \"") + peerServerPort + "\"");
        trace (TRACE_LEVEL_DEBUG, string ("                           MessageVersion                  : \"") + messageVersion + "\"");
        trace (TRACE_LEVEL_DEBUG, string ("Management Server Location Connection Interface Ip Address : \"") + peerIpAddress + "\"");
        trace (TRACE_LEVEL_DEBUG, string ("Management Server Unique Identifier From ServerPerspective  : \"") + serverUniqueIdentifierFromServerPerspective + "\"");
    }

    if (0 != (strcmp (peerIpAddress.c_str (),  peerServerIpAddress.c_str ())))
    {
        // Reset the value in peer Ip Address.

        peerIpAddress = peerServerIpAddress;
    }

    if (true == isSuccessful)
    {
        // The following must be assigned before we call addInterLocationMessageReceiverThreadToCache.

        setPeerServerDetails (peerServerIpAddress, (SI32) peerServerPort, messageVersion);

        peerServerId = (WaveClientTransportObjectManager::getInstance ())->getServerIdIfKnownForUniqueServerIdentifier (m_peerServerIpAddress, m_peerServerPort, serverUniqueIdentifierFromServerPerspective);

        if (0 == peerServerId)
        {
            isSuccessful = false;
			trace (TRACE_LEVEL_WARN, "WaveClientReceiverThread::start : Dropping this connection since forward connection is not available (Peer Server Is Not known)");
        }
	}

    if (true == isSuccessful)
    {
        setPeerServerId (peerServerId);

        // Add the service name/id cache to the server.

        (WaveClientTransportObjectManager::getInstance ())->addServiceCache (peerServerId, serverInformation.getServiceNames (), serverInformation.getServiceIds ());

        // Make sure that there is no other thread handling the same location.  If there is another one,
        // wait till it exits.

        WaveClientReceiverThread *pTempInterLocationMessageReceiverThread = this;

        bool cacheStatus = WaveClientReceiverObjectManager::addWaveClientReceiverThreadToCache (m_peerServerIpAddress, m_peerServerPort, pTempInterLocationMessageReceiverThread, messageVersion);

        if (true == cacheStatus)
        {
            // Before starting to do anything, let us make sure that we are connected to the peer.

            // The connection to the Server is originated by the client itself.  Hence, we are already connected to the server and there is no need to connect to the server again.
            //connectionStatus = (WaveClientTransportObjectManager::getInstance ())->connectToNewManagementInterfaceServer (m_peerServerIpAddress, m_peerServerPort);
            connectionStatus = FRAMEWORK_SUCCESS;
        }

        UI32 messageSize = 0;

        while ((FRAMEWORK_SUCCESS == connectionStatus) || (FRAMEWORK_UNKNOWN_LOCATION == connectionStatus))
        {
            isSuccessful = (*m_pServerStreamingSocket) >> messageSize;

            // Message size of 0xFFFFFFFF indicates that the peer is shutting down the connection.

            if ((false == isSuccessful) || (0xFFFFFFFF == messageSize) || (0 == messageSize))
            {
                break;
            }

            FixedSizeBuffer *pNewFixedSizeBuffer = new FixedSizeBuffer (messageSize);

            //trace (TRACE_LEVEL_DEVEL, string ("WaveClientReceiverThread::start : received a message size indicator : ") + messageSize + ".");

            if (NULL == pNewFixedSizeBuffer)
            {
                //trace (TRACE_LEVEL_FATAL, string ("WaveClientReceiverThread::start : Could not allocate a new FixedSizeBuffer of size ") + messageSize + ".");
                prismAssert (false, __FILE__, __LINE__);
            }

            isSuccessful = (*m_pServerStreamingSocket) >> (*pNewFixedSizeBuffer);

            //trace (TRACE_LEVEL_DEVEL, "WaveClientReceiverThread::start : Completed reading the buffer.");

            if ((false == isSuccessful) || (0 == (pNewFixedSizeBuffer->getCurrentSize ())))
            {
                delete pNewFixedSizeBuffer;
                break;
            }
            else
            {
                // First create a PrismMessage from the contents of the FixedBuffer that we have read from the network.
                // Now depending on the contents of the message that arrived decide if it is a new request or a response to a request
                // this location sent out.

                // We can safely cast from SerializableObject pointer to PrismMessage pointer since we know that only object that
                // travels brtween two nodes in a Prism based cluster is a PrismMessage;
                // FIXME : sagar : enforce that the object type that was returned is indeed a PrismMessage or a specialization of
                //                 PrismMessage.

                string messageString;

                pNewFixedSizeBuffer->toString (messageString);

                // We do not need the Fixed Size Buffer anymore.  So delete it.

                delete pNewFixedSizeBuffer;

                //trace (TRACE_LEVEL_DEVEL, messageString);

                //prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

                // Now read the buffer data

                UI32          numberOfBuffers = 0;
                UI32          i               = 0;
                vector<UI32>  bufferTags;
                vector<UI32>  bufferSizes;
                vector<UI8 *> buffers;

                isSuccessful = (*m_pServerStreamingSocket) >> numberOfBuffers;

                if (true == isSuccessful)
                {
                    for (i = 0; i < numberOfBuffers; i++)
                    {
                        UI32  bufferTag  = 0;
                        UI32  bufferSize = 0;
                        void *pBuffer    = NULL;

                        isSuccessful = (*m_pServerStreamingSocket) >> bufferTag;

                        if (false == isSuccessful)
                        {
                            break;
                        }

                        isSuccessful = (*m_pServerStreamingSocket) >> bufferSize;

                        if ((false == isSuccessful) || (0 == bufferSize))
                        {
                            break;
                        }

                        prismAssert (0 != bufferSize, __FILE__, __LINE__);

                        pBuffer = new UI8[bufferSize];

                        prismAssert (NULL != pBuffer, __FILE__, __LINE__);

                        FixedSizeBuffer tempBuffer (bufferSize, pBuffer, false);

                        isSuccessful = (*m_pServerStreamingSocket) >> tempBuffer;

                        if ((false == isSuccessful) || (0 == (tempBuffer.getCurrentSize ())))
                        {
                            delete[] (reinterpret_cast<UI8 *> (pBuffer));
                            break;
                        }

                        bufferTags.push_back  (bufferTag);
                        bufferSizes.push_back (bufferSize);
                        buffers.push_back     (reinterpret_cast<UI8 *> (pBuffer));
                    }
                }

                UI8              serializationType                    = m_peerServerSerializationType;
                UI32             messageIdAtOriginatingClientLocation = PrismMessage::getWaveClientMessageId (messageString, serializationType);
                WaveMessageType  messageType                          = PrismMessage::getType (messageString, serializationType);
                PrismMessage    *pPrismMessage                        = NULL;

                if (WAVE_MESSAGE_TYPE_REQUEST == messageType)
                {
                    pPrismMessage = PrismMessage::createAndLoadFromSerializedData2 (messageString, 0, serializationType);
                }
                else if (WAVE_MESSAGE_TYPE_RESPONSE == messageType)
                {
                    //UI32 messageIdAtOriginatingLocation = PrismMessage::getMessageIdAtOriginatingLocation (messageString);

                    pPrismMessage = (WaveClientTransportObjectManager::getInstance ())->getPendingMessage (messageIdAtOriginatingClientLocation);

                    if (NULL != pPrismMessage)
                    {
                        pPrismMessage->removeAllBuffers ();
                        pPrismMessage->loadFromSerializedData2 (messageString, serializationType);
                    }
                }
                else
                {
                    prismAssert (false, __FILE__, __LINE__);
                }

                // If we have a message at hand then attach the buffer to the message.
                // Otherwise destroy the buffer.

                numberOfBuffers = buffers.size ();

                if (NULL != pPrismMessage)
                {
                    for (i = 0; i < numberOfBuffers; i++)
                    {
                        pPrismMessage->addBuffer (bufferTags[i], bufferSizes[i], buffers[i], true);
                    }
                }
                else
                {
                    for (i = 0; i < numberOfBuffers; i++)
                    {
                        delete[] (reinterpret_cast<UI8 *> (buffers[i]));
                    }
                }
                
                // if we have a message at hand
                // Now depending on the message type process it.

                ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

                if (NULL != pPrismMessage)
                {
                    //Set the details of the client in the mmessage itself so that we know where to send it back.

                    pManagementInterfaceMessage = dynamic_cast<ManagementInterfaceMessage *> (pPrismMessage);

                    if (NULL == pManagementInterfaceMessage)
                    {
                        trace (TRACE_LEVEL_FATAL, "WaveClientReceiverThread::start : Please make sure that only the messages derived from ManagementInterfaceMessage are used in CLI/GUI clients.");
                        trace (TRACE_LEVEL_FATAL, "WaveClientReceiverThread::start : The type of the failed message : " + string ((typeid (*pPrismMessage)).name ()));
                        prismAssert (false, __FILE__, __LINE__);
                    }
                    else
                    {
                        pManagementInterfaceMessage->setServerId (m_peerServerId);
                    }
                }

                if (NULL != pManagementInterfaceMessage)
                {
                    if (WAVE_MESSAGE_TYPE_REQUEST == messageType)
                    {
                        trace (TRACE_LEVEL_DEVEL, "WaveClientReceiverThread::start : We received a Remote message destined to this location and delivering it to corresponding service.");

                        // If we successfully read the entire message and its buffers then send it.
                        // Otherwise simply destroy it.  We cannot deliver half read messages.

                        if (true == isSuccessful)
                        {
                            pManagementInterfaceMessage->setServiceCode (waveUserInterfaceObjectManagerServiceId);
                            (WaveClientTransportObjectManager::getInstance ())->sendToBeUsedByReceiverThreads (pManagementInterfaceMessage);
                        }
                        else
                        {
                            delete pManagementInterfaceMessage;
                        }
                    }
                    else if (WAVE_MESSAGE_TYPE_RESPONSE == messageType)
                    {
                        //trace (TRACE_LEVEL_DEVEL, "WaveClientReceiverThread::start : We received a Remote message response from a remote location.");

                        // If we successfully read the entire message and its buffers then process the message response normally.
                        // Otherwise indicate that the message buffers could not be read properly.

                        if (false == isSuccessful)
                        {
                            pManagementInterfaceMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_INCOMPLETE_BUFFER_READ_FROM_REMOTE_LOCATION);
                        }

                        (WaveClientTransportObjectManager::getInstance ())->replyToBeUsedByReceiverThreads (pManagementInterfaceMessage);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_FATAL, "WaveClientReceiverThread::start : Currently, Only Messages and their Responses can be transported across Locations.");
                        prismAssert (false, __FILE__, __LINE__);
                    }
                }

                // In any case if the buffer related information is not read successfully, bail out from here.
                // Some thing is wrong with the connection.

                if (false == isSuccessful)
                {
                    break;
                }
            }
        }
    }

    delete this;

    //pthread_exit (NULL);

    return (WAVE_THREAD_SUCCESS);
}

bool WaveClientReceiverThread::authorizeClient ()
{
    // Try to read the Prism connection Pass Phrase.  If the connected client does not supply the
    // proper pass phrase, then we know it is a rogue client.  We must drop the client.

    bool            isSuccessful              = false;
    string          prismPassPhrase           = FrameworkToolKit::getPrismConnectionPassPhrase ();
    FixedSizeBuffer passphraseFixedSizeBuffer (prismPassPhrase.size ());

    isSuccessful = (*m_pServerStreamingSocket) >> (passphraseFixedSizeBuffer);

    if ((false == isSuccessful) || (0 == (passphraseFixedSizeBuffer.getCurrentSize ())))
    {
        trace (TRACE_LEVEL_WARN, "WaveClientReceiverThread::start : A client could not supply proper credentials.  Ignoring client.");
    }
    else
    {
        string passPhraseGivenByClient;

        passphraseFixedSizeBuffer.toString (passPhraseGivenByClient);

        if (passPhraseGivenByClient != prismPassPhrase)
        {
            isSuccessful = false;

            trace (TRACE_LEVEL_WARN, "WaveClientReceiverThread::start : A client supplied improper credentials.  Ignoring client.");
        }
    }

    return (isSuccessful);
}

void WaveClientReceiverThread::setPeerServerDetails (const string &peerServerIpAddress, const SI32 &peerServerPort, const string& peerServerMessageVersion)
{
    m_peerServerMutex.lock ();

    m_peerServerIpAddress           = peerServerIpAddress;
    m_peerServerPort                = (SI32) peerServerPort;
    m_peerServerMessageVersion      = peerServerMessageVersion;

    if (peerServerMessageVersion.empty ())
    {
        m_peerServerSerializationType = SERIALIZE_WITH_UNKNOWN;
    }
    else
    {
        m_peerServerSerializationType   = ((0 == peerServerMessageVersion.compare ("0.0.0")) ? SERIALIZE_WITH_ATTRIBUTE_ORDER : SERIALIZE_WITH_ATTRIBUTE_NAME);
    }

    m_peerServerMutex.unlock ();
}

void WaveClientReceiverThread::closePeerServerStreamingSocket (void)
{
    m_peerServerMutex.lock ();

    if (NULL != m_pServerStreamingSocket)
    {
        m_pServerStreamingSocket->close ();
    }

    m_peerServerMutex.unlock ();
}

void WaveClientReceiverThread::setPeerServerId (const UI32 &peerServerId)
{
    m_peerServerId = peerServerId;
}

string WaveClientReceiverThread::getPeerServerMessageVersion () const
{
    return (m_peerServerMessageVersion);
}

UI8 WaveClientReceiverThread::getPeerServerSerializationType () const
{
    return (m_peerServerSerializationType);
}
}
