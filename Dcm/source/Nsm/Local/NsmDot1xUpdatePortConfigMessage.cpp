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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmDot1xUpdatePortConfigMessage.h"
#include "Nsm/Local/Dot1xPortConfigLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmDot1xUpdatePortConfigMessage::NsmDot1xUpdatePortConfigMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMDOT1XUPDATEPORTCONFIG)
    {
		m_cmdcode = 0;
		m_enabled = false;
		m_enable_auth = false;
		m_max_reauth_attempt = 0;
		m_opcode = 0;
		m_port_ctrl_type = 0;
		m_proto_version = 0;
		m_quiet_interval = 0;
		m_reauth_timeout_interval = 0;
		m_server_timeout_interval = 0;
		m_supp_timeout_interval = 0;
		m_tx_timeout_interval = 0;
        m_ifType = IF_TYPE_INVALID;
    }

    NsmDot1xUpdatePortConfigMessage::NsmDot1xUpdatePortConfigMessage (const string & ifName, const bool &enabled,const UI32 &port_ctrl_type,const UI32 &proto_version,const UI32 &quiet_interval,const UI32 &max_reauth_attempt,const bool &enable_auth,const UI32 &reauth_timeout_interval,const UI32 &server_timeout_interval,const UI32 &supp_timeout_interval,const UI32 &tx_timeout_interval,const UI32 &opcode,const UI32 &cmdcode)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMDOT1XUPDATEPORTCONFIG),
        m_ifName    (ifName),
        m_enabled    (enabled),
        m_port_ctrl_type    (port_ctrl_type),
        m_proto_version    (proto_version),
        m_quiet_interval    (quiet_interval),
        m_max_reauth_attempt    (max_reauth_attempt),
        m_enable_auth    (enable_auth),
        m_reauth_timeout_interval    (reauth_timeout_interval),
        m_server_timeout_interval    (server_timeout_interval),
        m_supp_timeout_interval    (supp_timeout_interval),
        m_tx_timeout_interval    (tx_timeout_interval),
        m_opcode    (opcode),
        m_cmdcode    (cmdcode)
    {
        m_ifType = IF_TYPE_INVALID;
    }

    NsmDot1xUpdatePortConfigMessage::~NsmDot1xUpdatePortConfigMessage ()
    {
    }

    void  NsmDot1xUpdatePortConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enabled,"enabled"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_port_ctrl_type,"port_ctrl_type"));
        addSerializableAttribute (new AttributeUI32(&m_proto_version,"proto_version"));
        addSerializableAttribute (new AttributeUI32(&m_quiet_interval,"quiet_interval"));
        addSerializableAttribute (new AttributeUI32(&m_max_reauth_attempt,"max_reauth_attempt"));
        addSerializableAttribute (new AttributeBool(&m_enable_auth,"enable_auth"));
        addSerializableAttribute (new AttributeUI32(&m_reauth_timeout_interval,"reauth_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_server_timeout_interval,"server_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_supp_timeout_interval,"supp_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_tx_timeout_interval,"tx_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void  NsmDot1xUpdatePortConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmDot1xUpdatePortConfigMessage::setEnabled(const bool &enabled)
    {
        m_enabled  =  enabled;
    }

    bool  NsmDot1xUpdatePortConfigMessage::getEnabled() const
    {
        return (m_enabled);
    }


    void  NsmDot1xUpdatePortConfigMessage::setPort_ctrl_type(const UI32 &port_ctrl_type)
    {
        m_port_ctrl_type  =  port_ctrl_type;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getPort_ctrl_type() const
    {
        return (m_port_ctrl_type);
    }

    void  NsmDot1xUpdatePortConfigMessage::setProto_version(const UI32 &proto_version)
    {
        m_proto_version  =  proto_version;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getProto_version() const
    {
        return (m_proto_version);
    }

    void  NsmDot1xUpdatePortConfigMessage::setQuiet_interval(const UI32 &quiet_interval)
    {
        m_quiet_interval  =  quiet_interval;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getQuiet_interval() const
    {
        return (m_quiet_interval);
    }

    void  NsmDot1xUpdatePortConfigMessage::setMax_reauth_attempt(const UI32 &max_reauth_attempt)
    {
        m_max_reauth_attempt  =  max_reauth_attempt;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getMax_reauth_attempt() const
    {
        return (m_max_reauth_attempt);
    }

    void  NsmDot1xUpdatePortConfigMessage::setEnable_auth(const bool &enable_auth)
    {
        m_enable_auth  =  enable_auth;
    }

    bool  NsmDot1xUpdatePortConfigMessage::getEnable_auth() const
    {
        return (m_enable_auth);
    }

    void  NsmDot1xUpdatePortConfigMessage::setReauth_timeout_interval(const UI32 &reauth_timeout_interval)
    {
        m_reauth_timeout_interval  =  reauth_timeout_interval;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getReauth_timeout_interval() const
    {
        return (m_reauth_timeout_interval);
    }

    void  NsmDot1xUpdatePortConfigMessage::setServer_timeout_interval(const UI32 &server_timeout_interval)
    {
        m_server_timeout_interval  =  server_timeout_interval;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getServer_timeout_interval() const
    {
        return (m_server_timeout_interval);
    }

    void  NsmDot1xUpdatePortConfigMessage::setSupp_timeout_interval(const UI32 &supp_timeout_interval)
    {
        m_supp_timeout_interval  =  supp_timeout_interval;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getSupp_timeout_interval() const
    {
        return (m_supp_timeout_interval);
    }

    void  NsmDot1xUpdatePortConfigMessage::setTx_timeout_interval(const UI32 &tx_timeout_interval)
    {
        m_tx_timeout_interval  =  tx_timeout_interval;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getTx_timeout_interval() const
    {
        return (m_tx_timeout_interval);
    }

    void  NsmDot1xUpdatePortConfigMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  NsmDot1xUpdatePortConfigMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  NsmDot1xUpdatePortConfigMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  NsmDot1xUpdatePortConfigMessage::setIfname(string  &ifName)
    {
        m_ifName  =  ifName ;
    }

    string  NsmDot1xUpdatePortConfigMessage::getIfname() const
    {
        return (m_ifName);
    }

}
