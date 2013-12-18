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

#ifndef AAALOCALCHANGEROLEMESSAGE_H
#define AAALOCALCHANGEROLEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAALocalChangeRoleMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();

  public:
	AAALocalChangeRoleMessage ();
	AAALocalChangeRoleMessage (const string &name, const string &description, const bool &descFlag, const string &users, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName);
	virtual    ~AAALocalChangeRoleMessage ();
	void setName(const string &name);
	string getName()  const;
	void setDescription(const string &description);
	string getDescription()  const;
	void setDescFlag(const bool &descF);
	bool getDescFlag()  const;
	void setUsers(const string &users);
	string getUsers()  const;
	void setLoginUser(const string &loginUser);
	string getLoginUser()  const;
	void setLoginRole(const string &loginRole);
	string getLoginRole()  const;
	void setLoginIp(const string &loginIp);
	string getLoginIp()  const;
	void setLoginProto(const string &loginProto);
	string getLoginProto()  const;
	void setLoginAppName(const string &loginAppName);
	string getLoginAppName()  const;

  /* Data members */
  private:
	string m_name;
	string m_description;
    bool   m_descF;
	string m_users;
	string  m_loginUser;
	string  m_loginRole;
	string m_loginIp;
	string m_loginProto;
	string m_loginAppName;
};

}
#endif	//AAALOCALCHANGEROLEMESSAGE_H
