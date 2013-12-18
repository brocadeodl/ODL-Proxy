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

#include "Framework/Attributes/AttributeEnum.h"

#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Global/RoleManagedObject.h"


#include "brocade-aaa.h"

using namespace WaveNs;

namespace DcmNs
{

UserManagedObject::UserManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (UserManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

UserManagedObject::UserManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &name, const string &password, const AAAEncryptionLevel &encryptionLevel, const ObjectId &role, const string &description, const bool &enabled, const string &md5password, const SI32 &uid, const SI32 &gid, const string &ssh_keydir, const string &homedir)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (UserManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_name    (name),
	m_password    (password),
	m_encryptionLevel    (encryptionLevel),
	m_role   (role),
	m_description    (description),
	m_enabled    (enabled),
	m_md5password    (md5password),
	m_uid    (uid),
	m_gid    (gid),
	m_ssh_keydir    (ssh_keydir),
	m_homedir    (homedir)
{
}

UserManagedObject::~UserManagedObject ()
{
}

string  UserManagedObject::getClassName()
{
	return ("UserManagedObject");
}

void  UserManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_name,"username", brcd_aaa_name));
	addPersistableAttribute (new AttributeString(&m_password,"password", brcd_aaa_user_password));
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_encryptionLevel),"encryption_level", brcd_aaa_encryption_level));
	addPersistableAttribute (new AttributeObjectIdAssociation(&m_role,"role", RoleManagedObject::getClassName (), false, brcd_aaa_role));
	addPersistableAttribute (new AttributeString(&m_description,"description", brcd_aaa_desc));
	addPersistableAttribute (new AttributeBool(&m_enabled,"enabled", brcd_aaa_enable));
}

void  UserManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeString(&m_name,"username", brcd_aaa_name));
	addPersistableAttributeForCreate (new AttributeString(&m_password,"password", brcd_aaa_user_password));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_encryptionLevel),"encryption_level", brcd_aaa_encryption_level));
	addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_role,"role", RoleManagedObject::getClassName (), false, brcd_aaa_role));
	addPersistableAttributeForCreate (new AttributeString(&m_description,"description", brcd_aaa_desc));
	addPersistableAttributeForCreate (new AttributeBool(&m_enabled,"enabled", brcd_aaa_enable));
}

void UserManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("username");
	setUserDefinedKeyCombination (key);
}

void  UserManagedObject::setName(const string &name)
{
	m_name  =  name;
}

string  UserManagedObject::getName() const
{
	return (m_name);
}

void  UserManagedObject::setPassword(const string &password)
{
	m_password  =  password;
}

string  UserManagedObject::getPassword() const
{
	return (m_password);
}

void  UserManagedObject::setEncryptionLevel(const AAAEncryptionLevel &encryptionLevel)
{
	m_encryptionLevel  =  encryptionLevel;
}

AAAEncryptionLevel  UserManagedObject::getEncryptionLevel() const
{
	return (m_encryptionLevel);
}

void  UserManagedObject::setRole(const ObjectId &role)
{
	m_role  =  role;
}

ObjectId  UserManagedObject::getRole() const
{
	return (m_role);
}

void  UserManagedObject::setDescription(const string &description)
{
	m_description  =  description;
}

string  UserManagedObject::getDescription() const
{
	return (m_description);
}

void  UserManagedObject::setEnabled(const bool &enabled)
{
	m_enabled  =  enabled;
}

bool  UserManagedObject::getEnabled() const
{
	return (m_enabled);
}

void  UserManagedObject::setMD5Password(const string &md5password)
{
	m_md5password  =  md5password;
}

string  UserManagedObject::getMD5Password() const
{
	return (m_md5password);
}

void  UserManagedObject::setUid(const SI32 &uid)
{
	m_uid  =  uid;
}

SI32  UserManagedObject::getUid() const
{
	return (m_uid);
}

void  UserManagedObject::setGid(const SI32 &gid)
{
	m_gid  =  gid;
}

SI32  UserManagedObject::getGid() const
{
	return (m_gid);
}

void  UserManagedObject::setSsh_keydir(const string &ssh_keydir)
{
	m_ssh_keydir  =  ssh_keydir;
}

string  UserManagedObject::getSsh_keydir() const
{
	return (m_ssh_keydir);
}

void  UserManagedObject::setHomedir(const string &homedir)
{
	m_homedir  =  homedir;
}

string  UserManagedObject::getHomedir() const
{
	return (m_homedir);
}

}
