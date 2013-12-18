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
 *   Author : Amitayu Das
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Eld/EldClearMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    EldClearMessage::EldClearMessage ()
        : DcmManagementInterfaceMessage (MSG_ELD_CLEAR)
    {
		m_cmdcode = 0;
                m_ifName = "";
                m_ifType = 0;
    }
    EldClearMessage::EldClearMessage (EldLocalClearMessage *pMsg)
        : DcmManagementInterfaceMessage (MSG_ELD_CLEAR)
    {
		m_cmdcode = pMsg->m_cmdcode;
                m_ifName = pMsg->m_ifName;
                m_ifType = pMsg->m_ifType;
    }

    EldClearMessage::EldClearMessage (const string &interface, const string &ifName,
			const string &Mac, const UI32 &vlan_id, const UI32 &type,
			const UI32 &cmdcode,  const UI32 &bridge, const UI32 &ifindex,
			const UI32 &interface_type, const UI32 &port_channel_id,const UI32 &mappedid)
        : DcmManagementInterfaceMessage (MSG_ELD_CLEAR),
		m_cmdcode   (cmdcode),
                m_ifName (ifName), 
                m_ifType (interface_type) 
    {
    }

    EldClearMessage::~EldClearMessage ()
    {
    }

    void  EldClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"iftype"));
    }

    void  EldClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdcode  =  cmdCode;
    }

    UI32  EldClearMessage::getCmdCode() const
    {
        return (m_cmdcode);
    }

    void EldClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *EldClearMessage::getCStructureForInputs ()
    {
		eld_clear_msg_t *eld_clear = (eld_clear_msg_t *)malloc (sizeof (eld_clear_msg_t));

		eld_clear->cmdcode     = m_cmdcode;
                eld_clear->if_type     = m_ifType;
                strncpy (eld_clear->if_name, m_ifName.c_str(), 32);

        return (eld_clear);
    }

	void EldClearMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
	}
}
