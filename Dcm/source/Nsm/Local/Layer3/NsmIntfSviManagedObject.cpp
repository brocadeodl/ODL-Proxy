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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "vcs.h"

#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"

// #include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
// #include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

namespace DcmNs
{
    NsmIntfSviManagedObject::NsmIntfSviManagedObject (
        WaveObjectManager *pWaveObjectManager)
        :PrismElement  (pWaveObjectManager),
        PrismPersistableObject (NsmIntfSviManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject          (pWaveObjectManager),
        DcmManagedObject           (pWaveObjectManager),
        IntfManagedObject          (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase  (this),
        CommonManagedObjectBase (this),
        NsmInterfaceVRFConfigManagedObjectBase (this)
    {
        m_ifId = 0;
        m_status = true;
    }

    NsmIntfSviManagedObject::~NsmIntfSviManagedObject()
    {
    }

    string NsmIntfSviManagedObject::getClassName()
    {
        return ("NsmIntfSviManagedObject");
    }

    void NsmIntfSviManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObjectBase::setupAttributesForPersistence();
        IntfManagedObject::setupAttributesForPersistence ();
        NsmInterfaceVRFConfigManagedObjectBase::setupAttributesForPersistence();

        addPersistableAttribute (new AttributeUI32(&m_ifId, "vlanId"));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_vlan, "vlan", VlanIntfManagedObject::getClassName (),true));

        setUserTagForAttribute ("vlanId", brocade_interface_name);
    }

    void NsmIntfSviManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObjectBase::setupAttributesForCreate();
        NsmInterfaceVRFConfigManagedObjectBase::setupAttributesForCreate();

        addPersistableAttributeForCreate (new AttributeUI32(&m_ifId, "vlanId"));
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_vlan, "vlan", VlanIntfManagedObject::getClassName (),true));
    }

    void NsmIntfSviManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("vlanId");
        //keyName.push_back("ownerWaveNodeObjectId");
        setUserDefinedKeyCombination (keyName);
    }

    UI32  NsmIntfSviManagedObject::getIfId() const
    {
        return m_ifId;
    }

    void  NsmIntfSviManagedObject::setIfId(const UI32 &if_id)
    {
        m_ifId  =  if_id;
    }

    ObjectId NsmIntfSviManagedObject :: getVlanId() const
    {
        return m_vlan;
    }

    void NsmIntfSviManagedObject :: setVlanId(const ObjectId &vlanId)
    {
        m_vlan = vlanId;
    }
} // DcmNs
