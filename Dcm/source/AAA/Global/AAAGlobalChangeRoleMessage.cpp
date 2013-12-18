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

#include "AAA/Global/AAAGlobalChangeRoleMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalChangeRoleMessage::AAAGlobalChangeRoleMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGEROLE)
{
}

AAAGlobalChangeRoleMessage::AAAGlobalChangeRoleMessage (const string &name, const string &description, const bool &descF, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGEROLE),
	m_name    (name),
	m_description    (description),
	m_descF    (descF),
	m_loginUser (loginUser),
	m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAAGlobalChangeRoleMessage::~AAAGlobalChangeRoleMessage ()
{
}

void  AAAGlobalChangeRoleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_description,"description"));
	addSerializableAttribute (new AttributeBool(&m_descF,"descF"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAAGlobalChangeRoleMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAAGlobalChangeRoleMessage::getName() const
{
	return (m_name);
}

void  AAAGlobalChangeRoleMessage::setDescription(const string &description)
{
	m_description  =  description;
}

string  AAAGlobalChangeRoleMessage::getDescription() const
{
	return (m_description);
}

void  AAAGlobalChangeRoleMessage::setDescFlag(const bool &descF)
{
	m_descF  =  descF;
}

bool  AAAGlobalChangeRoleMessage::getDescFlag() const
{
	return (m_descF);
}

void  AAAGlobalChangeRoleMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAAGlobalChangeRoleMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAAGlobalChangeRoleMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAAGlobalChangeRoleMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAAGlobalChangeRoleMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAAGlobalChangeRoleMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAAGlobalChangeRoleMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAAGlobalChangeRoleMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAAGlobalChangeRoleMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAAGlobalChangeRoleMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
