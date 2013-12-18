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

#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalClearSPMessage.h"
#include "L2sys/Local/L2sysTypes.h"
//#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    L2sysUpdateL2sysGlobalClearSPMessage::L2sysUpdateL2sysGlobalClearSPMessage ()
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSUPDATEL2SYSGLOBALCLEARSP),
        m_rbridgeid    (0),
        m_vlanid    (0),
        m_type    (0),
        m_opcode    (0),
        m_interface_type (0),
        m_port_channel_id (0)
    {
    }

    L2sysUpdateL2sysGlobalClearSPMessage::L2sysUpdateL2sysGlobalClearSPMessage (const UI32 &rbridgeid,const string &ifname,const string &Mac,const UI32 &vlanid,const UI32 &type,const UI32 &opcode, const UI32 interface_type,const UI32 port_channel_id)
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSUPDATEL2SYSGLOBALCLEARSP),
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

    L2sysUpdateL2sysGlobalClearSPMessage::~L2sysUpdateL2sysGlobalClearSPMessage ()
    {
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_type,"type"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_interface_type,"interfaceType"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"portChannelId"));
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  L2sysUpdateL2sysGlobalClearSPMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysUpdateL2sysGlobalClearSPMessage::getMac() const
    {
        return (m_Mac);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setType(const UI32 &type)
    {
        m_type  =  type;
    }

    UI32  L2sysUpdateL2sysGlobalClearSPMessage::getType() const
    {
        return (m_type);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysUpdateL2sysGlobalClearSPMessage::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysUpdateL2sysGlobalClearSPMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  L2sysUpdateL2sysGlobalClearSPMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setInterfaceType(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysUpdateL2sysGlobalClearSPMessage::getInterfaceType() const
    {
        return (m_interface_type);
    }

    void  L2sysUpdateL2sysGlobalClearSPMessage::setPortChannelId(const UI32 &port_channel_id)
    {
       m_port_channel_id = port_channel_id;
    }

    UI32  L2sysUpdateL2sysGlobalClearSPMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

}
