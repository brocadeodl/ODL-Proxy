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

#include "AAA/Global/BannerManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

namespace DcmNs
{
BannerManagedObject::BannerManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement (pAAAGlobalObjectManager),
	PrismPersistableObject (BannerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

BannerManagedObject::BannerManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const string &login, const string &motd, const string &incoming)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (BannerManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_login (login),
        m_motd (motd),
        m_incoming (incoming)
{
}

BannerManagedObject::~BannerManagedObject ()
{
}

string BannerManagedObject::getClassName()
{
	return ("BannerManagedObject");
}

void BannerManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_login,"login", brcd_aaa_login));
        addPersistableAttribute (new AttributeString(&m_motd,"motd", brcd_aaa_motd));
        addPersistableAttribute (new AttributeString(&m_incoming,"incoming", brcd_aaa_incoming));
}

void BannerManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate  (new AttributeString(&m_login,"login", brcd_aaa_login));
        addPersistableAttributeForCreate  (new AttributeString(&m_motd,"motd", brcd_aaa_motd));
        addPersistableAttributeForCreate  (new AttributeString(&m_incoming,"incoming", brcd_aaa_incoming));
}

void BannerManagedObject::setLogin(const string &login)
{
	m_login = login;
}

string BannerManagedObject::getLogin() const
{
	return (m_login);
}

void BannerManagedObject::setMotd(const string &login)
{
        m_motd = login;
}

string BannerManagedObject::getMotd() const
{
        return (m_motd);
}

void BannerManagedObject::setIncoming(const string &login)
{
        m_incoming = login;
}

string BannerManagedObject::getIncoming() const
{
        return (m_incoming);
}


}
