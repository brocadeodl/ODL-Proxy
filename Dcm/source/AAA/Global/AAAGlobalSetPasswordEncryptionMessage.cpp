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
 *   Author : Charanjith Reddy Kunduru                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"

#include "AAA/Global/AAAGlobalSetPasswordEncryptionMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

using namespace WaveNs;

namespace DcmNs
{

AAAGlobalSetPasswordEncryptionMessage::AAAGlobalSetPasswordEncryptionMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETPASSWORDENCRYPTION)
{
}

AAAGlobalSetPasswordEncryptionMessage::AAAGlobalSetPasswordEncryptionMessage (const bool &passwdEnc)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETPASSWORDENCRYPTION),
	m_passwdEnc (passwdEnc)
{
}

AAAGlobalSetPasswordEncryptionMessage::~AAAGlobalSetPasswordEncryptionMessage ()
{
}

void  AAAGlobalSetPasswordEncryptionMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeBool(&m_passwdEnc, "passwordEncryption"));
}

void AAAGlobalSetPasswordEncryptionMessage::setPasswordEncryption(const bool &passwdEnc)
{
	m_passwdEnc = passwdEnc;
}

bool AAAGlobalSetPasswordEncryptionMessage::getPasswordEncryption() const
{
	return (m_passwdEnc);
}

}
