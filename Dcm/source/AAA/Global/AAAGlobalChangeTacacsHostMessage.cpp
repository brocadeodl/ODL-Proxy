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

#include "AAA/Global/AAAGlobalChangeTacacsHostMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalChangeTacacsHostMessage::AAAGlobalChangeTacacsHostMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGETACACSHOST)
{
}

AAAGlobalChangeTacacsHostMessage::AAAGlobalChangeTacacsHostMessage (const string &host, const SI32 &port, const SI8 &protocol, const string &key, const SI8 &retries, const SI8 &timeout)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId(), AAAGLOBALCHANGETACACSHOST),
	m_host (host),
	m_port (port),
	m_protocol (protocol),
	m_key (key),
	m_retries (retries),
	m_timeout (timeout)
{
}

AAAGlobalChangeTacacsHostMessage::~AAAGlobalChangeTacacsHostMessage ()
{
}

void AAAGlobalChangeTacacsHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeSI32(&m_port,"port"));
	addSerializableAttribute (new AttributeSI8(&m_protocol,"protocol"));
	addSerializableAttribute (new AttributeString(&m_key,"key"));
	addSerializableAttribute (new AttributeSI8(&m_retries,"retries"));
	addSerializableAttribute (new AttributeSI8(&m_timeout,"timeout"));
}

void AAAGlobalChangeTacacsHostMessage::setHost(const string &host)
{
	m_host = host;
}

string AAAGlobalChangeTacacsHostMessage::getHost() const
{
	return (m_host);
}

void AAAGlobalChangeTacacsHostMessage::setPort(const SI32 &port)
{
	m_port  =  port;
}

SI32 AAAGlobalChangeTacacsHostMessage::getPort() const
{
	return (m_port);
}

void AAAGlobalChangeTacacsHostMessage::setProtocol(const SI8 &protocol)
{
	m_protocol = protocol;
}

SI8 AAAGlobalChangeTacacsHostMessage::getProtocol() const
{
	return (m_protocol);
}

void AAAGlobalChangeTacacsHostMessage::setKey(const string &key)
{
	m_key = key;
}

string AAAGlobalChangeTacacsHostMessage::getKey() const
{
	return (m_key);
}

void AAAGlobalChangeTacacsHostMessage::setRetries(const SI8 &retries)
{
	m_retries = retries;
}

SI8 AAAGlobalChangeTacacsHostMessage::getRetries() const
{
	return (m_retries);
}

void AAAGlobalChangeTacacsHostMessage::setTimeout(const SI8 &timeout)
{
	m_timeout  =  timeout;
}

SI8 AAAGlobalChangeTacacsHostMessage::getTimeout() const
{
	return (m_timeout);
}

}
