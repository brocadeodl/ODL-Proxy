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
 *   .Author : dchakrab                                                     *
 **************************************************************************/

#include "Nsm/Local/Rmon/RmonHistoryBookKeepLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

namespace DcmNs
{

    RmonHistoryBookKeepLocalManagedObject::RmonHistoryBookKeepLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (RmonHistoryBookKeepLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_historyIndex = 0;
    }

    RmonHistoryBookKeepLocalManagedObject::RmonHistoryBookKeepLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,
		const SI32 &historyIndex,const string &ifName)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (RmonHistoryBookKeepLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_historyIndex    (historyIndex),
	m_ifName  (ifName)
    {
    }

    RmonHistoryBookKeepLocalManagedObject::~RmonHistoryBookKeepLocalManagedObject ()
    {
    }

    string  RmonHistoryBookKeepLocalManagedObject::getClassName()
    {
        return ("RmonHistoryBookKeepLocalManagedObject");
    }

    void  RmonHistoryBookKeepLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_historyIndex,"historyIndex"));
		addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));
		setUserTagForAttribute("historyIndex", rmon_history_control_index);
    }

    void  RmonHistoryBookKeepLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_historyIndex,"historyIndex"));
		addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));

		vector<string > keyName;
        keyName.push_back ("historyIndex");
        setUserDefinedKeyCombination (keyName);

    }

    void  RmonHistoryBookKeepLocalManagedObject::setHistoryIndex(const SI32 &historyIndex)
    {
        m_historyIndex  =  historyIndex;
    }

    SI32  RmonHistoryBookKeepLocalManagedObject::getHistoryIndex() const
    {
        return (m_historyIndex);
    }
    void  RmonHistoryBookKeepLocalManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RmonHistoryBookKeepLocalManagedObject::getIfName() const
    {
        return (m_ifName);
    }

}
