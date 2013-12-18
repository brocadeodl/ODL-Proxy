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

#include "ClientInterface/Fcoe/FcoeClientETIntfMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientETIntfMessage::FcoeClientETIntfMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_ET_INTF_CONFIG)
    {
    }

    FcoeClientETIntfMessage::~FcoeClientETIntfMessage ()
    {
    }

    void  FcoeClientETIntfMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeBool(&m_isl_status,"isl_status"));
        addSerializableAttribute (new AttributeBool(&m_trunk_status,"trunk_status"));
        addSerializableAttribute (new AttributeUI32(&m_fc_port_num,"fc_port_num"));
    }

    void  FcoeClientETIntfMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientETIntfMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeClientETIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  FcoeClientETIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  FcoeClientETIntfMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  FcoeClientETIntfMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  FcoeClientETIntfMessage::setISLStatus(const bool &status)
    {
        m_isl_status  =  status;
    }

    bool FcoeClientETIntfMessage::getISLStatus() const
    {
        return (m_isl_status);
    }

    void  FcoeClientETIntfMessage::setTrunkStatus(const bool &status)
    {
        m_trunk_status  =  status;
    }

    bool FcoeClientETIntfMessage::getTrunkStatus() const
    {
        return (m_trunk_status);
    }
   

    void FcoeClientETIntfMessage::setFcPortNum(const UI32 &uport_num)
    {
        m_fc_port_num = uport_num;
    }

    UI32 FcoeClientETIntfMessage::getFcPortNum () const
    {
        return m_fc_port_num;
    }
   
    const void * FcoeClientETIntfMessage::getCStructureForInputs()
    {
        fcoe_et_intf_config_msg_t   *msg_p = NULL;
        UI32                        size = 0;
        
        if ((m_oper_bmp == FCOE_ET_INTF_OPER_CONFIG_REPLAY) ||
            (m_oper_bmp == FCOE_ET_INTF_OPER_SLOT_FAILOVER)) {
            msg_p = (fcoe_et_intf_config_msg_t *)transferBufferToUser(m_oper_bmp, size);
        } else {
            msg_p = new fcoe_et_intf_config_msg_t;

            msg_p->cmd_code = m_oper_bmp;
            msg_p->num_entries = 1;
            msg_p->list[0].status = m_isl_status;
            msg_p->list[0].if_index = m_ifIndex;
            msg_p->list[0].trunk_status = m_trunk_status;
            msg_p->list[0].fc_port_num = m_fc_port_num;
            strncpy((char *)msg_p->list[0].if_name, m_ifName.c_str(),
                    FCOE_PHY_INTF_NAME_LEN_MAX);
        }

        return msg_p;
    }

    void
    FcoeClientETIntfMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
        fcoe_intf_config_msg_resp_t *resp_p = (fcoe_intf_config_msg_resp_t *)pOutputCStructure;
        
        if (resp_p) {
            addBuffer(resp_p->cmd_code, resp_p->size,
                      (unsigned char *)resp_p, false);
        }
    }
}
