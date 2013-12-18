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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/APPMGlobalAPPMProfileMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMGlobalAPPMProfileMessage::APPMGlobalAPPMProfileMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMPROFILE)
    {
		m_isActivated = false;
    	m_isAllow	  = false;
		m_isFloodingRestricted = false;
	}

    APPMGlobalAPPMProfileMessage::APPMGlobalAPPMProfileMessage (const UI8
		&opCode,const UI8 &cmdCode,const string &profileName,const UI32
		&profileId,const bool &isActivated,const string &profileMac,const bool
		&isAllow, const bool &isFloodingRestricted, const vector<string> &profileMacs)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMPROFILE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_profileMac    (profileMac),
		m_isAllow        (isAllow),
		m_isFloodingRestricted  (isFloodingRestricted),
		m_profileMacs    (profileMacs)
    {
    }

    APPMGlobalAPPMProfileMessage::~APPMGlobalAPPMProfileMessage ()
    {
    }

    void  APPMGlobalAPPMProfileMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_isActivated,"isActivated"));
        addSerializableAttribute (new AttributeString(&m_profileMac,"profileMac"));
		addSerializableAttribute (new AttributeBool(&m_isAllow,"isAllow"));
		addSerializableAttribute (new AttributeBool(&m_isFloodingRestricted,"isFloodingRestricted"));
		addSerializableAttribute (new AttributeString(&m_profileMac,"profileMacs"));
    }

    void  APPMGlobalAPPMProfileMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalAPPMProfileMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMGlobalAPPMProfileMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalAPPMProfileMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMGlobalAPPMProfileMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMGlobalAPPMProfileMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMGlobalAPPMProfileMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMGlobalAPPMProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMGlobalAPPMProfileMessage::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  APPMGlobalAPPMProfileMessage::getIsActivated() const
    {
        return (m_isActivated);
    }
	void  APPMGlobalAPPMProfileMessage::setIsAllow(const bool &isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  APPMGlobalAPPMProfileMessage::getIsAllow() const
    {
        return (m_isAllow);
    }
	void APPMGlobalAPPMProfileMessage::setIsFloodingRestricted(const bool &isFloodingRestricted) {
        m_isFloodingRestricted = isFloodingRestricted;
    }

    bool APPMGlobalAPPMProfileMessage::getIsFloodingRestricted() const {
        return m_isFloodingRestricted;
    }	
    void  APPMGlobalAPPMProfileMessage::setProfileMac(const string &profileMac)
    {
        m_profileMac  =  profileMac;
    }

    string  APPMGlobalAPPMProfileMessage::getProfileMac() const
    {
        return (m_profileMac);
    }

	void  APPMGlobalAPPMProfileMessage::setProfileMacs(const vector<string> &profileMacs)
    {
        m_profileMacs  =  profileMacs;
    }

    vector<string>  APPMGlobalAPPMProfileMessage::getProfileMacs() const
    {
        return (m_profileMacs);
    }
}
