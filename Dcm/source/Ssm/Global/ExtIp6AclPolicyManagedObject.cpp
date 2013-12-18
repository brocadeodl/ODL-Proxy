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

#include "Ssm/Global/ExtIp6AclPolicyManagedObject.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/Ip6AclRuleManagedObject.h"
#include "Ssm/Global/ExtIp6AclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

#include <algorithm>


namespace DcmNs
{

    ExtIp6AclPolicyManagedObject::ExtIp6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtIp6AclPolicyManagedObject::getClassName (), Ip6AclPolicyManagedObject::getClassName()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclPolicyManagedObject(pSsmGlobalObjectManager)
    {
    }

    ExtIp6AclPolicyManagedObject::ExtIp6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,
		const string &policyName,const vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > &rules)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtIp6AclPolicyManagedObject::getClassName (), Ip6AclPolicyManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclPolicyManagedObject(pSsmGlobalObjectManager, policyName),
        m_extRules (rules)
    {
    }

    ExtIp6AclPolicyManagedObject::~ExtIp6AclPolicyManagedObject ()
    {
    }

    string  ExtIp6AclPolicyManagedObject::getClassName()
    {
        return ("ExtIp6AclPolicyManagedObject");
    }

    void  ExtIp6AclPolicyManagedObject::setupAttributesForPersistence()
    {
		Ip6AclPolicyManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<ExtIp6AclRuleManagedObject>
			(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIp6AclRuleManagedObject::getClassName ()));
    }

    void  ExtIp6AclPolicyManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ExtIp6AclRuleManagedObject> 
			(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIp6AclRuleManagedObject::getClassName ()));
	}

	void ExtIp6AclPolicyManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  ExtIp6AclPolicyManagedObject::setRules(const vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > &rules)
    {
        m_extRules  =  rules;
    }

    vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> >  ExtIp6AclPolicyManagedObject::getRules() const
    {
        return (m_extRules);
    }

    void ExtIp6AclPolicyManagedObject::addRule(Ip6AclRuleManagedObject *ruleMO)
	{
		ExtIp6AclRuleManagedObject *extRule = dynamic_cast<ExtIp6AclRuleManagedObject *>(ruleMO);
		WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> eRule (extRule);
		m_extRules.push_back(eRule);
		
	}
	int ExtIp6AclPolicyManagedObject::deleteRule(const UI64 &ruleid)
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

	static bool cmp_ext_ruleid(WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> rule1,
		WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> rule2)
	{
		return (rule1->getRuleid() < rule2->getRuleid());
	}

	void ExtIp6AclPolicyManagedObject::sortRules()
	{
		sort(m_extRules.begin(), m_extRules.end(), cmp_ext_ruleid);
	}
}
