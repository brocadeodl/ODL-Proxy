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
 *   Author : bpatel                                                     *
 **************************************************************************/

#include "Ssm/Global/ExtIpAclPolicyManagedObject.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/IpAclRuleManagedObject.h"
#include "Ssm/Global/ExtIpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

#include <algorithm>


namespace DcmNs
{

    ExtIpAclPolicyManagedObject::ExtIpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtIpAclPolicyManagedObject::getClassName (), IpAclPolicyManagedObject::getClassName()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        IpAclPolicyManagedObject(pSsmGlobalObjectManager)
    {
    }

    ExtIpAclPolicyManagedObject::ExtIpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,
		const string &policyName,const vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > &rules)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtIpAclPolicyManagedObject::getClassName (), IpAclPolicyManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        IpAclPolicyManagedObject(pSsmGlobalObjectManager, policyName),
        m_extRules (rules)
    {
    }

    ExtIpAclPolicyManagedObject::~ExtIpAclPolicyManagedObject ()
    {
    }

    string  ExtIpAclPolicyManagedObject::getClassName()
    {
        return ("ExtIpAclPolicyManagedObject");
    }

    void  ExtIpAclPolicyManagedObject::setupAttributesForPersistence()
    {
		IpAclPolicyManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<ExtIpAclRuleManagedObject>
			(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIpAclRuleManagedObject::getClassName ()));
    }

    void  ExtIpAclPolicyManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ExtIpAclRuleManagedObject> 
			(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIpAclRuleManagedObject::getClassName ()));
        setDisableValidations("extRules", true);
	}

	void ExtIpAclPolicyManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  ExtIpAclPolicyManagedObject::setRules(const vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > &rules)
    {
        m_extRules  =  rules;
    }

    vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> >  ExtIpAclPolicyManagedObject::getRules() const
    {
        return (m_extRules);
    }

    void ExtIpAclPolicyManagedObject::addRule(IpAclRuleManagedObject *ruleMO)
	{
		ExtIpAclRuleManagedObject *extRule = dynamic_cast<ExtIpAclRuleManagedObject *>(ruleMO);
		WaveManagedObjectPointer<ExtIpAclRuleManagedObject> eRule (extRule);
		m_extRules.push_back(eRule);
		
	}
	int ExtIpAclPolicyManagedObject::deleteRule(const UI64 &ruleid)
	{

		unsigned int i;

		for (i = 0; i < m_extRules.size(); i++) {
			if (m_extRules[i]->getRuleid() == ruleid) {
				trace(TRACE_LEVEL_INFO, "Extended Delete Rule: Found Rule");
				m_extRules.erase(m_extRules.begin() + i);
				return (0);
			}
		}

		return (1);

	}

	static bool cmp_ext_ruleid(WaveManagedObjectPointer<ExtIpAclRuleManagedObject> rule1,
		WaveManagedObjectPointer<ExtIpAclRuleManagedObject> rule2)
	{
		return (rule1->getRuleid() < rule2->getRuleid());
	}

	void ExtIpAclPolicyManagedObject::sortRules()
	{
		sort(m_extRules.begin(), m_extRules.end(), cmp_ext_ruleid);
	}
}

