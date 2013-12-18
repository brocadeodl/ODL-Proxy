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

#include "AAA/Global/AAAGlobalAddUserMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalAddUserMessage::AAAGlobalAddUserMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALADDUSER)
{
}

AAAGlobalAddUserMessage::AAAGlobalAddUserMessage (const string &name, const string &password, const SI8 &encryptionLevel, const string &role, const string &desc, const bool &enabled, const bool &enabledF, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALADDUSER),
	m_name    (name),
	m_password    (password),
	m_encryptionLevel    (encryptionLevel),
	m_role    (role),
	m_desc    (desc),
	m_enabled    (enabled),
	m_enabledF    (enabledF),
	m_loginUser (loginUser),
	m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAAGlobalAddUserMessage::~AAAGlobalAddUserMessage ()
{
}

void  AAAGlobalAddUserMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_password,"password"));
	addSerializableAttribute (new AttributeSI8(&m_encryptionLevel,"encryptionLevel"));
	addSerializableAttribute (new AttributeString(&m_role,"role"));
	addSerializableAttribute (new AttributeString(&m_desc,"desc"));
	addSerializableAttribute (new AttributeBool(&m_enabled,"enabled"));
	addSerializableAttribute (new AttributeBool(&m_enabledF,"enabledF"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAAGlobalAddUserMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAAGlobalAddUserMessage::getName() const
{
	return (m_name);
}

void  AAAGlobalAddUserMessage::setPassword(const string &password)
{
	m_password  =  password;
}

string  AAAGlobalAddUserMessage::getPassword() const
{
	return (m_password);
}

void  AAAGlobalAddUserMessage::setEncryptionLevel(const SI8 &encryptionLevel)
{
	m_encryptionLevel  =  encryptionLevel;
}

SI8  AAAGlobalAddUserMessage::getEncryptionLevel() const
{
	return (m_encryptionLevel);
}

void  AAAGlobalAddUserMessage::setRole(const string &role)
{
	m_role  =  role;
}

string  AAAGlobalAddUserMessage::getRole() const
{
	return (m_role);
}

void  AAAGlobalAddUserMessage::setDescription(const string &desc)
{
	m_desc  =  desc;
}

string  AAAGlobalAddUserMessage::getDescription() const
{
	return (m_desc);
}

void  AAAGlobalAddUserMessage::setEnabled(const bool &enabled)
{
	m_enabled  =  enabled;
}

bool  AAAGlobalAddUserMessage::getEnabled() const
{
	return (m_enabled);
}

void  AAAGlobalAddUserMessage::setEnabledFlag(const bool &enabledF)
{
	m_enabledF  =  enabledF;
}

bool  AAAGlobalAddUserMessage::getEnabledFlag() const
{
	return (m_enabledF);
}

void  AAAGlobalAddUserMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAAGlobalAddUserMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAAGlobalAddUserMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAAGlobalAddUserMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAAGlobalAddUserMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAAGlobalAddUserMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAAGlobalAddUserMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAAGlobalAddUserMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAAGlobalAddUserMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAAGlobalAddUserMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
