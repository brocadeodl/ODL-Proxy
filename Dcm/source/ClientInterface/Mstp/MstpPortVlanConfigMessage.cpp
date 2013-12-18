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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Mstp/MstpPortVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Nsm/Local/NsmUpdatePoVlanStpConfigSPMessage.h"
#include "Nsm/Local/NsmUpdatePhyVlanStpConfigMessage.h"

namespace DcmNs
{

    MstpPortVlanConfigMessage::MstpPortVlanConfigMessage ()
        : DcmManagementInterfaceMessage (MSTPPORTVLANCONFIG)
    {
        m_ifType    = IF_TYPE_INVALID;
        m_ifName    = "";
        m_vid       = 0;
        m_priority  = 0;
        m_cost      = 0;
        m_guard     = 0;
        m_restrictedTcn = 0;
        m_restrictedRole= 0;
        m_cmdCode       = 0;
    }

    MstpPortVlanConfigMessage::MstpPortVlanConfigMessage(NsmUpdatePhyVlanStpConfigMessage &message)
        : DcmManagementInterfaceMessage (MSTPPORTVLANCONFIG)
    {
        m_ifType    = message.m_ifType;
        m_ifName    = message.m_ifName;
        m_vid       = message.m_vid;
        m_priority  = message.m_priority;
        m_cost      = message.m_cost;
        m_guard     = 0;
        m_restrictedTcn     = 0;
        m_restrictedRole    = 0;
        m_cmdCode   = message.m_cmdCode;
    }

    MstpPortVlanConfigMessage::MstpPortVlanConfigMessage(NsmUpdatePoVlanStpConfigSPMessage &message)
        : DcmManagementInterfaceMessage (MSTPPORTVLANCONFIG)
    {
        m_ifType = IF_TYPE_PO;
        m_ifName = message.m_ifName;
        m_vid = message.m_vid;
        m_priority = message.m_priority;
        m_cost = message.m_cost;
        m_guard     = 0;
        m_restrictedTcn     = 0;
        m_restrictedRole    = 0;
        m_cmdCode = message.m_cmdCode;
    }


    MstpPortVlanConfigMessage::MstpPortVlanConfigMessage (const InterfaceType &ifType, const string &ifName,const UI32 &vid,const UI16 &priority,const UI32 &cost,const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (MSTPPORTVLANCONFIG),
        m_ifType    ( ifType ),
        m_ifName    (ifName),
        m_vid       (vid),
        m_priority  (priority),
        m_cost      (cost),
        m_guard     (0),
        m_restrictedTcn (0),
        m_restrictedRole(0),
        m_cmdCode   (cmdCode)
    {
    }

    MstpPortVlanConfigMessage::~MstpPortVlanConfigMessage ()
    {
    }

    void  MstpPortVlanConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_ifType), "ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_vid,"vid"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"cost"));
        addSerializableAttribute (new AttributeUI32(&m_guard,"guardroot"));
        addSerializableAttribute (new AttributeUI32(&m_restrictedTcn,"restrictedTcn"));
        addSerializableAttribute (new AttributeUI32(&m_restrictedRole,"restrictedRole"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  MstpPortVlanConfigMessage::setInterfaceInfo(const InterfaceType &ifType, const string &ifName)
    {
        m_ifType  =  ifType;
        m_ifName  =  ifName;
    }

    InterfaceType MstpPortVlanConfigMessage::getIfType() const
    {
        return m_ifType;
    }

    string  MstpPortVlanConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  MstpPortVlanConfigMessage::setVid(const UI32 &vid)
    {
        m_vid  =  vid;
    }

    UI32  MstpPortVlanConfigMessage::getVid() const
    {
        return (m_vid);
    }

    void  MstpPortVlanConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpPortVlanConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpPortVlanConfigMessage::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  MstpPortVlanConfigMessage::getCost() const
    {
        return (m_cost);
    }

    void  MstpPortVlanConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpPortVlanConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpPortVlanConfigMessage::setGuard(const UI32 &guard)
    {
        m_guard  =  guard;
    }

    UI32  MstpPortVlanConfigMessage::getGuard() const
    {
        return (m_guard);
    }

    void  MstpPortVlanConfigMessage::setRestrictedTcn(const UI32 &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }

    UI32  MstpPortVlanConfigMessage::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  MstpPortVlanConfigMessage::setRestrictedRole(const UI32 &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    UI32  MstpPortVlanConfigMessage::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

    const void *MstpPortVlanConfigMessage::getCStructureForInputs()
	{
		mstp_msg_port_vlan_cfg *port_vlan_cfg = new mstp_msg_port_vlan_cfg;
		memset(port_vlan_cfg, 0, sizeof(mstp_msg_port_vlan_cfg));

		port_vlan_cfg->cmd = m_cmdCode;
		port_vlan_cfg->id = m_vid;
		port_vlan_cfg->cost = m_cost;
		port_vlan_cfg->priority = m_priority;
		port_vlan_cfg->guard_root = m_guard;
		port_vlan_cfg->restricted_tcn = m_restrictedTcn;
		port_vlan_cfg->restricted_role = m_restrictedRole;

		MstpPortConfigMessage::writeIfName(port_vlan_cfg->name, m_ifType, m_ifName);
        port_vlan_cfg->if_type = m_ifType;

		return (port_vlan_cfg);
	}

}
