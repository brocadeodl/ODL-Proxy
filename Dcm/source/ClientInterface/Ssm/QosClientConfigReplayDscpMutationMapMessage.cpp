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
#include "ClientInterface/Ssm/QosClientConfigReplayDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientConfigReplayDscpMutationMapMessage::QosClientConfigReplayDscpMutationMapMessage()
        : DcmManagementInterfaceMessage(QOSCLIENTCONFIGREPLAYDSCPMUTATIONMAP)
    {
    }

    QosClientConfigReplayDscpMutationMapMessage::QosClientConfigReplayDscpMutationMapMessage(const string &dscpMutationMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTCONFIGREPLAYDSCPMUTATIONMAP),
        m_dscpMutationMapName    (dscpMutationMapName)
    {
    }

    QosClientConfigReplayDscpMutationMapMessage::~QosClientConfigReplayDscpMutationMapMessage()
    {
    }

	void QosClientConfigReplayDscpMutationMapMessage::initialize()
	{
		uint32_t i;

		for (i=0; i<64; i++) {
			m_map.push_back(i);
		}
	}
	
    void  QosClientConfigReplayDscpMutationMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpMutationMapName,"dscpMutationMapName"));
		addSerializableAttribute (new AttributeUI32Vector(&m_map, "map"));
    }

    void  QosClientConfigReplayDscpMutationMapMessage::setDscpMutationMapName(const string &dscpMutationMapName)
    {
        m_dscpMutationMapName  =  dscpMutationMapName;
    }

    string  QosClientConfigReplayDscpMutationMapMessage::getDscpMutationMapName() const
    {
        return (m_dscpMutationMapName);
    }

	void QosClientConfigReplayDscpMutationMapMessage::setDscpMarkListValues(const string &dscpMarkList, const SI32 &toValue)
	{
		int i, sz;
		vector <UI32> mapVector;
		UI32Range(dscpMarkList).getUI32RangeVector(mapVector);
		sz = mapVector.size();

		for (i=0; i<sz; i++) {
			m_map[mapVector[i]] = toValue;
		}
	}

    const void *QosClientConfigReplayDscpMutationMapMessage::getCStructureForInputs ()
    {
		int i;
        QosDscpMarkListMsg_t *pInput = (QosDscpMarkListMsg_t *)calloc (1, sizeof (QosDscpMarkListMsg_t));
        if ( NULL == pInput ) {
            return pInput;
        }

        strncpy (pInput->dscpMapName, m_dscpMutationMapName. c_str (), MAP_NAME_MAX_STR-1);
        pInput->dscpMapName[MAP_NAME_MAX_STR-1] = '\0';
		for (i=0; i<64; i++) {
			pInput->pValues[i] = m_map[i];
		}
		
        return (pInput);
    }

    void QosClientConfigReplayDscpMutationMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
