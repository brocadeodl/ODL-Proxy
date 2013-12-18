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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "Nsm/Local/PortSecAllowedMacsLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

#include "brocade-interface.h"

namespace DcmNs
{

    PortSecAllowedMacsLocalManagedObject::PortSecAllowedMacsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PortSecAllowedMacsLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    PortSecAllowedMacsLocalManagedObject::PortSecAllowedMacsLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PortSecAllowedMacsLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
    }

    PortSecAllowedMacsLocalManagedObject::PortSecAllowedMacsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const string &allowedMac)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PortSecAllowedMacsLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_allowedMac    (allowedMac)
    {
    }

    PortSecAllowedMacsLocalManagedObject::PortSecAllowedMacsLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const string &allowedMac)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PortSecAllowedMacsLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager),
        m_allowedMac    (allowedMac)
    {
    }

    PortSecAllowedMacsLocalManagedObject::~PortSecAllowedMacsLocalManagedObject ()
    {
    }

    string  PortSecAllowedMacsLocalManagedObject::getClassName()
    {
        return ("PortSecurityAllowedMacsLocalManagedObject");
    }

    void  PortSecAllowedMacsLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_allowedMac, "allowed_mac"));
        addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId"));
        addPersistableAttribute (new AttributeString(&m_ifName, "ifName"));
		setUserTagForAttribute("allowed_mac", brocade_interface_mac_address);
		setUserTagForAttribute("vlanId", brocade_interface_port_sec_vlan);
    }

    void  PortSecAllowedMacsLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_allowedMac, "allowed_mac", brocade_interface_mac_address));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_port_sec_vlan));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName, "ifName"));

		vector<string> keyName;
        keyName.push_back("allowed_mac");
        keyName.push_back("vlanId");
        setUserDefinedKeyCombination (keyName);
    }

    void PortSecAllowedMacsLocalManagedObject::setAllowedMac(const string &allowedMac)
    {
        m_allowedMac  =  allowedMac;
    }

    string PortSecAllowedMacsLocalManagedObject::getAllowedMac() const
    {
        return m_allowedMac;
    }

    void PortSecAllowedMacsLocalManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string PortSecAllowedMacsLocalManagedObject::getIfName() const
    {
        return m_ifName;
    }

    void PortSecAllowedMacsLocalManagedObject::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  = vlanId;
    }

    UI32 PortSecAllowedMacsLocalManagedObject::getVlanId() const
    {
        return m_vlanId;
    }
}
