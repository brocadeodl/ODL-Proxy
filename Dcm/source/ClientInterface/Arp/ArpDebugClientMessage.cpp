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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Arp/ArpDebugClientMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"

namespace DcmNs
{

    ArpDebugClientMessage::ArpDebugClientMessage ()
        : DcmManagementInterfaceMessage (ARPDEBUGCLIENT)
    {
    }

    ArpDebugClientMessage::ArpDebugClientMessage (const IpV4Address &IpAddress)
        : DcmManagementInterfaceMessage (ARPDEBUGCLIENT),
        m_IpAddress    (IpAddress)
    {
    }

    ArpDebugClientMessage::~ArpDebugClientMessage ()
    {
    }

    void  ArpDebugClientMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4Address(&m_IpAddress,"IpAddress"));
        addSerializableAttribute (new AttributeUI32(&m_NoFlag,"NoFlag"));
    }

    void  ArpDebugClientMessage::setIpAddress(const IpV4Address &IpAddress)
    {
        m_IpAddress  =  IpAddress;
    }

    IpV4Address  ArpDebugClientMessage::getIpAddress() const
    {
        return (m_IpAddress);
    }

	void  ArpDebugClientMessage::setNoFlag(const UI32 &NoFlag)
    {
        m_NoFlag  =  NoFlag;
    }

    UI32 ArpDebugClientMessage::getNoFlag() const
    {
        return (m_NoFlag);
    }


    const void *ArpDebugClientMessage::getCStructureForInputs ()
    {
        arp_debug_msg_t  *msg = (arp_debug_msg_t *) calloc (1, sizeof (arp_debug_msg_t));
        string str_ipaddr;
        str_ipaddr = m_IpAddress.toString();
        strncpy(msg->ip_address, str_ipaddr.c_str(), 15);
        msg->ip_address[15] = '\0';
		msg->noflag = m_NoFlag; 
        return msg;
    }

	void 
    ArpDebugClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
