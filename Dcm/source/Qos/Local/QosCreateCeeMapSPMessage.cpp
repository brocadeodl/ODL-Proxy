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
#include "Qos/Local/QosCreateCeeMapSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateCeeMapSPMessage::QosCreateCeeMapSPMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSCREATECEEMAPSP)
    {
    }

    QosCreateCeeMapSPMessage::QosCreateCeeMapSPMessage (const string &ceeMapName,const SI32 &precedence)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSCREATECEEMAPSP),
        m_ceeMapName    (ceeMapName),
        m_precedence    (precedence)
    {
    }

    QosCreateCeeMapSPMessage::~QosCreateCeeMapSPMessage ()
    {
    }

    void  QosCreateCeeMapSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_precedence,"precedence"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosCreateCeeMapSPMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosCreateCeeMapSPMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosCreateCeeMapSPMessage::setPrecedence(const SI32 &precedence)
    {
        m_precedence  =  precedence;
    }

    SI32  QosCreateCeeMapSPMessage::getPrecedence() const
    {
        return (m_precedence);
    }

	void  QosCreateCeeMapSPMessage::setConfigReplay(const SI32 &configReplay)
	{
		m_configReplay = configReplay;
	}
	SI32 QosCreateCeeMapSPMessage::getConfigReplay() const
	{
		return(m_configReplay);
	}
}
