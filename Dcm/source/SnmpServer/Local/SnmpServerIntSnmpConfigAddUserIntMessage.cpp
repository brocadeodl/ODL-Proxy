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

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddUserIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigAddUserIntMessage::SnmpServerIntSnmpConfigAddUserIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGADDUSERINT)
    {
    }

    SnmpServerIntSnmpConfigAddUserIntMessage::SnmpServerIntSnmpConfigAddUserIntMessage (const string &username,const string &groupname,const AuthType &auth,const string &authpswd,const PrivType &priv,const string &privpswd,const bool &encpswdflag)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGADDUSERINT),
        m_username    (username),
        m_groupname    (groupname),
        m_auth    (auth),
        m_authpswd    (authpswd),
        m_priv    (priv),
        m_privpswd    (privpswd),
        m_encpswdflag (encpswdflag)
    {
    }

    SnmpServerIntSnmpConfigAddUserIntMessage::~SnmpServerIntSnmpConfigAddUserIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_username,"username"));
        addSerializableAttribute (new AttributeString(&m_groupname,"groupname"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_auth,"auth"));
        addSerializableAttribute (new AttributeString(&m_authpswd,"authpswd"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_priv,"priv"));
        addSerializableAttribute (new AttributeString(&m_privpswd,"privpswd"));
        addSerializableAttribute (new AttributeBool(&m_encpswdflag,"encpswdflag"));
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerIntSnmpConfigAddUserIntMessage::getUsername() const
    {
        return (m_username);
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setGroupname(const string &groupname)
    {
        m_groupname  =  groupname;
    }

    string  SnmpServerIntSnmpConfigAddUserIntMessage::getGroupname() const
    {
        return (m_groupname);
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setAuth(const AuthType &auth)
    {
        m_auth  =  auth;
    }

    AuthType  SnmpServerIntSnmpConfigAddUserIntMessage::getAuth() const
    {
        return (m_auth);
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setAuthpswd(const string &authpswd)
    {
        m_authpswd  =  authpswd;
    }

    string  SnmpServerIntSnmpConfigAddUserIntMessage::getAuthpswd() const
    {
        return (m_authpswd);
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setPriv(const PrivType &priv)
    {
        m_priv  =  priv;
    }

    PrivType  SnmpServerIntSnmpConfigAddUserIntMessage::getPriv() const
    {
        return (m_priv);
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setPrivpswd(const string &privpswd)
    {
        m_privpswd  =  privpswd;
    }

    string  SnmpServerIntSnmpConfigAddUserIntMessage::getPrivpswd() const
    {
        return (m_privpswd);
    }

    void  SnmpServerIntSnmpConfigAddUserIntMessage::setEncpswdflag(const bool &encpswdflag)
    {
        m_encpswdflag	=  encpswdflag;
    }

    bool SnmpServerIntSnmpConfigAddUserIntMessage::getEncpswdflag() const
    {
        return (m_encpswdflag);
    }

}
