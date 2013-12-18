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
 *   Author : krao                                                     *
 **************************************************************************/

#include "Nsm/Local/VRRPWorker/VRRPVirtualIPLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    VRRPVirtualIPLocalManagedObject::VRRPVirtualIPLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPVirtualIPLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    VRRPVirtualIPLocalManagedObject::VRRPVirtualIPLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const IpV4Address &virtual_ip)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPVirtualIPLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_virtual_ip    (virtual_ip)
    {
    }

    VRRPVirtualIPLocalManagedObject::~VRRPVirtualIPLocalManagedObject ()
    {
    }

    string  VRRPVirtualIPLocalManagedObject::getClassName()
    {
        return ("VRRPVirtualIPLocalManagedObject");
    }

    void  VRRPVirtualIPLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_virtual_ip,"virtual_ip", vrrp_virtual_ipaddr));
    }

    void  VRRPVirtualIPLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_virtual_ip,"virtual_ip", vrrp_virtual_ipaddr));
    }

	void VRRPVirtualIPLocalManagedObject::setupKeys()
	{
        vector<string> keyName;
		keyName.push_back ("virtual_ip");
		setUserDefinedKeyCombination (keyName);
	}

    void  VRRPVirtualIPLocalManagedObject::setVirtual_ip(const IpV4Address &virtual_ip)
    {
        m_virtual_ip  =  virtual_ip;
    }

    IpV4Address  VRRPVirtualIPLocalManagedObject::getVirtual_ip() const
    {
        return (m_virtual_ip);
    }

}
