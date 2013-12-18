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
#include "IpPolicy/Framework/PbrVrfManagedObject.h"
#include "vcs.h"

namespace DcmNs
{

// ************* PbrVrfManagedObject ************

    PbrVrfManagedObject::PbrVrfManagedObject(IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
        : PrismElement  (pIpPolicyLocalObjectManager),
        PrismPersistableObject (PbrVrfManagedObject::getClassName (),
        DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pIpPolicyLocalObjectManager),
        DcmManagedObject (pIpPolicyLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pIpPolicyLocalObjectManager)
    {
	  	m_index = 0;
    }

    PbrVrfManagedObject::~PbrVrfManagedObject ()
    {
    }

    string  PbrVrfManagedObject::getClassName()
    {
        return ("PbrVrfManagedObject");
    }
    void  PbrVrfManagedObject::setupAttributesForPersistence()
    {

        DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeString(&m_vrfNameMO, "vrf_name"));
		addPersistableAttribute (new AttributeIpV4Address(&m_set_next_hop_address, "next_hop_address"));
		addPersistableAttribute (new AttributeUI64(&m_index, "index"));

		setUserTagForAttribute("vrf_name", ip_policy_vrf); //PBR not supported in nos4.0.0
		setUserTagForAttribute("next_hop_address", ip_policy_next_hop);
	}

    void  PbrVrfManagedObject::setupAttributesForCreate()
    {

        DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeString(&m_vrfNameMO, "vrf_name"));
		addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_set_next_hop_address, "next_hop_address"));
		addPersistableAttributeForCreate  (new AttributeUI64(&m_index, "index"));
	}

    void  PbrVrfManagedObject::setupKeys() {
        vector<string> keyName;
        keyName.push_back("vrf_name");
        keyName.push_back("next_hop_address");
        setUserDefinedKeyCombination (keyName);
    }

    void PbrVrfManagedObject::setVrf (const string &vrfName)
    {
        //m_vrfNameMO.setStringValue(vrfName);
        //m_vrfNameMO.setIsUserConfigured(true);
	  	m_vrfNameMO = vrfName;
    }

    void PbrVrfManagedObject::unsetVrf()
    {
        //m_vrfNameMO.setIsUserConfigured(false);
    }

    string PbrVrfManagedObject::getVrfNameMO() const
    {
        //return (m_vrfNameMO.getStringValue());
	  	return (m_vrfNameMO);
    }




    void  PbrVrfManagedObject::setSet_next_hop_address(const IpV4Address &set_next_hop_address)
    {
        //m_set_next_hop_address.setIpV4AddressValue(set_next_hop_address);
        //m_set_next_hop_address.setIsUserConfigured(true);
	  	m_set_next_hop_address = set_next_hop_address;
    }

    void  PbrVrfManagedObject::unsetSet_next_hop_address()
    {
        //m_set_next_hop_address.setIsUserConfigured(false);
    }

    IpV4Address  PbrVrfManagedObject::getSet_next_hop_address() const
    {
        //return (m_set_next_hop_address.getIpV4AddressValue());
	  	return (m_set_next_hop_address);
    }

	void PbrVrfManagedObject::setIndex(UI64 index)
	{
	  	m_index = index;
	}

	UI64 PbrVrfManagedObject::getIndex()
	{
	  	return (m_index);
	}
}
	
