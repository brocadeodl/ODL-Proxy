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
 *   Author : kverma, asharma,sriramr                                      *
 **************************************************************************/

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalAPPMProfileMessage.h"
#include "APPM/Local/APPMLocalTypes.h"

namespace DcmNs
{

    APPMLocalAPPMProfileMessage::APPMLocalAPPMProfileMessage ()
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMPROFILE)
    {
    m_isActivated  = false;
	m_isAllow	   = false;
	m_isFloodingRestricted = false;
	}

    APPMLocalAPPMProfileMessage::APPMLocalAPPMProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &isActivated,const string &profileMac,const bool isAllow, const bool &isFloodingRestricted)
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMPROFILE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_profileMac    (profileMac),
		m_isAllow      (isAllow),
		m_isFloodingRestricted	(isFloodingRestricted)
    {
    }

    APPMLocalAPPMProfileMessage::~APPMLocalAPPMProfileMessage ()
    {
    }

    void  APPMLocalAPPMProfileMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_isActivated,"isActivated"));
        addSerializableAttribute (new AttributeString(&m_profileMac,"profileMac"));
		addSerializableAttribute (new AttributeBool(&m_isAllow,"isAllow"));
		addSerializableAttribute (new AttributeBool(&m_isFloodingRestricted,"isFloodingRestricted"));
    }

    void  APPMLocalAPPMProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMLocalAPPMProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMLocalAPPMProfileMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMLocalAPPMProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMLocalAPPMProfileMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMLocalAPPMProfileMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMLocalAPPMProfileMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMLocalAPPMProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMLocalAPPMProfileMessage::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  APPMLocalAPPMProfileMessage::getIsActivated() const
    {
        return (m_isActivated);
    }
	void  APPMLocalAPPMProfileMessage::setIsAllow(const bool isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  APPMLocalAPPMProfileMessage::getIsAllow() const
    {
        return (m_isAllow);
    }	
    void  APPMLocalAPPMProfileMessage::setProfileMac(const string &profileMac)
    {
        m_profileMac  =  profileMac;
    }

	void APPMLocalAPPMProfileMessage::setIsFloodingRestricted(const bool &isFloodingRestricted) {
        m_isFloodingRestricted = isFloodingRestricted;
    }

    bool APPMLocalAPPMProfileMessage::getIsFloodingRestricted() const {
        return m_isFloodingRestricted;
    }

    string  APPMLocalAPPMProfileMessage::getProfileMac() const
    {
        return (m_profileMac);
    }

}
