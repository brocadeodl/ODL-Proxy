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

#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorPublishWorker.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorObjectManagerTypes.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicProcessorPublishMessage.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerTopicRepository.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClientRepository.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveMessagingBroker/WaveMessagingBrokerClient.h"

namespace WaveNs
{

WaveMessagingBrokerTopicProcessorPublishWorker::WaveMessagingBrokerTopicProcessorPublishWorker (WaveMessagingBrokerTopicProcessorObjectManager *pWaveMessagingBrokerTopicProcessorObjectManager)
    : WaveWorker                                        (pWaveMessagingBrokerTopicProcessorObjectManager),
      m_pWaveMessagingBrokerTopicProcessorObjectManager (pWaveMessagingBrokerTopicProcessorObjectManager)
{
    prismAssert (NULL != m_pWaveMessagingBrokerTopicProcessorObjectManager, __FILE__, __LINE__);

    addOperationMap (WAVE_MESSAGING_BROKER_TOPIC_PROCESSOR_PUBLISH, reinterpret_cast<PrismMessageHandler> (&WaveMessagingBrokerTopicProcessorPublishWorker::publishMessageHandler));
}

WaveMessagingBrokerTopicProcessorPublishWorker::~WaveMessagingBrokerTopicProcessorPublishWorker ()
{
}

void WaveMessagingBrokerTopicProcessorPublishWorker::publishMessageHandler (WaveMessagingBrokerTopicProcessorPublishMessage *pWaveMessagingBrokerTopicProcessorPublishMessage)
{
    //trace (TRACE_LEVEL_DEVEL, " WaveMessagingBrokerTopicProcessorPublishWorker::publishMessageHandler : Entering ...");

    prismAssert (NULL != pWaveMessagingBrokerTopicProcessorPublishMessage, __FILE__, __LINE__);

    WaveBrokerBasedMessagePayload *pWaveBrokerBasedMessagePayload = pWaveMessagingBrokerTopicProcessorPublishMessage->getPWaveBrokerBasedMessagePayload ();

    prismAssert (NULL != pWaveBrokerBasedMessagePayload, __FILE__, __LINE__);

    string         topicName                   = m_pWaveMessagingBrokerTopicProcessorObjectManager->getTopicName ();
    vector<string> currentlySubscribedClients;

    WaveMessagingBrokerTopicRepository *pWaveMessagingBrokerTopicRepository = WaveMessagingBrokerTopicRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerTopicRepository, __FILE__, __LINE__);

    pWaveMessagingBrokerTopicRepository->getCurrentlySubscribedClientsForTopic (topicName, currentlySubscribedClients);

    UI32 numberOfCurrentlySubscribedClients = currentlySubscribedClients.size ();
    UI32 i                                  = 0;

    WaveMessagingBrokerClientRepository *pWaveMessagingBrokerClientRepository = WaveMessagingBrokerClientRepository::getInstance ();

    prismAssert (NULL != pWaveMessagingBrokerClientRepository, __FILE__, __LINE__);

    string &publisher = pWaveMessagingBrokerTopicProcessorPublishMessage->getUniqueStringForPublisher ();

    for (i = 0; i < numberOfCurrentlySubscribedClients; i++)
    {
        const string &clientuniqueString = currentlySubscribedClients[i];

        //trace (TRACE_LEVEL_DEVEL, "WaveMessagingBrokerTopicProcessorPublishWorker::publishMessageHandler : Publishing Message to : " + clientuniqueString);

        if (publisher == clientuniqueString)
        {
            continue;
        }

        WaveMessagingBrokerClient *pWaveMessagingBrokerClient = pWaveMessagingBrokerClientRepository->checkoutWaveMessagingBrokerClient (clientuniqueString);

        if (NULL != pWaveMessagingBrokerClient)
        {
            (*pWaveMessagingBrokerClient) << pWaveBrokerBasedMessagePayload;

            pWaveMessagingBrokerClientRepository->checkinWaveMessagingBrokerClient (pWaveMessagingBrokerClient);
        }
    }

    pWaveMessagingBrokerTopicProcessorPublishMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pWaveMessagingBrokerTopicProcessorPublishMessage);
}

string WaveMessagingBrokerTopicProcessorPublishWorker::getTopicName () const
{
    return (m_pWaveMessagingBrokerTopicProcessorObjectManager->getTopicName ());
}

}
