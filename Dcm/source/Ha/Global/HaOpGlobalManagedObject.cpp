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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   December, 2012                                                         *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "Ha/Global/HaOpGlobalManagedObject.h"
#include "Ha/Global/HaOpGlobalObjectManager.h"
#include "Ha/Local/HaOpTypes.h"

namespace DcmNs
{

    HaOpGlobalManagedObject::HaOpGlobalManagedObject (HaOpGlobalObjectManager *pHaOpGlobalObjectManager)
        : PrismElement  (pHaOpGlobalObjectManager),
        PrismPersistableObject (HaOpGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pHaOpGlobalObjectManager),
        DcmManagedObject (pHaOpGlobalObjectManager)
    {
    }

    HaOpGlobalManagedObject::HaOpGlobalManagedObject (HaOpGlobalObjectManager *pHaOpGlobalObjectManager, const UI32 &rbridge_id, const UI32 &state)
        : PrismElement  (pHaOpGlobalObjectManager),
        PrismPersistableObject (HaOpGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pHaOpGlobalObjectManager),
        DcmManagedObject (pHaOpGlobalObjectManager),
		m_rbridge_id(rbridge_id),
		m_state	(state)
    {
    }

    HaOpGlobalManagedObject::~HaOpGlobalManagedObject ()
    {
    }

    string  HaOpGlobalManagedObject::getClassName()
    {
        return ("HaOpGlobalManagedObject");
    }

    void  HaOpGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_rbridge_id,"rbridge_id"));
        addPersistableAttribute (new AttributeUI32(&m_state,"state"));
    }

    void  HaOpGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI32(&m_rbridge_id,
			"rbridge_id"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_state,
			"state"));
    	/*vector<string > keyName;
		keyName.push_back ("name");
        setUserDefinedKeyCombination (keyName);     */

	}

    void HaOpGlobalManagedObject::setRbridge_Id(const UI32 &rbridge_id)
    {
        m_rbridge_id  =  rbridge_id;
    }

    UI32 HaOpGlobalManagedObject::getRbridge_Id() const
    {
        return (m_rbridge_id);
    }

    void HaOpGlobalManagedObject::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    UI32 HaOpGlobalManagedObject::getState() const
    {
        return (m_state);
    }

}
