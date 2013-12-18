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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalStaticMacSPMessage.h"
#include "L2sys/Local/L2sysTypes.h"
//#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    L2sysUpdateL2sysGlobalStaticMacSPMessage::L2sysUpdateL2sysGlobalStaticMacSPMessage ()
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSUPDATEL2SYSGLOBALSTATICMACSP),
        m_rbridgeid    (0),
        m_forward    (0),
        m_interface_type    (0),
        m_ifindex    (0),
        m_vlan    (0),
        m_vlanid    (0),
        m_opcode    (0),
        m_port_channel_id (0)
    {
    }

    L2sysUpdateL2sysGlobalStaticMacSPMessage::L2sysUpdateL2sysGlobalStaticMacSPMessage (const UI32 &rbridgeid,const string &Mac,const UI32 &forward,const UI32 &interface_type,const string &ifname,const UI32 &ifindex,const UI32 &vlan,const UI32 &vlanid,const UI32 &opcode, const UI32 &port_channel_id)
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSUPDATEL2SYSGLOBALSTATICMACSP),
        m_rbridgeid    (rbridgeid),
        m_Mac    (Mac),
        m_forward    (forward),
        m_interface_type    (interface_type),
        m_ifname    (ifname),
        m_ifindex    (ifindex),
        m_vlan    (vlan),
        m_vlanid    (vlanid),
        m_opcode    (opcode),
        m_port_channel_id (port_channel_id)
    {
    }

    L2sysUpdateL2sysGlobalStaticMacSPMessage::~L2sysUpdateL2sysGlobalStaticMacSPMessage ()
    {
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_forward,"forward"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_ifindex,"ifindex"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"portChannelId"));
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysUpdateL2sysGlobalStaticMacSPMessage::getMac() const
    {
        return (m_Mac);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setForward(const UI32 &forward)
    {
        m_forward  =  forward;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getForward() const
    {
        return (m_forward);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setInterface_type(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysUpdateL2sysGlobalStaticMacSPMessage::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setIfindex(const UI32 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getIfindex() const
    {
        return (m_ifindex);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getOpcode() const
    {
        return (m_opcode);
    }
    
    void  L2sysUpdateL2sysGlobalStaticMacSPMessage::setPortChannelId(const UI32 &port_channel_id)
    {
        m_port_channel_id  = port_channel_id;
    }

    UI32  L2sysUpdateL2sysGlobalStaticMacSPMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

}
