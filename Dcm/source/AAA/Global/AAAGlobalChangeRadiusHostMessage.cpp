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

#include "AAA/Global/AAAGlobalChangeRadiusHostMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

AAAGlobalChangeRadiusHostMessage::AAAGlobalChangeRadiusHostMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGERADIUSHOST)
{
}

AAAGlobalChangeRadiusHostMessage::AAAGlobalChangeRadiusHostMessage (const string &host,const SI32 &port,const AuthProtocol &protocol,const string &key,const SI32 &retransmit,const SI32 &timeout, const bool &portF, const bool &protocolF, const bool &retransmitF)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALCHANGERADIUSHOST),
	m_host    (host),
	m_port    (port),
	m_protocol    (protocol),
	m_key    (key),
	m_retransmit    (retransmit),
	m_timeout    (timeout),
	m_portFlag    (portF),
	m_protocolFlag    (protocolF),
	m_retransmitFlag    (retransmitF)
{
}

AAAGlobalChangeRadiusHostMessage::~AAAGlobalChangeRadiusHostMessage ()
{
}

void  AAAGlobalChangeRadiusHostMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_host,"host"));
	addSerializableAttribute (new AttributeSI32(&m_port,"port"));
	addSerializableAttribute (new AttributeEnum((UI32*)(&m_protocol),"protocol"));
	addSerializableAttribute (new AttributeString(&m_key,"key"));
	addSerializableAttribute (new AttributeSI32(&m_retransmit,"retransmit"));
	addSerializableAttribute (new AttributeSI32(&m_timeout,"timeout"));
	addSerializableAttribute (new AttributeBool(&m_portFlag,"portFlag"));
	addSerializableAttribute (new AttributeBool(&m_protocolFlag,"protocolFlag"));
	addSerializableAttribute (new AttributeBool(&m_retransmitFlag,"retransmitFlag"));
}

void  AAAGlobalChangeRadiusHostMessage::setHost(const string &host)
{
	m_host  =  host;
}

string  AAAGlobalChangeRadiusHostMessage::getHost() const
{
	return (m_host);
}

void  AAAGlobalChangeRadiusHostMessage::setPort(const SI32 &port)
{
	m_port  =  port;
}

SI32  AAAGlobalChangeRadiusHostMessage::getPort() const
{
	return (m_port);
}

void  AAAGlobalChangeRadiusHostMessage::setProtocol(const AuthProtocol &protocol)
{
	m_protocol  =  protocol;
}

AuthProtocol  AAAGlobalChangeRadiusHostMessage::getProtocol() const
{
	return (m_protocol);
}

void  AAAGlobalChangeRadiusHostMessage::setKey(const string &key)
{
	m_key  =  key;
}

string  AAAGlobalChangeRadiusHostMessage::getKey() const
{
	return (m_key);
}

void  AAAGlobalChangeRadiusHostMessage::setRetransmit(const SI32 &retransmit)
{
	m_retransmit  =  retransmit;
}

SI32  AAAGlobalChangeRadiusHostMessage::getRetransmit() const
{
	return (m_retransmit);
}

void  AAAGlobalChangeRadiusHostMessage::setTimeout(const SI32 &timeout)
{
	m_timeout  =  timeout;
}

SI32  AAAGlobalChangeRadiusHostMessage::getTimeout() const
{
	return (m_timeout);
}

void  AAAGlobalChangeRadiusHostMessage::setPortFlag(const bool &portF)
{
	m_portFlag  =  portF;
}

bool  AAAGlobalChangeRadiusHostMessage::getPortFlag() const
{
	return (m_portFlag);
}

void  AAAGlobalChangeRadiusHostMessage::setProtocolFlag(const bool &protocolF)
{
	m_protocolFlag  =  protocolF;
}

bool  AAAGlobalChangeRadiusHostMessage::getProtocolFlag() const
{
	return (m_protocolFlag);
}

void  AAAGlobalChangeRadiusHostMessage::setRetransmitFlag(const bool &retransmitF)
{
	m_retransmitFlag  =  retransmitF;
}

bool  AAAGlobalChangeRadiusHostMessage::getRetransmitFlag() const
{
	return (m_retransmitFlag);
}

}
