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

#include "ClientInterface/VRRP/VRRPDebugMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    VRRPDebugMessage::VRRPDebugMessage()
        : DcmManagementInterfaceMessage (VRRP_DEBUG_MSG)
    {
    }

    VRRPDebugMessage::VRRPDebugMessage(const UI32 &vrid, const bool &negation, const string &ifName, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (VRRP_DEBUG_MSG),
        m_vrid          (vrid),
        m_negation      (negation),
		m_if_name       (ifName),
        m_cmdCode       (cmdCode)
    {
    }

    VRRPDebugMessage::~VRRPDebugMessage ()
    {
    }

    void  VRRPDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32 (&m_vrid, "vrid"));
        addSerializableAttribute (new AttributeBool (&m_negation, "negation"));
        addSerializableAttribute (new AttributeString (&m_if_name, "ifName"));
        addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "cmdCode"));
    }

    void  VRRPDebugMessage::setVrid(const UI32 &vrid)
    {
        m_vrid = vrid;
    }

    UI32 VRRPDebugMessage::getVrid() const
    {
        return (m_vrid);
    }
    
    void  VRRPDebugMessage::setNegation(const bool &negation)
    {
        m_negation =  negation;
    }

    bool VRRPDebugMessage::getNegation() const
    {
        return (m_negation);
    }
    
    void  VRRPDebugMessage::setIfName(const string &ifName)
    {
        m_if_name  =  ifName;
    }

    string  VRRPDebugMessage::getIfName() const
    {
        return (m_if_name);
    }
    
    void  VRRPDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  VRRPDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void VRRPDebugMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *VRRPDebugMessage::getCStructureForInputs ()
    {
        vrrp_debug_input_msg_t *pInput = new vrrp_debug_input_msg_t;

        pInput->vrid = m_vrid;
        pInput->negation = m_negation;
        strncpy (pInput->if_name, m_if_name.c_str (), VRRP_IFNAME_LEN);
        pInput->cmdCode = m_cmdCode;

        return (pInput);
    }

	void VRRPDebugMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		vrrp_debug_output_msg_t *msg = ((vrrp_debug_output_msg_t *)pOutputCStructure);

        if (!msg) return;

        /* Allocate the C++ buffer for C message */
       	addBuffer (VRRP_DEBUG_MSG, sizeof(vrrp_debug_output_msg_t), pOutputCStructure, false);
	}

	void VRRPDebugMessage::dumpInputMessage()
	{
		trace(TRACE_LEVEL_DEBUG, "VRRPDebugMessage::dumpInputMessage");
		trace(TRACE_LEVEL_DEBUG, string("m_vrid = ") + m_vrid);
		trace(TRACE_LEVEL_DEBUG, string("m_negation = ") + m_negation);
		trace(TRACE_LEVEL_DEBUG, string("m_if_name = ") + m_if_name);
		trace(TRACE_LEVEL_DEBUG, string("m_cmdCode = ") + m_cmdCode);
	}
}
