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

#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpStaticConfigLocalMessage.h"
#include "Arp/Local/ArpTypes.h"

namespace DcmNs
{

    ArpStaticConfigLocalMessage::ArpStaticConfigLocalMessage ()
        : ManagementInterfaceMessage (ArpLocalObjectManager::getClassName (),ARPSTATICCONFIGLOCAL)
    {
    }

    ArpStaticConfigLocalMessage::ArpStaticConfigLocalMessage (const IpV4Address &IpAddress,const string &MacAdress,const UI32 &InterfaceType,const UI32 &PortChannelValue,const string &GigabitValue, const string &TenGigabitValue, const string &FortyGigabitValue, const UI32 &VlanValue, const bool &opCode)
        : ManagementInterfaceMessage (ArpLocalObjectManager::getClassName (),ARPSTATICCONFIGLOCAL),
        m_IpAddress    (IpAddress),
        m_MacAdress    (MacAdress),
        m_InterfaceType    (InterfaceType),
        m_PortChannelValue    (PortChannelValue),
        m_GigabitValue     (GigabitValue),
        m_TenGigabitValue     (TenGigabitValue),
        m_FortyGigabitValue     (FortyGigabitValue),
        m_VlanValue           (VlanValue),
        m_opCode    (opCode)
    {
    }

    ArpStaticConfigLocalMessage::~ArpStaticConfigLocalMessage ()
    {
    }

    void  ArpStaticConfigLocalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  ArpStaticConfigLocalMessage::setIpAddress(const IpV4Address &IpAddress)
    {
        m_IpAddress  =  IpAddress;
    }

    IpV4Address  ArpStaticConfigLocalMessage::getIpAddress() const
    {
        return (m_IpAddress);
    }

    void  ArpStaticConfigLocalMessage::setMacAdress(const string &MacAdress)
    {
        m_MacAdress  =  MacAdress;
    }

    string  ArpStaticConfigLocalMessage::getMacAdress() const
    {
        return (m_MacAdress);
    }

    void  ArpStaticConfigLocalMessage::setInterfaceType(const UI32 &InterfaceType)
    {
        m_InterfaceType  =  InterfaceType;
    }

    UI32  ArpStaticConfigLocalMessage::getInterfaceType() const
    {
        return (m_InterfaceType);
    }

    void  ArpStaticConfigLocalMessage::setPortChannelValue(const UI32 &PortChannelValue)
    {
        m_PortChannelValue  =  PortChannelValue;
    }

    UI32  ArpStaticConfigLocalMessage::getPortChannelValue() const
    {
        return (m_PortChannelValue);
    }

    void  ArpStaticConfigLocalMessage::setGigabitValue(const string &GigabitValue)
    {
        m_GigabitValue  =  GigabitValue;
    }

    string  ArpStaticConfigLocalMessage::getGigabitValue() const
    {
        return (m_GigabitValue);
    }


    void  ArpStaticConfigLocalMessage::setTenGigabitValue(const string &TenGigabitValue)
    {
        m_TenGigabitValue  =  TenGigabitValue;
    }

    string  ArpStaticConfigLocalMessage::getTenGigabitValue() const
    {
        return (m_TenGigabitValue);
    }
  
	void  ArpStaticConfigLocalMessage::setFortyGigabitValue(const string &FortyGigabitValue)
    {
        m_FortyGigabitValue  =  FortyGigabitValue;
    }

    string  ArpStaticConfigLocalMessage::getFortyGigabitValue() const
    {
        return (m_FortyGigabitValue);
    }


	void  ArpStaticConfigLocalMessage::setVlanValue(const UI32 &VlanValue)
    {
        m_VlanValue  =  VlanValue;
    }

    UI32  ArpStaticConfigLocalMessage::getVlanValue() const
    {
        return (m_VlanValue);
    }

	void  ArpStaticConfigLocalMessage::setOpCode(const bool &opCode)
    {
        m_opCode  =  opCode;
    }

    bool  ArpStaticConfigLocalMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
