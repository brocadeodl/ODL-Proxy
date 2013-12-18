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
#include "ClientInterface/Eld/EldDebugMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    EldDebugMessage::EldDebugMessage ()
        : DcmManagementInterfaceMessage (MSG_ELD_DEBUG)
    {
		m_cmdcode = 0;
		m_rbridge = 0;
		m_vlanid = 0;
		m_dbgFlags = 0;
    }

    EldDebugMessage::EldDebugMessage (const string &interface, const string &ifName,
			const string &Mac, const UI32 &vlan_id, const UI32 &type,
			const UI32 &cmdcode,  const UI32 &bridge, const UI32 &ifindex,
			const UI32 &interface_type, const UI32 &port_channel_id,const UI32 &mappedid)
        : DcmManagementInterfaceMessage (MSG_ELD_DEBUG),
		m_cmdcode   (cmdcode)
    {
		m_cmdcode = 0;
		m_rbridge = 0;
		m_vlanid = 0;
		m_dbgFlags = 0;
    }

    EldDebugMessage::~EldDebugMessage ()
    {
    }

    void  EldDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlan_id"));
        addSerializableAttribute (new AttributeString(&m_interface,"interface"));
        addSerializableAttribute (new AttributeUI32(&m_rbridge,"rbridge"));
		addSerializableAttribute(new AttributeUI32(&m_dbgFlags,"dbgFlags"));
    }

    void  EldDebugMessage::setDbgFlags(const UI32 &dbgFlags)
    {
        m_dbgFlags  =  dbgFlags;
    }

    UI32  EldDebugMessage::getDbgFlags() const
    {
        return (m_dbgFlags);
	}
    void  EldDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdcode  =  cmdCode;
    }

    UI32  EldDebugMessage::getCmdCode() const
    {
        return (m_cmdcode);
    }

    void  EldDebugMessage::setRbridge(const UI32 &rbridge_id)
    {
        m_rbridge  =  rbridge_id;
    }

    UI32  EldDebugMessage::getRbridge() const
    {
        return (m_rbridge);
    }
    
	void  EldDebugMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  EldDebugMessage::getInterface() const
    {
        return (m_interface);
    }

    UI32  EldDebugMessage::getVlanid() const
    {
        return (m_vlanid);
    }

	 void  EldDebugMessage::setVlanid(const UI32 &vlan_id)
    {
        m_vlanid  =  vlan_id;
    }

    void EldDebugMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *EldDebugMessage::getCStructureForInputs ()
    {
		eld_debug_msg_t *eld_debug = (eld_debug_msg_t *)malloc (sizeof (eld_debug_msg_t));
		if (eld_debug != NULL)
			trace(TRACE_LEVEL_DEBUG, "ELD_DEBUG IS NON-NULL. CONTINUING.....\n");
		eld_debug->cmdcode     = m_cmdcode;
		eld_debug->rbridge_id  = m_rbridge;
		eld_debug->vlan_id  = m_vlanid;
		eld_debug->dbgFlags  = m_dbgFlags;
		strncpy (eld_debug->if_name, m_interface.c_str(), 64);

        return (eld_debug);
    }

	void EldDebugMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
		eld_debug_resp_msg_t *msg = ((eld_debug_resp_msg_t *)pOutputCStructure);

		trace(TRACE_LEVEL_DEBUG, "Inside loadOutputsFromCStructure.....\n");

        if(msg == NULL)
			return;

		trace(TRACE_LEVEL_DEBUG, string("MSG is non-null with record_type ") + msg->record_type);

		switch (msg->record_type) {
			case ELD_BUF_INTERFACE:
				trace(TRACE_LEVEL_DEBUG, string("Record-type is ELD_BUF_INTERFACE"));
  				addBuffer (MSG_BUF_INTF, msg->size, pOutputCStructure, false); 
				break;

			case ELD_BUF_RBRIDGE:
				trace(TRACE_LEVEL_DEBUG, string("Record-type is ELD_BUF_RBRIDGE"));
				addBuffer (MSG_BUF_RBRIDGE, msg->size, pOutputCStructure, false);
				break;

			default:
				break;
		} 
	}
}
