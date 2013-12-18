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

#include "Framework/ObjectModel/WaveDeliverBrokerPublishMessageWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Messaging/MessagingBus/BrokerBasedMessagingBus/WaveBrokerPublishMessage.h"
#include "Framework/Core/PrismFrameworkMessages.h"

namespace WaveNs
{

WaveDeliverBrokerPublishMessageWorker::WaveDeliverBrokerPublishMessageWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (WAVE_OBJECT_MANAGER_DELIVER_WAVE_BROKER_PUBLISH_MESSAGE, reinterpret_cast<PrismMessageHandler> (&WaveDeliverBrokerPublishMessageWorker::deliverBrokerPublishMessageHandler));
}

WaveDeliverBrokerPublishMessageWorker::~WaveDeliverBrokerPublishMessageWorker ()
{
}

void WaveDeliverBrokerPublishMessageWorker::deliverBrokerPublishMessageHandler (WaveDeliverBrokerPublishMessage *pWaveDeliverBrokerPublishMessage)
{
    //trace (TRACE_LEVEL_DEVEL, "WaveDeliverBrokerPublishMessageWorker::deliverBrokerPublishMessageHandler: Entering ...");

    prismAssert (NULL != pWaveDeliverBrokerPublishMessage, __FILE__, __LINE__);

    WaveBrokerPublishMessage *pWaveBrokerPublishMessage = pWaveDeliverBrokerPublishMessage->getPWaveBrokerPublishMessage ();

    reply (pWaveDeliverBrokerPublishMessage);

    prismAssert (NULL != pWaveBrokerPublishMessage, __FILE__, __LINE__);

    pWaveBrokerPublishMessage->loadFromSerializedData2 ();

    string brokerName = pWaveBrokerPublishMessage->getBrokerName ();
    string topicName  = pWaveBrokerPublishMessage->getTopicName  ();

    deliverWaveBrokerPublishedEvent (brokerName, topicName, pWaveBrokerPublishMessage);
}

}
