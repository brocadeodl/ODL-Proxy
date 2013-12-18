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
 *   Author : rprashar                                                     *
 **************************************************************************/

#include "Nsm/Local/PVlanTrunkAssocManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    PVlanTrunkAssocManagedObject::PVlanTrunkAssocManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PVlanTrunkAssocManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

   PVlanTrunkAssocManagedObject::PVlanTrunkAssocManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PVlanTrunkAssocManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
    }

    PVlanTrunkAssocManagedObject::PVlanTrunkAssocManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &primaryVlanId,const UI32 &secondaryVlanId)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PVlanTrunkAssocManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_primaryVlanId    (primaryVlanId),
        m_secondaryVlanId    (secondaryVlanId)
    {
    }

    PVlanTrunkAssocManagedObject::PVlanTrunkAssocManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &primaryVlanId,const UI32 &secondaryVlanId)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PVlanTrunkAssocManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager),
        m_primaryVlanId    (primaryVlanId),
        m_secondaryVlanId    (secondaryVlanId)
    {
    }

    PVlanTrunkAssocManagedObject::~PVlanTrunkAssocManagedObject ()
    {
    }

    string  PVlanTrunkAssocManagedObject::getClassName()
    {
        return ("PVlanTrunkAssocManagedObject");
    }

    void  PVlanTrunkAssocManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();

        addPersistableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addPersistableAttribute (new AttributeUI32(&m_secondaryVlanId,"secondaryVlanId"));

	    setUserTagForAttribute("primaryVlanId", brocade_interface_trunk_pri_pvlan);
        setUserTagForAttribute("secondaryVlanId", brocade_interface_trunk_sec_pvlan);
    }

    void  PVlanTrunkAssocManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_primaryVlanId,"primaryVlanId",brocade_interface_trunk_pri_pvlan));
		addPersistableAttributeForCreate (new AttributeUI32(&m_secondaryVlanId,"secondaryVlanId",brocade_interface_trunk_sec_pvlan));

	    vector<string> keyName;
		keyName.push_back ("primaryVlanId");
		setUserDefinedKeyCombination (keyName);
		keyName.push_back ("secondaryVlanId");
		setUserDefinedKeyCombination (keyName);

    }

    void  PVlanTrunkAssocManagedObject::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  PVlanTrunkAssocManagedObject::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  PVlanTrunkAssocManagedObject::setSecondaryVlanId(const UI32 &secondaryVlanId)
    {
        m_secondaryVlanId  =  secondaryVlanId;
    }

    UI32  PVlanTrunkAssocManagedObject::getSecondaryVlanId() const
    {
        return (m_secondaryVlanId);
    }

}
