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
#include "AAA/Local/AAALocalChangeRadiusHostMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalChangeRadiusHostMessage::AAALocalChangeRadiusHostMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGERADIUSHOST)
{
}

AAALocalChangeRadiusHostMessage::AAALocalChangeRadiusHostMessage (const string &host, const SI32 &port, const UI32 &protocol, const string &key, const SI32 &retransmit, const SI32 &timeout)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCHANGERADIUSHOST),
	m_host    (host),
	m_port    (port),
	m_protocol    (protocol),
	m_key    (key),
	m_retransmit    (retransmit),
	m_timeout    (timeout)
{
}

AAALocalChangeRadiusHostMessage::~AAALocalChangeRadiusHostMessage ()
{
}

void  AAALocalChangeRadiusHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeSI32(&m_port,"port"));
	addSerializableAttribute (new AttributeUI32(&m_protocol,"protocol"));
	addSerializableAttribute (new AttributeString(&m_key,"key"));
	addSerializableAttribute (new AttributeSI32(&m_retransmit,"retransmit"));
	addSerializableAttribute (new AttributeSI32(&m_timeout,"timeout"));
}

void  AAALocalChangeRadiusHostMessage::setHost(const string &host)
{
	m_host  =  host;
}

string  AAALocalChangeRadiusHostMessage::getHost() const
{
	return (m_host);
}

void  AAALocalChangeRadiusHostMessage::setPort(const SI32 &port)
{
	m_port  =  port;
}

SI32  AAALocalChangeRadiusHostMessage::getPort() const
{
	return (m_port);
}

void  AAALocalChangeRadiusHostMessage::setProtocol(const UI32 &protocol)
{
	m_protocol  =  protocol;
}

UI32  AAALocalChangeRadiusHostMessage::getProtocol() const
{
	return (m_protocol);
}

void  AAALocalChangeRadiusHostMessage::setKey(const string &key)
{
	m_key  =  key;
}

string  AAALocalChangeRadiusHostMessage::getKey() const
{
	return (m_key);
}

void  AAALocalChangeRadiusHostMessage::setRetransmit(const SI32 &retransmit)
{
	m_retransmit  =  retransmit;
}

SI32  AAALocalChangeRadiusHostMessage::getRetransmit() const
{
	return (m_retransmit);
}

void  AAALocalChangeRadiusHostMessage::setTimeout(const SI32 &timeout)
{
	m_timeout  =  timeout;
}

SI32  AAALocalChangeRadiusHostMessage::getTimeout() const
{
	return (m_timeout);
}

}
