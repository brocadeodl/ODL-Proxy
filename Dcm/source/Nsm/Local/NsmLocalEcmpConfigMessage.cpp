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

#include "Nsm/Local/NsmLocalEcmpConfigMessage.h"
#include "Nsm/Local/EcmpConfigManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

	NsmLocalEcmpConfigMessage::NsmLocalEcmpConfigMessage ()
		: PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALECMPCONFIG)
		{
			m_loadBalanceMode = 7;
			m_cmdCode = 0;
			m_hashSwap = 0;
		}

	NsmLocalEcmpConfigMessage::NsmLocalEcmpConfigMessage (const UI32 &loadBalanceMode ,
		const UI32 &cmdCode,const UI32 &hashSwap)
		: PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALECMPCONFIG),
		m_loadBalanceMode   (loadBalanceMode),
		m_cmdCode    (cmdCode),
		m_hashSwap   (hashSwap)
		{
		}

	NsmLocalEcmpConfigMessage::~NsmLocalEcmpConfigMessage ()
	{
	}

	void  NsmLocalEcmpConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_loadBalanceMode,"loadBalanceMode"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_hashSwap,"hashSwap"));
	}

	void  NsmLocalEcmpConfigMessage::setLoadBalanceMode(const UI32 &loadBalanceMode)
	{
		m_loadBalanceMode  =  loadBalanceMode;
	}

	UI32 NsmLocalEcmpConfigMessage::getLoadBalanceMode() const
	{
		return (m_loadBalanceMode);
	}

	void  NsmLocalEcmpConfigMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32 NsmLocalEcmpConfigMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}
	void  NsmLocalEcmpConfigMessage::setHashSwap(const UI32 &hashSwap)
	{
		m_hashSwap  =  hashSwap;
	}

	UI32 NsmLocalEcmpConfigMessage::getHashSwap() const
	{
		return (m_hashSwap);
	}
}

