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

#include "AAA/Global/TacacsHostManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

namespace DcmNs
{

TacacsHostManagedObject::TacacsHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (TacacsHostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

TacacsHostManagedObject::TacacsHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &host, const UI16 &port, const AuthProtocol &protocol, const string &key, const UI8 &retries, const UI8 &timeout)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (TacacsHostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_host (host),
	m_port (port),
	m_protocol (protocol),
	m_key (key),
	m_retries (retries),
	m_timeout (timeout)
{
}

TacacsHostManagedObject::~TacacsHostManagedObject ()
{
}

string TacacsHostManagedObject::getClassName()
{
	return ("TacacsHostManagedObject");
}

void TacacsHostManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI16(&m_index,"index"));
	addPersistableAttribute (new AttributeString(&m_host,"host", brcd_aaa_hostname));
	addPersistableAttribute (new AttributeUI16(&m_port,"port", brcd_aaa_port));
    addPersistableAttribute (new AttributeEnum((UI32*)(&m_protocol),"protocol", brcd_aaa_protocol));
	addPersistableAttribute (new AttributeString(&m_key,"key", brcd_aaa_key));
	addPersistableAttribute (new AttributeUI8(&m_retries,"retries", brcd_aaa_retries));
	addPersistableAttribute (new AttributeUI8(&m_timeout,"timeout", brcd_aaa_timeout));
}

void TacacsHostManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeUI16(&m_index,"index"));
	addPersistableAttributeForCreate (new AttributeString(&m_host,"host", brcd_aaa_hostname));
	addPersistableAttributeForCreate (new AttributeUI16(&m_port,"port", brcd_aaa_port));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_protocol),"protocol", brcd_aaa_protocol));
	addPersistableAttributeForCreate (new AttributeString(&m_key,"key", brcd_aaa_key));
	addPersistableAttributeForCreate (new AttributeUI8(&m_retries,"retries", brcd_aaa_retries));
	addPersistableAttributeForCreate (new AttributeUI8(&m_timeout,"timeout", brcd_aaa_timeout));
}

void TacacsHostManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("host");
	setUserDefinedKeyCombination (key);
}

void TacacsHostManagedObject::setIndex(const UI16 &index)
{
	m_index = index;
}

UI16 TacacsHostManagedObject::getIndex() const
{
	return (m_index);
}

void TacacsHostManagedObject::setHost(const string &host)
{
	m_host = host;
}

string TacacsHostManagedObject::getHost() const
{
	return (m_host);
}

void TacacsHostManagedObject::setPort(const UI16 &port)
{
	m_port = port;
}

UI16 TacacsHostManagedObject::getPort() const
{
	return (m_port);
}

void TacacsHostManagedObject::setProtocol(const AuthProtocol &protocol)
{
	m_protocol = protocol;
}

AuthProtocol TacacsHostManagedObject::getProtocol() const
{
	return (m_protocol);
}

void TacacsHostManagedObject::setKey(const string &key)
{
	m_key = key;
}

string TacacsHostManagedObject::getKey() const
{
	return (m_key);
}

void TacacsHostManagedObject::setRetries(const UI8 &retries)
{
	m_retries = retries;
}

UI8 TacacsHostManagedObject::getRetries() const
{
	return (m_retries);
}

void TacacsHostManagedObject::setTimeout(const UI8 &timeout)
{
	m_timeout = timeout;
}

UI8 TacacsHostManagedObject::getTimeout() const
{
	return (m_timeout);
}

}
