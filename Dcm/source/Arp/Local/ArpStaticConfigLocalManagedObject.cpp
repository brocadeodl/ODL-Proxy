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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#include "Arp/Local/ArpStaticConfigLocalManagedObject.h"
#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpTypes.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    ArpStaticConfigLocalManagedObject::ArpStaticConfigLocalManagedObject (ArpLocalObjectManager *pArpLocalObjectManager)
		: PrismElement  (pArpLocalObjectManager),
		PrismPersistableObject (ArpStaticConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pArpLocalObjectManager),
		DcmManagedObject (pArpLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pArpLocalObjectManager),
		m_InterfaceName (0),
		m_PortChannelValue (0),
		m_GigabitValue (""),
		m_TenGigabitValue (""),
		m_FortyGigabitValue (""),
		m_VlanValue       (0)
	{
	}

    ArpStaticConfigLocalManagedObject::ArpStaticConfigLocalManagedObject (ArpLocalObjectManager *pArpLocalObjectManager,const IpV4Address &IpAddress,const string &MacAdress,const UI32 &InterfaceName,const UI32 &InterfaceType,const UI32 &PortChannelValue,const string &GigabitValue,const string &TenGigabitValue,const string &FortyGigabitValue,const UI32 &VlanValue)
		: PrismElement  (pArpLocalObjectManager),
		PrismPersistableObject (ArpStaticConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
		WaveManagedObject (pArpLocalObjectManager),
		DcmManagedObject (pArpLocalObjectManager),
		WaveLocalManagedObjectBase (this),
		DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pArpLocalObjectManager),
		m_IpAddress    (IpAddress),
		m_MacAdress    (MacAdress),
		m_InterfaceName (InterfaceName),
		m_InterfaceType    (InterfaceType),
		m_PortChannelValue    (PortChannelValue),
		m_GigabitValue     (GigabitValue),
		m_TenGigabitValue     (TenGigabitValue),
		m_FortyGigabitValue     (FortyGigabitValue),
		m_VlanValue           (VlanValue)
    {
    }

    ArpStaticConfigLocalManagedObject::~ArpStaticConfigLocalManagedObject ()
    {
    }

    string  ArpStaticConfigLocalManagedObject::getClassName()
    {
        return ("ArpStaticConfigLocalManagedObject");
    }

    void  ArpStaticConfigLocalManagedObject::setupAttributesForPersistence()
    {
		DcmLocalManagedObject::setupAttributesForPersistence ();
		addPersistableAttribute (new AttributeIpV4Address(&m_IpAddress,"IpAddress"));
		addPersistableAttribute (new AttributeString(&m_MacAdress,"MacAdress"));
		addPersistableAttribute (new AttributeEnum((UI32*)&m_InterfaceName,"InterfaceName"));
		addPersistableAttribute (new AttributeUI32(&m_InterfaceType,"InterfaceType"));
		addPersistableAttribute (new AttributeUI32(&m_PortChannelValue,"PortChannelValue"));
		addPersistableAttribute (new AttributeString(&m_GigabitValue,"GigabitValue"));
		addPersistableAttribute (new AttributeString(&m_TenGigabitValue,"TenGigabitValue"));
		addPersistableAttribute (new AttributeString(&m_FortyGigabitValue,"FortyGigabitValue"));
		addPersistableAttribute (new AttributeUI32(&m_VlanValue,"VlanValue"));
		setUserTagForAttribute("IpAddress", arp_arp_ip_address);
		setUserTagForAttribute("MacAdress", arp_mac_address_value);
		setUserTagForAttribute("InterfaceName", arp_interfacename);
		setUserTagForAttribute("InterfaceType", arp_interfacetype);
		setUserTagForAttribute("GigabitValue", arp_GigabitEthernet);
		setUserTagForAttribute("TenGigabitValue", arp_TenGigabitEthernet);
		setUserTagForAttribute("FortyGigabitValue", arp_FortyGigabitEthernet);
		//setUserTagForAttribute("PortChannelValue", arp_Port_channel);
		setUserTagForAttribute("VlanValue", arp_Ve);
    }

    void  ArpStaticConfigLocalManagedObject::setupAttributesForCreate()
    {
		DcmLocalManagedObject::setupAttributesForCreate ();
		addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_IpAddress,"IpAddress"));
		addPersistableAttributeForCreate  (new AttributeString(&m_MacAdress,"MacAdress"));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_InterfaceName,"InterfaceName"));
		addPersistableAttributeForCreate  (new AttributeUI32(&m_InterfaceType,"InterfaceType"));
		addPersistableAttributeForCreate  (new AttributeUI32(&m_PortChannelValue,"PortChannelValue"));
		addPersistableAttributeForCreate  (new AttributeString(&m_GigabitValue,"GigabitValue"));
		addPersistableAttributeForCreate  (new AttributeString(&m_TenGigabitValue,"TenGigabitValue"));
		addPersistableAttributeForCreate  (new AttributeString(&m_FortyGigabitValue,"FortyGigabitValue"));
		addPersistableAttributeForCreate  (new AttributeUI32(&m_VlanValue,"VlanValue"));
		
		vector<string > keyName;
		keyName.push_back ("IpAddress");
		setUserDefinedKeyCombination (keyName);
    }

    void  ArpStaticConfigLocalManagedObject::setIpAddress(const IpV4Address &IpAddress)
    {
        m_IpAddress  =  IpAddress;
    }

    IpV4Address  ArpStaticConfigLocalManagedObject::getIpAddress() const
    {
        return (m_IpAddress);
    }

    void  ArpStaticConfigLocalManagedObject::setMacAdress(const string &MacAdress)
    {
        m_MacAdress  =  MacAdress;
    }

    string  ArpStaticConfigLocalManagedObject::getMacAdress() const
    {
        return (m_MacAdress);
    }

	void  ArpStaticConfigLocalManagedObject::setInterfaceName(const UI32 &InterfaceName)
    {
        m_InterfaceName =  InterfaceName;
    }

    UI32  ArpStaticConfigLocalManagedObject::getInterfaceName() const
    {
        return (m_InterfaceName);
    }

    void  ArpStaticConfigLocalManagedObject::setInterfaceType(const UI32 &InterfaceType)
    {
        m_InterfaceType  =  InterfaceType;
    }

    UI32  ArpStaticConfigLocalManagedObject::getInterfaceType() const
    {
        return (m_InterfaceType);
    }

    void  ArpStaticConfigLocalManagedObject::setPortChannelValue(const UI32 &PortChannelValue)
    {
        m_PortChannelValue  =  PortChannelValue;
    }

    UI32  ArpStaticConfigLocalManagedObject::getPortChannelValue() const
    {
        return (m_PortChannelValue);
    }

    UI32  ArpStaticConfigLocalManagedObject::getVlanValue() const
    {
        return (m_VlanValue);
    }

	void  ArpStaticConfigLocalManagedObject::setVlanValue(const UI32 &VlanValue)
    {
        m_VlanValue  =  VlanValue;
    }

	string  ArpStaticConfigLocalManagedObject::getGigabitValue() const
    {
        return (m_GigabitValue);
    }

	void  ArpStaticConfigLocalManagedObject::setGigabitValue(const string &GigabitValue)
    {
        m_GigabitValue  =  GigabitValue;
    }


    string  ArpStaticConfigLocalManagedObject::getTenGigabitValue() const
    {
        return (m_TenGigabitValue);
    }

	void  ArpStaticConfigLocalManagedObject::setTenGigabitValue(const string &TenGigabitValue)
    {
        m_TenGigabitValue  =  TenGigabitValue;
    }

    string  ArpStaticConfigLocalManagedObject::getFortyGigabitValue() const
    {
        return (m_FortyGigabitValue);
    }

	void  ArpStaticConfigLocalManagedObject::setFortyGigabitValue(const string &FortyGigabitValue)
    {
        m_FortyGigabitValue  =  FortyGigabitValue;
    }


	UI32 ArpStaticConfigLocalManagedObject::getCase (const UI32 &userChoiceWyserTag)
	{
		trace (TRACE_LEVEL_DEBUG, "ArpStaticConfigLocalManagedObject::getcase: Entering ...");

		/*if ( CONFIG_ARP_PORTCHANNEL == m_InterfaceType)
		{
				return arp_Port_channel;
		}*/

		if ( CONFIG_ARP_GIGABIT == m_InterfaceType)
		{
				return arp_GigabitEthernet;
		}


		else if ( CONFIG_ARP_TENGIGABIT == m_InterfaceType)
		{
				return arp_TenGigabitEthernet;
		}

		else if ( CONFIG_ARP_FORTYGIGABIT == m_InterfaceType)
		{
				return arp_FortyGigabitEthernet;
		}



		else if ( CONFIG_ARP_VLAN == m_InterfaceType)
		{
				return arp_Ve;
		}

		return 0;
	}
}	
