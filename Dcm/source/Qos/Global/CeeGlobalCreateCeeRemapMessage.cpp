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

#include "Qos/Global/CeeGlobalCreateCeeRemapMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    CeeGlobalCreateCeeRemapMessage::CeeGlobalCreateCeeRemapMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATECEEREMAP)
    {
    }

	CeeGlobalCreateCeeRemapMessage::CeeGlobalCreateCeeRemapMessage (const string &ceeMapName, const SI32 remapPriority, const bool remapFabricP)
		:ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATECEEREMAP),
		m_ceeMapName    (ceeMapName),
		m_remapPriority (remapPriority),
		m_remapFabricP	(remapFabricP)
	{
	}

	CeeGlobalCreateCeeRemapMessage::~CeeGlobalCreateCeeRemapMessage()
	{
	}

	void  CeeGlobalCreateCeeRemapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_remapPriority,"remapPriority"));
        addSerializableAttribute (new AttributeBool(&m_remapFabricP,"remapFabricP"));
	}


    void CeeGlobalCreateCeeRemapMessage::setCeeMapName(const string &ceeMapName)
	{
		m_ceeMapName = ceeMapName;
	}
    string  CeeGlobalCreateCeeRemapMessage::getCeeMapName()     const
	{
		return(m_ceeMapName);
	}

    void CeeGlobalCreateCeeRemapMessage::setRemapPriority(const SI32 remapPriority)
	{
		m_remapPriority = remapPriority;
	}
    SI32 CeeGlobalCreateCeeRemapMessage::getRemapPriority()     const
	{
		return(m_remapPriority);
	}

    void CeeGlobalCreateCeeRemapMessage::setRemapFabricP(const bool remapFabricP)
	{
		m_remapFabricP = remapFabricP;
	}
    bool CeeGlobalCreateCeeRemapMessage::getRemapFabricP()      const
	{
		return(m_remapFabricP);
	}
}
