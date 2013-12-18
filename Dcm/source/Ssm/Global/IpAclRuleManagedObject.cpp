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
 *   Author : Bhavin                                                     *
 **************************************************************************/

#include "Ssm/Global/IpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "vcs.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{

    IpAclRuleManagedObject::IpAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (IpAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager)
    {
            m_ruleid = 0;
			m_srcType = IP_ADDR_TYPE_NONE;
    }

    IpAclRuleManagedObject::IpAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const UI64 &ruleid,const UI32 &action, const WaveUnion  &srcUnion, const IpV4Address &srcHostIpAddress, 
		const IpV4Address &srcIpAddress, const IpV4Address &srcIpAddressMask, const IpAddrType &srcType, 
		const bool &isCount, const bool &isLog)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (IpAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        m_ruleid    (ruleid),
        m_action    (action),
		m_srcUnion (srcUnion),
		m_srcHostIpAddress    (srcHostIpAddress),
		m_srcIpAddress    (srcIpAddress),
		m_srcIpAddressMask (srcIpAddressMask),
        m_srcType   (srcType),
        m_isCount    (isCount),
		m_isLog		(isLog)
    {
    }

    IpAclRuleManagedObject::~IpAclRuleManagedObject ()
    {
    }

    string  IpAclRuleManagedObject::getClassName()
    {
        return ("IpAclRuleManagedObject");
    }

    void  IpAclRuleManagedObject::setupAttributesForPersistence()
    {
		IpAddrType	defaultValsrcAddrType = IP_ADDR_TYPE_NONE; 
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI64(&m_ruleid,"ruleid",ip_access_list_seq_id));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_action), "action", ip_access_list_action));
		addPersistableAttribute (new AttributeIpV4Address(&m_srcHostIpAddress, "srcHostIpAddress",ip_access_list_src_host_ip));
		addPersistableAttribute (new AttributeIpV4Address(&m_srcIpAddress, "srcIpAddress"));
        addPersistableAttribute (new AttributeIpV4Address(&m_srcIpAddressMask, "srcIpAddressMask", ip_access_list_src_mask));
        addPersistableAttribute (new AttributeUnion(&m_srcUnion,"srcUnion", ip_access_list_src_host_any_sip));
		addPersistableAttribute (new AttributeEnum((UI32 *)(&m_srcType), defaultValsrcAddrType, "srcType"));
		addPersistableAttribute (new AttributeBool(&m_isCount, "isCount", ip_access_list_count));
		addPersistableAttribute (new AttributeBool(&m_isLog, "isLog", ip_access_list_log));
    }

    void  IpAclRuleManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI64(&m_ruleid,"ruleid"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_action),"action"));
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_srcHostIpAddress,"srcHostIpAddress"));
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_srcIpAddress,"srcIpAddress"));
		addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_srcIpAddressMask,"srcIpAddressMask"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isCount,"isCount"));
		addPersistableAttributeForCreate  (new AttributeUnion(&m_srcUnion,"srcUnion"));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_srcType),"srcType"));
		addPersistableAttributeForCreate  (new AttributeBool(&m_isLog,"isLog"));
    }

    void IpAclRuleManagedObject::setupKeys()
	{
		vector<string> keyName;
		keyName.push_back("ruleid");
		setUserDefinedKeyCombination (keyName);
	}

    void  IpAclRuleManagedObject::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  IpAclRuleManagedObject::getRuleid() const
    {
        return (m_ruleid);
    }

    void  IpAclRuleManagedObject::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  IpAclRuleManagedObject::getAction() const
    {
        return (m_action);
    }

 
	void IpAclRuleManagedObject::setSrcHostIpAddress(const IpV4Address &srcHostIpAddress)
    {
        m_srcHostIpAddress  =  srcHostIpAddress;
    }

    IpV4Address	IpAclRuleManagedObject::getSrcHostIpAddress() const
    {
        return (m_srcHostIpAddress);
    }
	
	void IpAclRuleManagedObject::setSrcIpAddress(const IpV4Address &srcIpAddress)
    {
        m_srcIpAddress  =  srcIpAddress;
    }

    IpV4Address	IpAclRuleManagedObject::getSrcIpAddress() const
    {
        return (m_srcIpAddress);
    }
    
    void  IpAclRuleManagedObject::setSrcIpAddressMask(const IpV4Address &srcIpAddressMask)
    {
        m_srcIpAddressMask  =  srcIpAddressMask;
    }

    IpV4Address	IpAclRuleManagedObject::getSrcIpAddressMask() const
    {
        return (m_srcIpAddressMask);
    }

    void  IpAclRuleManagedObject::setSrcType(const IpAddrType &srcType)
    {
        m_srcType  =  srcType;
    }

    IpAddrType  IpAclRuleManagedObject::getSrcType() const
    {
        return (m_srcType);
    }
    
    void IpAclRuleManagedObject::setSrcUnion(const WaveUnion &val)
	{
		m_srcUnion = val;
	}

	WaveUnion IpAclRuleManagedObject::getSrcUnion() const
	{
		return (m_srcUnion);
	}

    void  IpAclRuleManagedObject::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  IpAclRuleManagedObject::getIsCount() const
    {
        return (m_isCount);
    }
    void  IpAclRuleManagedObject::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool	IpAclRuleManagedObject::getIsLog() const
    {
        return (m_isLog);
    }

}
