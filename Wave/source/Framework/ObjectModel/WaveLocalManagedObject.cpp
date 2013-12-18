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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveLocalManagedObject.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"

namespace WaveNs
{

WaveLocalManagedObject::WaveLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (WaveLocalManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      WaveLocalManagedObjectBase (this)
{
}

WaveLocalManagedObject::~WaveLocalManagedObject ()
{
}

string WaveLocalManagedObject::getClassName ()
{
    return ("WaveLocalManagedObject");
}

void WaveLocalManagedObject::addWaveSlotLMOObjectId (const ObjectId &waveSlotObjectId)
{
    m_waveSlotLMOObjectIds.push_back (waveSlotObjectId);
}
vector<ObjectId> WaveLocalManagedObject::getWaveSlotLMOObjectIds ()
{
    return m_waveSlotLMOObjectIds;
}
ResourceId WaveLocalManagedObject::removeWaveSlotLMOObjectId (const ObjectId &waveSlotLMOObjectId)
{
    vector<ObjectId>::iterator waveSlotLMOElementIter = m_waveSlotLMOObjectIds.begin ();
    vector<ObjectId>::iterator waveSlotLMOElementLast = m_waveSlotLMOObjectIds.end ();
    bool OidFound = false;

    while (waveSlotLMOElementIter != waveSlotLMOElementLast)
    {
        if ((*waveSlotLMOElementIter) == waveSlotLMOObjectId)
        {
            OidFound = true;
            break;
        }
        ++waveSlotLMOElementIter;
    }
    if (OidFound)
    {
        m_waveSlotLMOObjectIds.erase (waveSlotLMOElementIter);
        return FRAMEWORK_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR,"WaveLocalManagedObject::removeWaveSlotLMOObjectId: Remove waveSlotLMO ObjectId not found");
        return FRAMEWORK_ERROR;
    }

}

void WaveLocalManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();
    

    WaveLocalManagedObjectBase::setupAttributesForPersistence ();
}

void WaveLocalManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();
    

    WaveLocalManagedObjectBase::setupAttributesForCreate ();
}

}
