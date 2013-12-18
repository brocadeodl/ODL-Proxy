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

/******************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.             *
 *   All rights reserved.                                                     *
 *   .Author : Mobin Mohan                                                     *
 *****************************************************************************/
#include "Nsm/Local/Layer3/NsmIntfLoopbackManagedObject.h"
#include "Nsm/Local/NsmInterfaceVRFConfigManagedObjectBase.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "brocade-intf-loopback.h"

namespace DcmNs
{
    NsmIntfLoopbackManagedObject::NsmIntfLoopbackManagedObject(NsmLocalObjectManager *pWaveObjectManager)
        : PrismElement  (pWaveObjectManager),
        PrismPersistableObject (NsmIntfLoopbackManagedObject::getClassName(), IntfManagedObject::getClassName()),
        WaveManagedObject (pWaveObjectManager),
        DcmManagedObject (pWaveObjectManager),
        IntfManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        CommonManagedObjectBase (this),
        NsmInterfaceVRFConfigManagedObjectBase (this)
    {
        m_ifId = 0;
    }

    NsmIntfLoopbackManagedObject::~NsmIntfLoopbackManagedObject ()
    {
    }

    string  NsmIntfLoopbackManagedObject::getClassName()
    {
        return ("NsmIntfLoopbackManagedObject");
    }

    void  NsmIntfLoopbackManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObjectBase::setupAttributesForPersistence ();
        IntfManagedObject::setupAttributesForPersistence ();
        NsmInterfaceVRFConfigManagedObjectBase::setupAttributesForPersistence();
        addPersistableAttribute (new AttributeUI32(&m_ifId, "ifId"));
        setUserTagForAttribute ("ifId", intf_loopback_id);
    }

    void  NsmIntfLoopbackManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObjectBase::setupAttributesForCreate ();
        NsmInterfaceVRFConfigManagedObjectBase::setupAttributesForCreate();
        addPersistableAttributeForCreate (new AttributeUI32(&m_ifId, "ifId"));
    }
    
    void NsmIntfLoopbackManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("ifId");
        setUserDefinedKeyCombination (keyName);
    }

    void NsmIntfLoopbackManagedObject::setIfId(const UI32 &id)
    {
        m_ifId = id;
    }

    UI32 NsmIntfLoopbackManagedObject::getIfId() const
    {
        return (m_ifId);
    }

} // DcmNs
