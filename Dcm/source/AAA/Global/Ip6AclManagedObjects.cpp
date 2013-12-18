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

#if 0 // use common MO
/***************************************************************************
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Kunduru                                           *
 **************************************************************************/

#include <Framework/ObjectModel/WaveManagedObjectPointer.cpp>
#include <Framework/Attributes/AttributeManagedObjectVectorComposition.cpp>
#include <Framework/Attributes/AttributeEnum.h>

#include "AAA/Global/Ip6AclManagedObjects.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "vcs.h"

using namespace WaveNs;

namespace DcmNs
{

Ip6AclRuleManagedObject::Ip6AclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (Ip6AclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager)
{
}

Ip6AclRuleManagedObject::Ip6AclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost, const IpAddrType &srcIpType, const WaveUnion &srcIpUnion)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (Ip6AclRuleManagedObject::getClassName (),
	DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	m_seqNum (seqNum),
	m_action (action),
	m_srcHost (IpV6Address(srcHost)),
	m_srcIpType (srcIpType),
	m_srcIpUnion (srcIpUnion)
{
}

Ip6AclRuleManagedObject::~Ip6AclRuleManagedObject ()
{
}

string  Ip6AclRuleManagedObject::getClassName()
{
	return ("Ip6AclRuleManagedObject");
}

void  Ip6AclRuleManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI64(&m_seqNum, "seqNum", ip_access_list_index));
	addPersistableAttribute (new AttributeEnum((UI32*)&m_action, "action", ip_access_list_action));
//	addPersistableAttribute (new AttributeEnum((UI32*)&m_srcAny, NO_WILD_CARD, "srcAny"));
	//addPersistableAttribute (new AttributeString(&m_srcHost, true, "", "srcHost", ip_access_list_srchost));
	addPersistableAttribute (new AttributeIpV6Address(&m_srcHost, "srcHost", ip_access_list_srchost6));
	//addPersistableAttribute (new AttributeString(&m_subnetAddr, "subnetAddr"));
//	addPersistableAttribute (new AttributeString(&m_srcMask, "srcMask"));
    //addPersistableAttribute (new AttributeUnion(&m_ipaddrUnion, "ipaddrUnion"));
    //addPersistableAttribute (new AttributeEnum((UI32 *)(&m_ipaddrType),"ipaddrType"));
    addPersistableAttribute (new AttributeUnion(&m_srcIpUnion, "srcIpUnion"));
    addPersistableAttribute (new AttributeEnum((UI32 *)(&m_srcIpType), IPV4_ADDR_UNDEFINED, "srcIpType"));
    setUserTagForAttribute ("srcIpUnion", ip_access_list_source);
}

void  Ip6AclRuleManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeUI64(&m_seqNum, "seqNum", ip_access_list_index));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_action, "action", ip_access_list_action));
	//addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_srcAny, NO_WILD_CARD, "srcAny"));
	addPersistableAttributeForCreate (new AttributeIpV6Address(&m_srcHost, "srcHost", ip_access_list_srchost6));
	//addPersistableAttributeForCreate (new AttributeString(&m_subnetAddr, "subnetAddr"));
	//addPersistableAttributeForCreate (new AttributeString(&m_srcMask, "srcMask"));
    addPersistableAttributeForCreate (new AttributeUnion(&m_srcIpUnion, "srcIpUnion"));
    addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_srcIpType), IPV4_ADDR_UNDEFINED, "srcIpType"));
}

void  Ip6AclRuleManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("seqNum");
	setUserDefinedKeyCombination (key);
}

void  Ip6AclRuleManagedObject::setSeqNum(const UI64 &seqNum)
{
	m_seqNum = seqNum;
}

UI64  Ip6AclRuleManagedObject::getSeqNum() const
{
	return (m_seqNum);
}

void  Ip6AclRuleManagedObject::setAction(const IpAclAction &action)
{
	m_action = action;
}

IpAclAction  Ip6AclRuleManagedObject::getAction() const
{
	return (m_action);
}

/*void  Ip6AclRuleManagedObject::setSrcAny(const AclWildCard &srcAny)
{
	m_srcAny = srcAny;
}

AclWildCard  Ip6AclRuleManagedObject::getSrcAny() const
{
	return (m_srcAny);
}*/

void Ip6AclRuleManagedObject::setSrcIpUnion(const WaveUnion &srcIpUnion)
{
	m_srcIpUnion = srcIpUnion;
}

WaveUnion Ip6AclRuleManagedObject::getSrcIpUnion() const
{
	return (m_srcIpUnion);
}

void  Ip6AclRuleManagedObject::setSrcIpType(const IpAddrType &srcIpType)
{
	m_srcIpType = srcIpType;
    m_srcIpUnion = "srcIpType";
}

IpAddrType  Ip6AclRuleManagedObject::getSrcIpType() const
{
	return (m_srcIpType);
}

void  Ip6AclRuleManagedObject::setSrcHost(const string &srcHost)
{
	m_srcHost = IpV6Address(srcHost);
}

string  Ip6AclRuleManagedObject::getSrcHost() const
{
	return (m_srcHost.toString());
}

#if 0
void  Ip6AclRuleManagedObject::setSrcAddr(const string &srcAddr)
{
	m_srcAddr = srcAddr;
}

string  Ip6AclRuleManagedObject::getSrcAddr() const
{
	return (m_srcAddr);
}
#endif

/*void  Ip6AclRuleManagedObject::setSubnetAddr(const string &subnetAddr)
{
	m_subnetAddr = subnetAddr;
     m_ipaddrUnion = "subnetAddr";
}

string  Ip6AclRuleManagedObject::getSubnetAddr() const
{
	return (m_subnetAddr);
}

void Ip6AclRuleManagedObject::setIp6addrUnion(const WaveUnion &val)
{
		m_ipaddrUnion = val;
}

WaveUnion Ip6AclRuleManagedObject::getIp6addrUnion() const
{
	return (m_ipaddrUnion);
}

void  Ip6AclRuleManagedObject::setIp6addrType(const AclWildCard &ipaddrType)
{
       m_ipaddrType  =  ipaddrType;
       m_ipaddrUnion = "ipaddrType";
} 

AclWildCard  Ip6AclRuleManagedObject::getIp6addrType() const
{
   return (m_ipaddrType);
}
*/
StdIp6AclRuleManagedObject::StdIp6AclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIp6AclRuleManagedObject::getClassName (),
	Ip6AclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclRuleManagedObject (pAAAObjectManager)
{
}

StdIp6AclRuleManagedObject::StdIp6AclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost, const IpAddrType &srcIpType, const WaveUnion &srcIpUnion)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIp6AclRuleManagedObject::getClassName (),
	Ip6AclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclRuleManagedObject (pAAAObjectManager, seqNum, action, srcHost, srcIpType, srcIpUnion)
{
}

StdIp6AclRuleManagedObject::~StdIp6AclRuleManagedObject ()
{
}

string  StdIp6AclRuleManagedObject::getClassName()
{
	return ("StdIp6AclRuleManagedObject");
}

void  StdIp6AclRuleManagedObject::setupAttributesForPersistence()
{
	Ip6AclRuleManagedObject::setupAttributesForPersistence();
}

void  StdIp6AclRuleManagedObject::setupAttributesForCreate()
{
	Ip6AclRuleManagedObject::setupAttributesForCreate();
}

ExtIp6AclRuleManagedObject::ExtIp6AclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIp6AclRuleManagedObject::getClassName (),
	Ip6AclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclRuleManagedObject (pAAAObjectManager)
{
}

ExtIp6AclRuleManagedObject::ExtIp6AclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost,  const IpAddrType &srcIpType, const WaveUnion &srcIpUnion, const AclProtocol &protocol, const IpAddrType &destIpType, const WaveUnion &destIpUnion, const AclPortType &destPortType, const UI32 &destPort, const UI32 &destEndPort)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIp6AclRuleManagedObject::getClassName (),
	Ip6AclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclRuleManagedObject (pAAAObjectManager, seqNum, action, srcHost, srcIpType, srcIpUnion),
	m_protocol    (protocol),
	m_destIpType    (destIpType),
	m_destIpUnion    (destIpUnion),
	m_destPortType (destPortType),
	m_destPort    (destPort),
	m_destEndPort    (destEndPort)

{
}

ExtIp6AclRuleManagedObject::~ExtIp6AclRuleManagedObject ()
{
}

string  ExtIp6AclRuleManagedObject::getClassName()
{
	return ("ExtIp6AclRuleManagedObject");
}

void  ExtIp6AclRuleManagedObject::setupAttributesForPersistence()
{
	Ip6AclRuleManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeEnum((UI32*)&m_protocol,"protocol", ip_access_list_protocol));
	//addPersistableAttribute (new AttributeEnum((UI32*)&m_destAddr,"destAddr", ip_access_list_dest_addr));
	//addPersistableAttribute (new AttributeUI16(&m_srcPort, "srcPort", ip_access_list_src_port));
  
	// Destination Port.
	addPersistableAttribute (new AttributeEnum((UI32 *)(&m_destPortType), ACL_PORT_UNDEFINED, "destPortType", ip_access_list_dest_op));
	addPersistableAttribute (new AttributeUI32(&m_destPort, IP_ACL_NO_PORT_VAL, "destPort", ip_access_list_dest_port));
	addPersistableAttribute (new AttributeUI32(&m_destEndPort, IP_ACL_NO_PORT_VAL, "destEndPort", ip_access_list_dest_end_port));
   
	//Destination IP Address.
	addPersistableAttribute (new AttributeUnion(&m_destIpUnion, "destIpUnion"));
    addPersistableAttribute (new AttributeEnum((UI32 *)(&m_destIpType), IPV4_ADDR_UNDEFINED, "destIpType"));
    setUserTagForAttribute ("destIpUnion", ip_access_list_dest);
}

void  ExtIp6AclRuleManagedObject::setupAttributesForCreate()
{
	Ip6AclRuleManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_protocol,"protocol", ip_access_list_protocol));
	//addPersistableAttributeForCreate (new AttributeUI16(&m_srcPort,  "srcPort", ip_access_list_src_port));
	// Destination Port.
	addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_destPortType), ACL_PORT_UNDEFINED, "destPortType", ip_access_list_dest_op));
	addPersistableAttributeForCreate (new AttributeUI32(&m_destPort, IP_ACL_NO_PORT_VAL, "destPort", ip_access_list_dest_port));
	addPersistableAttributeForCreate (new AttributeUI32(&m_destEndPort, IP_ACL_NO_PORT_VAL, "destEndPort", ip_access_list_dest_end_port));
   
	//Destination IP Address.
	addPersistableAttributeForCreate (new AttributeUnion(&m_destIpUnion, "destIpUnion"));
    addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_destIpType), IPV4_ADDR_UNDEFINED, "destIpType"));
}

void  ExtIp6AclRuleManagedObject::setProtocol(const AclProtocol &protocol)
{
	m_protocol = protocol;
}

AclProtocol  ExtIp6AclRuleManagedObject::getProtocol() const
{
	return (m_protocol);
}

/*void  ExtIp6AclRuleManagedObject::setDestAddr(const AclWildCard &destAddr)
{
	m_destAddr = destAddr;
}

AclWildCard  ExtIp6AclRuleManagedObject::getDestAddr() const
{
	return (m_destAddr);
}

void  ExtIp6AclRuleManagedObject::setSrcPort(const UI16 &srcPort)
{
	m_srcPort = srcPort;
}

UI16  ExtIp6AclRuleManagedObject::getSrcPort() const
{
	return (m_srcPort);
}*/

void  ExtIp6AclRuleManagedObject::setDestPort(const UI32 &destPort)
{
	m_destPort = destPort;
}

UI32  ExtIp6AclRuleManagedObject::getDestPort() const
{
	return (m_destPort);
}

void ExtIp6AclRuleManagedObject::setDestIpType(const IpAddrType	&destIpType) 
{
	m_destIpType = destIpType;
   	m_destIpUnion = "destIpType";
}

IpAddrType ExtIp6AclRuleManagedObject::getDestIpType() const
{
	return (m_destIpType);
}

void ExtIp6AclRuleManagedObject::setDestPortType(const AclPortType &destPortType)
{
		m_destPortType = destPortType;
}

AclPortType ExtIp6AclRuleManagedObject::getDestPortType() const
{
	return (m_destPortType);
}

void ExtIp6AclRuleManagedObject::setDestEndPort(const UI32 &destEndPort)
{
	m_destEndPort = destEndPort;	
}

UI32 ExtIp6AclRuleManagedObject::getDestEndPort()  const
{
	return (m_destEndPort);
}

Ip6AclPolicyManagedObject::Ip6AclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (Ip6AclPolicyManagedObject::getClassName (),
	DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager)
{
}

Ip6AclPolicyManagedObject::Ip6AclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (Ip6AclPolicyManagedObject::getClassName (),
	DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	m_name    (name)
{
}

Ip6AclPolicyManagedObject::~Ip6AclPolicyManagedObject ()
{
}

string  Ip6AclPolicyManagedObject::getClassName()
{
	return ("Ip6AclPolicyManagedObject");
}

void Ip6AclPolicyManagedObject::addRule(Ip6AclRuleManagedObject *ruleMO)
{
	trace (TRACE_LEVEL_ERROR, "Shouldn't be addind rule to base class");
	prismAssert(false, __FILE__, __LINE__);
}

int Ip6AclPolicyManagedObject::deleteRule(const UI64 &seqNum)
{
	trace (TRACE_LEVEL_ERROR, "Shouldn't be deleting rule from base class");
	prismAssert(false, __FILE__, __LINE__);
	return 0;
}

void  Ip6AclPolicyManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_name, "aclname"));
	addPersistableAttribute (new AttributeEnum((UI32*)&m_aclType, "aclType"));
	setUserTagForAttribute ("aclname", ip_access_list_policy_name);
}

void  Ip6AclPolicyManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_aclType, "aclType"));
	addPersistableAttributeForCreate (new AttributeString(&m_name, "aclname"));
}

void  Ip6AclPolicyManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("aclname");
	setUserDefinedKeyCombination (key);
}

void  Ip6AclPolicyManagedObject::setName(const string &name)
{
	m_name = name;
}

string  Ip6AclPolicyManagedObject::getName() const
{
	return (m_name);
}

void  Ip6AclPolicyManagedObject::setAclType(const IpAclType &aclType)
{
	m_aclType = aclType;
}

IpAclType  Ip6AclPolicyManagedObject::getAclType() const
{
	return (m_aclType);
}

StdIp6AclPolicyManagedObject::StdIp6AclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIp6AclPolicyManagedObject::getClassName (),
	Ip6AclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclPolicyManagedObject (pAAAObjectManager)
{
	setAclType(STANDARD_IP_ACL);
}

StdIp6AclPolicyManagedObject::StdIp6AclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name, const vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > &stdRules)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIp6AclPolicyManagedObject::getClassName (),
	Ip6AclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclPolicyManagedObject (pAAAObjectManager, name),
	m_stdRules    (stdRules)
{
	setAclType(STANDARD_IP_ACL);
}

StdIp6AclPolicyManagedObject::~StdIp6AclPolicyManagedObject ()
{
}

string  StdIp6AclPolicyManagedObject::getClassName()
{
	return ("StdIp6AclPolicyManagedObject");
}

void StdIp6AclPolicyManagedObject::addRule(Ip6AclRuleManagedObject *ruleMO)
{
	StdIp6AclRuleManagedObject *stdRule = dynamic_cast<StdIp6AclRuleManagedObject *>(ruleMO);
	WaveManagedObjectPointer<StdIp6AclRuleManagedObject> ruleObj (stdRule);
	m_stdRules.push_back(ruleObj);
}

int StdIp6AclPolicyManagedObject::deleteRule(const UI64 &seqNum)
{
	for (UI64 i = 0; i < m_stdRules.size(); i++)
	{
		if (m_stdRules[i]->getSeqNum() == seqNum)
		{
			m_stdRules.erase(m_stdRules.begin() + i);
			trace(TRACE_LEVEL_INFO, "Deleted the rule composition from StdIp6AclPolicyManagedObject");
			return (0);
		}
	}
	return (-1);
}

void  StdIp6AclPolicyManagedObject::setupAttributesForPersistence()
{
	Ip6AclPolicyManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeManagedObjectVectorComposition<StdIp6AclRuleManagedObject>(&m_stdRules,"stdRules", getClassName (), getObjectId (), StdIp6AclRuleManagedObject::getClassName ()));
}

void  StdIp6AclPolicyManagedObject::setupAttributesForCreate()
{
	Ip6AclPolicyManagedObject::setupAttributesForCreate();
	addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<StdIp6AclRuleManagedObject>(&m_stdRules,"stdRules", getClassName (), getObjectId (), StdIp6AclRuleManagedObject::getClassName ()));
}

void  StdIp6AclPolicyManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("aclname");
	setUserDefinedKeyCombination (key);
}

void  StdIp6AclPolicyManagedObject::setStdRules(const vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> > &stdRules)
{
	m_stdRules = stdRules;
}

vector<WaveManagedObjectPointer<StdIp6AclRuleManagedObject> >  StdIp6AclPolicyManagedObject::getStdRules() const
{
	return (m_stdRules);
}

ExtIp6AclPolicyManagedObject::ExtIp6AclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIp6AclPolicyManagedObject::getClassName (),
	Ip6AclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclPolicyManagedObject (pAAAObjectManager)
{
	setAclType(EXTENDED_IP_ACL);
}

ExtIp6AclPolicyManagedObject::ExtIp6AclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name, const vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > &extRules)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIp6AclPolicyManagedObject::getClassName (),
	Ip6AclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	Ip6AclPolicyManagedObject (pAAAObjectManager, name),
	m_extRules    (extRules)
{
	setAclType(EXTENDED_IP_ACL);
}

ExtIp6AclPolicyManagedObject::~ExtIp6AclPolicyManagedObject ()
{
}

string  ExtIp6AclPolicyManagedObject::getClassName()
{
	return ("ExtIp6AclPolicyManagedObject");
}

void ExtIp6AclPolicyManagedObject::addRule(Ip6AclRuleManagedObject *ruleMO)
{
	ExtIp6AclRuleManagedObject *extRule = dynamic_cast<ExtIp6AclRuleManagedObject *>(ruleMO);
	WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> ruleObj (extRule);
	m_extRules.push_back(ruleObj);
}

int ExtIp6AclPolicyManagedObject::deleteRule(const UI64 &seqNum)
{
	for (UI64 i = 0; i < m_extRules.size(); i++)
	{
		if (m_extRules[i]->getSeqNum() == seqNum)
		{
			m_extRules.erase(m_extRules.begin() + i);
			trace(TRACE_LEVEL_INFO, "Deleted the rule composition from ExtIp6AclPolicyManagedObject");
			return (0);
		}
	}
	return (-1);
}

void  ExtIp6AclPolicyManagedObject::setupAttributesForPersistence()
{
	Ip6AclPolicyManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeManagedObjectVectorComposition<ExtIp6AclRuleManagedObject>(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIp6AclRuleManagedObject::getClassName ()));
}

void  ExtIp6AclPolicyManagedObject::setupAttributesForCreate()
{
	Ip6AclPolicyManagedObject::setupAttributesForCreate();
	addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ExtIp6AclRuleManagedObject>(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIp6AclRuleManagedObject::getClassName ()));
}

void  ExtIp6AclPolicyManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("aclname");
	setUserDefinedKeyCombination (key);
}

void  ExtIp6AclPolicyManagedObject::setExtRules(const vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> > &extRules)
{
	m_extRules = extRules;
}

vector<WaveManagedObjectPointer<ExtIp6AclRuleManagedObject> >  ExtIp6AclPolicyManagedObject::getExtRules() const
{
	return (m_extRules);
}

}
#endif
