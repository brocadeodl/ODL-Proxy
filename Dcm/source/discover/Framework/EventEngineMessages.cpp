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

#include "discover/Framework/EventEngineMessages.h"
#include "discover/Framework/EventEngineTypes.h"
#include "discover/Framework/EventEngineObjectManager.h"
#include "discover/Framework/EventEngineLocalObjectManager.h"

namespace DcmNs {

    EventEngineLocalMessage::EventEngineLocalMessage() :
        PrismMessage(
                    EventEngineLocalObjectManager::getPrismServiceId(),
                    EVENTENGINE_LOCAL_PUBLISH) {
        m_cmdcode = 0;
    }

    EventEngineLocalMessage::EventEngineLocalMessage(const UI32 & cmdcode) :
        PrismMessage(
                    EventEngineLocalObjectManager::getPrismServiceId(),
                    EVENTENGINE_LOCAL_PUBLISH), m_cmdcode(cmdcode) {
    }

    EventEngineLocalMessage::~EventEngineLocalMessage() {
    }

    void EventEngineLocalMessage::setupAttributesForSerialization() {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI32(&m_cmdcode, "cmdcode"));
    }

    void EventEngineLocalMessage::setCmdcode(const UI32 & cmdcode) {
        m_cmdcode = cmdcode;
    }

    UI32 EventEngineLocalMessage::getCmdcode() const {
        return (m_cmdcode);
    }

}
