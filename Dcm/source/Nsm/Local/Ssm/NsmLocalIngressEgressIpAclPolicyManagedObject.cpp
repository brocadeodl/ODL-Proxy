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

#include "Nsm/Local/Ssm/NsmLocalIngressEgressIpAclPolicyManagedObject.h"


namespace DcmNs
{

    NsmLocalIngressEgressIpAclPolicyManagedObject::NsmLocalIngressEgressIpAclPolicyManagedObject
		(WaveObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

	NsmLocalIngressEgressIpAclPolicyManagedObject::NsmLocalIngressEgressIpAclPolicyManagedObject 
		(WaveObjectManager *pNsmLocalObjectManager, const ObjectId &ingressEgressIpAcl,
		 const ssmAcl_binding_direction &direction) 
		: PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
		m_ingressEgressIpAcl	(ingressEgressIpAcl),
		m_ipAcldirection		(direction)
	{
	}

    NsmLocalIngressEgressIpAclPolicyManagedObject::~NsmLocalIngressEgressIpAclPolicyManagedObject()
    {
    }

    string  NsmLocalIngressEgressIpAclPolicyManagedObject::getClassName()
    {
        return ("NsmLocalIngressEgressIpAclPolicyManagedObject");
    }

    void  NsmLocalIngressEgressIpAclPolicyManagedObject::setupAttributesForPersistence()
    {
		DcmLocalManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_ingressEgressIpAcl,"ingressEgressIpAcl", IpAclPolicyManagedObject::getClassName(), true));
		addPersistableAttribute (new AttributeEnum((UI32 *)&m_ipAcldirection, "ipAclDirection", ip_access_list_ip_direction));
    	setUserTagForAttribute ("ingressEgressIpAcl", ip_access_list_ip_access_list);
	}

    void  NsmLocalIngressEgressIpAclPolicyManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_ingressEgressIpAcl,"ingressEgressIpAcl", IpAclPolicyManagedObject::getClassName(), true));
		addPersistableAttributeForCreate (new AttributeEnum((UI32 *)&m_ipAcldirection, "ipAclDirection"));
    	
	}
    
	void NsmLocalIngressEgressIpAclPolicyManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("ingressEgressIpAcl");
        keyName.push_back("ipAclDirection");
        setUserDefinedKeyCombination (keyName);
    }
	
	void NsmLocalIngressEgressIpAclPolicyManagedObject::setDirection(ssmAcl_binding_direction &direction)
	{
		m_ipAcldirection = direction;
	}
		
	ssmAcl_binding_direction NsmLocalIngressEgressIpAclPolicyManagedObject::getDirection () const
	{
		return ((ssmAcl_binding_direction)(m_ipAcldirection));
	}

    ObjectId  NsmLocalIngressEgressIpAclPolicyManagedObject::getIngressEgressIpAcl() const
    {
        return (m_ingressEgressIpAcl);
    }
    
	void  NsmLocalIngressEgressIpAclPolicyManagedObject::setIngressEgressIpAcl(const ObjectId &ingressEgressIpAcl)
    {
        m_ingressEgressIpAcl = ingressEgressIpAcl;
    }
	
		
} // DcmNs
