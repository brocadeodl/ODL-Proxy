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
#include "Qos/Global/NasServerIpVrfManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include "brocade-qos.h"

namespace DcmNs
{

    NasServerIpVrfManagedObject::NasServerIpVrfManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasServerIpVrfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_vrfName(DEFAULT_NAS_VRF_NAME)
    {
    }

    NasServerIpVrfManagedObject::NasServerIpVrfManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager, string vrfName)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasServerIpVrfManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_vrfName(vrfName)
    {
    }


    NasServerIpVrfManagedObject::~NasServerIpVrfManagedObject ()
    {
    }

    string  NasServerIpVrfManagedObject::getClassName()
    {
        return ("NasServerIpVrfManagedObject");
    }

    void  NasServerIpVrfManagedObject::setupAttributesForPersistence()
    {
		DcmManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeString (&m_vrfName, "vrfName", qos_vrf_name)); 
    }

    void  NasServerIpVrfManagedObject::setupAttributesForCreate()
    {
		DcmManagedObject::setupAttributesForCreate();
		addPersistableAttributeForCreate (new AttributeString (&m_vrfName, "vrfName", qos_vrf_name)); 
    }

	void NasServerIpVrfManagedObject::setupKeys()
	{
 		vector<string> keyName;
		keyName.push_back ("vrfName");
		PrismPersistableObject::setUserDefinedKeyCombination(keyName);
	}

	void NasServerIpVrfManagedObject::setVrfName (const string &vrfName)
	{
    	m_vrfName = vrfName;
	}

	string NasServerIpVrfManagedObject::getVrfName ()
	{
    	return (m_vrfName);
	}
}
