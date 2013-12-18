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

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosCreateQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateQosQueueMulticastSchedulerSPMessage::QosCreateQosQueueMulticastSchedulerSPMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId(),QOSCREATEQOSQUEUEMULTICASTSCHEDULERSP)
    {
    }

    QosCreateQosQueueMulticastSchedulerSPMessage::QosCreateQosQueueMulticastSchedulerSPMessage (const SI32 &dwrrTrafficClass0,const SI32 &dwrrTrafficClass1,const SI32 &dwrrTrafficClass2,const SI32 &dwrrTrafficClass3,const SI32 &dwrrTrafficClass4,const SI32 &dwrrTrafficClass5,const SI32 &dwrrTrafficClass6,const SI32 &dwrrTrafficClass7)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId(),QOSCREATEQOSQUEUEMULTICASTSCHEDULERSP),
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

    QosCreateQosQueueMulticastSchedulerSPMessage::~QosCreateQosQueueMulticastSchedulerSPMessage ()
    {
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass7,"dwrrTrafficClass7"));
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass0(const SI32 &dwrrTrafficClass0)
    {
        m_dwrrTrafficClass0  =  dwrrTrafficClass0;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass0() const
    {
        return (m_dwrrTrafficClass0);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass1(const SI32 &dwrrTrafficClass1)
    {
        m_dwrrTrafficClass1  =  dwrrTrafficClass1;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass1() const
    {
        return (m_dwrrTrafficClass1);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass2(const SI32 &dwrrTrafficClass2)
    {
        m_dwrrTrafficClass2  =  dwrrTrafficClass2;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass2() const
    {
        return (m_dwrrTrafficClass2);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass3(const SI32 &dwrrTrafficClass3)
    {
        m_dwrrTrafficClass3  =  dwrrTrafficClass3;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass3() const
    {
        return (m_dwrrTrafficClass3);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass4(const SI32 &dwrrTrafficClass4)
    {
        m_dwrrTrafficClass4  =  dwrrTrafficClass4;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass4() const
    {
        return (m_dwrrTrafficClass4);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass5(const SI32 &dwrrTrafficClass5)
    {
        m_dwrrTrafficClass5  =  dwrrTrafficClass5;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass5() const
    {
        return (m_dwrrTrafficClass5);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass6(const SI32 &dwrrTrafficClass6)
    {
        m_dwrrTrafficClass6  =  dwrrTrafficClass6;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass6() const
    {
        return (m_dwrrTrafficClass6);
    }

    void  QosCreateQosQueueMulticastSchedulerSPMessage::setDwrrTrafficClass7(const SI32 &dwrrTrafficClass7)
    {
        m_dwrrTrafficClass7  =  dwrrTrafficClass7;
    }

    SI32  QosCreateQosQueueMulticastSchedulerSPMessage::getDwrrTrafficClass7() const
    {
        return (m_dwrrTrafficClass7);
    }

}
