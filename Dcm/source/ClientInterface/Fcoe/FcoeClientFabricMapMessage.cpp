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

#include "ClientInterface/Fcoe/FcoeClientFabricMapMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientFabricMapMessage::FcoeClientFabricMapMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_FABRIC_MAP_CONFIG)
    {
        m_config_replay = 0;
    }

    FcoeClientFabricMapMessage::FcoeClientFabricMapMessage (const UI32 &oper_bmp,const string &fabric_map_name,const UI32 &vlan,const UI32 &vfid,const UI32 &priority,const string &fcmap,const UI32 &fka_intvl,const bool &timeout,const UI32 &mtu, const UI32 &max_enodes, const UI32 config_replay)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_FABRIC_MAP_CONFIG),
        m_oper_bmp    (oper_bmp),
        m_fabric_map_name    (fabric_map_name),
        m_vlan    (vlan),
        m_vfid    (vfid),
        m_priority    (priority),
        m_fcmap    (fcmap),
        m_fka_intvl    (fka_intvl),
        m_timeout    (timeout),
        m_mtu      (mtu),
        m_max_enodes (max_enodes),
        m_config_replay (config_replay)
    {
    }

    FcoeClientFabricMapMessage::~FcoeClientFabricMapMessage ()
    {
    }

    void  FcoeClientFabricMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid,"vfid"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeString(&m_fcmap,"fcmap"));
        addSerializableAttribute (new AttributeUI32(&m_fka_intvl,"fka_intvl"));
        addSerializableAttribute (new AttributeBool(&m_timeout,"timeout"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeUI32(&m_max_enodes, "max_enodes"));
        addSerializableAttribute (new AttributeUI32(&m_config_replay, "config_replay"));
    }

    void  FcoeClientFabricMapMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientFabricMapMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeClientFabricMapMessage::setFabric_map_name(const string &fabric_map_name)
    {
        m_fabric_map_name  =  fabric_map_name;
    }

    string  FcoeClientFabricMapMessage::getFabric_map_name() const
    {
        return (m_fabric_map_name);
    }

    void  FcoeClientFabricMapMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  FcoeClientFabricMapMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  FcoeClientFabricMapMessage::setVfid(const UI32 &vfid)
    {
        m_vfid  =  vfid;
    }

    UI32  FcoeClientFabricMapMessage::getVfid() const
    {
        return (m_vfid);
    }

    void  FcoeClientFabricMapMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  FcoeClientFabricMapMessage::getPriority() const
    {
        return (m_priority);
    }

    void  FcoeClientFabricMapMessage::setFcmap(const string &fcmap)
    {
        m_fcmap  =  fcmap;
    }

    string  FcoeClientFabricMapMessage::getFcmap() const
    {
        return (m_fcmap);
    }

    void  FcoeClientFabricMapMessage::setFka_intvl(const UI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    UI32  FcoeClientFabricMapMessage::getFka_intvl() const
    {
        return (m_fka_intvl);
    }

    void  FcoeClientFabricMapMessage::setTimeout(const bool &timeout)
    {
        m_timeout  =  timeout;
    }

    bool  FcoeClientFabricMapMessage::getTimeout() const
    {
        return (m_timeout);
    }

    void  FcoeClientFabricMapMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  FcoeClientFabricMapMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  FcoeClientFabricMapMessage::setMaxEnodes(const UI32 &max_enodes)
    {
        m_max_enodes  =  max_enodes;
    }

    UI32  FcoeClientFabricMapMessage::getMaxEnodes() const
    {
        return (m_max_enodes);
    }

    void FcoeClientFabricMapMessage::setConfigReplay()
    {
        m_config_replay = 1;
    }

    UI32 FcoeClientFabricMapMessage::getConfigReplay()const
    {
        return m_config_replay;
    }

    const void * FcoeClientFabricMapMessage::getCStructureForInputs()
    {
        fcoe_fabric_map_msg_t   *msg_p = new fcoe_fabric_map_msg_t;
        char fcmap_str[16];
        const char *delims = ":";
        char *result = NULL;
        uint32_t fcmap = 0;
        
        msg_p->oper_bmp = m_oper_bmp;
        strncpy((char*)fcmap_str, m_fcmap.c_str(), 16);
        result = strtok(fcmap_str, delims);
        while (result != NULL) {
            fcmap = fcmap << 8;
            fcmap |= strtol(result, NULL, 16);
            result = strtok(NULL, delims);
        }
        msg_p->fcmap = fcmap;

        msg_p->config_replay = m_config_replay;

        strncpy((char *)msg_p->fab_map_name, m_fabric_map_name.c_str(),
                FCOE_FAB_MAP_NAME_LEN_MAX);
        msg_p->vlan_id = m_vlan;
        msg_p->vfid = m_vfid;
        msg_p->priority = m_priority;
        msg_p->fka_intvl = m_fka_intvl;
        msg_p->timeout = m_timeout;
        msg_p->mtu = m_mtu;
        msg_p->max_enodes = m_max_enodes;

        return msg_p;
    }

    void  
    FcoeClientFabricMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure) 
    {
        fcoe_fabric_map_msg_resp_t   *resp_p = (fcoe_fabric_map_msg_resp_t *)(pOutputCStructure);
        if (resp_p) {
            addBuffer(resp_p->cmd_code, resp_p->size,
                      (unsigned char *)resp_p, false);
        }

        return ;
    }
}
