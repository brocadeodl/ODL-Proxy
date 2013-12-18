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

#include "ClientInterface/L2sys/L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"

namespace DcmNs
{

    L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage ()
        : DcmManagementInterfaceMessage (L2SYSCLIENTGLOBALUPDATEL2SYSGLOBALSTATICMAC),
        m_rbridgeid    (0),
        m_forward    (0),
        m_interface_type    (0),
        m_ifindex    (0),
        m_vlan    (0),
        m_vlanid    (0),
        m_slotid    (0),
        m_opcode    (0),
        m_port_channel_id (0)
    {
    }

    L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage (const UI32 &rbridgeid,const string &Mac,const UI32 &forward,const UI32 &interface_type,const string &ifname,const UI64 &ifindex,const UI32 &vlan,const UI32 &vlanid,const UI32 &opcode, const UI32 &port_channel_id)
        : DcmManagementInterfaceMessage (L2SYSCLIENTGLOBALUPDATEL2SYSGLOBALSTATICMAC),
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

    L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::~L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage ()
    {
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeUI32(&m_forward,"forward"));
        addSerializableAttribute (new AttributeUI32(&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeUI64(&m_ifindex,"ifindex"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32(&m_slotid,"slotid"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_port_channel_id,"portChannelId"));
    }

    void L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getMac() const
    {
        return (m_Mac);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setForward(const UI32 &forward)
    {
        m_forward  =  forward;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getForward() const
    {
        return (m_forward);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setInterface_type(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setIfindex(const UI64 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI64  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getIfindex() const
    {
        return (m_ifindex);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setSlotid(const UI32 &slotid)
    {
        m_slotid  =  slotid;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getSlotid() const
    {
        return (m_slotid);
    }

	
    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::setPortChannelId(const UI32 &port_channel_id)
    {
       m_port_channel_id  = port_channel_id;
    }

    UI32  L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getPortChannelId() const
    {
        return (m_port_channel_id);
    }

    const void *L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::getCStructureForInputs ()
    {
	  	L2sysConfig_InputMsg_t *pInput = (L2sysConfig_InputMsg_t *)malloc (sizeof (L2sysConfig_InputMsg_t));

		strncpy (pInput->Mac, m_Mac.c_str (), 16);
		strncpy (pInput->if_name, m_ifname.c_str (), 64);
        pInput->vlan_id= m_vlanid;
        pInput->slotid= m_slotid;
        pInput->bridge= m_rbridgeid;
        pInput->interface_type= m_interface_type;
        pInput->cmdCode= m_opcode ;
        pInput->vlan= m_vlan ;
        pInput->if_index= m_ifindex ;
        pInput->forward= m_forward ;
        pInput->cmdCode= m_opcode ;
        pInput->port_channel_id = m_port_channel_id;
        return (pInput);
    }

    void L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
