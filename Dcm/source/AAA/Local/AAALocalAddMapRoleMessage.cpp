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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kghanta                                                      *
 **************************************************************************/

#include "AAA/Local/AAALocalAddMapRoleMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalAddMapRoleMessage::AAALocalAddMapRoleMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDMAPROLE)
{
}

AAALocalAddMapRoleMessage::AAALocalAddMapRoleMessage (const string &group,const string &role)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDMAPROLE),
	m_group    (group),
	m_role    (role)
{
}

AAALocalAddMapRoleMessage::~AAALocalAddMapRoleMessage ()
{
}

void  AAALocalAddMapRoleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_group,"group"));
	addSerializableAttribute (new AttributeString(&m_role,"role"));
}

void  AAALocalAddMapRoleMessage::setGroup(const string &group)
{
	m_group  =  group;
}

string  AAALocalAddMapRoleMessage::getGroup() const
{
	return (m_group);
}

void  AAALocalAddMapRoleMessage::setRole(const string &role)
{
	m_role  =  role;
}

string  AAALocalAddMapRoleMessage::getRole() const
{
	return (m_role);
}

}
