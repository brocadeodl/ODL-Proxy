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
 *   Author : bgangise			                                           *
 **************************************************************************/

#include "ClientInterface/APPM/APPMProfileMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    APPMProfileMessage::APPMProfileMessage ()
        : DcmManagementInterfaceMessage (APPMPROFILE)
    {
    }

    APPMProfileMessage::APPMProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &isActivated,const string &profileMac,const bool &isAllow,const bool &isFloodingRestricted)
        : DcmManagementInterfaceMessage (APPMPROFILE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_profileMac    (profileMac),
		m_isAllow       (isAllow),
		m_isFloodingRestricted	(isFloodingRestricted)
    {
    }

    APPMProfileMessage::~APPMProfileMessage ()
    {
    }

    void  APPMProfileMessage::setupAttributesForSerialization()
    {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_isActivated,"isActivated"));
        addSerializableAttribute (new AttributeString(&m_profileMac,"profileMac"));
		addSerializableAttribute (new AttributeBool(&m_isAllow, "isAllow"));
		addSerializableAttribute (new AttributeBool(&m_isFloodingRestricted, "isFloodingRestricted"));
    }

    void  APPMProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMProfileMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMProfileMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMProfileMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMProfileMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMProfileMessage::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  APPMProfileMessage::getIsActivated() const
    {
        return (m_isActivated);
    }
	void  APPMProfileMessage::setIsAllow(const bool &isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  APPMProfileMessage::getIsAllow() const
    {
        return (m_isAllow);
    }

	void APPMProfileMessage::setIsFloodingRestricted(const bool &isFloodingRestricted) {
        m_isFloodingRestricted = isFloodingRestricted;
    }

    bool APPMProfileMessage::getIsFloodingRestricted() const {
        return m_isFloodingRestricted;
    }

    void  APPMProfileMessage::setProfileMac(const string &profileMac)
    {
        m_profileMac  =  profileMac;
    }

    string  APPMProfileMessage::getProfileMac() const
    {
        return (m_profileMac);
    }
    const void *APPMProfileMessage::getCStructureForInputs ()
    {
        portProfileMsg_t *pInput = new portProfileMsg_t;
        unsigned int len = 0;
	    pInput->opCode = m_opCode; 
	    pInput->cmdCode = m_cmdCode; 
		
        len = strlen(m_profileName.c_str());
        strncpy (pInput->profileName, m_profileName.c_str (), len);
		pInput->profileName[len] = '\0';
		
		len = strlen(m_profileMac.c_str());
        strncpy (pInput->profileMacStr, m_profileMac.c_str (), len);
		pInput->profileMacStr[len] = '\0';
	    
		pInput->profileId = m_profileId; 
	    pInput->isActivated = m_isActivated; 
       	pInput->isAllow		= m_isAllow;
		pInput->isFloodingRestricted = m_isFloodingRestricted;
		return (pInput);
    }

	void APPMProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
