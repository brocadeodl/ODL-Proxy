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

#include "Zone/Global/EffectiveZoneMemberManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
//#include "brocade-zone.h"

namespace DcmNs
{

    EffectiveZoneMemberManagedObject::EffectiveZoneMemberManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (EffectiveZoneMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    EffectiveZoneMemberManagedObject::EffectiveZoneMemberManagedObject (ZoneObjectManager *pZoneObjectManager,const string &effectiveZoneEntry,const UI32 &listIndex)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (EffectiveZoneMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_effectiveZoneEntry    (effectiveZoneEntry),
	    m_listIndex    (listIndex)
    {
    }

    EffectiveZoneMemberManagedObject::~EffectiveZoneMemberManagedObject ()
    {
    }

    string  EffectiveZoneMemberManagedObject::getClassName()
    {
        return ("EffectiveZoneMemberManagedObject");
    }

    void  EffectiveZoneMemberManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_effectiveZoneEntry,"effectiveZoneEntry"));
	    addPersistableAttribute (new AttributeUI32(&m_listIndex,"listIndex"));

	    setUserTagForAttribute ("effectiveZoneEntry", brocade_zone_entry_name);   
    }

    void  EffectiveZoneMemberManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_effectiveZoneEntry,"effectiveZoneEntry"));
	    addPersistableAttributeForCreate  (new AttributeUI32(&m_listIndex,"listIndex"));

	    vector<string> keyName;
	    keyName.push_back ("effectiveZoneEntry");
	    setUserDefinedKeyCombination (keyName);
    }

    void  EffectiveZoneMemberManagedObject::setEffectiveZoneEntry(const string &effectiveZoneEntry)
    {
        m_effectiveZoneEntry  =  effectiveZoneEntry;
    }

    string  EffectiveZoneMemberManagedObject::getEffectiveZoneEntry() const
    {
        return (m_effectiveZoneEntry);
    }

    void  EffectiveZoneMemberManagedObject::setListIndex(const UI32 &listIndex)
    {
        m_listIndex  =  listIndex;
    }

    UI32  EffectiveZoneMemberManagedObject::getListIndex() const
    {
        return (m_listIndex);
    }

}
