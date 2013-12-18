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

#include "AAA/Global/AAAGlobalAddLDAPHostMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalAddLDAPHostMessage::AAAGlobalAddLDAPHostMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALADDLDAPHOST)
{
}

AAAGlobalAddLDAPHostMessage::AAAGlobalAddLDAPHostMessage (const string &host, const SI32 &port, const string &basedn, const SI8 &retries, const SI8 &timeout)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALADDLDAPHOST),
	m_host (host),
	m_port (port),
	m_basedn (basedn),
	m_retries (retries),
	m_timeout (timeout)
{
}

AAAGlobalAddLDAPHostMessage::~AAAGlobalAddLDAPHostMessage ()
{
}

void AAAGlobalAddLDAPHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeSI32(&m_port,"port"));
	addSerializableAttribute (new AttributeString(&m_basedn,"basedn"));
	addSerializableAttribute (new AttributeSI8(&m_retries,"retries"));
	addSerializableAttribute (new AttributeSI8(&m_timeout,"timeout"));
}

void AAAGlobalAddLDAPHostMessage::setHost(const string &host)
{
	m_host = host;
}

string AAAGlobalAddLDAPHostMessage::getHost() const
{
	return (m_host);
}

void AAAGlobalAddLDAPHostMessage::setPort(const SI32 &port)
{
	m_port = port;
}

SI32 AAAGlobalAddLDAPHostMessage::getPort() const
{
	return (m_port);
}

void AAAGlobalAddLDAPHostMessage::setBasedn(const string &basedn)
{
	m_basedn = basedn;
}

string AAAGlobalAddLDAPHostMessage::getBasedn() const
{
	return (m_basedn);
}

void AAAGlobalAddLDAPHostMessage::setRetries(const SI8 &retries)
{
	m_retries = retries;
}

SI8 AAAGlobalAddLDAPHostMessage::getRetries() const
{
	return (m_retries);
}

void AAAGlobalAddLDAPHostMessage::setTimeout(const SI8 &timeout)
{
	m_timeout = timeout;
}

SI8 AAAGlobalAddLDAPHostMessage::getTimeout() const
{
	return (m_timeout);
}

}
