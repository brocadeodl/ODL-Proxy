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
#include "Nsm/Global/VlanClassifierGroupAssocRulesManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    VlanClassifierGroupAssocRulesManagedObject::VlanClassifierGroupAssocRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanClassifierGroupAssocRulesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_dummy = 0;
		m_groupId = 0;
		m_oper = VLAN_UNKNOWN_ACTION;
    }

    VlanClassifierGroupAssocRulesManagedObject::VlanClassifierGroupAssocRulesManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &groupId,const  vlanClassifierOperation &oper,const UI32 &dummy,const ObjectId &ruleId)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanClassifierGroupAssocRulesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_groupId    (groupId),
	m_oper	    (oper),
	m_dummy     (dummy), 	
        m_ruleId    (ruleId)
    {
    }

    VlanClassifierGroupAssocRulesManagedObject::~VlanClassifierGroupAssocRulesManagedObject ()
    {
    }

    string  VlanClassifierGroupAssocRulesManagedObject::getClassName()
    {
        return ("VlanClassifierGroupAssocRulesManagedObject");
    }

    void  VlanClassifierGroupAssocRulesManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI32(&m_groupId,"groupId"));
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_oper),"oper"));
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_dummy),"dummy"));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_ruleId,"ruleId", VlanClassifierRulesManagedObject::getClassName ()));

	setUserTagForAttribute("groupId", brocade_vlan_groupid);
	setUserTagForAttribute("oper", brocade_vlan_oper);
	setUserTagForAttribute("dummy", brocade_vlan_rule_name);
	setUserTagForAttribute("ruleId", brocade_vlan_ruleid);

    }

    void  VlanClassifierGroupAssocRulesManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate  (new AttributeUI32(&m_groupId,"groupId", brocade_vlan_groupid));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_oper),"oper"));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_dummy),"dummy"));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_ruleId,"ruleId", VlanClassifierRulesManagedObject::getClassName ()));
	
	vector<string> keyName;
	keyName.push_back ("groupId");
	setUserDefinedKeyCombination (keyName);
	
	keyName.push_back ("oper");
	setUserDefinedKeyCombination (keyName);
	
	keyName.push_back ("dummy");
	setUserDefinedKeyCombination (keyName);

	keyName.push_back ("ruleId");
	setUserDefinedKeyCombination (keyName);


    }

    void  VlanClassifierGroupAssocRulesManagedObject::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  VlanClassifierGroupAssocRulesManagedObject::getGroupId() const
    {
        return (m_groupId);
    }

    void  VlanClassifierGroupAssocRulesManagedObject::setRuleId(const ObjectId &ruleId)
    {
        m_ruleId  =  ruleId;
    }

    ObjectId  VlanClassifierGroupAssocRulesManagedObject::getRuleId() const
    {
        return (m_ruleId);
    }
    void  VlanClassifierGroupAssocRulesManagedObject::setoper(const vlanClassifierOperation  &oper)
    {
        m_oper  =  oper;
    }

    vlanClassifierOperation  VlanClassifierGroupAssocRulesManagedObject::getoper() const
    {
        return (m_oper);
    }
    void  VlanClassifierGroupAssocRulesManagedObject::setdummy(const UI32 &dummy)
    {
        m_dummy  =  dummy;
    }

    UI32 VlanClassifierGroupAssocRulesManagedObject::getdummy() const
    {
        return (m_dummy);
    }

     	

}

