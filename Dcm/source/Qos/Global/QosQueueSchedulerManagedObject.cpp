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

#include "Qos/Global/QosQueueSchedulerManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    QosQueueSchedulerManagedObject::QosQueueSchedulerManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (QosQueueSchedulerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
	  m_dummy = 0;
    }

    QosQueueSchedulerManagedObject::QosQueueSchedulerManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const SI32 &strictPriority,const SI32 &dwrrTrafficClass0,const SI32 &dwrrTrafficClass1,const SI32 &dwrrTrafficClass2,const SI32 &dwrrTrafficClass3,const SI32 &dwrrTrafficClass4,const SI32 &dwrrTrafficClass5,const SI32 &dwrrTrafficClass6,const SI32 &dwrrTrafficClassLast)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (QosQueueSchedulerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_strictPriority    (strictPriority),
        m_dwrrTrafficClass0    (dwrrTrafficClass0),
        m_dwrrTrafficClass1    (dwrrTrafficClass1),
        m_dwrrTrafficClass2    (dwrrTrafficClass2),
        m_dwrrTrafficClass3    (dwrrTrafficClass3),
        m_dwrrTrafficClass4    (dwrrTrafficClass4),
        m_dwrrTrafficClass5    (dwrrTrafficClass5),
        m_dwrrTrafficClass6    (dwrrTrafficClass6),
        m_dwrrTrafficClassLast    (dwrrTrafficClassLast)
    {
    }

    QosQueueSchedulerManagedObject::~QosQueueSchedulerManagedObject ()
    {
    }

    string  QosQueueSchedulerManagedObject::getClassName()
    {
        return ("QosQueueSchedulerManagedObject");
    }

    void  QosQueueSchedulerManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_strictPriority,"strictPriority", qos_priority_number));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0", qos_dwrr_traffic_class0));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1", qos_dwrr_traffic_class1));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2", qos_dwrr_traffic_class2));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3", qos_dwrr_traffic_class3));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4", qos_dwrr_traffic_class4));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5", qos_dwrr_traffic_class5));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6", qos_dwrr_traffic_class6));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClassLast,"dwrrTrafficClassLast",
			  qos_dwrr_traffic_class_last));
        addPersistableAttribute (new AttributeEnum(&m_dummy,"dummy", qos_scheduler_type));
    }

    void  QosQueueSchedulerManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_strictPriority,"strictPriority",
			  qos_priority_number));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0",
			  qos_dwrr_traffic_class0));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1",
			  qos_dwrr_traffic_class1));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2",
			  qos_dwrr_traffic_class2));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3",
			  qos_dwrr_traffic_class3));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4",
			  qos_dwrr_traffic_class4));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5",
			  qos_dwrr_traffic_class5));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6",
			  qos_dwrr_traffic_class6));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClassLast,"dwrrTrafficClassLast",
			  qos_dwrr_traffic_class_last));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_dummy,"dummy",
			  qos_scheduler_type));
    }

    void  QosQueueSchedulerManagedObject::setStrictPriority(const SI32 &strictPriority)
    {
        m_strictPriority  =  strictPriority;
    }

    SI32  QosQueueSchedulerManagedObject::getStrictPriority() const
    {
        return (m_strictPriority);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass0(const SI32 &dwrrTrafficClass0)
    {
        m_dwrrTrafficClass0  =  dwrrTrafficClass0;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass0() const
    {
        return (m_dwrrTrafficClass0);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass1(const SI32 &dwrrTrafficClass1)
    {
        m_dwrrTrafficClass1  =  dwrrTrafficClass1;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass1() const
    {
        return (m_dwrrTrafficClass1);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass2(const SI32 &dwrrTrafficClass2)
    {
        m_dwrrTrafficClass2  =  dwrrTrafficClass2;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass2() const
    {
        return (m_dwrrTrafficClass2);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass3(const SI32 &dwrrTrafficClass3)
    {
        m_dwrrTrafficClass3  =  dwrrTrafficClass3;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass3() const
    {
        return (m_dwrrTrafficClass3);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass4(const SI32 &dwrrTrafficClass4)
    {
        m_dwrrTrafficClass4  =  dwrrTrafficClass4;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass4() const
    {
        return (m_dwrrTrafficClass4);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass5(const SI32 &dwrrTrafficClass5)
    {
        m_dwrrTrafficClass5  =  dwrrTrafficClass5;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass5() const
    {
        return (m_dwrrTrafficClass5);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClass6(const SI32 &dwrrTrafficClass6)
    {
        m_dwrrTrafficClass6  =  dwrrTrafficClass6;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClass6() const
    {
        return (m_dwrrTrafficClass6);
    }

    void  QosQueueSchedulerManagedObject::setDwrrTrafficClassLast(const SI32 &dwrrTrafficClassLast)
    {
        m_dwrrTrafficClassLast  =  dwrrTrafficClassLast;
    }

    SI32  QosQueueSchedulerManagedObject::getDwrrTrafficClassLast() const
    {
        return (m_dwrrTrafficClassLast);
    }

}
