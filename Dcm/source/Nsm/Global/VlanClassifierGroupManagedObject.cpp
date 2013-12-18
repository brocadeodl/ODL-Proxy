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
 *   Author : rmuthigi                                                     *
 **************************************************************************/
#if 0
#include "Nsm/Global/VlanClassifierGroupManagedObject.h"
#include "Nsm/Global/VlanClassifierRulesManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    VlanClassifierGroupManagedObject::VlanClassifierGroupManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanClassifierGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
    }

    VlanClassifierGroupManagedObject::VlanClassifierGroupManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &groupId)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanClassifierGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_groupId    (groupId)
    {
    }

    VlanClassifierGroupManagedObject::~VlanClassifierGroupManagedObject ()
    {
    }

    string  VlanClassifierGroupManagedObject::getClassName()
    {
        return ("VlanClassifierGroupManagedObject");
    }

    void  VlanClassifierGroupManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_groupId,"groupId"));
	setUserTagForAttribute("groupId", logical_switch_groupid);
    }

    void  VlanClassifierGroupManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_groupId,"groupId", logical_switch_groupid));
	
	vector<string > keyName;
	keyName.push_back ("groupId");
	setUserDefinedKeyCombination (keyName);
	
    }

    void  VlanClassifierGroupManagedObject::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }

    UI32  VlanClassifierGroupManagedObject::getGroupId() const
    {
        return (m_groupId);
    }

}
#endif
