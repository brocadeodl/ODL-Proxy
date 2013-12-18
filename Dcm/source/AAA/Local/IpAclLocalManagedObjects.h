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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Kumar Ghanta		                                           *
 **************************************************************************/

#ifndef IPACLLOCALMANAGEDOBJECTS_H
#define IPACLLOCALMANAGEDOBJECTS_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"

#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;
namespace DcmNs
{

class AAALocalLocalObjectManager;

class IpAclPolicyLocalManagedObject : virtual public DcmLocalManagedObject
{
  /* Data members */
  private:
	string		m_switchid;
	string			m_ipAclPolicy;
	ACLTrafficType	m_ipAclTrafFlow;
	UI32		m_ipAclType;

  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	IpAclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAAObjectManager);
    IpAclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAALocalObjectManager, const string &switchid, const string  &ipAclPolicy, const ACLTrafficType  &ipAclTrafFlow, const UI32 &ipAclType);
	virtual    ~IpAclPolicyLocalManagedObject ();
    static string  getClassName();
	void setSwitchId(const string &switchid);
	string getSwitchId() const;
	void setIpAclPolicy (const string &ipAcl);
	string getIpAclPolicy() const;
	void setIpAclTrafFlow(const ACLTrafficType &trafFlow);
	ACLTrafficType getIpAclTrafFlow();
	void setIpAclType(const UI32 &ipAclType);
	UI32 getIpAclType();
};

class Ip6AclPolicyLocalManagedObject : virtual public DcmLocalManagedObject
{
  /* Data members */
  private:
	string		m_switchid;
	string			m_ipv6AclPolicy;
	ACLTrafficType	m_ipv6AclTrafFlow;
	UI32        m_ipv6AclType;

  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	Ip6AclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAAObjectManager);
    Ip6AclPolicyLocalManagedObject (AAALocalLocalObjectManager *pAAALocalObjectManager, const string &switchid, const string &ipv6AclPolicy, const ACLTrafficType &ipv6AclTrafFlow, const UI32 &ipv6AclType);
	virtual    ~Ip6AclPolicyLocalManagedObject ();
    static string  getClassName();
	void setSwitchId(const string &switchid);
	string getSwitchId() const;
	void setIpv6AclPolicy (const string &ipv6Acl);
	string getIpv6AclPolicy() const;
	void setIpv6AclTrafFlow(const ACLTrafficType &trafFlow);
	ACLTrafficType getIpv6AclTrafFlow();
	void setIpv6AclType(const UI32 &ipv6AclType);
	UI32 getIpv6AclType();
};

}
#endif                                            // AAAMANAGEDOBJECTS_H
