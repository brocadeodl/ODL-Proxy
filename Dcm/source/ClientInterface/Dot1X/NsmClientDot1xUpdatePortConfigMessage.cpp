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
#include "ClientInterface/Dot1X/NsmClientDot1xUpdatePortConfigMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"

namespace DcmNs
{

    NsmClientDot1xUpdatePortConfigMessage::NsmClientDot1xUpdatePortConfigMessage ()
        : DcmManagementInterfaceMessage  (DOT1X_CLIENT_CONFIG)
    {
    }

    NsmClientDot1xUpdatePortConfigMessage::NsmClientDot1xUpdatePortConfigMessage (const string & ifName, const bool &enabled,const UI32 &port_ctrl_type,const UI32 &proto_version,const UI32 &quiet_interval,const UI32 &max_reauth_attempt,const bool &enable_auth,const UI32 &reauth_timeout_interval,const UI32 &server_timeout_interval,const UI32 &supp_timeout_interval,const UI32 &tx_timeout_interval,const UI32 &opcode,const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (DOT1X_CLIENT_CONFIG),
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
        m_cmdCode    (cmdCode)
    {
    	m_ifType = IF_TYPE_INVALID;
    }

    NsmClientDot1xUpdatePortConfigMessage::~NsmClientDot1xUpdatePortConfigMessage ()
    {
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifname"));
        addSerializableAttribute (new AttributeBool(&m_enabled,"enabled"));
        addSerializableAttribute (new AttributeUI32(&m_port_ctrl_type,"port_ctrl_type"));
        addSerializableAttribute (new AttributeUI32(&m_proto_version,"proto_version"));
        addSerializableAttribute (new AttributeUI32(&m_quiet_interval,"quiet_interval"));
        addSerializableAttribute (new AttributeUI32(&m_max_reauth_attempt,"max_reauth_attempt"));
        addSerializableAttribute (new AttributeBool(&m_enable_auth,"enable_auth"));
        addSerializableAttribute (new AttributeUI32(&m_reauth_timeout_interval,"reauth_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_server_timeout_interval,"server_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_supp_timeout_interval,"supp_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_tx_timeout_interval,"tx_timeout_interval"));
        addSerializableAttribute (new AttributeUI32(&m_opcode,"opcode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void NsmClientDot1xUpdatePortConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32 NsmClientDot1xUpdatePortConfigMessage::getIfType() const
    {
        return (m_ifType);
    }


    void  NsmClientDot1xUpdatePortConfigMessage::setEnabled(const bool &enabled)
    {
        m_enabled  =  enabled;
    }

    bool  NsmClientDot1xUpdatePortConfigMessage::getEnabled() const
    {
        return (m_enabled);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setIfName(const string  &ifName)
    {
        m_ifName  =  ifName ;
    }

    string  NsmClientDot1xUpdatePortConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setPort_ctrl_type(const UI32 &port_ctrl_type)
    {
        m_port_ctrl_type  =  port_ctrl_type;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getPort_ctrl_type() const
    {
        return (m_port_ctrl_type);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setProto_version(const UI32 &proto_version)
    {
        m_proto_version  =  proto_version;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getProto_version() const
    {
        return (m_proto_version);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setQuiet_interval(const UI32 &quiet_interval)
    {
        m_quiet_interval  =  quiet_interval;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getQuiet_interval() const
    {
        return (m_quiet_interval);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setMax_reauth_attempt(const UI32 &max_reauth_attempt)
    {
        m_max_reauth_attempt  =  max_reauth_attempt;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getMax_reauth_attempt() const
    {
        return (m_max_reauth_attempt);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setEnable_auth(const bool &enable_auth)
    {
        m_enable_auth  =  enable_auth;
    }

    bool  NsmClientDot1xUpdatePortConfigMessage::getEnable_auth() const
    {
        return (m_enable_auth);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setReauth_timeout_interval(const UI32 &reauth_timeout_interval)
    {
        m_reauth_timeout_interval  =  reauth_timeout_interval;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getReauth_timeout_interval() const
    {
        return (m_reauth_timeout_interval);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setServer_timeout_interval(const UI32 &server_timeout_interval)
    {
        m_server_timeout_interval  =  server_timeout_interval;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getServer_timeout_interval() const
    {
        return (m_server_timeout_interval);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setSupp_timeout_interval(const UI32 &supp_timeout_interval)
    {
        m_supp_timeout_interval  =  supp_timeout_interval;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getSupp_timeout_interval() const
    {
        return (m_supp_timeout_interval);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setTx_timeout_interval(const UI32 &tx_timeout_interval)
    {
        m_tx_timeout_interval  =  tx_timeout_interval;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getTx_timeout_interval() const
    {
        return (m_tx_timeout_interval);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setOpcode(const UI32 &opcode)
    {
        m_opcode  =  opcode;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getOpcode() const
    {
        return (m_opcode);
    }

    void  NsmClientDot1xUpdatePortConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmClientDot1xUpdatePortConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    const void *NsmClientDot1xUpdatePortConfigMessage::getCStructureForInputs ()
    {
		dot1x_config_Inputmsg_t *pInput = (dot1x_config_Inputmsg_t *)malloc (sizeof (dot1x_config_Inputmsg_t));

		strncpy (pInput->ifname, m_ifName.c_str(), 64);
		pInput->ifname[63] = '\0';
		pInput->iftype = m_ifType;

		pInput->enabled = m_enabled;
		pInput->port_ctrl_type = m_port_ctrl_type;
		pInput->proto_version = m_proto_version;
		pInput->quiet_interval = m_quiet_interval;
		pInput->max_reauth_attempt = m_max_reauth_attempt;
		pInput->enable_auth = m_enable_auth;
		pInput->reauth_timeout_interval = m_reauth_timeout_interval;
		pInput->server_timeout_interval = m_server_timeout_interval;
		pInput->supp_timeout_interval = m_supp_timeout_interval;
		pInput->tx_timeout_interval = m_tx_timeout_interval;
		pInput->cmdCode = m_cmdCode;
		pInput->opCode = m_opcode;

        return (pInput);
    }

    void NsmClientDot1xUpdatePortConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
	  return;
    }

}
