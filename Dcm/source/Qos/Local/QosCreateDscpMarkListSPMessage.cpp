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
#include "Qos/Local/QosCreateDscpMarkListSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateDscpMarkListSPMessage::QosCreateDscpMarkListSPMessage ()
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSCREATEDSCPMARKLISTSP)
    {
    }

    QosCreateDscpMarkListSPMessage::QosCreateDscpMarkListSPMessage (const string &dscpMapName,const string &markList,const SI32 &toValue, const UI8 &qosMapType)
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSCREATEDSCPMARKLISTSP),
        m_dscpMapName    (dscpMapName),
        m_markList    (markList),
        m_toValue    (toValue),
		m_qosMapType (qosMapType)
    {
    }

    QosCreateDscpMarkListSPMessage::~QosCreateDscpMarkListSPMessage ()
    {
    }

    void  QosCreateDscpMarkListSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpMapName,"dscpMapName"));
        addSerializableAttribute (new AttributeString(&m_markList,"markList"));
        addSerializableAttribute (new AttributeSI32(&m_toValue,"toValue"));
        addSerializableAttribute (new AttributeUI8(&m_qosMapType,"qosMapType"));
    }

    void  QosCreateDscpMarkListSPMessage::setDscpMapName(const string &dscpMapName)
    {
        m_dscpMapName  =  dscpMapName;
    }

    string  QosCreateDscpMarkListSPMessage::getDscpMapName() const
    {
        return (m_dscpMapName);
    }

    void  QosCreateDscpMarkListSPMessage::setMarkList(const string &markList)
    {
        m_markList  =  markList;
    }

    string  QosCreateDscpMarkListSPMessage::getMarkList() const
    {
        return (m_markList);
    }

    void  QosCreateDscpMarkListSPMessage::setToValue(const SI32 &toValue)
    {
        m_toValue  =  toValue;
    }

    SI32  QosCreateDscpMarkListSPMessage::getToValue() const
    {
        return (m_toValue);
    }

	void QosCreateDscpMarkListSPMessage::setQosMapType(const UI8 &qosMapType)
	{
		m_qosMapType = qosMapType;
	}
	
	UI8 QosCreateDscpMarkListSPMessage::getQosMapType()  const
	{
		return (m_qosMapType);
	}
}
