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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Nsm/Local/Rmon/RmonStatisticsLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

namespace DcmNs
{

    RmonStatisticsLocalManagedObject::RmonStatisticsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (RmonStatisticsLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_statsIndex = 0;
    }

    RmonStatisticsLocalManagedObject::RmonStatisticsLocalManagedObject
	  (NsmLocalObjectManager *pNsmLocalObjectManager,const SI32 &statsIndex,const string &owner)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (RmonStatisticsLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_statsIndex    (statsIndex),
        m_owner    (owner)
    {
    }

    RmonStatisticsLocalManagedObject::~RmonStatisticsLocalManagedObject ()
    {
    }

    string  RmonStatisticsLocalManagedObject::getClassName()
    {
        return ("RmonStatisticsLocalManagedObject");
    }

    void  RmonStatisticsLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_statsIndex,"statsIndex"));
        addPersistableAttribute (new AttributeString(&m_owner,"owner", rmon_ether_stats_owner));
		setUserTagForAttribute("statsIndex", rmon_ether_stats_index);
    }

    void  RmonStatisticsLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_statsIndex,"statsIndex"));
        addPersistableAttributeForCreate  (new AttributeString(&m_owner,"owner", rmon_ether_stats_owner));

		vector<string > keyName;
        keyName.push_back ("statsIndex");
        setUserDefinedKeyCombination (keyName);

    }

    void  RmonStatisticsLocalManagedObject::setStatsIndex(const SI32 &statsIndex)
    {
        m_statsIndex  =  statsIndex;
    }

    SI32 RmonStatisticsLocalManagedObject::getStatsIndex() const
    {
        return (m_statsIndex);
    }

    void  RmonStatisticsLocalManagedObject::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonStatisticsLocalManagedObject::getOwner() const
    {
        return (m_owner);
    }

}
