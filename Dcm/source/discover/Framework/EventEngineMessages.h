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

#ifndef EVENTENGINEMESSAGES_H
#define EVENTENGINEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

//Contains Message Definitions for both Global and Local Plugin
namespace DcmNs {

    // ************* EventEngineLocalMessage ************
    /**
     * Message sent from Local plugin to B.E
     * Events are published using this Message's buffer
     */
#define EVENT_ENGINE_LOCAL_PUBLISH_CMD_CODE 101
    class EventEngineLocalMessage: public PrismMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            EventEngineLocalMessage();
            EventEngineLocalMessage(const UI32 &cmdcode);
            virtual ~EventEngineLocalMessage();
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode() const;

            // Now the data members
        private:
        protected:
        public:
            UI32 m_cmdcode;

    };

}
#endif  // EVENTENGINEMESSAGES_H
