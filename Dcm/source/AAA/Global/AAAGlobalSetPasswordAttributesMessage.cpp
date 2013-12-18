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

#include "AAA/Global/AAAGlobalSetPasswordAttributesMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

AAAGlobalSetPasswordAttributesMessage::AAAGlobalSetPasswordAttributesMessage ()
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETPASSWORDATTRIBUTES)
{
}

AAAGlobalSetPasswordAttributesMessage::AAAGlobalSetPasswordAttributesMessage (const SI32 &minlength,const SI32 &maxretry,const SI32 &uppercase,const SI32 &lowercase,const SI32 &numerics,const SI32 &splchars, const SI32 &adminlockout)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETPASSWORDATTRIBUTES),
	m_minlength    (minlength),
	m_maxretry    (maxretry),
	m_uppercase    (uppercase),
	m_lowercase    (lowercase),
	m_numerics    (numerics),
	m_splchars    (splchars),
	m_adminlockout (adminlockout),
	m_reset (false)
{
}

AAAGlobalSetPasswordAttributesMessage::AAAGlobalSetPasswordAttributesMessage (const bool &reset)
	: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALSETPASSWORDATTRIBUTES),
	m_minlength    (PASSWD_VALUE_NOT_SET),
	m_maxretry    (PASSWD_VALUE_NOT_SET),
	m_uppercase    (PASSWD_VALUE_NOT_SET),
	m_lowercase    (PASSWD_VALUE_NOT_SET),
	m_numerics    (PASSWD_VALUE_NOT_SET),
	m_splchars    (PASSWD_VALUE_NOT_SET),
	m_adminlockout (PASSWD_VALUE_NOT_SET),
	m_reset (reset)
{
}

AAAGlobalSetPasswordAttributesMessage::~AAAGlobalSetPasswordAttributesMessage ()
{
}

void  AAAGlobalSetPasswordAttributesMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeSI32(&m_minlength,"minlength"));
	addSerializableAttribute (new AttributeSI32(&m_maxretry,"maxretry"));
	addSerializableAttribute (new AttributeSI32(&m_uppercase,"uppercase"));
	addSerializableAttribute (new AttributeSI32(&m_lowercase,"lowercase"));
	addSerializableAttribute (new AttributeSI32(&m_numerics,"numerics"));
	addSerializableAttribute (new AttributeSI32(&m_splchars,"splchars"));
	addSerializableAttribute (new AttributeSI32(&m_adminlockout,"adminlockout"));
	addSerializableAttribute (new AttributeBool(&m_reset,"reset"));
}

void  AAAGlobalSetPasswordAttributesMessage::setMinlength(const SI32 &minlength)
{
	m_minlength  =  minlength;
}

SI32  AAAGlobalSetPasswordAttributesMessage::getMinlength() const
{
	return (m_minlength);
}

void  AAAGlobalSetPasswordAttributesMessage::setMaxretry(const SI32 &maxretry)
{
	m_maxretry  =  maxretry;
}

SI32  AAAGlobalSetPasswordAttributesMessage::getMaxretry() const
{
	return (m_maxretry);
}

void  AAAGlobalSetPasswordAttributesMessage::setUppercase(const SI32 &uppercase)
{
	m_uppercase  =  uppercase;
}

SI32  AAAGlobalSetPasswordAttributesMessage::getUppercase() const
{
	return (m_uppercase);
}

void  AAAGlobalSetPasswordAttributesMessage::setLowercase(const SI32 &lowercase)
{
	m_lowercase  =  lowercase;
}

SI32  AAAGlobalSetPasswordAttributesMessage::getLowercase() const
{
	return (m_lowercase);
}

void  AAAGlobalSetPasswordAttributesMessage::setNumerics(const SI32 &numerics)
{
	m_numerics  =  numerics;
}

SI32  AAAGlobalSetPasswordAttributesMessage::getNumerics() const
{
	return (m_numerics);
}

void  AAAGlobalSetPasswordAttributesMessage::setSplchars(const SI32 &splchars)
{
	m_splchars  =  splchars;
}

SI32  AAAGlobalSetPasswordAttributesMessage::getSplchars() const
{
	return (m_splchars);
}

void AAAGlobalSetPasswordAttributesMessage::setAdminlockout(const SI32 &adminlockout)
{
	m_adminlockout = adminlockout;
}

SI32 AAAGlobalSetPasswordAttributesMessage::getAdminlockout() const
{
	return (m_adminlockout);
}

void  AAAGlobalSetPasswordAttributesMessage::setReset(const bool &reset)
{
	m_reset  =  reset;
}

bool  AAAGlobalSetPasswordAttributesMessage::getReset() const
{
	return (m_reset);
}

}
