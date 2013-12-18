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

#include "AAA/Global/AAAGlobalChangeAuthLoginModeMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

AAAGlobalChangeAuthLoginModeMessage::AAAGlobalChangeAuthLoginModeMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGEAUTHLOGINMODE)
{
}

AAAGlobalChangeAuthLoginModeMessage::AAAGlobalChangeAuthLoginModeMessage (const SI8 &first, const SI8 &second, const bool &isLogout)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGEAUTHLOGINMODE),
	m_first    (first),
	m_second    (second),
    m_isLogout  (isLogout)
{
}

AAAGlobalChangeAuthLoginModeMessage::~AAAGlobalChangeAuthLoginModeMessage ()
{
}

void  AAAGlobalChangeAuthLoginModeMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeSI8(&m_first, "first"));
	addSerializableAttribute (new AttributeSI8(&m_second, "second"));
	addSerializableAttribute (new AttributeBool(&m_isLogout, "logout"));
}

void AAAGlobalChangeAuthLoginModeMessage::setFirst(const SI8 &first)
{
	m_first  =  first;
}

SI8 AAAGlobalChangeAuthLoginModeMessage::getFirst() const
{
	return (m_first);
}

void AAAGlobalChangeAuthLoginModeMessage::setSecond(const SI8 &second)
{
	m_second  =  second;
}

SI8  AAAGlobalChangeAuthLoginModeMessage::getSecond() const
{
	return (m_second);
}

void AAAGlobalChangeAuthLoginModeMessage::setIsLogout(const bool &isLogout)
{
    m_isLogout = isLogout;
}
bool AAAGlobalChangeAuthLoginModeMessage::getIsLogout()  const
{
    return (m_isLogout);
}
}
