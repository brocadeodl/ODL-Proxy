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

#include "Ssm/Global/StdMacAclPolicyManagedObject.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/StdMacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

#include <algorithm>

using namespace std;

namespace DcmNs
{

    StdMacAclPolicyManagedObject::StdMacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdMacAclPolicyManagedObject::getClassName (), MacAclPolicyManagedObject::getClassName()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        MacAclPolicyManagedObject(pSsmGlobalObjectManager)
    {
    }

    StdMacAclPolicyManagedObject::StdMacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<StdMacAclRuleManagedObject> > &rules, const vector<ObjectId> &interfaces)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdMacAclPolicyManagedObject::getClassName (), MacAclPolicyManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        MacAclPolicyManagedObject(pSsmGlobalObjectManager, policyName, interfaces),
        m_rules (rules)
    {
    }

    StdMacAclPolicyManagedObject::~StdMacAclPolicyManagedObject ()
    {
    }

    string  StdMacAclPolicyManagedObject::getClassName()
    {
        return ("StdMacAclPolicyManagedObject");
    }

    void  StdMacAclPolicyManagedObject::setupAttributesForPersistence()
    {
        //DcmManagedObject::setupAttributesForPersistence ();
		MacAclPolicyManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<StdMacAclRuleManagedObject>(&m_rules,"rules", getClassName (), getObjectId (), StdMacAclRuleManagedObject::getClassName ()));
    }

    void  StdMacAclPolicyManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<StdMacAclRuleManagedObject> (&m_rules,"rules", getClassName (), getObjectId (), StdMacAclRuleManagedObject::getClassName ()));
	}

	void StdMacAclPolicyManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  StdMacAclPolicyManagedObject::setRules(const vector<WaveManagedObjectPointer<StdMacAclRuleManagedObject> > &rules)
    {
        m_rules  =  rules;
    }

    vector<WaveManagedObjectPointer<StdMacAclRuleManagedObject> >  StdMacAclPolicyManagedObject::getRules() const
    {
        return (m_rules);
    }

    void StdMacAclPolicyManagedObject::addRule(MacAclRuleManagedObject *ruleMO)
	{
		StdMacAclRuleManagedObject *stdRule = dynamic_cast<StdMacAclRuleManagedObject *>(ruleMO);
		WaveManagedObjectPointer<StdMacAclRuleManagedObject> sRule (stdRule);
		m_rules.push_back(sRule);
		
	}
	int StdMacAclPolicyManagedObject::deleteRule(const UI64 &ruleid)
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

	static bool cmp_ruleid(WaveManagedObjectPointer<StdMacAclRuleManagedObject> rule1,
		WaveManagedObjectPointer<StdMacAclRuleManagedObject> rule2)
	{
		return (rule1->getRuleid() < rule2->getRuleid());
	}

	void StdMacAclPolicyManagedObject::sortRules()
	{
		sort(m_rules.begin(), m_rules.end(), cmp_ruleid);
	}
}
