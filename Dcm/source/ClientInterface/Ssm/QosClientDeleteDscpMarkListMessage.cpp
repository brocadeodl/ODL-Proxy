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

#include "ClientInterface/Ssm/QosClientDeleteDscpMarkListMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "Framework/Types/UI32Range.h"


namespace DcmNs
{

    QosClientDeleteDscpMarkListMessage::QosClientDeleteDscpMarkListMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPMARKLIST)
    {
    }

    QosClientDeleteDscpMarkListMessage::QosClientDeleteDscpMarkListMessage (const string &markString, const string &dscpMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPMARKLIST),
        m_markString    (markString),
		m_dscpMapName	(dscpMapName)
    {
    }

    QosClientDeleteDscpMarkListMessage::~QosClientDeleteDscpMarkListMessage ()
    {
    }

    void  QosClientDeleteDscpMarkListMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_markString,"markString"));
        addSerializableAttribute (new AttributeString(&m_dscpMapName,"dscpMapName"));
    }

    void  QosClientDeleteDscpMarkListMessage::setMarkString(const string &markString)
    {
        m_markString  =  markString;
    }

    string  QosClientDeleteDscpMarkListMessage::getMarkString() const
    {
        return (m_markString);
    }

	void QosClientDeleteDscpMarkListMessage::setDscpMapName(const string &dscpMapName)
	{
		m_dscpMapName = dscpMapName;
	}
	
	string QosClientDeleteDscpMarkListMessage::getDscpMapName() const
	{
		return (m_dscpMapName);
	}

    const void *QosClientDeleteDscpMarkListMessage::getCStructureForInputs ()
    {
		int i,sz;
		vector <UI32> dscpVector;
		UI32Range(m_markString).getUI32RangeVector(dscpVector);
		sz = dscpVector.size();
		
        QosDscpMarkListMsg_t *pInput = (QosDscpMarkListMsg_t *)calloc (1, sizeof (QosDscpMarkListMsg_t));
        
        if ( NULL == pInput ) {
            return pInput;
        }
        strncpy (pInput->dscpMapName, m_dscpMapName. c_str (), MAP_NAME_MAX_STR-1);
        pInput->dscpMapName[MAP_NAME_MAX_STR-1] = '\0';

		for (i=0; i<sz; i++) {
			pInput->pValues[i] = dscpVector[i];
		}
		pInput->nValues = sz;

        return (pInput);
    }

    void QosClientDeleteDscpMarkListMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
