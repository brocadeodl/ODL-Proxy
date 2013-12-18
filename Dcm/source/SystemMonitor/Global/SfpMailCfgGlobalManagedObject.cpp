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
 *   Author : pahuja                                                       *
 **************************************************************************/

#include "SystemMonitor/Global/SfpMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

#include "brocade-system-monitor.h"

namespace DcmNs
{

    SfpMailCfgGlobalManagedObject::SfpMailCfgGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (SfpMailCfgGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager)
    {
    }

    SfpMailCfgGlobalManagedObject::SfpMailCfgGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const bool &enableFlag)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (SfpMailCfgGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager),
        m_enableFlag    (enableFlag)
    {
    }

    SfpMailCfgGlobalManagedObject::~SfpMailCfgGlobalManagedObject ()
    {
    }

    string  SfpMailCfgGlobalManagedObject::getClassName()
    {
        return ("SfpMailCfgGlobalManagedObject");
    }

    void  SfpMailCfgGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enableFlag,"enableFlag", system_monitor_enable));
    }

    void  SfpMailCfgGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_enableFlag,"enableFlag", system_monitor_enable));
    }

    void  SfpMailCfgGlobalManagedObject::setEnableFlag(const bool &enableFlag)
    {
        m_enableFlag  =  enableFlag;
    }

    bool  SfpMailCfgGlobalManagedObject::getEnableFlag() const
    {
        return (m_enableFlag);
    }

}