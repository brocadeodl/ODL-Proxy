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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "ClientInterface/Fcoe/FcoeClientIntfMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientIntfMessage::FcoeClientIntfMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_INTF_CONFIG)
    {
        m_oper_bmp = 0;
        m_ifName = "";
        m_ifIndex = 0;
        m_desc = "";
        m_status = 0;
        m_mode = "";
        m_binding = "";
        m_binding_type = 0;
        m_config_replay = 0;
    }

    FcoeClientIntfMessage::~FcoeClientIntfMessage ()
    {
    }

    void
    FcoeClientIntfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_status,"status"));
        addSerializableAttribute (new AttributeString(&m_mode,"mode"));
        addSerializableAttribute (new AttributeString(&m_binding,"binding"));
        addSerializableAttribute (new AttributeUI32(&m_fc_port_num, "fc_port_num"));
        addSerializableAttribute (new AttributeUI32(&m_port_num, "port_num"));
        addSerializableAttribute (new AttributeUI32(&m_vn_num, "vn_num"));
        addSerializableAttribute (new AttributeUI32(&m_binding_type, "binding_type"));
    }

    void
    FcoeClientIntfMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32
    FcoeClientIntfMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void
    FcoeClientIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string
    FcoeClientIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void
    FcoeClientIntfMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64
    FcoeClientIntfMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void
    FcoeClientIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string
    FcoeClientIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void
    FcoeClientIntfMessage::setStatus(const bool &status)
    {
        m_status  =  status;
    }

    bool
    FcoeClientIntfMessage::getStatus() const
    {
        return (m_status);
    }

    void
    FcoeClientIntfMessage::setMode(const string &mode)
    {
        m_mode  =  mode;
    }

    string
    FcoeClientIntfMessage::getMode() const
    {
        return (m_mode);
    }

    void
    FcoeClientIntfMessage::setBinding(const string &binding)
    {
        m_binding  =  binding;
    }

    string
    FcoeClientIntfMessage::getBinding() const
    {
        return (m_binding);
    }

    void 
    FcoeClientIntfMessage::setFcPortNum(const UI32 &uport_num)
    {
        m_fc_port_num = uport_num;
    }

    UI32 
    FcoeClientIntfMessage::getFcPortNum () const
    {
        return m_fc_port_num;
    }

    void 
    FcoeClientIntfMessage::setPortNum(const UI32 &port_num)
    {
        m_port_num = port_num;
    }

    UI32 
    FcoeClientIntfMessage::getPortNum () const
    {
        return m_port_num;
    }
    void 
    FcoeClientIntfMessage::setVnNum(const UI32 &vn_num)
    {
        m_vn_num = vn_num;
    }

    UI32 
    FcoeClientIntfMessage::getVnNum () const
    {
        return m_vn_num;
    }

    void
    FcoeClientIntfMessage::setBindingType(const UI32 &binding_type)
    {
        m_binding_type = binding_type;
    }

    UI32
    FcoeClientIntfMessage::getBindingType() const
    {
        return m_binding_type;
    }

    void
    FcoeClientIntfMessage::setConfigReplay(const UI32 &config_replay)
    {
        m_config_replay = config_replay;
    }

    UI32
    FcoeClientIntfMessage::getConfigReplay() const
    {
        return m_config_replay;
    }

    const void *
    FcoeClientIntfMessage::getCStructureForInputs()
    {
        fcoe_intf_config_msg_t  *msg_p = NULL;
        UI32                    size = 0;
        
        if (m_oper_bmp == FCOE_INTF_OPER_CONFIG_REPLAY) {
            msg_p = (fcoe_intf_config_msg_t *)transferBufferToUser(m_oper_bmp, size);
        } else {
            msg_p = new fcoe_intf_config_msg_t;

            msg_p->cmd_code = m_oper_bmp;
            msg_p->num_entries = 1;
            msg_p->config_replay = m_config_replay;
            msg_p->list[0].status = m_status;
            msg_p->list[0].fc_port_num = m_fc_port_num;
            msg_p->list[0].port_num = m_port_num;
            msg_p->list[0].vn_num = m_vn_num;
            msg_p->list[0].bind_type = m_binding_type;
            msg_p->list[0].if_index = m_ifIndex;

            strncpy((char *)msg_p->list[0].bind_name, m_binding.c_str(),
                    FCOE_PHY_INTF_NAME_LEN_MAX);

            strncpy((char *)msg_p->list[0].if_name, m_ifName.c_str(),
                    FCOE_PHY_INTF_NAME_LEN_MAX);
        }
        
        return msg_p;
    }

    void
    FcoeClientIntfMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        fcoe_intf_config_msg_resp_t *resp_p = (fcoe_intf_config_msg_resp_t *)pOutputCStructure;
        
        if (resp_p) {
            addBuffer(resp_p->cmd_code, resp_p->size,
                      (unsigned char *)resp_p, false);
        }
    }

}
