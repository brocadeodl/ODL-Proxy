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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Bhavin                                                      *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "Nsm/Local/Ssm/NsmLocalIngressEgressMacAclPolicyManagedObject.h"


namespace DcmNs
{

    NsmLocalIngressEgressMacAclPolicyManagedObject::NsmLocalIngressEgressMacAclPolicyManagedObject
		(WaveObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

	NsmLocalIngressEgressMacAclPolicyManagedObject::NsmLocalIngressEgressMacAclPolicyManagedObject 
		(WaveObjectManager *pNsmLocalObjectManager, const ObjectId &ingressEgressMacAcl,
		 const ssmAcl_binding_direction &direction) 
		: PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
		m_ingressEgressMacAcl	(ingressEgressMacAcl),
		m_macAcldirection		(direction)
	{
	}

    NsmLocalIngressEgressMacAclPolicyManagedObject::~NsmLocalIngressEgressMacAclPolicyManagedObject()
    {
    }

    string  NsmLocalIngressEgressMacAclPolicyManagedObject::getClassName()
    {
        return ("NsmLocalIngressEgressMacAclPolicyManagedObject");
    }

    void  NsmLocalIngressEgressMacAclPolicyManagedObject::setupAttributesForPersistence()
    {
		DcmLocalManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_ingressEgressMacAcl,"ingressEgressMacAcl", MacAclPolicyManagedObject::getClassName(), true));
		addPersistableAttribute (new AttributeEnum((UI32 *)&m_macAcldirection, "macAclDirection", mac_access_list_mac_direction));
    	setUserTagForAttribute ("ingressEgressMacAcl", mac_access_list_mac_access_list);
	}

    void  NsmLocalIngressEgressMacAclPolicyManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_ingressEgressMacAcl,"ingressEgressMacAcl", MacAclPolicyManagedObject::getClassName(), true));
		addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_macAcldirection, "macAclDirection"));
    	
	}
    
	void NsmLocalIngressEgressMacAclPolicyManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("ingressEgressMacAcl");
        keyName.push_back("macAclDirection");
        setUserDefinedKeyCombination (keyName);
    }
	
	void NsmLocalIngressEgressMacAclPolicyManagedObject::setDirection(ssmAcl_binding_direction &direction)
	{
		m_macAcldirection = direction;
	}
		
	ssmAcl_binding_direction NsmLocalIngressEgressMacAclPolicyManagedObject::getDirection () const
	{
		return ((ssmAcl_binding_direction)(m_macAcldirection));
	}

    ObjectId  NsmLocalIngressEgressMacAclPolicyManagedObject::getIngressEgressMacAcl() const
    {
        return (m_ingressEgressMacAcl);
    }
    
	void  NsmLocalIngressEgressMacAclPolicyManagedObject::setIngressEgressMacAcl(const ObjectId &ingressEgressMacAcl)
    {
        m_ingressEgressMacAcl = ingressEgressMacAcl;
    }
	
		
} // DcmNs
