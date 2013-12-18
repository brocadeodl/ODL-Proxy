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

#include "Zone/Global/CfgMemberManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/
namespace DcmNs
{

    CfgMemberManagedObject::CfgMemberManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (CfgMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    CfgMemberManagedObject::CfgMemberManagedObject (ZoneObjectManager *pZoneObjectManager,const string &cfgEntry,const UI32 &listIndex)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (CfgMemberManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_cfgEntry    (cfgEntry),
	    m_listIndex    (listIndex)
    {
    }

    CfgMemberManagedObject::~CfgMemberManagedObject ()
    {
    }

    string  CfgMemberManagedObject::getClassName()
    {
        return ("CfgMemberManagedObject");
    }

    void  CfgMemberManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_cfgEntry,"cfgEntry"));
	    addPersistableAttribute (new AttributeUI32(&m_listIndex,"listIndex"));

	    setUserTagForAttribute ("cfgEntry", brocade_zone_zone_name);
    }

    void  CfgMemberManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_cfgEntry,"cfgEntry"));
	    addPersistableAttributeForCreate  (new AttributeUI32(&m_listIndex,"listIndex"));

	    vector<string> keyName;
	    keyName.push_back ("cfgEntry");
	    setUserDefinedKeyCombination (keyName);
    }

    void  CfgMemberManagedObject::setCfgEntry(const string &cfgEntry)
    {
        m_cfgEntry  =  cfgEntry;
    }

    string  CfgMemberManagedObject::getCfgEntry() const
    {
        return (m_cfgEntry);
    }

    void  CfgMemberManagedObject::setListIndex(const UI32 &listIndex)
    {
        m_listIndex  =  listIndex;
    }

    UI32  CfgMemberManagedObject::getListIndex() const
    {
        return (m_listIndex);
    }

}
