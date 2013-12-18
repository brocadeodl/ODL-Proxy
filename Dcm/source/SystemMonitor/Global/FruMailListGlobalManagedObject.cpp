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
 *   Author : noku                                                     *
 **************************************************************************/

#include "SystemMonitor/Global/FruMailListGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

#include "brocade-system-monitor.h"

namespace DcmNs
{

    FruMailListGlobalManagedObject::FruMailListGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (FruMailListGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager)
    {
    }

    FruMailListGlobalManagedObject::FruMailListGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const string &email)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (FruMailListGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager),
        m_email    (email)
    {
    }

    FruMailListGlobalManagedObject::~FruMailListGlobalManagedObject ()
    {
    }

    string  FruMailListGlobalManagedObject::getClassName()
    {
        return ("FruMailListGlobalManagedObject");
    }

    void  FruMailListGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_email,"email", system_monitor_email));
    }

    void  FruMailListGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_email,"email", system_monitor_email));
        vector<string > keyList;
        keyList.push_back ("email");
        setUserDefinedKeyCombination (keyList);
    }

    void  FruMailListGlobalManagedObject::setEmail(const string &email)
    {
        m_email  =  email;
    }

    string  FruMailListGlobalManagedObject::getEmail() const
    {
        return (m_email);
    }

}
