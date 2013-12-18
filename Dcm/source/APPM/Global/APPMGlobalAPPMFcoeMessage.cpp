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

#include "APPM/Global/APPMGlobalAPPMFcoeMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMGlobalAPPMFcoeMessage::APPMGlobalAPPMFcoeMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMFCOE)
    {
    }

    APPMGlobalAPPMFcoeMessage::APPMGlobalAPPMFcoeMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &fcoePort)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMFCOE),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_fcoePort    (fcoePort)
	
    {
    }

    APPMGlobalAPPMFcoeMessage::~APPMGlobalAPPMFcoeMessage ()
    {
    }

    void  APPMGlobalAPPMFcoeMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeBool(&m_fcoePort,"fcoePort"));
	addSerializableAttribute (new AttributeString(&m_mapName,"fcoemapname"));
}

    void  APPMGlobalAPPMFcoeMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalAPPMFcoeMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMGlobalAPPMFcoeMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalAPPMFcoeMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMGlobalAPPMFcoeMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMGlobalAPPMFcoeMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMGlobalAPPMFcoeMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMGlobalAPPMFcoeMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMGlobalAPPMFcoeMessage::setFcoePort(const bool &fcoePort)
    {
        m_fcoePort  =  fcoePort;
    }

    bool  APPMGlobalAPPMFcoeMessage::getFcoePort() const
    {
        return (m_fcoePort);
    }
    
    void  APPMGlobalAPPMFcoeMessage::setMapName(const string &map_name)
    {
        m_mapName = map_name;
    }
    string APPMGlobalAPPMFcoeMessage::getMapName() const
    {
       return (m_mapName);
    }
}
