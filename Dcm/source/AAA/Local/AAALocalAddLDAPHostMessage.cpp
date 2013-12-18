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
 *   Author : kghanta                                                     *
 **************************************************************************/
#include "AAA/Local/AAALocalAddLDAPHostMessage.h"
#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalAddLDAPHostMessage::AAALocalAddLDAPHostMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDLDAPHOST)
{
}

AAALocalAddLDAPHostMessage::AAALocalAddLDAPHostMessage (const string &host, const SI32 &port, const string &basedn, const SI8 &retries, const SI8 &timeout)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALADDLDAPHOST),
	m_host (host),
	m_port (port),
	m_basedn (basedn),
	m_retries (retries),
	m_timeout (timeout)
{
}

AAALocalAddLDAPHostMessage::~AAALocalAddLDAPHostMessage ()
{
}

void AAALocalAddLDAPHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeSI32(&m_port,"port"));
	addSerializableAttribute (new AttributeString(&m_basedn,"basedn"));
	addSerializableAttribute (new AttributeSI8(&m_retries,"retries"));
	addSerializableAttribute (new AttributeSI8(&m_timeout,"timeout"));
}

void AAALocalAddLDAPHostMessage::setHost(const string &host)
{
	m_host = host;
}

string AAALocalAddLDAPHostMessage::getHost() const
{
	return (m_host);
}

void AAALocalAddLDAPHostMessage::setPort(const SI32 &port)
{
	m_port = port;
}

SI32 AAALocalAddLDAPHostMessage::getPort() const
{
	return (m_port);
}

void AAALocalAddLDAPHostMessage::setBasedn(const string &basedn)
{
	m_basedn = basedn;
}

string AAALocalAddLDAPHostMessage::getBasedn() const
{
	return (m_basedn);
}

void AAALocalAddLDAPHostMessage::setRetries(const SI8 &retries)
{
	m_retries = retries;
}

SI8 AAALocalAddLDAPHostMessage::getRetries() const
{
	return (m_retries);
}

void AAALocalAddLDAPHostMessage::setTimeout(const SI8 &timeout)
{
	m_timeout = timeout;
}

SI8 AAALocalAddLDAPHostMessage::getTimeout() const
{
	return (m_timeout);
}

}
