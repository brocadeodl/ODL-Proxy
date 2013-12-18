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

#ifndef USERMANAGEDOBJECT_H
#define USERMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class UserManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();

  public:
	UserManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	UserManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &name, const string &password, const AAAEncryptionLevel &encryptionLevel, const ObjectId &role, const string &description, const bool &enabled, const string &md5password, const SI32 &uid, const SI32 &gid, const string &ssh_keydir, const string &homedir);
	virtual    ~UserManagedObject ();
	static string getClassName();
	void setName(const string &name);
	string getName()  const;
	void setPassword(const string &password);
	string getPassword()  const;
	void setEncryptionLevel(const AAAEncryptionLevel &encryptionLevel);
	AAAEncryptionLevel getEncryptionLevel()  const;
	void setRole(const ObjectId &role);
	ObjectId getRole()  const;
	void setDescription(const string &description);
	string getDescription()  const;
	void setEnabled(const bool &enabled);
	bool getEnabled()  const;

	void setMD5Password(const string &md5password);
	string getMD5Password()  const;
	void setUid(const SI32 &uid);
	SI32 getUid()  const;
	void setGid(const SI32 &gid);
	SI32 getGid()  const;
	void setSsh_keydir(const string &ssh_keydir);
	string getSsh_keydir()  const;
	void setHomedir(const string &homedir);
	string getHomedir()  const;

  /* Data members */
  private:
	string  m_name;
	string  m_password;
	AAAEncryptionLevel  m_encryptionLevel;
	ObjectId  m_role;
	string  m_description;
	bool  m_enabled;

	string  m_md5password;
	SI32  m_uid;
	SI32  m_gid;
	string  m_ssh_keydir;
	string  m_homedir;
};

}
#endif	//USERMANAGEDOBJECT_H
