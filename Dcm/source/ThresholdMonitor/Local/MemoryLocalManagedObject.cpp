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

#include "ThresholdMonitor/Local/MemoryLocalManagedObject.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "vcs.h"

namespace DcmNs
{

    MemoryLocalManagedObject::MemoryLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager)
        : PrismElement  (pThresholdMonitorLocalObjectManager),
        PrismPersistableObject (MemoryLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pThresholdMonitorLocalObjectManager),
        DcmManagedObject (pThresholdMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pThresholdMonitorLocalObjectManager)
    {
    }

    MemoryLocalManagedObject::MemoryLocalManagedObject (ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager,const UI32 &Poll,const UI32 &Retry,const UI32 &Limit,const UI32 &Highlimit,const UI32 &Lowlimit,const actionType &Actions)
        : PrismElement  (pThresholdMonitorLocalObjectManager),
        PrismPersistableObject (MemoryLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pThresholdMonitorLocalObjectManager),
        DcmManagedObject (pThresholdMonitorLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pThresholdMonitorLocalObjectManager),
        m_Poll    (Poll),
        m_Retry    (Retry),
        m_Limit    (Limit),
        m_Highlimit    (Highlimit),
        m_Lowlimit    (Lowlimit),
        m_Actions    (Actions)
    {
    }

    MemoryLocalManagedObject::~MemoryLocalManagedObject ()
    {
    }

    string  MemoryLocalManagedObject::getClassName()
    {
        return ("MemoryLocalManagedObject");
    }

    void  MemoryLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_Poll,"Poll",threshold_monitor_poll));
        addPersistableAttribute (new AttributeUI32(&m_Retry,"Retry",threshold_monitor_retry));
        addPersistableAttribute (new AttributeUI32(&m_Limit,"MemoryLimit",threshold_monitor_limit));
        addPersistableAttribute (new AttributeUI32(&m_Highlimit,"Highlimit",threshold_monitor_high_limit));
        addPersistableAttribute (new AttributeUI32(&m_Lowlimit,"Lowlimit",threshold_monitor_low_limit));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_Actions,"Actions",threshold_monitor_actions));
    }

    void  MemoryLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Poll,"Poll",threshold_monitor_poll));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Retry,"Retry",threshold_monitor_retry));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Limit,"MemoryLimit",threshold_monitor_limit));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Highlimit,"Highlimit",threshold_monitor_high_limit));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Lowlimit,"Lowlimit",threshold_monitor_low_limit));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_Actions,"Actions",threshold_monitor_actions));
    }

    void  MemoryLocalManagedObject::setPoll(const UI32 &Poll)
    {
        m_Poll  =  Poll;
    }

    UI32  MemoryLocalManagedObject::getPoll() const
    {
        return (m_Poll);
    }

    void  MemoryLocalManagedObject::setRetry(const UI32 &Retry)
    {
        m_Retry  =  Retry;
    }

    UI32  MemoryLocalManagedObject::getRetry() const
    {
        return (m_Retry);
    }

    void  MemoryLocalManagedObject::setLimit(const UI32 &Limit)
    {
        m_Limit  =  Limit;
    }

    UI32  MemoryLocalManagedObject::getLimit() const
    {
        return (m_Limit);
    }

    void  MemoryLocalManagedObject::setHighlimit(const UI32 &Highlimit)
    {
        m_Highlimit  =  Highlimit;
    }

    UI32  MemoryLocalManagedObject::getHighlimit() const
    {
        return (m_Highlimit);
    }

    void  MemoryLocalManagedObject::setLowlimit(const UI32 &Lowlimit)
    {
        m_Lowlimit  =  Lowlimit;
    }

    UI32  MemoryLocalManagedObject::getLowlimit() const
    {
        return (m_Lowlimit);
    }

    void  MemoryLocalManagedObject::setActions(const actionType &Actions)
    {
        m_Actions  =  Actions;
    }

    actionType  MemoryLocalManagedObject::getActions() const
    {
        return (m_Actions);
    }

}
