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

#include "ClientInterface/Ssm/QosClientCreateCosToCosMutationMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateCosToCosMutationMapMessage::QosClientCreateCosToCosMutationMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATECOSTOCOSMUTATIONMAP)
    {
    }

    QosClientCreateCosToCosMutationMapMessage::QosClientCreateCosToCosMutationMapMessage (const string &cosToCosMutationMapName,const SI32 &cos0MutatedCosValue,const SI32 &cos1MutatedCosValue,const SI32 &cos2MutatedCosValue,const SI32 &cos3MutatedCosValue,const SI32 &cos4MutatedCosValue,const SI32 &cos5MutatedCosValue,const SI32 &cos6MutatedCosValue,const SI32 &cos7MutatedCosValue)
        : DcmManagementInterfaceMessage (QOSCLIENTCREATECOSTOCOSMUTATIONMAP),
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

    QosClientCreateCosToCosMutationMapMessage::~QosClientCreateCosToCosMutationMapMessage ()
    {
    }

    void  QosClientCreateCosToCosMutationMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  QosClientCreateCosToCosMutationMapMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  QosClientCreateCosToCosMutationMapMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos0MutatedCosValue(const SI32 &cos0MutatedCosValue)
    {
        m_cos0MutatedCosValue  =  cos0MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos0MutatedCosValue() const
    {
        return (m_cos0MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos1MutatedCosValue(const SI32 &cos1MutatedCosValue)
    {
        m_cos1MutatedCosValue  =  cos1MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos1MutatedCosValue() const
    {
        return (m_cos1MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos2MutatedCosValue(const SI32 &cos2MutatedCosValue)
    {
        m_cos2MutatedCosValue  =  cos2MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos2MutatedCosValue() const
    {
        return (m_cos2MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos3MutatedCosValue(const SI32 &cos3MutatedCosValue)
    {
        m_cos3MutatedCosValue  =  cos3MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos3MutatedCosValue() const
    {
        return (m_cos3MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos4MutatedCosValue(const SI32 &cos4MutatedCosValue)
    {
        m_cos4MutatedCosValue  =  cos4MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos4MutatedCosValue() const
    {
        return (m_cos4MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos5MutatedCosValue(const SI32 &cos5MutatedCosValue)
    {
        m_cos5MutatedCosValue  =  cos5MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos5MutatedCosValue() const
    {
        return (m_cos5MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos6MutatedCosValue(const SI32 &cos6MutatedCosValue)
    {
        m_cos6MutatedCosValue  =  cos6MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos6MutatedCosValue() const
    {
        return (m_cos6MutatedCosValue);
    }

    void  QosClientCreateCosToCosMutationMapMessage::setCos7MutatedCosValue(const SI32 &cos7MutatedCosValue)
    {
        m_cos7MutatedCosValue  =  cos7MutatedCosValue;
    }

    SI32  QosClientCreateCosToCosMutationMapMessage::getCos7MutatedCosValue() const
    {
        return (m_cos7MutatedCosValue);
    }

	const void *QosClientCreateCosToCosMutationMapMessage::getCStructureForInputs ()
    {
        QosCosToCosMsg_t *pInput = (QosCosToCosMsg_ *)calloc (1, sizeof (struct QosCosToCosMsg_));

        if ( NULL == pInput ) {
            return pInput;
        }

        strncpy (pInput->cosToCosMutationMapName, m_cosToCosMutationMapName.c_str (), MAP_NAME_MAX_STR-1);
        pInput->cosToCosMutationMapName[MAP_NAME_MAX_STR-1] = '\0';
        pInput->cos0MutatedCosValue = m_cos0MutatedCosValue;
        pInput->cos1MutatedCosValue = m_cos1MutatedCosValue;
        pInput->cos2MutatedCosValue = m_cos2MutatedCosValue;
        pInput->cos3MutatedCosValue = m_cos3MutatedCosValue;
        pInput->cos4MutatedCosValue = m_cos4MutatedCosValue;
        pInput->cos5MutatedCosValue = m_cos5MutatedCosValue;
        pInput->cos6MutatedCosValue = m_cos6MutatedCosValue;
        pInput->cos7MutatedCosValue = m_cos7MutatedCosValue;

        return (pInput);
    }

    void QosClientCreateCosToCosMutationMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
