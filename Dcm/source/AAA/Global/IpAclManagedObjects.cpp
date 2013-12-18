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

#include "AAA/Global/IpAclManagedObjects.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "vcs.h"

using namespace WaveNs;

namespace DcmNs
{

IpAclRuleManagedObject::IpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (IpAclRuleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager)
{
}

IpAclRuleManagedObject::IpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost, const IpAddrType &srcIpType, const WaveUnion &srcIpUnion)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (IpAclRuleManagedObject::getClassName (),
	DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	m_seqNum (seqNum),
	m_action (action),
	m_srcHost (IpV4Address(srcHost)),
	m_srcIpType (srcIpType),
	m_srcIpUnion (srcIpUnion)
{
}

IpAclRuleManagedObject::~IpAclRuleManagedObject ()
{
}

string  IpAclRuleManagedObject::getClassName()
{
	return ("IpAclRuleManagedObject");
}

void  IpAclRuleManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI64(&m_seqNum, "seqNum", ip_access_list_index));
	addPersistableAttribute (new AttributeEnum((UI32*)&m_action, "action", ip_access_list_action));
//	addPersistableAttribute (new AttributeEnum((UI32*)&m_srcAny, NO_WILD_CARD, "srcAny"));
	//addPersistableAttribute (new AttributeString(&m_srcHost, true, "", "srcHost", ip_access_list_srchost));
	addPersistableAttribute (new AttributeIpV4Address(&m_srcHost, "srcHost", ip_access_list_srchost));
	//addPersistableAttribute (new AttributeString(&m_subnetAddr, "subnetAddr"));
//	addPersistableAttribute (new AttributeString(&m_srcMask, "srcMask"));
    //addPersistableAttribute (new AttributeUnion(&m_ipaddrUnion, "ipaddrUnion"));
    //addPersistableAttribute (new AttributeEnum((UI32 *)(&m_ipaddrType),"ipaddrType"));
    addPersistableAttribute (new AttributeUnion(&m_srcIpUnion, "srcIpUnion"));
    addPersistableAttribute (new AttributeEnum((UI32 *)(&m_srcIpType), IPV4_ADDR_UNDEFINED, "srcIpType"));
    setUserTagForAttribute ("srcIpUnion", ip_access_list_source);
}

void  IpAclRuleManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeUI64(&m_seqNum, "seqNum", ip_access_list_index));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_action, "action", ip_access_list_action));
	//addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_srcAny, NO_WILD_CARD, "srcAny"));
	addPersistableAttributeForCreate (new AttributeIpV4Address(&m_srcHost, "srcHost", ip_access_list_srchost));
	//addPersistableAttributeForCreate (new AttributeString(&m_subnetAddr, "subnetAddr"));
	//addPersistableAttributeForCreate (new AttributeString(&m_srcMask, "srcMask"));
    addPersistableAttributeForCreate (new AttributeUnion(&m_srcIpUnion, "srcIpUnion"));
    addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_srcIpType), IPV4_ADDR_UNDEFINED, "srcIpType"));
}

void  IpAclRuleManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("seqNum");
	setUserDefinedKeyCombination (key);
}

void  IpAclRuleManagedObject::setSeqNum(const UI64 &seqNum)
{
	m_seqNum = seqNum;
}

UI64  IpAclRuleManagedObject::getSeqNum() const
{
	return (m_seqNum);
}

void  IpAclRuleManagedObject::setAction(const IpAclAction &action)
{
	m_action = action;
}

IpAclAction  IpAclRuleManagedObject::getAction() const
{
	return (m_action);
}

/*void  IpAclRuleManagedObject::setSrcAny(const AclWildCard &srcAny)
{
	m_srcAny = srcAny;
}

AclWildCard  IpAclRuleManagedObject::getSrcAny() const
{
	return (m_srcAny);
}*/

void IpAclRuleManagedObject::setSrcIpUnion(const WaveUnion &srcIpUnion)
{
	m_srcIpUnion = srcIpUnion;
}

WaveUnion IpAclRuleManagedObject::getSrcIpUnion() const
{
	return (m_srcIpUnion);
}

void  IpAclRuleManagedObject::setSrcIpType(const IpAddrType &srcIpType)
{
	m_srcIpType = srcIpType;
    m_srcIpUnion = "srcIpType";
}

IpAddrType  IpAclRuleManagedObject::getSrcIpType() const
{
	return (m_srcIpType);
}

void  IpAclRuleManagedObject::setSrcHost(const string &srcHost)
{
	m_srcHost = IpV4Address(srcHost);
}

string  IpAclRuleManagedObject::getSrcHost() const
{
	return (m_srcHost.toString());
}

#if 0
void  IpAclRuleManagedObject::setSrcAddr(const string &srcAddr)
{
	m_srcAddr = srcAddr;
}

string  IpAclRuleManagedObject::getSrcAddr() const
{
	return (m_srcAddr);
}
#endif

/*void  IpAclRuleManagedObject::setSubnetAddr(const string &subnetAddr)
{
	m_subnetAddr = subnetAddr;
     m_ipaddrUnion = "subnetAddr";
}

string  IpAclRuleManagedObject::getSubnetAddr() const
{
	return (m_subnetAddr);
}

void IpAclRuleManagedObject::setIpaddrUnion(const WaveUnion &val)
{
		m_ipaddrUnion = val;
}

WaveUnion IpAclRuleManagedObject::getIpaddrUnion() const
{
	return (m_ipaddrUnion);
}

void  IpAclRuleManagedObject::setIpaddrType(const AclWildCard &ipaddrType)
{
       m_ipaddrType  =  ipaddrType;
       m_ipaddrUnion = "ipaddrType";
} 

AclWildCard  IpAclRuleManagedObject::getIpaddrType() const
{
   return (m_ipaddrType);
}
*/
StdIpAclRuleManagedObject::StdIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIpAclRuleManagedObject::getClassName (),
	IpAclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclRuleManagedObject (pAAAObjectManager)
{
}

StdIpAclRuleManagedObject::StdIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost, const IpAddrType &srcIpType, const WaveUnion &srcIpUnion)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIpAclRuleManagedObject::getClassName (),
	IpAclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclRuleManagedObject (pAAAObjectManager, seqNum, action, srcHost, srcIpType, srcIpUnion)
{
}

StdIpAclRuleManagedObject::~StdIpAclRuleManagedObject ()
{
}

string  StdIpAclRuleManagedObject::getClassName()
{
	return ("StdIpAclRuleManagedObject");
}

void  StdIpAclRuleManagedObject::setupAttributesForPersistence()
{
	IpAclRuleManagedObject::setupAttributesForPersistence();
}

void  StdIpAclRuleManagedObject::setupAttributesForCreate()
{
	IpAclRuleManagedObject::setupAttributesForCreate();
}

ExtIpAclRuleManagedObject::ExtIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIpAclRuleManagedObject::getClassName (),
	IpAclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclRuleManagedObject (pAAAObjectManager)
{
}

ExtIpAclRuleManagedObject::ExtIpAclRuleManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const UI64 &seqNum, const IpAclAction &action, const string &srcHost,  const IpAddrType &srcIpType, const WaveUnion &srcIpUnion, const AclProtocol &protocol, const IpAddrType &destIpType, const WaveUnion &destIpUnion, const AclPortType &destPortType, const UI32 &destPort, const UI32 &destEndPort)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIpAclRuleManagedObject::getClassName (),
	IpAclRuleManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclRuleManagedObject (pAAAObjectManager, seqNum, action, srcHost, srcIpType, srcIpUnion),
	m_protocol    (protocol),
	m_destIpType    (destIpType),
	m_destIpUnion    (destIpUnion),
	m_destPortType (destPortType),
	m_destPort    (destPort),
	m_destEndPort    (destEndPort)

{
}

ExtIpAclRuleManagedObject::~ExtIpAclRuleManagedObject ()
{
}

string  ExtIpAclRuleManagedObject::getClassName()
{
	return ("ExtIpAclRuleManagedObject");
}

void  ExtIpAclRuleManagedObject::setupAttributesForPersistence()
{
	IpAclRuleManagedObject::setupAttributesForPersistence ();
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

void  ExtIpAclRuleManagedObject::setupAttributesForCreate()
{
	IpAclRuleManagedObject::setupAttributesForCreate ();
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

void  ExtIpAclRuleManagedObject::setProtocol(const AclProtocol &protocol)
{
	m_protocol = protocol;
}

AclProtocol  ExtIpAclRuleManagedObject::getProtocol() const
{
	return (m_protocol);
}

/*void  ExtIpAclRuleManagedObject::setDestAddr(const AclWildCard &destAddr)
{
	m_destAddr = destAddr;
}

AclWildCard  ExtIpAclRuleManagedObject::getDestAddr() const
{
	return (m_destAddr);
}

void  ExtIpAclRuleManagedObject::setSrcPort(const UI16 &srcPort)
{
	m_srcPort = srcPort;
}

UI16  ExtIpAclRuleManagedObject::getSrcPort() const
{
	return (m_srcPort);
}*/

void  ExtIpAclRuleManagedObject::setDestPort(const UI32 &destPort)
{
	m_destPort = destPort;
}

UI32  ExtIpAclRuleManagedObject::getDestPort() const
{
	return (m_destPort);
}

void ExtIpAclRuleManagedObject::setDestIpType(const IpAddrType	&destIpType) 
{
	m_destIpType = destIpType;
   	m_destIpUnion = "destIpType";
}

IpAddrType ExtIpAclRuleManagedObject::getDestIpType() const
{
	return (m_destIpType);
}

/*void ExtIpAclRuleManagedObject::setDestIpUnion(const WaveUnion	&m_destIpUnion)
{
	m_destIpUnion = destIpUnion;
}

WaveUnion ExtIpAclRuleManagedObject::getDestIpUnion() const
{
	return (m_destIpUnion);
}*/

void ExtIpAclRuleManagedObject::setDestPortType(const AclPortType &destPortType)
{
		m_destPortType = destPortType;
}

AclPortType ExtIpAclRuleManagedObject::getDestPortType() const
{
	return (m_destPortType);
}

void ExtIpAclRuleManagedObject::setDestEndPort(const UI32 &destEndPort)
{
	m_destEndPort = destEndPort;	
}

UI32 ExtIpAclRuleManagedObject::getDestEndPort()  const
{
	return (m_destEndPort);
}

IpAclPolicyManagedObject::IpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (IpAclPolicyManagedObject::getClassName (),
	DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager)
{
}

IpAclPolicyManagedObject::IpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (IpAclPolicyManagedObject::getClassName (),
	DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	m_name    (name)
{
}

IpAclPolicyManagedObject::~IpAclPolicyManagedObject ()
{
}

string  IpAclPolicyManagedObject::getClassName()
{
	return ("IpAclPolicyManagedObject");
}

void IpAclPolicyManagedObject::addRule(IpAclRuleManagedObject *ruleMO)
{
	trace (TRACE_LEVEL_ERROR, "Shouldn't be addind rule to base class");
	prismAssert(false, __FILE__, __LINE__);
}

int IpAclPolicyManagedObject::deleteRule(const UI64 &seqNum)
{
	trace (TRACE_LEVEL_ERROR, "Shouldn't be deleting rule from base class");
	prismAssert(false, __FILE__, __LINE__);
	return 0;
}

void  IpAclPolicyManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_name, "aclname"));
	addPersistableAttribute (new AttributeEnum((UI32*)&m_aclType, "aclType"));
	setUserTagForAttribute ("aclname", ip_access_list_policy_name);
}

void  IpAclPolicyManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_aclType, "aclType"));
	addPersistableAttributeForCreate (new AttributeString(&m_name, "aclname"));
}

void  IpAclPolicyManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("aclname");
	setUserDefinedKeyCombination (key);
}

void  IpAclPolicyManagedObject::setName(const string &name)
{
	m_name = name;
}

string  IpAclPolicyManagedObject::getName() const
{
	return (m_name);
}

void  IpAclPolicyManagedObject::setAclType(const IpAclType &aclType)
{
	m_aclType = aclType;
}

IpAclType  IpAclPolicyManagedObject::getAclType() const
{
	return (m_aclType);
}

StdIpAclPolicyManagedObject::StdIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIpAclPolicyManagedObject::getClassName (),
	IpAclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclPolicyManagedObject (pAAAObjectManager)
{
	setAclType(STANDARD_IP_ACL);
}

StdIpAclPolicyManagedObject::StdIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name, const vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > &stdRules)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (StdIpAclPolicyManagedObject::getClassName (),
	IpAclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclPolicyManagedObject (pAAAObjectManager, name),
	m_stdRules    (stdRules)
{
	setAclType(STANDARD_IP_ACL);
}

StdIpAclPolicyManagedObject::~StdIpAclPolicyManagedObject ()
{
}

string  StdIpAclPolicyManagedObject::getClassName()
{
	return ("StdIpAclPolicyManagedObject");
}

void StdIpAclPolicyManagedObject::addRule(IpAclRuleManagedObject *ruleMO)
{
	StdIpAclRuleManagedObject *stdRule = dynamic_cast<StdIpAclRuleManagedObject *>(ruleMO);
	WaveManagedObjectPointer<StdIpAclRuleManagedObject> ruleObj (stdRule);
	m_stdRules.push_back(ruleObj);
}

int StdIpAclPolicyManagedObject::deleteRule(const UI64 &seqNum)
{
	for (UI64 i = 0; i < m_stdRules.size(); i++)
	{
		if (m_stdRules[i]->getSeqNum() == seqNum)
		{
			m_stdRules.erase(m_stdRules.begin() + i);
			trace(TRACE_LEVEL_INFO, "Deleted the rule composition from StdIpAclPolicyManagedObject");
			return (0);
		}
	}
	return (-1);
}

void  StdIpAclPolicyManagedObject::setupAttributesForPersistence()
{
	IpAclPolicyManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeManagedObjectVectorComposition<StdIpAclRuleManagedObject>(&m_stdRules,"stdRules", getClassName (), getObjectId (), StdIpAclRuleManagedObject::getClassName ()));
}

void  StdIpAclPolicyManagedObject::setupAttributesForCreate()
{
	IpAclPolicyManagedObject::setupAttributesForCreate();
	addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<StdIpAclRuleManagedObject>(&m_stdRules,"stdRules", getClassName (), getObjectId (), StdIpAclRuleManagedObject::getClassName ()));
}

void  StdIpAclPolicyManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("aclname");
	setUserDefinedKeyCombination (key);
}

void  StdIpAclPolicyManagedObject::setStdRules(const vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> > &stdRules)
{
	m_stdRules = stdRules;
}

vector<WaveManagedObjectPointer<StdIpAclRuleManagedObject> >  StdIpAclPolicyManagedObject::getStdRules() const
{
	return (m_stdRules);
}

ExtIpAclPolicyManagedObject::ExtIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIpAclPolicyManagedObject::getClassName (),
	IpAclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclPolicyManagedObject (pAAAObjectManager)
{
	setAclType(EXTENDED_IP_ACL);
}

ExtIpAclPolicyManagedObject::ExtIpAclPolicyManagedObject (AAAGlobalObjectManager *pAAAObjectManager, const string &name, const vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > &extRules)
	: PrismElement  (pAAAObjectManager),
	PrismPersistableObject (ExtIpAclPolicyManagedObject::getClassName (),
	IpAclPolicyManagedObject::getClassName ()),
	WaveManagedObject (pAAAObjectManager),
	DcmManagedObject (pAAAObjectManager),
	IpAclPolicyManagedObject (pAAAObjectManager, name),
	m_extRules    (extRules)
{
	setAclType(EXTENDED_IP_ACL);
}

ExtIpAclPolicyManagedObject::~ExtIpAclPolicyManagedObject ()
{
}

string  ExtIpAclPolicyManagedObject::getClassName()
{
	return ("ExtIpAclPolicyManagedObject");
}

void ExtIpAclPolicyManagedObject::addRule(IpAclRuleManagedObject *ruleMO)
{
	ExtIpAclRuleManagedObject *extRule = dynamic_cast<ExtIpAclRuleManagedObject *>(ruleMO);
	WaveManagedObjectPointer<ExtIpAclRuleManagedObject> ruleObj (extRule);
	m_extRules.push_back(ruleObj);
}

int ExtIpAclPolicyManagedObject::deleteRule(const UI64 &seqNum)
{
	for (UI64 i = 0; i < m_extRules.size(); i++)
	{
		if (m_extRules[i]->getSeqNum() == seqNum)
		{
			m_extRules.erase(m_extRules.begin() + i);
			trace(TRACE_LEVEL_INFO, "Deleted the rule composition from ExtIpAclPolicyManagedObject");
			return (0);
		}
	}
	return (-1);
}

void  ExtIpAclPolicyManagedObject::setupAttributesForPersistence()
{
	IpAclPolicyManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeManagedObjectVectorComposition<ExtIpAclRuleManagedObject>(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIpAclRuleManagedObject::getClassName ()));
}

void  ExtIpAclPolicyManagedObject::setupAttributesForCreate()
{
	IpAclPolicyManagedObject::setupAttributesForCreate();
	addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<ExtIpAclRuleManagedObject>(&m_extRules,"extRules", getClassName (), getObjectId (), ExtIpAclRuleManagedObject::getClassName ()));
}

void  ExtIpAclPolicyManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("aclname");
	setUserDefinedKeyCombination (key);
}

void  ExtIpAclPolicyManagedObject::setExtRules(const vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> > &extRules)
{
	m_extRules = extRules;
}

vector<WaveManagedObjectPointer<ExtIpAclRuleManagedObject> >  ExtIpAclPolicyManagedObject::getExtRules() const
{
	return (m_extRules);
}

}
#endif //
