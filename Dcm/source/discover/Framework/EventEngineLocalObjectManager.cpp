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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#include "discover/Framework/EventEngineLocalObjectManager.h"
#include "discover/Framework/EventEngineTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/vCenter/vCenterEventMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/vCenter/vCenterClientParseUtility.h"

namespace DcmNs {

    EventEngineLocalObjectManager::EventEngineLocalObjectManager() :
            WaveLocalObjectManager(getClassName()) {

        //Message handler for publishing Events to B.E (Recieves Events from Global
        //plugin and sends to B.E)
        addOperationMap(
                EVENTENGINE_LOCAL_PUBLISH,
                reinterpret_cast<PrismMessageHandler>(&EventEngineLocalObjectManager::eventEnginePublishHandler));
        //  WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient ("EVENTENGINE_LOCAL", &RASLocalObjectManager::createManagementInterfaceMessageInstance);

    }

    EventEngineLocalObjectManager::~EventEngineLocalObjectManager() {
    }

    EventEngineLocalObjectManager *EventEngineLocalObjectManager::getInstance() {
        static EventEngineLocalObjectManager *eventEngineLocalObjectManager =
                new EventEngineLocalObjectManager();

        WaveNs::prismAssert(NULL != eventEngineLocalObjectManager, __FILE__,
                __LINE__);

        return (eventEngineLocalObjectManager);
    }

    string EventEngineLocalObjectManager::getClassName() {
        return ("VCenterEventEngineLocal");
    }

    PrismServiceId EventEngineLocalObjectManager::getPrismServiceId() {
        return ((getInstance())->getServiceId());
    }

    PrismMessage *EventEngineLocalObjectManager::createMessageInstance(
            const UI32 &operationCode) {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode) {
            //Message used by other plugins (Global Event Engine plugin)
            //to coummunicate withe this plugin.
            case EVENTENGINE_LOCAL_PUBLISH:
                pPrismMessage = new EventEngineLocalMessage();
                break;
            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject *EventEngineLocalObjectManager::createManagedObjectInstance(
            const string &managedClassName) {
        return NULL;
    }

    void EventEngineLocalObjectManager::eventEnginePublishHandler(
            EventEngineLocalMessage *eventEngineLocalMessage) {
        trace(
                TRACE_LEVEL_DEBUG,
                "EventLocalEngine ::Recieved Event from vCenter Event Engine Global");

        ResourceId status = WAVE_MESSAGE_ERROR;

        UI32 size;
        //Extract the buffer holding the message sent by Global Plugin
        void *buffer = eventEngineLocalMessage->findBuffer(NOTIFICATION_PLD_TAG,
                size);
        if (buffer != NULL) {
            vCenterEventMessage *pMsg = new vCenterEventMessage();

            //buffer would get cleared by Framework;
            //so allocate buffer for sending to B.E
            void *copy_of_buffer = (void *) malloc(size);
            memcpy(copy_of_buffer, buffer, size);

            pMsg->addBuffer(NOTIFICATION_PLD_TAG, size, copy_of_buffer);

            //Send the Message to B.E
            status = sendSynchronouslyToWaveClient("vCenterDcmClient", pMsg);

            delete (pMsg);

            if (status == WAVE_MESSAGE_SUCCESS) {
                trace(
                        TRACE_LEVEL_DEBUG,
                        "EventLocalEngine: Succeeded to send message to B.E client");
            }
            else {
                trace(
                        TRACE_LEVEL_INFO,
                        "EventLocalEngine: Failed to send message to B.E client");
            }
        }
        else {
            trace(TRACE_LEVEL_DEBUG,
                    "EventLocalEngine ::No buffer in the received Message");
        }

        //Acknowledge the Global Plugin
        eventEngineLocalMessage->setCompletionStatus(status);
        eventEngineLocalMessage->setCmdcode(
                EVENT_ENGINE_LOCAL_PUBLISH_CMD_CODE);
        reply(eventEngineLocalMessage);

    }

}
