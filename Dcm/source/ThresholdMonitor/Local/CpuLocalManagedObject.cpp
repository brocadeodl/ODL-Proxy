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

#include "ThresholdMonitor/Local/CpuLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "vcs.h"

namespace DcmNs
{

    CpuLocalManagedObject::CpuLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
        : PrismElement  (pThresholdMonitorLocalObjectManager),
        PrismPersistableObject (CpuLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pThresholdMonitorLocalObjectManager),
        DcmManagedObject (pThresholdMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
    {
    }

    CpuLocalManagedObject::CpuLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager,const UI32 &Poll,const UI32 &Retry,const UI32 &Limit,const actionType &Actions)
        : PrismElement  (pThresholdMonitorLocalObjectManager),
        PrismPersistableObject (CpuLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pThresholdMonitorLocalObjectManager),
        DcmManagedObject (pThresholdMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pThresholdMonitorLocalObjectManager),
        m_Poll    (Poll),
        m_Retry    (Retry),
        m_Limit    (Limit),
        m_Actions    (Actions)
    {
    }

    CpuLocalManagedObject::~CpuLocalManagedObject ()
    {
    }

    string  CpuLocalManagedObject::getClassName()
    {
        return ("CpuLocalManagedObject");
    }

    void  CpuLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_Poll,"Poll",threshold_monitor_poll));
        addPersistableAttribute (new AttributeUI32(&m_Retry,"CpuRetry",threshold_monitor_retry));
        addPersistableAttribute (new AttributeUI32(&m_Limit,"CpuLimit",threshold_monitor_limit));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_Actions,"Actions",threshold_monitor_actions));
    }

    void  CpuLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Poll,"Poll",threshold_monitor_poll));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Retry,"CpuRetry",threshold_monitor_retry));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Limit,"CpuLimit",threshold_monitor_limit));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_Actions,"Actions",threshold_monitor_actions));
    }

    void  CpuLocalManagedObject::setPoll(const UI32 &Poll)
    {
        m_Poll  =  Poll;
    }

    UI32  CpuLocalManagedObject::getPoll() const
    {
        return (m_Poll);
    }

    void  CpuLocalManagedObject::setRetry(const UI32 &Retry)
    {
        m_Retry  =  Retry;
    }

    UI32  CpuLocalManagedObject::getRetry() const
    {
        return (m_Retry);
    }

    void  CpuLocalManagedObject::setLimit(const UI32 &Limit)
    {
        m_Limit  =  Limit;
    }

    UI32  CpuLocalManagedObject::getLimit() const
    {
        return (m_Limit);
    }

    void  CpuLocalManagedObject::setActions(const actionType &Actions)
    {
        m_Actions  =  Actions;
    }

    actionType  CpuLocalManagedObject::getActions() const
    {
        return (m_Actions);
    }

}
