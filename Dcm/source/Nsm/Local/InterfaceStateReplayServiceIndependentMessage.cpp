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

 /**************************************************************************
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mayank Maheshwari		                                       *
 ***************************************************************************/

#include "Nsm/Local/InterfaceStateReplayServiceIndependentMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"


namespace DcmNs
{

InterfaceStateReplayServiceIndependentMessage::InterfaceStateReplayServiceIndependentMessage ()
    : ManagementInterfaceServiceIndependentMessage (NsmLocalObjectManager::getClassName (), INTERFACE_STATE_REPLAY_MSG)
{
    m_eventId = 0;
    m_parameter = 0;
    m_isPluginEnabled = true;
}

InterfaceStateReplayServiceIndependentMessage::InterfaceStateReplayServiceIndependentMessage (const UI32 &event, const UI32 &parameter)
    : ManagementInterfaceServiceIndependentMessage (NsmLocalObjectManager::getClassName (), INTERFACE_STATE_REPLAY_MSG),
    m_eventId (event), m_parameter (parameter)
{
    m_isPluginEnabled = true;
}

InterfaceStateReplayServiceIndependentMessage::~InterfaceStateReplayServiceIndependentMessage ()
{
}

void InterfaceStateReplayServiceIndependentMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32 (&m_parameter, "parameter"));
    addSerializableAttribute (new AttributeUI32 (&m_eventId, "eventid"));
    addSerializableAttribute (new AttributeBool (&m_isPluginEnabled, "isPluginEnabled"));
}

void InterfaceStateReplayServiceIndependentMessage::setEventId(const UI32 &event) {
    m_eventId = event;
}

UI32 InterfaceStateReplayServiceIndependentMessage::getEventId()  const {
    return m_eventId;
}

void InterfaceStateReplayServiceIndependentMessage::setParameter(const UI32 &parameter) {
    m_parameter = parameter;
}

UI32 InterfaceStateReplayServiceIndependentMessage::getParameter()  const {
    return m_parameter;
}

void InterfaceStateReplayServiceIndependentMessage::setIsPluginEnabled(bool isEnabled) {
    m_isPluginEnabled = isEnabled;
}

bool InterfaceStateReplayServiceIndependentMessage::isPluginEnabled()  {
    return m_isPluginEnabled;
}
}
