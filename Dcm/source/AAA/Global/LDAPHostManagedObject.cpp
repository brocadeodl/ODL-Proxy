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

#include "AAA/Global/LDAPHostManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

namespace DcmNs
{

LDAPHostManagedObject::LDAPHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (LDAPHostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

LDAPHostManagedObject::LDAPHostManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &host, const UI16 &port, const string &basedn, const UI8 &retries, const UI8 &timeout)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (LDAPHostManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_host (host),
	m_port (port),
	m_basedn (basedn),
	m_retries (retries),
	m_timeout (timeout)
{
}

LDAPHostManagedObject::~LDAPHostManagedObject ()
{
}

string LDAPHostManagedObject::getClassName()
{
	return ("LDAPHostManagedObject");
}

void LDAPHostManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI16(&m_index,"index"));
	addPersistableAttribute (new AttributeString(&m_host,"host", brcd_aaa_hostname));
	addPersistableAttribute (new AttributeUI16(&m_port,"port", brcd_aaa_port));
	addPersistableAttribute (new AttributeString(&m_basedn,"basedn", brcd_aaa_basedn));
	addPersistableAttribute (new AttributeUI8(&m_retries,"retries", brcd_aaa_retries));
	addPersistableAttribute (new AttributeUI8(&m_timeout,"timeout", brcd_aaa_timeout));
}

void LDAPHostManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeUI16(&m_index,"index"));
	addPersistableAttributeForCreate (new AttributeString(&m_host,"host", brcd_aaa_hostname));
	addPersistableAttributeForCreate (new AttributeUI16(&m_port,"port", brcd_aaa_port));
	addPersistableAttributeForCreate (new AttributeString(&m_basedn,"basedn", brcd_aaa_basedn));
	addPersistableAttributeForCreate (new AttributeUI8(&m_retries,"retries", brcd_aaa_retries));
	addPersistableAttributeForCreate (new AttributeUI8(&m_timeout,"timeout", brcd_aaa_timeout));
}

void LDAPHostManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("host");
	setUserDefinedKeyCombination (key);
}

void LDAPHostManagedObject::setIndex(const UI16 &index)
{
	m_index = index;
}

UI16 LDAPHostManagedObject::getIndex() const
{
	return (m_index);
}

void LDAPHostManagedObject::setHost(const string &host)
{
	m_host = host;
}

string LDAPHostManagedObject::getHost() const
{
	return (m_host);
}

void LDAPHostManagedObject::setPort(const UI16 &port)
{
	m_port = port;
}

UI16 LDAPHostManagedObject::getPort() const
{
	return (m_port);
}

void LDAPHostManagedObject::setBasedn(const string &basedn)
{
	m_basedn = basedn;
}

string LDAPHostManagedObject::getBasedn() const
{
	return (m_basedn);
}

void LDAPHostManagedObject::setRetries(const UI8 &retries)
{
	m_retries = retries;
}

UI8 LDAPHostManagedObject::getRetries() const
{
	return (m_retries);
}

void LDAPHostManagedObject::setTimeout(const UI8 &timeout)
{
	m_timeout = timeout;
}

UI8 LDAPHostManagedObject::getTimeout() const
{
	return (m_timeout);
}

}
