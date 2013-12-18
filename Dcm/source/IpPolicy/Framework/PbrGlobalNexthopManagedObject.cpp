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
 *   Author : debojyoti                                                     *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "IpPolicy/Framework/PbrGlobalNexthopManagedObject.h"
#include "vcs.h"

namespace DcmNs
{

// ************* PbrGlobalNexthopManagedObject ************

    PbrGlobalNexthopManagedObject::PbrGlobalNexthopManagedObject(IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
        : PrismElement  (pIpPolicyLocalObjectManager),
        PrismPersistableObject (PbrGlobalNexthopManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpPolicyLocalObjectManager),
        DcmManagedObject (pIpPolicyLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpPolicyLocalObjectManager)
    {
	  	m_index = 0;
    }

    PbrGlobalNexthopManagedObject::~PbrGlobalNexthopManagedObject ()
    {
    }

    string  PbrGlobalNexthopManagedObject::getClassName()
    {
        return ("PbrGlobalNexthopManagedObject");
    }

    void  PbrGlobalNexthopManagedObject::setupAttributesForPersistence()
    {

        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_set_next_hop_address, "next_hop_address"));
		addPersistableAttribute (new AttributeUI64(&m_index, "index"));

        setUserTagForAttribute("next_hop_address", ip_policy_next_hop);  //PBR not supported in nos4.0.0
    }

    void  PbrGlobalNexthopManagedObject::setupAttributesForCreate()
    {

        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_set_next_hop_address, "next_hop_address"));
		addPersistableAttributeForCreate  (new AttributeUI64(&m_index, "index"));
    }

    void  PbrGlobalNexthopManagedObject::setupKeys() {
        vector<string> keyName;
        keyName.push_back("next_hop_address");
        setUserDefinedKeyCombination (keyName);
    }

    void  PbrGlobalNexthopManagedObject::setSet_next_hop_address(const IpV4Address &set_next_hop_address)
    {
        //m_set_next_hop_address.setIpV4AddressValue(set_next_hop_address);
        //m_set_next_hop_address.setIsUserConfigured(true);
        m_set_next_hop_address = set_next_hop_address;
    }

    void  PbrGlobalNexthopManagedObject::unsetSet_next_hop_address()
    {
        //m_set_next_hop_address.setIsUserConfigured(false);
    }

    IpV4Address  PbrGlobalNexthopManagedObject::getSet_next_hop_address() const
    {
        //return (m_set_next_hop_address.getIpV4AddressValue());
        return (m_set_next_hop_address);
    }

    void PbrGlobalNexthopManagedObject::setIndex(UI64 index)
    {
        m_index = index;
    }

    UI64 PbrGlobalNexthopManagedObject::getIndex()
    {
        return (m_index);
    }
}
