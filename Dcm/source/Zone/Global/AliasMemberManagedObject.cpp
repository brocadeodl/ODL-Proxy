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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Zone/Global/AliasMemberManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/

namespace DcmNs
{

    AliasMemberManagedObject::AliasMemberManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (AliasMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    AliasMemberManagedObject::AliasMemberManagedObject (ZoneObjectManager *pZoneObjectManager,const string &aliasEntry,const UI32 &listIndex)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (AliasMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_aliasEntry    (aliasEntry),
        m_listIndex    (listIndex)
    {
    }

    AliasMemberManagedObject::~AliasMemberManagedObject ()
    {
    }

    string  AliasMemberManagedObject::getClassName()
    {
        return ("AliasMemberManagedObject");
    }

    void  AliasMemberManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_aliasEntry,"aliasEntry"));
        addPersistableAttribute (new AttributeUI32(&m_listIndex,"listIndex"));

	    setUserTagForAttribute ("aliasEntry", brocade_zone_alias_entry_name);
    }

    void  AliasMemberManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_aliasEntry,"aliasEntry"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_listIndex,"listIndex"));

		vector<string> keyName;
	    keyName.push_back ("aliasEntry");
	    setUserDefinedKeyCombination (keyName);
    }

    void  AliasMemberManagedObject::setAliasEntry(const string &aliasEntry)
    {
        m_aliasEntry  =  aliasEntry;
    }

    string  AliasMemberManagedObject::getAliasEntry() const
    {
        return (m_aliasEntry);
    }

    void  AliasMemberManagedObject::setListIndex(const UI32 &listIndex)
    {
        m_listIndex  =  listIndex;
    }

    UI32  AliasMemberManagedObject::getListIndex() const
    {
        return (m_listIndex);
    }

}
