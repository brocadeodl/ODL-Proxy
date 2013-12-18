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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kghanta                                                      *
 **************************************************************************/

#include "AAA/Global/MapRoleManagedObject.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/UserManagedObject.h"
#include "AAA/Global/AAAGlobalTypes.h"

#include "brocade-aaa.h"


namespace DcmNs
{

MapRoleManagedObject::MapRoleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (MapRoleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager)
{
}

MapRoleManagedObject::MapRoleManagedObject (AAAGlobalObjectManager *pAAAGlobalObjectManager, const string &group, const string &role)
	: PrismElement  (pAAAGlobalObjectManager),
	PrismPersistableObject (MapRoleManagedObject::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pAAAGlobalObjectManager),
	DcmManagedObject (pAAAGlobalObjectManager),
	m_group    (group),
	m_role    (role)
{
}

MapRoleManagedObject::~MapRoleManagedObject ()
{
}

string  MapRoleManagedObject::getClassName()
{
	return ("MapRoleManagedObject");
}

void  MapRoleManagedObject::setupAttributesForPersistence()
{
	DcmManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeString(&m_group,"adgroup", brcd_aaa_ad_group));
	addPersistableAttribute (new AttributeString(&m_role,"switchrole", brcd_aaa_switch_role));
}

void  MapRoleManagedObject::setupAttributesForCreate()
{
	DcmManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeString(&m_group,"adgroup", brcd_aaa_ad_group));
	addPersistableAttributeForCreate (new AttributeString(&m_role,"switchrole", brcd_aaa_switch_role));
}

void MapRoleManagedObject::setupKeys()
{
	vector<string> key;
	key.push_back ("adgroup");
	setUserDefinedKeyCombination (key);
}

void  MapRoleManagedObject::setGroup(const string &group)
{
	m_group  =  group;
}

string  MapRoleManagedObject::getGroup() const
{
	return (m_group);
}

void  MapRoleManagedObject::setRole(const string &role)
{
	m_role  =  role;
}

string  MapRoleManagedObject::getRole() const
{
	return (m_role);
}
}
