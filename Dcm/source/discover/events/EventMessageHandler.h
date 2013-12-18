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


#ifndef EVENTMESSAGEHANDLER_H_
#define EVENTMESSAGEHANDLER_H_
#include <string>
#include <iostream>
#include "vCenterEventsMessage.h"
#include "EventDefines.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/Framework/discoverManagementInterfaceMessages.h"

using namespace std;
namespace DcmNs {

    class EventMessageHandler {

        public:
            EventMessageHandler();
            virtual ~EventMessageHandler();
            static void handleDataCenterEvent(LocalDatacenterEventMessage *pMessage);
            static void handleVswitchEvent(LocalVswitchEventMessage *pMessage);
            static void handleVmnicEvent(LocalVmnicEventMessage *pMessage);
            static void handlePgHostVsEvent(LocalPgHostVsEventMessage *pMessage);
            static void handleVMEvent(LocalVMEventMessage *message);
            static void handleDVSEvent(LocalDVSEventMessage *message);
            static void handleDVPGEvent(LocalDVPGEventMessage *message);
            static void handleHostEvent(LocalHostEventMessage *message);
            static void handleVnicEvent(LocalVNICEventMessage *message);
            static void handleSpgEvent(LocalSPGEventMessage *message);
            static void handleSnicEvent(LocalSNICEventMessage *message);
            static void handleDPEvent(LocalVNMDPMessage *message);
    		static bool isPublishValid();
            static bool isUpLink(string uplink);

    };

} /* namespace DcmNs */
#endif /* EVENTMESSAGEHANDLER_H_ */
