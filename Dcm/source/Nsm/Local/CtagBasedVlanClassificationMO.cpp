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

#include "Nsm/Local/CtagBasedVlanClassificationMO.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

	VlanClassificationMO::VlanClassificationMO (WaveObjectManager *pWaveObjectManager)
	: PrismElement (pWaveObjectManager),
	PrismPersistableObject (VlanClassificationMO::getClassName (), DcmLocalManagedObject::getClassName ()),
	WaveManagedObject (pWaveObjectManager),
	DcmManagedObject (pWaveObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pWaveObjectManager)
	{
	}

	VlanClassificationMO::~VlanClassificationMO()
	{
	}

	string VlanClassificationMO::getClassName()
	{
		return ("VlanClassificationMO");
	}

	void VlanClassificationMO::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_gvlan_trunk_vlan_id));
		addPersistableAttribute (new AttributeUI32(&m_ctagId, "ctagId", brocade_interface_gvlan_trunk_ctag_id));
	}

	void VlanClassificationMO::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_gvlan_trunk_vlan_id));
		addPersistableAttributeForCreate (new AttributeUI32(&m_ctagId, "ctagId", brocade_interface_gvlan_trunk_ctag_id));
	}

    void VlanClassificationMO::setupKeys ()
	{
		vector<string> keyName;
		keyName.push_back ("vlanId");
		keyName.push_back ("ctagId");
		setUserDefinedKeyCombination (keyName);
	}

	void VlanClassificationMO::setVlanId(const UI32 &vlanId)
	{
		m_vlanId  =  vlanId;
	}

	UI32 VlanClassificationMO::getVlanId() const
	{
		return m_vlanId;
	}

	void VlanClassificationMO::setCtagId(const UI32 &ctagId)
	{
		m_ctagId = ctagId;
	}

	UI32 VlanClassificationMO::getCtagId() const
	{
		return m_ctagId;
	}

	CtagVlanClassificationMO::CtagVlanClassificationMO (WaveObjectManager *pWaveObjectManager)
	: PrismElement (pWaveObjectManager),
	PrismPersistableObject (CtagVlanClassificationMO::getClassName (), VlanClassificationMO::getClassName ()),
	WaveManagedObject (pWaveObjectManager),
	DcmManagedObject (pWaveObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pWaveObjectManager),
	VlanClassificationMO (pWaveObjectManager)
	{
	}

	CtagVlanClassificationMO::~CtagVlanClassificationMO()
	{
	}

	string CtagVlanClassificationMO::getClassName()
	{
		return ("CtagVlanClassificationMO");
	}

	void CtagVlanClassificationMO::setupAttributesForPersistence()
	{
		VlanClassificationMO::setupAttributesForPersistence();
	}

	void  CtagVlanClassificationMO::setupAttributesForCreate()
	{
	}

	CtagVlanClassificationRemoveMO::CtagVlanClassificationRemoveMO (WaveObjectManager *pWaveObjectManager)
	: PrismElement (pWaveObjectManager),
	PrismPersistableObject (CtagVlanClassificationRemoveMO::getClassName (), VlanClassificationMO::getClassName ()),
	WaveManagedObject (pWaveObjectManager),
	DcmManagedObject (pWaveObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pWaveObjectManager),
	VlanClassificationMO (pWaveObjectManager)
	{
	}

	CtagVlanClassificationRemoveMO::~CtagVlanClassificationRemoveMO()
	{
	}

	string CtagVlanClassificationRemoveMO::getClassName()
	{
		return ("CtagVlanClassificationRemoveMO");
	}

	void CtagVlanClassificationRemoveMO::setupAttributesForPersistence()
	{
		VlanClassificationMO::setupAttributesForPersistence();
	}

	void  CtagVlanClassificationRemoveMO::setupAttributesForCreate()
	{
	}

	CtagPVlanClassificationMO::CtagPVlanClassificationMO (WaveObjectManager *pWaveObjectManager)
	: PrismElement (pWaveObjectManager),
	PrismPersistableObject (CtagPVlanClassificationMO::getClassName (), VlanClassificationMO::getClassName ()),
	WaveManagedObject (pWaveObjectManager),
	DcmManagedObject (pWaveObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pWaveObjectManager),
	VlanClassificationMO (pWaveObjectManager)
	{
	}

	CtagPVlanClassificationMO::~CtagPVlanClassificationMO()
	{
	}

	string CtagPVlanClassificationMO::getClassName()
	{
		return ("CtagPVlanClassificationMO");
	}

	void CtagPVlanClassificationMO::setupAttributesForPersistence()
	{
		VlanClassificationMO::setupAttributesForPersistence();
	}

	void  CtagPVlanClassificationMO::setupAttributesForCreate()
	{
	}

	CtagPVlanClassificationRemoveMO::CtagPVlanClassificationRemoveMO (WaveObjectManager *pWaveObjectManager)
	: PrismElement (pWaveObjectManager),
	PrismPersistableObject (CtagPVlanClassificationRemoveMO::getClassName (), VlanClassificationMO::getClassName ()),
	WaveManagedObject (pWaveObjectManager),
	DcmManagedObject (pWaveObjectManager),
	WaveLocalManagedObjectBase (this),
	DcmLocalManagedObjectBase (this),
	DcmLocalManagedObject (pWaveObjectManager),
	VlanClassificationMO (pWaveObjectManager)
	{
	}

	CtagPVlanClassificationRemoveMO::~CtagPVlanClassificationRemoveMO()
	{
	}

	string CtagPVlanClassificationRemoveMO::getClassName()
	{
		return ("CtagPVlanClassificationRemoveMO");
	}

	void CtagPVlanClassificationRemoveMO::setupAttributesForPersistence()
	{
		VlanClassificationMO::setupAttributesForPersistence();
	}

	void  CtagPVlanClassificationRemoveMO::setupAttributesForCreate()
	{
	}
}
