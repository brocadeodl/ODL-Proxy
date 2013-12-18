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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/ContainerInList/InterfaceContainerInListLocalManagedObject.h"
#include "Interface/ContainerInList/InterfaceContainerInListTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeSI64.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "interface_container_in_list.h"

namespace DcmNs
{

InterfaceContainerInListLocalManagedObject::InterfaceContainerInListLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceContainerInListLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceContainerInListLocalManagedObject::~InterfaceContainerInListLocalManagedObject ()
{
}

string InterfaceContainerInListLocalManagedObject::getClassName ()
{
    return ("InterfaceContainerInListLocalManagedObject");
}

void InterfaceContainerInListLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeIpV4Address   (&m_ip,       	"ip",       interface_container_in_list_ip));
    addPersistableAttribute (new AttributeSI32          (&m_mtu32,      "mtu32",    interface_container_in_list_mtu32));
    addPersistableAttribute (new AttributeWorldWideName (&m_wwn,        "wwn",      interface_container_in_list_wwn));
	addPersistableAttribute (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
	addPersistableAttribute (new AttributeSI32(&m_fcFabricId,"fcFabricId",interface_container_in_list_fabricid));
	addPersistableAttribute (new AttributeSI32(&m_ethIpAddress,"ethIpAddress",interface_container_in_list_ipaddress));
    
    setUserTagForAttribute ("name", interface_container_in_list_name);
}

void InterfaceContainerInListLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeIpV4Address   (&m_ip,       "ip",       interface_container_in_list_ip));
    addPersistableAttributeForCreate (new AttributeSI32          (&m_mtu32,    "mtu32",    interface_container_in_list_mtu32));
    addPersistableAttributeForCreate (new AttributeWorldWideName (&m_wwn,      "wwn",      interface_container_in_list_wwn));
	addPersistableAttributeForCreate (new AttributeUI32(&m_configTypeChoice,"configTypeChoice"));
	addPersistableAttributeForCreate (new AttributeSI32          (&m_fcFabricId,        "fcFabricId",       interface_container_in_list_fabricid));
	addPersistableAttributeForCreate (new AttributeSI32          (&m_ethIpAddress,      "ethIpAddress",     interface_container_in_list_ipaddress));

    vector<string > keyName;
    keyName.push_back ("name");
    setUserDefinedKeyCombination (keyName);
}
                
UI32 InterfaceContainerInListLocalManagedObject::getCase (const UI32 &userChoiceWyserTag)
{
    trace (TRACE_LEVEL_DEBUG, "InterfaceContainerInListLocalManagedObject::getCase: Entering ..."); 
    
    if (FIBRECHANNEL_TYPE == m_configTypeChoice)
    {
        return interface_container_in_list_fibrechannel; 
    }
    else if (ETHERNET_TYPE == m_configTypeChoice)
    {
        return interface_container_in_list_ethernet;
    }
    
    return 0; 
}

IpV4Address InterfaceContainerInListLocalManagedObject::getIp() const
{
    return (m_ip);
}

void InterfaceContainerInListLocalManagedObject::setIp(const IpV4Address &ip)
{
    m_ip = ip;
}

SI32 InterfaceContainerInListLocalManagedObject::getMtu32 () const
{
    return (m_mtu32);
}

void InterfaceContainerInListLocalManagedObject::setMtu32 (const SI32 &mtu32)
{
    m_mtu32 = mtu32;
}

WorldWideName InterfaceContainerInListLocalManagedObject::getWwn () const
{
    return (m_wwn);
}

void InterfaceContainerInListLocalManagedObject::setWwn (const WorldWideName &wwn)
{
    m_wwn = wwn;
}

UI32 InterfaceContainerInListLocalManagedObject::getConfigTypeChoice () const
{
    return (m_configTypeChoice);
}

void InterfaceContainerInListLocalManagedObject::setConfigTypeChoice (const UI32 &configTypeChoice)
{
    m_configTypeChoice = configTypeChoice;
}
                
SI32 InterfaceContainerInListLocalManagedObject::getFcFabricId () const
{
    return (m_fcFabricId);
}

void InterfaceContainerInListLocalManagedObject::setFcFabricId (const SI32 &fcFabricId)
{
    m_fcFabricId = fcFabricId;
}
                
SI32 InterfaceContainerInListLocalManagedObject::getEthIpAddress () const
{
    return (m_ethIpAddress);
}

void InterfaceContainerInListLocalManagedObject::setEthIpAddress (const SI32 &ethIpAddress)
{
    m_ethIpAddress = ethIpAddress;
}

}

