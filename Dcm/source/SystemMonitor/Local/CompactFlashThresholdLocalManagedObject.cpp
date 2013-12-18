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

#include "SystemMonitor/Local/CompactFlashThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
//#include "brocade-system-monitor.h"
#include "vcs.h"
namespace DcmNs
{

    CompactFlashThresholdLocalManagedObject::CompactFlashThresholdLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : PrismElement  (pSystemMonitorLocalObjectManager),
        PrismPersistableObject (CompactFlashThresholdLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorLocalObjectManager),
        DcmManagedObject (pSystemMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSystemMonitorLocalObjectManager)
    {
    }

    CompactFlashThresholdLocalManagedObject::CompactFlashThresholdLocalManagedObject (SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager,const UI32 &downthreshold,const UI32 &marginalthreshold)
        : PrismElement  (pSystemMonitorLocalObjectManager),
        PrismPersistableObject (CompactFlashThresholdLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorLocalObjectManager),
        DcmManagedObject (pSystemMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSystemMonitorLocalObjectManager),
        m_downthreshold    (downthreshold),
        m_marginalthreshold    (marginalthreshold)
    {
    }

    CompactFlashThresholdLocalManagedObject::~CompactFlashThresholdLocalManagedObject ()
    {
    }

    string  CompactFlashThresholdLocalManagedObject::getClassName()
    {
        return ("CompactFlashThresholdLocalManagedObject");
    }

    void  CompactFlashThresholdLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_downthreshold,"downthreshold", system_monitor_down_threshold));
        addPersistableAttribute (new AttributeUI32(&m_marginalthreshold,"marginalthreshold", system_monitor_marginal_threshold));
    }

    void  CompactFlashThresholdLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_downthreshold,"downthreshold", system_monitor_down_threshold));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_marginalthreshold,"marginalthreshold", system_monitor_marginal_threshold));
    }

    void  CompactFlashThresholdLocalManagedObject::setDownthreshold(const UI32 &downthreshold)
    {
        m_downthreshold  =  downthreshold;
    }

    UI32  CompactFlashThresholdLocalManagedObject::getDownthreshold() const
    {
        return (m_downthreshold);
    }

    void  CompactFlashThresholdLocalManagedObject::setMarginalthreshold(const UI32 &marginalthreshold)
    {
        m_marginalthreshold  =  marginalthreshold;
    }

    UI32  CompactFlashThresholdLocalManagedObject::getMarginalthreshold() const
    {
        return (m_marginalthreshold);
    }

}
