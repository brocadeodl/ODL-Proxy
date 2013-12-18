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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Qos/Global/QosGlobalCreateQosQueueSchedulerMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCreateQosQueueSchedulerMessage::QosGlobalCreateQosQueueSchedulerMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEQOSQUEUESCHEDULER)
    {
    }

    QosGlobalCreateQosQueueSchedulerMessage::QosGlobalCreateQosQueueSchedulerMessage (const SI32 &strictPriority,const SI32 &dwrrTrafficClass0,const SI32 &dwrrTrafficClass1,const SI32 &dwrrTrafficClass2,const SI32 &dwrrTrafficClass3,const SI32 &dwrrTrafficClass4,const SI32 &dwrrTrafficClass5,const SI32 &dwrrTrafficClass6,const SI32 &dwrrTrafficClass7)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEQOSQUEUESCHEDULER),
        m_strictPriority    (strictPriority),
        m_dwrrTrafficClass0    (dwrrTrafficClass0),
        m_dwrrTrafficClass1    (dwrrTrafficClass1),
        m_dwrrTrafficClass2    (dwrrTrafficClass2),
        m_dwrrTrafficClass3    (dwrrTrafficClass3),
        m_dwrrTrafficClass4    (dwrrTrafficClass4),
        m_dwrrTrafficClass5    (dwrrTrafficClass5),
        m_dwrrTrafficClass6    (dwrrTrafficClass6),
        m_dwrrTrafficClass7    (dwrrTrafficClass7)
    {
    }

    QosGlobalCreateQosQueueSchedulerMessage::~QosGlobalCreateQosQueueSchedulerMessage ()
    {
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_strictPriority,"strictPriority"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass7,"dwrrTrafficClass7"));
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setStrictPriority(const SI32 &strictPriority)
    {
        m_strictPriority  =  strictPriority;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getStrictPriority() const
    {
        return (m_strictPriority);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass0(const SI32 &dwrrTrafficClass0)
    {
        m_dwrrTrafficClass0  =  dwrrTrafficClass0;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass0() const
    {
        return (m_dwrrTrafficClass0);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass1(const SI32 &dwrrTrafficClass1)
    {
        m_dwrrTrafficClass1  =  dwrrTrafficClass1;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass1() const
    {
        return (m_dwrrTrafficClass1);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass2(const SI32 &dwrrTrafficClass2)
    {
        m_dwrrTrafficClass2  =  dwrrTrafficClass2;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass2() const
    {
        return (m_dwrrTrafficClass2);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass3(const SI32 &dwrrTrafficClass3)
    {
        m_dwrrTrafficClass3  =  dwrrTrafficClass3;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass3() const
    {
        return (m_dwrrTrafficClass3);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass4(const SI32 &dwrrTrafficClass4)
    {
        m_dwrrTrafficClass4  =  dwrrTrafficClass4;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass4() const
    {
        return (m_dwrrTrafficClass4);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass5(const SI32 &dwrrTrafficClass5)
    {
        m_dwrrTrafficClass5  =  dwrrTrafficClass5;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass5() const
    {
        return (m_dwrrTrafficClass5);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass6(const SI32 &dwrrTrafficClass6)
    {
        m_dwrrTrafficClass6  =  dwrrTrafficClass6;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass6() const
    {
        return (m_dwrrTrafficClass6);
    }

    void  QosGlobalCreateQosQueueSchedulerMessage::setDwrrTrafficClass7(const SI32 &dwrrTrafficClass7)
    {
        m_dwrrTrafficClass7  =  dwrrTrafficClass7;
    }

    SI32  QosGlobalCreateQosQueueSchedulerMessage::getDwrrTrafficClass7() const
    {
        return (m_dwrrTrafficClass7);
    }

}
