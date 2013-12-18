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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgGetFCPortInfoMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgGetFCPortInfoMessage::PortCfgGetFCPortInfoMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGGETFCPORTINFO)
    {
    }

    PortCfgGetFCPortInfoMessage::PortCfgGetFCPortInfoMessage (const UI32 &cmdCode,const UI32 &rbridgeId,const UI32 &slotId,const UI32 &respCode)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGGETFCPORTINFO),
        m_cmdCode    (cmdCode),
        m_rbridgeId    (rbridgeId),
        m_slotId    (slotId),
        m_respCode    (respCode)
    {
    }

    PortCfgGetFCPortInfoMessage::~PortCfgGetFCPortInfoMessage ()
    {
    }

    void  PortCfgGetFCPortInfoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
        addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
        addSerializableAttribute (new AttributeUI32(&m_respCode,"respCode"));
    }

    void  PortCfgGetFCPortInfoMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PortCfgGetFCPortInfoMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PortCfgGetFCPortInfoMessage::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  PortCfgGetFCPortInfoMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

    void  PortCfgGetFCPortInfoMessage::setSlotId(const UI32 &slotId)
    {
        m_slotId  =  slotId;
    }

    UI32  PortCfgGetFCPortInfoMessage::getSlotId() const
    {
        return (m_slotId);
    }

    void  PortCfgGetFCPortInfoMessage::setRespCode(const UI32 &respCode)
    {
        m_respCode  =  respCode;
    }

    UI32  PortCfgGetFCPortInfoMessage::getRespCode() const
    {
        return (m_respCode);
    }

}
