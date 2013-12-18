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

#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalFcoeFabricMapMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeFabricMapMessage::FcoeLocalFcoeFabricMapMessage ()
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_FABRIC_MAP)
    {
    }

    FcoeLocalFcoeFabricMapMessage::FcoeLocalFcoeFabricMapMessage (const UI32 &oper_bmp,const string &fabric_map_name,const UI32 &vlan,const UI32 &vfid,const UI32 &priority,const string &fcmap,const UI32 &fka_intvl,const bool &timeout,const UI32 &mtu, const UI32 &max_enodes, const string &enodes_cfg_mode)
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_FABRIC_MAP),
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
        m_enodes_cfg_mode (enodes_cfg_mode)
    {
    }

    FcoeLocalFcoeFabricMapMessage::~FcoeLocalFcoeFabricMapMessage ()
    {
    }

    void  FcoeLocalFcoeFabricMapMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addSerializableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid,"vfid"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeString(&m_fcmap,"fcmap"));
        addSerializableAttribute (new AttributeUI32(&m_fka_intvl,"fka_intvl"));
        addSerializableAttribute (new AttributeBool(&m_timeout,"timeout"));
        addSerializableAttribute (new AttributeUI32(&m_mtu,"mtu"));
        addSerializableAttribute (new AttributeUI32(&m_max_enodes, "max-enodes"));
        addSerializableAttribute (new AttributeString(&m_enodes_cfg_mode, "enodes_cfg_mode"));
    }

    void  FcoeLocalFcoeFabricMapMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeLocalFcoeFabricMapMessage::setFabric_map_name(const string &fabric_map_name)
    {
        m_fabric_map_name  =  fabric_map_name;
    }

    string  FcoeLocalFcoeFabricMapMessage::getFabric_map_name() const
    {
        return (m_fabric_map_name);
    }

    void  FcoeLocalFcoeFabricMapMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  FcoeLocalFcoeFabricMapMessage::setVfid(const UI32 &vfid)
    {
        m_vfid  =  vfid;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getVfid() const
    {
        return (m_vfid);
    }

    void  FcoeLocalFcoeFabricMapMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getPriority() const
    {
        return (m_priority);
    }

    void  FcoeLocalFcoeFabricMapMessage::setFcmap(const string &fcmap)
    {
        m_fcmap  =  fcmap;
    }

    string  FcoeLocalFcoeFabricMapMessage::getFcmap() const
    {
        return (m_fcmap);
    }

    void  FcoeLocalFcoeFabricMapMessage::setFka_intvl(const UI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getFka_intvl() const
    {
        return (m_fka_intvl);
    }

    void  FcoeLocalFcoeFabricMapMessage::setTimeout(const bool &timeout)
    {
        m_timeout  =  timeout;
    }

    bool  FcoeLocalFcoeFabricMapMessage::getTimeout() const
    {
        return (m_timeout);
    }

    void  FcoeLocalFcoeFabricMapMessage::setMtu(const UI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getMtu() const
    {
        return (m_mtu);
    }

    void  FcoeLocalFcoeFabricMapMessage::setMaxEnodes(const UI32 &max_enodes)
    {
        m_max_enodes  =  max_enodes;
    }

    UI32  FcoeLocalFcoeFabricMapMessage::getMaxEnodes() const
    {
        return (m_max_enodes);
    }

    void  FcoeLocalFcoeFabricMapMessage::setEnodesCfgMode(const string &enodes_cfg_mode)
    {   
        m_enodes_cfg_mode  =  enodes_cfg_mode;
    }

    string  FcoeLocalFcoeFabricMapMessage::getEnodesCfgMode() const
    {   
        return (m_enodes_cfg_mode);
    }
}
