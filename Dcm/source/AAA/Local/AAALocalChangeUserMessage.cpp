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
#include "AAA/Local/AAALocalChangeUserMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalChangeUserMessage::AAALocalChangeUserMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEUSER)
{
}

AAALocalChangeUserMessage::AAALocalChangeUserMessage (const string &name, const string &password, const string &role, const string &usersOfRole, const string &oldRole, const string &usersOfOldRole, const string &desc, const bool &descF, const bool &enabled, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEUSER),
	m_name    (name),
	m_password    (password),
	m_role    (role),
	m_usersOfRole    (usersOfRole),
	m_oldRole    (oldRole),
	m_usersOfOldRole    (usersOfOldRole),
	m_desc    (desc),
	m_descF    (descF),
	m_enabled    (enabled),
    m_loginUser  (loginUser),
    m_loginRole  (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAALocalChangeUserMessage::~AAALocalChangeUserMessage ()
{
}

void  AAALocalChangeUserMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_password,"password"));
	addSerializableAttribute (new AttributeString(&m_role,"role"));
	addSerializableAttribute (new AttributeString(&m_usersOfRole,"usersOfRole"));
	addSerializableAttribute (new AttributeString(&m_oldRole,"oldRole"));
	addSerializableAttribute (new AttributeString(&m_usersOfOldRole,"usersOfOldRole"));
	addSerializableAttribute (new AttributeString(&m_desc,"desc"));
	addSerializableAttribute (new AttributeBool(&m_descF,"descF"));
	addSerializableAttribute (new AttributeBool(&m_enabled,"enabled"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAALocalChangeUserMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAALocalChangeUserMessage::getName() const
{
	return (m_name);
}

void  AAALocalChangeUserMessage::setPassword(const string &password)
{
	m_password  =  password;
}

string  AAALocalChangeUserMessage::getPassword() const
{
	return (m_password);
}

void  AAALocalChangeUserMessage::setRole(const string &role)
{
	m_role  =  role;
}

string  AAALocalChangeUserMessage::getRole() const
{
	return (m_role);
}

void  AAALocalChangeUserMessage::setUsersOfRole(const string &usersOfRole)
{
	m_usersOfRole  =  usersOfRole;
}

string  AAALocalChangeUserMessage::getUsersOfRole() const
{
	return (m_usersOfRole);
}

void  AAALocalChangeUserMessage::setOldRole(const string &oldRole)
{
	m_oldRole  =  oldRole;
}

string  AAALocalChangeUserMessage::getOldRole() const
{
	return (m_oldRole);
}

void  AAALocalChangeUserMessage::setUsersOfOldRole(const string &usersOfOldRole)
{
	m_usersOfOldRole  =  usersOfOldRole;
}

string  AAALocalChangeUserMessage::getUsersOfOldRole() const
{
	return (m_usersOfOldRole);
}

void  AAALocalChangeUserMessage::setDescription(const string &desc)
{
	m_desc  =  desc;
}

string  AAALocalChangeUserMessage::getDescription() const
{
	return (m_desc);
}

void  AAALocalChangeUserMessage::setDescFlag(const bool &descF)
{
    m_descF  =  descF;
}

bool  AAALocalChangeUserMessage::getDescFlag() const
{
    return (m_descF);
}

void  AAALocalChangeUserMessage::setEnabled(const bool &enabled)
{
	m_enabled  =  enabled;
}

bool  AAALocalChangeUserMessage::getEnabled() const
{
	return (m_enabled);
}

void  AAALocalChangeUserMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAALocalChangeUserMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAALocalChangeUserMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAALocalChangeUserMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAALocalChangeUserMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAALocalChangeUserMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAALocalChangeUserMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAALocalChangeUserMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAALocalChangeUserMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAALocalChangeUserMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
