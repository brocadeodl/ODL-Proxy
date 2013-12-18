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

#include "SystemMonitor/Global/PowerThresholdManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    PowerThresholdManagedObject::PowerThresholdManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (PowerThresholdManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager)
    {
    }

    PowerThresholdManagedObject::PowerThresholdManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const UI32 &downthreshold,const UI32 &marginalthreshold)
        : PrismElement  (pSystemMonitorGlobalObjectManager),
        PrismPersistableObject (PowerThresholdManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSystemMonitorGlobalObjectManager),
        DcmManagedObject (pSystemMonitorGlobalObjectManager),
        m_downthreshold    (downthreshold),
        m_marginalthreshold    (marginalthreshold)
    {
    }

    PowerThresholdManagedObject::~PowerThresholdManagedObject ()
    {
    }

    string  PowerThresholdManagedObject::getClassName()
    {
        return ("PowerThresholdManagedObject");
    }

    void  PowerThresholdManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_downthreshold,"downthreshold"));
        addPersistableAttribute (new AttributeUI32(&m_marginalthreshold,"marginalthreshold"));
    }

    void  PowerThresholdManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_downthreshold,"downthreshold"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_marginalthreshold,"marginalthreshold"));
    }

    void  PowerThresholdManagedObject::setDownthreshold(const UI32 &downthreshold)
    {
        m_downthreshold  =  downthreshold;
    }

    UI32  PowerThresholdManagedObject::getDownthreshold() const
    {
        return (m_downthreshold);
    }

    void  PowerThresholdManagedObject::setMarginalthreshold(const UI32 &marginalthreshold)
    {
        m_marginalthreshold  =  marginalthreshold;
    }

    UI32  PowerThresholdManagedObject::getMarginalthreshold() const
    {
        return (m_marginalthreshold);
    }

}
