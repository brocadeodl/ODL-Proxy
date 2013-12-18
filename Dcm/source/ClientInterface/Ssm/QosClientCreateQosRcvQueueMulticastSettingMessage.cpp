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


#include "ClientInterface/Ssm/QosClientCreateQosRcvQueueMulticastSettingMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateQosRcvQueueMulticastSettingMessage::QosClientCreateQosRcvQueueMulticastSettingMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEQOSRCVQUEUEMULTICASTSETTING)
    {
    }

    QosClientCreateQosRcvQueueMulticastSettingMessage::QosClientCreateQosRcvQueueMulticastSettingMessage
	  (const SI32 &rateLimit,const SI32 &burst,const SI32
	   &thresholdTrafficClass0,const SI32 &thresholdTrafficClass1,const SI32
	   &thresholdTrafficClass2,const SI32 &thresholdTrafficClass3,
	   const SI32 &thresholdTrafficClass4,const SI32
	   &thresholdTrafficClass5,const SI32 &thresholdTrafficClass6,const SI32
	   &thresholdTrafficClass7,
	   const SI32 &opCode)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEQOSRCVQUEUEMULTICASTSETTING),
        m_rateLimit    (rateLimit),
        m_burst    (burst),
        m_thresholdTrafficClass0    (thresholdTrafficClass0),
        m_thresholdTrafficClass1    (thresholdTrafficClass1),
        m_thresholdTrafficClass2    (thresholdTrafficClass2),
        m_thresholdTrafficClass3    (thresholdTrafficClass3),
        m_thresholdTrafficClass4    (thresholdTrafficClass4),
        m_thresholdTrafficClass5    (thresholdTrafficClass5),
        m_thresholdTrafficClass6    (thresholdTrafficClass6),
        m_thresholdTrafficClass7    (thresholdTrafficClass7),
		
        m_opCode    (opCode)
    {
    }

    QosClientCreateQosRcvQueueMulticastSettingMessage::~QosClientCreateQosRcvQueueMulticastSettingMessage ()
    {
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_rateLimit,"rateLimit"));
        addSerializableAttribute (new AttributeSI32(&m_burst,"burst"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass0,"thresholdTrafficClass0"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass1,"thresholdTrafficClass1"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass2,"thresholdTrafficClass2"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass3,"thresholdTrafficClass3"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass4,"thresholdTrafficClass4"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass5,"thresholdTrafficClass5"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass6,"thresholdTrafficClass6"));
        addSerializableAttribute (new AttributeSI32(&m_thresholdTrafficClass7,"thresholdTrafficClass7"));
        addSerializableAttribute (new AttributeSI32(&m_opCode,"opCode"));
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setRateLimit(const SI32 &rateLimit)
    {
        m_rateLimit  =  rateLimit;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getRateLimit() const
    {
        return (m_rateLimit);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setBurst(const SI32 &burst)
    {
        m_burst  =  burst;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getBurst() const
    {
        return (m_burst);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass0(const SI32 &thresholdTrafficClass0)
    {
        m_thresholdTrafficClass0  =  thresholdTrafficClass0;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass0() const
    {
        return (m_thresholdTrafficClass0);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass1(const SI32 &thresholdTrafficClass1)
    {
        m_thresholdTrafficClass1  =  thresholdTrafficClass1;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass1() const
    {
        return (m_thresholdTrafficClass1);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass2(const SI32 &thresholdTrafficClass2)
    {
        m_thresholdTrafficClass2  =  thresholdTrafficClass2;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass2() const
    {
        return (m_thresholdTrafficClass2);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass3(const SI32 &thresholdTrafficClass3)
    {
        m_thresholdTrafficClass3  =  thresholdTrafficClass3;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass3() const
    {
        return (m_thresholdTrafficClass3);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass4(const SI32 &thresholdTrafficClass4)
    {
        m_thresholdTrafficClass4  =  thresholdTrafficClass4;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass4() const
    {
        return (m_thresholdTrafficClass4);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass5(const SI32 &thresholdTrafficClass5)
    {
        m_thresholdTrafficClass5  =  thresholdTrafficClass5;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass5() const
    {
        return (m_thresholdTrafficClass5);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass6(const SI32 &thresholdTrafficClass6)
    {
        m_thresholdTrafficClass6  =  thresholdTrafficClass6;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass6() const
    {
        return (m_thresholdTrafficClass6);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass7(const SI32 &thresholdTrafficClass7)
    {
        m_thresholdTrafficClass7  =  thresholdTrafficClass7;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass7() const
    {
        return (m_thresholdTrafficClass7);
    }

    void  QosClientCreateQosRcvQueueMulticastSettingMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosClientCreateQosRcvQueueMulticastSettingMessage::getOpCode() const
    {
        return (m_opCode);
    }

	const void *QosClientCreateQosRcvQueueMulticastSettingMessage::getCStructureForInputs ()
    {
        QosRcvQueueMulticastMsg_t *pInput = (QosRcvQueueMulticastMsg_ *)malloc (sizeof (struct QosRcvQueueMulticastMsg_));

        pInput->rateLimit= m_rateLimit;
        pInput->burst = m_burst;
        pInput->thresholdTrafficClass0= m_thresholdTrafficClass0;
        pInput->thresholdTrafficClass1= m_thresholdTrafficClass1;
        pInput->thresholdTrafficClass2= m_thresholdTrafficClass2;
        pInput->thresholdTrafficClass3= m_thresholdTrafficClass3;
        pInput->thresholdTrafficClass4= m_thresholdTrafficClass4;
        pInput->thresholdTrafficClass5= m_thresholdTrafficClass5;
        pInput->thresholdTrafficClass6= m_thresholdTrafficClass6;
        pInput->thresholdTrafficClass7= m_thresholdTrafficClass7;
		pInput->opCode = m_opCode; 

        return (pInput);
    }

    void QosClientCreateQosRcvQueueMulticastSettingMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
