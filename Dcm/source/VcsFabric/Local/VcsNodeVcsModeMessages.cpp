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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeVcsModeMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSetVcsModeMessage::VcsNodeSetVcsModeMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_VCSMODE)
{
    m_vcsMode = 0;
}

VcsNodeSetVcsModeMessage::VcsNodeSetVcsModeMessage (const UI32 &vcsMode)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_VCSMODE),
      m_vcsMode                    (vcsMode)
{
}

VcsNodeSetVcsModeMessage::~VcsNodeSetVcsModeMessage ()
{
}

void VcsNodeSetVcsModeMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_vcsMode, "vcsMode"));
}

UI32 VcsNodeSetVcsModeMessage::getVcsMode () const
{
    return (m_vcsMode);
}

/************************************ 
* GET VCS_ID
*************************************/

VcsNodeGetVcsModeMessage::VcsNodeGetVcsModeMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_VCSMODE)
{
    m_vcsMode = 0;
    setIsMessageSupportedWhenServiceIsPaused (true);
}

VcsNodeGetVcsModeMessage::~VcsNodeGetVcsModeMessage ()
{
}

void VcsNodeGetVcsModeMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_vcsMode, "vcsMode"));
}

void VcsNodeGetVcsModeMessage::setVcsMode (const UI32 &vcsMode)
{
    m_vcsMode = vcsMode;
}

UI32 VcsNodeGetVcsModeMessage::getVcsMode () const
{
    return (m_vcsMode);
}

/************************************
* GET Cluster type
*************************************/

VcsNodeGetClusterTypeMessage::VcsNodeGetClusterTypeMessage ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_CLUSTER_TYPE)
{
    m_clusterType = 0;
}

VcsNodeGetClusterTypeMessage::~VcsNodeGetClusterTypeMessage ()
{
}

void VcsNodeGetClusterTypeMessage::setupAttributesForSerialization ()
{
     ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_clusterType, "clusterType"));
}

void VcsNodeGetClusterTypeMessage::setClusterType (const UI32 &clusterType)
{
    m_clusterType = clusterType;
}

UI32 VcsNodeGetClusterTypeMessage::getClusterType () const
{
    return (m_clusterType);
}

/************************************
* Validate vcs enable
*************************************/

VcsNodeEnableValidationMessage::VcsNodeEnableValidationMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_ENABLE_VALIDATE)
{
}

VcsNodeEnableValidationMessage::~VcsNodeEnableValidationMessage ()
{
}

/************************************
* Select ShowVcs Format
*************************************/
VcsNodeSetNewShowVcsFormatMessage::VcsNodeSetNewShowVcsFormatMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_NEWSHOWVCS)
{
    m_newShowVcs = false;
}

VcsNodeSetNewShowVcsFormatMessage::VcsNodeSetNewShowVcsFormatMessage (const bool &newShowVcs)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_NEWSHOWVCS)
{
    m_newShowVcs = newShowVcs;
}

VcsNodeSetNewShowVcsFormatMessage::~VcsNodeSetNewShowVcsFormatMessage ()
{
}

void VcsNodeSetNewShowVcsFormatMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_newShowVcs, "newShowVcs"));
}

void VcsNodeSetNewShowVcsFormatMessage::setNewShowVcs (const bool &newShowVcs)
{
    m_newShowVcs = newShowVcs;
}

bool VcsNodeSetNewShowVcsFormatMessage::getNewShowVcs () const
{
    return (m_newShowVcs);
}

}
