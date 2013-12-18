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

#include "SystemMonitor/Global/WwnCardAlertManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    WwnCardAlertManagedObject::WwnCardAlertManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (WwnCardAlertManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager)
    {
    }

    WwnCardAlertManagedObject::WwnCardAlertManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const UI32 &state,const UI32 &action)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (WwnCardAlertManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager),
        m_state    (state),
        m_action    (action)
    {
    }

    WwnCardAlertManagedObject::~WwnCardAlertManagedObject ()
    {
    }

    string  WwnCardAlertManagedObject::getClassName()
    {
        return ("WwnCardAlertManagedObject");
    }

    void  WwnCardAlertManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_state,"state"));
        addPersistableAttribute (new AttributeUI32(&m_action,"action"));
    }

    void  WwnCardAlertManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_state,"state"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_action,"action"));
    }

    void  WwnCardAlertManagedObject::setState(const UI32 &state)
    {
        m_state  =  state;
    }

    UI32  WwnCardAlertManagedObject::getState() const
    {
        return (m_state);
    }

    void  WwnCardAlertManagedObject::setAction(const UI32 &action)
    {
        m_action  =  action;
    }

    UI32  WwnCardAlertManagedObject::getAction() const
    {
        return (m_action);
    }

}
