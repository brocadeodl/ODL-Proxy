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

#include "Qos/Global/CeeGlobalCreateCeeMapMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    CeeGlobalCreateCeeMapMessage::CeeGlobalCreateCeeMapMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATECEEMAP)
    {
    }

    CeeGlobalCreateCeeMapMessage::CeeGlobalCreateCeeMapMessage (const string &ceeMapName,const SI32 &precedence)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATECEEMAP),
        m_ceeMapName    (ceeMapName),
        m_precedence    (precedence)
    {
    }

    CeeGlobalCreateCeeMapMessage::~CeeGlobalCreateCeeMapMessage ()
    {
    }

    void  CeeGlobalCreateCeeMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_precedence,"precedence"));
		addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  CeeGlobalCreateCeeMapMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  CeeGlobalCreateCeeMapMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  CeeGlobalCreateCeeMapMessage::setPrecedence(const SI32 &precedence)
    {
        m_precedence  =  precedence;
    }

    SI32  CeeGlobalCreateCeeMapMessage::getPrecedence() const
    {
        return (m_precedence);
    }

	void CeeGlobalCreateCeeMapMessage::setConfigReplay (const SI32 &configReplay)
	{
		m_configReplay = configReplay;
	}

	SI32 CeeGlobalCreateCeeMapMessage::getConfigReplay () const
	{
		return (m_configReplay);
	}
}
