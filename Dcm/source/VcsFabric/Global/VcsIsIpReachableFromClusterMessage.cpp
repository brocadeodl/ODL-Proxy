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
*   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Sudhakar Jha                                               *
***************************************************************************/


#include "VcsFabric/Global/VcsIsIpReachableFromClusterMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"


using namespace WaveNs;

namespace DcmNs
{

    VcsIsIpReachableFromClusterMessage::VcsIsIpReachableFromClusterMessage ()
		:ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_IS_IP_REACHABLE_FROM_CLUSTER)
    {
        m_IpAddress.clear();
        m_IsReachable = false;
        m_rbridgeIdOfIpReachableNodes.clear();
    }



    VcsIsIpReachableFromClusterMessage::VcsIsIpReachableFromClusterMessage (const string &IpAddress)
		:  ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_IS_IP_REACHABLE_FROM_CLUSTER)
    {
        m_IpAddress = IpAddress;
        m_IsReachable = false;
        m_rbridgeIdOfIpReachableNodes.clear();
    }

    VcsIsIpReachableFromClusterMessage::~VcsIsIpReachableFromClusterMessage ()
    {

    }

    void VcsIsIpReachableFromClusterMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
            
         addSerializableAttribute (new AttributeString      (&m_IpAddress,                      "IpAddress"));
         addSerializableAttribute (new AttributeBool        (&m_IsReachable,                    "IsReachable"));
         addSerializableAttribute (new AttributeUI32Vector  (&m_rbridgeIdOfIpReachableNodes,    "rbridgeIdOfIpReachableNodes"));


    }

    
    string VcsIsIpReachableFromClusterMessage::getIpAddress()  const
    {
        return m_IpAddress;
    }

    void VcsIsIpReachableFromClusterMessage::setIsReachableFlag(const bool IsReachable)
    {
        m_IsReachable = IsReachable;
    }

    bool VcsIsIpReachableFromClusterMessage::getIsReachableFlag() const
    {
        return m_IsReachable;
    }

void VcsIsIpReachableFromClusterMessage::addRbridgeIdOfConnectedNodes(const UI32 rbridgeID)
{
    m_rbridgeIdOfIpReachableNodes.push_back(rbridgeID);
}

vector<UI32> VcsIsIpReachableFromClusterMessage::getPingResponseForNode() const
{
    return m_rbridgeIdOfIpReachableNodes;
}

}

