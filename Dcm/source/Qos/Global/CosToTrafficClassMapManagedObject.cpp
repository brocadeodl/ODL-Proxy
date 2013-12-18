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

#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    CosToTrafficClassMapManagedObject::CosToTrafficClassMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (CosToTrafficClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    CosToTrafficClassMapManagedObject::CosToTrafficClassMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &cosToTrafficClassMapName,const SI32 &cos0MappedTrafficClassValue,const SI32 &cos1MappedTrafficClassValue,const SI32 &cos2MappedTrafficClassValue,const SI32 &cos3MappedTrafficClassValue,const SI32 &cos4MappedTrafficClassValue,const SI32 &cos5MappedTrafficClassValue,const SI32 &cos6MappedTrafficClassValue,const SI32 &cos7MappedTrafficClassValue)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (CosToTrafficClassMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_cosToTrafficClassMapName    (cosToTrafficClassMapName),
        m_cos0MappedTrafficClassValue    (cos0MappedTrafficClassValue),
        m_cos1MappedTrafficClassValue    (cos1MappedTrafficClassValue),
        m_cos2MappedTrafficClassValue    (cos2MappedTrafficClassValue),
        m_cos3MappedTrafficClassValue    (cos3MappedTrafficClassValue),
        m_cos4MappedTrafficClassValue    (cos4MappedTrafficClassValue),
        m_cos5MappedTrafficClassValue    (cos5MappedTrafficClassValue),
        m_cos6MappedTrafficClassValue    (cos6MappedTrafficClassValue),
        m_cos7MappedTrafficClassValue    (cos7MappedTrafficClassValue)
    {
    }

    CosToTrafficClassMapManagedObject::~CosToTrafficClassMapManagedObject ()
    {
    }

    string  CosToTrafficClassMapManagedObject::getClassName()
    {
        return ("CosToTrafficClassMapManagedObject");
    }

    void  CosToTrafficClassMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_cosToTrafficClassMapName,"cosToTrafficClassMapName"));
        addPersistableAttribute (new AttributeSI32(&m_cos0MappedTrafficClassValue,"cos0MappedTrafficClassValue", qos_cos0));
        addPersistableAttribute (new AttributeSI32(&m_cos1MappedTrafficClassValue,"cos1MappedTrafficClassValue", qos_cos1));
        addPersistableAttribute (new AttributeSI32(&m_cos2MappedTrafficClassValue,"cos2MappedTrafficClassValue", qos_cos2));
        addPersistableAttribute (new AttributeSI32(&m_cos3MappedTrafficClassValue,"cos3MappedTrafficClassValue", qos_cos3));
        addPersistableAttribute (new AttributeSI32(&m_cos4MappedTrafficClassValue,"cos4MappedTrafficClassValue", qos_cos4));
        addPersistableAttribute (new AttributeSI32(&m_cos5MappedTrafficClassValue,"cos5MappedTrafficClassValue", qos_cos5));
        addPersistableAttribute (new AttributeSI32(&m_cos6MappedTrafficClassValue,"cos6MappedTrafficClassValue", qos_cos6));
        addPersistableAttribute (new AttributeSI32(&m_cos7MappedTrafficClassValue,"cos7MappedTrafficClassValue", qos_cos7));
		setUserTagForAttribute("cosToTrafficClassMapName", qos_name);
    }

    void  CosToTrafficClassMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_cosToTrafficClassMapName,"cosToTrafficClassMapName"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos0MappedTrafficClassValue,"cos0MappedTrafficClassValue", qos_cos0));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos1MappedTrafficClassValue,"cos1MappedTrafficClassValue", qos_cos1));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos2MappedTrafficClassValue,"cos2MappedTrafficClassValue", qos_cos2));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos3MappedTrafficClassValue,"cos3MappedTrafficClassValue", qos_cos3));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos4MappedTrafficClassValue,"cos4MappedTrafficClassValue", qos_cos4));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos5MappedTrafficClassValue,"cos5MappedTrafficClassValue", qos_cos5));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos6MappedTrafficClassValue,"cos6MappedTrafficClassValue", qos_cos6));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos7MappedTrafficClassValue,"cos7MappedTrafficClassValue", qos_cos7));

		vector<string > keyName;
		keyName.push_back ("cosToTrafficClassMapName");
		setUserDefinedKeyCombination (keyName);
    }

    void  CosToTrafficClassMapManagedObject::setCosToTrafficClassMapName(const string &cosToTrafficClassMapName)
    {
        m_cosToTrafficClassMapName  =  cosToTrafficClassMapName;
    }

    string  CosToTrafficClassMapManagedObject::getCosToTrafficClassMapName() const
    {
        return (m_cosToTrafficClassMapName);
    }

    void  CosToTrafficClassMapManagedObject::setCos0MappedTrafficClassValue(const SI32 &cos0MappedTrafficClassValue)
    {
        m_cos0MappedTrafficClassValue  =  cos0MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos0MappedTrafficClassValue() const
    {
        return (m_cos0MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos1MappedTrafficClassValue(const SI32 &cos1MappedTrafficClassValue)
    {
        m_cos1MappedTrafficClassValue  =  cos1MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos1MappedTrafficClassValue() const
    {
        return (m_cos1MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos2MappedTrafficClassValue(const SI32 &cos2MappedTrafficClassValue)
    {
        m_cos2MappedTrafficClassValue  =  cos2MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos2MappedTrafficClassValue() const
    {
        return (m_cos2MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos3MappedTrafficClassValue(const SI32 &cos3MappedTrafficClassValue)
    {
        m_cos3MappedTrafficClassValue  =  cos3MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos3MappedTrafficClassValue() const
    {
        return (m_cos3MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos4MappedTrafficClassValue(const SI32 &cos4MappedTrafficClassValue)
    {
        m_cos4MappedTrafficClassValue  =  cos4MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos4MappedTrafficClassValue() const
    {
        return (m_cos4MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos5MappedTrafficClassValue(const SI32 &cos5MappedTrafficClassValue)
    {
        m_cos5MappedTrafficClassValue  =  cos5MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos5MappedTrafficClassValue() const
    {
        return (m_cos5MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos6MappedTrafficClassValue(const SI32 &cos6MappedTrafficClassValue)
    {
        m_cos6MappedTrafficClassValue  =  cos6MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos6MappedTrafficClassValue() const
    {
        return (m_cos6MappedTrafficClassValue);
    }

    void  CosToTrafficClassMapManagedObject::setCos7MappedTrafficClassValue(const SI32 &cos7MappedTrafficClassValue)
    {
        m_cos7MappedTrafficClassValue  =  cos7MappedTrafficClassValue;
    }

    SI32  CosToTrafficClassMapManagedObject::getCos7MappedTrafficClassValue() const
    {
        return (m_cos7MappedTrafficClassValue);
    }

}
