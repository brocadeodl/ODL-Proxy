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
 *   Author : Venkat														*
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/SwitchportVlanConfigLocalMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

SwitchportVlanConfigLocalMessage::SwitchportVlanConfigLocalMessage ()
  : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), SWITCHPORT_VLAN_CONFIG_LOCAL_MESSAGE)
{
}
   
void SwitchportVlanConfigLocalMessage::setupAttributesForSerialization ()
{
	PrismMessage::setupAttributesForSerialization ();

	addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "cmdCode"));
	addSerializableAttribute (new AttributeUI32 (&m_vlanId, "vlanId"));
	addSerializableAttribute (new AttributeUI32 (&m_ctagId, "ctagId"));
	addSerializableAttribute (new AttributeString (&m_mac, "mac"));
	addSerializableAttribute (new AttributeString (&m_ifName, "ifName"));
	addSerializableAttribute (new AttributeUI32 (&m_macGroupId, "macGroupId"));
}

SwitchportVlanConfigLocalMessage::~SwitchportVlanConfigLocalMessage ()
{
}

UI32 SwitchportVlanConfigLocalMessage::getCmdCode () const
{
	return m_cmdCode;
}

void SwitchportVlanConfigLocalMessage::setCmdCode (const UI32 &cmdCode)
{
	m_cmdCode = cmdCode;
}

UI32 SwitchportVlanConfigLocalMessage::getVlanId () const
{
	return m_vlanId;
}

void SwitchportVlanConfigLocalMessage::setVlanId (const UI32 &vlanId)
{
	m_vlanId = vlanId;
}

UI32 SwitchportVlanConfigLocalMessage::getCtagId () const
{
	return m_ctagId;
}

void SwitchportVlanConfigLocalMessage::setCtagId (const UI32 &ctagId)
{
	m_ctagId = ctagId;
}

UI32 SwitchportVlanConfigLocalMessage::getMacGroupId () const
{
	return m_macGroupId;
}

void SwitchportVlanConfigLocalMessage::setMacGroupId (const UI32 &macGroupId)
{
	m_macGroupId = macGroupId;
}

string SwitchportVlanConfigLocalMessage::getMac () const
{
	return m_mac;
}

void SwitchportVlanConfigLocalMessage::setMac (const string &mac)
{
	m_mac = mac;
}

string SwitchportVlanConfigLocalMessage::getIfName () const
{
	return m_ifName;
}

void SwitchportVlanConfigLocalMessage::setIfName (const string &ifName)
{
	m_ifName = ifName;
}

}

