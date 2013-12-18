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
 *   Author : jindala                                                      *
 **************************************************************************/

#include "ClientInterface/APPM/APPMProfileBulkMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    APPMProfileBulkMessage::APPMProfileBulkMessage ()
        : DcmManagementInterfaceMessage (APPMPROFILEBULKMACASSOC)
    {
    }


    APPMProfileBulkMessage::APPMProfileBulkMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &isActivated,const bool &isAllow, const UI32 &macCount, const vector<string> &profileMacs)
        : DcmManagementInterfaceMessage (APPMPROFILEBULKMACASSOC),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_isAllow       (isAllow),
		m_macCount       (macCount),
        m_profileMacs    (profileMacs)
    {
    }

    APPMProfileBulkMessage::~APPMProfileBulkMessage ()
    {
    }

    void  APPMProfileBulkMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_isActivated,"isActivated"));
        addSerializableAttribute (new AttributeBool(&m_isAllow, "isAllow"));
        addSerializableAttribute (new AttributeUI32(&m_macCount,"macCount"));
        addSerializableAttribute (new AttributeStringVector(&m_profileMacs,"profileMacs"));
    }

    void  APPMProfileBulkMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMProfileBulkMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMProfileBulkMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMProfileBulkMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMProfileBulkMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMProfileBulkMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMProfileBulkMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMProfileBulkMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMProfileBulkMessage::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  APPMProfileBulkMessage::getIsActivated() const
    {
        return (m_isActivated);
    }
    void  APPMProfileBulkMessage::setIsAllow(const bool &isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  APPMProfileBulkMessage::getIsAllow() const
    {
        return (m_isAllow);
    }

    void  APPMProfileBulkMessage::setMacCount(const UI32 &macCount)
    {
        m_macCount  =  macCount;
    }

    UI32  APPMProfileBulkMessage::getMacCount() const
    {
        return (m_macCount);
    }

    void  APPMProfileBulkMessage::setProfileMacs(const vector<string> &profileMacs)
    {
        m_profileMacs  =  profileMacs;
    }

    vector<string>  APPMProfileBulkMessage::getProfileMacs() const
    {
        return (m_profileMacs);
    }

    const void *APPMProfileBulkMessage::getCStructureForInputs ()
    {
		portProfileBulkMsg_t *pInput = (portProfileBulkMsg_t *) malloc(sizeof(portProfileBulkMsg_t) + ( (m_macCount - 1 )*sizeof(portProfileMacMsg_t)));
		unsigned int len = 0;
        pInput->opCode = m_opCode;
        pInput->cmdCode = m_cmdCode;
		pInput->macCount = m_macCount;
        len = strlen(m_profileName.c_str());
        strncpy (pInput->profileName, m_profileName.c_str (), len);
        pInput->profileName[len] = '\0';
		for (unsigned int i = 0; i < pInput->macCount; ++i) 
		{
        	len = strlen(m_profileMacs[i].c_str());
        	strncpy(pInput->macListAndFlags[i].profileMacsStr, m_profileMacs[i].c_str (), len);
			pInput->macListAndFlags[i].profileMacsStr[len] = '\0';
			pInput->macListAndFlags[i].macErrorFlag = 0;
		}

        pInput->profileId 	= m_profileId;
        pInput->isActivated = m_isActivated;
        pInput->isAllow     = m_isAllow;
		pInput->allSuccessFlag = 1;
        return (pInput);
    }

    void APPMProfileBulkMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
	  portProfileBulkMsg_t *pOutput = ( (portProfileBulkMsg_t *) pOutputCStructure);

	  if (!pOutput) {
			return;
        }

	  UI32 mac_count = pOutput->macCount;
	  addBuffer(1,(sizeof(portProfileBulkMsg_t) + ( (mac_count - 1 )*sizeof(portProfileMacMsg_t))), pOutputCStructure, false);

    }
}
