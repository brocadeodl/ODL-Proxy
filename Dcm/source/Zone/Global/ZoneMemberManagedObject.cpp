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

#include "Zone/Global/ZoneMemberManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/

namespace DcmNs
{

    ZoneMemberManagedObject::ZoneMemberManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (ZoneMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    ZoneMemberManagedObject::ZoneMemberManagedObject (ZoneObjectManager *pZoneObjectManager,const string &zoneEntry,const UI32 &listIndex)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (ZoneMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_zoneEntry    (zoneEntry),
	    m_listIndex    (listIndex)
    {
    }

    ZoneMemberManagedObject::~ZoneMemberManagedObject ()
    {
    }

    string  ZoneMemberManagedObject::getClassName()
    {
        return ("ZoneMemberManagedObject");
    }

    void  ZoneMemberManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_zoneEntry,"zoneEntry"));
	    addPersistableAttribute (new AttributeUI32(&m_listIndex,"listIndex"));

	    setUserTagForAttribute ("zoneEntry", brocade_zone_entry_name);
    }

    void  ZoneMemberManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_zoneEntry,"zoneEntry"));
	    addPersistableAttributeForCreate  (new AttributeUI32(&m_listIndex,"listIndex"));

	    vector<string> keyName;
	    keyName.push_back ("zoneEntry");
	    setUserDefinedKeyCombination (keyName);
    }

    void  ZoneMemberManagedObject::setZoneEntry(const string &zoneEntry)
    {
        m_zoneEntry  =  zoneEntry;
    }

    string  ZoneMemberManagedObject::getZoneEntry() const
    {
        return (m_zoneEntry);
    }

    void  ZoneMemberManagedObject::setListIndex(const UI32 &listIndex)
    {
        m_listIndex  =  listIndex;
    }

    UI32  ZoneMemberManagedObject::getListIndex() const
    {
        return (m_listIndex);
    }

}
