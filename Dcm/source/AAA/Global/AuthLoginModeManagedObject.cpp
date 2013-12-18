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

#include "AAA/Global/AuthLoginModeManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

using namespace WaveNs;

namespace DcmNs
{

AuthLoginModeManagedObject::AuthLoginModeManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (AuthLoginModeManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

AuthLoginModeManagedObject::AuthLoginModeManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager,const AAAServerType &first,const AAAServerType &second)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (AuthLoginModeManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_first    (first),
	m_second    (second)
{
}

AuthLoginModeManagedObject::~AuthLoginModeManagedObject ()
{
}

string  AuthLoginModeManagedObject::getClassName()
{
	return ("AuthLoginModeManagedObject");
}

void  AuthLoginModeManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_first),"first", brcd_aaa_first));
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_second), NO_AAA, "second", brcd_aaa_second));
}

void  AuthLoginModeManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_first),"first", brcd_aaa_first));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_second), NO_AAA, "second", brcd_aaa_second));
}

void  AuthLoginModeManagedObject::setFirst(const AAAServerType &first)
{
	m_first  =  first;
}

AAAServerType  AuthLoginModeManagedObject::getFirst() const
{
	return (m_first);
}

void  AuthLoginModeManagedObject::setSecond(const AAAServerType &second)
{
	m_second  =  second;
}

AAAServerType  AuthLoginModeManagedObject::getSecond() const
{
	return (m_second);
}

}
