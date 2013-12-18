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

#include "AAA/Global/AAAGlobalAddTacacsHostMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalAddTacacsHostMessage::AAAGlobalAddTacacsHostMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALADDTACACSHOST)
{
}

AAAGlobalAddTacacsHostMessage::AAAGlobalAddTacacsHostMessage (const string &host, const SI32 &port, const SI8 &protocol, const string &key, const SI8 &retries, const SI8 &timeout)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALADDTACACSHOST),
	m_host (host),
	m_port (port),
	m_protocol (protocol),
	m_key (key),
	m_retries (retries),
	m_timeout (timeout)
{
}

AAAGlobalAddTacacsHostMessage::~AAAGlobalAddTacacsHostMessage ()
{
}

void AAAGlobalAddTacacsHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeSI32(&m_port,"port"));
	addSerializableAttribute (new AttributeSI8(&m_protocol,"protocol"));
	addSerializableAttribute (new AttributeString(&m_key,"key"));
	addSerializableAttribute (new AttributeSI8(&m_retries,"retries"));
	addSerializableAttribute (new AttributeSI8(&m_timeout,"timeout"));
}

void AAAGlobalAddTacacsHostMessage::setHost(const string &host)
{
	m_host = host;
}

string AAAGlobalAddTacacsHostMessage::getHost() const
{
	return (m_host);
}

void AAAGlobalAddTacacsHostMessage::setPort(const SI32 &port)
{
	m_port = port;
}

SI32 AAAGlobalAddTacacsHostMessage::getPort() const
{
	return (m_port);
}

void AAAGlobalAddTacacsHostMessage::setProtocol(const SI8 &protocol)
{
	m_protocol = protocol;
}

SI8 AAAGlobalAddTacacsHostMessage::getProtocol() const
{
	return (m_protocol);
}

void AAAGlobalAddTacacsHostMessage::setKey(const string &key)
{
	m_key = key;
}

string AAAGlobalAddTacacsHostMessage::getKey() const
{
	return (m_key);
}

void AAAGlobalAddTacacsHostMessage::setRetries(const SI8 &retries)
{
	m_retries = retries;
}

SI8 AAAGlobalAddTacacsHostMessage::getRetries() const
{
	return (m_retries);
}

void AAAGlobalAddTacacsHostMessage::setTimeout(const SI8 &timeout)
{
	m_timeout = timeout;
}

SI8 AAAGlobalAddTacacsHostMessage::getTimeout() const
{
	return (m_timeout);
}

}
