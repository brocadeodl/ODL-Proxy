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

#include "Ssm/Global/ExtMacAclPolicyManagedObject.h"
#include "Ssm/Global/MacAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/MacAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

#include <algorithm>

using namespace std;

namespace DcmNs
{

    ExtMacAclPolicyManagedObject::ExtMacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtMacAclPolicyManagedObject::getClassName (), MacAclPolicyManagedObject::getClassName()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        MacAclPolicyManagedObject(pSsmGlobalObjectManager)
    {
    }

    ExtMacAclPolicyManagedObject::ExtMacAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > &rules, const vector<ObjectId> &interfaces)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (ExtMacAclPolicyManagedObject::getClassName (), MacAclPolicyManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        MacAclPolicyManagedObject(pSsmGlobalObjectManager, policyName, interfaces),
        m_extRules (rules)
    {
    }

    ExtMacAclPolicyManagedObject::~ExtMacAclPolicyManagedObject ()
    {
    }

    string  ExtMacAclPolicyManagedObject::getClassName()
    {
        return ("ExtMacAclPolicyManagedObject");
    }

    void  ExtMacAclPolicyManagedObject::setupAttributesForPersistence()
    {
		MacAclPolicyManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<ExtMacAclRuleManagedObject>(&m_extRules,"extRules", getClassName (), getObjectId (), ExtMacAclRuleManagedObject::getClassName ()));
    }

    void  ExtMacAclPolicyManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ExtMacAclRuleManagedObject> (&m_extRules,"extRules", getClassName (), getObjectId (), ExtMacAclRuleManagedObject::getClassName ()));
	}

	void ExtMacAclPolicyManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  ExtMacAclPolicyManagedObject::setRules(const vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> > &rules)
    {
        m_extRules  =  rules;
    }

    vector<WaveManagedObjectPointer<ExtMacAclRuleManagedObject> >  ExtMacAclPolicyManagedObject::getRules() const
    {
        return (m_extRules);
    }

    void ExtMacAclPolicyManagedObject::addRule(MacAclRuleManagedObject *ruleMO)
	{
		ExtMacAclRuleManagedObject *extRule = dynamic_cast<ExtMacAclRuleManagedObject *>(ruleMO);
		WaveManagedObjectPointer<ExtMacAclRuleManagedObject> eRule (extRule);
		m_extRules.push_back(eRule);
		
	}
	int ExtMacAclPolicyManagedObject::deleteRule(const UI64 &ruleid)
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

	static bool cmp_ext_ruleid(WaveManagedObjectPointer<ExtMacAclRuleManagedObject> rule1,
		WaveManagedObjectPointer<ExtMacAclRuleManagedObject> rule2)
	{
		return (rule1->getRuleid() < rule2->getRuleid());
	}

	void ExtMacAclPolicyManagedObject::sortRules()
	{
		sort(m_extRules.begin(), m_extRules.end(), cmp_ext_ruleid);
	}
}
