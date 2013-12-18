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

#ifndef EVENTENGINEMANAGEMENTINTERFACEMESSAGES_H
#define EVENTENGINEMANAGEMENTINTERFACEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

//Contains Message Definitions for both Global and Local Plugin
namespace DcmNs {

    typedef enum EventEngineMessageCmdCode_ {
        EVENT_ENGINE_START=0,
        EVENT_ENGINE_STOP

    } EventEngineMessageCmdCode;


    // ************* EventEngineMessage ************
    class EventEngineMessage: public ManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            EventEngineMessage();
            EventEngineMessage(const UI32 &cmdcode);
            virtual ~EventEngineMessage();
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode() const;
        string getPassword() const;
        string getUrl() const;
        string getUsername() const;
        string getVcenter() const;
        void setPassword(string password);
        void setUrl(string url);
        void setUsername(string username);
        void setVcenter(string vcenter);
        private:
        protected:
        public:
            UI32 m_cmdcode;
            string m_vcenter;
            string m_url;
            string m_username;
            string m_password;

    };
}
#endif  // EVENTENGINEMANAGEMENTINTERFACEMESSAGES_H
