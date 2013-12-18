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

#ifndef AAAGLOBALIPACLMESSAGES_H
#define AAAGLOBALIPACLMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

/*class AAAGenericMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGenericMessage ();
	virtual ~AAAGenericMessage();
	void setOpType(const AAAGlobalMessageOperationType &opType);
	AAAGlobalMessageOperationType getOpType();

  private:
	AAAGlobalMessageOperationType m_opType;
};*/

class AAAGlobalIpAclPolicyMessage : public PrismMessage
{
  /* Data members*/
  private:
	string m_name;
	IpAclType m_aclType;
	IpAclVersion m_aclVersion;
	AAAGlobalMessageOperationType m_opType;

  /* Member functions */
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalIpAclPolicyMessage ();
	AAAGlobalIpAclPolicyMessage (const string &name, const IpAclType &aclType, IpAclVersion aclVersion, const AAAGlobalMessageOperationType &opType);
	virtual ~AAAGlobalIpAclPolicyMessage();
	void setName(const string &name);
	string getName();
	void setAclType(const IpAclType &aclType);
	IpAclType getAclType();
	IpAclVersion getAclVersion();
	void setOpType(const AAAGlobalMessageOperationType &opType);
	AAAGlobalMessageOperationType getOpType();

};


class AAAGlobalIpAclRuleMessage : public PrismMessage
{
  /* Data members*/
  private:
	string m_policyName;
	UI64 m_seqNum;
	IpAclAction	m_action;
	string  m_srcHost;
	//string  m_srcSubnetAddr; //Future
	IpAddrType	m_srcIpType;
	IpAclVersion m_aclVersion;
	AAAGlobalMessageOperationType m_opType;

  /* Member functions */
  protected:
	AAAGlobalIpAclRuleMessage (UI32 opCode);
	AAAGlobalIpAclRuleMessage (const string &policyName, const UI64 &seqNum, const IpAclAction &action,	
								const string &srcHost, const IpAddrType &srcIpType, IpAclVersion aclVersion,
								const AAAGlobalMessageOperationType &opType, UI32 opCode);
	virtual void setupAttributesForSerialization();
  public:
	virtual ~AAAGlobalIpAclRuleMessage();
	string getPolicyName();
	UI64 getSeqNum();
	IpAclAction getAction();
	IpAddrType  getSrcIpType();
	string getSrcHost();
	IpAclVersion getAclVersion();
	AAAGlobalMessageOperationType getOpType();
};

class AAAGlobalStdIpAclRuleMessage : public AAAGlobalIpAclRuleMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalStdIpAclRuleMessage ();
	AAAGlobalStdIpAclRuleMessage (const string &policyName, const UI64 &seqNum, const IpAclAction &action,	
								const string &srcHost, const IpAddrType &srcIpType, IpAclVersion aclVersion,
								const AAAGlobalMessageOperationType &opType);
	virtual ~AAAGlobalStdIpAclRuleMessage();
};

class AAAGlobalExtIpAclRuleMessage : public AAAGlobalIpAclRuleMessage//, public PrismMessage
{
  /* Data members*/
  private:
	AclProtocol m_proto;
	IpAddrType  m_destIpType;
	AclPortType m_destPortType;
	UI32 m_destPort;
	UI32 m_destEndPort;

  /* Member functions */
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalExtIpAclRuleMessage ();
	AAAGlobalExtIpAclRuleMessage (const string &policyName, const UI64 &seqNum, const IpAclAction &action,
						const string &srcHost,  const IpAddrType &srcIpType, IpAclVersion aclVersion,
						const AclProtocol &protocol, const IpAddrType &m_destIpType, 
						const AclPortType &m_destPortType, const UI32 &m_destPort,
						const UI32 &m_destEndPort, const AAAGlobalMessageOperationType &opType);
	virtual ~AAAGlobalExtIpAclRuleMessage();
	AclProtocol getProtocol();
	//SI8 getDestAddr();
	IpAddrType getDestIpType();
	AclPortType getDestPortType();
	UI32 getDestPort();
	UI32 getDestEndPort();

};

}

#endif
#endif
