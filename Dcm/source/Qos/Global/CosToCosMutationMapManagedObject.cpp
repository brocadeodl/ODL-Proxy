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

#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "vcs.h"

namespace DcmNs
{

    CosToCosMutationMapManagedObject::CosToCosMutationMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (CosToCosMutationMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    CosToCosMutationMapManagedObject::CosToCosMutationMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &cosToCosMutationMapName,const SI32 &cos0MutatedCosValue,const SI32 &cos1MutatedCosValue,const SI32 &cos2MutatedCosValue,const SI32 &cos3MutatedCosValue,const SI32 &cos4MutatedCosValue,const SI32 &cos5MutatedCosValue,const SI32 &cos6MutatedCosValue,const SI32 &cos7MutatedCosValue)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (CosToCosMutationMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
        m_cosToCosMutationMapName    (cosToCosMutationMapName),
        m_cos0MutatedCosValue    (cos0MutatedCosValue),
        m_cos1MutatedCosValue    (cos1MutatedCosValue),
        m_cos2MutatedCosValue    (cos2MutatedCosValue),
        m_cos3MutatedCosValue    (cos3MutatedCosValue),
        m_cos4MutatedCosValue    (cos4MutatedCosValue),
        m_cos5MutatedCosValue    (cos5MutatedCosValue),
        m_cos6MutatedCosValue    (cos6MutatedCosValue),
        m_cos7MutatedCosValue    (cos7MutatedCosValue)
    {
    }

    CosToCosMutationMapManagedObject::~CosToCosMutationMapManagedObject ()
    {
    }

    string  CosToCosMutationMapManagedObject::getClassName()
    {
        return ("CosToCosMutationMapManagedObject");
    }

    void  CosToCosMutationMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addPersistableAttribute (new AttributeSI32(&m_cos0MutatedCosValue,"cos0MutatedCosValue", qos_cos0));
        addPersistableAttribute (new AttributeSI32(&m_cos1MutatedCosValue,"cos1MutatedCosValue", qos_cos1));
        addPersistableAttribute (new AttributeSI32(&m_cos2MutatedCosValue,"cos2MutatedCosValue", qos_cos2));
        addPersistableAttribute (new AttributeSI32(&m_cos3MutatedCosValue,"cos3MutatedCosValue", qos_cos3));
        addPersistableAttribute (new AttributeSI32(&m_cos4MutatedCosValue,"cos4MutatedCosValue", qos_cos4));
        addPersistableAttribute (new AttributeSI32(&m_cos5MutatedCosValue,"cos5MutatedCosValue", qos_cos5));
        addPersistableAttribute (new AttributeSI32(&m_cos6MutatedCosValue,"cos6MutatedCosValue", qos_cos6));
        addPersistableAttribute (new AttributeSI32(&m_cos7MutatedCosValue,"cos7MutatedCosValue", qos_cos7));
		setUserTagForAttribute("cosToCosMutationMapName", qos_name);
    }

    void  CosToCosMutationMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos0MutatedCosValue,"cos0MutatedCosValue", qos_cos0));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos1MutatedCosValue,"cos1MutatedCosValue", qos_cos1));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos2MutatedCosValue,"cos2MutatedCosValue", qos_cos2));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos3MutatedCosValue,"cos3MutatedCosValue", qos_cos3));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos4MutatedCosValue,"cos4MutatedCosValue", qos_cos4));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos5MutatedCosValue,"cos5MutatedCosValue", qos_cos5));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos6MutatedCosValue,"cos6MutatedCosValue", qos_cos6));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos7MutatedCosValue,"cos7MutatedCosValue", qos_cos7));


		vector<string > keyName;
		keyName.push_back ("cosToCosMutationMapName");
		setUserDefinedKeyCombination (keyName);

    }

    void  CosToCosMutationMapManagedObject::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  CosToCosMutationMapManagedObject::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  CosToCosMutationMapManagedObject::setCos0MutatedCosValue(const SI32 &cos0MutatedCosValue)
    {
        m_cos0MutatedCosValue  =  cos0MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos0MutatedCosValue() const
    {
        return (m_cos0MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos1MutatedCosValue(const SI32 &cos1MutatedCosValue)
    {
        m_cos1MutatedCosValue  =  cos1MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos1MutatedCosValue() const
    {
        return (m_cos1MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos2MutatedCosValue(const SI32 &cos2MutatedCosValue)
    {
        m_cos2MutatedCosValue  =  cos2MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos2MutatedCosValue() const
    {
        return (m_cos2MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos3MutatedCosValue(const SI32 &cos3MutatedCosValue)
    {
        m_cos3MutatedCosValue  =  cos3MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos3MutatedCosValue() const
    {
        return (m_cos3MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos4MutatedCosValue(const SI32 &cos4MutatedCosValue)
    {
        m_cos4MutatedCosValue  =  cos4MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos4MutatedCosValue() const
    {
        return (m_cos4MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos5MutatedCosValue(const SI32 &cos5MutatedCosValue)
    {
        m_cos5MutatedCosValue  =  cos5MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos5MutatedCosValue() const
    {
        return (m_cos5MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos6MutatedCosValue(const SI32 &cos6MutatedCosValue)
    {
        m_cos6MutatedCosValue  =  cos6MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos6MutatedCosValue() const
    {
        return (m_cos6MutatedCosValue);
    }

    void  CosToCosMutationMapManagedObject::setCos7MutatedCosValue(const SI32 &cos7MutatedCosValue)
    {
        m_cos7MutatedCosValue  =  cos7MutatedCosValue;
    }

    SI32  CosToCosMutationMapManagedObject::getCos7MutatedCosValue() const
    {
        return (m_cos7MutatedCosValue);
    }

}
