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
 *   Author : jvenkata                                                     *
 **************************************************************************/

#include "Cluster/Local/HeartBeat/HeartBeatConfigLocalManagedObject.h"
#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"

namespace WaveNs
{

    HeartBeatConfigLocalManagedObject::HeartBeatConfigLocalManagedObject (HeartBeatObjectManager *pHeartBeatObjectManager)
        : PrismElement  (pHeartBeatObjectManager),
        PrismPersistableObject (HeartBeatConfigLocalManagedObject::getClassName (), WaveLocalManagedObject::getClassName ()),
        WaveManagedObject (pHeartBeatObjectManager),
        WaveLocalManagedObjectBase (this),
        WaveLocalManagedObject (pHeartBeatObjectManager),
        m_defaultHeartBeatInterval (0),
        m_defaultHeartBeatFailureThreshold (0)
    {
    }

    HeartBeatConfigLocalManagedObject::HeartBeatConfigLocalManagedObject (HeartBeatObjectManager *pHeartBeatObjectManager,const UI32 &defaultHeartBeatInterval,const UI32 &defaultHeartBeatFailureThreshold)
        : PrismElement  (pHeartBeatObjectManager),
        PrismPersistableObject (HeartBeatConfigLocalManagedObject::getClassName (), WaveLocalManagedObject::getClassName ()),
        WaveManagedObject (pHeartBeatObjectManager),
        WaveLocalManagedObjectBase (this),
        WaveLocalManagedObject (pHeartBeatObjectManager),
        m_defaultHeartBeatInterval    (defaultHeartBeatInterval),
        m_defaultHeartBeatFailureThreshold    (defaultHeartBeatFailureThreshold)
    {
    }

    HeartBeatConfigLocalManagedObject::~HeartBeatConfigLocalManagedObject ()
    {
    }

    string  HeartBeatConfigLocalManagedObject::getClassName() 
    {
        return ("HeartBeatConfigLocalManagedObject");
    }

    void  HeartBeatConfigLocalManagedObject::setupAttributesForPersistence()
    {
        WaveLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_defaultHeartBeatInterval,"defaultHeartBeatInterval"));
        addPersistableAttribute (new AttributeUI32(&m_defaultHeartBeatFailureThreshold,"defaultHeartBeatFailureThreshold"));
    }

    void  HeartBeatConfigLocalManagedObject::setupAttributesForCreate()
    {
        WaveLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_defaultHeartBeatInterval,"defaultHeartBeatInterval"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_defaultHeartBeatFailureThreshold,"defaultHeartBeatFailureThreshold"));
    }

    void  HeartBeatConfigLocalManagedObject::setDefaultHeartBeatInterval(const UI32 &defaultHeartBeatInterval)
    {
        m_defaultHeartBeatInterval  =  defaultHeartBeatInterval;
    }

    UI32  HeartBeatConfigLocalManagedObject::getDefaultHeartBeatInterval() const
    {
        return (m_defaultHeartBeatInterval);
    }

    void  HeartBeatConfigLocalManagedObject::setDefaultHeartBeatFailureThreshold(const UI32 &defaultHeartBeatFailureThreshold)
    {
        m_defaultHeartBeatFailureThreshold  =  defaultHeartBeatFailureThreshold;
    }

    UI32  HeartBeatConfigLocalManagedObject::getDefaultHeartBeatFailureThreshold() const
    {
        return (m_defaultHeartBeatFailureThreshold);
    }

}
