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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#include "FCAuth/Local/ActivePolicyLocalManagedObject.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/ActivePolicyMemberLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    ActivePolicyLocalManagedObject::ActivePolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (ActivePolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager)
    {
    }

    ActivePolicyLocalManagedObject::ActivePolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > &PolicyMembers)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (ActivePolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager),
        m_policyName    (policyName),
        m_PolicyMembers    (PolicyMembers)
    {
    }

    ActivePolicyLocalManagedObject::~ActivePolicyLocalManagedObject ()
    {
    }

    string  ActivePolicyLocalManagedObject::getClassName()
    {
        return ("ActivePolicyLocalManagedObject");
    }

    void  ActivePolicyLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<ActivePolicyMemberLocalManagedObject>(&m_PolicyMembers,"PolicyMembers", getClassName (), getObjectId (), ActivePolicyMemberLocalManagedObject::getClassName ()));

		setUserTagForAttribute ("policyName", brcd_fc_auth_policy);
    }

    void  ActivePolicyLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ActivePolicyMemberLocalManagedObject>(&m_PolicyMembers,"PolicyMembers", getClassName (), getObjectId (), ActivePolicyMemberLocalManagedObject::getClassName ()));

		vector<string> keyName;
        keyName.push_back ("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  ActivePolicyLocalManagedObject::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  ActivePolicyLocalManagedObject::getPolicyName() const
    {
        return (m_policyName);
    }

    void  ActivePolicyLocalManagedObject::setPolicyMembers(const vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > &PolicyMembers)
    {
        m_PolicyMembers  =  PolicyMembers;
    }

    vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> >  ActivePolicyLocalManagedObject::getPolicyMembers() const
    {
        return (m_PolicyMembers);
    }

	void ActivePolicyLocalManagedObject::addPolicyMembers(ActivePolicyMemberLocalManagedObject *policyMember)
    {
        m_PolicyMembers.push_back(policyMember);
    }

    int ActivePolicyLocalManagedObject::removePolicyMembers(const string policyMember)
    {
        unsigned int i;

        for (i = 0; i < m_PolicyMembers.size(); i++) {
            if (m_PolicyMembers[i]->getPolicyMemberEntry() == policyMember) {
                trace(TRACE_LEVEL_INFO, "ActivePolicyLocalManagedObject::removePolicyMembers: Found Entry");
                m_PolicyMembers.erase(m_PolicyMembers.begin() + i);
                return (0);
            }
        }
        
        return (1);
    }

	int ActivePolicyLocalManagedObject::numOfLeftPolicyMembers(const string policyMember,
        int *maxListIndex)
    {
        unsigned int i;
        int num_of_left = 0;

        if (maxListIndex) {
            *maxListIndex = 0;
        }
        
        for (i = 0; i < m_PolicyMembers.size(); i++) {
            if (m_PolicyMembers[i]->getPolicyMemberEntry() == policyMember) {
                trace(TRACE_LEVEL_INFO, "ActivePolicyLocalManagedObject::numOfLeftPolicyMembers: Found Entry");
            } else {
                // if (maxListIndex) {
                    if (m_PolicyMembers[i]->getListIndex() > *maxListIndex) {
                        *maxListIndex = m_PolicyMembers[i]->getListIndex();
                    }
                // }
                num_of_left++;
            }
        }
        
        return (num_of_left);
    }
}
