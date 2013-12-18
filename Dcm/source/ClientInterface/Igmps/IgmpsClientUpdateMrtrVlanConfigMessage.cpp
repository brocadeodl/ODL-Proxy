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

#include "ClientInterface/Igmps/IgmpsClientUpdateMrtrVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsClientUpdateMrtrVlanConfigMessage::IgmpsClientUpdateMrtrVlanConfigMessage ()
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATEMRTRVLANCONFIG),
        m_ifType        (0),
        m_vlanId        (0),
        m_vlanIfIndex   (0),
        m_negation      (false)
    {
    }

    IgmpsClientUpdateMrtrVlanConfigMessage::IgmpsClientUpdateMrtrVlanConfigMessage (const UI32 &ifType,const string &ifName,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const bool &negation)
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATEMRTRVLANCONFIG),
        m_ifType    (ifType),
        m_ifName    (ifName),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_negation    (negation)
    {
    }

    IgmpsClientUpdateMrtrVlanConfigMessage::~IgmpsClientUpdateMrtrVlanConfigMessage ()
    {
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeString(&m_vlanIfName,"vlanIfName"));
        addSerializableAttribute (new AttributeUI64(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  IgmpsClientUpdateMrtrVlanConfigMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsClientUpdateMrtrVlanConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  IgmpsClientUpdateMrtrVlanConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  IgmpsClientUpdateMrtrVlanConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  IgmpsClientUpdateMrtrVlanConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  IgmpsClientUpdateMrtrVlanConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  IgmpsClientUpdateMrtrVlanConfigMessage::getNegation() const
    {
        return (m_negation);
    }

	const void *IgmpsClientUpdateMrtrVlanConfigMessage::getCStructureForInputs ()
	{
		igmps_mrtr_vlan_config_msg_t *pInput = (igmps_mrtr_vlan_config_msg_t *)malloc (sizeof (struct igmps_mrtr_vlan_config_msg_));

        if (IGMPS_IF_TYPE_FORTYGIGABIT == m_ifType)
            strncpy (pInput->if_type, "fo", IGMPS_MAX_STR);
        else if (IGMPS_IF_TYPE_TENGIGABIT == m_ifType)
            strncpy (pInput->if_type, "te", IGMPS_MAX_STR);
		else if (IGMPS_IF_TYPE_GIGABIT == m_ifType)
            strncpy (pInput->if_type, "gi", IGMPS_MAX_STR);
        else if (IGMPS_IF_TYPE_PORTCHANNEL == m_ifType)
            strncpy (pInput->if_type, "po", IGMPS_MAX_STR);

		pInput->vlan_id         = m_vlanId;
		pInput->vlan_if_index   = m_vlanIfIndex;
		pInput->negation        = m_negation;
        strncpy (pInput->if_name, m_ifName.c_str(), IGMPS_MAX_STR);
        strncpy (pInput->vlan_if_name, m_vlanIfName.c_str(), IGMPS_MAX_STR);

		return (pInput);
	}

	void IgmpsClientUpdateMrtrVlanConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
