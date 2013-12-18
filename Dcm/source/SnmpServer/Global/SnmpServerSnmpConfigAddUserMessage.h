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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SNMPSERVERSNMPCONFIGADDUSERMESSAGE_H
#define SNMPSERVERSNMPCONFIGADDUSERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "SnmpServer/Global/enumdatatype.h"
//#include "snmp.h"
#include "vcs.h"
using namespace WaveNs;

namespace DcmNs
{

    class SnmpServerSnmpConfigAddUserMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SnmpServerSnmpConfigAddUserMessage ();
            SnmpServerSnmpConfigAddUserMessage (const string &username,const string &groupname,const AuthType &auth,const string &authpswd,const PrivType &priv,const string &privpswd,const bool &encpswdflag);
            virtual    ~SnmpServerSnmpConfigAddUserMessage ();
            void setUsername(const string &username);
            string getUsername()  const;
            void setGroupname(const string &groupname);
            string getGroupname()  const;
            void setAuth(const AuthType &auth);
            AuthType getAuth()  const;
            void setAuthpswd(const string &authpswd);
            string getAuthpswd()  const;
            void setPriv(const PrivType &priv);
            PrivType getPriv()  const;
            void setPrivpswd(const string &privpswd);
            string getPrivpswd()  const;
			void setEncpswdflag(const bool &encpswdflag);	
			bool getEncpswdflag() const;

// Now the data members

        private:
        protected:
        public:
            string  m_username;
            string  m_groupname;
            AuthType  m_auth;
            string  m_authpswd;
            PrivType  m_priv;
            string  m_privpswd;
			bool m_encpswdflag;
    };
}
#endif                                            //SNMPSERVERSNMPCONFIGADDUSERMESSAGE_H
