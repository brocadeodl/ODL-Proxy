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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    NsmDefaultConfigurationCmdsMessage::NsmDefaultConfigurationCmdsMessage ()
        : DcmManagementInterfaceMessage (NSM_DEFAULT_CONFIG_CMD)
    {
    }

    NsmDefaultConfigurationCmdsMessage::NsmDefaultConfigurationCmdsMessage (const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (NSM_DEFAULT_CONFIG_CMD),
        m_cmdCode    (cmdCode)
    {
    }

    NsmDefaultConfigurationCmdsMessage::~NsmDefaultConfigurationCmdsMessage ()
    {
    }

    void  NsmDefaultConfigurationCmdsMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmDefaultConfigurationCmdsMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmDefaultConfigurationCmdsMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void NsmDefaultConfigurationCmdsMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *NsmDefaultConfigurationCmdsMessage::getCStructureForInputs ()
    {
        nsm_req_msg_t *pInput = new nsm_req_msg_t;

        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

	void NsmDefaultConfigurationCmdsMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
        nsm_intf_all_msg_t *msg = ((nsm_intf_all_msg_t *)pOutputCStructure);
		int num_records = msg->num_records;

		/* Set LacpMessageBufferId */
		switch (msg->record_type) {
            case NSM_DEFAULT_CONFIG_RESPONSE: {
        		addBuffer (NSM_DEFAULT_CONFIG_RESPONSE, (sizeof(nsm_intf_all_msg_t)+ ((num_records - 1) * sizeof (nsm_intf_msg_t))), pOutputCStructure, false);
				break;
			}
			default :
				break;
		}
	}
}
