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

#include "FCAuth/Local/ActivePolicyMemberLocalManagedObject.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/FCAuthTypes.h"

namespace DcmNs
{

    ActivePolicyMemberLocalManagedObject::ActivePolicyMemberLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (ActivePolicyMemberLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager)
    {
    }

    ActivePolicyMemberLocalManagedObject::ActivePolicyMemberLocalManagedObject (FCAuthLocalObjectManager *pFCAuthLocalObjectManager,const string &policyMemberEntry,const SI32 &listIndex)
        : PrismElement  (pFCAuthLocalObjectManager),
        PrismPersistableObject (ActivePolicyMemberLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFCAuthLocalObjectManager),
        DcmManagedObject (pFCAuthLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFCAuthLocalObjectManager),
        m_policyMemberEntry    (policyMemberEntry),
        m_listIndex    (listIndex)
    {
    }

    ActivePolicyMemberLocalManagedObject::~ActivePolicyMemberLocalManagedObject ()
    {
    }

    string  ActivePolicyMemberLocalManagedObject::getClassName()
    {
        return ("ActivePolicyMemberLocalManagedObject");
    }

    void  ActivePolicyMemberLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_policyMemberEntry,"policyMemberEntry"));
        addPersistableAttribute (new AttributeSI32(&m_listIndex,"listIndex"));

		setUserTagForAttribute ("policyMemberEntry", brcd_fc_auth_member);
    }

    void  ActivePolicyMemberLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_policyMemberEntry,"policyMemberEntry"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_listIndex,"listIndex"));

		vector<string> keyName;
        keyName.push_back ("policyMemberEntry");
        setUserDefinedKeyCombination (keyName);
    }

    void  ActivePolicyMemberLocalManagedObject::setPolicyMemberEntry(const string &policyMemberEntry)
    {
        m_policyMemberEntry  =  policyMemberEntry;
    }

    string  ActivePolicyMemberLocalManagedObject::getPolicyMemberEntry() const
    {
        return (m_policyMemberEntry);
    }

    void  ActivePolicyMemberLocalManagedObject::setListIndex(const SI32 &listIndex)
    {
        m_listIndex  =  listIndex;
    }

    SI32  ActivePolicyMemberLocalManagedObject::getListIndex() const
    {
        return (m_listIndex);
    }

}
