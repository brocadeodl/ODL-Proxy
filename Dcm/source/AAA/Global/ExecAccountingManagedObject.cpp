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
 *   Author : singhb	                                                   *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"

#include "AAA/Global/ExecAccountingManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"

using namespace WaveNs;

namespace DcmNs
{

ExecAccountingManagedObject::ExecAccountingManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (ExecAccountingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

ExecAccountingManagedObject::ExecAccountingManagedObject(AAAGlobalObjectManager *pAAAGlobalObjectManager,const AccServerType &accSrvType)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (ExecAccountingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	/* m_accType    (accType),*/
	m_accSrvType    (accSrvType)
{
}

ExecAccountingManagedObject::~ExecAccountingManagedObject ()
{
}

string  ExecAccountingManagedObject::getClassName()
{
	return ("ExecAccountingManagedObject");
}

void  ExecAccountingManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	// addPersistableAttribute (new AttributeEnum((UI32*)(&m_accType),"accType", brcd_aaa_accounting_type));
	addPersistableAttribute (new AttributeEnum((UI32*)(&m_accSrvType), "accSrvType", brcd_aaa_server_type));
}

void  ExecAccountingManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	// addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_accType),"accType", brcd_aaa_accounting_type));
	addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_accSrvType), "accSrvType", brcd_aaa_server_type));
}

/*void  ExecAccountingManagedObject::setAccType(const AccType &accType)
{
	m_accType =  accType;
}

AccType  ExecAccountingManagedObject::getAccType() const
{
	return (m_accType);
}*/

void  ExecAccountingManagedObject::setAccSrvType(const AccServerType &accSrvType)
{
	m_accSrvType =  accSrvType;
}

AccServerType  ExecAccountingManagedObject::getAccSrvType() const
{
	return (m_accSrvType);
}

}
