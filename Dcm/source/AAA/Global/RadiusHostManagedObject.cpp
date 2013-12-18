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

#include "AAA/Global/RadiusHostManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

using namespace WaveNs;

namespace DcmNs
{

RadiusHostManagedObject::RadiusHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (RadiusHostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

RadiusHostManagedObject::RadiusHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const string &host,const UI16 &port,const AuthProtocol &protocol,const string &key,const UI32 &retransmit,const UI32 &timeout)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (RadiusHostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_host    (host),
	m_port    (port),
	m_protocol    (protocol),
	m_key    (key),
	m_retransmit    (retransmit),
	m_timeout    (timeout)
{
}

RadiusHostManagedObject::~RadiusHostManagedObject ()
{
}

string  RadiusHostManagedObject::getClassName()
{
	return ("RadiusHostManagedObject");
}

void  RadiusHostManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI16(&m_index,"index"));
	addPersistableAttribute (new AttributeString(&m_host,"host", brcd_aaa_hostname));
	addPersistableAttribute (new AttributeUI16(&m_port,"port", brcd_aaa_auth_port));
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_protocol),"protocol", brcd_aaa_protocol));
	addPersistableAttribute (new AttributeString(&m_key,"key", brcd_aaa_key));
	addPersistableAttribute (new AttributeUI32(&m_retransmit,"retries", brcd_aaa_retries));
	addPersistableAttribute (new AttributeUI32(&m_timeout,"timeout", brcd_aaa_timeout));
}

void  RadiusHostManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate  (new AttributeUI16(&m_index,"index"));
	addPersistableAttributeForCreate  (new AttributeString(&m_host,"host", brcd_aaa_hostname));
	addPersistableAttributeForCreate  (new AttributeUI16(&m_port,"port", brcd_aaa_auth_port));
	addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_protocol),"protocol", brcd_aaa_protocol));
	addPersistableAttributeForCreate  (new AttributeString(&m_key,"key", brcd_aaa_key));
	addPersistableAttributeForCreate  (new AttributeUI32(&m_retransmit,"retries", brcd_aaa_retries));
	addPersistableAttributeForCreate  (new AttributeUI32(&m_timeout,"timeout", brcd_aaa_timeout));
}

void RadiusHostManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("host");
	setUserDefinedKeyCombination (key);
}

void  RadiusHostManagedObject::setIndex(const UI16 &index)
{
	m_index  =  index;
}

UI16  RadiusHostManagedObject::getIndex() const
{
	return (m_index);
}

void  RadiusHostManagedObject::setHost(const string &host)
{
	m_host  =  host;
}

string  RadiusHostManagedObject::getHost() const
{
	return (m_host);
}

void  RadiusHostManagedObject::setPort(const UI16 &port)
{
	m_port  =  port;
}

UI16  RadiusHostManagedObject::getPort() const
{
	return (m_port);
}

void  RadiusHostManagedObject::setProtocol(const AuthProtocol &protocol)
{
	m_protocol  =  protocol;
}

AuthProtocol  RadiusHostManagedObject::getProtocol() const
{
	return (m_protocol);
}

void  RadiusHostManagedObject::setKey(const string &key)
{
	m_key  =  key;
}

string  RadiusHostManagedObject::getKey() const
{
	return (m_key);
}

void  RadiusHostManagedObject::setRetransmit(const UI32 &retransmit)
{
	m_retransmit  =  retransmit;
}

UI32  RadiusHostManagedObject::getRetransmit() const
{
	return (m_retransmit);
}

void  RadiusHostManagedObject::setTimeout(const UI32 &timeout)
{
	m_timeout  =  timeout;
}

UI32  RadiusHostManagedObject::getTimeout() const
{
	return (m_timeout);
}

}
