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
 *   Author : Hari Om                                                      *
 **************************************************************************/

#include "Ssm/Global/Ip6AclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "vcs.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{

    Ip6AclRuleManagedObject::Ip6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (Ip6AclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager)
    {
            m_ruleid = 0;
			m_srcType = IP_ADDR_TYPE_NONE;
    }

    Ip6AclRuleManagedObject::Ip6AclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const UI64 &ruleid,const UI32 &action, const WaveUnion  &srcUnion, const IpV6Address &srcHostIpAddress, 
		const IpV6AddressNetworkMask &srcIpAddress,
// const IpV6Address &srcIpAddressMask,
		 const IpAddrType &srcType, 
		const bool &isCount, const bool &isLog)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (Ip6AclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        m_ruleid    (ruleid),
        m_action    (action),
		m_srcUnion (srcUnion),
		m_srcHostIpAddress    (srcHostIpAddress),
		m_srcIpAddress    (srcIpAddress),
//		m_srcIpAddressMask (srcIpAddressMask),
        m_srcType   (srcType),
        m_isCount    (isCount),
		m_isLog		(isLog)
    {
    }

    Ip6AclRuleManagedObject::~Ip6AclRuleManagedObject ()
    {
    }

    string  Ip6AclRuleManagedObject::getClassName()
    {
        return ("Ip6AclRuleManagedObject");
    }

    void  Ip6AclRuleManagedObject::setupAttributesForPersistence()
    {
		IpAddrType	defaultValsrcAddrType = IP_ADDR_TYPE_NONE; 
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI64(&m_ruleid, "ruleid",ipv6_access_list_seq_id));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_action), "action", ipv6_access_list_action));
		addPersistableAttribute (new AttributeIpV6Address(&m_srcHostIpAddress, "srcHostIpAddress",ipv6_access_list_src_host_ip));
		addPersistableAttribute (new AttributeIpV6AddressNetworkMask(&m_srcIpAddress, "srcIpAddress"));
//        addPersistableAttribute (new AttributeIpV6Address(&m_srcIpAddressMask, "srcIpAddressMask", ipv6_access_list_src_mask));
        addPersistableAttribute (new AttributeUnion(&m_srcUnion,"srcUnion", ipv6_access_list_src_host_any_sip));
		addPersistableAttribute (new AttributeEnum((UI32 *)(&m_srcType), defaultValsrcAddrType, "srcType"));
		addPersistableAttribute (new AttributeBool(&m_isCount, "isCount", ipv6_access_list_count));
		addPersistableAttribute (new AttributeBool(&m_isLog, "isLog", ipv6_access_list_log));
    }

    void  Ip6AclRuleManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI64(&m_ruleid,"ruleid"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_action),"action"));
        addPersistableAttributeForCreate  (new AttributeIpV6Address(&m_srcHostIpAddress,"srcHostIpAddress"));
        addPersistableAttributeForCreate  (new AttributeIpV6AddressNetworkMask(&m_srcIpAddress,"srcIpAddress"));
//		addPersistableAttributeForCreate  (new AttributeIpV6Address(&m_srcIpAddressMask,"srcIpAddressMask"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isCount,"isCount"));
		addPersistableAttributeForCreate  (new AttributeUnion(&m_srcUnion,"srcUnion"));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_srcType),"srcType"));
		addPersistableAttributeForCreate  (new AttributeBool(&m_isLog,"isLog"));
    }

    void Ip6AclRuleManagedObject::setupKeys()
	{
		vector<string> keyName;
		keyName.push_back("ruleid");
		setUserDefinedKeyCombination (keyName);
	}

    void  Ip6AclRuleManagedObject::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  Ip6AclRuleManagedObject::getRuleid() const
    {
        return (m_ruleid);
    }

    void  Ip6AclRuleManagedObject::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  Ip6AclRuleManagedObject::getAction() const
    {
        return (m_action);
    }

 
	void Ip6AclRuleManagedObject::setSrcHostIpAddress(const IpV6Address &srcHostIpAddress)
    {
        m_srcHostIpAddress  =  srcHostIpAddress;
    }

    IpV6Address	Ip6AclRuleManagedObject::getSrcHostIpAddress() const
    {
        return (m_srcHostIpAddress);
    }
	
	void Ip6AclRuleManagedObject::setSrcIpAddress(const IpV6AddressNetworkMask &srcIpAddress)
    {
        m_srcIpAddress  =  srcIpAddress;
    }

    IpV6AddressNetworkMask Ip6AclRuleManagedObject::getSrcIpAddress() const
    {
        return (m_srcIpAddress);
    }
    
/*
    void  Ip6AclRuleManagedObject::setSrcIpAddressMask(const IpV6Address &srcIpAddressMask)
    {
        m_srcIpAddressMask  =  srcIpAddressMask;
    }

    IpV6Address	Ip6AclRuleManagedObject::getSrcIpAddressMask() const
    {
        return (m_srcIpAddressMask);
    }
*/

    void  Ip6AclRuleManagedObject::setSrcType(const IpAddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    IpAddrType  Ip6AclRuleManagedObject::getSrcType() const
    {
        return (m_srcType);
    }
    
    void Ip6AclRuleManagedObject::setSrcUnion(const WaveUnion &val)
	{
		m_srcUnion = val;
	}

	WaveUnion Ip6AclRuleManagedObject::getSrcUnion() const
	{
		return (m_srcUnion);
	}

    void  Ip6AclRuleManagedObject::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  Ip6AclRuleManagedObject::getIsCount() const
    {
        return (m_isCount);
    }
    void  Ip6AclRuleManagedObject::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool	Ip6AclRuleManagedObject::getIsLog() const
    {
        return (m_isLog);
    }

}
