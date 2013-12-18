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
 *   Author : rmuthigi                                                     *
 **************************************************************************/

#include "Nsm/Global/VlanClassifierActivateGlobalManagedObject.h"
#include "Nsm/Global/VlanClassifierGroupManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "vcs.h"

namespace DcmNs
{

    VlanClassifierActivateGlobalManagedObject::VlanClassifierActivateGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanClassifierActivateGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_dummy = 0;
		m_groupId = 0;
    }

    VlanClassifierActivateGlobalManagedObject::VlanClassifierActivateGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32  &groupId,const ObjectId &vlanId)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (VlanClassifierActivateGlobalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_groupId    (groupId),
    	m_vlanId     (vlanId)	
    {
		m_dummy  = 0;
    }

    VlanClassifierActivateGlobalManagedObject::~VlanClassifierActivateGlobalManagedObject ()
    {
    }

    string  VlanClassifierActivateGlobalManagedObject::getClassName()
    {
        return ("VlanClassifierActivateGlobalManagedObject");
    }

    void  VlanClassifierActivateGlobalManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
	    addPersistableAttribute (new AttributeUI32(&m_groupId,"groupId"));
		addPersistableAttribute (new AttributeEnum((UI32*)(&m_dummy),"dummy"));
		addPersistableAttribute (new AttributeObjectIdAssociation(&m_vlanId,"vlan", VlanIntfManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));

		setUserTagForAttribute("groupId", brocade_vlan_groupid);
		setUserTagForAttribute("dummy", brocade_vlan_vlan_name);
		setUserTagForAttribute("vlan", brocade_vlan_vlan);

    }

    void  VlanClassifierActivateGlobalManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeUI32(&m_groupId,"groupId", brocade_vlan_groupid));
		addPersistableAttributeForCreate (new AttributeEnum((UI32*)(&m_dummy),"dummy"));
		addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_vlanId,"vlan", VlanIntfManagedObject::getClassName ()));
		addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));


		vector<string> keyName;
		keyName.push_back ("groupId");
		setUserDefinedKeyCombination (keyName);
		keyName.push_back ("dummy");
		setUserDefinedKeyCombination (keyName);
		keyName.push_back ("vlan");
		setUserDefinedKeyCombination (keyName);

    }

    void  VlanClassifierActivateGlobalManagedObject::setGroupId(const UI32 &groupId)
    {
        m_groupId  =  groupId;
    }
    UI32  VlanClassifierActivateGlobalManagedObject::getGroupId() const
    {
        return (m_groupId);
    }
    void  VlanClassifierActivateGlobalManagedObject::setdummy(const UI32 &dummy)
    {
        m_dummy  =  dummy;
    }
    UI32  VlanClassifierActivateGlobalManagedObject::getdummy() const
    {
        return (m_dummy);
    }
     void  VlanClassifierActivateGlobalManagedObject::setVlanId(const ObjectId &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    ObjectId  VlanClassifierActivateGlobalManagedObject::getVlanId() const
    {
        return (m_vlanId);
    }
    
    void  VlanClassifierActivateGlobalManagedObject::setIfName(const string ifName)
    {
        m_ifName  =  ifName;
    }

    string VlanClassifierActivateGlobalManagedObject::getIfName() const
    {
        return (m_ifName);
    }


}
