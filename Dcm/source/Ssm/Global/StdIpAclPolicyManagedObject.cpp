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

#include "Ssm/Global/StdIpAclPolicyManagedObject.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/StdIpAclRuleManagedObject.h"
#include "Ssm/Global/SsmGlobalTypes.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"
#include "Ssm/Global/IpAclRuleManagedObject.h"
#include <algorithm>


namespace DcmNs
{

    StdIpAclPolicyManagedObject::StdIpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIpAclPolicyManagedObject::getClassName (), IpAclPolicyManagedObject::getClassName()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        IpAclPolicyManagedObject(pSsmGlobalObjectManager)
    {
    }

    StdIpAclPolicyManagedObject::StdIpAclPolicyManagedObject (SsmGlobalObjectManager *pSsmGlobalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > &rules)
      : PrismElement  (pSsmGlobalObjectManager),
        PrismPersistableObject (StdIpAclPolicyManagedObject::getClassName (), IpAclPolicyManagedObject::getClassName ()),
        WaveManagedObject (pSsmGlobalObjectManager),
        DcmManagedObject (pSsmGlobalObjectManager),
        IpAclPolicyManagedObject(pSsmGlobalObjectManager, policyName),
        m_rules (rules)
    {
    }

    StdIpAclPolicyManagedObject::~StdIpAclPolicyManagedObject ()
    {
    }

    string  StdIpAclPolicyManagedObject::getClassName()
    {
        return ("StdIpAclPolicyManagedObject");
    }

    void  StdIpAclPolicyManagedObject::setupAttributesForPersistence()
    {
        //DcmManagedObject::setupAttributesForPersistence ();
		IpAclPolicyManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<StdIpAclRuleManagedObject>(&m_rules,"rules", getClassName (), getObjectId (), StdIpAclRuleManagedObject::getClassName ()));
    }

    void  StdIpAclPolicyManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<StdIpAclRuleManagedObject> (&m_rules,"rules", getClassName (), getObjectId (), StdIpAclRuleManagedObject::getClassName ()));
	}

	void StdIpAclPolicyManagedObject::setupKeys()
	{
        vector<string> keyName;
        keyName.push_back("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  StdIpAclPolicyManagedObject::setRules(const vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > &rules)
    {
        m_rules  =  rules;
    }

    vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> >  StdIpAclPolicyManagedObject::getRules() const
    {
        return (m_rules);
    }

    void StdIpAclPolicyManagedObject::addRule(IpAclRuleManagedObject *ruleMO)
	{
		StdIpAclRuleManagedObject *stdRule = dynamic_cast<StdIpAclRuleManagedObject *>(ruleMO);
		WaveManagedObjectPointer<StdIpAclRuleManagedObject> sRule (stdRule);
		m_rules.push_back(sRule);
		
	}
	int StdIpAclPolicyManagedObject::deleteRule(const UI64 &ruleid)
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

	static bool cmp_ruleid(WaveManagedObjectPointer<StdIpAclRuleManagedObject> rule1,
		WaveManagedObjectPointer<StdIpAclRuleManagedObject> rule2)
	{
		return (rule1->getRuleid() < rule2->getRuleid());
	}

	void StdIpAclPolicyManagedObject::sortRules()
	{
		sort(m_rules.begin(), m_rules.end(), cmp_ruleid);
	}
}
