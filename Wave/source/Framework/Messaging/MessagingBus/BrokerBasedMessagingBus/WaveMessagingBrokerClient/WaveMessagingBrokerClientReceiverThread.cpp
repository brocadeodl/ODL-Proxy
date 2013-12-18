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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientReceiverThread.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerClientInformation.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClientRepository.h"
#include "WaveResourceIdEnums.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerInformation.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBrokerRepository.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerBasedMessageFactory.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBrokerClient/WaveMessagingBroker.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

namespace WaveNs
{

WaveMessagingBrokerClientReceiverThread::WaveMessagingBrokerClientReceiverThread (ServerStreamingSocket *pServerStreamingSocket)
     : PrismPosixThread         (),
       m_pServerStreamingSocket (pServerStreamingSocket),
       m_peerServerPort         (0)
{
}

WaveMessagingBrokerClientReceiverThread::~WaveMessagingBrokerClientReceiverThread ()
{
}

WaveThreadStatus WaveMessagingBrokerClientReceiverThread::start ()
{
    trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerClientReceiverThread::start : Starting ...");

    ResourceId connectionStatus                             = FRAMEWORK_SUCCESS;
    UI32       sizeOfConnectionInformation                  = 0;
    string     peerIpAddress                                = m_pServerStreamingSocket->getPeerIpAddress ();
    string     brokerInformationString;
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
        trace (TRACE_LEVEL_ERROR, string ("WaveMessagingBrokerClientReceiverThread::start : \"setCustomKeepAliveTimeouts ()\" failed. peerIpAddress [") + peerIpAddress + string ("]"));
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
                trace (TRACE_LEVEL_FATAL, "WaveThreadStatus WaveMessagingBrokerClientReceiverThread::start : If we have read the size successfully, it cannot be zero.");
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

    FixedSizeBuffer waveMessagingBrokerInformationBuffer (sizeOfConnectionInformation);

    if (true == isSuccessful)
    {
        isSuccessful = (*m_pServerStreamingSocket) >> waveMessagingBrokerInformationBuffer;
    }

    if (true == isSuccessful)
    {
        if (0 == (waveMessagingBrokerInformationBuffer.getCurrentSize ()))
        {
            isSuccessful = false;
        }
    }

    if (true == isSuccessful)
    {
        WaveMessagingBrokerInformation brokerInformation;

        waveMessagingBrokerInformationBuffer.toString (brokerInformationString);
        brokerInformation.loadFromSerializedData2 (brokerInformationString, SERIALIZE_WITH_ATTRIBUTE_NAME);

        peerName                                    = brokerInformation.getName      ();
        peerServerIpAddress                         = brokerInformation.getIpAddress ();
        peerServerPort                              = brokerInformation.getPort      ();

        trace (TRACE_LEVEL_INFO, string ("Management Client Location Ip Address                      : \"") + peerServerIpAddress + "\"");
        trace (TRACE_LEVEL_INFO, string ("                           Port                            : \"") + peerServerPort + "\"");
        trace (TRACE_LEVEL_INFO, string ("                           Name                            : \"") + peerName + "\"");
        trace (TRACE_LEVEL_INFO, string ("Management Client Location Connection Interface IP Address : \"") + peerIpAddress + "\"");
    }

    if (0 != (strcmp (peerIpAddress.c_str (),  peerServerIpAddress.c_str ())))
    {
        // Reset the value in peer Ip Address.

        peerIpAddress = peerServerIpAddress;
    }

    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

    // At this point make sure that the client is coming with a unique name.  We cannot allow two clients to connect to us with the same name.

    if (true != (pWaveMessagingBrokerRepository->isAKnownBroker (peerServerIpAddress, peerServerPort)))
    {
        trace (TRACE_LEVEL_ERROR, string ("This Broker is not known to us : \'") + peerServerIpAddress + string (":") + peerServerPort + string ("\'.  Dropping this Broker."));
        isSuccessful = false;
    }

    if (true == isSuccessful)
    {
        // The following must be assigned before we call addInterLocationMessageReceiverThreadToCache.

        setPeerServerDetails (peerServerIpAddress, (SI32) peerServerPort);

        // Cache the Broker name

        m_brokerNameFromClientPerspective = pWaveMessagingBrokerRepository->getBrokerName (peerServerIpAddress, peerServerPort);

        WaveMessagingBrokerClientReceiverThread *pWaveMessagingBrokerClientReceiverThread = this;

        pWaveMessagingBrokerClientReceiverThread->setPeerClientName (peerName);

        UI32 messageSize = 0;

        while (FRAMEWORK_SUCCESS == connectionStatus)
        {
            isSuccessful = (*m_pServerStreamingSocket) >> messageSize;

            // Message size of 0xFFFFFFFF indicates that the peer is shutting down the connection.

            if ((false == isSuccessful) || (0xFFFFFFFF == messageSize) || (0 == messageSize))
            {
                trace (TRACE_LEVEL_INFO, string ("WaveMessagingBrokerClientReceiverThread::start : peer is shutting down the connection: ") + messageSize);
                break;
            }

            FixedSizeBuffer *pNewFixedSizeBuffer = new FixedSizeBuffer (messageSize);

            //trace (TRACE_LEVEL_DEVEL, string ("WaveMessagingBrokerClientReceiverThread::start : received a message size indicator : ") + messageSize + ".");

            if (NULL == pNewFixedSizeBuffer)
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveMessagingBrokerClientReceiverThread::start : Could not allocate a new FixedSizeBuffer of size ") + messageSize + ".");
                prismAssert (false, __FILE__, __LINE__);
            }

            isSuccessful = (*m_pServerStreamingSocket) >> (*pNewFixedSizeBuffer);

            //trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerClientReceiverThread::start : Completed reading the buffer.");

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
                // travels between two nodes in a Prism based cluster is a PrismMessage;
                // FIXME : sagar : enforce that the object type that was returned is indeed a PrismMessage or a specialization of
                //                 PrismMessage.

                UI8  *pMessageBuffer = NULL;
                UI32  currentSize    = 0;
                UI32  maximumSize    = 0;

                pMessageBuffer = pNewFixedSizeBuffer->transferBufferToUser (currentSize, maximumSize);

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

                WaveBrokerBasedMessageType waveBrokerBasedMessageType = WaveBrokerBasedMessage::getWaveBrokerBasedMessageType ((const char *) pMessageBuffer);
                string                     topicName;

                if (WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH == waveBrokerBasedMessageType)
                {
                    topicName = WaveBrokerBasedMessage::getTopicName ((const char *) pMessageBuffer);
                }

                WaveBrokerBasedMessage *pWaveBrokerBasedMessage = WaveBrokerBasedMessageFactory::createWaveBrokerBasedMessageInstance (waveBrokerBasedMessageType, topicName);

                //pWaveBrokerBasedMessage->loadFromSerializedData2 (messageString);

                // If we have a message at hand then attach the buffer to the message.
                // Otherwise destroy the buffer.

                numberOfBuffers = buffers.size ();

                if (NULL != pWaveBrokerBasedMessage)
                {
                    for (i = 0; i < numberOfBuffers; i++)
                    {
                        pWaveBrokerBasedMessage->addBuffer (bufferTags[i], bufferSizes[i], buffers[i], true);
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

                if (NULL != pWaveBrokerBasedMessage)
                {
                    processWaveBrokerBasedMessage (pWaveBrokerBasedMessage, (const char *) pMessageBuffer);
                }

                // In any case if the buffer related information is not read successfully, bail out from here.
                // Some thing is wrong with the connection.

                if (false == isSuccessful)
                {
                    trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerClientReceiverThread::start : isSuccessful = false");
                    break;
                }
            }
        }
    }

    trace (TRACE_LEVEL_INFO, string ("WaveMessagingBrokerClientReceiverThread : start : ") + peerName + " Retrying to Connect ...");

    reconnectToPeerServer ();

    trace (TRACE_LEVEL_INFO, string ("WaveMessagingBrokerClientReceiverThread : start : ") + peerName + " ReceiverThread exit");

    delete this;

    pthread_exit (NULL);

    return (WAVE_THREAD_SUCCESS);
}

bool WaveMessagingBrokerClientReceiverThread::authorizeClient ()
{
    // Try to read the Prism connection Pass Phrase.  If the connected client does not supply the
    // proper pass phrase, then we know it is a rogue client.  We must drop the client.

    bool            isSuccessful              = false;
    string          prismPassPhrase           = FrameworkToolKit::getPrismConnectionPassPhraseForMessageBroker ();
    FixedSizeBuffer passphraseFixedSizeBuffer (prismPassPhrase.size ());

    isSuccessful = (*m_pServerStreamingSocket) >> (passphraseFixedSizeBuffer);

    if ((false == isSuccessful) || (0 == (passphraseFixedSizeBuffer.getCurrentSize ())))
    {
        trace (TRACE_LEVEL_WARN, "WaveMessagingBrokerClientReceiverThread::start : A client could not supply proper credentials.  Ignoring client.");
    }
    else
    {
        string passPhraseGivenByClient;

        passphraseFixedSizeBuffer.toString (passPhraseGivenByClient);

        if (passPhraseGivenByClient != prismPassPhrase)
        {
            isSuccessful = false;

            trace (TRACE_LEVEL_WARN, "WaveMessagingBrokerClientReceiverThread::start : A client supplied improper credentials.  Ignoring client.");
        }
    }

    return (isSuccessful);
}

void WaveMessagingBrokerClientReceiverThread::setPeerServerDetails (const string &peerServerIpAddress, const SI32 &peerServerPort)
{
    m_peerServerMutex.lock ();

    m_peerServerIpAddress           = peerServerIpAddress;
    m_peerServerPort                = (SI32) peerServerPort;

    m_peerServerMutex.unlock ();
}

void WaveMessagingBrokerClientReceiverThread::setPeerClientName (const string &peerClientName)
{
    m_peerClientName = peerClientName;
}

string WaveMessagingBrokerClientReceiverThread::getPeerClientName () const
{
    return (m_peerClientName);
}

void WaveMessagingBrokerClientReceiverThread::processWaveBrokerBasedMessage (WaveBrokerBasedMessage *pWaveBrokerBasedMessage, const char *pMessageBuffer)
{
    if (NULL == pWaveBrokerBasedMessage)
    {
        return;
    }

    WaveBrokerBasedMessageType  waveBrokerBasedMessageType = pWaveBrokerBasedMessage->getWaveBrokerBasedMessageType ();
    WaveBrokerPublishMessage   *pWaveBrokerPublishMessage  = NULL;

    switch (waveBrokerBasedMessageType)
    {
        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_PUBLISH :
            pWaveBrokerPublishMessage = dynamic_cast<WaveBrokerPublishMessage *> (pWaveBrokerBasedMessage);

            prismAssert (NULL != pWaveBrokerPublishMessage, __FILE__, __LINE__);

            pWaveBrokerPublishMessage->setPMessageBuffer (pMessageBuffer);

            processWaveBrokerBasedMessage (pWaveBrokerPublishMessage);

            break;

        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_SUBSCRIBE :
        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNSUBSCRIBE :

        case WAVE_BROKER_BASED_MESSAGING_BUS_MESSAGE_TYPE_UNKNOWN :
            break;
    }
}

void WaveMessagingBrokerClientReceiverThread::reconnectToPeerServer ()
{
    m_peerServerMutex.lock ();

    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

    WaveMessagingBroker *pWaveMessagingBroker = pWaveMessagingBrokerRepository->checkoutBroker (m_peerServerIpAddress, m_peerServerPort);

    if (NULL != pWaveMessagingBroker)
    {
        pWaveMessagingBroker->connect (1, 10);
    }

    m_peerServerMutex.unlock ();
}

void WaveMessagingBrokerClientReceiverThread::processWaveBrokerBasedMessage (WaveBrokerPublishMessage *pWaveBrokerPublishMessage)
{
    prismAssert (NULL != pWaveBrokerPublishMessage, __FILE__, __LINE__);

    WaveMessagingBrokerRepository *pWaveMessagingBrokerRepository = WaveMessagingBrokerRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerRepository, __FILE__, __LINE__);

    string               topicName            = pWaveBrokerPublishMessage->getTopicName ();

    WaveMessagingBroker *pWaveMessagingBroker                = pWaveMessagingBrokerRepository->checkoutBroker (m_brokerNameFromClientPerspective);
    UI32                 numberOfCurrentlySubscribedServices = 0;

    if (NULL != pWaveMessagingBroker)
    {
        vector<PrismServiceId> currentlySubscribedServices;

        pWaveMessagingBroker->getCurrentlySubscribedServicesForTopicName (topicName, currentlySubscribedServices);

        pWaveMessagingBrokerRepository->checkinBroker (pWaveMessagingBroker);

        numberOfCurrentlySubscribedServices = currentlySubscribedServices.size ();

        pWaveBrokerPublishMessage->setBrokerName     (m_brokerNameFromClientPerspective);
        pWaveBrokerPublishMessage->setReferenceCount (numberOfCurrentlySubscribedServices);

        UI32 i= 0;

        for (i = 0; i < numberOfCurrentlySubscribedServices; i++)
        {
            WaveDeliverBrokerPublishMessage *pWaveDeliverBrokerPublishMessage = new WaveDeliverBrokerPublishMessage (currentlySubscribedServices[i]);

            prismAssert (NULL != pWaveDeliverBrokerPublishMessage, __FILE__, __LINE__);

            pWaveDeliverBrokerPublishMessage->setPWaveBrokerPublishMessage (pWaveBrokerPublishMessage);

            ResourceId status = WaveObjectManagerToolKit::sendOneWay (pWaveDeliverBrokerPublishMessage);

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_ERROR, "WaveMessagingBrokerClientReceiverThread::processWaveBrokerBasedMessage : Failed to deliver a broker published message.  BrokerName : " + m_brokerNameFromClientPerspective + " : Topic Name : " + topicName);

                pWaveBrokerPublishMessage->garbageCollect ();
            }
        }
    }

    if (0 == numberOfCurrentlySubscribedServices)
    {
        pWaveBrokerPublishMessage->setReferenceCount (1);
        pWaveBrokerPublishMessage->garbageCollect ();
    }
}

}
