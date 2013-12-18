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
#include "ThresholdMonitor/Local/ThresholdMonitorSetCpuMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    ThresholdMonitorSetCpuMessage::ThresholdMonitorSetCpuMessage ()
        : PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), THRESHOLDMONITORSETCPU)
    {
    }

    ThresholdMonitorSetCpuMessage::ThresholdMonitorSetCpuMessage (const UI32 &Poll,const UI32 &Retry,const UI32 &Limit,const actionType &Actions)
        : PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), THRESHOLDMONITORSETCPU),
        m_Poll    (Poll),
        m_Retry    (Retry),
        m_Limit    (Limit),
        m_Actions    (Actions)
    {
    }

    ThresholdMonitorSetCpuMessage::~ThresholdMonitorSetCpuMessage ()
    {
    }

    void  ThresholdMonitorSetCpuMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_Poll,"Poll"));
        addSerializableAttribute (new AttributeUI32(&m_Retry,"Retry"));
        addSerializableAttribute (new AttributeUI32(&m_Limit,"Limit"));
        addSerializableAttribute (new AttributeEnum( (UI32 *)&m_Actions,"Actions"));
    }

    void  ThresholdMonitorSetCpuMessage::setPoll(const UI32 &Poll)
    {
        m_Poll  =  Poll;
    }

    UI32  ThresholdMonitorSetCpuMessage::getPoll() const
    {
        return (m_Poll);
    }

    void  ThresholdMonitorSetCpuMessage::setRetry(const UI32 &Retry)
    {
        m_Retry  =  Retry;
    }

    UI32  ThresholdMonitorSetCpuMessage::getRetry() const
    {
        return (m_Retry);
    }

    void  ThresholdMonitorSetCpuMessage::setLimit(const UI32 &Limit)
    {
        m_Limit  =  Limit;
    }

    UI32  ThresholdMonitorSetCpuMessage::getLimit() const
    {
        return (m_Limit);
    }

    void  ThresholdMonitorSetCpuMessage::setActions(const actionType &Actions)
    {
        m_Actions  =  Actions;
    }

    actionType  ThresholdMonitorSetCpuMessage::getActions() const
    {
        return (m_Actions);
    }

}
