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

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmFcoeportAttrMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    NsmFcoeportAttrMessage::NsmFcoeportAttrMessage ()
        : DcmManagementInterfaceMessage (NSMFCOEPORT)
    {
    }

    NsmFcoeportAttrMessage::NsmFcoeportAttrMessage (const UI32 &oper_bmp,const string &map_name, const string &cee_map_name, const string &if_id)
        : DcmManagementInterfaceMessage (NSMFCOEPORT),
        m_oper_bmp    (oper_bmp),
        m_map_name    (map_name),
        m_cee_map_name(cee_map_name),
        m_if_id (if_id)
    {
    }

    NsmFcoeportAttrMessage::~NsmFcoeportAttrMessage ()
    {
    }

    void  NsmFcoeportAttrMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_map_name,"map_name"));
        addSerializableAttribute (new AttributeString(&m_cee_map_name, "cee_map_name"));
        addSerializableAttribute (new AttributeUI32Vector(&m_fcoe_vlan_vec, "fcoe_vlan_vec"));
        addSerializableAttribute (new AttributeString(&m_if_id, "if_id"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "ifindex"));
        addSerializableAttribute (new AttributeUI32(&m_phy_type, "phyType"));
        
    }

    void  NsmFcoeportAttrMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  NsmFcoeportAttrMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  NsmFcoeportAttrMessage::setMap_name(const string &map_name)
    {
        m_map_name = map_name;
    }

    void NsmFcoeportAttrMessage::setIfId(const string &if_id)
    {
        m_if_id = if_id;
    }

    string NsmFcoeportAttrMessage::getIfId() const
    {
        return m_if_id;
    }

    string  NsmFcoeportAttrMessage::getMap_name() const
    {
        return (m_map_name);
    }

    void  NsmFcoeportAttrMessage::setCeeMapName(const string &cee_map_name)
    {
        m_cee_map_name = cee_map_name;
    }

    string NsmFcoeportAttrMessage::getCeeMapName() const
    {
        return (m_cee_map_name);
    }

    void NsmFcoeportAttrMessage::addFcoeVlan (const UI32 &vlan_id)
    {
        m_fcoe_vlan_vec.push_back(vlan_id);
    }

    vector<UI32> NsmFcoeportAttrMessage::getFcoeVlanVec() const
    {
        return m_fcoe_vlan_vec;
    }

    void NsmFcoeportAttrMessage::setIfindex (const UI64 &ifindex)
    {
        m_if_index = ifindex;
    }

    UI64 NsmFcoeportAttrMessage::getIfindex () const
    {
        return m_if_index;
    }

    void NsmFcoeportAttrMessage::setPhyType (UI32 type)
    {
        m_phy_type = type;
    }

    UI32 NsmFcoeportAttrMessage::getPhyType () const
    {
        return m_phy_type;
    }

    const void *NsmFcoeportAttrMessage::getCStructureForInputs ()
    {
        NsmFcoeportMsg_t            *msg_p;
        vector<UI32>::iterator      vlan_itr;
        UI32                        i = 0;
		string twoTupleName = "";

        msg_p = new NsmFcoeportMsg_t;
		
		if (NsmUtils::isValidThreeTuple(m_if_id)) {
            NsmUtils::getTwoTupleIfName(m_if_id, twoTupleName);
        } else {
            twoTupleName = m_if_id;
        }
        msg_p->cmd_code = m_oper_bmp;
        strncpy((char *)msg_p->map_name, m_map_name.c_str(), MAX_STR);
        strncpy((char *)msg_p->cee_map_name, m_cee_map_name.c_str(), MAX_STR);
        strncpy((char *)msg_p->if_name, twoTupleName.c_str(), MAX_STR);
        msg_p->ifindex = m_if_index;
		msg_p->ifType = m_phy_type;
        for (i = 0, vlan_itr = m_fcoe_vlan_vec.begin();
             vlan_itr < m_fcoe_vlan_vec.end();
             vlan_itr++) {
            msg_p->fcoe_vlan_list[i] = *vlan_itr;
        }
        msg_p->num_vlans = m_fcoe_vlan_vec.size();
        
        return msg_p;
    }

	void NsmFcoeportAttrMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
