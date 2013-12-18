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

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalUserActionMessage.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalUserActionMessage::AAAGlobalUserActionMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALUSERACTION)
{
}

AAAGlobalUserActionMessage::AAAGlobalUserActionMessage(const string &name, const AAAActionType &actionType)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALUSERACTION),
	m_name (name),
	m_actionType (actionType)
{
}

AAAGlobalUserActionMessage::~AAAGlobalUserActionMessage ()
{
}

void  AAAGlobalUserActionMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_name,"name"));
	addSerializableAttribute (new AttributeEnum((UI32*)(&m_actionType),"actionType"));
}

void  AAAGlobalUserActionMessage::setName(const string &name)
{
	m_name  =  name;
}

string  AAAGlobalUserActionMessage::getName() const
{
	return (m_name);
}

void  AAAGlobalUserActionMessage::setActionType(const AAAActionType &actionType)
{
	m_actionType  =  actionType;
}

AAAActionType  AAAGlobalUserActionMessage::getActionType() const
{
	return (m_actionType);
}

}
