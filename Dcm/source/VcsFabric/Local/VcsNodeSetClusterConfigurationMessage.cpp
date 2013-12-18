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

#include "VcsFabric/Local/VcsNodeSetClusterConfigurationMessage.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

VcsNodeSetClusterConfigurationMessage::VcsNodeSetClusterConfigurationMessage ()
    : VcsNodeClusterConfigurationChangeMessage (0, 0, 0, VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE, VCS_FABRIC_SET_VCS_CLUSTER_CONFIG)
{
    m_isCommitTransactionCompleted = false;
}

VcsNodeSetClusterConfigurationMessage::VcsNodeSetClusterConfigurationMessage (const UI32 &vcsId, const UI32 &vcsMode, const UI32 &rbridgeId, const ResourceId &clusterType)
    : VcsNodeClusterConfigurationChangeMessage (vcsId, vcsMode, rbridgeId, clusterType, VCS_FABRIC_SET_VCS_CLUSTER_CONFIG)
{
    m_isCommitTransactionCompleted = false;
}

VcsNodeSetClusterConfigurationMessage::~VcsNodeSetClusterConfigurationMessage ()
{
}

void VcsNodeSetClusterConfigurationMessage::setupAttributesForSerialization ()
{
    VcsNodeClusterConfigurationChangeMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeBool   (&m_isCommitTransactionCompleted, "isCommitTransactionCompleted")); 
}

void VcsNodeSetClusterConfigurationMessage::setCommitTransactionCompleted (const bool& completed)
{
    m_isCommitTransactionCompleted = completed;
}

const bool VcsNodeSetClusterConfigurationMessage::getCommitTransactionCompleted () const
{
    return m_isCommitTransactionCompleted;
}

}

