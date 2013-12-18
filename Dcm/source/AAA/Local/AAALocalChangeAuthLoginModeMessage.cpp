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

#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalChangeAuthLoginModeMessage.h"
#include "AAA/Local/AAALocalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

AAALocalChangeAuthLoginModeMessage::AAALocalChangeAuthLoginModeMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEAUTHLOGINMODE)
{
}

AAALocalChangeAuthLoginModeMessage::AAALocalChangeAuthLoginModeMessage (const UI8 &authMode, const bool &radMode, const bool &tacMode, const bool &ldapMode, const bool &logout)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGEAUTHLOGINMODE),
	m_authMode (authMode),
	m_radiusMode (radMode),
	m_tacacsMode (tacMode),
	m_ldapMode (ldapMode),
	m_logout (logout)
{
}

AAALocalChangeAuthLoginModeMessage::~AAALocalChangeAuthLoginModeMessage ()
{
}

void  AAALocalChangeAuthLoginModeMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeUI8(&m_authMode,"authMode"));
	addSerializableAttribute (new AttributeBool(&m_radiusMode,"radiusMode"));
	addSerializableAttribute (new AttributeBool(&m_tacacsMode,"tacacsMode"));
	addSerializableAttribute (new AttributeBool(&m_ldapMode,"ldapMode"));
	addSerializableAttribute (new AttributeBool(&m_logout,"logout"));
}

UI8 AAALocalChangeAuthLoginModeMessage::getAuthMode() const
{
	return (m_authMode);
}

bool AAALocalChangeAuthLoginModeMessage::getRadiusMode() const
{
	return (m_radiusMode);
}

bool AAALocalChangeAuthLoginModeMessage::getTacacsMode() const
{
	return (m_tacacsMode);
}

bool AAALocalChangeAuthLoginModeMessage::getLDAPMode() const
{
	return (m_ldapMode);
}

bool AAALocalChangeAuthLoginModeMessage::getLogout() const
{
	return (m_logout);
}

}
