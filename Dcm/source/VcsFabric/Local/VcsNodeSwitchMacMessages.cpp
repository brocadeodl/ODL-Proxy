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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeSwitchMacMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

/************************************ 
* GET SWITCH MAC 
*************************************/

VcsNodeGetSwitchMacMessage::VcsNodeGetSwitchMacMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_SWITCHMAC)
{
    m_mappedId = 0;
}

VcsNodeGetSwitchMacMessage::VcsNodeGetSwitchMacMessage (const UI32 &mappedId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_SWITCHMAC),
      m_mappedId                 (mappedId)
{
}

VcsNodeGetSwitchMacMessage::~VcsNodeGetSwitchMacMessage ()
{
}

void VcsNodeGetSwitchMacMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32        (&m_mappedId,   "mappedId"));
     addSerializableAttribute (new AttributeMacAddress  (&m_switchMac,  "switchMac"));
}

void VcsNodeGetSwitchMacMessage::setMappedId (const UI32 &mappedId)
{
    m_mappedId = mappedId;
}

void VcsNodeGetSwitchMacMessage::setSwitchMac (const MacAddress &switch_mac)
{
    m_switchMac = switch_mac;
}

UI32 VcsNodeGetSwitchMacMessage::getMappedId ()
{
    return (m_mappedId);
}

}
