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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "VcsFabric/Global/VcsRemoveSwitchMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"


namespace DcmNs
{
VcsRemoveSwitchMessage::VcsRemoveSwitchMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_REMOVE_SWITCH),
    m_switchMappedId                 (0),
    m_isDisconnected                 (false)
{

}
VcsRemoveSwitchMessage::VcsRemoveSwitchMessage (const vector<UI32> &switchMappedId)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_REMOVE_SWITCH),
    m_switchMappedId                (switchMappedId),
    m_isDisconnected                (false)
{
}
VcsRemoveSwitchMessage::~VcsRemoveSwitchMessage ()
{
}

void VcsRemoveSwitchMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32Vector (&m_switchMappedId, "switchMappedId"));
     addSerializableAttribute (new AttributeBool       (&m_isDisconnected, "isDisconnected"));
}

vector<UI32> VcsRemoveSwitchMessage::getMappedId ()
{
	return m_switchMappedId;
}

void VcsRemoveSwitchMessage::setMappedId (const vector<UI32> &switchMappedId)
{
	m_switchMappedId = switchMappedId;
}

void VcsRemoveSwitchMessage::setIsDisconnected(bool is_Disconnected)
{
    m_isDisconnected = is_Disconnected;
}

bool VcsRemoveSwitchMessage::getIsDisconnected ()
{
    return m_isDisconnected;
}
}

