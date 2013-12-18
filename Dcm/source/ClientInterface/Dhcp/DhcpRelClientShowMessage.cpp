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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Dhcp/DhcpRelClientShowMessage.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    DhcpRelClientShowMessage::DhcpRelClientShowMessage()
        : DcmManagementInterfaceMessage (IPHELPSHOW),
        m_cmdCode   (0),
        m_ifName    (""),
        m_ifType    (IF_TYPE_INVALID),
        m_vlanValue (0)
    {
    }

    DhcpRelClientShowMessage::DhcpRelClientShowMessage(const UI32 &cmdCode, const string &ifName, const UI32 &ifType, const UI32 &vlanValue)
        : DcmManagementInterfaceMessage (IPHELPSHOW),
        m_cmdCode    (cmdCode),
        m_ifName    (ifName),
        m_ifType   (ifType),
        m_vlanValue   (vlanValue)
    {
    }

    DhcpRelClientShowMessage::~DhcpRelClientShowMessage ()
    {
    }

    void  DhcpRelClientShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32 (&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI32 (&m_vlanValue,"vlanValue"));
    }

    void  DhcpRelClientShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  DhcpRelClientShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  DhcpRelClientShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  DhcpRelClientShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  DhcpRelClientShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  DhcpRelClientShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  DhcpRelClientShowMessage::setVlanValue(const UI32 &vlanValue)
    {
        m_vlanValue  =  vlanValue;
    }

    UI32  DhcpRelClientShowMessage::getVlanValue() const
    {
        return (m_vlanValue);
    }

    void DhcpRelClientShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *DhcpRelClientShowMessage::getCStructureForInputs ()
    {
		void *pCMsg = NULL;

		if (m_cmdCode == IPHELP_SHOW_IF_DRADDR)
		{
        	iphelp_dhcprelay_show_intf_in_msg_t *pInput = (iphelp_dhcprelay_show_intf_in_msg_t *) 
										malloc (sizeof (iphelp_dhcprelay_show_intf_in_msg_t));

        	strncpy (pInput->if_name, m_ifName.c_str (), DHCP_MAX_IFNAME_STR);
        	pInput->cmd = m_cmdCode;
        	pInput->interface_type = m_ifType;
        	pInput->interface_id = m_vlanValue;
			pCMsg = pInput;
		}
		else if (m_cmdCode == IPHELP_SHOW_DRADDR)
		{
        	iphelp_dhcprelay_show_in_msg_t *pInput = (iphelp_dhcprelay_show_in_msg_t *) 
										malloc (sizeof (iphelp_dhcprelay_show_in_msg_t));

        	pInput->cmd = m_cmdCode;
			pCMsg = pInput;
		}
		else
		{ /* has to be IPHELP_SHOW_DCHP_RELAY_STATS */
        	iphelp_dhcprelay_show_stats_in_msg_t *pInput = (iphelp_dhcprelay_show_stats_in_msg_t *) 
										malloc (sizeof (iphelp_dhcprelay_show_stats_in_msg_t));

        	pInput->cmd = m_cmdCode;
			pCMsg = pInput;
		}
        
		return (pCMsg);
    }

	void DhcpRelClientShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		UI32 size = 0;

        if (!pOutputCStructure) 
			return;

		/* For DHCP Relay, we have 2 different output structs for 3 show commands. 
		   To distinguish between the 2, we first typecast pOutputCStructure to one 
		   of the structure types, and read the cmdCode. Then, based on the cmdCode, 
		   we typecast to the correct structure type. This works because both the out 
		   structures have the cmdCode as the first member. */

        iphelp_dhcprelay_show_out_msg_t *msg = (iphelp_dhcprelay_show_out_msg_t *)pOutputCStructure;

		if ((msg->cmd == IPHELP_SHOW_DRADDR) || (msg->cmd == IPHELP_SHOW_IF_DRADDR))
		{
			iphelp_dhcprelay_show_out_msg_t  *msg1 = (iphelp_dhcprelay_show_out_msg_t *)pOutputCStructure;

			if (msg1->num_records > 0)
				size =  sizeof(iphelp_dhcprelay_show_out_msg_t) + ((msg1->num_records - 1) * sizeof(draddr_t));
			else
				size =  sizeof(iphelp_dhcprelay_show_out_msg_t);
		}
		else /* If we reach here, cmd should be IPHELP_SHOW_DCHP_RELAY_STATS */
		{
			iphelp_dhcprelay_show_stats_out_msg_t  *msg2 = (iphelp_dhcprelay_show_stats_out_msg_t *)pOutputCStructure;

			if (msg2->num_of_recs > 0)
				size =  sizeof(iphelp_dhcprelay_show_stats_out_msg_t) + (msg2->num_of_recs - 1) * sizeof(iphelp_dhcprelay_stats_rec_t);
			else
				size = sizeof(iphelp_dhcprelay_show_stats_out_msg_t);
		}
			 
        addBuffer (IPHELPSHOW, size, pOutputCStructure, false);

	}
}

