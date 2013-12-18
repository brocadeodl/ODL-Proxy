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

#include "Fcoe/Global/FcoeGlobalFcoeFabricMapMessage.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"
#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeGlobalFcoeFabricMapMessage::FcoeGlobalFcoeFabricMapMessage ()
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),FCOE_GLOBAL_FCOE_FABRIC_MAP)
    {
    }

    FcoeGlobalFcoeFabricMapMessage::FcoeGlobalFcoeFabricMapMessage (const string &name, 
                                                                          const uint32_t &oper_bmp, 
                                                                          const uint32_t &vlan, 
                                                                          const uint32_t &vfid, 
                                                                          const uint32_t &priority, 
                                                                          const string &fcmap, 
                                                                          const uint32_t &adv_intvl, 
                                                                          const uint32_t &timeout,
                                                                          const uint32_t &mtu,
                                                                          const uint32_t &max_enodes,
                                                                          const string &enodes_cfg_mode)
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),FCOE_GLOBAL_FCOE_FABRIC_MAP),
        m_fabric_map_name (name),
        m_oper_bmp (oper_bmp),
        m_vlan (vlan),
        m_vfid (vfid),
        m_priority (priority),
        m_fcmap (fcmap),
        m_adv_intvl (adv_intvl),
        m_timeout (timeout),
        m_mtu (mtu),
        m_max_enodes (max_enodes),
        m_enodes_cfg_mode (enodes_cfg_mode)
    {
    }

    FcoeGlobalFcoeFabricMapMessage::~FcoeGlobalFcoeFabricMapMessage ()
    {
    }

    void  FcoeGlobalFcoeFabricMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp, "oper_bmp"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "vfid"));
        addSerializableAttribute (new AttributeUI32(&m_priority, "priority"));
        addSerializableAttribute (new AttributeString(&m_fcmap, "fcmap"));
        addSerializableAttribute (new AttributeUI32(&m_adv_intvl, "adv_intvl"));
        addSerializableAttribute (new AttributeBool(&m_timeout, "timeout"));
        addSerializableAttribute (new AttributeUI32(&m_mtu, "mtu"));
        addSerializableAttribute (new AttributeUI32(&m_max_enodes, "max_enodes"));
        addSerializableAttribute (new AttributeString(&m_enodes_cfg_mode, "enodes_cfg_mode"));
    }

    void  FcoeGlobalFcoeFabricMapMessage::setName(const string &name)
    {
        m_fabric_map_name  =  name;
    }

    string  FcoeGlobalFcoeFabricMapMessage::getName() const
    {
        return (m_fabric_map_name);
    }

    void FcoeGlobalFcoeFabricMapMessage::setOperBmp(const uint32_t &oper_bmp)
    {
        m_oper_bmp = oper_bmp;
    }
    
    uint32_t FcoeGlobalFcoeFabricMapMessage::getOperBmp() const
    {
        return (m_oper_bmp);
    }
    
    void FcoeGlobalFcoeFabricMapMessage::setVlan(const uint32_t &vlan)
    {
        m_vlan = vlan;
    }
    
    uint32_t FcoeGlobalFcoeFabricMapMessage::getVlan() const
    {
        return (m_vlan);
    }
    
    void FcoeGlobalFcoeFabricMapMessage::setVfid(const uint32_t &vfid)
    {
        m_vfid = vfid;
    }
    
    uint32_t FcoeGlobalFcoeFabricMapMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void FcoeGlobalFcoeFabricMapMessage::setPriority(const uint32_t &priority)
    {
        m_priority = priority;
    }
   
    uint32_t FcoeGlobalFcoeFabricMapMessage::getPriority() const
    {
        return m_priority;
    }
    
    void FcoeGlobalFcoeFabricMapMessage::setFcmap(const string &fcmap)
    {
        m_fcmap = fcmap;
    }
    
    string FcoeGlobalFcoeFabricMapMessage::getFcmap() const
    {
        return m_fcmap;
    }
    
    void FcoeGlobalFcoeFabricMapMessage::setAdvIntvl(const uint32_t &adv_intvl)
    {
        m_adv_intvl = adv_intvl;
    }
    
    uint32_t FcoeGlobalFcoeFabricMapMessage::getAdvIntvl() const
    {
        return m_adv_intvl;
    }
    
    void FcoeGlobalFcoeFabricMapMessage::setTimeout(const bool &timeout)
    {
        m_timeout = timeout;
    }
    
    bool FcoeGlobalFcoeFabricMapMessage::getTimeout() const
    {
        return m_timeout;
    }

    void FcoeGlobalFcoeFabricMapMessage::setMtu(const uint32_t &mtu)
    {
        m_mtu = mtu;
    }

    uint32_t FcoeGlobalFcoeFabricMapMessage::getMtu() const
    {
        return m_mtu;
    }

    void FcoeGlobalFcoeFabricMapMessage::setMaxEnodes(const uint32_t &max_enodes)
    {
        m_max_enodes = max_enodes;
    }

    uint32_t FcoeGlobalFcoeFabricMapMessage::getMaxEnodes() const
    {
        return m_max_enodes;
    }

    void FcoeGlobalFcoeFabricMapMessage::setEnodesCfgMode(const string &enodes_cfg_mode)
    {
        m_enodes_cfg_mode = enodes_cfg_mode;
    }   

    string FcoeGlobalFcoeFabricMapMessage::getEnodesCfgMode() const
    {
        return m_enodes_cfg_mode;
    }   

    uint8_t*
    FcoeGlobalFcoeFabricMapMessage::getFCFMacForLocation(const UI32 &loc_id)
    {
        fcoe_fabric_map_msg_resp_t  *resp_p = NULL;
        UI32                        size = 0;

        resp_p = (fcoe_fabric_map_msg_resp_t *) findBuffer(loc_id, size);
        if (resp_p == NULL) {
            return NULL;
        }

        return resp_p->fcf_mac_addr_list[0];
    }
        
}
