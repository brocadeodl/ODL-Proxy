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
#include "AAA/Local/AAALocalDeleteUserMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalDeleteUserMessage::AAALocalDeleteUserMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALDELETEUSER)
{
}

AAALocalDeleteUserMessage::AAALocalDeleteUserMessage (const string &name, const string &role, const string &usersOfRole, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALDELETEUSER),
	m_name    (name),
	m_role    (role),
	m_usersOfRole    (usersOfRole),
    m_loginUser (loginUser),
    m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAALocalDeleteUserMessage::~AAALocalDeleteUserMessage ()
{
}

void  AAALocalDeleteUserMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_role,"role"));
	addSerializableAttribute (new AttributeString(&m_usersOfRole,"usersOfRole"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"m_loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"m_loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAALocalDeleteUserMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAALocalDeleteUserMessage::getName() const
{
	return (m_name);
}

void  AAALocalDeleteUserMessage::setRole(const string &role)
{
	m_role  =  role;
}

string  AAALocalDeleteUserMessage::getRole() const
{
	return (m_role);
}

void  AAALocalDeleteUserMessage::setUsersOfRole(const string &usersOfRole)
{
	m_usersOfRole  =  usersOfRole;
}

string  AAALocalDeleteUserMessage::getUsersOfRole() const
{
	return (m_usersOfRole);
}

void  AAALocalDeleteUserMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAALocalDeleteUserMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAALocalDeleteUserMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAALocalDeleteUserMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAALocalDeleteUserMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAALocalDeleteUserMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAALocalDeleteUserMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAALocalDeleteUserMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAALocalDeleteUserMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAALocalDeleteUserMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
