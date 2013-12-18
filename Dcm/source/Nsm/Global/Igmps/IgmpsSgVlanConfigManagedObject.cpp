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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Nsm/Global/Igmps/IgmpsSgVlanConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    IgmpsSgVlanConfigManagedObject::IgmpsSgVlanConfigManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (IgmpsSgVlanConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_ifType = 0;
		m_interface = 0;
    }

    IgmpsSgVlanConfigManagedObject::IgmpsSgVlanConfigManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const IpV4Address &groupAddress,const UI32 &interface,const UI32 &ifType,const string &ifName)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (IgmpsSgVlanConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_groupAddress    (groupAddress),
        m_interface    (interface),
        m_ifType    (ifType),
        m_ifName    (ifName)
    {
    }

    IgmpsSgVlanConfigManagedObject::~IgmpsSgVlanConfigManagedObject ()
    {
    }

    string  IgmpsSgVlanConfigManagedObject::getClassName()
    {
        return ("IgmpsSgVlanConfigManagedObject");
    }

    void  IgmpsSgVlanConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_interface,"interface"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_ifType,"ifType"));
        addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));

        setUserTagForAttribute("groupAddress", igmp_snooping_mcast_address);
        setUserTagForAttribute("interface", igmp_snooping_interface);
        setUserTagForAttribute("ifType", igmp_snooping_if_type);
        setUserTagForAttribute("ifName", igmp_snooping_value);
    }

    void  IgmpsSgVlanConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_interface,"interface"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_ifType,"ifType"));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));

        vector<string > keyName;
        keyName.push_back ("groupAddress");
        keyName.push_back ("interface");
        keyName.push_back ("ifType");
        keyName.push_back ("ifName");
        setUserDefinedKeyCombination (keyName);
    }

    void  IgmpsSgVlanConfigManagedObject::setGroupAddress(const IpV4Address &groupAddress)
    {
        m_groupAddress  =  groupAddress;
    }

    IpV4Address  IgmpsSgVlanConfigManagedObject::getGroupAddress() const
    {
        return (m_groupAddress);
    }

    void  IgmpsSgVlanConfigManagedObject::setInterface(const UI32 &interface)
    {
        m_interface  =  interface;
    }

    UI32  IgmpsSgVlanConfigManagedObject::getInterface() const
    {
        return (m_interface);
    }

    void  IgmpsSgVlanConfigManagedObject::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  IgmpsSgVlanConfigManagedObject::getIfType() const
    {
        return (m_ifType);
    }

    void  IgmpsSgVlanConfigManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsSgVlanConfigManagedObject::getIfName() const
    {
        return (m_ifName);
    }

}
