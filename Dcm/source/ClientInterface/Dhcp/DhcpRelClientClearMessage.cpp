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
#include "ClientInterface/Dhcp/DhcpRelClientClearMessage.h" 
#include "ClientInterface/Dhcp/DhcpMessageDef.h"

namespace DcmNs
{

    DhcpRelClientClearMessage::DhcpRelClientClearMessage()
        : DcmManagementInterfaceMessage (IPHELPCLEAR),
          m_cmdCode       (0),
		  m_serverAddress ("")
    {
    }

    DhcpRelClientClearMessage::~DhcpRelClientClearMessage ()
    {
    }

    void  DhcpRelClientClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeIpV4Address(&m_serverAddress,"serverAddress"));
    }

    void  DhcpRelClientClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  DhcpRelClientClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  DhcpRelClientClearMessage::setServerAddress(const IpV4Address &serverAddress)
    {
        m_serverAddress  =  serverAddress;
    }

    IpV4Address  DhcpRelClientClearMessage::getServerAddress() const
    {
        return (m_serverAddress);
    }


    void DhcpRelClientClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *DhcpRelClientClearMessage::getCStructureForInputs ()
    {
       	iphelp_dhcprelay_clear_stats_in_msg_t *pInput = (iphelp_dhcprelay_clear_stats_in_msg_t *) calloc (1, sizeof (iphelp_dhcprelay_clear_stats_in_msg_t));

       	pInput->cmd = m_cmdCode;

		if (m_cmdCode == IPHELP_CLEAR_IP_DHCPRELAY_STATS)
			strncpy(pInput->ip_address, ((m_serverAddress.toString()).c_str()), DHCP_MAX_IPV4ADDR_STR);
       	
		return (pInput);
    }

	void DhcpRelClientClearMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{

	}
}
