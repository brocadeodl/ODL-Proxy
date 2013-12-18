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

#include "ClientInterface/Ssm/QosClientCreateDscpToDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateDscpToDscpMutationMapMessage::QosClientCreateDscpToDscpMutationMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPTODSCPMUTATIONMAP)
    {
    }

    QosClientCreateDscpToDscpMutationMapMessage::QosClientCreateDscpToDscpMutationMapMessage (const string &dscpMutationMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPTODSCPMUTATIONMAP),
        m_dscpMutationMapName    (dscpMutationMapName)
    {
    }

    QosClientCreateDscpToDscpMutationMapMessage::~QosClientCreateDscpToDscpMutationMapMessage ()
    {
    }

    void  QosClientCreateDscpToDscpMutationMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpMutationMapName,"dscpMutationMapName"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreateDscpToDscpMutationMapMessage::setDscpMutationMapName(const string &dscpMutationMapName)
    {
        m_dscpMutationMapName  =  dscpMutationMapName;
    }

    string  QosClientCreateDscpToDscpMutationMapMessage::getDscpMutationMapName() const
    {
        return (m_dscpMutationMapName);
    }

    void  QosClientCreateDscpToDscpMutationMapMessage::setConfigReplay(const SI32 &configReplay)
	{
		m_configReplay = configReplay;
	}

	SI32 QosClientCreateDscpToDscpMutationMapMessage::getConfigReplay() const
	{
		return (m_configReplay);
	}

    const void *QosClientCreateDscpToDscpMutationMapMessage::getCStructureForInputs ()
    {
        QosDscpMutationMapMsg_t *pInput = (QosDscpMutationMapMsg_t *)malloc (sizeof (QosDscpMutationMapMsg_t));

        strncpy (pInput->dscpMutationMapName, m_dscpMutationMapName. c_str (), sizeof(pInput->dscpMutationMapName) - 1);
		pInput->dscpMutationMapName[sizeof(pInput->dscpMutationMapName) - 1] = '\0';
		pInput->configReplay = (int)m_configReplay;

        return (pInput);
    }

    void QosClientCreateDscpToDscpMutationMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
