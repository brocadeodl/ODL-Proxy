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
 *   Copyright (C) 2010-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "FilterChangeUpdateLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

FilterChangeUpdateLocalManagedObject::FilterChangeUpdateLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
	: PrismElement  (pIpPolicyLocalObjectManager),
	PrismPersistableObject (FilterChangeUpdateLocalManagedObject::getClassName (),
	DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pIpPolicyLocalObjectManager),
	DcmManagedObject (pIpPolicyLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pIpPolicyLocalObjectManager)
{
}

FilterChangeUpdateLocalManagedObject::FilterChangeUpdateLocalManagedObject (IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager,const UI32 &filterUpdateDelay)
	: PrismElement  (pIpPolicyLocalObjectManager),
	PrismPersistableObject (FilterChangeUpdateLocalManagedObject::getClassName (),
	DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pIpPolicyLocalObjectManager),
	DcmManagedObject (pIpPolicyLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pIpPolicyLocalObjectManager),
	m_filterUpdateDelay     (filterUpdateDelay)
{
}

FilterChangeUpdateLocalManagedObject::~FilterChangeUpdateLocalManagedObject ()
{
}

string  FilterChangeUpdateLocalManagedObject::getClassName()
{
	return ("FilterChangeUpdateLocalManagedObject");
}

void  FilterChangeUpdateLocalManagedObject::setupAttributesForPersistence()
{
	DcmLocalManagedObject::setupAttributesForPersistence ();
	addPersistableAttribute (new AttributeUI32(&m_filterUpdateDelay,"filterUpdateDelay", ip_policy_filter_delay_value));
}

void  FilterChangeUpdateLocalManagedObject::setupAttributesForCreate()
{
	DcmLocalManagedObject::setupAttributesForCreate ();
	addPersistableAttributeForCreate (new AttributeUI32(&m_filterUpdateDelay,"filterUpdateDelay"));
}

void  FilterChangeUpdateLocalManagedObject::setupKeys() {
	vector<string> keyName;
	keyName.push_back("filterUpdateDelay");
	setUserDefinedKeyCombination (keyName);
}

void FilterChangeUpdateLocalManagedObject::setFilterUpdateDelay(const UI32 &filterUpdateDelay) {
	m_filterUpdateDelay = filterUpdateDelay;
}

UI32 FilterChangeUpdateLocalManagedObject::getFilterUpdateDelay()  const {
	return m_filterUpdateDelay;
}

}

