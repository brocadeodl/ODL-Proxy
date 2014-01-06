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

#include "ClientInterface/Ssm/QosClientCreateQosQueueMulticastSchedulerMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateQosQueueMulticastSchedulerMessage::QosClientCreateQosQueueMulticastSchedulerMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEQOSQUEUEMULTICASTSCHEDULER)
    {
    }

    QosClientCreateQosQueueMulticastSchedulerMessage::QosClientCreateQosQueueMulticastSchedulerMessage (const SI32 &dwrrTrafficClass0,const SI32 &dwrrTrafficClass1,const SI32 &dwrrTrafficClass2,const SI32 &dwrrTrafficClass3,const SI32 &dwrrTrafficClass4,const SI32 &dwrrTrafficClass5,const SI32 &dwrrTrafficClass6,const SI32 &dwrrTrafficClass7)
        : DcmManagementInterfaceMessage (QOSCLIENTCREATEQOSQUEUEMULTICASTSCHEDULER),
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

    QosClientCreateQosQueueMulticastSchedulerMessage::~QosClientCreateQosQueueMulticastSchedulerMessage ()
    {
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6"));
        addSerializableAttribute (new AttributeSI32(&m_dwrrTrafficClass7,"dwrrTrafficClass7"));
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass0(const SI32 &dwrrTrafficClass0)
    {
        m_dwrrTrafficClass0  =  dwrrTrafficClass0;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass0() const
    {
        return (m_dwrrTrafficClass0);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass1(const SI32 &dwrrTrafficClass1)
    {
        m_dwrrTrafficClass1  =  dwrrTrafficClass1;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass1() const
    {
        return (m_dwrrTrafficClass1);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass2(const SI32 &dwrrTrafficClass2)
    {
        m_dwrrTrafficClass2  =  dwrrTrafficClass2;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass2() const
    {
        return (m_dwrrTrafficClass2);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass3(const SI32 &dwrrTrafficClass3)
    {
        m_dwrrTrafficClass3  =  dwrrTrafficClass3;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass3() const
    {
        return (m_dwrrTrafficClass3);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass4(const SI32 &dwrrTrafficClass4)
    {
        m_dwrrTrafficClass4  =  dwrrTrafficClass4;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass4() const
    {
        return (m_dwrrTrafficClass4);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass5(const SI32 &dwrrTrafficClass5)
    {
        m_dwrrTrafficClass5  =  dwrrTrafficClass5;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass5() const
    {
        return (m_dwrrTrafficClass5);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass6(const SI32 &dwrrTrafficClass6)
    {
        m_dwrrTrafficClass6  =  dwrrTrafficClass6;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass6() const
    {
        return (m_dwrrTrafficClass6);
    }

    void  QosClientCreateQosQueueMulticastSchedulerMessage::setDwrrTrafficClass7(const SI32 &dwrrTrafficClass7)
    {
        m_dwrrTrafficClass7  =  dwrrTrafficClass7;
    }

    SI32  QosClientCreateQosQueueMulticastSchedulerMessage::getDwrrTrafficClass7() const
    {
        return (m_dwrrTrafficClass7);
    }

	const void *QosClientCreateQosQueueMulticastSchedulerMessage::getCStructureForInputs ()
    {
        QosMcastSchedulerMsg_t *pInput = (QosMcastSchedulerMsg_ *)malloc (sizeof (struct QosMcastSchedulerMsg_));

        pInput->dwrrTrafficClass0	= m_dwrrTrafficClass0;
        pInput->dwrrTrafficClass1	= m_dwrrTrafficClass1;
        pInput->dwrrTrafficClass2	= m_dwrrTrafficClass2;
        pInput->dwrrTrafficClass3	= m_dwrrTrafficClass3;
        pInput->dwrrTrafficClass4	= m_dwrrTrafficClass4;
        pInput->dwrrTrafficClass5	= m_dwrrTrafficClass5;
        pInput->dwrrTrafficClass6	= m_dwrrTrafficClass6;
        pInput->dwrrTrafficClass7	= m_dwrrTrafficClass7;

        return (pInput);
    }

    void QosClientCreateQosQueueMulticastSchedulerMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}