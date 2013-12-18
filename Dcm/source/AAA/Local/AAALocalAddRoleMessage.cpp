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
#include "AAA/Local/AAALocalAddRoleMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalAddRoleMessage::AAALocalAddRoleMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDROLE)
{
}

AAALocalAddRoleMessage::AAALocalAddRoleMessage (const string &name, const string &description, const UI32 &gid, const string &loginUser, const string &loginRole, const string &loginIp, const string &loginProto, const string &loginAppName)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDROLE),
	m_name (name),
	m_description (description),
	m_gid (gid),
    m_loginUser (loginUser),
    m_loginRole (loginRole),
	m_loginIp (loginIp),
	m_loginProto (loginProto),
	m_loginAppName (loginAppName)
{
}

AAALocalAddRoleMessage::~AAALocalAddRoleMessage ()
{
}

void  AAALocalAddRoleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeString(&m_description,"description"));
	addSerializableAttribute (new AttributeUI32(&m_gid,"gid"));
	addSerializableAttribute (new AttributeString(&m_loginUser,"loginUser"));
	addSerializableAttribute (new AttributeString(&m_loginRole,"loginRole"));
	addSerializableAttribute (new AttributeString(&m_loginIp,"loginIp"));
	addSerializableAttribute (new AttributeString(&m_loginProto,"loginProto"));
	addSerializableAttribute (new AttributeString(&m_loginAppName,"loginAppName"));
}

void  AAALocalAddRoleMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAALocalAddRoleMessage::getName() const
{
	return (m_name);
}

void  AAALocalAddRoleMessage::setDescription(const string &description)
{
	m_description  =  description;
}

string  AAALocalAddRoleMessage::getDescription() const
{
	return (m_description);
}

void  AAALocalAddRoleMessage::setGid(const UI32 &gid)
{
	m_gid  =  gid;
}

UI32  AAALocalAddRoleMessage::getGid() const
{
	return (m_gid);
}

void  AAALocalAddRoleMessage::setLoginUser(const string &loginUser)
{
	m_loginUser  =  loginUser;
}

string  AAALocalAddRoleMessage::getLoginUser() const
{
	return (m_loginUser);
}

void  AAALocalAddRoleMessage::setLoginRole(const string &loginRole)
{
	m_loginRole  =  loginRole;
}

string  AAALocalAddRoleMessage::getLoginRole() const
{
	return (m_loginRole);
}

void  AAALocalAddRoleMessage::setLoginIp(const string &loginIp)
{
	m_loginIp  =  loginIp;
}

string  AAALocalAddRoleMessage::getLoginIp() const
{
	return (m_loginIp);
}

void  AAALocalAddRoleMessage::setLoginProto(const string &loginProto)
{
	m_loginProto  =  loginProto;
}

string  AAALocalAddRoleMessage::getLoginProto() const
{
	return (m_loginProto);
}

void  AAALocalAddRoleMessage::setLoginAppName(const string &loginAppName)
{
	m_loginAppName  =  loginAppName;
}

string  AAALocalAddRoleMessage::getLoginAppName() const
{
	return (m_loginAppName);
}

}
