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

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalChangeRoleMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalChangeRoleMessage::AAALocalChangeRoleMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEROLE)
{
}

AAALocalChangeRoleMessage::AAALocalChangeRoleMessage (const string &name, const string &description, const bool &descFlag, const string &users, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEROLE),
	m_name (name),
	m_description (description),
	m_descF    (descFlag),
	m_users (users),
    m_loginUser  (loginUser),
    m_loginRole  (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAALocalChangeRoleMessage::~AAALocalChangeRoleMessage ()
{
}

void  AAALocalChangeRoleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_description,"description"));
	addSerializableAttribute (new AttributeBool(&m_descF,"descF"));
	addSerializableAttribute (new AttributeString(&m_users,"users"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAALocalChangeRoleMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAALocalChangeRoleMessage::getName() const
{
	return (m_name);
}

void  AAALocalChangeRoleMessage::setDescription(const string &description)
{
	m_description  =  description;
}

string  AAALocalChangeRoleMessage::getDescription() const
{
	return (m_description);
}

void  AAALocalChangeRoleMessage::setDescFlag(const bool &descF)
{
    m_descF  =  descF;
}

bool  AAALocalChangeRoleMessage::getDescFlag() const
{
    return (m_descF);
}

void  AAALocalChangeRoleMessage::setUsers(const string &users)
{
	m_users  =  users;
}

string  AAALocalChangeRoleMessage::getUsers() const
{
	return (m_users);
}

void  AAALocalChangeRoleMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAALocalChangeRoleMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAALocalChangeRoleMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAALocalChangeRoleMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAALocalChangeRoleMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAALocalChangeRoleMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAALocalChangeRoleMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAALocalChangeRoleMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAALocalChangeRoleMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAALocalChangeRoleMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
