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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : Venkat														*
 ****************************************************************************/

#include "Nsm/Local/MacBasedVlanClassificationMO.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{
	MacAddressVlanClassificationMO::MacAddressVlanClassificationMO (NsmLocalObjectManager *pNsmLocalObjectManager)
	: PrismElement (pNsmLocalObjectManager),
	PrismPersistableObject (MacAddressVlanClassificationMO::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pNsmLocalObjectManager),
	DcmManagedObject (pNsmLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	MacAddressVlanClassificationMO::MacAddressVlanClassificationMO (NsmGlobalObjectManager *pNsmGlobalObjectManager)
	: PrismElement (pNsmGlobalObjectManager),
	PrismPersistableObject (MacAddressVlanClassificationMO::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pNsmGlobalObjectManager),
	DcmManagedObject (pNsmGlobalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmGlobalObjectManager)
	{
	}

	MacAddressVlanClassificationMO::~MacAddressVlanClassificationMO ()
	{
	}

	string MacAddressVlanClassificationMO::getClassName()
	{
		return ("MacAddressVlanClassificationMO");
	}

	void MacAddressVlanClassificationMO::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_accessVlanId, "vlanId", brocade_interface_access_gvlan));
		addPersistableAttribute (new AttributeString(&m_mac, "mac", brocade_interface_index_address));
	}

	void MacAddressVlanClassificationMO::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_accessVlanId, "vlanId", brocade_interface_access_gvlan));
		addPersistableAttributeForCreate (new AttributeString(&m_mac, "mac", brocade_interface_index_address));

		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("mac");
		setUserDefinedKeyCombination (keyName);
	}

	void MacAddressVlanClassificationMO::setAccessVlanId(const UI32 &accessVlanId)
	{
		m_accessVlanId = accessVlanId;
	}

	UI32 MacAddressVlanClassificationMO::getAccessVlanId() const
	{
		return m_accessVlanId;
	}

	void MacAddressVlanClassificationMO::setMac(const string &mac)
	{
		m_mac = mac;
	}

	string MacAddressVlanClassificationMO::getMac() const
	{
		return m_mac;
	}

	MacGroupVlanClassificationMO::MacGroupVlanClassificationMO (NsmLocalObjectManager *pNsmLocalObjectManager)
	: PrismElement (pNsmLocalObjectManager),
	PrismPersistableObject (MacGroupVlanClassificationMO::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pNsmLocalObjectManager),
	DcmManagedObject (pNsmLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	MacGroupVlanClassificationMO::MacGroupVlanClassificationMO (NsmGlobalObjectManager *pNsmGlobalObjectManager)
	: PrismElement (pNsmGlobalObjectManager),
	PrismPersistableObject (MacGroupVlanClassificationMO::getClassName (), DcmManagedObject::getClassName ()),
	WaveManagedObject (pNsmGlobalObjectManager),
	DcmManagedObject (pNsmGlobalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmGlobalObjectManager)
	{
	}

	MacGroupVlanClassificationMO::~MacGroupVlanClassificationMO ()
	{
	}

	string MacGroupVlanClassificationMO::getClassName()
	{
		return ("MacGroupVlanClassificationMO");
	}

	void MacGroupVlanClassificationMO::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_mac_group_classification_vlan));
		addPersistableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId", brocade_interface_mac_group_classification_group));
	}

	void MacGroupVlanClassificationMO::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_mac_group_classification_vlan));
		addPersistableAttributeForCreate (new AttributeUI32(&m_macGroupId, "macGroupId", brocade_interface_mac_group_classification_group));

		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("macGroupId");
		setUserDefinedKeyCombination (keyName);
	}

	void MacGroupVlanClassificationMO::setVlanId(const UI32 &vlanId)
	{
		m_vlanId = vlanId;
	}

	UI32 MacGroupVlanClassificationMO::getVlanId() const
	{
		return m_vlanId;
	}

	void MacGroupVlanClassificationMO::setMacGroupId(const UI32 &macGroupId)
	{
		m_macGroupId = macGroupId;
	}

	UI32 MacGroupVlanClassificationMO::getMacGroupId() const
	{
		return m_macGroupId;
	}
}

