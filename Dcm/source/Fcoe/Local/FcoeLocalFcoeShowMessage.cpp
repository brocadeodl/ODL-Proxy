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
#include "Fcoe/Local/FcoeLocalFcoeShowMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeShowMessage::FcoeLocalFcoeShowMessage ()
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),
                                      FCOE_LOCAL_FCOE_SHOW)
    {
    }

    FcoeLocalFcoeShowMessage::FcoeLocalFcoeShowMessage (const UI32 &cmdCode)
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (), 
                        FCOE_LOCAL_FCOE_SHOW),
        m_cmdCode (cmdCode),
        m_vlan (0),
        m_vfid (0),
        m_fc_uport (FCOE_INTF_UPORT_NUM_INVALID),
        m_fab_map_name (""),
        m_map_name (""),
        m_phy_intf_name (""),
        m_locationId(0),
        m_stats_enable(true)
    {
    }

    FcoeLocalFcoeShowMessage::~FcoeLocalFcoeShowMessage ()
    {
    }

    void  FcoeLocalFcoeShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_fab_map_name, "Fab_map_name"));
        addSerializableAttribute (new AttributeString(&m_map_name, "map_name"));
        addSerializableAttribute (new AttributeString(&m_phy_intf_name, "phy_intf_name"));
        addSerializableAttribute (new AttributeBool(&m_stats_enable, "stats_enable"));
    }

    void  FcoeLocalFcoeShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeLocalFcoeShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32 
    FcoeLocalFcoeShowMessage::getVlan() const
    {
        return m_vlan;
    }
    
    void 
    FcoeLocalFcoeShowMessage::setVlan(const UI32 &vlan)
    {
        m_vlan = vlan;
    }
    
    UI32 
    FcoeLocalFcoeShowMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void 
    FcoeLocalFcoeShowMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }
    
    UI32 
    FcoeLocalFcoeShowMessage::getFcUport() const
    {
        return m_fc_uport;
    }
    
    void 
    FcoeLocalFcoeShowMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }
    
    string 
    FcoeLocalFcoeShowMessage::getFabMapName() const
    {
        return m_fab_map_name;
    }
    
    void 
    FcoeLocalFcoeShowMessage::setFabMapName(const string &fab_map_name)
    {
        m_fab_map_name = fab_map_name;
    }
    
    string 
    FcoeLocalFcoeShowMessage::getMapName() const
    {
        return m_map_name;
    }
    
    void 
    FcoeLocalFcoeShowMessage::setMapName(const string &map_name)
    {
        m_map_name = map_name;
    }
    
    string 
    FcoeLocalFcoeShowMessage::getPhyIfName() const
    {
        return m_phy_intf_name;
    }
    
    void 
    FcoeLocalFcoeShowMessage::setPhyIfName(const string &if_name)
    {
        m_phy_intf_name = if_name;
    }

    void FcoeLocalFcoeShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                                       UI32 &size, 
                                                       void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    UI32
    FcoeLocalFcoeShowMessage::getDestLocationId() const
    {
        return (m_locationId);
    }

    void
    FcoeLocalFcoeShowMessage::setDestLocationId(const UI32 &loc_id)
    {
        m_locationId = loc_id;
    }

    void
    FcoeLocalFcoeShowMessage::setStatsEnable(const bool &stats_enable)
    {
        m_stats_enable = stats_enable;
    }

    bool
    FcoeLocalFcoeShowMessage::getStatsEnable() const
    {
        return m_stats_enable;
    }

}
