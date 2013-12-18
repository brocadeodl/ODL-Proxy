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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu                                                      *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeClusterConfigurationChangeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace DcmNs
{

VcsNodeClusterConfigurationChangeMessage::VcsNodeClusterConfigurationChangeMessage (const UI32 &vcsId, const UI32 &vcsMode, const UI32 &rbridgeId, const ResourceId &clusterType, const UI32 &operationId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), operationId),
    m_vcsId (vcsId), m_vcsMode (vcsMode), m_rbridgeId (rbridgeId), m_clusterType (clusterType)
{
}

VcsNodeClusterConfigurationChangeMessage::~VcsNodeClusterConfigurationChangeMessage ()
{
}

void VcsNodeClusterConfigurationChangeMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32         (&m_vcsId,      "vcsId"));
    addSerializableAttribute (new AttributeUI32         (&m_vcsMode,    "vcsMode"));
    addSerializableAttribute (new AttributeUI32         (&m_rbridgeId,  "rbridgeId"));
    addSerializableAttribute (new AttributeResourceId   (&m_clusterType,"clusterType"));
}

UI32 VcsNodeClusterConfigurationChangeMessage::getVcsId () const
{
    return (m_vcsId);
}

void VcsNodeClusterConfigurationChangeMessage::setVcsId (const UI32 &vcsId)
{
    m_vcsId = vcsId;
}

UI32 VcsNodeClusterConfigurationChangeMessage::getVcsMode () const
{
    return (m_vcsMode);
}

void VcsNodeClusterConfigurationChangeMessage::setVcsMode (const UI32 &vcsMode)
{
    m_vcsMode = vcsMode;
}

UI32 VcsNodeClusterConfigurationChangeMessage::getRbridgeId () const
{
    return (m_rbridgeId);
}

void VcsNodeClusterConfigurationChangeMessage::setRbridgeId (const UI32 &rbridgeId)
{
    m_rbridgeId = rbridgeId;
}

ResourceId VcsNodeClusterConfigurationChangeMessage::getClusterType () const
{
    return (m_clusterType);
}

void VcsNodeClusterConfigurationChangeMessage::setClusterType (const ResourceId& clusterType)
{
    m_clusterType = clusterType;
}

}

