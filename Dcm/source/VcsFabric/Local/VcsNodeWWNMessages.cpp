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

#include "VcsFabric/Local/VcsNodeWWNMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Attributes/AttributeBitMap.h"

namespace DcmNs
{

VcsNodeSetSwitchWWNMessage::VcsNodeSetSwitchWWNMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_SWITCHWWN)
{
    m_rBridgeId = 0;
}

VcsNodeSetSwitchWWNMessage::VcsNodeSetSwitchWWNMessage (const UI32 &rBridgeId, const WorldWideName &newWWN)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_SWITCHWWN),
      m_rBridgeId                 (rBridgeId),
      m_newWWN                    (newWWN)
{
}

VcsNodeSetSwitchWWNMessage::~VcsNodeSetSwitchWWNMessage ()
{
}

void VcsNodeSetSwitchWWNMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32            (&m_rBridgeId,  "rBridgeId"));
     addSerializableAttribute (new AttributeWorldWideName   (&m_newWWN,     "newWWN"));
}

WorldWideName VcsNodeSetSwitchWWNMessage::getNewWWN () const
{
    return (m_newWWN);
}

UI32 VcsNodeSetSwitchWWNMessage::getRBridgeId () const
{
    return (m_rBridgeId);
}

/////////////////////////////////////////////////////////////////////////////
// Get Switch WWN from RbridgeId
//////////////////////////////////////////////////////////////////////////////
VcsNodeGetSwitchWWNMessage::VcsNodeGetSwitchWWNMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_SWITCHWWN)
{
}

VcsNodeGetSwitchWWNMessage::~VcsNodeGetSwitchWWNMessage ()
{
}

void VcsNodeGetSwitchWWNMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeWorldWideName (&m_switchWWN, "switchWWN"));
}

WorldWideName VcsNodeGetSwitchWWNMessage::getSwitchWWN () const
{
    return (m_switchWWN);
}

void VcsNodeGetSwitchWWNMessage::setSwitchWWN (const WorldWideName &wwn)
{
    m_switchWWN = wwn;
}


VcsNodeGetSwitchWWNFromRBridgeIdMessage::VcsNodeGetSwitchWWNFromRBridgeIdMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_SWITCHWWN_FROM_RBRIDGEID)
{
    m_rBridgeId = 0;
}

VcsNodeGetSwitchWWNFromRBridgeIdMessage::~VcsNodeGetSwitchWWNFromRBridgeIdMessage ()
{
}

void VcsNodeGetSwitchWWNFromRBridgeIdMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeWorldWideName   (&m_switchWWN, "switchWWN"));
     addSerializableAttribute (new AttributeUI32            (&m_rBridgeId, "rBridgeId"));
}

UI32 VcsNodeGetSwitchWWNFromRBridgeIdMessage::getRBridgeId () const
{
    return (m_rBridgeId);
}

void VcsNodeGetSwitchWWNFromRBridgeIdMessage::setRBridgeId (const UI32 rBridgeId)
{
    m_rBridgeId = rBridgeId;
}

WorldWideName VcsNodeGetSwitchWWNFromRBridgeIdMessage::getSwitchWWN () const
{
    return (m_switchWWN);
}

void VcsNodeGetSwitchWWNFromRBridgeIdMessage::setSwitchWWN (const WorldWideName &switchWWN)
{
    m_switchWWN = switchWWN;
}

/************************************
* Get available RbridgeId from WWN
*************************************/

VcsGetClusterRbridgeIdFromWWNMessage::VcsGetClusterRbridgeIdFromWWNMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_GET_CLUSTER_RBRIDGEID_FROM_WWN)
{
}

VcsGetClusterRbridgeIdFromWWNMessage::VcsGetClusterRbridgeIdFromWWNMessage (const WorldWideName &wwn)
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_GET_CLUSTER_RBRIDGEID_FROM_WWN),
      m_inputWWN (wwn)
{
}

VcsGetClusterRbridgeIdFromWWNMessage::~VcsGetClusterRbridgeIdFromWWNMessage ()
{
}

void VcsGetClusterRbridgeIdFromWWNMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeWorldWideName   (&m_inputWWN,        "inputWWN"));
    addSerializableAttribute (new AttributeBitMap          (&m_rbridgeIdBitMap, "rbridgeIdBitMap"));
}

void VcsGetClusterRbridgeIdFromWWNMessage::setRbridgeIdBitMap (BitMap &rbridgeIdBitMap)
{
    m_rbridgeIdBitMap = rbridgeIdBitMap;
}

BitMap VcsGetClusterRbridgeIdFromWWNMessage::getRbridgeIdBitMap () const
{
    return (m_rbridgeIdBitMap);
}

void VcsGetClusterRbridgeIdFromWWNMessage::setInputWWN (const WorldWideName &wwn)
{
    m_inputWWN = wwn;
}

WorldWideName VcsGetClusterRbridgeIdFromWWNMessage::getInputWWN () const
{
    return (m_inputWWN);
}

}
