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

#include "discover/Framework/EventEngineManagementInterfaceMessages.h"
#include "discover/Framework/EventEngineTypes.h"
#include "discover/Framework/EventEngineObjectManager.h"
#include "discover/Framework/EventEngineLocalObjectManager.h"

namespace DcmNs {
    EventEngineMessage::EventEngineMessage() :
            ManagementInterfaceMessage(EventEngineObjectManager::getClassName(),
                    EVENTENGINE_START_STOP) {
        m_cmdcode = 0;
        ;
    }

    EventEngineMessage::EventEngineMessage(const UI32 & cmdcode) :
            ManagementInterfaceMessage(EventEngineObjectManager::getClassName(),
                    EVENTENGINE_START_STOP), m_cmdcode(cmdcode) {
    }

    EventEngineMessage::~EventEngineMessage() {
    }

    void EventEngineMessage::setupAttributesForSerialization() {
        ManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI32(&m_cmdcode, "cmdcode"));
        addSerializableAttribute(new AttributeString(&m_vcenter, "vcenter"));
        addSerializableAttribute(new AttributeString(&m_url, "url"));
        addSerializableAttribute(new AttributeString(&m_username, "username"));
        addSerializableAttribute(new AttributeString(&m_password, "password"));
    }

    void EventEngineMessage::setCmdcode(const UI32 & cmdcode) {
        m_cmdcode = cmdcode;
    }

    UI32 EventEngineMessage::getCmdcode() const {
        return (m_cmdcode);
    }
    string EventEngineMessage::getPassword() const {
        return m_password;
    }

    string EventEngineMessage::getUrl() const {
        return m_url;
    }

    string EventEngineMessage::getUsername() const {
        return m_username;
    }

    string EventEngineMessage::getVcenter() const {
        return m_vcenter;
    }

    void EventEngineMessage::setPassword(string password) {
        m_password = password;
    }

    void EventEngineMessage::setUrl(string url) {
        m_url = url;
    }

    void EventEngineMessage::setUsername(string username) {
        m_username = username;
    }

    void EventEngineMessage::setVcenter(string vcenter) {
        m_vcenter = vcenter;
    }

}
