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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "ClientInterface/Policer/PolicerClientTeMessage.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"
#include "ClientInterface/DceWaveClientMessage.h"


namespace DcmNs
{

    PolicerClientTeMessage::PolicerClientTeMessage ()
        : DcmManagementInterfaceMessage (POLICERCLIENTTE)
    {
    }

    PolicerClientTeMessage::PolicerClientTeMessage (const string &ifName,const string &mapName,const UI32 &cmdCode,const UI32 &opCode)
        : DcmManagementInterfaceMessage (POLICERCLIENTTE),
		m_ifName	(ifName),
        m_mapName    (mapName),
        m_cmdCode	(cmdCode),
		m_opCode    (opCode)
    {
    }

    PolicerClientTeMessage::~PolicerClientTeMessage ()
    {
    }

    void  PolicerClientTeMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeString(&m_mapName,"mapName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

	void PolicerClientTeMessage::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string PolicerClientTeMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  PolicerClientTeMessage::setMapName(const string &mapName)
    {
        m_mapName  =  mapName;
    }

    string  PolicerClientTeMessage::getMapName() const
    {
        return (m_mapName);
    }

    void  PolicerClientTeMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PolicerClientTeMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PolicerClientTeMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  PolicerClientTeMessage::getOpCode() const
    {
        return (m_opCode);
    }

	const void *PolicerClientTeMessage::getCStructureForInputs () {

		fbDcmBindPolicyMapMessage_t *pInput = (fbDcmBindPolicyMapMessage_t *)malloc(sizeof (fbDcmBindPolicyMapMessage_t));
		strncpy (pInput->policy_name, m_mapName.c_str (), FB_MAX_MAP_NAME_SIZE-1);
		pInput->policy_name[FB_MAX_MAP_NAME_SIZE-1] = '\0';
		strncpy (pInput->if_name, m_ifName.c_str(), MAX_IF_NAME_LEN+3 );
		pInput->if_name[MAX_IF_NAME_LEN+3] = '\0';
		pInput->opcode = (fbDcmCommandCode_t)m_opCode;

		if ((FB_POLICYMAP_BIND_INTERFACE_IN == m_cmdCode)
				|| (FB_POLICYMAP_UNBIND_INTERFACE_IN == m_cmdCode)) {
			pInput->dir = (fbPolicyDirection_t)FB_POLICY_DIR_IN;
		} 
        if ((FB_POLICYMAP_BIND_INTERFACE_OUT == m_cmdCode)
				|| (FB_POLICYMAP_UNBIND_INTERFACE_OUT == m_cmdCode)) {
			pInput->dir = (fbPolicyDirection_t)FB_POLICY_DIR_OUT;
		} 

        /*trace (TRACE_LEVEL_INFO, "Entering PolicerClientTeMessage::getCStructureForInputs\n");
        trace (TRACE_LEVEL_INFO, string("PolicerClientTeMessage::getCStructureForInputs: Opcode : <") +  pInput->opcode + ">\n");
        trace (TRACE_LEVEL_INFO, string("PolicerClientTeMessage::getCStructureForInputs: IfName : <") +  (pInput->if_name) + ">\n");
        trace (TRACE_LEVEL_INFO, string("PolicerClientTeMessage::getCStructureForInputs: PolicyMapName : <")+ (pInput->policy_name)+ ">\n");
        trace (TRACE_LEVEL_INFO, string("PolicerClientTeMessage::getCStructureForInputs: Direction: <")+ pInput->dir + ">\n");*/

        return (pInput);
    }

    void PolicerClientTeMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
