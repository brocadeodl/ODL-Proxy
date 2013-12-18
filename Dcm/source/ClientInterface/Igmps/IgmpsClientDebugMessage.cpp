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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "ClientInterface/Igmps/IgmpsClientDebugMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    IgmpsClientDebugMessage::IgmpsClientDebugMessage()
        : DcmManagementInterfaceMessage (IGMPSCLIENTDEBUG),
          m_cmdCode     (IGMPS_DEFAULT_CMD_CODE),
          m_isNegation  (false)
    {
    }

    IgmpsClientDebugMessage::IgmpsClientDebugMessage(const string &grpName, const string &ifName, const UI32 &cmdCode, const bool &isNegation)
        : DcmManagementInterfaceMessage (IGMPSCLIENTDEBUG),
        m_grpName   (grpName),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode),
        m_isNegation   (isNegation)
    {
    }

    IgmpsClientDebugMessage::~IgmpsClientDebugMessage ()
    {
    }

    void  IgmpsClientDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_grpName,"grpName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeBool (&m_isNegation,"isNegation"));
    }

    void  IgmpsClientDebugMessage::setGrpName(const string &grpName)
    {
        m_grpName  =  grpName;
    }

    string  IgmpsClientDebugMessage::getGrpName() const
    {
        return (m_grpName);
    }
    
    void  IgmpsClientDebugMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsClientDebugMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  IgmpsClientDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  IgmpsClientDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  IgmpsClientDebugMessage::setNegation(const bool &isNegation)
    {
        m_isNegation  =  isNegation;
    }

    bool  IgmpsClientDebugMessage::getNegation() const
    {
        return (m_isNegation);
    }

    const void *IgmpsClientDebugMessage::getCStructureForInputs ()
    {
        igmps_debug_input_msg_t *pInput = (igmps_debug_input_msg_t *) malloc (sizeof (igmps_debug_input_msg_t));

        trace (TRACE_LEVEL_DEBUG, "Entering IgmpsClientDebugMessage::getCStructureForInputs\n");
        strncpy (pInput->grp_name, m_grpName.c_str (), IGMPS_MAX_STR);
        strncpy (pInput->if_name, m_ifName.c_str (), IGMPS_MAX_STR);
        pInput->cmdCode = m_cmdCode;
        pInput->negation = m_isNegation;

        return (pInput);
    }

	void IgmpsClientDebugMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
        trace (TRACE_LEVEL_DEBUG, "Entering IgmpsClientDebugMessage::loadOutputsFromCStructure\n");

        if (!pOutputCStructure) return;


        if (m_cmdCode == IGMPS_SHOW_DEBUG_CLI) {
            igmps_debug_output_msg_t *ptr = (igmps_debug_output_msg_t *) pOutputCStructure;
            addBuffer (IGMPS_SHOW_DEBUG_CLI, ptr->buf_len, pOutputCStructure, false);
        }
	}
}
