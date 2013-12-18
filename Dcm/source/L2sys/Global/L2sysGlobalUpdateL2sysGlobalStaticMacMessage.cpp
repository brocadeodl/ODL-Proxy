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

#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"
#include "L2sys/Global/L2sysGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    L2sysGlobalUpdateL2sysGlobalStaticMacMessage::L2sysGlobalUpdateL2sysGlobalStaticMacMessage ()
        : ManagementInterfaceMessage (L2sysGlobalObjectManager::getClassName (),L2SYSGLOBALUPDATEL2SYSGLOBALSTATICMAC),
        m_rbridgeid    (0),
        m_forward    (0),
        m_interface_type    (0),
        m_vlan    (0),
        m_vlanid    (0),
        m_slotid    (0),
        m_opcode    (0),
        m_port_channel_id (0)
    {
    }

    L2sysGlobalUpdateL2sysGlobalStaticMacMessage::L2sysGlobalUpdateL2sysGlobalStaticMacMessage (const UI32 &rbridgeid,const string &Mac,const UI32 &forward,const UI32 &interface_type,const string &ifname,const UI32 &vlan,const UI32 &vlanid,const UI32 &opcode, const UI32 &port_channel_id)
        : ManagementInterfaceMessage (L2sysGlobalObjectManager::getClassName (),L2SYSGLOBALUPDATEL2SYSGLOBALSTATICMAC),
        m_rbridgeid    (rbridgeid),
        m_Mac    (Mac),
        m_forward    (forward),
        m_interface_type    (interface_type),
        m_ifname    (ifname),
        m_vlan    (vlan),
        m_vlanid    (vlanid),
        m_opcode    (opcode),
        m_port_channel_id (port_channel_id)
    {
    }

    L2sysGlobalUpdateL2sysGlobalStaticMacMessage::~L2sysGlobalUpdateL2sysGlobalStaticMacMessage ()
    {
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_forward,"forward"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"portChannelId"));
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getMac() const
    {
        return (m_Mac);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setForward(const UI32 &forward)
    {
        m_forward  =  forward;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getForward() const
    {
        return (m_forward);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setInterface_type(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setSlotid(const UI32 &slotid)
    {
        m_slotid  =  slotid;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getSlotid() const
    {
        return (m_slotid);
    }


    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::setPortChannelId(const UI32 &port_channel_id)
    {
        m_port_channel_id  = port_channel_id;
    }

    UI32  L2sysGlobalUpdateL2sysGlobalStaticMacMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

}
