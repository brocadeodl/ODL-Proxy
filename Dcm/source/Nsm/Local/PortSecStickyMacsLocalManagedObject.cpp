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

#include "Nsm/Local/PortSecStickyMacsLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

#include "brocade-interface.h"

namespace DcmNs
{

    PortSecStickyMacsLocalManagedObject::PortSecStickyMacsLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PortSecStickyMacsLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    PortSecStickyMacsLocalManagedObject::PortSecStickyMacsLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PortSecStickyMacsLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
    }

    PortSecStickyMacsLocalManagedObject::~PortSecStickyMacsLocalManagedObject ()
    {
    }

    string  PortSecStickyMacsLocalManagedObject::getClassName()
    {
        return ("PortSecStickyMacsLocalManagedObject");
    }

    void  PortSecStickyMacsLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_stickyMac, "stickyMac"));
        addPersistableAttribute (new AttributeUI32(&m_vlanId, "vlanId"));
        addPersistableAttribute (new AttributeString(&m_ifName, "ifName"));
		setUserTagForAttribute("stickyMac", brocade_interface_mac_address);
		setUserTagForAttribute("vlanId", brocade_interface_port_sec_vlan);
    }

    void  PortSecStickyMacsLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_stickyMac, "stickyMac", brocade_interface_mac_address));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanId, "vlanId", brocade_interface_port_sec_vlan));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName, "ifName"));

		vector<string> keyName;
        keyName.push_back("stickyMac");
        keyName.push_back("vlanId");
        setUserDefinedKeyCombination (keyName);
    }

    void PortSecStickyMacsLocalManagedObject::setStickyMac(const string &stickyMac)
    {
        m_stickyMac  =  stickyMac;
    }

    string PortSecStickyMacsLocalManagedObject::getStickyMac() const
    {
        return m_stickyMac;
    }

    void PortSecStickyMacsLocalManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string PortSecStickyMacsLocalManagedObject::getIfName() const
    {
        return m_ifName;
    }

    void PortSecStickyMacsLocalManagedObject::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  = vlanId;
    }

    UI32 PortSecStickyMacsLocalManagedObject::getVlanId() const
    {
        return m_vlanId;
    }
}