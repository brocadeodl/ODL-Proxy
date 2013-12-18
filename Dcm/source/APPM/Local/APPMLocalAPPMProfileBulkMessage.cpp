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
 *   Author : jindala				                                       *
 **************************************************************************/

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalAPPMProfileBulkMessage.h"
#include "APPM/Local/APPMLocalTypes.h"

namespace DcmNs
{

    APPMLocalAPPMProfileBulkMessage::APPMLocalAPPMProfileBulkMessage ()
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMPROFILEBULK)
    {
    m_isActivated  = false;
    m_isAllow      = false;
    }

    APPMLocalAPPMProfileBulkMessage::APPMLocalAPPMProfileBulkMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &isActivated,const bool isAllow, const UI32 &macCount, const vector<string> &profileMacs)
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMPROFILEBULK),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_isActivated    (isActivated),
        m_isAllow      (isAllow),
        m_macCount       (macCount),
        m_profileMacs    (profileMacs)
    {
    }

    APPMLocalAPPMProfileBulkMessage::~APPMLocalAPPMProfileBulkMessage ()
    {
    }

    void  APPMLocalAPPMProfileBulkMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_isActivated,"isActivated"));
        addSerializableAttribute (new AttributeBool(&m_isAllow,"isAllow"));
        addSerializableAttribute (new AttributeUI32(&m_macCount,"macCount"));
        addSerializableAttribute (new AttributeStringVector(&m_profileMacs,"profileMacs"));
    }

    void  APPMLocalAPPMProfileBulkMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMLocalAPPMProfileBulkMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMLocalAPPMProfileBulkMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMLocalAPPMProfileBulkMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMLocalAPPMProfileBulkMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMLocalAPPMProfileBulkMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMLocalAPPMProfileBulkMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMLocalAPPMProfileBulkMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMLocalAPPMProfileBulkMessage::setIsActivated(const bool &isActivated)
    {
        m_isActivated  =  isActivated;
    }

    bool  APPMLocalAPPMProfileBulkMessage::getIsActivated() const
    {
        return (m_isActivated);
    }
    void  APPMLocalAPPMProfileBulkMessage::setIsAllow(const bool isAllow)
    {
        m_isAllow  =  isAllow;
    }

    bool  APPMLocalAPPMProfileBulkMessage::getIsAllow() const
    {
        return (m_isAllow);
    }

    void  APPMLocalAPPMProfileBulkMessage::setMacCount(const UI32 &macCount)
    {
        m_macCount  =  macCount;
    }

    UI32  APPMLocalAPPMProfileBulkMessage::getMacCount() const
    {
        return (m_macCount);
    }

    void  APPMLocalAPPMProfileBulkMessage::setProfileMacs(const vector<string> &profileMacs)
    {
        m_profileMacs  =  profileMacs;
    }

    vector<string>  APPMLocalAPPMProfileBulkMessage::getProfileMacs() const
    {
        return (m_profileMacs);
    }

	void APPMLocalAPPMProfileBulkMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

    void APPMLocalAPPMProfileBulkMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }


}
