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
#include "Nsm/Local/MacGroupConfigLocalMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

MacGroupConfigLocalMessage::MacGroupConfigLocalMessage ()
  : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), MAC_GROUP_CONFIG_LOCAL_MESSAGE)
{
}
   
void MacGroupConfigLocalMessage::setupAttributesForSerialization ()
{
	PrismMessage::setupAttributesForSerialization ();

	addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "cmdCode"));
	addSerializableAttribute (new AttributeUI32 (&m_macGroupId, "macGroupId"));
	addSerializableAttribute (new AttributeString(&m_mac, "mac"));
	addSerializableAttribute (new AttributeString(&m_mask, "mask"));
}

MacGroupConfigLocalMessage::~MacGroupConfigLocalMessage ()
{
}

UI32 MacGroupConfigLocalMessage::getCmdCode () const
{
	return m_cmdCode;
}

void MacGroupConfigLocalMessage::setCmdCode (const UI32 &cmdCode)
{
	m_cmdCode = cmdCode;
}

UI32 MacGroupConfigLocalMessage::getMacGroupId () const
{
	return m_macGroupId;
}

void MacGroupConfigLocalMessage::setMacGroupId (const UI32 &macGroupId)
{
	m_macGroupId = macGroupId;
}

void MacGroupConfigLocalMessage::setMac(const string &mac)
{
	m_mac = mac;
}

string MacGroupConfigLocalMessage::getMac() const
{
	return m_mac;
}

void MacGroupConfigLocalMessage::setMask(const string &mask)
{
	m_mask = mask;
}

string MacGroupConfigLocalMessage::getMask() const
{
	return m_mask;
}
}



