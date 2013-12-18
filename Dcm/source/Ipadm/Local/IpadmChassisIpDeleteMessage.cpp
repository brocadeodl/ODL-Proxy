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

#include "Ipadm/Local/IpadmChassisIpDeleteMessage.h"
#include "Ipadm/Local/IpadmChassisObjectManager.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

	IpadmChassisIpDeleteMessage::IpadmChassisIpDeleteMessage ()
		: PrismMessage (IpadmChassisObjectManager::getPrismServiceId (), IPADMCHASSISDELETE)
	{
	}

	IpadmChassisIpDeleteMessage::IpadmChassisIpDeleteMessage (const UI32 &VirtualIpTag, const IpadmAddressType &IpAddrType)
		: PrismMessage (IpadmChassisObjectManager::getPrismServiceId (), IPADMCHASSISDELETE),
		m_VirtualIpTag	(VirtualIpTag),
		m_AddrType (IpAddrType)
	{
	}

	IpadmChassisIpDeleteMessage::~IpadmChassisIpDeleteMessage ()
	{
	}

	void  IpadmChassisIpDeleteMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_VirtualIpTag,"VirtualIpTag"));
		addSerializableAttribute (new AttributeEnum((UI32 *)(&m_AddrType),"AddrType"));
	}

	void  IpadmChassisIpDeleteMessage::setVirtualIpTag(const UI32 &VirtualIpTag)
	{
		m_VirtualIpTag  =  VirtualIpTag;
	}

	UI32  IpadmChassisIpDeleteMessage::getVirtualIpTag() const
	{
		return (m_VirtualIpTag);
	}

        void  IpadmChassisIpDeleteMessage::setIpAddrFlag(const IpadmAddressType &IpAddrFlag)
        {
                m_AddrType  =  IpAddrFlag;
        }

        IpadmAddressType  IpadmChassisIpDeleteMessage::getIpAddrFlag() const
        {
                return (m_AddrType);
        }


}
