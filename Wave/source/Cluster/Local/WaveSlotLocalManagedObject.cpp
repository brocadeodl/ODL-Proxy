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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.                               *
 *   All rights reserved.                                                                                                   *
 *   Author : Jitendra Singh                                                                                             *
 ***************************************************************************/

#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObject.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"

namespace WaveNs
{

// Dummy Constructor: This should not be called
WaveSlotLocalManagedObject::WaveSlotLocalManagedObject ()
    : PrismElement               (*this),
      PrismPersistableObject     (WaveSlotLocalManagedObjectBase::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (*this),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (*this),
      WaveSlotLocalManagedObjectBase (*this)
{
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
} 

WaveSlotLocalManagedObject::WaveSlotLocalManagedObject (ClusterLocalObjectManager *pClusterLocalObjectManager)
    : PrismElement               (pClusterLocalObjectManager),
      PrismPersistableObject     (WaveSlotLocalManagedObject::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (pClusterLocalObjectManager),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (pClusterLocalObjectManager),
      WaveSlotLocalManagedObjectBase (this)
{
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
}

WaveSlotLocalManagedObject::WaveSlotLocalManagedObject (ClusterLocalObjectManager *pClusterLocalObjectManager, UI32 &slotNumber, UI32 &objectType, UI32 &hwType, UI32 &hwId, UI32 &slotState, UI32 &reason, UI32 &version)
    : PrismElement               (pClusterLocalObjectManager),
      PrismPersistableObject     (WaveSlotLocalManagedObject::getClassName (), WaveLocalManagedObject::getClassName ()),
      WaveManagedObject          (pClusterLocalObjectManager),
      WaveLocalManagedObjectBase (this),
      WaveLocalManagedObject     (pClusterLocalObjectManager),
      WaveSlotLocalManagedObjectBase (this, slotNumber, objectType, hwType, hwId, slotState, reason, version)
{
    setEmptyNeededOnPersistentBoot (true);
    setEmptyNeededOnPersistentBootWithDefault (false);
}

WaveSlotLocalManagedObject::~WaveSlotLocalManagedObject ()
{
}

string WaveSlotLocalManagedObject::getClassName ()
{
    return ("WaveSlotLocalManagedObject");
}

void WaveSlotLocalManagedObject::setupAttributesForPersistence ()
{
    WaveLocalManagedObject::setupAttributesForPersistence ();

    WaveSlotLocalManagedObjectBase::setupAttributesForPersistence ();
}

void WaveSlotLocalManagedObject::setupAttributesForCreate ()
{
    WaveLocalManagedObject::setupAttributesForCreate ();

    WaveSlotLocalManagedObjectBase::setupAttributesForCreate ();
}

}
