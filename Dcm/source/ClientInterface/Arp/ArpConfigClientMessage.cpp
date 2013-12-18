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
#include "ClientInterface/Arp/ArpConfigClientMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"

namespace DcmNs
{

    ArpConfigClientMessage::ArpConfigClientMessage ()
        : DcmManagementInterfaceMessage (ARPCONFIGCLIENT)
    {
    }

    ArpConfigClientMessage::ArpConfigClientMessage (const UI32 &SystemMaxArp)
        : DcmManagementInterfaceMessage (ARPCONFIGCLIENT),
        m_SystemMaxArp    (SystemMaxArp)
    {
    }

    ArpConfigClientMessage::~ArpConfigClientMessage ()
    {
    }

    void  ArpConfigClientMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_SystemMaxArp,"SystemMaxArp"));
    }

    void  ArpConfigClientMessage::setSystemMaxArp(const UI32 &SystemMaxArp)
    {
        m_SystemMaxArp  =  SystemMaxArp;
    }

    UI32  ArpConfigClientMessage::getSystemMaxArp() const
    {
        return (m_SystemMaxArp);
    }

    const void *ArpConfigClientMessage::getCStructureForInputs ()
    {
        arp_config_msg_t  *msg = (arp_config_msg_t *) calloc (1, sizeof (arp_config_msg_t));
        msg->system_max = m_SystemMaxArp;
        return msg;
    }

	void 
    ArpConfigClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
