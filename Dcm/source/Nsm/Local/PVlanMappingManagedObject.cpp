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

#include "Nsm/Local/PVlanMappingManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    PVlanMappingManagedObject::PVlanMappingManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PVlanMappingManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    PVlanMappingManagedObject::PVlanMappingManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PVlanMappingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
    }

    PVlanMappingManagedObject::PVlanMappingManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  PVlanMappingOper &oper)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PVlanMappingManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_primaryVlanId    (primaryVlanId),
        m_secVlanRange		(secVlanRange),
        m_oper	    (oper)
    {
    }

    PVlanMappingManagedObject::PVlanMappingManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  PVlanMappingOper &oper)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PVlanMappingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager),
        m_primaryVlanId    (primaryVlanId),
        m_secVlanRange		(secVlanRange),
        m_oper	    (oper)
    {
    }

    PVlanMappingManagedObject::~PVlanMappingManagedObject ()
    {
    }

    string  PVlanMappingManagedObject::getClassName()
    {
        return ("PVlanMappingManagedObject");
    }

    void  PVlanMappingManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        const UI32Range defaultData("");
        addPersistableAttribute (new AttributeUI32(&m_primaryVlanId,"primaryVlanId"));
        addPersistableAttribute (new AttributeUI32Range(&m_secVlanRange,defaultData,"secVlanRange",brocade_interface_promis_sec_pvlan_range));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_oper),"oper",brocade_interface_oper));

	    setUserTagForAttribute("primaryVlanId", brocade_interface_promis_pri_pvlan);
    }

    void  PVlanMappingManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        const UI32Range defaultData("");
		addPersistableAttributeForCreate (new AttributeUI32(&m_primaryVlanId,"primaryVlanId", brocade_interface_promis_pri_pvlan));
		addPersistableAttributeForCreate (new AttributeUI32Range(&m_secVlanRange,defaultData,"secVlanRange",brocade_interface_promis_sec_pvlan_range));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_oper),"oper",brocade_interface_oper));

	    vector<string> keyName;
		keyName.push_back ("primaryVlanId");
		setUserDefinedKeyCombination (keyName);

    }

    void  PVlanMappingManagedObject::setPrimaryVlanId(const UI32 &primaryVlanId)
    {
        m_primaryVlanId  =  primaryVlanId;
    }

    UI32  PVlanMappingManagedObject::getPrimaryVlanId() const
    {
        return (m_primaryVlanId);
    }

    void  PVlanMappingManagedObject::setSecPvlanRange(const UI32Range &secVlanRange)
    {
    	m_secVlanRange = secVlanRange;
    }

    UI32Range  PVlanMappingManagedObject::getSecPvlanRange() const
    {
        return (m_secVlanRange);
    }

    void  PVlanMappingManagedObject::setoper(const PVlanMappingOper  &oper)
    {
        m_oper  =  oper;
    }

    PVlanMappingOper PVlanMappingManagedObject::getoper() const
    {
        return (m_oper);
    }


}
