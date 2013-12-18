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

#include "Framework/Attributes/AttributeEnum.h"

#include "Ssm/Global/SsmGlobalIp6AclPolicyMessage.h"
#include "Ssm/Global/SsmGlobalObjectManager.h"
#include "Ssm/Global/SsmGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

SsmGlobalIp6AclPolicyMessage::SsmGlobalIp6AclPolicyMessage ()
    : PrismMessage (SsmGlobalObjectManager::getPrismServiceId (), SSMGLOBALIP6ACLPOLICY)
{
}

SsmGlobalIp6AclPolicyMessage::SsmGlobalIp6AclPolicyMessage (const string &name, const Ip6AclType &aclType, const SsmGlobalMessageOperationType &opType)
    : PrismMessage (SsmGlobalObjectManager::getPrismServiceId (), SSMGLOBALIP6ACLPOLICY),
	m_name(name),
	m_aclType(aclType),
//	m_aclVersion(aclVersion),
	m_opType(opType)
{
}

SsmGlobalIp6AclPolicyMessage::~SsmGlobalIp6AclPolicyMessage ()
{
}

void SsmGlobalIp6AclPolicyMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_aclType,"aclType"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_opType,"opType"));
//	addSerializableAttribute (new AttributeEnum((UI32*)&m_aclVersion,"aclVersion"));
}

void SsmGlobalIp6AclPolicyMessage::setName (const string &name)
{
	m_name = name;
}

string SsmGlobalIp6AclPolicyMessage::getName ()
{
	return m_name;
}

void SsmGlobalIp6AclPolicyMessage::setAclType (const Ip6AclType &aclType)
{
	m_aclType = aclType;
}

Ip6AclType SsmGlobalIp6AclPolicyMessage::getAclType ()
{
	return m_aclType;
}

void SsmGlobalIp6AclPolicyMessage::setOpType (const SsmGlobalMessageOperationType &opType)
{
	m_opType = opType;
}

SsmGlobalMessageOperationType SsmGlobalIp6AclPolicyMessage::getOpType()
{
	return m_opType;
}

/*
IpAclVersion SsmGlobalIp6AclPolicyMessage::getAclVersion()
{
	return (m_aclVersion);
}
*/
}
