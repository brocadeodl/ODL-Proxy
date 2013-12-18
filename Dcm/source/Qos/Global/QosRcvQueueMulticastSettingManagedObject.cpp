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

#include "Qos/Global/QosRcvQueueMulticastSettingManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "vcs.h"
namespace DcmNs
{

    QosRcvQueueMulticastSettingManagedObject::QosRcvQueueMulticastSettingManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (QosRcvQueueMulticastSettingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    QosRcvQueueMulticastSettingManagedObject::QosRcvQueueMulticastSettingManagedObject
	  (QosGlobalObjectManager *pQosGlobalObjectManager,const SI32
	   &rateLimit,const SI32 &burst,const SI32 &thresholdTrafficClass0,const
	   SI32 &thresholdTrafficClass1,const SI32 &thresholdTrafficClass2,const
	   SI32 &thresholdTrafficClass3,
		const SI32 &thresholdTrafficClass4,const SI32 &thresholdTrafficClass5,const SI32 &thresholdTrafficClass6,const SI32  &thresholdTrafficClass7)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (QosRcvQueueMulticastSettingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_rateLimit    (rateLimit),
        m_burst    (burst),
        m_thresholdTrafficClass0    (thresholdTrafficClass0),
        m_thresholdTrafficClass1    (thresholdTrafficClass1),
        m_thresholdTrafficClass2    (thresholdTrafficClass2),
        m_thresholdTrafficClass3    (thresholdTrafficClass3),
        m_thresholdTrafficClass4    (thresholdTrafficClass4),
        m_thresholdTrafficClass5    (thresholdTrafficClass5),
        m_thresholdTrafficClass6    (thresholdTrafficClass6),
        m_thresholdTrafficClass7    (thresholdTrafficClass7)
    {
    }

    QosRcvQueueMulticastSettingManagedObject::~QosRcvQueueMulticastSettingManagedObject ()
    {
    }

    string  QosRcvQueueMulticastSettingManagedObject::getClassName()
    {
        return ("QosRcvQueueMulticastSettingManagedObject");
    }

    void  QosRcvQueueMulticastSettingManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_rateLimit,"rateLimit", qos_limit));
        addPersistableAttribute (new AttributeSI32(&m_burst,"burst", qos_burst));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass0,"thresholdTrafficClass0", qos_traffic_class0));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass1,"thresholdTrafficClass1", qos_traffic_class1));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass2,"thresholdTrafficClass2", qos_traffic_class2));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass3,"thresholdTrafficClass3", qos_traffic_class3));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass4,"thresholdTrafficClass4", qos_traffic_class4));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass5,"thresholdTrafficClass5", qos_traffic_class5));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass6,"thresholdTrafficClass6", qos_traffic_class6));
        addPersistableAttribute (new AttributeSI32(&m_thresholdTrafficClass7,"thresholdTrafficClass7", qos_traffic_class7));
		//setUserTagForAttribute("thresholdTrafficClass0", qos_traffic_class0);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_rateLimit,"rateLimit", qos_limit));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_burst,"burst", qos_burst));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass0,"thresholdTrafficClass0", qos_traffic_class0));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass1,"thresholdTrafficClass1", qos_traffic_class1));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass2,"thresholdTrafficClass2", qos_traffic_class2));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass3,"thresholdTrafficClass3", qos_traffic_class3));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass4,"thresholdTrafficClass4", qos_traffic_class4));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass5,"thresholdTrafficClass5", qos_traffic_class5));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass6,"thresholdTrafficClass6", qos_traffic_class6));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_thresholdTrafficClass7,"thresholdTrafficClass7", qos_traffic_class7));

		/*
		vector<string > keyName;
		keyName.push_back ("thresholdTrafficClass0");
		setUserDefinedKeyCombination (keyName);
		*/
    }

    void  QosRcvQueueMulticastSettingManagedObject::setRateLimit(const SI32 &rateLimit)
    {
        m_rateLimit  =  rateLimit;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getRateLimit() const
    {
        return (m_rateLimit);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setBurst(const SI32 &burst)
    {
        m_burst  =  burst;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getBurst() const
    {
        return (m_burst);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass0(const SI32 &thresholdTrafficClass0)
    {
        m_thresholdTrafficClass0  =  thresholdTrafficClass0;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass0() const
    {
        return (m_thresholdTrafficClass0);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass1(const SI32 &thresholdTrafficClass1)
    {
        m_thresholdTrafficClass1  =  thresholdTrafficClass1;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass1() const
    {
        return (m_thresholdTrafficClass1);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass2(const SI32 &thresholdTrafficClass2)
    {
        m_thresholdTrafficClass2  =  thresholdTrafficClass2;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass2() const
    {
        return (m_thresholdTrafficClass2);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass3(const SI32 &thresholdTrafficClass3)
    {
        m_thresholdTrafficClass3  =  thresholdTrafficClass3;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass3() const
    {
        return (m_thresholdTrafficClass3);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass4(const SI32 &thresholdTrafficClass4)
    {
        m_thresholdTrafficClass4  =  thresholdTrafficClass4;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass4() const
    {
        return (m_thresholdTrafficClass4);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass5(const SI32 &thresholdTrafficClass5)
    {
        m_thresholdTrafficClass5  =  thresholdTrafficClass5;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass5() const
    {
        return (m_thresholdTrafficClass5);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass6(const SI32 &thresholdTrafficClass6)
    {
        m_thresholdTrafficClass6  =  thresholdTrafficClass6;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass6() const
    {
        return (m_thresholdTrafficClass6);
    }

    void  QosRcvQueueMulticastSettingManagedObject::setThresholdTrafficClass7(const SI32 &thresholdTrafficClass7)
    {
        m_thresholdTrafficClass7  =  thresholdTrafficClass7;
    }

    SI32  QosRcvQueueMulticastSettingManagedObject::getThresholdTrafficClass7() const
    {
        return (m_thresholdTrafficClass7);
    }

}
