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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : vsatyana														*
 **************************************************************************/

#include "Nsm/Global/MacGroupConfigGlobalMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

	MacGroupConfigGlobalMessage::MacGroupConfigGlobalMessage ()
		: ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), MAC_GROUP_CONFIG_GLOBAL_MESSAGE)
	{
	}

	MacGroupConfigGlobalMessage::~MacGroupConfigGlobalMessage ()
	{
	}

	void MacGroupConfigGlobalMessage::setupAttributesForSerialization()
	{
		ManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId"));
		addSerializableAttribute (new AttributeString(&m_mac, "mac"));
		addSerializableAttribute (new AttributeString(&m_mask, "mask"));
	}

	void MacGroupConfigGlobalMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode = cmdCode;
	}

	UI32 MacGroupConfigGlobalMessage::getCmdCode() const
	{
		return m_cmdCode;
	}

	void MacGroupConfigGlobalMessage::setMacGroupId(const UI32 &macGroupId)
	{
		m_macGroupId = macGroupId;
	}

	UI32 MacGroupConfigGlobalMessage::getMacGroupId() const
	{
		return m_macGroupId;
	}

	void MacGroupConfigGlobalMessage::setMac(const string &mac)
	{
		m_mac = mac;
	}

	string MacGroupConfigGlobalMessage::getMac() const
	{
		return m_mac;
	}

	void MacGroupConfigGlobalMessage::setMask(const string &mask)
	{
		m_mask = mask;
	}

	string MacGroupConfigGlobalMessage::getMask() const
	{
		return m_mask;
	}
}



