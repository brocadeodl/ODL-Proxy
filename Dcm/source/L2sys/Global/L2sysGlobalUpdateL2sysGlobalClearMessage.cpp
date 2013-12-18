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
 *   Author :                                                      *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalClearMessage.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"
#include "L2sys/Global/L2sysGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    L2sysGlobalUpdateL2sysGlobalClearMessage::L2sysGlobalUpdateL2sysGlobalClearMessage ()
        : ManagementInterfaceMessage (L2sysGlobalObjectManager::getClassName(),L2SYSGLOBALUPDATEL2SYSGLOBALCLEAR),
        m_rbridgeid    (0),
        m_vlanid    (0),
        m_type    (0),
        m_opcode    (0),
        m_interface_type (0),
        m_port_channel_id (0)
    {
    }

    L2sysGlobalUpdateL2sysGlobalClearMessage::L2sysGlobalUpdateL2sysGlobalClearMessage (const UI32 &rbridgeid,const string &ifname,const string &Mac,const UI32 &vlanid,const UI32 &type,const UI32 &opcode, const UI32 &interface_type, const UI32 &port_channel_id)
        : ManagementInterfaceMessage (L2sysGlobalObjectManager::getClassName(),L2SYSGLOBALUPDATEL2SYSGLOBALCLEAR),
        m_rbridgeid    (rbridgeid),
        m_ifname    (ifname),
        m_Mac    (Mac),
        m_vlanid    (vlanid),
        m_type    (type),
        m_opcode    (opcode),
        m_interface_type (interface_type),
        m_port_channel_id (port_channel_id)
    {
    }

    L2sysGlobalUpdateL2sysGlobalClearMessage::~L2sysGlobalUpdateL2sysGlobalClearMessage ()
    {
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_type,"type"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_interface_type,"interfaceType"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"portChannelId"));
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalClearMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysGlobalUpdateL2sysGlobalClearMessage::getMac() const
    {
        return (m_Mac);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setType(const UI32 &type)
    {
        m_type  =  type;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalClearMessage::getType() const
    {
        return (m_type);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysGlobalUpdateL2sysGlobalClearMessage::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalClearMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalClearMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setInterfaceType(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalClearMessage::getInterfaceType() const
    {
        return (m_interface_type);
    }

    void  L2sysGlobalUpdateL2sysGlobalClearMessage::setPortChannelId(const UI32 &port_channel_id)
    {
        m_port_channel_id  =  port_channel_id;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalClearMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

}
