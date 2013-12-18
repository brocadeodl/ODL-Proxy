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

#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalFcoeShowMessage.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

namespace DcmNs
{

    FcoeGlobalFcoeShowMessage::FcoeGlobalFcoeShowMessage ()
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),
                                      FCOE_GLOBAL_FCOE_SHOW)
    {
    }

    FcoeGlobalFcoeShowMessage::FcoeGlobalFcoeShowMessage (const UI32 &cmdCode)
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (), 
                                      FCOE_GLOBAL_FCOE_SHOW),
        m_cmdCode (cmdCode)
    {
    }

    FcoeGlobalFcoeShowMessage::~FcoeGlobalFcoeShowMessage ()
    {
    }

    void  FcoeGlobalFcoeShowMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_fab_map_name, "Fab_map_name"));
        addSerializableAttribute (new AttributeString(&m_map_name, "map_name"));
        addSerializableAttribute (new AttributeString(&m_map_name, "phy_intf_name"));
        addSerializableAttribute (new AttributeBool(&m_stats_enable, "stats_enable"));
    }

    void  FcoeGlobalFcoeShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeGlobalFcoeShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32 
    FcoeGlobalFcoeShowMessage::getVlan() const
    {
        return m_vlan;
    }
    
    void 
    FcoeGlobalFcoeShowMessage::setVlan(const UI32 &vlan)
    {
        m_vlan = vlan;
    }
    
    UI32 
    FcoeGlobalFcoeShowMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void 
    FcoeGlobalFcoeShowMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }
    
    UI32 
    FcoeGlobalFcoeShowMessage::getFcUport() const
    {
        return m_fc_uport;
    }
    
    void 
    FcoeGlobalFcoeShowMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }
    
    string 
    FcoeGlobalFcoeShowMessage::getFabMapName() const
    {
        return m_fab_map_name;
    }
    
    void 
    FcoeGlobalFcoeShowMessage::setFabMapName(const string &fab_map_name)
    {
        m_fab_map_name = fab_map_name;
    }
    
    string 
    FcoeGlobalFcoeShowMessage::getMapName() const
    {
        return m_map_name;
    }
    
    void 
    FcoeGlobalFcoeShowMessage::setMapName(const string &map_name)
    {
        m_map_name = map_name;
    }
    
    string 
    FcoeGlobalFcoeShowMessage::getPhyIfName() const
    {
        return m_phy_intf_name;
    }
    
    void 
    FcoeGlobalFcoeShowMessage::setPhyIfName(const string &if_name)
    {
        m_phy_intf_name = if_name;
    }

    void
    FcoeGlobalFcoeShowMessage::setStatsEnable(const bool &stats_enable)
    {
        m_stats_enable = stats_enable;
    }

    bool
    FcoeGlobalFcoeShowMessage::getStatsEnable() const
    {
        return m_stats_enable;
    }

    void FcoeGlobalFcoeShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                                       UI32 &size, 
                                                       void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
}
