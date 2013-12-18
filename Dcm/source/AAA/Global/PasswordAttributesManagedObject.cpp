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

#include "AAA/Global/PasswordAttributesManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

namespace DcmNs
{

PasswordAttributesManagedObject::PasswordAttributesManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (PasswordAttributesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

PasswordAttributesManagedObject::PasswordAttributesManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const UI32 &minlength,const UI32 &maxretry,const UI32 &uppercase,const UI32 &lowercase,const UI32 &numerics,const UI32 &splchars, const bool &adminlockout)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (PasswordAttributesManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_minlength    (minlength),
	m_maxretry    (maxretry),
	m_uppercase    (uppercase),
	m_lowercase    (lowercase),
	m_numerics    (numerics),
	m_splchars    (splchars),
	m_adminlockout (adminlockout)	
{
}

PasswordAttributesManagedObject::~PasswordAttributesManagedObject ()
{
}

string  PasswordAttributesManagedObject::getClassName()
{
	return ("PasswordAttributesManagedObject");
}

void  PasswordAttributesManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI32(&m_minlength,"minlength", brcd_aaa_min_length));
	addPersistableAttribute (new AttributeUI32(&m_maxretry,"maxretry", brcd_aaa_max_retry));
	addPersistableAttribute (new AttributeUI32(&m_uppercase,"uppercase", brcd_aaa_upper));
	addPersistableAttribute (new AttributeUI32(&m_lowercase,"lowercase", brcd_aaa_lower));
	addPersistableAttribute (new AttributeUI32(&m_numerics,"numerics", brcd_aaa_numeric));
	addPersistableAttribute (new AttributeUI32(&m_splchars,"splchars", brcd_aaa_special_char));
	addPersistableAttribute (new AttributeBool(&m_adminlockout,"adminlockout", brcd_aaa_admin_lockout_enable));
}

void  PasswordAttributesManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeUI32(&m_minlength,"minlength", brcd_aaa_min_length));
	addPersistableAttributeForCreate (new AttributeUI32(&m_maxretry,"maxretry", brcd_aaa_max_retry));
	addPersistableAttributeForCreate (new AttributeUI32(&m_uppercase,"uppercase", brcd_aaa_upper));
	addPersistableAttributeForCreate (new AttributeUI32(&m_lowercase,"lowercase", brcd_aaa_lower));
	addPersistableAttributeForCreate (new AttributeUI32(&m_numerics,"numerics", brcd_aaa_numeric));
	addPersistableAttributeForCreate (new AttributeUI32(&m_splchars,"splchars", brcd_aaa_special_char));
	addPersistableAttributeForCreate (new AttributeBool(&m_adminlockout,"adminlockout", brcd_aaa_admin_lockout_enable));
}

void  PasswordAttributesManagedObject::setMinlength(const UI32 &minlength)
{
	m_minlength  =  minlength;
}

UI32  PasswordAttributesManagedObject::getMinlength() const
{
	return (m_minlength);
}

void  PasswordAttributesManagedObject::setMaxretry(const UI32 &maxretry)
{
	m_maxretry  =  maxretry;
}

UI32  PasswordAttributesManagedObject::getMaxretry() const
{
	return (m_maxretry);
}

void  PasswordAttributesManagedObject::setUppercase(const UI32 &uppercase)
{
	m_uppercase  =  uppercase;
}

UI32  PasswordAttributesManagedObject::getUppercase() const
{
	return (m_uppercase);
}

void  PasswordAttributesManagedObject::setLowercase(const UI32 &lowercase)
{
	m_lowercase  =  lowercase;
}

UI32  PasswordAttributesManagedObject::getLowercase() const
{
	return (m_lowercase);
}

void  PasswordAttributesManagedObject::setNumerics(const UI32 &numerics)
{
	m_numerics  =  numerics;
}

UI32  PasswordAttributesManagedObject::getNumerics() const
{
	return (m_numerics);
}

void  PasswordAttributesManagedObject::setSplchars(const UI32 &splchars)
{
	m_splchars  =  splchars;
}

UI32  PasswordAttributesManagedObject::getSplchars() const
{
	return (m_splchars);
}

void PasswordAttributesManagedObject::setAdminlockout(const bool &adminlockout)
{
	m_adminlockout = adminlockout;
}

bool PasswordAttributesManagedObject::getAdminlockout() const
{
	return (m_adminlockout);
}

}
