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

#ifndef ROLEMANAGEDOBJECT_H
#define ROLEMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalObjectManager;

class RoleManagedObject : virtual public DcmManagedObject
{
  protected:
	virtual void setupAttributesForPersistence();
	virtual void setupAttributesForCreate();
	virtual void setupKeys();

  public:
	RoleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager);
	RoleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &name, const string &description, const string &users, const UI32 &gid);
	virtual    ~RoleManagedObject ();
	static string getClassName();
	void setName(const string &name);
	string getName()  const;
	void setDescription(const string &description);
	string getDescription()  const;

	void setUsers(const string &users);
	string getUsers()  const;
	void setGID(const UI32 &gid);
	UI32 getGID()  const;

  /* Data members */
  private:
	string  m_name;
	string  m_description;

	string  m_users;
	UI32 m_gid;
};

}
#endif	//ROLEMANAGEDOBJECT_H
