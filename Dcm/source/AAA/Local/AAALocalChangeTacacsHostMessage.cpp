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

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalChangeTacacsHostMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalChangeTacacsHostMessage::AAALocalChangeTacacsHostMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGETACACSHOST)
{
}

AAALocalChangeTacacsHostMessage::AAALocalChangeTacacsHostMessage (const string &host, const UI16 &port, const SI8 &protocol, const string &key, const SI8 &retries, const SI8 &timeout)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGETACACSHOST),
	m_host (host),
	m_port (port),
	m_protocol (protocol),
	m_key (key),
	m_retries (retries),
	m_timeout (timeout)
{
}

AAALocalChangeTacacsHostMessage::~AAALocalChangeTacacsHostMessage ()
{
}

void AAALocalChangeTacacsHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeUI16(&m_port,"port"));
	addSerializableAttribute (new AttributeSI8(&m_protocol,"protocol"));
	addSerializableAttribute (new AttributeString(&m_key,"key"));
	addSerializableAttribute (new AttributeSI8(&m_retries,"retries"));
	addSerializableAttribute (new AttributeSI8(&m_timeout,"timeout"));
}

void AAALocalChangeTacacsHostMessage::setHost(const string &host)
{
	m_host = host;
}

string AAALocalChangeTacacsHostMessage::getHost() const
{
	return (m_host);
}

void AAALocalChangeTacacsHostMessage::setPort(const UI16 &port)
{
	m_port = port;
}

UI16 AAALocalChangeTacacsHostMessage::getPort() const
{
	return (m_port);
}

void AAALocalChangeTacacsHostMessage::setProtocol(const SI8 &protocol)
{
	m_protocol = protocol;
}

SI8 AAALocalChangeTacacsHostMessage::getProtocol() const
{
	return (m_protocol);
}

void AAALocalChangeTacacsHostMessage::setKey(const string &key)
{
	m_key = key;
}

string AAALocalChangeTacacsHostMessage::getKey() const
{
	return (m_key);
}

void AAALocalChangeTacacsHostMessage::setRetries(const SI8 &retries)
{
	m_retries = retries;
}

SI8 AAALocalChangeTacacsHostMessage::getRetries() const
{
	return (m_retries);
}

void AAALocalChangeTacacsHostMessage::setTimeout(const SI8 &timeout)
{
	m_timeout = timeout;
}

SI8 AAALocalChangeTacacsHostMessage::getTimeout() const
{
	return (m_timeout);
}

}
