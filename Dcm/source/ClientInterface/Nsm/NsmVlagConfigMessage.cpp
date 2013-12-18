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

#include "ClientInterface/Nsm/NsmVlagConfigMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

	NsmVlagConfigMessage::NsmVlagConfigMessage ()
		: DcmManagementInterfaceMessage (NSMVLAGCONFIG)
		{
			m_loadBalanceMode = 7;
		}


	NsmVlagConfigMessage::NsmVlagConfigMessage (const UI32 &loadBalanceMode,const UI32 &cmdCode,
		const UI32 &poId) : DcmManagementInterfaceMessage (NSMVLAGCONFIG),
							m_loadBalanceMode    (loadBalanceMode),
							m_cmdCode    (cmdCode),
							m_poId   (poId)
	{
	}

	NsmVlagConfigMessage::~NsmVlagConfigMessage ()
	{
	}

	void  NsmVlagConfigMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_loadBalanceMode,"loadBalanceMode"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
	}

	void  NsmVlagConfigMessage::setLoadBalanceMode(const UI32 &loadBalanceMode)
	{
		m_loadBalanceMode  =  loadBalanceMode;
	}

	UI32  NsmVlagConfigMessage::getLoadBalanceMode()const
	{
		return(m_loadBalanceMode);
	}

	void  NsmVlagConfigMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32 NsmVlagConfigMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

	void  NsmVlagConfigMessage::setPoId(const UI32 &poId)
	{
		m_poId  =  poId;
	}

	UI32 NsmVlagConfigMessage::getPoId() const
	{
		return (m_poId);
	}

	const void *NsmVlagConfigMessage::getCStructureForInputs ()
	{
		NsmVlagMsg_t *pInput = new NsmVlagMsg_t;
		pInput->loadBalanceMode = m_loadBalanceMode;
		pInput->cmdCode =  m_cmdCode;
		pInput->poId = m_poId;
		return (pInput);
	}

	void NsmVlagConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}

