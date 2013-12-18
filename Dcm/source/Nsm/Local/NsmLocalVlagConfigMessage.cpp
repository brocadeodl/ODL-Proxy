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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLocalVlagConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

	NsmLocalVlagConfigMessage::NsmLocalVlagConfigMessage ()
		: PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),NSMLOCALVLAGCONFIG)
		{
			m_mode = 7;
			m_cmdCode = 0;
		}

	NsmLocalVlagConfigMessage::NsmLocalVlagConfigMessage (const UI32 &poId,
		const UI32 &mode,const UI32 &cmdCode)
		: PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),NSMLOCALVLAGCONFIG),
		m_poId    (poId),
		m_mode    (mode),
		m_cmdCode (cmdCode)
		{
		}

	NsmLocalVlagConfigMessage::~NsmLocalVlagConfigMessage ()
	{
	}

	void  NsmLocalVlagConfigMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
		addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
	}

	void  NsmLocalVlagConfigMessage::setLoadBalanceMode(const UI32 &mode)
	{
		m_mode  =  mode;
	}

	UI32 NsmLocalVlagConfigMessage::getLoadBalanceMode() const
	{
		return (m_mode);
	}

	void  NsmLocalVlagConfigMessage::setPoId(const UI32 &poId)
	{
		m_poId  =  poId;
	}

	UI32 NsmLocalVlagConfigMessage::getPoId() const
	{
		return (m_poId);
	}

	void  NsmLocalVlagConfigMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32 NsmLocalVlagConfigMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

}

