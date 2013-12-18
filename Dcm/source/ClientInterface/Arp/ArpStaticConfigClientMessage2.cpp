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

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Arp/ArpStaticConfigClientMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    ArpStaticConfigClientMessage::ArpStaticConfigClientMessage ()
        : DcmManagementInterfaceMessage (ARPSTATICCONFIGCLIENT)
    {
    }

    ArpStaticConfigClientMessage::~ArpStaticConfigClientMessage ()
    {
    }

    void  ArpStaticConfigClientMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4Address(&m_IpAddress,"IpAddress"));
        addSerializableAttribute (new AttributeString(&m_MacAdress,"MacAdress"));
        addSerializableAttribute (new AttributeUI32(&m_InterfaceType,"InterfaceType"));
        addSerializableAttribute (new AttributeUI32(&m_PortChannelValue,"PortChannelValue"));
        addSerializableAttribute (new AttributeString(&m_GigabitValue,"GigabitValue"));
        addSerializableAttribute (new AttributeString(&m_TenGigabitValue,"TenGigabitValue"));
        addSerializableAttribute (new AttributeString(&m_FortyGigabitValue,"FortyGigabitValue"));
        addSerializableAttribute (new AttributeUI32(&m_VlanValue,"VlanValue"));
        addSerializableAttribute (new AttributeBool(&m_opCode,"opCode"));
    }

    void  ArpStaticConfigClientMessage::setIpAddress(const IpV4Address &IpAddress)
    {
        m_IpAddress  =  IpAddress;
    }

    IpV4Address  ArpStaticConfigClientMessage::getIpAddress() const
    {
        return (m_IpAddress);
    }

    void  ArpStaticConfigClientMessage::setMacAdress(const string &MacAdress)
    {
        m_MacAdress  =  MacAdress;
    }

    string  ArpStaticConfigClientMessage::getMacAdress() const
    {
        return (m_MacAdress);
    }

    void  ArpStaticConfigClientMessage::setInterfaceType(const UI32 &InterfaceType)
    {
        m_InterfaceType  =  InterfaceType;
    }

    UI32  ArpStaticConfigClientMessage::getInterfaceType() const
    {
        return (m_InterfaceType);
    }

    void  ArpStaticConfigClientMessage::setPortChannelValue(const UI32 &PortChannelValue)
    {
        m_PortChannelValue  =  PortChannelValue;
    }

    UI32  ArpStaticConfigClientMessage::getPortChannelValue() const
    {
        return (m_PortChannelValue);
    }

	void  ArpStaticConfigClientMessage::setGigabitValue(const string &GigabitValue)
    {
        m_GigabitValue  =  GigabitValue;
    }

    string  ArpStaticConfigClientMessage::getGigabitValue() const
    {
        return (m_GigabitValue);
    }


    void  ArpStaticConfigClientMessage::setTenGigabitValue(const string &TenGigabitValue)
    {
        m_TenGigabitValue  =  TenGigabitValue;
    }

    string  ArpStaticConfigClientMessage::getTenGigabitValue() const
    {
        return (m_TenGigabitValue);
    }

	void  ArpStaticConfigClientMessage::setFortyGigabitValue(const string &FortyGigabitValue)
    {
        m_FortyGigabitValue  =  FortyGigabitValue;
    }

    string  ArpStaticConfigClientMessage::getFortyGigabitValue() const
    {
        return (m_FortyGigabitValue);
    }


	void  ArpStaticConfigClientMessage::setVlanValue(const UI32 &VlanValue)
    {
        m_VlanValue  =  VlanValue;
    }

    UI32  ArpStaticConfigClientMessage::getVlanValue() const
    {
        return (m_VlanValue);
    }

	void  ArpStaticConfigClientMessage::setOpCode(const bool &opCode)
    {
        m_opCode  =  opCode;
    }

    bool  ArpStaticConfigClientMessage::getOpCode() const
    {
        return (m_opCode);
    }

    string ArpStaticConfigClientMessage::getTwoTouple (string ifName)
    {
        string twoTupleName = "";

        if (NsmUtils::isValidThreeTuple(ifName)) { 
                NsmUtils::getTwoTupleIfName(ifName, twoTupleName);
        } else {
            twoTupleName = ifName;
        }

        return twoTupleName;
    }


    const void *ArpStaticConfigClientMessage::getCStructureForInputs ()
    {
		string str_ipaddr;
        arp_static_config_msg_t  *msg = (arp_static_config_msg_t *) calloc (1, sizeof (arp_static_config_msg_t));

		msg->interface_type = m_InterfaceType;
		if(m_opCode) msg->negation = 1;
		else msg->negation = 0;
		if(msg->negation == 0){

			switch (m_InterfaceType)
			{
				case CONFIG_ARP_PORTCHANNEL:
					msg->interface_id = m_PortChannelValue;
					break;
				case CONFIG_ARP_VLAN:
					msg->interface_id = m_VlanValue;
					break;
				case CONFIG_ARP_GIGABIT:
					strncpy ((char *)msg->if_name,
							(const char*) getTwoTouple(m_GigabitValue).c_str(),
							ARP_DCM_IF_NAME_LEN);
					msg->if_name[ARP_DCM_IF_NAME_LEN - 1] = '\0';
					break;
				case CONFIG_ARP_TENGIGABIT:
					strncpy ((char *)msg->if_name,
							(const char*) getTwoTouple(m_TenGigabitValue).c_str(),
							ARP_DCM_IF_NAME_LEN);
					msg->if_name[ARP_DCM_IF_NAME_LEN - 1] = '\0';
					break;
				case CONFIG_ARP_FORTYGIGABIT:
					strncpy ((char *)msg->if_name,
							(const char*) getTwoTouple(m_FortyGigabitValue).c_str(),
							ARP_DCM_IF_NAME_LEN);
					msg->if_name[ARP_DCM_IF_NAME_LEN - 1] = '\0';
					break;
			}
		}
		str_ipaddr = m_IpAddress.toString();
		strncpy(msg->ip_address, str_ipaddr.c_str(), 15);
		strncpy(msg->vrf_ctxt, getPartitionName().c_str(), ARP_VRF_NAME_LEN);
		msg->ip_address[15] = '\0';

		strncpy((char *)(msg->mac_address), (const char*)(m_MacAdress.c_str()), ARP_MAC_ADDRESS_LEN);
		msg->mac_address[ARP_MAC_ADDRESS_LEN-1] = '\0';

        return msg;
    }

	void 
    ArpStaticConfigClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}






}
