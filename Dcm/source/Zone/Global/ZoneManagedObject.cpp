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

#include "Zone/Global/ZoneManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/ZoneMemberManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/

namespace DcmNs
{

    ZoneManagedObject::ZoneManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (ZoneManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    ZoneManagedObject::ZoneManagedObject (ZoneObjectManager *pZoneObjectManager,const string &zoneName,const vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > &zoneMembers)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (ZoneManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_zoneName    (zoneName),
        m_zoneMembers    (zoneMembers)
    {
    }

    ZoneManagedObject::~ZoneManagedObject ()
    {
    }

    string  ZoneManagedObject::getClassName()
    {
        return ("ZoneManagedObject");
    }

    void  ZoneManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_zoneName,"zoneName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<ZoneMemberManagedObject>(&m_zoneMembers,"zoneMembers", getClassName (), getObjectId (), ZoneMemberManagedObject::getClassName ()));

	    setUserTagForAttribute ("zoneName", brocade_zone_zone_name);
    }

    void  ZoneManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_zoneName,"zoneName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ZoneMemberManagedObject>(&m_zoneMembers,"zoneMembers", getClassName (), getObjectId (), ZoneMemberManagedObject::getClassName ()));

        setDisableValidations ("zoneMembers", true);
        vector<string> keyName;
        keyName.push_back ("zoneName");
        setUserDefinedKeyCombination (keyName);
    }

    void  ZoneManagedObject::setZoneName(const string &zoneName)
    {
        m_zoneName  =  zoneName;
    }

    string  ZoneManagedObject::getZoneName() const
    {
        return (m_zoneName);
    }

    void  ZoneManagedObject::setZoneMembers(const vector<WaveManagedObjectPointer<ZoneMemberManagedObject> > &zoneMembers)
    {
        m_zoneMembers  =  zoneMembers;
    }

    vector<WaveManagedObjectPointer<ZoneMemberManagedObject> >  ZoneManagedObject::getZoneMembers() const
    {
        return (m_zoneMembers);
    }

    void ZoneManagedObject::addZoneMembers(ZoneMemberManagedObject *zoneMember)
	{
//		ExtMacAclRuleManagedObject *extRule = dynamic_cast<ExtMacAclRuleManagedObject *>(ruleMO);
//		WaveManagedObjectPointer<ExtMacAclRuleManagedObject> eRule (extRule);
		m_zoneMembers.push_back(zoneMember);
		
	}

	int ZoneManagedObject::removeZoneMembers(const string zoneEntry)
	{
		unsigned int i;

		for (i = 0; i < m_zoneMembers.size(); i++) {
			if (m_zoneMembers[i]->getZoneEntry() == zoneEntry) {
				trace(TRACE_LEVEL_INFO, "ZoneManagedObject::removeZoneMembers: Found cfgEntry");
				m_zoneMembers.erase(m_zoneMembers.begin() + i);
				return (0);
			}
		}

		return (1);
	}

	int ZoneManagedObject::numOfLeftZoneMembers(const string zoneEntry,
	    unsigned int *maxListIndex)
    {
        unsigned int i;
        int num_of_left = 0;

	    if (maxListIndex) {
	        *maxListIndex = 0;
	    }

        for (i = 0; i < m_zoneMembers.size(); i++) {
            if (m_zoneMembers[i]->getZoneEntry() == zoneEntry) {
                trace(TRACE_LEVEL_INFO, "ZoneManagedObject::removeCfgMembers: Found cfgEntry");
            } else {
                if (maxListIndex) {
                    if (m_zoneMembers[i]->getListIndex() > *maxListIndex) {
                        *maxListIndex = m_zoneMembers[i]->getListIndex();
                    }
                }
                num_of_left++;
            }
        }

        return (num_of_left);
    }
}
