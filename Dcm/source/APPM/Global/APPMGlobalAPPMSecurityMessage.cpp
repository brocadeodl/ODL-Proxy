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

#include "APPM/Global/APPMGlobalAPPMSecurityMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMGlobalAPPMSecurityMessage::APPMGlobalAPPMSecurityMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMSECURITY)
    {
    }

    APPMGlobalAPPMSecurityMessage::APPMGlobalAPPMSecurityMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const string &accessGroup)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMSECURITY),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_accessGroup    (accessGroup)
    {
    }

    APPMGlobalAPPMSecurityMessage::~APPMGlobalAPPMSecurityMessage ()
    {
    }

    void  APPMGlobalAPPMSecurityMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeString(&m_accessGroup,"accessGroup"));
    }

    void  APPMGlobalAPPMSecurityMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalAPPMSecurityMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMGlobalAPPMSecurityMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalAPPMSecurityMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMGlobalAPPMSecurityMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMGlobalAPPMSecurityMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMGlobalAPPMSecurityMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMGlobalAPPMSecurityMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMGlobalAPPMSecurityMessage::setAccessGroup(const string &accessGroup)
    {
        m_accessGroup  =  accessGroup;
    }

    string  APPMGlobalAPPMSecurityMessage::getAccessGroup() const
    {
        return (m_accessGroup);
    }

}
