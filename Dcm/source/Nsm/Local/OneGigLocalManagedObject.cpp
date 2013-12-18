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
 *   Author : vsatyana                                                     *
 **************************************************************************/
#include "Nsm/Local/OneGigLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

namespace DcmNs
{
    OneGigLocalManagedObject::OneGigLocalManagedObject (NsmLocalObjectManager *pWaveObjectManager)
        :PrismElement  (pWaveObjectManager),
        PrismPersistableObject (OneGigLocalManagedObject::getClassName (), PhyIntfLocalManagedObject::getClassName ()),
        WaveManagedObject          (pWaveObjectManager),
        DcmManagedObject           (pWaveObjectManager),
        IntfManagedObject          (pWaveObjectManager),
        PoPhyIntfManagedObject (pWaveObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase  (this),
        CommonManagedObjectBase (this),
        NsmInterfaceVRFConfigManagedObjectBase (this),
        PhyIntfLocalManagedObject (pWaveObjectManager)
        {
    		setPhyType(IF_TYPE_GI);
        }

    OneGigLocalManagedObject::~OneGigLocalManagedObject ()
    {
    }

    string  OneGigLocalManagedObject::getClassName()
    {
        return ("OneGigLocalManagedObject");
    }

    void OneGigLocalManagedObject::setupAttributesForPersistence ()
    {
        PhyIntfLocalManagedObject::setupAttributesForPersistence ();
    }

    void OneGigLocalManagedObject::setupAttributesForCreate ()
    {
    }
}
