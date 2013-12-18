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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SystemMonitor/Local/LineCardAlertLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

//#include <brocade-system-monitor.h>
#include "vcs.h"
namespace DcmNs
{

    LineCardAlertLocalManagedObject::LineCardAlertLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : PrismElement  (pSystemMonitorLocalObjectManager),
        PrismPersistableObject (LineCardAlertLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorLocalObjectManager),
        DcmManagedObject (pSystemMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSystemMonitorLocalObjectManager)
    {
    }

    LineCardAlertLocalManagedObject::LineCardAlertLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager,const BitMap &state,const BitMap &action)
        : PrismElement  (pSystemMonitorLocalObjectManager),
        PrismPersistableObject (LineCardAlertLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorLocalObjectManager),
        DcmManagedObject (pSystemMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSystemMonitorLocalObjectManager),
        m_state    (state),
        m_action    (action)
    {
    }

    LineCardAlertLocalManagedObject::~LineCardAlertLocalManagedObject ()
    {
    }

    string  LineCardAlertLocalManagedObject::getClassName()
    {
        return ("LineCardAlertLocalManagedObject");
    }

    void  LineCardAlertLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBitMap(&m_state,"state",system_monitor_state));
        addPersistableAttribute (new AttributeBitMap(&m_action,"action",system_monitor_action));
    }

    void  LineCardAlertLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBitMap(&m_state,"state",system_monitor_state));
        addPersistableAttributeForCreate  (new AttributeBitMap(&m_action,"action",system_monitor_action));
    }

    void  LineCardAlertLocalManagedObject::setState(const BitMap &state)
    {
        m_state  =  state;
    }

    BitMap  LineCardAlertLocalManagedObject::getState() const
    {
        return (m_state);
    }

    void  LineCardAlertLocalManagedObject::setAction(const BitMap &action)
    {
        m_action  =  action;
    }

    BitMap  LineCardAlertLocalManagedObject::getAction() const
    {
        return (m_action);
    }

}
