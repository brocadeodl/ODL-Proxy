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

#include "SnmpServer/Global/SnmpUserManagedObject.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
//#include "snmp.h"
#include "vcs.h"
namespace DcmNs
{

    SnmpUserManagedObject::SnmpUserManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (SnmpUserManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager)
    {
    }

    SnmpUserManagedObject::SnmpUserManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const string &username,const string &groupname,const AuthType &auth,const string &authpswd,const PrivType &priv,const string &privpswd,const bool &encpswdflag)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (SnmpUserManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager),
        m_username    (username),
        m_groupname    (groupname),
        m_auth    (auth),
        m_authpswd    (authpswd),
        m_priv    (priv),
        m_privpswd    (privpswd),
        m_encpswdflag (encpswdflag)
    {
		m_auth		= AuthNone;
		m_priv		= PrivNone;
		m_encpswdflag = false;
    }

    SnmpUserManagedObject::~SnmpUserManagedObject ()
    {
    }

    string  SnmpUserManagedObject::getClassName()
    {
        return ("SnmpUserManagedObject");
    }

    void  SnmpUserManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_username,"username", snmp_server_username));
        addPersistableAttribute (new AttributeString(&m_groupname,"groupname", snmp_server_groupname));
        addPersistableAttribute (new AttributeEnum((UI32 * )&m_auth,"auth", snmp_server_auth));
        addPersistableAttribute (new AttributeString(&m_authpswd,"authpswd", snmp_server_auth_password));
        addPersistableAttribute (new AttributeEnum((UI32 * )&m_priv,"priv", snmp_server_priv));
        addPersistableAttribute (new AttributeString(&m_privpswd,"privpswd", snmp_server_priv_password));
        addPersistableAttribute (new AttributeBool(&m_encpswdflag,"encrypted", snmp_server_encrypted));
    }

    void  SnmpUserManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_username,"username", snmp_server_username));
        addPersistableAttributeForCreate  (new AttributeString(&m_groupname,"groupname", snmp_server_groupname));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_auth,"auth", snmp_server_auth));
        addPersistableAttributeForCreate  (new AttributeString(&m_authpswd,"authpswd", snmp_server_auth_password));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_priv,"priv", snmp_server_priv));
        addPersistableAttributeForCreate  (new AttributeString(&m_privpswd,"privpswd", snmp_server_priv_password));
        addPersistableAttributeForCreate  (new AttributeBool(&m_encpswdflag,"encrypted", snmp_server_encrypted));
        vector <string> keyList;
        keyList.push_back ("username");
//        keyList.push_back ("groupname");
        setUserDefinedKeyCombination (keyList);
    }

    void  SnmpUserManagedObject::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpUserManagedObject::getUsername() const
    {
        return (m_username);
    }

    void  SnmpUserManagedObject::setGroupname(const string &groupname)
    {
        m_groupname  =  groupname;
    }

    string  SnmpUserManagedObject::getGroupname() const
    {
        return (m_groupname);
    }

    void  SnmpUserManagedObject::setAuth(const AuthType &auth)
    {
        m_auth  =  auth;
    }

    AuthType  SnmpUserManagedObject::getAuth() const
    {
        return (m_auth);
    }

    void  SnmpUserManagedObject::setAuthpswd(const string &authpswd)
    {
        m_authpswd  =  authpswd;
    }

    string  SnmpUserManagedObject::getAuthpswd() const
    {
        return (m_authpswd);
    }

    void  SnmpUserManagedObject::setPriv(const PrivType &priv)
    {
        m_priv  =  priv;
    }

    PrivType  SnmpUserManagedObject::getPriv() const
    {
        return (m_priv);
    }

    void  SnmpUserManagedObject::setPrivpswd(const string &privpswd)
    {
        m_privpswd  =  privpswd;
    }

    string  SnmpUserManagedObject::getPrivpswd() const
    {
        return (m_privpswd);
    }

    void  SnmpUserManagedObject::setEncpswdflag(const bool &encpswdflag)
    {
        m_encpswdflag	=  encpswdflag;
    }

    bool SnmpUserManagedObject::getEncpswdflag() const
    {
        return (m_encpswdflag);
    }

}
