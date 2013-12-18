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
 *   Author : bgangise, asharma                                            *
 **************************************************************************/

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalAPPMFcoeMessage.h"
#include "APPM/Local/APPMLocalTypes.h"

namespace DcmNs
{

    APPMLocalAPPMFcoeMessage::APPMLocalAPPMFcoeMessage ()
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId(),APPMLOCALAPPMFCOE)
    {
    }

    APPMLocalAPPMFcoeMessage::APPMLocalAPPMFcoeMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &fcoePort)
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId(),APPMLOCALAPPMFCOE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_fcoePort    (fcoePort)
    {
    }

    APPMLocalAPPMFcoeMessage::~APPMLocalAPPMFcoeMessage ()
    {
    }

    void  APPMLocalAPPMFcoeMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_fcoePort,"fcoePort"));
    }

    void  APPMLocalAPPMFcoeMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMLocalAPPMFcoeMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMLocalAPPMFcoeMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMLocalAPPMFcoeMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMLocalAPPMFcoeMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMLocalAPPMFcoeMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMLocalAPPMFcoeMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMLocalAPPMFcoeMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMLocalAPPMFcoeMessage::setFcoePort(const bool &fcoePort)
    {
        m_fcoePort  =  fcoePort;
    }

    bool  APPMLocalAPPMFcoeMessage::getFcoePort() const
    {
        return (m_fcoePort);
    }

}
