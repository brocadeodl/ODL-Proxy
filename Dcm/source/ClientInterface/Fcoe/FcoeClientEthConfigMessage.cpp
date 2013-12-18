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

#include "ClientInterface/Fcoe/FcoeClientEthConfigMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    FcoeClientEthConfigMessage::FcoeClientEthConfigMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_ETH_INTF_CONFIG)
    {
    }

    FcoeClientEthConfigMessage::~FcoeClientEthConfigMessage ()
    {
    }

    void  FcoeClientEthConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_map_name,"map_name"));
        addSerializableAttribute (new AttributeString(&m_cee_map_name, "cee_map_name"));
        addSerializableAttribute (new AttributeUI32Vector(&m_fcoe_vlan_vec, "fcoe_vlan_vec"));
        addSerializableAttribute (new AttributeString(&m_if_id, "if_id"));
        addSerializableAttribute (new AttributeUI64(&m_ifindex, "if_index"));
        addSerializableAttribute (new AttributeUI64Vector(&m_mbr_ifindex_list, "mbr_ifindex_list"));
    }

    void  FcoeClientEthConfigMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientEthConfigMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeClientEthConfigMessage::setMap_name(const string &map_name)
    {
        m_map_name = map_name;
    }

    void FcoeClientEthConfigMessage::setIfId(const string &if_id)
    {
        m_if_id = if_id;
    }

    string FcoeClientEthConfigMessage::getIfId() const
    {
        return m_if_id;
    }

    string  FcoeClientEthConfigMessage::getMap_name() const
    {
        return (m_map_name);
    }

    void  FcoeClientEthConfigMessage::setCeeMapName(const string &cee_map_name)
    {
        m_cee_map_name = cee_map_name;
    }

    string FcoeClientEthConfigMessage::getCeeMapName() const
    {
        return (m_cee_map_name);
    }

    void FcoeClientEthConfigMessage::addFcoeVlan (const UI32 &vlan_id)
    {
        m_fcoe_vlan_vec.push_back(vlan_id);
    }

    void FcoeClientEthConfigMessage::addMbrIfindex (const UI64 &ifindex)
    {
        m_mbr_ifindex_list.push_back(ifindex);
    }

    vector<UI32> FcoeClientEthConfigMessage::getFcoeVlanVec() const
    {
        return m_fcoe_vlan_vec;
    }

    void FcoeClientEthConfigMessage::setFcoeVlanVec(const vector<UI32> &vlan_vec)
    {
        m_fcoe_vlan_vec = vlan_vec;
    }

    void FcoeClientEthConfigMessage::setFcoeMbrIfindexVec(const vector<UI64> &ifindex_vec)
    {
        UI32 i = 0;
        for (i = 0; i < ifindex_vec.size(); i++) {
            m_mbr_ifindex_list.push_back(ifindex_vec.at(i));
        }
    }

    void FcoeClientEthConfigMessage::setIfindex(const UI64 &ifindex)
    {
        m_ifindex = ifindex;
    }

    UI64 FcoeClientEthConfigMessage::getIfindex() const
    {
        return m_ifindex;
    }

    const void *FcoeClientEthConfigMessage::getCStructureForInputs ()
    {
        fcoe_eth_intf_config_msg_t        *msg_p;
        UI32                        i = 0;

        msg_p = new fcoe_eth_intf_config_msg_t;

        msg_p->cmd_code = m_oper_bmp;
        msg_p->if_index = m_ifindex;
        strncpy((char *)msg_p->map_name, m_map_name.c_str(),
                FCOE_MAP_NAME_LEN_MAX);
        strncpy((char *)msg_p->cee_map_name, m_cee_map_name.c_str(),
                FCOE_CEE_MAP_NAME_LEN_MAX);
        strncpy((char *)msg_p->if_name, m_if_id.c_str(),
                FCOE_PHY_INTF_NAME_LEN_MAX);

        for (i = 0; i < m_fcoe_vlan_vec.size(); i++) {
            msg_p->fcoe_vlan_list[i] = m_fcoe_vlan_vec.at(i);
        }
        msg_p->num_vlans = m_fcoe_vlan_vec.size();
        
        for (i = 0; i < m_mbr_ifindex_list.size(); i++) {
            msg_p->mbr_if_index_list[i] = m_mbr_ifindex_list.at(i);
        }
        msg_p->num_entries = m_mbr_ifindex_list.size();

        return msg_p;
    }

	void FcoeClientEthConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
