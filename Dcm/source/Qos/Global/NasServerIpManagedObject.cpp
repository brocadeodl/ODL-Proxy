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
 *   Author : pbalacha                                                     *
 **************************************************************************/
#include "Qos/Global/NasServerIpManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp" 
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "brocade-qos.h"

namespace DcmNs
{

    NasServerIpManagedObject::NasServerIpManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasServerIpManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager)
    {
    }

    NasServerIpManagedObject::NasServerIpManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,
		const IpV4AddressNetworkMask serverIpAddressPrefix)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasServerIpManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_serverIpAddressPrefix (serverIpAddressPrefix)
    {
    }


    NasServerIpManagedObject::~NasServerIpManagedObject ()
    {
    }

    string  NasServerIpManagedObject::getClassName()
    {
        return ("NasServerIpManagedObject");
    }

    void  NasServerIpManagedObject::setupAttributesForPersistence()
    {
		DcmManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeIpV4AddressNetworkMask (&m_serverIpAddressPrefix, "serverIpAddressPrefix", qos_server_ip));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<NasServerIpVlanManagedObject>(&m_vlans,"vlans", getClassName (),
			  getObjectId (),NasServerIpVlanManagedObject::getClassName ()));
		addPersistableAttribute (new AttributeManagedObjectVectorComposition<NasServerIpVrfManagedObject>(&m_vrfs,"vrfs", getClassName (),
			  getObjectId (), NasServerIpVrfManagedObject::getClassName ()));
    }

    void  NasServerIpManagedObject::setupAttributesForCreate()
    {
		DcmManagedObject::setupAttributesForCreate();
		addPersistableAttributeForCreate (new AttributeIpV4AddressNetworkMask (&m_serverIpAddressPrefix, "serverIpAddressPrefix", qos_server_ip));
		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NasServerIpVlanManagedObject>(&m_vlans,"vlans", getClassName (),
			  getObjectId (),NasServerIpVlanManagedObject::getClassName ()));
		addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<NasServerIpVrfManagedObject>(&m_vrfs,"vrfs", getClassName (),
			  getObjectId (), NasServerIpVrfManagedObject::getClassName ()));
    }

	void NasServerIpManagedObject::setupKeys()
	{
 		vector<string> keyName;
		keyName.push_back ("serverIpAddressPrefix");
		PrismPersistableObject::setUserDefinedKeyCombination(keyName);
	}

	void NasServerIpManagedObject::setServerIpAddressPrefix (const IpV4AddressNetworkMask &serverIpAddressPrefix)
	{
    	m_serverIpAddressPrefix = serverIpAddressPrefix;
	}

	IpV4AddressNetworkMask NasServerIpManagedObject::getServerIpAddressPrefix()
	{
    	return (m_serverIpAddressPrefix);
	}

	vector<WaveManagedObjectPointer<NasServerIpVlanManagedObject> >  NasServerIpManagedObject::getVlans() const
    {
        return (m_vlans);
    }
	vector<WaveManagedObjectPointer<NasServerIpVrfManagedObject> >  NasServerIpManagedObject::getVrfs() const
    {
        return (m_vrfs);
    }

	NasServerIpVlanManagedObject* NasServerIpManagedObject::getVlanServerConfig(const UI32 &vlanNumber)
    {
        UI32 i;
        for (i = 0; i < m_vlans.size(); i++) {
            if (m_vlans[i]->getVlanNumber() == vlanNumber)
                return (m_vlans[i].operator ->());
        }
        return (NULL);
    }
	NasServerIpVrfManagedObject* NasServerIpManagedObject::getVrfServerConfig(const string &vrfName)
    {
        UI32 i;
        for (i = 0; i < m_vrfs.size(); i++) {
            if (m_vrfs[i]->getVrfName() == vrfName)
                return (m_vrfs[i].operator ->());
        }
        return (NULL);
    }
}
