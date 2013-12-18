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

#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "vcs.h"

namespace DcmNs
{

    MacAclPolicyManagedObject::MacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (MacAclPolicyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager)
    {
    }

    MacAclPolicyManagedObject::MacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName, const vector<ObjectId> &interfaces)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (MacAclPolicyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        m_policyName    (policyName),
        m_interfaces (interfaces)
    {
    }

    MacAclPolicyManagedObject::~MacAclPolicyManagedObject ()
    {
    }

    string  MacAclPolicyManagedObject::getClassName()
    {
        return ("MacAclPolicyManagedObject");
    }

    void  MacAclPolicyManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttribute (new AttributeObjectIdVectorAssociation(&m_interfaces,"interfaces", getClassName (), getObjectId (), MacAclRuleManagedObject::getClassName ()));

        setUserTagForAttribute ("policyName", mac_access_list_name);

    }

    void  MacAclPolicyManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttributeForCreate  (new AttributeObjectIdVectorAssociation(&m_interfaces,"interfaces", getClassName (), getObjectId (), MacAclRuleManagedObject::getClassName ()));
	}

	void MacAclPolicyManagedObject::setupKeys()
	{

        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  MacAclPolicyManagedObject::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  MacAclPolicyManagedObject::getPolicyName() const
    {
        return (m_policyName);
    }

    void  MacAclPolicyManagedObject::setInterfaces(const vector<ObjectId> &interfaces)
    {
        m_interfaces  =  interfaces;
    }

    vector<ObjectId>  MacAclPolicyManagedObject::getInterfaces() const
    {
        return (m_interfaces);
    }

    void MacAclPolicyManagedObject::addRule(MacAclRuleManagedObject *ruleMO)
	{
		trace(TRACE_LEVEL_FATAL, "addRule: This function should never be called");
		prismAssert(false, __FILE__, __LINE__);
	}
    int MacAclPolicyManagedObject::deleteRule(const UI64 &ruleid)
	{
		trace(TRACE_LEVEL_FATAL, "deleteRule: This function should never be called");
		prismAssert(false, __FILE__, __LINE__);
		return (0);
	}


    

}
