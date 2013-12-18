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

#include "ClientInterface/Ssm/QosClientCreateCeeMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateCeeMapMessage::QosClientCreateCeeMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATECEEMAP)
    {
    }

    QosClientCreateCeeMapMessage::QosClientCreateCeeMapMessage (const string &ceeMapName,const SI32 &precedence)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATECEEMAP),
        m_ceeMapName    (ceeMapName),
        m_precedence    (precedence)
    {
    }

    QosClientCreateCeeMapMessage::~QosClientCreateCeeMapMessage ()
    {
    }

    void  QosClientCreateCeeMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_precedence,"precedence"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreateCeeMapMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientCreateCeeMapMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosClientCreateCeeMapMessage::setPrecedence(const SI32 &precedence)
    {
        m_precedence  =  precedence;
    }

    SI32  QosClientCreateCeeMapMessage::getPrecedence() const
    {
        return (m_precedence);
    }

    void  QosClientCreateCeeMapMessage::setConfigReplay(const SI32 &configReplay)
	{
		m_configReplay = configReplay;
	}

	SI32 QosClientCreateCeeMapMessage::getConfigReplay() const
	{
		return (m_configReplay);
	}

    const void *QosClientCreateCeeMapMessage::getCStructureForInputs ()
    {
        QosCeeMapMsg_t *pInput = (QosCeeMapMsg_t *)calloc (1, sizeof (QosCeeMapMsg_t));

        if ( NULL == pInput ) {
            return pInput;
        }
        strncpy (pInput->ceeMapName, m_ceeMapName. c_str (), MAP_NAME_MAX_STR-1);
        pInput->ceeMapName[MAP_NAME_MAX_STR-1] = '\0';
        pInput->precedence = m_precedence;
		pInput->configReplay = (int)m_configReplay;

        return (pInput);
    }

    void QosClientCreateCeeMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
