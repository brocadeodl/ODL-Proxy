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

#include "AAA/Global/PasswordEncryptionManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"


namespace DcmNs
{

PasswordEncryptionManagedObject::PasswordEncryptionManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (PasswordEncryptionManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

PasswordEncryptionManagedObject::PasswordEncryptionManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const bool &passwdEnc)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (PasswordEncryptionManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_passwdEnc (passwdEnc)
{
}

PasswordEncryptionManagedObject::~PasswordEncryptionManagedObject ()
{
}

string PasswordEncryptionManagedObject::getClassName()
{
	return ("PasswordEncryptionManagedObject");
}

void PasswordEncryptionManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeBool(&m_passwdEnc,"passwordEncryption", brcd_aaa_password_encryption));
}

void PasswordEncryptionManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeBool(&m_passwdEnc,"passwordEncryption", brcd_aaa_password_encryption));
}

void PasswordEncryptionManagedObject::setPasswordEncryption(const bool &passwdEnc)
{
	m_passwdEnc = passwdEnc;
}

bool PasswordEncryptionManagedObject::getPasswordEncryption() const
{
	return (m_passwdEnc);
}

}
