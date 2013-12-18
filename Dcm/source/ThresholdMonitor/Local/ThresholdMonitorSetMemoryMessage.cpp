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

#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorSetMemoryMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    ThresholdMonitorSetMemoryMessage::ThresholdMonitorSetMemoryMessage ()
        : PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), THRESHOLDMONITORSETMEMORY)
    {
    }

    ThresholdMonitorSetMemoryMessage::ThresholdMonitorSetMemoryMessage (const UI32 &Poll,const UI32 &Retry,const UI32 &Limit,const UI32 &Highlimit,const UI32 &Lowlimit,const actionType &Actions)
        : PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), THRESHOLDMONITORSETMEMORY),
        m_Poll    (Poll),
        m_Retry    (Retry),
        m_Limit    (Limit),
        m_Highlimit    (Highlimit),
        m_Lowlimit    (Lowlimit),
        m_Actions    (Actions)
    {
    }

    ThresholdMonitorSetMemoryMessage::~ThresholdMonitorSetMemoryMessage ()
    {
    }

    void  ThresholdMonitorSetMemoryMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_Poll,"Poll"));
        addSerializableAttribute (new AttributeUI32(&m_Retry,"Retry"));
        addSerializableAttribute (new AttributeUI32(&m_Limit,"Limit"));
        addSerializableAttribute (new AttributeUI32(&m_Highlimit,"Highlimit"));
        addSerializableAttribute (new AttributeUI32(&m_Lowlimit,"Lowlimit"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_Actions,"Actions"));
    }

    void  ThresholdMonitorSetMemoryMessage::setPoll(const UI32 &Poll)
    {
        m_Poll  =  Poll;
    }

    UI32  ThresholdMonitorSetMemoryMessage::getPoll() const
    {
        return (m_Poll);
    }

    void  ThresholdMonitorSetMemoryMessage::setRetry(const UI32 &Retry)
    {
        m_Retry  =  Retry;
    }

    UI32  ThresholdMonitorSetMemoryMessage::getRetry() const
    {
        return (m_Retry);
    }

    void  ThresholdMonitorSetMemoryMessage::setLimit(const UI32 &Limit)
    {
        m_Limit  =  Limit;
    }

    UI32  ThresholdMonitorSetMemoryMessage::getLimit() const
    {
        return (m_Limit);
    }

    void  ThresholdMonitorSetMemoryMessage::setHighlimit(const UI32 &Highlimit)
    {
        m_Highlimit  =  Highlimit;
    }

    UI32  ThresholdMonitorSetMemoryMessage::getHighlimit() const
    {
        return (m_Highlimit);
    }

    void  ThresholdMonitorSetMemoryMessage::setLowlimit(const UI32 &Lowlimit)
    {
        m_Lowlimit  =  Lowlimit;
    }

    UI32  ThresholdMonitorSetMemoryMessage::getLowlimit() const
    {
        return (m_Lowlimit);
    }

    void  ThresholdMonitorSetMemoryMessage::setActions(const actionType &Actions)
    {
        m_Actions  =  Actions;
    }

    actionType  ThresholdMonitorSetMemoryMessage::getActions() const
    {
        return (m_Actions);
    }

}
