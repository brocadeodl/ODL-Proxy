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

#include "Framework/Attributes/AttributeEnum.h"

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalUserActionMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalUserActionMessage::AAALocalUserActionMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALUSERACTION)
{
}

AAALocalUserActionMessage::AAALocalUserActionMessage(const string &name, const AAAActionType &actionType)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALUSERACTION),
	m_name (name),
	m_actionType (actionType)
{
}

AAALocalUserActionMessage::~AAALocalUserActionMessage ()
{
}

void  AAALocalUserActionMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeEnum((UI32*)(&m_actionType),"actionType"));
}

void  AAALocalUserActionMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAALocalUserActionMessage::getName() const
{
	return (m_name);
}

void  AAALocalUserActionMessage::setActionType(const AAAActionType &actionType)
{
	m_actionType  =  actionType;
}

AAAActionType  AAALocalUserActionMessage::getActionType() const
{
	return (m_actionType);
}

}
