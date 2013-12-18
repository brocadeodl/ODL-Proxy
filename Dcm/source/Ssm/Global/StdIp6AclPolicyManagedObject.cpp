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
 *   Author : Hari Om                                                      *
 **************************************************************************/

#include "Ssm/Global/StdIp6AclPolicyManagedObject.h"
#include "Ssm/Global/Ip6AclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/StdIp6AclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"
#include "Ssm/Global/Ip6AclRuleManagedObject.h"
#include <algorithm>


namespace DcmNs
{

    StdIp6AclPolicyManagedObject::StdIp6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIp6AclPolicyManagedObject::getClassName (), Ip6AclPolicyManagedObject::getClassName()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclPolicyManagedObject(pSsmGlobalObjectManager)
    {
    }

    StdIp6AclPolicyManagedObject::StdIp6AclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > &rules)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIp6AclPolicyManagedObject::getClassName (), Ip6AclPolicyManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        Ip6AclPolicyManagedObject(pSsmGlobalObjectManager, policyName),
        m_rules (rules)
    {
    }

    StdIp6AclPolicyManagedObject::~StdIp6AclPolicyManagedObject ()
    {
    }

    string  StdIp6AclPolicyManagedObject::getClassName()
    {
        return ("StdIp6AclPolicyManagedObject");
    }

    void  StdIp6AclPolicyManagedObject::setupAttributesForPersistence()
    {
        //DcmManagedObject::setupAttributesForPersistence ();
		Ip6AclPolicyManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<StdIp6AclRuleManagedObject>(&m_rules,"rules", getClassName (), getObjectId (), StdIp6AclRuleManagedObject::getClassName ()));
    }

    void  StdIp6AclPolicyManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<StdIp6AclRuleManagedObject> (&m_rules,"rules", getClassName (), getObjectId (), StdIp6AclRuleManagedObject::getClassName ()));
	}

	void StdIp6AclPolicyManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  StdIp6AclPolicyManagedObject::setRules(const vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > &rules)
    {
        m_rules  =  rules;
    }

    vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> >  StdIp6AclPolicyManagedObject::getRules() const
    {
        return (m_rules);
    }

    void StdIp6AclPolicyManagedObject::addRule(Ip6AclRuleManagedObject *ruleMO)
	{
		StdIp6AclRuleManagedObject *stdRule = dynamic_cast<StdIp6AclRuleManagedObject *>(ruleMO);
		WaveManagedObjectPointer<StdIp6AclRuleManagedObject> sRule (stdRule);
		m_rules.push_back(sRule);
		
	}
	int StdIp6AclPolicyManagedObject::deleteRule(const UI64 &ruleid)
	{

		unsigned int i;

		for (i = 0; i < m_rules.size(); i++) {
			if (m_rules[i]->getRuleid() == ruleid) {
				trace(TRACE_LEVEL_INFO, string("Standard Delete Rule: Found Rule") + ruleid);
				m_rules.erase(m_rules.begin() + i);
				return (0);
			}
		}

		return (1);

	}

	static bool cmp_ruleid(WaveManagedObjectPointer<StdIp6AclRuleManagedObject> rule1,
		WaveManagedObjectPointer<StdIp6AclRuleManagedObject> rule2)
	{
		return (rule1->getRuleid() < rule2->getRuleid());
	}

	void StdIp6AclPolicyManagedObject::sortRules()
	{
		sort(m_rules.begin(), m_rules.end(), cmp_ruleid);
	}
}
