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

#if 0 // use common ACL MO
/***************************************************************************
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Kunduru                                           *
 **************************************************************************/

#ifndef IPACLMANAGEDOBJECTS_H
#define IPACLMANAGEDOBJECTS_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Types/IpV4Address.h"

#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

class AAAGlobalObjectManager;

class IpAclRuleManagedObject : virtual public DcmManagedObject
{
  /* Data Members */
  private:
	UI64  m_seqNum;
	IpAclAction  m_action;
	/*
	 * <srcAny>, <srcHost> and <srcAddr, srcMask>
	 * are mutually exclusive
	 */
	//Source IP address. 
	IpV4Address  m_srcHost;
	//string  m_srcSubnetAddr; //Future
	IpAddrType	m_srcIpType;
    WaveUnion	m_srcIpUnion;

  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();
  public:
	IpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager);
	IpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost, const IpAddrType &srcIpType, const WaveUnion &srcIpUnion);
	virtual ~IpAclRuleManagedObject ();
	static string getClassName();

	void setSeqNum(const UI64 &seqNum);
	UI64 getSeqNum()  const;
	void setAction(const IpAclAction &action);
	IpAclAction getAction()  const;

	void setSrcIpUnion(const WaveUnion &srcIpUnion);
	WaveUnion getSrcIpUnion() const;
	void  setSrcIpType(const IpAddrType &srcIpType);
	IpAddrType  getSrcIpType() const;

	void setSrcHost(const string &srcHost);
	string getSrcHost()  const;
/*	void setSubnetAddr(const string &subnetAddr);
	string getSubnetAddr()  const;*/
};

class StdIpAclRuleManagedObject : virtual public IpAclRuleManagedObject
{
  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	StdIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager);
	StdIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost, const IpAddrType &srcIpType, const WaveUnion &srcIpUnion);
	virtual ~StdIpAclRuleManagedObject ();
	static string getClassName();
};

class ExtIpAclRuleManagedObject : virtual public IpAclRuleManagedObject
{
  /* Data members */
  private:
	AclProtocol m_protocol;
	//string  m_srcSubnetAddr; //Future
	IpAddrType	m_destIpType;
    WaveUnion	m_destIpUnion;

	// No source port for now. Added for future reference.
	//AclPort m_srcPortEnum;
	//UI16 m_srcPort;
	//UI16 m_srcPortEnd;

	//Destination port
	AclPortType m_destPortType;
	UI32 m_destPort;
	UI32 m_destEndPort;


  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
  public:
	ExtIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager);
	ExtIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost,  const IpAddrType &srcIpType, const WaveUnion &srcIpUnion, const AclProtocol &protocol, const IpAddrType &m_destIpType, const WaveUnion &m_destIpUnion, const AclPortType &m_destPortType, const UI32 &m_destPort, const UI32 &m_destEndPort);
	virtual ~ExtIpAclRuleManagedObject ();
	static string getClassName();

	void setProtocol(const AclProtocol &protocol);
	AclProtocol getProtocol()  const;
/*	void setDestAddr(const AclWildCard &destAddr);
	AclWildCard getDestAddr()  const;
	void setSrcPort(const UI16 &srcPort);
	UI16 getSrcPort()  const;*/
	void setDestIpType(const IpAddrType	&destIpType);
	IpAddrType getDestIpType() const;
    void setDestIpUnion(const WaveUnion	&m_destIpUnion);
	WaveUnion getDestIpUnion() const;
	void setDestPortType(const AclPortType &destPortType);
	AclPortType getDestPortType() const;
	void setDestPort(const UI32 &destPort);
	UI32 getDestPort()  const;
	void setDestEndPort(const UI32 &destEndPort);
	UI32 getDestEndPort()  const;
};



class IpAclPolicyManagedObject : virtual public DcmManagedObject
{
  /* Data members */
  private:
	string  m_name;
	IpAclType m_aclType;

  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();
  public:
	IpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager);
	IpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager,const string &name);
	virtual    ~IpAclPolicyManagedObject ();
	static string getClassName();
	virtual void addRule(IpAclRuleManagedObject *ruleMO);
	virtual int deleteRule(const UI64 &seqNum);
	void setName(const string &name);
	string getName()  const;
	void setAclType(const IpAclType &aclType);
	IpAclType getAclType() const;
};

class StdIpAclPolicyManagedObject : virtual public IpAclPolicyManagedObject
{
  /* Data members */
  private:
	vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> >  m_stdRules;

  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();
  public:
	StdIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager);
	StdIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name, const vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > &stdRules);
	virtual    ~StdIpAclPolicyManagedObject ();
	static string getClassName();
	virtual void addRule(IpAclRuleManagedObject *ruleMO);
	virtual int deleteRule(const UI64 &seqNum);
	void setStdRules(const vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > &stdRules);
	vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > getStdRules()  const;
};

class ExtIpAclPolicyManagedObject : virtual public IpAclPolicyManagedObject
{
  /* Data members */
  private:
	vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> >  m_extRules;

  /* Member functions */
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();
  public:
	ExtIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager);
	ExtIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name, const vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > &extRules);
	virtual    ~ExtIpAclPolicyManagedObject ();
	static string getClassName();
	virtual void addRule(IpAclRuleManagedObject *ruleMO);
	virtual int deleteRule(const UI64 &seqNum);
	void setExtRules(const vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > &extRules);
	vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > getExtRules()  const;
};

}
#endif                                            // AAAMANAGEDOBJECTS_H
#endif
