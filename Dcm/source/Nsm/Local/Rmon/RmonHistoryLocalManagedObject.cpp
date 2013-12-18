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
 *   .Author : pbalacha                                                     *
 **************************************************************************/

#include "Nsm/Local/Rmon/RmonHistoryLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

namespace DcmNs
{

    RmonHistoryLocalManagedObject::RmonHistoryLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (RmonHistoryLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_buckets = 0;
		m_historyIndex = 0;
		m_interval = 0;
    }

    RmonHistoryLocalManagedObject::RmonHistoryLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,
		const SI32 &historyIndex,const SI32 &buckets,const SI32 &interval,const string &owner)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (RmonHistoryLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_historyIndex    (historyIndex),
        m_buckets    (buckets),
        m_interval    (interval),
        m_owner    (owner)
    {
    }

    RmonHistoryLocalManagedObject::~RmonHistoryLocalManagedObject ()
    {
    }

    string  RmonHistoryLocalManagedObject::getClassName()
    {
        return ("RmonHistoryLocalManagedObject");
    }

    void  RmonHistoryLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_historyIndex,"historyIndex"));
        addPersistableAttribute (new AttributeSI32(&m_buckets,"buckets",rmon_history_control_buckets_requested));
        addPersistableAttribute (new AttributeSI32(&m_interval,"interval",rmon_history_control_interval));
        addPersistableAttribute (new AttributeString(&m_owner,"owner",rmon_history_control_owner));
		setUserTagForAttribute("historyIndex", rmon_history_control_index);
    }

    void  RmonHistoryLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_historyIndex,"historyIndex"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_buckets,"buckets",rmon_history_control_buckets_requested));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_interval,"interval",rmon_history_control_interval));
        addPersistableAttributeForCreate  (new AttributeString(&m_owner,"owner",rmon_history_control_owner));

		vector<string > keyName;
        keyName.push_back ("historyIndex");
        setUserDefinedKeyCombination (keyName);

    }

    void  RmonHistoryLocalManagedObject::setHistoryIndex(const SI32 &historyIndex)
    {
        m_historyIndex  =  historyIndex;
    }

    SI32  RmonHistoryLocalManagedObject::getHistoryIndex() const
    {
        return (m_historyIndex);
    }

    void  RmonHistoryLocalManagedObject::setBuckets(const SI32 &buckets)
    {
        m_buckets  =  buckets;
    }

    SI32  RmonHistoryLocalManagedObject::getBuckets() const
    {
        return (m_buckets);
    }

    void  RmonHistoryLocalManagedObject::setInterval(const SI32 &interval)
    {
        m_interval  =  interval;
    }

    SI32  RmonHistoryLocalManagedObject::getInterval() const
    {
        return (m_interval);
    }

    void  RmonHistoryLocalManagedObject::setOwner(const string &owner)
    {
        m_owner  =  owner;
    }

    string  RmonHistoryLocalManagedObject::getOwner() const
    {
        return (m_owner);
    }

}
