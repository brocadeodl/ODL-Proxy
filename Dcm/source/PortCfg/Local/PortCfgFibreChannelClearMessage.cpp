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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgFibreChannelClearMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgFibreChannelClearMessage::PortCfgFibreChannelClearMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFIBRECHANNELCLEAR)
    {
    }

    PortCfgFibreChannelClearMessage::PortCfgFibreChannelClearMessage (const UI32 &cmdCode,const SI32 &switchPort,const UI32 &slotId,const string &portId,const UI32 &rbridgeId)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFIBRECHANNELCLEAR),
        m_cmdCode    (cmdCode),
        m_switchPort    (switchPort),
        m_slotId    (slotId),
        m_portId    (portId),
        m_rbridgeId    (rbridgeId)
    {
    }

    PortCfgFibreChannelClearMessage::~PortCfgFibreChannelClearMessage ()
    {
    }

    void  PortCfgFibreChannelClearMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_switchPort,"switchPort"));
        addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
        addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
    }

    void  PortCfgFibreChannelClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PortCfgFibreChannelClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PortCfgFibreChannelClearMessage::setSwitchPort(const SI32 &switchPort)
    {
        m_switchPort  =  switchPort;
    }

    SI32  PortCfgFibreChannelClearMessage::getSwitchPort() const
    {
        return (m_switchPort);
    }

    void  PortCfgFibreChannelClearMessage::setSlotId(const UI32 &slotId)
    {
        m_slotId  =  slotId;
    }

    UI32  PortCfgFibreChannelClearMessage::getSlotId() const
    {
        return (m_slotId);
    }

    void  PortCfgFibreChannelClearMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  PortCfgFibreChannelClearMessage::getPortId() const
    {
        return (m_portId);
    }

    void  PortCfgFibreChannelClearMessage::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  PortCfgFibreChannelClearMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

}
