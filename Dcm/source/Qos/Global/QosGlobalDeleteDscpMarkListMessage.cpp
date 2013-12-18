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

#include "Qos/Global/QosGlobalDeleteDscpMarkListMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalDeleteDscpMarkListMessage::QosGlobalDeleteDscpMarkListMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALDELETEDSCPMARKLIST)
    {
    }

    QosGlobalDeleteDscpMarkListMessage::QosGlobalDeleteDscpMarkListMessage (const string &dscpMapName,const string &markList, const UI8 &qosMapType)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALDELETEDSCPMARKLIST),
        m_dscpMapName    (dscpMapName),
        m_markList    (markList),
		m_qosMapType (qosMapType)
    {
    }

    QosGlobalDeleteDscpMarkListMessage::~QosGlobalDeleteDscpMarkListMessage ()
    {
    }

    void  QosGlobalDeleteDscpMarkListMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpMapName,"dscpMapName"));
        addSerializableAttribute (new AttributeString(&m_markList,"markList"));
        addSerializableAttribute (new AttributeUI8(&m_qosMapType,"qosMapType"));
    }

    void  QosGlobalDeleteDscpMarkListMessage::setDscpMapName(const string &dscpMapName)
    {
        m_dscpMapName  =  dscpMapName;
    }

    string  QosGlobalDeleteDscpMarkListMessage::getDscpMapName() const
    {
        return (m_dscpMapName);
    }

    void  QosGlobalDeleteDscpMarkListMessage::setMarkList(const string &markList)
    {
        m_markList  =  markList;
    }

    string  QosGlobalDeleteDscpMarkListMessage::getMarkList() const
    {
        return (m_markList);
    }

	void QosGlobalDeleteDscpMarkListMessage::setQosMapType(const UI8 &qosMapType)
	{
		m_qosMapType = qosMapType;
	}
	
	UI8 QosGlobalDeleteDscpMarkListMessage::getQosMapType()  const
	{
		return (m_qosMapType);
	}
}
