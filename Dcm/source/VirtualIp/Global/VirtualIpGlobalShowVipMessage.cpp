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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.	   *
 *   All rights reserved.						   *
 *   Author : aviswana							   *
 **************************************************************************/

#include "VirtualIp/Global/VirtualIpGlobalShowVipMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

namespace DcmNs
{

	VirtualIpGlobalShowVipMessage::VirtualIpGlobalShowVipMessage ()
		: PrismMessage (VcsFabObjectManager::getPrismServiceId (), VIRTUAL_IP_GLOBAL_SHOW_VIP)
	{
	}

	VirtualIpGlobalShowVipMessage::VirtualIpGlobalShowVipMessage (const string &VirtualIpAddress, const UI32 &pNodeId)
		: PrismMessage (VcsFabObjectManager::getPrismServiceId (), VIRTUAL_IP_GLOBAL_SHOW_VIP),
		m_VirtualIpAddress	(VirtualIpAddress),
		m_pNodeId		(pNodeId)
	{
	}

	VirtualIpGlobalShowVipMessage::~VirtualIpGlobalShowVipMessage ()
	{
	}

	void  VirtualIpGlobalShowVipMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_VirtualIpAddress,"VirtualIpAddress"));
		addSerializableAttribute (new AttributeUI32(&m_pNodeId,"pNodId"));
	}

	void  VirtualIpGlobalShowVipMessage::setVirtualIpAddress(const string &VirtualIpAddress)
	{
		m_VirtualIpAddress  =  VirtualIpAddress;
	}

	string  VirtualIpGlobalShowVipMessage::getVirtualIpAddress() const
	{
		return (m_VirtualIpAddress);
	}

        void  VirtualIpGlobalShowVipMessage::setpNodeId(const UI32 &pNodeId)
        {
                m_pNodeId  =  pNodeId;
        }

        UI32  VirtualIpGlobalShowVipMessage::getpNodeId() const
        {
                return (m_pNodeId);
        }
}
