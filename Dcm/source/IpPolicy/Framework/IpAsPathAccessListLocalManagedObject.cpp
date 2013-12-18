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
 *   Copyright (C) 2010-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "IpAsPathAccessListLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

IpAsPathAccessListLocalManagedObject::IpAsPathAccessListLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
	: PrismElement  (pIpPolicyLocalObjectManager),
	PrismPersistableObject (IpAsPathAccessListLocalManagedObject::getClassName (),
	DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pIpPolicyLocalObjectManager),
	DcmManagedObject (pIpPolicyLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pIpPolicyLocalObjectManager)
{
}

IpAsPathAccessListLocalManagedObject::IpAsPathAccessListLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager,const string &name,
	const UI32 &seq,
	const UI16 &instance,
	const PolicyAction &action,
	const string &regExpr)
	: PrismElement  (pIpPolicyLocalObjectManager),
	PrismPersistableObject (IpAsPathAccessListLocalManagedObject::getClassName (),
	DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pIpPolicyLocalObjectManager),
	DcmManagedObject (pIpPolicyLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pIpPolicyLocalObjectManager),
	m_seq       (seq),
	m_instance    (instance),
	m_action    (action),
	m_regExpr    (regExpr)
{
}

IpAsPathAccessListLocalManagedObject::~IpAsPathAccessListLocalManagedObject ()
{
}

string  IpAsPathAccessListLocalManagedObject::getClassName()
{
	return ("IpAsPathAccessListLocalManagedObject");
}

void  IpAsPathAccessListLocalManagedObject::setupAttributesForPersistence()
{
	DcmLocalManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeEnum(&m_seq,"Seq"));
	addPersistableAttribute (new AttributeUI16(&m_instance,"instance"));
	addPersistableAttribute (new AttributeEnum((UI32*)&m_action,"action"));
	addPersistableAttribute (new AttributeString(&m_regExpr,"regExpr"));

	setUserTagForAttribute ("name", ip_policy_name);
	setUserTagForAttribute ("Seq", ip_policy_seq_keyword);
	setUserTagForAttribute ("instance", ip_policy_instance);
	setUserTagForAttribute("action", ip_policy_ip_action);
	setUserTagForAttribute("regExpr", ip_policy_ip_reg_expr);
}

void  IpAsPathAccessListLocalManagedObject::setupAttributesForCreate()
{
	DcmLocalManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate  (new AttributeEnum(&m_seq,"Seq"));
	addPersistableAttributeForCreate  (new AttributeUI16(&m_instance,"instance"));
	addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_action,"action"));
	addPersistableAttributeForCreate  (new AttributeString(&m_regExpr,"regExpr"));
}

void  IpAsPathAccessListLocalManagedObject::setupKeys() {
	vector<string> keyName;
	keyName.push_back("name");
	keyName.push_back("Seq");
	keyName.push_back("instance");
	setUserDefinedKeyCombination (keyName);
}

void  IpAsPathAccessListLocalManagedObject::setInstance(const UI16 &instance)
{
	m_seq = 1;
	m_instance = instance;
}

UI16  IpAsPathAccessListLocalManagedObject::getInstance() const
{
	return (m_instance);
}

void  IpAsPathAccessListLocalManagedObject::setAction(const PolicyAction &action)
{
	m_action = action;
}

PolicyAction  IpAsPathAccessListLocalManagedObject::getAction() const
{
	return (m_action);
}

void  IpAsPathAccessListLocalManagedObject::setRegExpr(const string &regExpr)
{
	m_regExpr = regExpr;
}

string  IpAsPathAccessListLocalManagedObject::getRegExpr() const
{
	return (m_regExpr);
}

}
