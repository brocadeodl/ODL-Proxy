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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMVnmLocalCdpNotifyMessage.h"
#include "APPM/Local/APPMLocalTypes.h"

namespace DcmNs
{

    APPMVnmLocalCdpNotifyMessage::APPMVnmLocalCdpNotifyMessage ()
        : ManagementInterfaceMessage (APPMLocalObjectManager::getClassName(),APPMLOCALVNMCDPNOTIFY)
    {
    }

    APPMVnmLocalCdpNotifyMessage::APPMVnmLocalCdpNotifyMessage (const UI32 &cmdCode,const string &ifName,const string &hostMor,const string &portId)
        : ManagementInterfaceMessage (APPMLocalObjectManager::getClassName(),APPMLOCALVNMCDPNOTIFY),
        m_cmdCode    (cmdCode),
        m_ifName    (ifName),
        m_hostMor    (hostMor),
        m_portId    (portId)
    {
    }

    APPMVnmLocalCdpNotifyMessage::~APPMVnmLocalCdpNotifyMessage ()
    {
    }

    void  APPMVnmLocalCdpNotifyMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_hostMor,"hostMor"));
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
    }

    void  APPMVnmLocalCdpNotifyMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  APPMVnmLocalCdpNotifyMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMVnmLocalCdpNotifyMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  APPMVnmLocalCdpNotifyMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  APPMVnmLocalCdpNotifyMessage::setHostMor(const string &hostMor)
    {
        m_hostMor  =  hostMor;
    }

    string  APPMVnmLocalCdpNotifyMessage::getHostMor() const
    {
        return (m_hostMor);
    }

    void  APPMVnmLocalCdpNotifyMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  APPMVnmLocalCdpNotifyMessage::getPortId() const
    {
        return (m_portId);
    }

}
