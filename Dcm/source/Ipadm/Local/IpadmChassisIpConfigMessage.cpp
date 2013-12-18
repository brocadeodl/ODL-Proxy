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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : cshah													 *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "Ipadm/Local/IpadmChassisIpConfigMessage.h"
#include "Ipadm/Local/IpadmChassisObjectManager.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

	IpadmChassisIpConfigMessage::IpadmChassisIpConfigMessage ()
		: PrismMessage (IpadmChassisObjectManager::getPrismServiceId (), IPADMCHASSISCONFIG)
	{
	}

	IpadmChassisIpConfigMessage::IpadmChassisIpConfigMessage (const string &VirtualIpAddress, const IpadmAddressType &IpAddrType)
		: PrismMessage (IpadmChassisObjectManager::getPrismServiceId (), IPADMCHASSISCONFIG),
		m_VirtualIpAddress	(VirtualIpAddress),
		m_AddrType	(IpAddrType)
	{
	}

	IpadmChassisIpConfigMessage::~IpadmChassisIpConfigMessage ()
	{
	}

	void  IpadmChassisIpConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_VirtualIpAddress,"VirtualIpAddress"));
        	addSerializableAttribute (new AttributeEnum((UI32 *)(&m_AddrType),"AddrType"));
	}

	void  IpadmChassisIpConfigMessage::setVirtualIpAddress(const string &VirtualIpAddress)
	{
		m_VirtualIpAddress  =  VirtualIpAddress;
	}

	string  IpadmChassisIpConfigMessage::getVirtualIpAddress() const
	{
		return (m_VirtualIpAddress);
	}

	void  IpadmChassisIpConfigMessage::setIpAddrFlag(const IpadmAddressType &IpAddrFlag)
	{
		m_AddrType  =  IpAddrFlag;
	}

	IpadmAddressType  IpadmChassisIpConfigMessage::getIpAddrFlag() const
	{
		return (m_AddrType);
	}

}
