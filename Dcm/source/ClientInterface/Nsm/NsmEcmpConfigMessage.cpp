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

#include "ClientInterface/Nsm/NsmEcmpConfigMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

	NsmEcmpConfigMessage::NsmEcmpConfigMessage ()
		: DcmManagementInterfaceMessage (NSMECMPCONFIG)
		{
			m_loadBalanceMode = 7;
			m_hashSwap = 0;
		}


	NsmEcmpConfigMessage::NsmEcmpConfigMessage (const UI32 &loadBalanceMode,const UI32 &cmdCode,
		const UI32 &hashSwap) : DcmManagementInterfaceMessage (NSMECMPCONFIG),
								m_loadBalanceMode    (loadBalanceMode),
								m_cmdCode    (cmdCode),
								m_hashSwap   (hashSwap)
	{
	}

	NsmEcmpConfigMessage::~NsmEcmpConfigMessage ()
	{
	}

	void  NsmEcmpConfigMessage::setupAttributesForSerialization()
	{
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_loadBalanceMode,"loadBalanceMode"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_hashSwap,"hashSwap"));
	}

	void  NsmEcmpConfigMessage::setLoadBalanceMode(const UI32 &loadBalanceMode)
	{
		m_loadBalanceMode  =  loadBalanceMode;
	}

	UI32  NsmEcmpConfigMessage::getLoadBalanceMode()const
	{
		return(m_loadBalanceMode);
	}

	void  NsmEcmpConfigMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32 NsmEcmpConfigMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

	void  NsmEcmpConfigMessage::setHashSwap(const UI32 &hashSwap)
	{
		m_hashSwap  =  hashSwap;
	}

	UI32 NsmEcmpConfigMessage::getHashSwap() const
	{
		return (m_hashSwap);
	}

	const void *NsmEcmpConfigMessage::getCStructureForInputs ()
	{
		NsmEcmpMsg_t *pInput = new NsmEcmpMsg_t;
		pInput->loadBalanceMode = m_loadBalanceMode;
		pInput->cmdCode =  m_cmdCode;
		pInput->hashSwap = m_hashSwap;
		return (pInput);
	}

	void NsmEcmpConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}

