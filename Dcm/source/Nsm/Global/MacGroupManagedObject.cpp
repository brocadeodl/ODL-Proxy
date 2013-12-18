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
 ***************************************************************************/

#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Nsm/Global/MacGroupManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "brocade-mac-address-table.h"

namespace DcmNs
{

	MacGroupManagedObject::MacGroupManagedObject (WaveObjectManager *pWaveObjectManager)
		: PrismElement (pWaveObjectManager),
		PrismPersistableObject (MacGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pWaveObjectManager),
		DcmManagedObject (pWaveObjectManager)
	{
	}

	MacGroupManagedObject::MacGroupManagedObject (WaveObjectManager *pWaveObjectManager, const UI32 &id)
		: PrismElement  (pWaveObjectManager),
		PrismPersistableObject (MacGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pWaveObjectManager),
		DcmManagedObject (pWaveObjectManager),
		m_id(id)
	{
	}

	MacGroupManagedObject::~MacGroupManagedObject()
	{
	}

	string MacGroupManagedObject::getClassName()
	{
		return ("MacGroupManagedObject");
	}

	void MacGroupManagedObject::setupAttributesForPersistence()
	{
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_id, "id", mac_address_table_mac_group_id));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<MacGroupEntryManagedObject>(&m_entry, "entry", getClassName(), getObjectId(), MacGroupEntryManagedObject::getClassName()));
	}

	void MacGroupManagedObject::setupAttributesForCreate()
	{
		DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_id, "id", mac_address_table_mac_group_id));
        addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<MacGroupEntryManagedObject>(&m_entry, "entry", getClassName(), getObjectId(), MacGroupEntryManagedObject::getClassName()));

		vector<string> keyName;
		keyName.push_back ("id");
		setUserDefinedKeyCombination (keyName);
	}

	void MacGroupManagedObject::setMacGroupId(const UI32 &id)
	{
		m_id = id;
	}

	UI32 MacGroupManagedObject::getMacGroupId() const
	{
		return m_id;
	}

	vector<WaveManagedObjectPointer<MacGroupEntryManagedObject> > MacGroupManagedObject::getMacGroupEntry() const
	{
		return m_entry;
	}
	
	MacGroupEntryManagedObject::MacGroupEntryManagedObject (WaveObjectManager *pWaveObjectManager)
		: PrismElement (pWaveObjectManager),
		PrismPersistableObject (MacGroupEntryManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pWaveObjectManager),
		DcmManagedObject (pWaveObjectManager)
	{
	}

	MacGroupEntryManagedObject::MacGroupEntryManagedObject (WaveObjectManager *pWaveObjectManager, const string &address)
		: PrismElement  (pWaveObjectManager),
		PrismPersistableObject (MacGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pWaveObjectManager),
		DcmManagedObject (pWaveObjectManager),
		m_address(address)
	{
	}

	MacGroupEntryManagedObject::~MacGroupEntryManagedObject()
	{
	}

	string MacGroupEntryManagedObject::getClassName()
	{
		return ("MacGroupEntryManagedObject");
	}

	void MacGroupEntryManagedObject::setupAttributesForPersistence()
	{
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_address, "address", mac_address_table_entry_address));
		addPersistableAttribute (new AttributeString(&m_mask, "mask", mac_address_table_entry_mask));
	}

	void MacGroupEntryManagedObject::setupAttributesForCreate()
	{
		DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeString(&m_address, "address", mac_address_table_entry_address));
		addPersistableAttributeForCreate (new AttributeString(&m_address, "mask", mac_address_table_entry_mask));

		vector<string> keyName;
		keyName.push_back ("address");
		setUserDefinedKeyCombination (keyName);
	}

	void MacGroupEntryManagedObject::setAddress(const string &address)
	{
		m_address = address;
	}

	string MacGroupEntryManagedObject::getAddress() const
	{
		return m_address;
	}

	void MacGroupEntryManagedObject::setMask(const string &mask)
	{
		m_mask = mask;
	}

	string MacGroupEntryManagedObject::getMask() const
	{
		return m_mask;
	}
}


