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

#include "Qos/Global/QosGlobalCreateCosToTrafficClassMapMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCreateCosToTrafficClassMapMessage::QosGlobalCreateCosToTrafficClassMapMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATECOSTOTRAFFICCLASSMAP)
    {
    }

    QosGlobalCreateCosToTrafficClassMapMessage::QosGlobalCreateCosToTrafficClassMapMessage (const string &cosToTrafficClassMapName,const SI32 &cos0MappedTrafficClassValue,const SI32 &cos1MappedTrafficClassValue,const SI32 &cos2MappedTrafficClassValue,const SI32 &cos3MappedTrafficClassValue,const SI32 &cos4MappedTrafficClassValue,const SI32 &cos5MappedTrafficClassValue,const SI32 &cos6MappedTrafficClassValue,const SI32 &cos7MappedTrafficClassValue)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATECOSTOTRAFFICCLASSMAP),
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

    QosGlobalCreateCosToTrafficClassMapMessage::~QosGlobalCreateCosToTrafficClassMapMessage ()
    {
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void QosGlobalCreateCosToTrafficClassMapMessage::setCosToTrafficClassMapName(const string &cosToTrafficClassMapName)
    {
        m_cosToTrafficClassMapName  =  cosToTrafficClassMapName;
    }

    string  QosGlobalCreateCosToTrafficClassMapMessage::getCosToTrafficClassMapName() const
    {
        return (m_cosToTrafficClassMapName);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos0MappedTrafficClassValue(const SI32 &cos0MappedTrafficClassValue)
    {
        m_cos0MappedTrafficClassValue  =  cos0MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos0MappedTrafficClassValue() const
    {
        return (m_cos0MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos1MappedTrafficClassValue(const SI32 &cos1MappedTrafficClassValue)
    {
        m_cos1MappedTrafficClassValue  =  cos1MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos1MappedTrafficClassValue() const
    {
        return (m_cos1MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos2MappedTrafficClassValue(const SI32 &cos2MappedTrafficClassValue)
    {
        m_cos2MappedTrafficClassValue  =  cos2MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos2MappedTrafficClassValue() const
    {
        return (m_cos2MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos3MappedTrafficClassValue(const SI32 &cos3MappedTrafficClassValue)
    {
        m_cos3MappedTrafficClassValue  =  cos3MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos3MappedTrafficClassValue() const
    {
        return (m_cos3MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos4MappedTrafficClassValue(const SI32 &cos4MappedTrafficClassValue)
    {
        m_cos4MappedTrafficClassValue  =  cos4MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos4MappedTrafficClassValue() const
    {
        return (m_cos4MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos5MappedTrafficClassValue(const SI32 &cos5MappedTrafficClassValue)
    {
        m_cos5MappedTrafficClassValue  =  cos5MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos5MappedTrafficClassValue() const
    {
        return (m_cos5MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos6MappedTrafficClassValue(const SI32 &cos6MappedTrafficClassValue)
    {
        m_cos6MappedTrafficClassValue  =  cos6MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos6MappedTrafficClassValue() const
    {
        return (m_cos6MappedTrafficClassValue);
    }

    void  QosGlobalCreateCosToTrafficClassMapMessage::setCos7MappedTrafficClassValue(const SI32 &cos7MappedTrafficClassValue)
    {
        m_cos7MappedTrafficClassValue  =  cos7MappedTrafficClassValue;
    }

    SI32  QosGlobalCreateCosToTrafficClassMapMessage::getCos7MappedTrafficClassValue() const
    {
        return (m_cos7MappedTrafficClassValue);
    }

}
