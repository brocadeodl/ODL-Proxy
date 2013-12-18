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

#ifndef SSMGLOBALIP6ACLPOLICYMESSAGE_H
#define SSMGLOBALIP6ACLPOLICYMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Ssm/Global/SsmGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class SsmGlobalIp6AclPolicyMessage : public PrismMessage
{
  /* Data members*/
  private:
	string m_name;
	Ip6AclType m_aclType;
//	IpAclVersion m_aclVersion;
	SsmGlobalMessageOperationType m_opType;

  /* Member functions */
  protected:
	virtual void setupAttributesForSerialization();
  public:
	SsmGlobalIp6AclPolicyMessage ();
	SsmGlobalIp6AclPolicyMessage (const string &name, const Ip6AclType &aclType, const SsmGlobalMessageOperationType &opType);
	virtual ~SsmGlobalIp6AclPolicyMessage();
	void setName(const string &name);
	string getName();
	void setAclType(const Ip6AclType &aclType);
	Ip6AclType getAclType();
	void setOpType(const SsmGlobalMessageOperationType &opType);
	SsmGlobalMessageOperationType getOpType();

};

}
#endif
