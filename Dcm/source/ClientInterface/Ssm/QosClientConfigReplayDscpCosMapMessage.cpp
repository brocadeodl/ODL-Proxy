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
 *   Author : nbellari *
 **************************************************************************/

#include "Framework/Types/UI32Range.h"
#include "ClientInterface/Ssm/QosClientConfigReplayDscpCosMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientConfigReplayDscpCosMapMessage::QosClientConfigReplayDscpCosMapMessage()
        : DcmManagementInterfaceMessage(QOSCLIENTCONFIGREPLAYDSCPCOSMAP)
    {
    }

    QosClientConfigReplayDscpCosMapMessage::QosClientConfigReplayDscpCosMapMessage(const string &dscpCosMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTCONFIGREPLAYDSCPCOSMAP),
        m_dscpCosMapName    (dscpCosMapName)
    {
    }

    QosClientConfigReplayDscpCosMapMessage::~QosClientConfigReplayDscpCosMapMessage()
    {
    }

	void QosClientConfigReplayDscpCosMapMessage::initialize()
	{
		uint32_t i,j;

		for (i=0, j=0; i<64; i++) {
			if (i && (i%8 == 0)) {
				j++;
			}
			m_map.push_back(j);
		}
	}

    void  QosClientConfigReplayDscpCosMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpCosMapName,"dscpCosMapName"));
		addSerializableAttribute (new AttributeUI32Vector(&m_map, "map"));
    }

    void  QosClientConfigReplayDscpCosMapMessage::setDscpCosMapName(const string &dscpCosMapName)
    {
        m_dscpCosMapName  =  dscpCosMapName;
    }

    string  QosClientConfigReplayDscpCosMapMessage::getDscpCosMapName() const
    {
        return (m_dscpCosMapName);
    }

	void QosClientConfigReplayDscpCosMapMessage::setDscpMarkListValues(const string &dscpMarkList, const SI32 &toValue)
	{
		int i, sz;
		vector <UI32> mapVector;
		UI32Range(dscpMarkList).getUI32RangeVector(mapVector);
		sz = mapVector.size();

		for (i=0; i<sz; i++) {
			m_map[mapVector[i]] = toValue;
		}
	}

    const void *QosClientConfigReplayDscpCosMapMessage::getCStructureForInputs ()
    {
		int i;
        QosDscpMarkListMsg_t *pInput = (QosDscpMarkListMsg_t *)calloc (1, sizeof (QosDscpMarkListMsg_t));

        if ( NULL == pInput ) {
            return pInput;
        }

        strncpy (pInput->dscpMapName, m_dscpCosMapName. c_str (), MAP_NAME_MAX_STR-1);
        pInput->dscpMapName[MAP_NAME_MAX_STR-1] = '\0';
		for (i=0; i<64; i++) {
			pInput->pValues[i] = m_map[i];
		}
		
        return (pInput);
    }

    void QosClientConfigReplayDscpCosMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
