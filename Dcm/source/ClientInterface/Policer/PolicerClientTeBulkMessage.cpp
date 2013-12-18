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

#include "ClientInterface/Policer/PolicerClientTeBulkMessage.h"
#include "ssm_policer_dcmd_public.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "qosd_fb_public.h"

namespace DcmNs
{

    PolicerClientTeBulkMessage::PolicerClientTeBulkMessage ()
        : DcmManagementInterfaceMessage (POLICERCLIENTTEBULK)
    {
    }

    PolicerClientTeBulkMessage::PolicerClientTeBulkMessage (const string &ifName,const string &inputMapName,const string &outputMapName)
        : DcmManagementInterfaceMessage (POLICERCLIENTTEBULK),
		m_ifName	(ifName),
        m_inputMapName    (inputMapName),
        m_outputMapName	(outputMapName)
    {
    }

    PolicerClientTeBulkMessage::~PolicerClientTeBulkMessage ()
    {
    }

    void  PolicerClientTeBulkMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeString(&m_inputMapName,"inputMapName"));
        addSerializableAttribute (new AttributeString(&m_outputMapName,"outputMapName"));
    }

	void PolicerClientTeBulkMessage::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string PolicerClientTeBulkMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  PolicerClientTeBulkMessage::setInputMapName(const string &inputMapName)
    {
        m_inputMapName  =  inputMapName;
    }

    string  PolicerClientTeBulkMessage::getInputMapName() const
    {
        return (m_inputMapName);
    }

    void  PolicerClientTeBulkMessage::setOutputMapName(const string &outputMapName)
    {
        m_outputMapName  =  outputMapName;
    }

    string  PolicerClientTeBulkMessage::getOutputMapName() const
    {
        return (m_outputMapName);
    }


	const void *PolicerClientTeBulkMessage::getCStructureForInputs ()
    {
		ssm_pol_bind_policymap_bulk_msg_t *pInput = (ssm_pol_bind_policymap_bulk_msg_t *)malloc(sizeof (ssm_pol_bind_policymap_bulk_msg_t));
		strncpy (pInput->input_policy_map_name.name, m_inputMapName.c_str (), FB_MAX_MAP_NAME_SIZE-1);
		pInput->input_policy_map_name.name[FB_MAX_MAP_NAME_SIZE-1] = '\0';
		strncpy (pInput->output_policy_map_name.name, m_outputMapName.c_str (), FB_MAX_MAP_NAME_SIZE-1);
		pInput->output_policy_map_name.name[FB_MAX_MAP_NAME_SIZE-1] = '\0';
		strncpy (pInput->if_name, m_ifName.c_str(), MAX_IF_NAME_LEN );
		pInput->if_name[MAX_IF_NAME_LEN] = '\0';
        return (pInput);
    }

    void PolicerClientTeBulkMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
