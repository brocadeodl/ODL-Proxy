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

#include "Framework/Types/UI32Range.h"
#include "ClientInterface/Ssm/QosClientCreateDscpMarkListMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateDscpMarkListMessage::QosClientCreateDscpMarkListMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPMARKLIST)
    {
    }

	QosClientCreateDscpMarkListMessage::QosClientCreateDscpMarkListMessage (const string &markString, const string &dscpMapName, const SI32 &toValue, const UI8 &qosMapType)
		: DcmManagementInterfaceMessage(QOSCLIENTCREATEDSCPMARKLIST),
			m_markString(markString),
			m_dscpMapName(dscpMapName),
			m_toValue(toValue),
			m_qosMapType(qosMapType)
	{
	}

    QosClientCreateDscpMarkListMessage::~QosClientCreateDscpMarkListMessage ()
    {
    }

    void  QosClientCreateDscpMarkListMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_markString,"markString"));
        addSerializableAttribute (new AttributeString(&m_dscpMapName,"dscpMapName"));
        addSerializableAttribute (new AttributeSI32(&m_toValue,"toValue"));
        addSerializableAttribute (new AttributeUI8(&m_qosMapType,"qosMapType"));
    }

	void QosClientCreateDscpMarkListMessage::setMarkString(const string &dscpMarkString)
	{
		m_markString = dscpMarkString;
	}
	
	string QosClientCreateDscpMarkListMessage::getMarkString() const
	{
		return (m_markString);
	}
	
	void QosClientCreateDscpMarkListMessage::setDscpMapName(const string &dscpMapName)
	{
		m_dscpMapName = dscpMapName;
	}
	
	string QosClientCreateDscpMarkListMessage::getDscpMapName() const
	{
		return (m_dscpMapName);
	}
	
	void QosClientCreateDscpMarkListMessage::setDscpMarkToValue(const SI32 &toValue)
	{
		m_toValue = toValue;
	}
	
	SI32 QosClientCreateDscpMarkListMessage::getDscpMarkToValue() const
	{
		return (m_toValue);
	}
	
	void QosClientCreateDscpMarkListMessage::setQosMapType(const UI8 &qosMapType)
	{
		m_qosMapType = qosMapType;
	}
	
	UI8 QosClientCreateDscpMarkListMessage::getQosMapType() const
	{
		return (m_qosMapType);
	}
	

    const void *QosClientCreateDscpMarkListMessage::getCStructureForInputs ()
    {
		int i, sz;
		vector <UI32> dscpVector;
		UI32Range(m_markString).getUI32RangeVector(dscpVector);
		sz = dscpVector.size();
		
        QosDscpMarkListMsg_t *pInput = (QosDscpMarkListMsg_t *)malloc (sizeof (QosDscpMarkListMsg_t));
        strncpy (pInput->dscpMapName, m_dscpMapName. c_str (), sizeof(pInput->dscpMapName) - 1);
		pInput->dscpMapName[sizeof(pInput->dscpMapName) - 1] = '\0';
		pInput->toValue = (int)m_toValue;
		pInput->qosMapType = m_qosMapType;

		/* Convert the mark string to an array of values */
		pInput->nValues = sz;

		for (i=0; i<sz; i++) {
			pInput->pValues[i] = dscpVector[i];
		}

        return (pInput);
    }

    void QosClientCreateDscpMarkListMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
