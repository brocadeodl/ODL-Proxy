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
#include "Ssm/Global/ExtMacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Ssm/Local/SsmTypes.h"

namespace DcmNs
{

    ExtMacAclRuleManagedObject::ExtMacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
        : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtMacAclRuleManagedObject::getClassName (), MacAclRuleManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        MacAclRuleManagedObject(pSsmGlobalObjectManager)
    {
    }

    ExtMacAclRuleManagedObject::ExtMacAclRuleManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,
		const UI64 &ruleid, const UI32 &action, const string &srcMac, const string &srcMacMask, 
		const string &srcHost, const AddrType &srcType, const string &dstMac, const string &dstMacMask,
		const AddrType &dstType, const string &dstHost, const EtherType &appType, const UI16 &appTypInt, 
		const bool &iscount, const bool &islog)
        : PrismElement  			(pSsmGlobalObjectManager),
        PrismPersistableObject 		(ExtMacAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject 			(pSsmGlobalObjectManager),
        DcmManagedObject 			(pSsmGlobalObjectManager),
        MacAclRuleManagedObject		(pSsmGlobalObjectManager, ruleid, action, srcMac, srcMacMask, srcHost, srcType, iscount, islog),
        m_dstMac					(dstMac),
        m_dstMacMask				(dstMacMask),
        m_dstHost					(dstHost),
        m_dstType					(dstType),
        m_appType					(appType),
        m_appTypeInt				(appTypInt)
    {
    }

    ExtMacAclRuleManagedObject::~ExtMacAclRuleManagedObject ()
    {
    }

    string  ExtMacAclRuleManagedObject::getClassName()
    {
        return ("ExtMacAclRuleManagedObject");
    }

    void  ExtMacAclRuleManagedObject::setupAttributesForPersistence()
    {
    	UI32 appTypeDefault = (UI32)ETHER_TYPE_UNKNOWN;
        MacAclRuleManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_dstMac,"dstMac"));
        addPersistableAttribute (new AttributeString(&m_dstMacMask,true,"","dstMacMask"));
        addPersistableAttribute (new AttributeString(&m_dstHost,true,"","dstHost"));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_dstType),"dstType"));
        addPersistableAttribute (new AttributeUnion(&m_dstUnion, "dstUnion"));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_appType), appTypeDefault,"appType"));
        addPersistableAttribute (new AttributeUI16(&m_appTypeInt,"appTypeInt"));
        addPersistableAttribute (new AttributeUnion(&m_appTypeUnion,"appTypeUnion"));
		
		setUserTagForAttribute ("dstMacMask", mac_access_list_dst_mac_addr_mask);
        setUserTagForAttribute ("dstUnion", mac_access_list_dst);
        // setUserTagForAttribute ("srcHost", mac_access_list_srchost);
        setUserTagForAttribute ("dstHost", mac_access_list_dsthost);
        setUserTagForAttribute ("appTypeUnion", mac_access_list_ethertype);

    }

    void  ExtMacAclRuleManagedObject::setupAttributesForCreate()
    {
    	UI32 appTypeDefault = (UI32)ETHER_TYPE_UNKNOWN;
        addPersistableAttributeForCreate  (new AttributeString(&m_dstMac,"dstMac"));
        addPersistableAttributeForCreate  (new AttributeString(&m_dstMacMask,true,"","dstMacMask"));
        addPersistableAttributeForCreate  (new AttributeString(&m_dstHost,true,"","dstHost"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_dstType),"dstType"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_dstUnion, "dstUnion"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_appType),appTypeDefault,"appType"));
        addPersistableAttributeForCreate  (new AttributeUI16(&m_appTypeInt,"appTypeInt"));
        addPersistableAttributeForCreate  (new AttributeUnion(&m_appTypeUnion,"appTypeUnion"));

    }


    void  ExtMacAclRuleManagedObject::setDstMac(const string &dstMac)
    {
        m_dstMac  =  dstMac;
        m_dstUnion = "dstMac";
        m_dstType  =  ADDR_TYPE_NONE;
    }

    string  ExtMacAclRuleManagedObject::getDstMac() const
    {
        return (m_dstMac);
    }

    void  ExtMacAclRuleManagedObject::setAppType(const EtherType &appType)
    {
        m_appType  =  appType;
		m_appTypeUnion = "appType";
		m_appTypeInt = 0;
    }

    EtherType  ExtMacAclRuleManagedObject::getAppType() const
    {
        return (m_appType);
    }

    void  ExtMacAclRuleManagedObject::setDstHost(const string &dstHost)
    {
        m_dstHost  =  dstHost;
    }

    string  ExtMacAclRuleManagedObject::getDstHost() const
    {
        return (m_dstHost);
    }
    
    void  ExtMacAclRuleManagedObject::setDstType(const AddrType &dstType)
    {
        m_dstType  =  dstType;
        m_dstUnion = "dstType";
    }

    AddrType  ExtMacAclRuleManagedObject::getDstType() const
    {
        return (m_dstType);
    }
    void ExtMacAclRuleManagedObject::setDstUnion(const WaveUnion &val)
	{
		m_dstUnion = val;
	}

	WaveUnion ExtMacAclRuleManagedObject::getDstUnion() const
	{
		return (m_dstUnion);
	}
    
    void ExtMacAclRuleManagedObject::setAppTypeUnion(const WaveUnion &val)
	{
		m_appTypeUnion = val;
	}

	WaveUnion ExtMacAclRuleManagedObject::getAppTypeUnion() const
	{
		return (m_appTypeUnion);
	}

    void  ExtMacAclRuleManagedObject::setAppTypeInt(const UI16 &appType)
    {
        m_appTypeInt  =  appType;
		m_appTypeUnion = "appTypeInt";
		m_appType = ETHER_TYPE_UNKNOWN;
    }

    UI16  ExtMacAclRuleManagedObject::getAppTypeInt() const
    {
        return (m_appTypeInt);
    }

    void  ExtMacAclRuleManagedObject::setDstMacMask(const string &dstMacMask)
    {
        m_dstMacMask  =  dstMacMask;
    }

    string  ExtMacAclRuleManagedObject::getDstMacMask() const
    {
        return (m_dstMacMask);
    }


}
