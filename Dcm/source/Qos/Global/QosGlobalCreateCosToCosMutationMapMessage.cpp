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

#include "Qos/Global/QosGlobalCreateCosToCosMutationMapMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCreateCosToCosMutationMapMessage::QosGlobalCreateCosToCosMutationMapMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATECOSTOCOSMUTATIONMAP)
    {
    }

    QosGlobalCreateCosToCosMutationMapMessage::QosGlobalCreateCosToCosMutationMapMessage (const string &cosToCosMutationMapName,const SI32 &cos0MutatedCosValue,const SI32 &cos1MutatedCosValue,const SI32 &cos2MutatedCosValue,const SI32 &cos3MutatedCosValue,const SI32 &cos4MutatedCosValue,const SI32 &cos5MutatedCosValue,const SI32 &cos6MutatedCosValue,const SI32 &cos7MutatedCosValue)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATECOSTOCOSMUTATIONMAP),
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

    QosGlobalCreateCosToCosMutationMapMessage::~QosGlobalCreateCosToCosMutationMapMessage ()
    {
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addSerializableAttribute (new AttributeSI32(&m_cos0MutatedCosValue,"cos0MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos1MutatedCosValue,"cos1MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos2MutatedCosValue,"cos2MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos3MutatedCosValue,"cos3MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos4MutatedCosValue,"cos4MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos5MutatedCosValue,"cos5MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos6MutatedCosValue,"cos6MutatedCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cos7MutatedCosValue,"cos7MutatedCosValue"));
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  QosGlobalCreateCosToCosMutationMapMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos0MutatedCosValue(const SI32 &cos0MutatedCosValue)
    {
        m_cos0MutatedCosValue  =  cos0MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos0MutatedCosValue() const
    {
        return (m_cos0MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos1MutatedCosValue(const SI32 &cos1MutatedCosValue)
    {
        m_cos1MutatedCosValue  =  cos1MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos1MutatedCosValue() const
    {
        return (m_cos1MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos2MutatedCosValue(const SI32 &cos2MutatedCosValue)
    {
        m_cos2MutatedCosValue  =  cos2MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos2MutatedCosValue() const
    {
        return (m_cos2MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos3MutatedCosValue(const SI32 &cos3MutatedCosValue)
    {
        m_cos3MutatedCosValue  =  cos3MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos3MutatedCosValue() const
    {
        return (m_cos3MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos4MutatedCosValue(const SI32 &cos4MutatedCosValue)
    {
        m_cos4MutatedCosValue  =  cos4MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos4MutatedCosValue() const
    {
        return (m_cos4MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos5MutatedCosValue(const SI32 &cos5MutatedCosValue)
    {
        m_cos5MutatedCosValue  =  cos5MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos5MutatedCosValue() const
    {
        return (m_cos5MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos6MutatedCosValue(const SI32 &cos6MutatedCosValue)
    {
        m_cos6MutatedCosValue  =  cos6MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos6MutatedCosValue() const
    {
        return (m_cos6MutatedCosValue);
    }

    void  QosGlobalCreateCosToCosMutationMapMessage::setCos7MutatedCosValue(const SI32 &cos7MutatedCosValue)
    {
        m_cos7MutatedCosValue  =  cos7MutatedCosValue;
    }

    SI32  QosGlobalCreateCosToCosMutationMapMessage::getCos7MutatedCosValue() const
    {
        return (m_cos7MutatedCosValue);
    }

}
