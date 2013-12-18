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

/*
 *  RspanGVlanAccessPortManagedObject.cpp

 *  Copyright (C) 2005-2013 Brocade Communications Systems, Inc.
 *  All rights reserved. 

 *  Created on: Jun 9, 2013
 *  Author: sunniram
 */


#include "Nsm/Local/RspanGVlanAccessPortManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

	RspanMacAddressVlanClassificationMO::RspanMacAddressVlanClassificationMO (NsmLocalObjectManager *pNsmLocalObjectManager)
	: PrismElement  (pNsmLocalObjectManager),
	PrismPersistableObject (RspanMacAddressVlanClassificationMO::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pNsmLocalObjectManager),
	DcmManagedObject (pNsmLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	RspanMacAddressVlanClassificationMO::~RspanMacAddressVlanClassificationMO ()
	{
	}

	string  RspanMacAddressVlanClassificationMO::getClassName()
	{
		return ("RspanMacAddressVlanClassificationMO");
	}

	void  RspanMacAddressVlanClassificationMO::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_accessVlanId, "vlanId", brocade_interface_rspan_mac_address_classification_vlan));
		addPersistableAttribute (new AttributeString(&m_mac, "mac", brocade_interface_rspan_mac_address_classification_address));
	}

	void  RspanMacAddressVlanClassificationMO::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_accessVlanId, "vlanId", brocade_interface_rspan_mac_address_classification_vlan));
		addPersistableAttributeForCreate (new AttributeString(&m_mac, "mac", brocade_interface_rspan_mac_address_classification_address));

		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("mac");
		setUserDefinedKeyCombination (keyName);
	}

	void  RspanMacAddressVlanClassificationMO::setAccessVlanId(const UI32 &accessVlanId)
	{
		m_accessVlanId  =  accessVlanId;
	}

	UI32  RspanMacAddressVlanClassificationMO::getAccessVlanId() const
	{
		return (m_accessVlanId);
	}

	void  RspanMacAddressVlanClassificationMO::setMac(const string &mac)
	{
		m_mac = mac;
	}

	string  RspanMacAddressVlanClassificationMO::getMac() const
	{
		return (m_mac);
	}

	RspanMacGroupVlanClassificationMO::RspanMacGroupVlanClassificationMO (NsmLocalObjectManager *pNsmLocalObjectManager)
	: PrismElement (pNsmLocalObjectManager),
	PrismPersistableObject (RspanMacGroupVlanClassificationMO::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pNsmLocalObjectManager),
	DcmManagedObject (pNsmLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	RspanMacGroupVlanClassificationMO::~RspanMacGroupVlanClassificationMO ()
	{
	}

	string RspanMacGroupVlanClassificationMO::getClassName()
	{
		return ("RspanMacGroupVlanClassificationMO");
	}

	void RspanMacGroupVlanClassificationMO::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_rspan_mac_group_classification_vlan));
		addPersistableAttribute (new AttributeUI32(&m_macGroupId, "macGroupId", brocade_interface_rspan_mac_group_classification_group));
	}

	void RspanMacGroupVlanClassificationMO::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_rspan_mac_group_classification_vlan));
		addPersistableAttributeForCreate (new AttributeUI32(&m_macGroupId, "macGroupId", brocade_interface_rspan_mac_group_classification_group));

		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("macGroupId");
		setUserDefinedKeyCombination (keyName);
	}

	void RspanMacGroupVlanClassificationMO::setVlanId(const UI32 &vlanId)
	{
		m_vlanId = vlanId;
	}

	UI32 RspanMacGroupVlanClassificationMO::getVlanId() const
	{
		return m_vlanId;
	}

	void RspanMacGroupVlanClassificationMO::setMacGroupId(const UI32 &macGroupId)
	{
		m_macGroupId = macGroupId;
	}

	UI32 RspanMacGroupVlanClassificationMO::getMacGroupId() const
	{
		return m_macGroupId;
	}
}




