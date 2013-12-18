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

#include "Nsm/Global/SwitchportVlanConfigGlobalMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

	SwitchportVlanConfigGlobalMessage::SwitchportVlanConfigGlobalMessage ()
		: ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), SWITCHPORT_VLAN_CONFIG_GLOBAL_MESSAGE)
	{
	}

	SwitchportVlanConfigGlobalMessage::~SwitchportVlanConfigGlobalMessage ()
	{
	}

	void SwitchportVlanConfigGlobalMessage::setupAttributesForSerialization()
	{
		ManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_vlanId, "vlanId"));
		addSerializableAttribute (new AttributeUI32(&m_ctagId, "ctagId"));
		addSerializableAttribute (new AttributeString(&m_mac, "mac"));
		addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId"));
	}

	void SwitchportVlanConfigGlobalMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode = cmdCode;
	}

	UI32 SwitchportVlanConfigGlobalMessage::getCmdCode() const
	{
		return m_cmdCode;
	}

	void SwitchportVlanConfigGlobalMessage::setVlanId(const UI32 &vlanId)
	{
		m_vlanId = vlanId;
	}

	UI32 SwitchportVlanConfigGlobalMessage::getVlanId() const
	{
		return m_vlanId;
	}

	void SwitchportVlanConfigGlobalMessage::setCtagId(const UI32 &ctagId)
	{
		m_ctagId = ctagId;
	}

	UI32 SwitchportVlanConfigGlobalMessage::getCtagId() const
	{
		return m_ctagId;
	}

	void SwitchportVlanConfigGlobalMessage::setMac(const string &mac)
	{
		m_mac = mac;
	}

	string SwitchportVlanConfigGlobalMessage::getMac() const
	{
		return m_mac;
	}

	void SwitchportVlanConfigGlobalMessage::setIfName(const string &ifName)
	{
		m_ifName = ifName;
	}

	string SwitchportVlanConfigGlobalMessage::getIfName() const
	{
		return m_ifName;
	}

	void SwitchportVlanConfigGlobalMessage::setMacGroupId(const UI32 &macGroupId)
	{
		m_macGroupId = macGroupId;
	}

	UI32 SwitchportVlanConfigGlobalMessage::getMacGroupId() const
	{
		return m_macGroupId;
	}

}

