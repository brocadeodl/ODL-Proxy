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

#include "Qos/Global/QosGlobalCreateQosRcvQueueMulticastSettingMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCreateQosRcvQueueMulticastSettingMessage::QosGlobalCreateQosRcvQueueMulticastSettingMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEQOSRCVQUEUEMULTICASTSETTING)
    {
    }

    QosGlobalCreateQosRcvQueueMulticastSettingMessage::QosGlobalCreateQosRcvQueueMulticastSettingMessage
	  (const SI32 &rateLimit,const SI32 &burst,const SI32
	   &thresholdTrafficClass0,const SI32 &thresholdTrafficClass1,const SI32
	   &thresholdTrafficClass2,const SI32 &thresholdTrafficClass3,
	   const SI32 &thresholdTrafficClass4,const SI32 &thresholdTrafficClass5,const SI32 &thresholdTrafficClass6,const SI32 &thresholdTrafficClass7,
	   const SI32 &opCode)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEQOSRCVQUEUEMULTICASTSETTING),
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

    QosGlobalCreateQosRcvQueueMulticastSettingMessage::~QosGlobalCreateQosRcvQueueMulticastSettingMessage ()
    {
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setRateLimit(const SI32 &rateLimit)
    {
        m_rateLimit  =  rateLimit;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getRateLimit() const
    {
        return (m_rateLimit);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setBurst(const SI32 &burst)
    {
        m_burst  =  burst;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getBurst() const
    {
        return (m_burst);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass0(const SI32 &thresholdTrafficClass0)
    {
        m_thresholdTrafficClass0  =  thresholdTrafficClass0;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass0() const
    {
        return (m_thresholdTrafficClass0);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass1(const SI32 &thresholdTrafficClass1)
    {
        m_thresholdTrafficClass1  =  thresholdTrafficClass1;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass1() const
    {
        return (m_thresholdTrafficClass1);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass2(const SI32 &thresholdTrafficClass2)
    {
        m_thresholdTrafficClass2  =  thresholdTrafficClass2;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass2() const
    {
        return (m_thresholdTrafficClass2);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass3(const SI32 &thresholdTrafficClass3)
    {
        m_thresholdTrafficClass3  =  thresholdTrafficClass3;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass3() const
    {
        return (m_thresholdTrafficClass3);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass4(const SI32 &thresholdTrafficClass4)
    {
        m_thresholdTrafficClass4  =  thresholdTrafficClass4;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass4() const
    {
        return (m_thresholdTrafficClass4);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass5(const SI32 &thresholdTrafficClass5)
    {
        m_thresholdTrafficClass5  =  thresholdTrafficClass5;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass5() const
    {
        return (m_thresholdTrafficClass5);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass6(const SI32 &thresholdTrafficClass6)
    {
        m_thresholdTrafficClass6  =  thresholdTrafficClass6;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass6() const
    {
        return (m_thresholdTrafficClass6);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setThresholdTrafficClass7(const SI32 &thresholdTrafficClass7)
    {
        m_thresholdTrafficClass7  =  thresholdTrafficClass7;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getThresholdTrafficClass7() const
    {
        return (m_thresholdTrafficClass7);
    }

    void  QosGlobalCreateQosRcvQueueMulticastSettingMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosGlobalCreateQosRcvQueueMulticastSettingMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
