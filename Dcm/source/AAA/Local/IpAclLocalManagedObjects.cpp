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
 *   Author : cshah                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "AAA/Local/IpAclLocalManagedObjects.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    IpAclPolicyLocalManagedObject::IpAclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAALocalObjectManager)
        : PrismElement  (pAAALocalObjectManager),
        PrismPersistableObject (IpAclPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalObjectManager),
        DcmManagedObject (pAAALocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalObjectManager)
    {
    }
    IpAclPolicyLocalManagedObject::IpAclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAALocalObjectManager, const string &switchid, const string &ipAclPolicy, const ACLTrafficType &ipAclTrafFlow, const UI32 &ipAclType)
        : PrismElement  (pAAALocalObjectManager),
        PrismPersistableObject (IpAclPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalObjectManager),
        DcmManagedObject (pAAALocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalObjectManager),
		m_switchid (switchid),
        m_ipAclPolicy  (ipAclPolicy),
        m_ipAclTrafFlow  (ipAclTrafFlow),
		m_ipAclType	(ipAclType)
    {
    }

    IpAclPolicyLocalManagedObject::~IpAclPolicyLocalManagedObject ()
    {
    }

    string  IpAclPolicyLocalManagedObject::getClassName()
    {
        return ("IpAclPolicyLocalManagedObject");
    }

    void  IpAclPolicyLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_switchid,"switchid", ip_access_list_name));
		addPersistableAttribute (new AttributeString(&m_ipAclPolicy, "ipaclpolicy", ip_access_list_mgmt_ip_access_list));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_ipAclTrafFlow,"ipacltrafflow", ip_access_list_mgmt_ip_direction));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_ipAclType,"ipacltype"));
    }

    void  IpAclPolicyLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeString(&m_switchid,"switchid", ip_access_list_name));
		addPersistableAttributeForCreate (new AttributeString(&m_ipAclPolicy, "ipaclpolicy", ip_access_list_mgmt_ip_access_list));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_ipAclTrafFlow,"ipacltrafflow", ip_access_list_mgmt_ip_direction));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_ipAclType,"ipacltype"));
		//addPersistableAttributeForCreate (new AttributeObjectId(&m_ipv6AclPolicy,"ipv6aclpolicy"));
		
		vector<string > keyName;
		keyName.push_back ("switchid");
		setUserDefinedKeyCombination (keyName);
	}

	void IpAclPolicyLocalManagedObject::setSwitchId(const string &switchid) 
	{
		m_switchid = switchid;
	}
	
	string IpAclPolicyLocalManagedObject::getSwitchId()  const
	{
		return (m_switchid);
	}

	void IpAclPolicyLocalManagedObject::setIpAclPolicy(const string &ipAclPolicy) 
	{
		m_ipAclPolicy = ipAclPolicy;
	}
	
	string IpAclPolicyLocalManagedObject::getIpAclPolicy()  const
	{
		return (m_ipAclPolicy);
	}
	void IpAclPolicyLocalManagedObject::setIpAclTrafFlow(const ACLTrafficType &ipAclTrafFlow) 
	{
		m_ipAclTrafFlow = ipAclTrafFlow;
	}
	
	ACLTrafficType IpAclPolicyLocalManagedObject::getIpAclTrafFlow()
	{
		return (m_ipAclTrafFlow);
	}
	void IpAclPolicyLocalManagedObject::setIpAclType(const UI32 &ipAclType)
	{
		m_ipAclType = ipAclType;
	}
	UI32 IpAclPolicyLocalManagedObject::getIpAclType()
	{
		return(m_ipAclType);
	}


    Ip6AclPolicyLocalManagedObject::Ip6AclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAALocalObjectManager)
        : PrismElement  (pAAALocalObjectManager),
        PrismPersistableObject (Ip6AclPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalObjectManager),
        DcmManagedObject (pAAALocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalObjectManager)
    {
    }
    Ip6AclPolicyLocalManagedObject::Ip6AclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAALocalObjectManager, const string &switchid, const string &ipv6AclPolicy, const ACLTrafficType &ipv6AclTrafFlow, const UI32 &ipv6AclType)
        : PrismElement  (pAAALocalObjectManager),
        PrismPersistableObject (IpAclPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pAAALocalObjectManager),
        DcmManagedObject (pAAALocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pAAALocalObjectManager),
		m_switchid (switchid),
        m_ipv6AclPolicy  (ipv6AclPolicy),
        m_ipv6AclTrafFlow  (ipv6AclTrafFlow),
		m_ipv6AclType	(ipv6AclType)
    {
    }

    Ip6AclPolicyLocalManagedObject::~Ip6AclPolicyLocalManagedObject ()
    {
    }

    string  Ip6AclPolicyLocalManagedObject::getClassName()
    {
        return ("Ip6AclPolicyLocalManagedObject");
    }

    void  Ip6AclPolicyLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_switchid,"switchid", ipv6_access_list_name));
		addPersistableAttribute (new AttributeString(&m_ipv6AclPolicy,  "ip6aclpolicy", ipv6_access_list_mgmt_ipv6_access_list));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_ipv6AclTrafFlow,"ip6acltrafflow", ipv6_access_list_mgmt_ip_direction));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_ipv6AclType,"ip6acltype"));
    }

    void  Ip6AclPolicyLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeString(&m_switchid,"switchid", ipv6_access_list_name));
		addPersistableAttributeForCreate (new AttributeString(&m_ipv6AclPolicy, "ip6aclpolicy", ipv6_access_list_mgmt_ipv6_access_list));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_ipv6AclTrafFlow,"ip6acltrafflow", ipv6_access_list_mgmt_ip_direction));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_ipv6AclType,"ip6acltype"));

		vector<string > keyName;
		keyName.push_back ("switchid");
		setUserDefinedKeyCombination (keyName);
	}

	void Ip6AclPolicyLocalManagedObject::setSwitchId(const string &switchid) 
	{
		m_switchid = switchid;
	}
	
	string Ip6AclPolicyLocalManagedObject::getSwitchId()  const
	{
		return (m_switchid);
	}

	void Ip6AclPolicyLocalManagedObject::setIpv6AclPolicy(const string &ipv6AclPolicy) 
	{
		m_ipv6AclPolicy = ipv6AclPolicy;
	}
	
	string Ip6AclPolicyLocalManagedObject::getIpv6AclPolicy()  const
	{
		return (m_ipv6AclPolicy);
	}

	void Ip6AclPolicyLocalManagedObject::setIpv6AclTrafFlow(const ACLTrafficType &ipv6AclTrafFlow) 
	{
		m_ipv6AclTrafFlow = ipv6AclTrafFlow;
	}
	
	ACLTrafficType Ip6AclPolicyLocalManagedObject::getIpv6AclTrafFlow()
	{
		return (m_ipv6AclTrafFlow);
	}

	void Ip6AclPolicyLocalManagedObject::setIpv6AclType(const UI32 &ipv6AclType)
	{
		m_ipv6AclType = ipv6AclType;
	}

	UI32 Ip6AclPolicyLocalManagedObject::getIpv6AclType()
	{
		return(m_ipv6AclType);
	}

}
