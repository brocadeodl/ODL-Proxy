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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.		  *
 *   All rights reserved.												  *
 *   Author : cshah													 *
 **************************************************************************/

#include "VirtualIp/Global/VirtualIpGlobalManagedObject.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
//#include "VcsYangModel.h"
#include "vcs.h"
namespace DcmNs
{

	VirtualIpGlobalManagedObject::VirtualIpGlobalManagedObject (VcsFabObjectManager *pVcsFabObjectManager)
		: PrismElement  (pVcsFabObjectManager),
		PrismPersistableObject (VirtualIpGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pVcsFabObjectManager),
		DcmManagedObject (pVcsFabObjectManager)
	{
	}

	VirtualIpGlobalManagedObject::VirtualIpGlobalManagedObject (VcsFabObjectManager *pVcsFabObjectManager,const string &VipAddress)
		: PrismElement  (pVcsFabObjectManager),
		PrismPersistableObject (VirtualIpGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
		WaveManagedObject (pVcsFabObjectManager),
		DcmManagedObject (pVcsFabObjectManager),
		m_VipAddress	(VipAddress)
	{
	}

	VirtualIpGlobalManagedObject::~VirtualIpGlobalManagedObject ()
	{
	}

	string  VirtualIpGlobalManagedObject::getClassName()
	{
		return ("VirtualIpGlobalManagedObject");
	}

	void  VirtualIpGlobalManagedObject::setupAttributesForPersistence()
	{
		DcmManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_VipAddress,"VipAddress", vcs_address));
	}

	void  VirtualIpGlobalManagedObject::setupAttributesForCreate()
	{
		DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeString(&m_VipAddress,"VipAddress", vcs_address));
	}

	void  VirtualIpGlobalManagedObject::setVipAddress(const string &VipAddress)
	{
		m_VipAddress  =  VipAddress;
	}

	string  VirtualIpGlobalManagedObject::getVipAddress() const
	{
		return (m_VipAddress);
	}

}
