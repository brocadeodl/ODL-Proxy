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
 *   Author : Karthik A                                                    *
 **************************************************************************/

#include "ClientInterface/VRRP/VRRPClearStatsMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    VRRPClearStatsMessage::VRRPClearStatsMessage()
        : DcmManagementInterfaceMessage (VRRP_CLEAR_STATS_MSG)
    {
    }

    VRRPClearStatsMessage::VRRPClearStatsMessage(const UI32 &vrid, const string &ifName, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (VRRP_CLEAR_STATS_MSG),
        m_vrid          (vrid),
        m_if_name       (ifName),
        m_cmdCode       (cmdCode)
    {
    }

    VRRPClearStatsMessage::~VRRPClearStatsMessage ()
    {
    }

    void  VRRPClearStatsMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32 (&m_vrid, "vrid"));
        addSerializableAttribute (new AttributeString (&m_if_name, "ifName"));
    }

    void  VRRPClearStatsMessage::setVrid(const UI32 &vrid)
    {
        m_vrid = vrid;
    }

    UI32 VRRPClearStatsMessage::getVrid() const
    {
        return (m_vrid);
    }
    
    void  VRRPClearStatsMessage::setIfName(const string &ifName)
    {
        m_if_name  =  ifName;
    }

    string  VRRPClearStatsMessage::getIfName() const
    {
        return (m_if_name);
    }
    
    void  VRRPClearStatsMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  VRRPClearStatsMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void VRRPClearStatsMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *VRRPClearStatsMessage::getCStructureForInputs ()
    {
        vrrp_clear_stats_input_msg_t *pInput = new vrrp_clear_stats_input_msg_t;

        pInput->vrid = m_vrid;
        strncpy (pInput->if_name, m_if_name.c_str (), 64);
        pInput->cmdCode = m_cmdCode;

        return (pInput);
    }

	void VRRPClearStatsMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{

		vrrp_clear_stats_output_msg_t *msg = ((vrrp_clear_stats_output_msg_t *)pOutputCStructure);

        if (!msg)
			return;

       	addBuffer (VRRP_CLEAR_STATS_MSG, sizeof(vrrp_clear_stats_output_msg_t), pOutputCStructure, false);
	}
	
	void VRRPClearStatsMessage::dumpInputMessage()
	{
		trace(TRACE_LEVEL_DEBUG, "VRRPClearStatsMessage::dumpInputMessage");
		trace(TRACE_LEVEL_DEBUG, string("m_vrid = ") + m_vrid);
		trace(TRACE_LEVEL_DEBUG, string("m_if_name = ") + m_if_name);
		trace(TRACE_LEVEL_DEBUG, string("m_cmdCode = ") + m_cmdCode);
	}
}
