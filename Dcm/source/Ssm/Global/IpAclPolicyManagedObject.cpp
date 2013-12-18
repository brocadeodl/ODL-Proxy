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

#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/IpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "vcs.h"

namespace DcmNs
{

    IpAclPolicyManagedObject::IpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (IpAclPolicyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager)
    {
    }

    IpAclPolicyManagedObject::IpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (IpAclPolicyManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        m_policyName    (policyName)
    {
    }

    IpAclPolicyManagedObject::~IpAclPolicyManagedObject ()
    {
    }

    string  IpAclPolicyManagedObject::getClassName()
    {
        return ("IpAclPolicyManagedObject");
    }

    void  IpAclPolicyManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_policyName,"policyName"));

        setUserTagForAttribute ("policyName", ip_access_list_name);

    }

    void  IpAclPolicyManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_policyName,"policyName"));
	}

	void IpAclPolicyManagedObject::setupKeys()
	{

        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  IpAclPolicyManagedObject::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  IpAclPolicyManagedObject::getPolicyName() const
    {
        return (m_policyName);
    }

    void IpAclPolicyManagedObject::addRule(IpAclRuleManagedObject *ruleMO)
	{
		trace(TRACE_LEVEL_FATAL, "addRule: This function should never be called");
		prismAssert(false, __FILE__, __LINE__);
	}
    int IpAclPolicyManagedObject::deleteRule(const UI64 &ruleid)
	{
		trace(TRACE_LEVEL_FATAL, "deleteRule: This function should never be called");
		prismAssert(false, __FILE__, __LINE__);
		return (0);
	}


    

}
