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
 *   Author : ckunduru                                                     *
 **************************************************************************/

#include "AAA/Global/RoleManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"


namespace DcmNs
{

RoleManagedObject::RoleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (RoleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

RoleManagedObject::RoleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &name, const string &description, const string &users, const UI32 &gid)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (RoleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_name    (name),
	m_description    (description),
	m_users    (users),
	m_gid (gid)
{
}

RoleManagedObject::~RoleManagedObject ()
{
}

string  RoleManagedObject::getClassName()
{
	return ("RoleManagedObject");
}

void  RoleManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_name,"rolename", brcd_aaa_name));
	addPersistableAttribute (new AttributeString(&m_description,"description", brcd_aaa_desc));
}

void  RoleManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeString(&m_name,"rolename", brcd_aaa_name));
	addPersistableAttributeForCreate (new AttributeString(&m_description,"description", brcd_aaa_desc));
}

void RoleManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("rolename");
	setUserDefinedKeyCombination (key);
}

void  RoleManagedObject::setName(const string &name)
{
	m_name  =  name;
}

string  RoleManagedObject::getName() const
{
	return (m_name);
}

void  RoleManagedObject::setDescription(const string &description)
{
	m_description  =  description;
}

string  RoleManagedObject::getDescription() const
{
	return (m_description);
}

void  RoleManagedObject::setUsers(const string &users)
{
	m_users  =  users;
}

string  RoleManagedObject::getUsers() const
{
	return (m_users);
}

void  RoleManagedObject::setGID(const UI32 &gid)
{
	m_gid  =  gid;
}

UI32  RoleManagedObject::getGID() const
{
	return (m_gid);
}

}
