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

#include "Qos/Global/QosQueueMulticastSchedulerManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    QosQueueMulticastSchedulerManagedObject::QosQueueMulticastSchedulerManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (QosQueueMulticastSchedulerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    QosQueueMulticastSchedulerManagedObject::QosQueueMulticastSchedulerManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const SI32 &dwrrTrafficClass0,const SI32 &dwrrTrafficClass1,const SI32 &dwrrTrafficClass2,const SI32 &dwrrTrafficClass3,const SI32 &dwrrTrafficClass4,const SI32 &dwrrTrafficClass5,const SI32 &dwrrTrafficClass6,const SI32 &dwrrTrafficClass7)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (QosQueueMulticastSchedulerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
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

    QosQueueMulticastSchedulerManagedObject::~QosQueueMulticastSchedulerManagedObject ()
    {
    }

    string  QosQueueMulticastSchedulerManagedObject::getClassName()
    {
        return ("QosQueueMulticastSchedulerManagedObject");
    }

    void  QosQueueMulticastSchedulerManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0", qos_dwrr_traffic_class0));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1", qos_dwrr_traffic_class1));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2", qos_dwrr_traffic_class2));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3", qos_dwrr_traffic_class3));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4", qos_dwrr_traffic_class4));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5", qos_dwrr_traffic_class5));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6", qos_dwrr_traffic_class6));
        addPersistableAttribute (new AttributeSI32(&m_dwrrTrafficClass7,"dwrrTrafficClass7", qos_dwrr_traffic_class7));
    }

    void  QosQueueMulticastSchedulerManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass0,"dwrrTrafficClass0", qos_dwrr_traffic_class0));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass1,"dwrrTrafficClass1", qos_dwrr_traffic_class1));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass2,"dwrrTrafficClass2", qos_dwrr_traffic_class2));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass3,"dwrrTrafficClass3", qos_dwrr_traffic_class3));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass4,"dwrrTrafficClass4", qos_dwrr_traffic_class4));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass5,"dwrrTrafficClass5", qos_dwrr_traffic_class5));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass6,"dwrrTrafficClass6", qos_dwrr_traffic_class6));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_dwrrTrafficClass7,"dwrrTrafficClass7", qos_dwrr_traffic_class7));
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass0(const SI32 &dwrrTrafficClass0)
    {
        m_dwrrTrafficClass0  =  dwrrTrafficClass0;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass0() const
    {
        return (m_dwrrTrafficClass0);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass1(const SI32 &dwrrTrafficClass1)
    {
        m_dwrrTrafficClass1  =  dwrrTrafficClass1;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass1() const
    {
        return (m_dwrrTrafficClass1);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass2(const SI32 &dwrrTrafficClass2)
    {
        m_dwrrTrafficClass2  =  dwrrTrafficClass2;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass2() const
    {
        return (m_dwrrTrafficClass2);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass3(const SI32 &dwrrTrafficClass3)
    {
        m_dwrrTrafficClass3  =  dwrrTrafficClass3;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass3() const
    {
        return (m_dwrrTrafficClass3);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass4(const SI32 &dwrrTrafficClass4)
    {
        m_dwrrTrafficClass4  =  dwrrTrafficClass4;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass4() const
    {
        return (m_dwrrTrafficClass4);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass5(const SI32 &dwrrTrafficClass5)
    {
        m_dwrrTrafficClass5  =  dwrrTrafficClass5;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass5() const
    {
        return (m_dwrrTrafficClass5);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass6(const SI32 &dwrrTrafficClass6)
    {
        m_dwrrTrafficClass6  =  dwrrTrafficClass6;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass6() const
    {
        return (m_dwrrTrafficClass6);
    }

    void  QosQueueMulticastSchedulerManagedObject::setDwrrTrafficClass7(const SI32 &dwrrTrafficClass7)
    {
        m_dwrrTrafficClass7  =  dwrrTrafficClass7;
    }

    SI32  QosQueueMulticastSchedulerManagedObject::getDwrrTrafficClass7() const
    {
        return (m_dwrrTrafficClass7);
    }

}
