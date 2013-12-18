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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeRBridgeIdMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSetRBridgeIdMessage::VcsNodeSetRBridgeIdMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_RBRIDGEID)
{
}

VcsNodeSetRBridgeIdMessage::VcsNodeSetRBridgeIdMessage (const UI32 &rbridgeId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_RBRIDGEID),
      m_rbridgeId                    (rbridgeId)
{
    m_rbridgeId = rbridgeId;
}

VcsNodeSetRBridgeIdMessage::~VcsNodeSetRBridgeIdMessage ()
{
}

void VcsNodeSetRBridgeIdMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_rbridgeId, "rbridgeId"));
}

UI32 VcsNodeSetRBridgeIdMessage::getRBridgeId () const
{
    return (m_rbridgeId);
}


VcsNodeGetRBridgeIdMessage::VcsNodeGetRBridgeIdMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_RBRIDGEID)
{
    m_rbridgeId = 0;
}
VcsNodeGetRBridgeIdMessage::~VcsNodeGetRBridgeIdMessage ()
{
}

void VcsNodeGetRBridgeIdMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_rbridgeId, "rbridgeId"));
}

UI32 VcsNodeGetRBridgeIdMessage::getRBridgeId () const
{
    return (m_rbridgeId);
}

void VcsNodeGetRBridgeIdMessage::setRBridgeId (const UI32 &rbridgeId)
{
    m_rbridgeId = rbridgeId;
}

VcsNodeGetRBridgeIdFromSwitchWWNMessage::VcsNodeGetRBridgeIdFromSwitchWWNMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_RBRIDGEID_FROM_SWITCHWWN)
{
    m_rBridgeId = 0;
}

VcsNodeGetRBridgeIdFromSwitchWWNMessage::~VcsNodeGetRBridgeIdFromSwitchWWNMessage ()
{
}

void VcsNodeGetRBridgeIdFromSwitchWWNMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
    
     addSerializableAttribute (new AttributeUI32            (&m_rBridgeId,  "rBridgeId"));
     addSerializableAttribute (new AttributeWorldWideName   (&m_switchWWN,  "switchWWN"));
}

UI32 VcsNodeGetRBridgeIdFromSwitchWWNMessage::getRBridgeId () const
{
    return (m_rBridgeId);
}

void VcsNodeGetRBridgeIdFromSwitchWWNMessage::setRBridgeId (const UI32 rBridgeId)
{
    m_rBridgeId = rBridgeId;
}

WorldWideName VcsNodeGetRBridgeIdFromSwitchWWNMessage::getSwitchWWN () const
{
    return (m_switchWWN);
}

void VcsNodeGetRBridgeIdFromSwitchWWNMessage::setSwitchWWN (const WorldWideName &switchWWN)
{
    m_switchWWN = switchWWN;
}

}
