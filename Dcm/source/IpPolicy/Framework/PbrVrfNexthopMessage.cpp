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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   Author : debojyoti													   *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/PbrVrfNexthopMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    PbrVrfNexthopMessage::PbrVrfNexthopMessage ()
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYPBRVRF)
    {
        m_name_flag = false;
        m_instance_flag = false;
        m_action_flag = false;
        m_vrfFlag = false;
        m_set_next_hop_address_flag = false;
		m_vrfName = "";
		m_global_flag = false;
    }


    PbrVrfNexthopMessage::~PbrVrfNexthopMessage ()
    {
    }

    void  PbrVrfNexthopMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)&m_op_code,"op_code"));
        addSerializableAttribute (new AttributeString(&m_name,"name"));
        addSerializableAttribute (new AttributeUI16(&m_instance,"instance"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_action,"action"));

        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeIpV4Address(&m_set_next_hop_address,"next_hop_address"));

        addSerializableAttribute (new AttributeBool(&m_vrfFlag,"vrf_flag"));
        addSerializableAttribute (new AttributeBool(&m_set_next_hop_address_flag,"next_hop_address_flag"));
		addSerializableAttribute (new AttributeBool(&m_global_flag,"global_flag"));
    }

    void  PbrVrfNexthopMessage::setName(const string &name)
    {
        m_name = name;
        m_name_flag = true;
    }

    string  PbrVrfNexthopMessage::getName() const
    {
        return (m_name);
    }

    bool  PbrVrfNexthopMessage::getNameFlag() const
    {
        return (m_name_flag);
    }

    void  PbrVrfNexthopMessage::setInstance(const UI16 &instance)
    {
        m_instance = instance;
        m_instance_flag = true;
    }

    UI16  PbrVrfNexthopMessage::getInstance() const
    {
        return (m_instance);
    }

    bool  PbrVrfNexthopMessage::getInstanceFlag() const
    {
        return (m_instance_flag);
    }

    void  PbrVrfNexthopMessage::setAction(const PolicyAction &action)
    {
        m_action = action;
        m_action_flag = true;
    }

    PolicyAction  PbrVrfNexthopMessage::getAction() const
    {
        return (m_action);
    }

    bool  PbrVrfNexthopMessage::getActionFlag() const
    {
        return (m_action_flag);
    }

    void PbrVrfNexthopMessage::setVrf (const string &vrfName)
    {
        m_vrfName = vrfName;
        m_vrfFlag = true;
    }

    string PbrVrfNexthopMessage::getVrf() const
    {
        return (m_vrfName);
    }

    bool PbrVrfNexthopMessage::getVrfFlag() const
    {
        return (m_vrfFlag);
    }


    void  PbrVrfNexthopMessage::setSet_next_hop_address(const IpV4Address &set_next_hop_address)
    {
        m_set_next_hop_address = set_next_hop_address;
        m_set_next_hop_address_flag = true;
    }

    IpV4Address  PbrVrfNexthopMessage::getSet_next_hop_address() const
    {
        return (m_set_next_hop_address);
    }

    bool  PbrVrfNexthopMessage::getSet_next_hop_addressFlag() const
    {
        return (m_set_next_hop_address_flag);
    }

	bool PbrVrfNexthopMessage::getGlobalFlag() const
	{
	  	return (m_global_flag);
	}

	void PbrVrfNexthopMessage::setGlobalFlag(bool globalFlag)
	{
	  	m_global_flag = globalFlag;
	}

}

