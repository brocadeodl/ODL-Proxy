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

#include "SnmpServer/Global/SnmpServerSnmpConfigAddUserMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"

#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"

#include <snmp/snmplib.h>

namespace DcmNs
{

    SnmpServerSnmpConfigAddUserMessage::SnmpServerSnmpConfigAddUserMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDUSER)
    {
    }

    SnmpServerSnmpConfigAddUserMessage::SnmpServerSnmpConfigAddUserMessage (const string &username,const string &groupname,const AuthType &auth,const string &authpswd,const PrivType &priv,const string &privpswd,const bool &encpswdflag /*, const bool &authpswdflag*/)
		: PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDUSER),
        m_username    (username),
        m_groupname    (groupname),
        m_auth    (auth),
        m_authpswd    (authpswd),
        m_priv    (priv),
        m_privpswd    (privpswd),
        m_encpswdflag (encpswdflag)
    {
    }

    SnmpServerSnmpConfigAddUserMessage::~SnmpServerSnmpConfigAddUserMessage ()
    {
    }

    void  SnmpServerSnmpConfigAddUserMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_username,"username", snmp_server_username));
        addSerializableAttribute (new AttributeString(&m_groupname,"groupname", snmp_server_groupname));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_auth,"auth", snmp_server_auth));
        addSerializableAttribute (new AttributeString(&m_authpswd,"authpswd", snmp_server_auth_password));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_priv,"priv", snmp_server_priv));
        addSerializableAttribute (new AttributeString(&m_privpswd,"privpswd", snmp_server_priv_password));
        addSerializableAttribute (new AttributeBool(m_encpswdflag,"encrypted", snmp_server_encrypted));
    }

    void  SnmpServerSnmpConfigAddUserMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerSnmpConfigAddUserMessage::getUsername() const
    {
        return (m_username);
    }

    void  SnmpServerSnmpConfigAddUserMessage::setGroupname(const string &groupname)
    {
        m_groupname  =  groupname;
    }

    string  SnmpServerSnmpConfigAddUserMessage::getGroupname() const
    {
        return (m_groupname);
    }

    void  SnmpServerSnmpConfigAddUserMessage::setAuth(const AuthType &auth)
    {
        m_auth  =  auth;
    }

    AuthType  SnmpServerSnmpConfigAddUserMessage::getAuth() const
    {
        return (m_auth);
    }

    void  SnmpServerSnmpConfigAddUserMessage::setAuthpswd(const string &authpswd)
    {
        m_authpswd  =  authpswd;
    }

    string  SnmpServerSnmpConfigAddUserMessage::getAuthpswd() const
    {
        return (m_authpswd);
    }

    void  SnmpServerSnmpConfigAddUserMessage::setPriv(const PrivType &priv)
    {
        m_priv  =  priv;
    }

    PrivType  SnmpServerSnmpConfigAddUserMessage::getPriv() const
    {
        return (m_priv);
    }

    void  SnmpServerSnmpConfigAddUserMessage::setPrivpswd(const string &privpswd)
    {
        m_privpswd  =  privpswd;
    }

    string  SnmpServerSnmpConfigAddUserMessage::getPrivpswd() const
    {
        return (m_privpswd);
    }
	
    void  SnmpServerSnmpConfigAddUserMessage::setEncpswdflag(const bool &encpswdflag)
    {
        m_encpswdflag	=  encpswdflag;
    }

    bool SnmpServerSnmpConfigAddUserMessage::getEncpswdflag() const
    {
        return (m_encpswdflag);
    }

}
