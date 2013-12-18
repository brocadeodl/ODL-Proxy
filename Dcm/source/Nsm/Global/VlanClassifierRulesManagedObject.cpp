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
 *   Author : rmuthigi                                                     *
 **************************************************************************/
//#include "Nsm/ClientInterface/VlanClassifierCommon.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "vcs.h"

namespace DcmNs
{

	VlanClassifierRulesManagedObject::VlanClassifierRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
		: PrismElement  (pNsmGlobalObjectManager),
		PrismPersistableObject (VlanClassifierRulesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pNsmGlobalObjectManager),
		DcmManagedObject (pNsmGlobalObjectManager)
		{
			m_configTypeChoice = 0;
			m_encap = ENCAP_TYPE_UNKNOWN;
			m_protoValEnum = PROTO_TYPE_UNKNOWN;
			m_ruleId = 0;
		}

	VlanClassifierRulesManagedObject::VlanClassifierRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &ruleId,const string &mac,const ProtoType &protoValEnum,const string &protoValStr,const WaveUnion &appType,const EncapType &encap)
		: PrismElement  (pNsmGlobalObjectManager),
		PrismPersistableObject (VlanClassifierRulesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pNsmGlobalObjectManager),
		DcmManagedObject (pNsmGlobalObjectManager),
		m_ruleId    (ruleId),
		m_mac    (mac),
		m_protoValEnum    (protoValEnum),
		m_protoValStr     (protoValStr),
		m_appType     (appType),
		m_encap    (encap)
		{
			m_configTypeChoice = 0;
		}

	VlanClassifierRulesManagedObject::~VlanClassifierRulesManagedObject ()
	{
	}

	string  VlanClassifierRulesManagedObject::getClassName()
	{
		return ("VlanClassifierRulesManagedObject");
	}

	void  VlanClassifierRulesManagedObject::setupAttributesForPersistence()
	{
		UI32    defaultVal_proto = PROTO_TYPE_UNKNOWN;
		UI32    defaultVal_encap = ENCAP_TYPE_UNKNOWN ;
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_ruleId,"ruleId"));
		addPersistableAttribute (new AttributeString(&m_mac,"mac", brocade_vlan_address));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_protoValEnum), defaultVal_proto,"protoval"));
		addPersistableAttribute (new AttributeString(&m_protoValStr,"protovalstr"));
		addPersistableAttribute (new AttributeUnion(&m_appType,"appTypeUnion", brocade_vlan_proto_val));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_encap), defaultVal_encap,"encap", brocade_vlan_encap));
		addPersistableAttribute (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
		setUserTagForAttribute("ruleId", brocade_vlan_ruleid);
	}

	void  VlanClassifierRulesManagedObject::setupAttributesForCreate()
	{
		DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeUI32(&m_ruleId,"ruleId"));
		addPersistableAttributeForCreate  (new AttributeString(&m_mac,"mac"));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_protoValEnum),"protoVal"));
		addPersistableAttributeForCreate (new AttributeString(&m_protoValStr,"protoValStr"));
		addPersistableAttributeForCreate (new AttributeUnion(&m_appType,"appTypeUnion"));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_encap),"encap"));
		addPersistableAttributeForCreate (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
		vector<string > keyName;
		keyName.push_back ("ruleId");
		setUserDefinedKeyCombination (keyName);
	}

	UI32 VlanClassifierRulesManagedObject::getCase (const UI32 &userChoiceWyserTag)
	{
		trace (TRACE_LEVEL_DEBUG, "VlanClassfierRuleManagedObject::getCase: Entering ...");

		if (VLAN_CLASSIFIER_MAC == m_configTypeChoice)
		{
			NSM_PLUG_DBG("config choice = VLAN_CLASSIFIER_MAC");
			return brocade_vlan_mac;
		}
		else if (VLAN_CLASSFIER_PROTO == m_configTypeChoice)
		{
			NSM_PLUG_DBG("config choice = VLAN_CLASSIFIER_PROTO");
			return brocade_vlan_proto;
		}
		NSM_PLUG_DBG("Invalid choice");
		return 0;
	}


	void  VlanClassifierRulesManagedObject::setRuleId(const UI32 &ruleId)
	{
		m_ruleId  =  ruleId;
	}

	UI32  VlanClassifierRulesManagedObject::getRuleId() const
	{
		return (m_ruleId);
	}

	void  VlanClassifierRulesManagedObject::setMac(const string &mac)
	{
		m_mac  =  mac;
	}

	string  VlanClassifierRulesManagedObject::getMac() const
	{
		return (m_mac);
	}

	void  VlanClassifierRulesManagedObject::setProtoValEnum(const ProtoType &protoValEnum)
	{
		m_protoValEnum  =  protoValEnum;
		//	m_appType = "protoval";
	}

	ProtoType  VlanClassifierRulesManagedObject::getProtoValEnum() const
	{
		return (m_protoValEnum);
	}

	void  VlanClassifierRulesManagedObject::setProtoValStr(const string &protoValStr)
	{
		m_protoValStr =  protoValStr;
		//	m_appType = "protovalstr";
	}

	string VlanClassifierRulesManagedObject::getProtoValStr() const
	{
		return (m_protoValStr);
	}

	void  VlanClassifierRulesManagedObject::setAppType(const WaveUnion &appType)
	{
		m_appType  =  appType;
	}

	WaveUnion  VlanClassifierRulesManagedObject::getAppType() const
	{
		return (m_appType);
	}     

	void  VlanClassifierRulesManagedObject::setEncap(const EncapType &encap)
	{
		m_encap  =  encap;
	}

	EncapType  VlanClassifierRulesManagedObject::getEncap() const
	{
		return (m_encap);
	}
	UI32 VlanClassifierRulesManagedObject::getConfigTypeChoice () const
	{
		return (m_configTypeChoice);
	}

	void VlanClassifierRulesManagedObject::setConfigTypeChoice (const UI32 &configTypeChoice)
	{
		m_configTypeChoice = configTypeChoice;
	}

}
