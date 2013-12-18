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
#include "Qos/Global/NasServerIpVlanManagedObject.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include "brocade-qos.h"

namespace DcmNs
{

    NasServerIpVlanManagedObject::NasServerIpVlanManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasServerIpVlanManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_vlanNumber (DEFAULT_NAS_VLAN_NUMBER)
    {
    }

    NasServerIpVlanManagedObject::NasServerIpVlanManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager, const UI32 vlanNumber)
        : PrismElement  (pQosGlobalObjectManager),
        PrismPersistableObject (NasServerIpVlanManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pQosGlobalObjectManager),
        DcmManagedObject (pQosGlobalObjectManager),
		m_vlanNumber (vlanNumber)
    {
    }


    NasServerIpVlanManagedObject::~NasServerIpVlanManagedObject ()
    {
    }

    string  NasServerIpVlanManagedObject::getClassName()
    {
        return ("NasServerIpVlanManagedObject");
    }

    void  NasServerIpVlanManagedObject::setupAttributesForPersistence()
    {
		DcmManagedObject::setupAttributesForPersistence();
		addPersistableAttribute (new AttributeUI32(&m_vlanNumber, "vlanNumber", qos_vlan_number)); 

    }

    void  NasServerIpVlanManagedObject::setupAttributesForCreate()
    {
		DcmManagedObject::setupAttributesForCreate();
		addPersistableAttributeForCreate (new AttributeUI32 (&m_vlanNumber, "vlanNumber", qos_vlan_number)); 
    }

	void NasServerIpVlanManagedObject::setupKeys()
	{
 		vector<string> keyName;
		keyName.push_back ("vlanNumber");
		PrismPersistableObject::setUserDefinedKeyCombination(keyName);
	}

	void NasServerIpVlanManagedObject::setVlanNumber (const UI32 &vlanNumber)
	{
    	m_vlanNumber = vlanNumber ;
	}

	UI32 NasServerIpVlanManagedObject::getVlanNumber ()
	{
    	return (m_vlanNumber);
	}
}
