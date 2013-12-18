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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeIpMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeGetSwitchIPFromRBridgeIdMessage::VcsNodeGetSwitchIPFromRBridgeIdMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_SWITCHIP_FROM_RBRIDGEID)
{
    m_rBridgeId = 0;
}

VcsNodeGetSwitchIPFromRBridgeIdMessage::~VcsNodeGetSwitchIPFromRBridgeIdMessage ()
{
}

void VcsNodeGetSwitchIPFromRBridgeIdMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString  (&m_switchIP,   "switchIP"));
     addSerializableAttribute (new AttributeUI32    (&m_rBridgeId,  "rBridgeId"));
}

UI32 VcsNodeGetSwitchIPFromRBridgeIdMessage::getRBridgeId () const
{
    return (m_rBridgeId);
}

void VcsNodeGetSwitchIPFromRBridgeIdMessage::setRBridgeId (const UI32 rBridgeId)
{
    m_rBridgeId = rBridgeId;
}

string VcsNodeGetSwitchIPFromRBridgeIdMessage::getSwitchIP () const
{
    return (m_switchIP);
}

void VcsNodeGetSwitchIPFromRBridgeIdMessage::setSwitchIP (const string &switchIP)
{
    m_switchIP = switchIP;
}

}
