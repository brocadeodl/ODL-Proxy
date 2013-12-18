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

#include "ClientInterface/APPM/APPMSecProfileMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    APPMSecProfileMessage::APPMSecProfileMessage ()
        : DcmManagementInterfaceMessage(APPMSECPROFILE)
    {
    }

    APPMSecProfileMessage::APPMSecProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const string &accessGroup)
        : DcmManagementInterfaceMessage(APPMSECPROFILE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_accessGroup    (accessGroup)
    {
    }

    APPMSecProfileMessage::~APPMSecProfileMessage ()
    {
    }

    void  APPMSecProfileMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeString(&m_accessGroup,"accessGroup"));
    }

    void  APPMSecProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMSecProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMSecProfileMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMSecProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMSecProfileMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMSecProfileMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMSecProfileMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMSecProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMSecProfileMessage::setAccessGroup(const string &accessGroup)
    {
        m_accessGroup  =  accessGroup;
    }

    string  APPMSecProfileMessage::getAccessGroup() const
    {
        return (m_accessGroup);
    }

    const void *APPMSecProfileMessage::getCStructureForInputs ()
    {
        secProfileMsg_t *pInput = new secProfileMsg_t;
        unsigned int len = 0;
	    pInput->opCode = m_opCode; 
	    pInput->cmdCode = m_cmdCode; 
		
        len = strlen(m_profileName.c_str());
        strncpy (pInput->profileName, m_profileName.c_str (), len);
		pInput->profileName[len] = '\0';
		
		len = strlen(m_accessGroup.c_str());
        strncpy (pInput->accessGroup, m_accessGroup.c_str (), len);
		pInput->accessGroup[len] = '\0';
	    
		pInput->profileId = m_profileId; 
        return (pInput);
    }

	void APPMSecProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}

}
