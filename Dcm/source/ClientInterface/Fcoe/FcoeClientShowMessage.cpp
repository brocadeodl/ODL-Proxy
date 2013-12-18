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
 *   Author : Vikas B Dharwadkar                                           *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Fcoe/FcoeClientShowMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"


namespace DcmNs
{

    FcoeClientShowMessage::FcoeClientShowMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_SHOW)
    {
    }

    FcoeClientShowMessage::FcoeClientShowMessage (const UI32 &cmdCode,
                                                  const UI32 &vlan,
                                                  const UI32 &vfid,
                                                  const UI32 &fc_uport,
                                                  const string &fab_map_name,
                                                  const string &map_name,
                                                  const string &phy_intf_name,
                                                  const bool &stats_enable)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_SHOW),
        m_cmdCode (cmdCode),
        m_vlan (vlan),
        m_vfid (vfid),
        m_fc_uport (fc_uport),
        m_fab_map_name (fab_map_name),
        m_map_name (map_name),
        m_phy_intf_name (phy_intf_name),
        m_stats_enable (stats_enable)
    {
    }

    FcoeClientShowMessage::~FcoeClientShowMessage ()
    {
    }

    void  FcoeClientShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_fab_map_name, "Fab_map_name"));
        addSerializableAttribute (new AttributeString(&m_map_name, "map_name"));
        addSerializableAttribute (new AttributeString(&m_phy_intf_name, "phy_intf_name"));
        addSerializableAttribute (new AttributeBool(&m_stats_enable, "stats_enable"));
    }

    void  FcoeClientShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeClientShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void FcoeClientShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                                    UI32 &size, 
                                                    void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *FcoeClientShowMessage::getCStructureForInputs ()
    {
        fcoe_show_cmd_req_t *req_p = new fcoe_show_cmd_req_t;

        req_p->cmd_code = m_cmdCode;

        switch (m_cmdCode) {
            case FCOE_SHOW_CMD_CODE_FAB_MAP:
                strncpy((char *)req_p->req.fab_map_req.fab_map_name,
                        m_fab_map_name.c_str(),
                        FCOE_FAB_MAP_NAME_LEN_MAX);
                break;

            case FCOE_SHOW_CMD_CODE_MAP:
                strncpy((char *)req_p->req.map_req.map_name,
                        m_map_name.c_str(),
                        FCOE_MAP_NAME_LEN_MAX);
                break;

            case FCOE_SHOW_CMD_CODE_FIP:
                break;
                
            case FCOE_SHOW_CMD_CODE_LOGIN:
            case FCOE_SHOW_CMD_CODE_LOGIN_INTF:
            case FCOE_SHOW_CMD_CODE_LOGIN_VFID:
            case FCOE_SHOW_CMD_CODE_LOGIN_VLAN:
                req_p->req.login_req.fc_uport = m_fc_uport;
                req_p->req.login_req.vlan_id = m_vlan;
                req_p->req.login_req.vfid = m_vfid;
                break;
                
            case FCOE_SHOW_CMD_CODE_INTF:
            case FCOE_SHOW_CMD_CODE_INTF_BRIEF:
                req_p->req.intf_req.fc_uport = m_fc_uport; 
                req_p->req.intf_req.stats_enable = m_stats_enable;
                break;

            case FCOE_SHOW_CMD_CODE_PHY_INTF_FCOE:
                strncpy((char *)req_p->req.phy_intf_req.phy_intf_name,
                        m_phy_intf_name.c_str(),
                        FCOE_PHY_INTF_NAME_LEN_MAX);
                break;

            default:
                break;
        }

        return req_p;
    }

	void 
    FcoeClientShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        fcoe_show_cmd_resp_t *resp_p = (fcoe_show_cmd_resp_t *)pOutputCStructure;
        if (resp_p) {
            addBuffer(resp_p->cmd_code, resp_p->size,
                      (unsigned char *)resp_p, false);
        }
	}

    UI32 
    FcoeClientShowMessage::getVlan() const
    {
        return m_vlan;
    }
    
    void 
    FcoeClientShowMessage::setVlan(const UI32 &vlan)
    {
        m_vlan = vlan;
    }
    
    UI32 
    FcoeClientShowMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void 
    FcoeClientShowMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }
    
    UI32 
    FcoeClientShowMessage::getFcUport() const
    {
        return m_fc_uport;
    }
    
    void 
    FcoeClientShowMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }
    
    string 
    FcoeClientShowMessage::getFabMapName() const
    {
        return m_fab_map_name;
    }
    
    void 
    FcoeClientShowMessage::setFabMapName(const string &fab_map_name)
    {
        m_fab_map_name = fab_map_name;
    }
    
    string 
    FcoeClientShowMessage::getMapName() const
    {
        return m_map_name;
    }
    
    void 
    FcoeClientShowMessage::setMapName(const string &map_name)
    {
        m_map_name = map_name;
    }
    
    string 
    FcoeClientShowMessage::getPhyIfName() const
    {
        return m_phy_intf_name;
    }
    
    void 
    FcoeClientShowMessage::setPhyIfName(const string &if_name)
    {
        m_phy_intf_name = if_name;
    }

    void
    FcoeClientShowMessage::setStatsEnable(const bool &stats_enable)
    {
        m_stats_enable = stats_enable;
    }
    
    bool
    FcoeClientShowMessage::getStatsEnable() const
    {
        return m_stats_enable;
    }

}
