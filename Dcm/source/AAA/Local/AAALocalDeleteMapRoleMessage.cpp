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

#include "AAA/Local/AAALocalDeleteMapRoleMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalDeleteMapRoleMessage::AAALocalDeleteMapRoleMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALDELETEMAPROLE)
{
}

AAALocalDeleteMapRoleMessage::AAALocalDeleteMapRoleMessage (const string &group)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALDELETEMAPROLE),
	m_group    (group)
{
}

AAALocalDeleteMapRoleMessage::~AAALocalDeleteMapRoleMessage ()
{
}

void  AAALocalDeleteMapRoleMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_group,"group"));
}

void  AAALocalDeleteMapRoleMessage::setGroup(const string &group)
{
	m_group  =  group;
}

string  AAALocalDeleteMapRoleMessage::getGroup() const
{
	return (m_group);
}

}
