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
#include "Qos/Local/QosCreateCosToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateCosToTrafficClassMapSPMessage::QosCreateCosToTrafficClassMapSPMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId(),QOSCREATECOSTOTRAFFICCLASSMAPSP)
    {
    }

    QosCreateCosToTrafficClassMapSPMessage::QosCreateCosToTrafficClassMapSPMessage (const string &cosToTrafficClassMapName,const SI32 &cos0MappedTrafficClassValue,const SI32 &cos1MappedTrafficClassValue,const SI32 &cos2MappedTrafficClassValue,const SI32 &cos3MappedTrafficClassValue,const SI32 &cos4MappedTrafficClassValue,const SI32 &cos5MappedTrafficClassValue,const SI32 &cos6MappedTrafficClassValue,const SI32 &cos7MappedTrafficClassValue)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId(),QOSCREATECOSTOTRAFFICCLASSMAPSP),
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

    QosCreateCosToTrafficClassMapSPMessage::~QosCreateCosToTrafficClassMapSPMessage ()
    {
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_cosToTrafficClassMapName,"cosToTrafficClassMapName"));
        addSerializableAttribute (new AttributeSI32(&m_cos0MappedTrafficClassValue,"cos0MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos1MappedTrafficClassValue,"cos1MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos2MappedTrafficClassValue,"cos2MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos3MappedTrafficClassValue,"cos3MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos4MappedTrafficClassValue,"cos4MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos5MappedTrafficClassValue,"cos5MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos6MappedTrafficClassValue,"cos6MappedTrafficClassValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos7MappedTrafficClassValue,"cos7MappedTrafficClassValue"));
    }

    void
	  QosCreateCosToTrafficClassMapSPMessage::setCosToTrafficClassMapName(const string &cosToTrafficClassMapName)
    {
        m_cosToTrafficClassMapName  =  cosToTrafficClassMapName;
    }

    string  QosCreateCosToTrafficClassMapSPMessage::getCosToTrafficClassMapName() const
    {
        return (m_cosToTrafficClassMapName);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos0MappedTrafficClassValue(const SI32 &cos0MappedTrafficClassValue)
    {
        m_cos0MappedTrafficClassValue  =  cos0MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos0MappedTrafficClassValue() const
    {
        return (m_cos0MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos1MappedTrafficClassValue(const SI32 &cos1MappedTrafficClassValue)
    {
        m_cos1MappedTrafficClassValue  =  cos1MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos1MappedTrafficClassValue() const
    {
        return (m_cos1MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos2MappedTrafficClassValue(const SI32 &cos2MappedTrafficClassValue)
    {
        m_cos2MappedTrafficClassValue  =  cos2MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos2MappedTrafficClassValue() const
    {
        return (m_cos2MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos3MappedTrafficClassValue(const SI32 &cos3MappedTrafficClassValue)
    {
        m_cos3MappedTrafficClassValue  =  cos3MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos3MappedTrafficClassValue() const
    {
        return (m_cos3MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos4MappedTrafficClassValue(const SI32 &cos4MappedTrafficClassValue)
    {
        m_cos4MappedTrafficClassValue  =  cos4MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos4MappedTrafficClassValue() const
    {
        return (m_cos4MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos5MappedTrafficClassValue(const SI32 &cos5MappedTrafficClassValue)
    {
        m_cos5MappedTrafficClassValue  =  cos5MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos5MappedTrafficClassValue() const
    {
        return (m_cos5MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos6MappedTrafficClassValue(const SI32 &cos6MappedTrafficClassValue)
    {
        m_cos6MappedTrafficClassValue  =  cos6MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos6MappedTrafficClassValue() const
    {
        return (m_cos6MappedTrafficClassValue);
    }

    void  QosCreateCosToTrafficClassMapSPMessage::setCos7MappedTrafficClassValue(const SI32 &cos7MappedTrafficClassValue)
    {
        m_cos7MappedTrafficClassValue  =  cos7MappedTrafficClassValue;
    }

    SI32  QosCreateCosToTrafficClassMapSPMessage::getCos7MappedTrafficClassValue() const
    {
        return (m_cos7MappedTrafficClassValue);
    }

}
