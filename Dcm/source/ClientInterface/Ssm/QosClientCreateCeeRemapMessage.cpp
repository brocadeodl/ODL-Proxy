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

#include "ClientInterface/Ssm/QosClientCreateCeeRemapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateCeeRemapMessage::QosClientCreateCeeRemapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATECEEREMAP)
    {
    }

    QosClientCreateCeeRemapMessage::QosClientCreateCeeRemapMessage (const string &ceeMapName,const SI32 &remapPriority, const bool &remapFabricP)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATECEEREMAP),
        m_ceeMapName    (ceeMapName),
        m_remapPriority    (remapPriority),
		m_remapFabricP		(remapFabricP)
    {
    }

    QosClientCreateCeeRemapMessage::~QosClientCreateCeeRemapMessage ()
    {
    }

    void  QosClientCreateCeeRemapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_remapPriority,"remapPriority"));
        addSerializableAttribute (new AttributeBool(&m_remapFabricP,"remapFabricP"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreateCeeRemapMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientCreateCeeRemapMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosClientCreateCeeRemapMessage::setRemapPriority(const SI32 &remapPriority)
    {
        m_remapPriority  =  remapPriority;
    }

    SI32  QosClientCreateCeeRemapMessage::getRemapPriority() const
    {
        return (m_remapPriority);
    }

    void  QosClientCreateCeeRemapMessage::setRemapFabricP(const bool &remapFabricP)
    {
        m_remapFabricP  =  remapFabricP;
    }

    bool  QosClientCreateCeeRemapMessage::getRemapFabricP() const
    {
        return (m_remapFabricP);
    }

    void  QosClientCreateCeeRemapMessage::setConfigReplay(const SI32 &configReplay)
    {
        m_configReplay = configReplay;
    }

    SI32 QosClientCreateCeeRemapMessage::getConfigReplay() const
    {
        return (m_configReplay);
    }

    const void *QosClientCreateCeeRemapMessage::getCStructureForInputs ()
    {
        QosCeeRemapMsg_t *pInput = (QosCeeRemapMsg_t *)calloc (1, sizeof (QosCeeRemapMsg_t));

        if ( NULL == pInput ) {
            return pInput;
        }

        strncpy (pInput->ceeMapName, m_ceeMapName. c_str (), MAP_NAME_MAX_STR-1);
        pInput->ceeMapName[MAP_NAME_MAX_STR-1] = '\0';
		pInput->remapPriority = m_remapPriority;
        pInput->remapFabricP = m_remapFabricP;
        pInput->configReplay = (int)m_configReplay;

        return (pInput);
	}

    void QosClientCreateCeeRemapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}

