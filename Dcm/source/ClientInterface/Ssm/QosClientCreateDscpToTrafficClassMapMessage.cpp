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

#include "ClientInterface/Ssm/QosClientCreateDscpToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateDscpToTrafficClassMapMessage::QosClientCreateDscpToTrafficClassMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAP)
    {
    }

    QosClientCreateDscpToTrafficClassMapMessage::QosClientCreateDscpToTrafficClassMapMessage (const string &dscpTrafficClassMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAP),
        m_dscpTrafficClassMapName    (dscpTrafficClassMapName)
    {
    }

    QosClientCreateDscpToTrafficClassMapMessage::~QosClientCreateDscpToTrafficClassMapMessage ()
    {
    }

    void  QosClientCreateDscpToTrafficClassMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpTrafficClassMapName,"dscpTrafficClassMapName"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreateDscpToTrafficClassMapMessage::setDscpTrafficClassMapName(const string &dscpTrafficClassMapName)
    {
        m_dscpTrafficClassMapName  =  dscpTrafficClassMapName;
    }

    string  QosClientCreateDscpToTrafficClassMapMessage::getDscpTrafficClassMapName() const
    {
        return (m_dscpTrafficClassMapName);
    }

    void  QosClientCreateDscpToTrafficClassMapMessage::setConfigReplay(const SI32 &configReplay)
	{
		m_configReplay = configReplay;
	}

	SI32 QosClientCreateDscpToTrafficClassMapMessage::getConfigReplay() const
	{
		return (m_configReplay);
	}

    const void *QosClientCreateDscpToTrafficClassMapMessage::getCStructureForInputs ()
    {
        QosDscpTrafficClassMapMsg_t *pInput = (QosDscpTrafficClassMapMsg_t *)malloc (sizeof (QosDscpTrafficClassMapMsg_t));

        strncpy (pInput->dscpTrafficClassMapName, m_dscpTrafficClassMapName. c_str (), sizeof(pInput->dscpTrafficClassMapName) - 1);
		pInput->dscpTrafficClassMapName[sizeof(pInput->dscpTrafficClassMapName) - 1] = '\0';
		pInput->configReplay = (int)m_configReplay;

        return (pInput);
    }

    void QosClientCreateDscpToTrafficClassMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
