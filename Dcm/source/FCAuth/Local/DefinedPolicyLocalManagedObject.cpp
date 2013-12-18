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

#include "FCAuth/Local/DefinedPolicyLocalManagedObject.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/DefinedPolicyMemberLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    DefinedPolicyLocalManagedObject::DefinedPolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (DefinedPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager)
    {
    }

    DefinedPolicyLocalManagedObject::DefinedPolicyLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const string &policyName,const vector<WaveManagedObjectPointer<DefinedPolicyMemberLocalManagedObject> > &policyMembers)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (DefinedPolicyLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager),
        m_policyName    (policyName),
        m_policyMembers    (policyMembers)
    {
    }

    DefinedPolicyLocalManagedObject::~DefinedPolicyLocalManagedObject ()
    {
    }

    string  DefinedPolicyLocalManagedObject::getClassName()
    {
        return ("DefinedPolicyLocalManagedObject");
    }

    void  DefinedPolicyLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<DefinedPolicyMemberLocalManagedObject>(&m_policyMembers,"policyMembers", getClassName (), getObjectId (), DefinedPolicyMemberLocalManagedObject::getClassName ()));

		setUserTagForAttribute ("policyName", brcd_fc_auth_policy);
    }

    void  DefinedPolicyLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_policyName,"policyName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<DefinedPolicyMemberLocalManagedObject>(&m_policyMembers,"policyMembers", getClassName (), getObjectId (), DefinedPolicyMemberLocalManagedObject::getClassName ()));

		vector<string> keyName;
        keyName.push_back ("policyName");
        setUserDefinedKeyCombination (keyName);
    }

    void  DefinedPolicyLocalManagedObject::setPolicyName(const string &policyName)
    {
        m_policyName  =  policyName;
    }

    string  DefinedPolicyLocalManagedObject::getPolicyName() const
    {
        return (m_policyName);
    }

    void  DefinedPolicyLocalManagedObject::setPolicyMembers(const vector<WaveManagedObjectPointer<DefinedPolicyMemberLocalManagedObject> > &policyMembers)
    {
        m_policyMembers  =  policyMembers;
    }

    vector<WaveManagedObjectPointer<DefinedPolicyMemberLocalManagedObject> >  DefinedPolicyLocalManagedObject::getPolicyMembers() const
    {
        return (m_policyMembers);
    }

	void DefinedPolicyLocalManagedObject::addPolicyMembers(DefinedPolicyMemberLocalManagedObject *policyMember)
    {
        m_policyMembers.push_back(policyMember);
    }

	int DefinedPolicyLocalManagedObject::removePolicyMembers(const string policyMember)
    {
        unsigned int i;

        for (i = 0; i < m_policyMembers.size(); i++) {
            if (m_policyMembers[i]->getPolicyMemberEntry() == policyMember) {
                trace(TRACE_LEVEL_INFO, "DefinedPolicyLocalManagedObject::removePolicyMembers: Found Entry");
                m_policyMembers.erase(m_policyMembers.begin() + i);
                return (0);
            }
        }

        return (1);
    }

	int DefinedPolicyLocalManagedObject::numOfLeftPolicyMembers(const string policyMember,
        int *maxListIndex)
    {
        unsigned int i;
        int num_of_left = 0;

        if (maxListIndex) {
            *maxListIndex = 0;
        }

        for (i = 0; i < m_policyMembers.size(); i++) {
            if (m_policyMembers[i]->getPolicyMemberEntry() == policyMember) {
                trace(TRACE_LEVEL_INFO, "DefinedPolicyLocalManagedObject::numOfLeftPolicyMembers: Found Entry");
            } else {
                // if (maxListIndex) {
                    if (m_policyMembers[i]->getListIndex() > *maxListIndex) {
                        *maxListIndex = m_policyMembers[i]->getListIndex();
                    }
                // }
                num_of_left++;
            }
        }

        return (num_of_left);
    }
}
