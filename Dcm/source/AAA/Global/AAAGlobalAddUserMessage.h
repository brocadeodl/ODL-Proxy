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

#ifndef AAAGLOBALADDUSERMESSAGE_H
#define AAAGLOBALADDUSERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class AAAGlobalAddUserMessage : public PrismMessage
{
  protected:
	virtual void setupAttributesForSerialization();
  public:
	AAAGlobalAddUserMessage ();
	AAAGlobalAddUserMessage (const string &name, const string &password, const SI8 &encryptionLevel, const string &role, const string &desc, const bool &enabled, const bool &enabledF, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName);
	virtual    ~AAAGlobalAddUserMessage ();
	void setName(const string &name);
	string getName()  const;
	void setPassword(const string &password);
	string getPassword()  const;
	void setEncryptionLevel(const SI8 &encryptionLevel);
	SI8 getEncryptionLevel()  const;
	void setRole(const string &role);
	string getRole()  const;
	void setDescription(const string &desc);
	string getDescription()  const;
	void setEnabled(const bool &enabled);
	bool getEnabled()  const;
	void setEnabledFlag(const bool &enabledF);
	bool getEnabledFlag()  const;
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
	string  m_name;
	string  m_password;
	SI8  m_encryptionLevel;
	string  m_role;
	string  m_desc;
	bool  m_enabled;
	bool  m_enabledF;
	// loginUser is to pass to PAM to get MD5 Hash of the password successfully
	string m_loginUser;
	string m_loginRole;
	string m_loginIp;
	string m_loginProto;
	string m_loginAppName;
};

}
#endif //AAAGLOBALADDUSERMESSAGE_H
