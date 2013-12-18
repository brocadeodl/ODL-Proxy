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
 *   Author : singhb	                                                   *
 **************************************************************************/
#include "Framework/Attributes/AttributeEnum.h"

#include "AAA/Local/AAALocalChangeAccountingMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

AAALocalChangeAccountingMessage::AAALocalChangeAccountingMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEACCOUNTING)
{
}

AAALocalChangeAccountingMessage::AAALocalChangeAccountingMessage (const SI8 &accType, const SI8 &serverType)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEACCOUNTING),
	m_accType    (accType),
	m_serverType    (serverType)
{
}

AAALocalChangeAccountingMessage::~AAALocalChangeAccountingMessage ()
{
}

void  AAALocalChangeAccountingMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeSI8(&m_accType, "accType"));
	addSerializableAttribute (new AttributeSI8(&m_serverType, "serverType"));
}

void AAALocalChangeAccountingMessage::setAccType(const SI8 &accType)
{
	m_accType  =  accType;
}

SI8 AAALocalChangeAccountingMessage::getAccType() const
{
	return (m_accType);
}

void AAALocalChangeAccountingMessage::setServerType(const SI8 &serverType)
{
	m_serverType  =  serverType;
}

SI8  AAALocalChangeAccountingMessage::getServerType() const
{
	return (m_serverType);
}

}
