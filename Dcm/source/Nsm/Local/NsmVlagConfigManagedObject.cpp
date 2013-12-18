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
 *   Author : Amulya Makam                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmVlagConfigManagedObject.h"
#include "Nsm/Local/NsmLocalVlagConfigMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "ClientInterface/Nsm/NsmUtils.h"
namespace DcmNs
{

	NsmVlagConfigManagedObject::NsmVlagConfigManagedObject (WaveObjectManager *pWaveObjectManager)
		: PrismElement  (pWaveObjectManager),
		PrismPersistableObject (NsmVlagConfigManagedObject::getClassName (),
			DcmLocalManagedObject::getClassName()),
		WaveManagedObject (pWaveObjectManager),
		DcmManagedObject (pWaveObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pWaveObjectManager)
		{
			m_loadBalanceMode = 7;
			m_poId = 0;
		}

	NsmVlagConfigManagedObject::NsmVlagConfigManagedObject (WaveObjectManager *pWaveObjectManager,
		const UI32 &loadBalanceMode,const UI32 &poId)  : PrismElement  (pWaveObjectManager),   PrismPersistableObject (NsmVlagConfigManagedObject::getClassName (),
		DcmLocalManagedObject::getClassName()),
		WaveManagedObject (pWaveObjectManager),
		DcmManagedObject (pWaveObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pWaveObjectManager),
		m_loadBalanceMode (loadBalanceMode),
		m_poId(poId)
	{
	}

	NsmVlagConfigManagedObject::~NsmVlagConfigManagedObject ()
	{
	}

	string  NsmVlagConfigManagedObject::getClassName()
	{
		return ("NsmVlagConfigManagedObject");
	}

	void  NsmVlagConfigManagedObject::setupAttributesForPersistence()
	{
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeEnum((UI32*)&m_loadBalanceMode,"loadBalanceMode",
				fabric_service_vlag_load_balance));
		addPersistableAttribute (new AttributeUI32(&m_poId,"poId",fabric_service_po_id));
	}


	void  NsmVlagConfigManagedObject::setupAttributesForCreate()
	{
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)&m_loadBalanceMode,"loadBalanceMode",
				fabric_service_vlag_load_balance));
		addPersistableAttributeForCreate (new AttributeUI32(&m_poId,"poId",fabric_service_po_id));

	}
	void NsmVlagConfigManagedObject::setupKeys()
	{
		vector<string> keyName;
		keyName.push_back("poId");
		setUserDefinedKeyCombination (keyName);
	}

	void  NsmVlagConfigManagedObject::setLoadBalanceMode(const UI32 &loadBalanceMode)
	{
		m_loadBalanceMode  =  loadBalanceMode;
	}

	UI32 NsmVlagConfigManagedObject::getLoadBalanceMode() const
	{
		return (m_loadBalanceMode);
	}

	void NsmVlagConfigManagedObject::setPoId(const UI32 &poId)
	{
		m_poId = poId;
	}

	UI32  NsmVlagConfigManagedObject::getPoId() const
	{
		return (m_poId);
	}

	Attribute *NsmVlagConfigManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
	{
		Attribute *pAttribute = NULL;
		pAttribute = new AttributeBool (true, "vlag");
		trace (TRACE_LEVEL_ERROR,
			string("NsmVlagConfigManagedObject::getAttributeByUserTagUserDefined: Entry "));
		return (pAttribute);
	}
}

