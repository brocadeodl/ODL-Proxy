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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "vcs.h"

#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"

// JH TO DO REMOVE LATER
#ifndef ip_config_secondary
#define ip_config_secondary 0x999999
#endif

namespace DcmNs
{

    NsmIpAddrManagedObject::NsmIpAddrManagedObject 
        (WaveObjectManager *pNsmLocalObjectManager)
        : PrismElement(pNsmLocalObjectManager),
        PrismPersistableObject(NsmIpAddrManagedObject::getClassName(), 
            DcmLocalManagedObject::getClassName()),
        WaveManagedObject(pNsmLocalObjectManager),
        DcmManagedObject(pNsmLocalObjectManager),
        WaveLocalManagedObjectBase(this),
        DcmLocalManagedObjectBase(this),
        DcmLocalManagedObject(pNsmLocalObjectManager)
    {
		m_ipv4AddrMask = "";
        m_secondary = false; 
        m_ospfPassive = false;
        m_ospfIgnore = false;
    }

    NsmIpAddrManagedObject::~NsmIpAddrManagedObject ()
    {
    }

    string  NsmIpAddrManagedObject::getClassName()
    {
        return ("NsmIpAddrManagedObject");
    }

    void  NsmIpAddrManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();

        addPersistableAttribute (
            new AttributeString(&m_ipv4AddrMask, "ipv4AddrMask"));
        addPersistableAttribute (new AttributeBool(&m_secondary, "secondary", 
            ip_config_secondary));
        addPersistableAttribute (new AttributeBool(&m_ospfPassive, "ospfPassive", 
            ip_config_ospf_passive));
        addPersistableAttribute (new AttributeBool(&m_ospfIgnore, "ospfIgnore", 
            ip_config_ospf_ignore));

       setUserTagForAttribute ("ipv4AddrMask", ip_config_address);
    }

    void  NsmIpAddrManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();

        addPersistableAttributeForCreate(
            new AttributeString(&m_ipv4AddrMask, "ipv4AddrMask"));
        addPersistableAttributeForCreate (
            new AttributeBool(&m_secondary, "secondary", 
            ip_config_secondary));
        addPersistableAttributeForCreate (
            new AttributeBool(&m_ospfPassive, "ospfPassive", 
            ip_config_ospf_passive));
        addPersistableAttributeForCreate (
            new AttributeBool(&m_ospfIgnore, "ospfIgnore", 
            ip_config_ospf_ignore));
    }

    void NsmIpAddrManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("ipv4AddrMask");
        setUserDefinedKeyCombination (keyName);
    }

    void  NsmIpAddrManagedObject::setIpv4AddrMask(const string &ipaddr)
    {
        m_ipv4AddrMask  =  ipaddr;
    }

    string  NsmIpAddrManagedObject::getIpv4AddrMask() const
    {
        return (m_ipv4AddrMask);
    }

    void  NsmIpAddrManagedObject::setSecond(const bool &sec)
    {
        m_secondary  =  sec;
    }

    bool  NsmIpAddrManagedObject::isSecond() const
    {
        return (m_secondary);
    }
    void NsmIpAddrManagedObject::setOspfPassive(const bool &ospf)
    {
        m_ospfPassive = ospf;
    }

    bool NsmIpAddrManagedObject::isOspfPassive() const
    {
        return (m_ospfPassive);
    }

    void NsmIpAddrManagedObject::setOspfIgnore(const bool &ospf)
    {       
        m_ospfIgnore = ospf;
    }
 
    bool NsmIpAddrManagedObject::isOspfIgnore() const
    {   
        return (m_ospfIgnore);
    }  	
				

} // DcmNs
