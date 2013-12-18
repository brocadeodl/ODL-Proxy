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

#include "ClientInterface/Ssm/QosClientCreateDscpToCosMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateDscpToCosMapMessage::QosClientCreateDscpToCosMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPTOCOSMAP)
    {
    }

    QosClientCreateDscpToCosMapMessage::QosClientCreateDscpToCosMapMessage (const string &dscpCosMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPTOCOSMAP),
        m_dscpCosMapName    (dscpCosMapName)
    {
    }

    QosClientCreateDscpToCosMapMessage::~QosClientCreateDscpToCosMapMessage ()
    {
    }

    void  QosClientCreateDscpToCosMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpCosMapName,"dscpCosMapName"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreateDscpToCosMapMessage::setDscpCosMapName(const string &dscpCosMapName)
    {
        m_dscpCosMapName  =  dscpCosMapName;
    }

    string  QosClientCreateDscpToCosMapMessage::getDscpCosMapName() const
    {
        return (m_dscpCosMapName);
    }

    void  QosClientCreateDscpToCosMapMessage::setConfigReplay(const SI32 &configReplay)
	{
		m_configReplay = configReplay;
	}

	SI32 QosClientCreateDscpToCosMapMessage::getConfigReplay() const
	{
		return (m_configReplay);
	}

    const void *QosClientCreateDscpToCosMapMessage::getCStructureForInputs ()
    {
        QosDscpCosMapMsg_t *pInput = (QosDscpCosMapMsg_t *)malloc (sizeof (QosDscpCosMapMsg_t));

        strncpy (pInput->dscpCosMapName, m_dscpCosMapName. c_str (), sizeof(pInput->dscpCosMapName) - 1);
		pInput->dscpCosMapName[sizeof(pInput->dscpCosMapName) - 1] = '\0';
		pInput->configReplay = (int)m_configReplay;

        return (pInput);
    }

    void QosClientCreateDscpToCosMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
