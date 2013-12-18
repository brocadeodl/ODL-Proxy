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

#include "AAA/Global/AAAGlobalChangeUserMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalChangeUserMessage::AAAGlobalChangeUserMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGEUSER)
{
}

AAAGlobalChangeUserMessage::AAAGlobalChangeUserMessage (const string &name, const string &password, const SI8 &encryptionLevel, const string &role, const string &desc, const bool &descF, const bool &enabled, const bool &enabledF, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGEUSER),
	m_name    (name),
	m_password    (password),
	m_encryptionLevel    (encryptionLevel),
	m_role    (role),
	m_desc    (desc),
	m_descF    (descF),
	m_enabled    (enabled),
	m_enabledF    (enabledF),
	m_loginUser (loginUser),
	m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAAGlobalChangeUserMessage::~AAAGlobalChangeUserMessage ()
{
}

void  AAAGlobalChangeUserMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_password,"password"));
	addSerializableAttribute (new AttributeSI8(&m_encryptionLevel,"encryptionLevel"));
	addSerializableAttribute (new AttributeString(&m_role,"role"));
	addSerializableAttribute (new AttributeString(&m_desc,"desc"));
	addSerializableAttribute (new AttributeBool(&m_descF,"descF"));
	addSerializableAttribute (new AttributeBool(&m_enabled,"enabled"));
	addSerializableAttribute (new AttributeBool(&m_enabledF,"enabledF"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAAGlobalChangeUserMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAAGlobalChangeUserMessage::getName() const
{
	return (m_name);
}

void  AAAGlobalChangeUserMessage::setPassword(const string &password)
{
	m_password  =  password;
}

string  AAAGlobalChangeUserMessage::getPassword() const
{
	return (m_password);
}

void  AAAGlobalChangeUserMessage::setEncryptionLevel(const SI8 &encryptionLevel)
{
	m_encryptionLevel  =  encryptionLevel;
}

SI8  AAAGlobalChangeUserMessage::getEncryptionLevel() const
{
	return (m_encryptionLevel);
}

void  AAAGlobalChangeUserMessage::setRole(const string &role)
{
	m_role  =  role;
}

string  AAAGlobalChangeUserMessage::getRole() const
{
	return (m_role);
}

void  AAAGlobalChangeUserMessage::setDescription(const string &desc)
{
	m_desc  =  desc;
}

string  AAAGlobalChangeUserMessage::getDescription() const
{
	return (m_desc);
}

void  AAAGlobalChangeUserMessage::setDescFlag(const bool &descF)
{
	m_descF  =  descF;
}

bool  AAAGlobalChangeUserMessage::getDescFlag() const
{
	return (m_descF);
}

void  AAAGlobalChangeUserMessage::setEnabled(const bool &enabled)
{
	m_enabled  =  enabled;
}

bool  AAAGlobalChangeUserMessage::getEnabled() const
{
	return (m_enabled);
}

void  AAAGlobalChangeUserMessage::setEnabledFlag(const bool &enabledF)
{
	m_enabledF  =  enabledF;
}

bool  AAAGlobalChangeUserMessage::getEnabledFlag() const
{
	return (m_enabledF);
}

void  AAAGlobalChangeUserMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAAGlobalChangeUserMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAAGlobalChangeUserMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAAGlobalChangeUserMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAAGlobalChangeUserMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAAGlobalChangeUserMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAAGlobalChangeUserMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAAGlobalChangeUserMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAAGlobalChangeUserMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAAGlobalChangeUserMessage::getLoginAppName() const
{
	return (m_loginAppName);
}
}
