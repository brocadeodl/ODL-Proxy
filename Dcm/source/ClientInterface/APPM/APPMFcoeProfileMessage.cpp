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
 *   Author : bgangise                                                     *
 **************************************************************************/

#include "ClientInterface/APPM/APPMFcoeProfileMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    APPMFcoeProfileMessage::APPMFcoeProfileMessage ()
        : DcmManagementInterfaceMessage(APPMFCOEPROFILE)
    {
    }

    APPMFcoeProfileMessage::APPMFcoeProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &fcoePort)
        : DcmManagementInterfaceMessage(APPMFCOEPROFILE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_fcoePort    (fcoePort)
    {
    }

    APPMFcoeProfileMessage::~APPMFcoeProfileMessage ()
    {
    }

    void  APPMFcoeProfileMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_fcoePort,"fcoePort"));
    }

    void  APPMFcoeProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMFcoeProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMFcoeProfileMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMFcoeProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMFcoeProfileMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMFcoeProfileMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMFcoeProfileMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMFcoeProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMFcoeProfileMessage::setFcoePort(const bool &fcoePort)
    {
        m_fcoePort  =  fcoePort;
    }

    bool APPMFcoeProfileMessage::getFcoePort() const
    {
        return (m_fcoePort);
    }

    const void *APPMFcoeProfileMessage::getCStructureForInputs ()
    {
        fcoeProfileMsg_t *pInput = new fcoeProfileMsg_t;
        unsigned int len = 0;
	    pInput->opCode = m_opCode; 
	    pInput->cmdCode = m_cmdCode; 
		
        len = strlen(m_profileName.c_str());
        strncpy (pInput->profileName, m_profileName.c_str (), len);
		pInput->profileName[len] = '\0';
		pInput->profileId = m_profileId; 

		pInput->isFcoePort = m_fcoePort;
	    
        return (pInput);
    }

	void APPMFcoeProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}

}
