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
 *   Author : pasu                                                     *
 **************************************************************************/

#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    MacAclRuleManagedObject::MacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (MacAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager)
    {
    }

    MacAclRuleManagedObject::MacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager, 
		const UI64 &ruleid,const UI32 &action, const string &srcMac, const string &srcMacMask, 
		const string &srcHost, const AddrType &srcType, const bool &isCount, const bool &isLog)
        : PrismElement  		(pSsmGlobalObjectManager),
        PrismPersistableObject  (MacAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject 		(pSsmGlobalObjectManager),
        DcmManagedObject 		(pSsmGlobalObjectManager),
        m_ruleid				(ruleid),
        m_action				(action),
        m_srcMac				(srcMac),
		m_srcMacMask			(srcMacMask),
		m_srcHost				(srcHost),
        m_srcType				(srcType),
        m_isCount				(isCount),
		m_isLog					(isLog)
    {
    }

    MacAclRuleManagedObject::~MacAclRuleManagedObject ()
    {
    }

    string  MacAclRuleManagedObject::getClassName()
    {
        return ("MacAclRuleManagedObject");
    }

    void  MacAclRuleManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        //addPersistableAttribute (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttribute (new AttributeUI64(&m_ruleid,"ruleid"));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_action),"action"));
        addPersistableAttribute (new AttributeString(&m_srcMac,"srcMac"));
        addPersistableAttribute (new AttributeString(&m_srcMacMask,true,"","srcMacMask"));
		addPersistableAttribute (new AttributeString(&m_srcHost,true,"","srcHost"));
        addPersistableAttribute (new AttributeBool((&m_isCount),"isCount"));
        addPersistableAttribute (new AttributeBool((&m_isLog),"isLog"));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_srcType),"srcType"));
        addPersistableAttribute (new AttributeUnion(&m_srcUnion, "srcUnion"));


        setUserTagForAttribute ("ruleid", mac_access_list_seq_id);
        setUserTagForAttribute ("action", mac_access_list_action);
        setUserTagForAttribute ("srcUnion", mac_access_list_source);
        setUserTagForAttribute ("isCount", mac_access_list_count);
		setUserTagForAttribute ("srcHost", mac_access_list_srchost);
		setUserTagForAttribute ("srcMacMask", mac_access_list_src_mac_addr_mask);
        setUserTagForAttribute ("isLog", mac_access_list_log);
    }

    void  MacAclRuleManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        //addPersistableAttributeForCreate  (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttributeForCreate  (new AttributeUI64(&m_ruleid,"ruleid"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_action),"action"));
        addPersistableAttributeForCreate  (new AttributeString(&m_srcMac,"srcMac"));
        addPersistableAttributeForCreate  (new AttributeString(&m_srcMacMask,true,"","srcMacMask"));
		addPersistableAttributeForCreate  (new AttributeString(&m_srcHost,true,"","srcHost"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_srcType),"srcType"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_srcUnion, "srcUnion"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isCount,"isCount"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_isLog,"isLog"));
    }

    void MacAclRuleManagedObject::setupKeys()
	{
		vector<string> keyName;
		keyName.push_back("ruleid");
		setUserDefinedKeyCombination (keyName);
	}
/*
    void  MacAclRuleManagedObject::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  MacAclRuleManagedObject::getPolicyName() const
    {
        return (m_policyName);
    }
    */

    void  MacAclRuleManagedObject::setSrcHost(const string &srcHost)
    {
        m_srcHost  =  srcHost;
    }

    string  MacAclRuleManagedObject::getSrcHost() const
    {
        return (m_srcHost);
    }

    void  MacAclRuleManagedObject::setRuleid(const UI64 &ruleid)
    {
        m_ruleid  =  ruleid;
    }

    UI64  MacAclRuleManagedObject::getRuleid() const
    {
        return (m_ruleid);
    }

    void  MacAclRuleManagedObject::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  MacAclRuleManagedObject::getAction() const
    {
        return (m_action);
    }

    void  MacAclRuleManagedObject::setSrcMac(const string &srcMac)
    {
        m_srcMac  =  srcMac;
        m_srcUnion = "srcMac";
        m_srcType = ADDR_TYPE_NONE;
    }

    string  MacAclRuleManagedObject::getSrcMac() const
    {
        return (m_srcMac);
    }

    void  MacAclRuleManagedObject::setSrcMacMask(const string &srcMacMask)
    {
        m_srcMacMask  =  srcMacMask;
    }

    string  MacAclRuleManagedObject::getSrcMacMask() const
    {
        return (m_srcMacMask);
    }
    
    void  MacAclRuleManagedObject::setSrcType(const AddrType &srcType)
    {
        m_srcType  =  srcType;
        m_srcUnion = "srcType";
    }

    AddrType  MacAclRuleManagedObject::getSrcType() const
    {
        return (m_srcType);
    }
    
    void MacAclRuleManagedObject::setSrcUnion(const WaveUnion &val)
	{
		m_srcUnion = val;
	}

	WaveUnion MacAclRuleManagedObject::getSrcUnion() const
	{
		return (m_srcUnion);
	}

/*
    void  MacAclRuleManagedObject::setFlag(const AclFlag &flag)
    {
        m_flag  =  flag;
    }

    AclFlag  MacAclRuleManagedObject::getFlag() const
    {
        return (m_flag);
    }
    */
    void  MacAclRuleManagedObject::setIsCount(const bool &isCount)
    {
        m_isCount  =  isCount;
    }

    bool  MacAclRuleManagedObject::getIsCount() const
    {
        return (m_isCount);
    }

    void  MacAclRuleManagedObject::setIsLog(const bool &isLog)
    {
        m_isLog  =  isLog;
    }

    bool  MacAclRuleManagedObject::getIsLog() const
    {
        return (m_isLog);
    }
	
}
