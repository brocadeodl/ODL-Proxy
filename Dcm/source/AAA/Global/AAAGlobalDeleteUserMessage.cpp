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

#include "AAA/Global/AAAGlobalDeleteUserMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalDeleteUserMessage::AAAGlobalDeleteUserMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALDELETEUSER)
{
}

AAAGlobalDeleteUserMessage::AAAGlobalDeleteUserMessage (const string &name, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALDELETEUSER),
	m_name    (name),
	m_loginUser (loginUser),
	m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAAGlobalDeleteUserMessage::~AAAGlobalDeleteUserMessage ()
{
}

void  AAAGlobalDeleteUserMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAAGlobalDeleteUserMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAAGlobalDeleteUserMessage::getName() const
{
	return (m_name);
}

void  AAAGlobalDeleteUserMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAAGlobalDeleteUserMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAAGlobalDeleteUserMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAAGlobalDeleteUserMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAAGlobalDeleteUserMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAAGlobalDeleteUserMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAAGlobalDeleteUserMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAAGlobalDeleteUserMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAAGlobalDeleteUserMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAAGlobalDeleteUserMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
