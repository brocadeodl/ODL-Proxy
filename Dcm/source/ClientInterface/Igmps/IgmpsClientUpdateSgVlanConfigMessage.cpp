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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "ClientInterface/Igmps/IgmpsClientUpdateSgVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsClientUpdateSgVlanConfigMessage::IgmpsClientUpdateSgVlanConfigMessage ()
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATESGVLANCONFIG),
        m_interface     (0),
        m_ifType        (0),
        m_vlanId        (0),
        m_vlanIfIndex   (0),
        m_negation      (false)
    {
    }

    IgmpsClientUpdateSgVlanConfigMessage::IgmpsClientUpdateSgVlanConfigMessage (const IpV4Address &groupAddress,const UI32 &interface,const UI32 &ifType,const string &ifName,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const bool &negation)
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATESGVLANCONFIG),
        m_groupAddress    (groupAddress),
        m_interface    (interface),
        m_ifType    (ifType),
        m_ifName    (ifName),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_negation    (negation)
    {
    }

    IgmpsClientUpdateSgVlanConfigMessage::~IgmpsClientUpdateSgVlanConfigMessage ()
    {
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
        addSerializableAttribute (new AttributeUI32(&m_interface,"interface"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeString(&m_vlanIfName,"vlanIfName"));
        addSerializableAttribute (new AttributeUI64(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setGroupAddress(const IpV4Address &groupAddress)
    {
        m_groupAddress  =  groupAddress;
    }

    IpV4Address  IgmpsClientUpdateSgVlanConfigMessage::getGroupAddress() const
    {
        return (m_groupAddress);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setInterface(const UI32 &interface)
    {
        m_interface  =  interface;
    }

    UI32  IgmpsClientUpdateSgVlanConfigMessage::getInterface() const
    {
        return (m_interface);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  IgmpsClientUpdateSgVlanConfigMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsClientUpdateSgVlanConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  IgmpsClientUpdateSgVlanConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  IgmpsClientUpdateSgVlanConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  IgmpsClientUpdateSgVlanConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  IgmpsClientUpdateSgVlanConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  IgmpsClientUpdateSgVlanConfigMessage::getNegation() const
    {
        return (m_negation);
    }

	const void *IgmpsClientUpdateSgVlanConfigMessage::getCStructureForInputs ()
    {
        string str_ipaddr;
        igmps_sg_vlan_config_msg_t *pInput = (igmps_sg_vlan_config_msg_t *)malloc (sizeof (struct igmps_sg_vlan_config_msg_));

        str_ipaddr = m_groupAddress.toString();
        strncpy (pInput->group, str_ipaddr.c_str (), IGMPS_MAX_STR);
        pInput->group[IGMPS_MAX_STR - 1] = '\0';

        if (IGMPS_IF_TYPE_FORTYGIGABIT == m_ifType) {
            strncpy (pInput->if_type, "fo", IGMPS_MAX_STR);
        } else if (IGMPS_IF_TYPE_TENGIGABIT == m_ifType) {
            strncpy (pInput->if_type, "te", IGMPS_MAX_STR);
        } else if (IGMPS_IF_TYPE_GIGABIT == m_ifType) {
            strncpy (pInput->if_type, "gi", IGMPS_MAX_STR);
        } else if (IGMPS_IF_TYPE_PORTCHANNEL == m_ifType) {
            strncpy (pInput->if_type, "po", IGMPS_MAX_STR);
        }

		pInput->vlan_id         = m_vlanId;
		pInput->vlan_if_index   = m_vlanIfIndex;
		pInput->negation        = m_negation;
        strncpy (pInput->if_name, m_ifName.c_str(), IGMPS_MAX_STR);
        strncpy (pInput->vlan_if_name, m_vlanIfName.c_str(), IGMPS_MAX_STR);

		return (pInput);
	}

	void IgmpsClientUpdateSgVlanConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
