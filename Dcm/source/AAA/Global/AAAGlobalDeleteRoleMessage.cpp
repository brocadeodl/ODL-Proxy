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

#include "AAA/Global/AAAGlobalDeleteRoleMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalDeleteRoleMessage::AAAGlobalDeleteRoleMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALDELETEROLE)
{
}

AAAGlobalDeleteRoleMessage::AAAGlobalDeleteRoleMessage (const string &name, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALDELETEROLE),
	m_name    (name),
	m_loginUser (loginUser),
	m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAAGlobalDeleteRoleMessage::~AAAGlobalDeleteRoleMessage ()
{
}

void  AAAGlobalDeleteRoleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAAGlobalDeleteRoleMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAAGlobalDeleteRoleMessage::getName() const
{
	return (m_name);
}

void  AAAGlobalDeleteRoleMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAAGlobalDeleteRoleMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAAGlobalDeleteRoleMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAAGlobalDeleteRoleMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAAGlobalDeleteRoleMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAAGlobalDeleteRoleMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAAGlobalDeleteRoleMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAAGlobalDeleteRoleMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAAGlobalDeleteRoleMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAAGlobalDeleteRoleMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
