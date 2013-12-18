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

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosCreateCeeRemapSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateCeeRemapSPMessage::QosCreateCeeRemapSPMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSCREATECEEREMAPSP)
    {
    }

    QosCreateCeeRemapSPMessage::QosCreateCeeRemapSPMessage (const string &ceeMapName,const SI32 &remapPriority, const bool &remapFabricP)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSCREATECEEREMAPSP),
        m_ceeMapName    (ceeMapName),
        m_remapPriority    (remapPriority),
		m_remapFabricP		(remapFabricP)
    {
    }

    QosCreateCeeRemapSPMessage::~QosCreateCeeRemapSPMessage ()
    {
    }

    void  QosCreateCeeRemapSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_remapPriority,"remapPriority"));
        addSerializableAttribute (new AttributeBool(&m_remapFabricP,"remapFabricP"));
    }

    void  QosCreateCeeRemapSPMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosCreateCeeRemapSPMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosCreateCeeRemapSPMessage::setRemapPriority(const SI32 &remapPriority)
    {
        m_remapPriority  =  remapPriority;
    }

    SI32  QosCreateCeeRemapSPMessage::getRemapPriority() const
    {
        return (m_remapPriority);
    }

	void QosCreateCeeRemapSPMessage::setRemapFabricP(const bool &remapFabricP)
	{
		m_remapFabricP = remapFabricP;
	}

	bool QosCreateCeeRemapSPMessage::getRemapFabricP()	const
	{
		return(m_remapFabricP);
	}
}

