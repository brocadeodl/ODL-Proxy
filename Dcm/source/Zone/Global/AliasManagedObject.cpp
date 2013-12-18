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

#include "Zone/Global/AliasManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/AliasMemberManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"

namespace DcmNs
{

    AliasManagedObject::AliasManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (AliasManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    AliasManagedObject::AliasManagedObject (ZoneObjectManager *pZoneObjectManager,const string &aliasName,const vector<WaveManagedObjectPointer<AliasMemberManagedObject> > &aliasMembers)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (AliasManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_aliasName    (aliasName),
        m_aliasMembers    (aliasMembers)
    {
    }

    AliasManagedObject::~AliasManagedObject ()
    {
    }

    string  AliasManagedObject::getClassName()
    {
        return ("AliasManagedObject");
    }

    void  AliasManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_aliasName,"aliasName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<AliasMemberManagedObject>(&m_aliasMembers,"aliasMembers", getClassName (), getObjectId (), AliasMemberManagedObject::getClassName ()));

        setUserTagForAttribute ("aliasName", brocade_zone_alias_name);
    }

    void  AliasManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_aliasName,"aliasName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<AliasMemberManagedObject>(&m_aliasMembers,"aliasMembers", getClassName (), getObjectId (), AliasMemberManagedObject::getClassName ()));

        setDisableValidations ("aliasMembers", true);
        vector<string> keyName;
        keyName.push_back ("aliasName");
        setUserDefinedKeyCombination (keyName);
    }

    void  AliasManagedObject::setAliasName(const string &aliasName)
    {
        m_aliasName  =  aliasName;
    }

    string  AliasManagedObject::getAliasName() const
    {
        return (m_aliasName);
    }

    void  AliasManagedObject::setAliasMembers(const vector<WaveManagedObjectPointer<AliasMemberManagedObject> > &aliasMembers)
    {
        m_aliasMembers  =  aliasMembers;
    }

    vector<WaveManagedObjectPointer<AliasMemberManagedObject> >  AliasManagedObject::getAliasMembers() const
    {
        return (m_aliasMembers);
    }

    int AliasManagedObject::removeAliasMembers(const string aliasEntry)
	{
		unsigned int i;

		for (i = 0; i < m_aliasMembers.size(); i++) {
			if (m_aliasMembers[i]->getAliasEntry() == aliasEntry) {
				trace(TRACE_LEVEL_INFO, "AliasManagedObject::removeAliasMembers: Found aliasEntry");
				m_aliasMembers.erase(m_aliasMembers.begin() + i);
				return (0);
			}
		}

		return (1);
	}

    int AliasManagedObject::numOfLeftAliasMembers(const string aliasEntry,
	    unsigned int *maxListIndex)
    {
        unsigned int i;
        int num_of_left = 0;

	    if (maxListIndex) {
	        *maxListIndex = 0;
	    }

        for (i = 0; i < m_aliasMembers.size(); i++) {
            if (m_aliasMembers[i]->getAliasEntry() == aliasEntry) {
                trace(TRACE_LEVEL_INFO, "AliasManagedObject::numOfLeftAliasMembers: Found aliasEntry\n");
            } else {
                if (maxListIndex) {
                    if (m_aliasMembers[i]->getListIndex() > *maxListIndex) {
                        *maxListIndex = m_aliasMembers[i]->getListIndex();
                    }
                }
                num_of_left++;
            }
        }

        return (num_of_left);
    }

    int AliasManagedObject::queryAliasMembersForEntry(const string entryToFind)
    {
        unsigned int i;

        for (i = 0; i < m_aliasMembers.size(); i++) {
            if (m_aliasMembers[i]->getAliasEntry() == entryToFind) {
                //trace(TRACE_LEVEL_INFO, "AliasManagedObject::queryAliasMembersForEntry: Found entryToFind\n");
                return (1);
            }
        }

        return (0);
    }

    int AliasManagedObject::numOfAliasMembers()
    {
        //tracePrintf (TRACE_LEVEL_INFO, false, false, "AliasManagedObject::numOfAliasMembers = %d\n", m_aliasMembers.size());
        return (m_aliasMembers.size());
    }

}
