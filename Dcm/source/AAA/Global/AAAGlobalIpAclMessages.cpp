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

#if 0
/***************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Kunduru                                           *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"

#include "AAA/Global/AAAGlobalIpAclMessages.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

AAAGlobalIpAclPolicyMessage::AAAGlobalIpAclPolicyMessage ()
    : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALIPACLPOLICY)
{
}

AAAGlobalIpAclPolicyMessage::AAAGlobalIpAclPolicyMessage (const string &name, const IpAclType &aclType, IpAclVersion aclVersion, const AAAGlobalMessageOperationType &opType)
    : PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALIPACLPOLICY),
	m_name(name),
	m_aclType(aclType),
	m_aclVersion(aclVersion),
	m_opType(opType)
{
}

AAAGlobalIpAclPolicyMessage::~AAAGlobalIpAclPolicyMessage ()
{
}

void AAAGlobalIpAclPolicyMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_aclType,"aclType"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_opType,"opType"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_aclVersion,"aclVersion"));
}

void AAAGlobalIpAclPolicyMessage::setName (const string &name)
{
	m_name = name;
}

string AAAGlobalIpAclPolicyMessage::getName ()
{
	return m_name;
}

void AAAGlobalIpAclPolicyMessage::setAclType (const IpAclType &aclType)
{
	m_aclType = aclType;
}

IpAclType AAAGlobalIpAclPolicyMessage::getAclType ()
{
	return m_aclType;
}

void AAAGlobalIpAclPolicyMessage::setOpType (const AAAGlobalMessageOperationType &opType)
{
	m_opType = opType;
}

AAAGlobalMessageOperationType AAAGlobalIpAclPolicyMessage::getOpType()
{
	return m_opType;
}


IpAclVersion AAAGlobalIpAclPolicyMessage::getAclVersion()
{
	return (m_aclVersion);
}

AAAGlobalIpAclRuleMessage::AAAGlobalIpAclRuleMessage (UI32 opCode)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), opCode)
{
}

AAAGlobalIpAclRuleMessage::AAAGlobalIpAclRuleMessage (const string &policyName, const UI64 &seqNum, const IpAclAction &action,	
								const string &srcHost, const IpAddrType &srcIpType, IpAclVersion aclVersion,
								const AAAGlobalMessageOperationType &opType, UI32 opCode)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), opCode),
	m_policyName(policyName),
	m_seqNum(seqNum),
	m_action(action),
	m_srcHost(srcHost),
	m_srcIpType(srcIpType),
	m_aclVersion(aclVersion),
	m_opType(opType)
{
}

AAAGlobalIpAclRuleMessage::~AAAGlobalIpAclRuleMessage ()
{
}

void AAAGlobalIpAclRuleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization();
	addSerializableAttribute (new AttributeString(&m_policyName,"policyName"));
	addSerializableAttribute (new AttributeUI64(&m_seqNum,"seqNum"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
	addSerializableAttribute (new AttributeString(&m_srcHost,"srcHost"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_srcIpType,"srcIpType"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_opType,"opType"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_aclVersion,"aclVersion"));
}

string AAAGlobalIpAclRuleMessage::getPolicyName()
{
	return m_policyName;
}

UI64 AAAGlobalIpAclRuleMessage::getSeqNum()
{
	return m_seqNum;
}

IpAclAction AAAGlobalIpAclRuleMessage::getAction()
{
	return m_action;
}

string AAAGlobalIpAclRuleMessage::getSrcHost()
{
	return m_srcHost;
}

IpAddrType  AAAGlobalIpAclRuleMessage::getSrcIpType()
{
	return (m_srcIpType);
}

AAAGlobalMessageOperationType AAAGlobalIpAclRuleMessage::getOpType()
{
	return m_opType;
}

IpAclVersion AAAGlobalIpAclRuleMessage::getAclVersion()
{
	return (m_aclVersion);
}

AAAGlobalStdIpAclRuleMessage::AAAGlobalStdIpAclRuleMessage ()
    : AAAGlobalIpAclRuleMessage (AAAGLOBALSTDIPACLRULE)
{
}

AAAGlobalStdIpAclRuleMessage::AAAGlobalStdIpAclRuleMessage (const string &policyName, const UI64 &seqNum,
											const IpAclAction &action, const string &srcHost,
											const IpAddrType &srcIpType, IpAclVersion aclVersion,
											const AAAGlobalMessageOperationType &opType)
	: AAAGlobalIpAclRuleMessage(policyName, seqNum, action, srcHost, srcIpType, aclVersion, opType, AAAGLOBALSTDIPACLRULE)
{
}

AAAGlobalStdIpAclRuleMessage::~AAAGlobalStdIpAclRuleMessage ()
{
}

void AAAGlobalStdIpAclRuleMessage::setupAttributesForSerialization()
{
	AAAGlobalIpAclRuleMessage::setupAttributesForSerialization ();
}




AAAGlobalExtIpAclRuleMessage::AAAGlobalExtIpAclRuleMessage ()
    : AAAGlobalIpAclRuleMessage (AAAGLOBALEXTIPACLRULE)
{
}

AAAGlobalExtIpAclRuleMessage::AAAGlobalExtIpAclRuleMessage (const string &policyName, const UI64 &seqNum,
						const IpAclAction &action, const string &srcHost,  const IpAddrType &srcIpType,
						IpAclVersion aclVersion, const AclProtocol &protocol, const IpAddrType &destIpType,
						const AclPortType &destPortType, const UI32 &destPort,
						const UI32 &destEndPort, const AAAGlobalMessageOperationType &opType)
	: AAAGlobalIpAclRuleMessage(policyName, seqNum, action, srcHost, srcIpType, aclVersion, opType, AAAGLOBALEXTIPACLRULE),
	m_proto(protocol),
	m_destIpType(destIpType),
	m_destPortType(destPortType),
	m_destPort(destPort),
	m_destEndPort(destEndPort)
{
}

AAAGlobalExtIpAclRuleMessage::~AAAGlobalExtIpAclRuleMessage ()
{
}

void AAAGlobalExtIpAclRuleMessage::setupAttributesForSerialization()
{
	AAAGlobalIpAclRuleMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeEnum((UI32*)&m_proto,"proto"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_destIpType, "destIpType"));
	addSerializableAttribute (new AttributeEnum((UI32*)&m_destPortType, "destPortType"));
	addSerializableAttribute (new AttributeUI32(&m_destPort, "destPort"));
	addSerializableAttribute (new AttributeUI32(&m_destEndPort, "destEndPort"));
}

AclProtocol AAAGlobalExtIpAclRuleMessage::getProtocol()
{
	return m_proto;
}

IpAddrType AAAGlobalExtIpAclRuleMessage::getDestIpType()
{
	return m_destIpType;
}

AclPortType AAAGlobalExtIpAclRuleMessage::getDestPortType()
{
	return m_destPortType;
}

UI32 AAAGlobalExtIpAclRuleMessage::getDestPort()
{
	return m_destPort;
}

UI32 AAAGlobalExtIpAclRuleMessage::getDestEndPort()
{
	return m_destEndPort;
}

}
#endif
