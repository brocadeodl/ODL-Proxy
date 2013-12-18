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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/

#include "APPM/Global/APPMGlobalAPPMProfileBulkMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMGlobalAPPMProfileBulkMessage::APPMGlobalAPPMProfileBulkMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMPROFILEBULK)
    {
        m_isActivated = false;
        m_isAllow     = false;
    }

    APPMGlobalAPPMProfileBulkMessage::APPMGlobalAPPMProfileBulkMessage (const UI8
        &opCode,const UI8 &cmdCode,const string &profileName,const UI32
        &profileId,const bool &isActivated,const bool &isAllow, const UI32 &macCount, const vector<string> &profileMacs)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMPROFILEBULK),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_isAllow        (isAllow),
		m_macCount       (macCount),
        m_profileMacs    (profileMacs)
    {
    }

    APPMGlobalAPPMProfileBulkMessage::~APPMGlobalAPPMProfileBulkMessage ()
    {
    }

    void  APPMGlobalAPPMProfileBulkMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_isActivated,"isActivated"));
        addSerializableAttribute (new AttributeBool(&m_isAllow,"isAllow"));
		addSerializableAttribute (new AttributeUI32(&m_macCount,"macCount"));
        addSerializableAttribute (new AttributeStringVector(&m_profileMacs,"profileMacs"));
    }

    void  APPMGlobalAPPMProfileBulkMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalAPPMProfileBulkMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMGlobalAPPMProfileBulkMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalAPPMProfileBulkMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMGlobalAPPMProfileBulkMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMGlobalAPPMProfileBulkMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMGlobalAPPMProfileBulkMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMGlobalAPPMProfileBulkMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMGlobalAPPMProfileBulkMessage::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  APPMGlobalAPPMProfileBulkMessage::getIsActivated() const
    {
        return (m_isActivated);
    }
    void  APPMGlobalAPPMProfileBulkMessage::setIsAllow(const bool &isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  APPMGlobalAPPMProfileBulkMessage::getIsAllow() const
    {
        return (m_isAllow);
    }

    void  APPMGlobalAPPMProfileBulkMessage::setMacCount(const UI32 &macCount)
    {
        m_macCount  =  macCount;
    }

    UI32  APPMGlobalAPPMProfileBulkMessage::getMacCount() const
    {
        return (m_macCount);
    }

    void  APPMGlobalAPPMProfileBulkMessage::setProfileMacs(const vector<string> &profileMacs)
    {
        m_profileMacs  =  profileMacs;
    }

    vector<string>  APPMGlobalAPPMProfileBulkMessage::getProfileMacs() const
    {
        return (m_profileMacs);
    }

	void APPMGlobalAPPMProfileBulkMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
	
    void APPMGlobalAPPMProfileBulkMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

}
