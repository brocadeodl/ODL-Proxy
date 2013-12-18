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
 *   Author : Charanjith Kunduru                                           *
 **************************************************************************/

#include "AAA/Local/AAALocalIpAclDeleteMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

AAALocalIpAclDeleteMessage::AAALocalIpAclDeleteMessage ()
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALIPACLDELETE)
{
}

AAALocalIpAclDeleteMessage::AAALocalIpAclDeleteMessage (const string &switchId, const string &policyName, const IpAclVersion &aclVersion)
    : PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALIPACLDELETE),
	m_switchId(switchId),
	m_policyName(policyName),
	m_aclVersion(aclVersion)

{
}

AAALocalIpAclDeleteMessage::~AAALocalIpAclDeleteMessage ()
{
}

void AAALocalIpAclDeleteMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_switchId,"switchId"));
	addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_aclVersion,"aclVersion"));
}

void AAALocalIpAclDeleteMessage::setSwitchId (const string &switchId)
{
	m_switchId = switchId;
}

string AAALocalIpAclDeleteMessage::getSwitchId ()
{
	return m_switchId;
}

void AAALocalIpAclDeleteMessage::setPolicyName (const string &policyName)
{
	m_policyName = policyName;
}

string AAALocalIpAclDeleteMessage::getPolicyName ()
{
	return m_policyName;
}

void AAALocalIpAclDeleteMessage::setAclVersion(const IpAclVersion &aclVersion)
{
	m_aclVersion = aclVersion; 
}

IpAclVersion AAALocalIpAclDeleteMessage::getAclVersion()
{
	return (m_aclVersion);
}
}
