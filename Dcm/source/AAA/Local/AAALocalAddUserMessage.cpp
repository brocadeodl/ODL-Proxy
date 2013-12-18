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
#include "AAA/Local/AAALocalAddUserMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalAddUserMessage::AAALocalAddUserMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDUSER)
{
}

AAALocalAddUserMessage::AAALocalAddUserMessage (const string &name, const string &password, const string &role, const string &usersOfRole, const string &desc, const bool &enabled, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDUSER),
	m_name    (name),
	m_password    (password),
	m_role    (role),
	m_usersOfRole    (usersOfRole),
	m_desc    (desc),
	m_enabled    (enabled),
    m_loginUser (loginUser),
    m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAALocalAddUserMessage::~AAALocalAddUserMessage ()
{
}

void  AAALocalAddUserMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_password,"password"));
	addSerializableAttribute (new AttributeString(&m_role,"role"));
	addSerializableAttribute (new AttributeString(&m_usersOfRole,"usersOfRole"));
	addSerializableAttribute (new AttributeString(&m_desc,"desc"));
	addSerializableAttribute (new AttributeBool(&m_enabled,"enabled"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAALocalAddUserMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAALocalAddUserMessage::getName() const
{
	return (m_name);
}

void  AAALocalAddUserMessage::setPassword(const string &password)
{
	m_password  =  password;
}

string  AAALocalAddUserMessage::getPassword() const
{
	return (m_password);
}

void  AAALocalAddUserMessage::setRole(const string &role)
{
	m_role  =  role;
}

string  AAALocalAddUserMessage::getRole() const
{
	return (m_role);
}

void  AAALocalAddUserMessage::setUsersOfRole(const string &usersOfRole)
{
	m_usersOfRole  =  usersOfRole;
}

string  AAALocalAddUserMessage::getUsersOfRole() const
{
	return (m_usersOfRole);
}

void  AAALocalAddUserMessage::setDescription(const string &desc)
{
	m_desc  =  desc;
}

string  AAALocalAddUserMessage::getDescription() const
{
	return (m_desc);
}

void  AAALocalAddUserMessage::setEnabled(const bool &enabled)
{
	m_enabled  =  enabled;
}

bool  AAALocalAddUserMessage::getEnabled() const
{
	return (m_enabled);
}

void  AAALocalAddUserMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAALocalAddUserMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAALocalAddUserMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAALocalAddUserMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAALocalAddUserMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAALocalAddUserMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAALocalAddUserMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAALocalAddUserMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAALocalAddUserMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAALocalAddUserMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
