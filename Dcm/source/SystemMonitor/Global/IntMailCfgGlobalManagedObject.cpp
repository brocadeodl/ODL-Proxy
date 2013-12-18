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

#include "SystemMonitor/Global/IntMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

#include "brocade-system-monitor.h"

namespace DcmNs
{

    IntMailCfgGlobalManagedObject::IntMailCfgGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (IntMailCfgGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager)
    {
    }

    IntMailCfgGlobalManagedObject::IntMailCfgGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const bool &enableFlag)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (IntMailCfgGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager),
        m_enableFlag    (enableFlag)
    {
    }

    IntMailCfgGlobalManagedObject::~IntMailCfgGlobalManagedObject ()
    {
    }

    string  IntMailCfgGlobalManagedObject::getClassName()
    {
        return ("IntMailCfgGlobalManagedObject");
    }

    void  IntMailCfgGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_enableFlag,"enableFlag", system_monitor_enable));
    }

    void  IntMailCfgGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_enableFlag,"enableFlag", system_monitor_enable));
    }

    void  IntMailCfgGlobalManagedObject::setEnableFlag(const bool &enableFlag)
    {
        m_enableFlag  =  enableFlag;
    }

    bool  IntMailCfgGlobalManagedObject::getEnableFlag() const
    {
        return (m_enableFlag);
    }

}
