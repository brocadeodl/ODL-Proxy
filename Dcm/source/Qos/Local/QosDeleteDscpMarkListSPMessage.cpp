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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nbellari                                                     *
 **************************************************************************/

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosDeleteDscpMarkListSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosDeleteDscpMarkListSPMessage::QosDeleteDscpMarkListSPMessage ()
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSDELETEDSCPMARKLISTSP)
    {
    }

    QosDeleteDscpMarkListSPMessage::QosDeleteDscpMarkListSPMessage (const string &dscpMapName,const string &markList, const UI8 &qosMapType)
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSDELETEDSCPMARKLISTSP),
        m_dscpMapName    (dscpMapName),
        m_markList    (markList),
		m_qosMapType (qosMapType)
    {
    }

    QosDeleteDscpMarkListSPMessage::~QosDeleteDscpMarkListSPMessage ()
    {
    }

    void  QosDeleteDscpMarkListSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpMapName,"dscpMapName"));
        addSerializableAttribute (new AttributeString(&m_markList,"markList"));
    }

    void  QosDeleteDscpMarkListSPMessage::setDscpMapName(const string &dscpMapName)
    {
        m_dscpMapName  =  dscpMapName;
    }

    string  QosDeleteDscpMarkListSPMessage::getDscpMapName() const
    {
        return (m_dscpMapName);
    }

    void  QosDeleteDscpMarkListSPMessage::setMarkList(const string &markList)
    {
        m_markList  =  markList;
    }

    string  QosDeleteDscpMarkListSPMessage::getMarkList() const
    {
        return (m_markList);
    }

	void QosDeleteDscpMarkListSPMessage::setQosMapType(const UI8 &qosMapType)
	{
		m_qosMapType = qosMapType;
	}
	
	UI8 QosDeleteDscpMarkListSPMessage::getQosMapType()  const
	{
		return (m_qosMapType);
	}
}
