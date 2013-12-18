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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerReceiverThread.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientInformation.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClientRepository.h"
#include "WaveResourceIdEnums.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessageFactory.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerSubscribeMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerUnsubscribeMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClient.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessagePayload.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorPublishMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

namespace WaveNs
{

WaveMessagingBrokerReceiverThread::WaveMessagingBrokerReceiverThread (ServerStreamingSocket *pServerStreamingSocket)
     : PrismPosixThread         (),
       m_pServerStreamingSocket (pServerStreamingSocket),
       m_peerServerPort         (0)
{
}

WaveMessagingBrokerReceiverThread::~WaveMessagingBrokerReceiverThread ()
{
    m_peerServerMutex.lock ();

    WaveMessagingBrokerClientRepository *pWaveMessagingBrokerClientRepository = WaveMessagingBrokerClientRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerClientRepository, __FILE__, __LINE__);

    pWaveMessagingBrokerClientRepository->removeWaveMessagingBrokerClient (m_peerServerIpAddress, m_peerServerPort);

    m_peerServerMutex.unlock ();
}

WaveThreadStatus WaveMessagingBrokerReceiverThread::start ()
{
    trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerReceiverThread::start : Starting ...");

    ResourceId connectionStatus                             = FRAMEWORK_ERROR;
    UI32       sizeOfConnectionInformation                  = 0;
    string     peerIpAddress                                = m_pServerStreamingSocket->getPeerIpAddress ();
    string     clientInformationString;
    string     peerServerIpAddress;
    SI32       peerServerPort                               = 0;
    string     peerName;
    string     peerNameWithoutExtension;
    bool       isSuccessful                                 = false;

    // This is ManagementInterface receive socket. Set custom Keep Alive timeout settings.

    isSuccessful = m_pServerStreamingSocket->setCustomKeepAliveTimeouts (50000,10000,10);

    // Need a trace message to indicate if 'Set custom Keep Alive timeout settings' failed.

    if (false == isSuccessful)
    {
        trace (TRACE_LEVEL_ERROR, string ("WaveMessagingBrokerReceiverThread::start : \"setCustomKeepAliveTimeouts ()\" failed. peerIpAddress [") + peerIpAddress + string ("]"));
    }

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
                trace (TRACE_LEVEL_FATAL, "WaveThreadStatus WaveMessagingBrokerReceiverThread::start : If we have read the size successfully, it cannot be zero.");
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

    FixedSizeBuffer waveMessagingBrokerClientClientInformationBuffer (sizeOfConnectionInformation);

    if (true == isSuccessful)
    {
        isSuccessful = (*m_pServerStreamingSocket) >> waveMessagingBrokerClientClientInformationBuffer;
    }

    if (true == isSuccessful)
    {
        if (0 == (waveMessagingBrokerClientClientInformationBuffer.getCurrentSize ()))
        {
            isSuccessful = false;
        }
    }

    if (true == isSuccessful)
    {
        WaveMessagingBrokerClientInformation clientInformation;

        waveMessagingBrokerClientClientInformationBuffer.toString (clientInformationString);
        clientInformation.loadFromSerializedData2 (clientInformationString, SERIALIZE_WITH_ATTRIBUTE_NAME);

        peerName                                    = clientInformation.getName      ();
        peerServerIpAddress                         = clientInformation.getIpAddress ();
        peerServerPort                              = clientInformation.getPort      ();

        //trace (TRACE_LEVEL_INFO, string ("Management Client Location Ip Address                      : \"") + peerServerIpAddress + "\"");
        //trace (TRACE_LEVEL_INFO, string ("                           Port                            : \"") + peerServerPort + "\"");
        //trace (TRACE_LEVEL_INFO, string ("                           Name                            : \"") + peerName + "\"");
        //trace (TRACE_LEVEL_INFO, string ("Management Client Location Connection Interface IP Address : \"") + peerIpAddress + "\"");

        tracePrintf (TRACE_LEVEL_INFO, true, false, "New Client : %s : %d : %s : %s", peerServerIpAddress.c_str (), peerServerPort, peerName.c_str (), peerIpAddress.c_str ());
    }

    if (0 != (strcmp (peerIpAddress.c_str (),  peerServerIpAddress.c_str ())))
    {
        // Reset the value in peer Ip Address.

        peerIpAddress = peerServerIpAddress;
    }

    WaveMessagingBrokerClientRepository *pWaveMessagingBrokerClientRepository = WaveMessagingBrokerClientRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerClientRepository, __FILE__, __LINE__);

    // At this point make sure that the client is coming with a unique name.  We cannot allow two clients to connect to us with the same name.

    if (false != (pWaveMessagingBrokerClientRepository->isAKnownWaveMessagingBrokerClient (peerServerIpAddress, peerServerPort)))
    {
        trace (TRACE_LEVEL_ERROR, string (" There is already a client connected to us with the name : \'") + peerServerIpAddress + string (":") + peerServerPort + string ("\'.  Dropping this client."));
        isSuccessful = false;
    }

    if (true == isSuccessful)
    {
        // The following must be assigned before we call addInterLocationMessageReceiverThreadToCache.

        setPeerServerDetails (peerServerIpAddress, (SI32) peerServerPort);

        // Make sure that there is no other thread handling the same location.  If there is another one,
        // wait till it exits.

        WaveMessagingBrokerReceiverThread *pWaveMessagingBrokerReceiverThread = this;
        pWaveMessagingBrokerReceiverThread->setPeerClientName (peerName);

        bool cacheStatus = pWaveMessagingBrokerClientRepository->addWaveMessagingBrokerClientIfNotKnown (peerName, peerServerIpAddress, peerServerPort);

        if (true == cacheStatus)
        {
            WaveMessagingBrokerClient *pWaveMessagingBrokerClient = pWaveMessagingBrokerClientRepository->checkoutWaveMessagingBrokerClient (peerServerIpAddress, peerServerPort);

            if (NULL != pWaveMessagingBrokerClient)
            {
                connectionStatus = pWaveMessagingBrokerClient->connect (30, 30);
            }

            if (FRAMEWORK_SUCCESS != connectionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerReceiverThread::start : We could not connect back to Client : " + peerName + peerServerIpAddress + string (" : ") + peerServerPort);
            }
        }

        while (FRAMEWORK_SUCCESS == connectionStatus)
        {
            WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload = new WaveBrokerBasedMessagePayload ();

            prismAssert (NULL != pWaveBrokerBasedMessagePayload, __FILE__, __LINE__);

            UI32 &messageSize = pWaveBrokerBasedMessagePayload->getMessageBodySize ();

            isSuccessful = (*m_pServerStreamingSocket) >> messageSize;

            // Message size of 0xFFFFFFFF indicates that the peer is shutting down the connection.

            if ((false == isSuccessful) || (0xFFFFFFFF == messageSize) || (0 == messageSize))
            {
                trace (TRACE_LEVEL_INFO, string ("WaveMessagingBrokerReceiverThread::start : peer is shutting down the connection: ") + messageSize);
                break;
            }

            FixedSizeBuffer *pNewFixedSizeBuffer = new FixedSizeBuffer (messageSize);

            //trace (TRACE_LEVEL_DEVEL, string ("WaveMessagingBrokerReceiverThread::start : received a message size indicator : ") + messageSize + ".");

            if (NULL == pNewFixedSizeBuffer)
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBrokerReceiverThread::start : Could not allocate a new FixedSizeBuffer of size ") + messageSize + ".");
                prismAssert (false, __FILE__, __LINE__);
            }

            isSuccessful = (*m_pServerStreamingSocket) >> (*pNewFixedSizeBuffer);

            //trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerReceiverThread::start : Completed reading the buffer.");

            if ((false == isSuccessful) || (0 == (pNewFixedSizeBuffer->getCurrentSize ())))
            {
                delete pNewFixedSizeBuffer;
                delete pWaveBrokerBasedMessagePayload;
                break;
            }
            else
            {
                // First create a PrismMessage from the contents of the FixedBuffer that we have read from the network.
                // Now depending on the contents of the message that arrived decide if it is a new request or a response to a request
                // this location sent out.

                // We can safely cast from SerializableObject pointer to PrismMessage pointer since we know that only object that
                // travels between two nodes in a Prism based cluster is a PrismMessage;
                // FIXME : sagar : enforce that the object type that was returned is indeed a PrismMessage or a specialization of
                //                 PrismMessage.

                char *&messageString = pWaveBrokerBasedMessagePayload->getMessageBody ();
                UI32   maximumSize   = 0;

                messageString = (char *) pNewFixedSizeBuffer->transferBufferToUser(messageSize, maximumSize);

                // We do not need the Fixed Size Buffer anymore.  So delete it.

                delete pNewFixedSizeBuffer;

                //trace (TRACE_LEVEL_DEVEL, messageString);

                //prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

                // Now read the buffer data

                UI32           numberOfBuffers = 0;
                UI32           i               = 0;
                vector<UI32>  &bufferTags      = pWaveBrokerBasedMessagePayload->getBufferTags  ();
                vector<UI32>  &bufferSizes     = pWaveBrokerBasedMessagePayload->getBufferSizes ();
                vector<UI8 *> &buffers         = pWaveBrokerBasedMessagePayload->getBuffers     ();

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
                            delete pWaveBrokerBasedMessagePayload;
                            break;
                        }

                        isSuccessful = (*m_pServerStreamingSocket) >> bufferSize;

                        if ((false == isSuccessful) || (0 == bufferSize))
                        {
                            delete pWaveBrokerBasedMessagePayload;
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
                            delete pWaveBrokerBasedMessagePayload;
                            break;
                        }

                        bufferTags.push_back  (bufferTag);
                        bufferSizes.push_back (bufferSize);
                        buffers.push_back     (reinterpret_cast<UI8 *> (pBuffer));

                    }
                }

                // In any case if the buffer related information is not read successfully, bail out from here.
                // Some thing is wrong with the connection.

                if (false == isSuccessful)
                {
                    trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerReceiverThread::start : isSuccessful = false");
                    delete pWaveBrokerBasedMessagePayload;
                    break;
                }
                else
                {
                    processWaveBrokerBasedMessage (pWaveBrokerBasedMessagePayload);
                }
            }
        }
    }

    trace (TRACE_LEVEL_INFO, string ("Client: ") + peerName + " ReceiverThread exit");

    delete this;

    pthread_exit (NULL);

    return (WAVE_THREAD_SUCCESS);
}

bool WaveMessagingBrokerReceiverThread::authorizeClient ()
{
    // Try to read the Prism connection Pass Phrase.  If the connected client does not supply the
    // proper pass phrase, then we know it is a rogue client.  We must drop the client.

    bool            isSuccessful              = false;
    string          prismPassPhrase           = FrameworkToolKit::getPrismConnectionPassPhraseForMessageBroker ();
    FixedSizeBuffer passphraseFixedSizeBuffer (prismPassPhrase.size ());

    isSuccessful = (*m_pServerStreamingSocket) >> (passphraseFixedSizeBuffer);

    if ((false == isSuccessful) || (0 == (passphraseFixedSizeBuffer.getCurrentSize ())))
    {
        trace (TRACE_LEVEL_WARN, "WaveMessagingBrokerReceiverThread::start : A client could not supply proper credentials.  Ignoring client.");
    }
    else
    {
        string passPhraseGivenByClient;

        passphraseFixedSizeBuffer.toString (passPhraseGivenByClient);

        if (passPhraseGivenByClient != prismPassPhrase)
        {
            isSuccessful = false;

            trace (TRACE_LEVEL_WARN, "WaveMessagingBrokerReceiverThread::start : A client supplied improper credentials.  Ignoring client.");
        }
    }

    return (isSuccessful);
}

void WaveMessagingBrokerReceiverThread::setPeerServerDetails (const string &peerServerIpAddress, const SI32 &peerServerPort)
{
    m_peerServerMutex.lock ();

    m_peerServerIpAddress           = peerServerIpAddress;
    m_peerServerPort                = (SI32) peerServerPort;

    m_peerServerUniqueString = m_peerServerIpAddress + string (":") + m_peerServerPort;

    m_peerServerMutex.unlock ();
}

void WaveMessagingBrokerReceiverThread::setPeerClientName (const string &peerClientName)
{
    m_peerClientName = peerClientName;
}

string WaveMessagingBrokerReceiverThread::getPeerClientName () const
{
    return (m_peerClientName);
}

void WaveMessagingBrokerReceiverThread::processWaveBrokerBasedMessage (WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload)
{
    if (NULL == pWaveBrokerBasedMessagePayload)
    {
        return;
    }

    char                        *&messageString                 = pWaveBrokerBasedMessagePayload->getMessageBody ();
    WaveBrokerBasedMessageType    waveBrokerBasedMessageType    = WaveBrokerBasedMessage::getWaveBrokerBasedMessageType (messageString);
    string                        tempString;

    WaveBrokerBasedMessage       *pWaveBrokerBasedMessage       = NULL;
    WaveBrokerSubscribeMessage   *pWaveBrokerSubscribeMessage   = NULL;
    WaveBrokerUnsubscribeMessage *pWaveBrokerUnsubscribeMessage = NULL;

    switch (waveBrokerBasedMessageType)
    {
        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE :
            pWaveBrokerBasedMessage = WaveBrokerBasedMessageFactory::createWaveBrokerBasedMessageInstance (waveBrokerBasedMessageType, tempString);

            prismAssert (NULL != pWaveBrokerBasedMessage, __FILE__, __LINE__);

            pWaveBrokerSubscribeMessage = dynamic_cast<WaveBrokerSubscribeMessage *> (pWaveBrokerBasedMessage);

            prismAssert (NULL != pWaveBrokerSubscribeMessage, __FILE__, __LINE__);

            pWaveBrokerSubscribeMessage->loadFromSerializedData2 (pWaveBrokerBasedMessagePayload->getMessageBody ());

            processWaveBrokerBasedMessage (pWaveBrokerSubscribeMessage);

            break;

        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNSUBSCRIBE :
            pWaveBrokerBasedMessage = WaveBrokerBasedMessageFactory::createWaveBrokerBasedMessageInstance (waveBrokerBasedMessageType, tempString);

            prismAssert (NULL != pWaveBrokerBasedMessage, __FILE__, __LINE__);

            pWaveBrokerUnsubscribeMessage = dynamic_cast<WaveBrokerUnsubscribeMessage *> (pWaveBrokerBasedMessage);

            prismAssert (NULL != pWaveBrokerUnsubscribeMessage, __FILE__, __LINE__);

            processWaveBrokerBasedMessage (pWaveBrokerUnsubscribeMessage);

            break;

        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH :
            processWaveBrokerPublishMessage (pWaveBrokerBasedMessagePayload);
            break;

        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNKNOWN :
            break;
    }
}

void WaveMessagingBrokerReceiverThread::processWaveBrokerBasedMessage (WaveBrokerSubscribeMessage *pWaveBrokerSubscribeMessage)
{
    trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerReceiverThread::processWaveBrokerBasedMessage : Processing Subscribe message ...");

    WaveMessagingBrokerClientRepository *pWaveMessagingBrokerClientRepository = WaveMessagingBrokerClientRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerClientRepository, __FILE__, __LINE__);

    vector<string> topicNames         = pWaveBrokerSubscribeMessage->getTopicNames ();
    UI32           numberOfTopicNames = topicNames.size ();
    UI32           i                  = 0;

    trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerReceiverThread::processWaveBrokerBasedMessage : Subscribed Topics listed below :");

    for (i = 0; i < numberOfTopicNames; i++)
    {
        trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerReceiverThread::processWaveBrokerBasedMessage :     " + topicNames[i]);

        pWaveMessagingBrokerClientRepository->addWaveMessagingBrokerClientSubscription (m_peerServerIpAddress, m_peerServerPort, topicNames[i]);
    }
}

void WaveMessagingBrokerReceiverThread::processWaveBrokerBasedMessage (WaveBrokerUnsubscribeMessage *pWaveBrokerUnsubscribeMessage)
{
}

void WaveMessagingBrokerReceiverThread::processWaveBrokerPublishMessage   (WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload)
{
    prismAssert (NULL != pWaveBrokerBasedMessagePayload, __FILE__, __LINE__);

    string topicName = WaveBrokerBasedMessage::getTopicName (pWaveBrokerBasedMessagePayload->getMessageBody ());

    WaveMessagingBrokerTopicProcessorPublishMessage *pWaveMessagingBrokerTopicProcessorPublishMessage = new WaveMessagingBrokerTopicProcessorPublishMessage (WaveMessagingBrokerTopicProcessorObjectManager::getPrismServiceIdByTopicName (topicName));

    prismAssert (NULL != pWaveBrokerBasedMessagePayload, __FILE__, __LINE__);

    pWaveMessagingBrokerTopicProcessorPublishMessage->setPWaveBrokerBasedMessagePayload (pWaveBrokerBasedMessagePayload);

    (pWaveMessagingBrokerTopicProcessorPublishMessage->getUniqueStringForPublisher ()) = m_peerServerUniqueString;

    ResourceId status = WaveObjectManagerToolKit::sendOneWay (pWaveMessagingBrokerTopicProcessorPublishMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        delete pWaveMessagingBrokerTopicProcessorPublishMessage;
    }
}

}
