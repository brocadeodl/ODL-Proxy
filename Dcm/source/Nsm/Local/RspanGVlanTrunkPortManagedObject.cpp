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
 * RspanGVlanTrunkPortManagedObject.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: sunniram
 */


#include "Nsm/Local/RspanGVlanTrunkPortManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

	RspanGVlanTrunkPortManagedObject::RspanGVlanTrunkPortManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
	: PrismElement (pNsmLocalObjectManager),
	PrismPersistableObject (RspanGVlanTrunkPortManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pNsmLocalObjectManager),
	DcmManagedObject (pNsmLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	RspanGVlanTrunkPortManagedObject::~RspanGVlanTrunkPortManagedObject()
	{
	}

	string RspanGVlanTrunkPortManagedObject::getClassName()
	{
		return ("RspanGVlanTrunkPortManagedObject");
	}

	void RspanGVlanTrunkPortManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_rspan_gvlan_trunk_vlan_id));
		addPersistableAttribute (new AttributeUI32(&m_ctagId, "ctagId", brocade_interface_rspan_gvlan_trunk_ctag_id));
	}

	void  RspanGVlanTrunkPortManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_rspan_gvlan_trunk_vlan_id));
		addPersistableAttributeForCreate (new AttributeUI32(&m_ctagId, "ctagId", brocade_interface_rspan_gvlan_trunk_ctag_id));

		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("ctagId");
		setUserDefinedKeyCombination (keyName);
	}

	void RspanGVlanTrunkPortManagedObject::setVlanId(const UI32 &vlanId)
	{
		m_vlanId  =  vlanId;
	}

	UI32 RspanGVlanTrunkPortManagedObject::getVlanId() const
	{
		return m_vlanId;
	}

	void RspanGVlanTrunkPortManagedObject::setCtagId(const UI32 &ctagId)
	{
		m_ctagId = ctagId;
	}

	UI32 RspanGVlanTrunkPortManagedObject::getCtagId() const
	{
		return m_ctagId;
	}

	RspanGVlanTrunkPortRemoveManagedObject::RspanGVlanTrunkPortRemoveManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
	: PrismElement (pNsmLocalObjectManager),
	PrismPersistableObject (RspanGVlanTrunkPortRemoveManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pNsmLocalObjectManager),
	DcmManagedObject (pNsmLocalObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pNsmLocalObjectManager)
	{
	}

	RspanGVlanTrunkPortRemoveManagedObject::~RspanGVlanTrunkPortRemoveManagedObject()
	{
	}

	string RspanGVlanTrunkPortRemoveManagedObject::getClassName()
	{
		return ("RspanGVlanTrunkPortRemoveManagedObject");
	}

	void RspanGVlanTrunkPortRemoveManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_rspan_gvlan_trunk_remove_vlan_id));
		addPersistableAttribute (new AttributeUI32(&m_ctagId, "ctagId", brocade_interface_rspan_gvlan_trunk_remove_ctag_id));
	}

	void  RspanGVlanTrunkPortRemoveManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_rspan_gvlan_trunk_remove_vlan_id));
		addPersistableAttributeForCreate (new AttributeUI32(&m_ctagId, "ctagId", brocade_interface_rspan_gvlan_trunk_remove_ctag_id));

		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("ctagId");
		setUserDefinedKeyCombination (keyName);
	}
}


