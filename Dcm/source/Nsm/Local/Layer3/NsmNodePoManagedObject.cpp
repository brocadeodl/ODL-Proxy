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
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"

// #include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

namespace DcmNs
{
    NsmNodePoManagedObject::NsmNodePoManagedObject (
        WaveObjectManager *pWaveObjectManager)
        :PrismElement  (pWaveObjectManager),
        PrismPersistableObject (NsmNodePoManagedObject::getClassName (), IntfManagedObject::getClassName ()),
        WaveManagedObject          (pWaveObjectManager),
        DcmManagedObject           (pWaveObjectManager),
        IntfManagedObject          (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase  (this)
    {
        m_ifId = 0;
    }

    NsmNodePoManagedObject::~NsmNodePoManagedObject()
    {
    }

    string  NsmNodePoManagedObject::getClassName()
    {
        return ("NsmNodePoManagedObject");
    }

    void NsmNodePoManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObjectBase::setupAttributesForPersistence();
        IntfManagedObject::setupAttributesForPersistence ();

        addPersistableAttribute (new AttributeUI32(&m_ifId, "poId"));

        setUserTagForAttribute ("poId", brocade_interface_name);
    }

    void NsmNodePoManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObjectBase::setupAttributesForCreate();

        addPersistableAttributeForCreate (new AttributeUI32(&m_ifId, "poId"));
    }

    void NsmNodePoManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("poId");
        setUserDefinedKeyCombination (keyName);
    }

    UI32  NsmNodePoManagedObject::getIfId() const
    {
        return m_ifId;
    }

    void  NsmNodePoManagedObject::setIfId(const UI32 &if_id)
    {
        m_ifId  =  if_id;
    }

} // DcmNs
