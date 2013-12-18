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


#include "Framework/Messaging/Local/PrismMessage.h"
#include "VcsFabric/Local/VcsIsIpReachableFromNodeMessage.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"



using namespace WaveNs;

namespace DcmNs
{

    VcsIsIpReachableFromNodeMessage::VcsIsIpReachableFromNodeMessage ()
		: PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_IS_IP_REACHABLE_FROM_LOCAL_NODE)
{
        m_IsReachable = false;
        m_IpAddress.clear();
        m_RbridgeId = 0;
}



    VcsIsIpReachableFromNodeMessage::VcsIsIpReachableFromNodeMessage (const string &IpAddress)
		: PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_IS_IP_REACHABLE_FROM_LOCAL_NODE)
{
        m_IsReachable = false;
        m_IpAddress = IpAddress;
        m_RbridgeId = 0;
}

    VcsIsIpReachableFromNodeMessage::~VcsIsIpReachableFromNodeMessage ()
    {

    }

    void VcsIsIpReachableFromNodeMessage::setupAttributesForSerialization()
    {
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_IpAddress,"IpAddress"));
		addSerializableAttribute (new AttributeBool(&m_IsReachable,"IsReachable"));
		addSerializableAttribute (new AttributeUI32(&m_RbridgeId,"RbridgeId"));
    }

    string VcsIsIpReachableFromNodeMessage::getIpAddress()  const
    {
        return m_IpAddress;
    }

    void VcsIsIpReachableFromNodeMessage::setIsReachableFlag(const bool IsReachable)
    {
        m_IsReachable = IsReachable;
    }

    bool VcsIsIpReachableFromNodeMessage::getIsReachableFlag() const
    {
        return m_IsReachable;
    }

void VcsIsIpReachableFromNodeMessage::setRbridgeId(const UI32 rbId)
{
    m_RbridgeId = rbId;
}

UI32 VcsIsIpReachableFromNodeMessage::getRbridgeId() const
{
    return m_RbridgeId;
}


}
