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

#include "SnmpServer/Global/SnmpServerSnmpConfigSetUserMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"

namespace DcmNs
{

    SnmpServerSnmpConfigSetUserMessage::SnmpServerSnmpConfigSetUserMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGSETUSER)
    {
    }

    SnmpServerSnmpConfigSetUserMessage::SnmpServerSnmpConfigSetUserMessage (const string &username,const string &groupname,const AuthType &auth,const string &authpswd,const PrivType &priv,const string &privpswd,const bool &encpswdflag)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGSETUSER),
        m_username    (username),
        m_groupname    (groupname),
        m_auth    (auth),
        m_authpswd    (authpswd),
        m_priv    (priv),
        m_privpswd    (privpswd),
        m_encpswdflag (encpswdflag)
    {
    }

    SnmpServerSnmpConfigSetUserMessage::~SnmpServerSnmpConfigSetUserMessage ()
    {
    }

    void  SnmpServerSnmpConfigSetUserMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_username,"username"));
        addSerializableAttribute (new AttributeString(&m_groupname,"groupname", snmp_server_groupname));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_auth,"auth", snmp_server_auth));
        addSerializableAttribute (new AttributeString(&m_authpswd,"authpswd", snmp_server_auth_password));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_priv,"priv", snmp_server_priv));
        addSerializableAttribute (new AttributeString(&m_privpswd,"privpswd", snmp_server_priv_password));
        addSerializableAttribute (new AttributeBool(&m_encpswdflag,"encrypted", snmp_server_encrypted));
    }

    void  SnmpServerSnmpConfigSetUserMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerSnmpConfigSetUserMessage::getUsername() const
    {
        return (m_username);
    }

    void  SnmpServerSnmpConfigSetUserMessage::setGroupname(const string &groupname)
    {
        m_groupname  =  groupname;
    }

    string  SnmpServerSnmpConfigSetUserMessage::getGroupname() const
    {
        return (m_groupname);
    }

    void  SnmpServerSnmpConfigSetUserMessage::setAuth(const AuthType &auth)
    {
        m_auth  =  auth;
    }

    AuthType  SnmpServerSnmpConfigSetUserMessage::getAuth() const
    {
        return (m_auth);
    }

    void  SnmpServerSnmpConfigSetUserMessage::setAuthpswd(const string &authpswd)
    {
        m_authpswd  =  authpswd;
    }

    string  SnmpServerSnmpConfigSetUserMessage::getAuthpswd() const
    {
        return (m_authpswd);
    }

    void  SnmpServerSnmpConfigSetUserMessage::setPriv(const PrivType &priv)
    {
        m_priv  =  priv;
    }

    PrivType  SnmpServerSnmpConfigSetUserMessage::getPriv() const
    {
        return (m_priv);
    }

    void  SnmpServerSnmpConfigSetUserMessage::setPrivpswd(const string &privpswd)
    {
        m_privpswd  =  privpswd;
    }

    string  SnmpServerSnmpConfigSetUserMessage::getPrivpswd() const
    {
        return (m_privpswd);
    }

    void  SnmpServerSnmpConfigSetUserMessage::setEncpswdflag(const bool &encpswdflag)
    {
        m_encpswdflag	=  encpswdflag;
    }

    bool SnmpServerSnmpConfigSetUserMessage::getEncpswdflag() const
    {
        return (m_encpswdflag);
    }

}
