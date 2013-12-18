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
#include "vcs.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "Nsm/Local/Layer3/NsmIpConfigManagedObject.h"
#include "Nsm/Local/Layer3/NsmIpAddrManagedObject.h"

namespace DcmNs
{
    class NsmIpAddrManagedObject;

    NsmIpConfigManagedObject::NsmIpConfigManagedObject(
     WaveObjectManager *pNsmLocalObjectManager)
     : PrismElement(pNsmLocalObjectManager),
        PrismPersistableObject(NsmIpConfigManagedObject::getClassName(), 
            DcmLocalManagedObject::getClassName()),
        WaveManagedObject(pNsmLocalObjectManager),
        DcmManagedObject(pNsmLocalObjectManager),
        WaveLocalManagedObjectBase(this),
        DcmLocalManagedObjectBase(this),
        DcmLocalManagedObject(pNsmLocalObjectManager)
    {
        m_mtu = NSM_IP_MTU_DEF_VAL;
        m_directed_broadcast = false;
        m_proxy_arp = false;
        m_arp_aging_timeout = NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL;
    }

    NsmIpConfigManagedObject::~NsmIpConfigManagedObject()
    {
    }

    string  NsmIpConfigManagedObject::getClassName()
    {
        return ("NsmIpConfigManagedObject");
    }

    void  NsmIpConfigManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence();

        addPersistableAttribute (new AttributeUI32(&m_mtu,"mtu", 
            ip_config_mtu));
        addPersistableAttribute (new AttributeBool(&m_proxy_arp,"proxy_arp", 
            ip_config_proxy_arp));
        addPersistableAttribute (new AttributeBool(&m_directed_broadcast, 
            "directed_broadcast", ip_config_directed_broadcast));
        addPersistableAttribute (new AttributeUI32(&m_arp_aging_timeout,
            "arp_aging_timeout", ip_config_arp_aging_timeout));
    }

    void  NsmIpConfigManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();

        addPersistableAttributeForCreate (new AttributeUI32(&m_mtu,"mtu", 
            ip_config_mtu));
        addPersistableAttributeForCreate (new AttributeBool(
            &m_proxy_arp,"proxy_arp", ip_config_proxy_arp));
        addPersistableAttributeForCreate (new AttributeBool(&m_directed_broadcast, 
            "directed_broadcast", ip_config_directed_broadcast));
        addPersistableAttributeForCreate (
            new AttributeUI32(&m_arp_aging_timeout,
            "arp_aging_timeout", ip_config_arp_aging_timeout));
    }

    void  NsmIpConfigManagedObject::setMtu(const UI32 &mtu)
    {
        m_mtu = mtu;
    }

    UI32  NsmIpConfigManagedObject::getMtu() const
    {
        return (m_mtu);
    }

    void  NsmIpConfigManagedObject::setDirectedBroadcast(const bool &flag)
    {
        m_directed_broadcast = flag;
    }

    bool  NsmIpConfigManagedObject::getDirectedBroadcast() const
    {
        return (m_directed_broadcast);
    }

    void  NsmIpConfigManagedObject::setProxyArp(const bool &flag)
    {
        m_proxy_arp = flag;
    }

    bool  NsmIpConfigManagedObject::getProxyArp() const
    {
        return (m_proxy_arp);
    }

    void  NsmIpConfigManagedObject::setArpAgingTimeout(const UI32 &timeout)
    {
        m_arp_aging_timeout = timeout;
    }

    UI32  NsmIpConfigManagedObject::getArpAgingTimeout() const
    {
        return (m_arp_aging_timeout);
    }

    bool NsmIpConfigManagedObject::hasNonDefaultConfig() 
    {
       if((m_mtu != NSM_IP_MTU_DEF_VAL) || (m_arp_aging_timeout != NSM_IP_ARP_AGING_TIMEOUT_DEF_VAL) ||
           (m_proxy_arp != false) || (m_directed_broadcast != false))
       {
           return true;
       }
       return false;
    }
} // DcmNs
