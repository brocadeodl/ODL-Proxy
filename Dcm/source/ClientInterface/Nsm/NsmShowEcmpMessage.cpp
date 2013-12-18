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
 *   Author : Amulya Makam                                                 *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/NsmShowEcmpMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

	NsmShowEcmpMessage::NsmShowEcmpMessage ()
		: DcmManagementInterfaceMessage (NSM_SHOW_ECMP)
		{
		}

	NsmShowEcmpMessage::NsmShowEcmpMessage (const UI32 &cmdCode,const UI32 &rbridgeId)
		: DcmManagementInterfaceMessage (NSM_SHOW_ECMP),
		m_cmdCode   (cmdCode),
		m_rbridgeId  (rbridgeId)
		{
		}

	NsmShowEcmpMessage::~NsmShowEcmpMessage ()
	{
	}

	void  NsmShowEcmpMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute(new AttributeUI32(&m_rbridgeId,"rbridgeId"));
	}

	void  NsmShowEcmpMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32  NsmShowEcmpMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}
	void  NsmShowEcmpMessage::setRbridgeId(const UI32 &rbridgeId)
	{
		m_rbridgeId  =  rbridgeId;
	}

	UI32  NsmShowEcmpMessage::getRbridgeId() const
	{
		return (m_rbridgeId);
	}


	void NsmShowEcmpMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer   =   findBuffer(bufferNum, size);
	}

	const void *NsmShowEcmpMessage::getCStructureForInputs ()
	{
		NsmShowEcmp_InputMsg_t *pInput = new NsmShowEcmp_InputMsg_t;
		pInput->cmdCode     = m_cmdCode;
		pInput->rbridgeId   = m_rbridgeId;
		return (pInput);
	}

	void NsmShowEcmpMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		int no_of_records = 0 ;

		if(pOutputCStructure != NULL)
			no_of_records = *((int *)pOutputCStructure);

		nsm_mgmt_buffer_num_t  buff_num;
		memset(&buff_num, 0, sizeof(buff_num));
		buff_num.buff.mapped_id = this->getRbridgeId();
		buff_num.buff.tag = NSMSHOW_ECMP_LOADBALANCE;

		switch(m_cmdCode)
		{
		case NSMSHOW_ECMP_LOADBALANCE:
			{

				if(no_of_records == 0 )
					break;

				addBuffer (buff_num.num, (sizeof(nsm_ecmp_loadbalance_all_msg_t) +
						((no_of_records-1)* sizeof(nsm_ecmp_loadbalance_info_t))),
					pOutputCStructure , false);
				break;
			}

		default:
			break;
		}
	}
}

