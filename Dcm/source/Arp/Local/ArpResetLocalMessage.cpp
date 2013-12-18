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
#include "Arp/Local/ArpResetLocalMessage.h"
#include "Arp/Local/ArpTypes.h"

namespace DcmNs
{

    ArpResetLocalMessage::ArpResetLocalMessage ()
        : ManagementInterfaceMessage (ArpLocalObjectManager::getClassName (),ARPRESETLOCAL)
    {
    }

    ArpResetLocalMessage::ArpResetLocalMessage (const IpV4Address &IpAddress,const string &MacAdress,const UI32 &InterfaceType,const string &InterfaceValue,const UI32 &SlotNumber,const UI32 &CommandCode)
        : ManagementInterfaceMessage (ArpLocalObjectManager::getClassName (),ARPRESETLOCAL),
        m_IpAddress    (IpAddress),
        m_MacAdress    (MacAdress),
        m_InterfaceType    (InterfaceType),
        m_InterfaceValue    (InterfaceValue),
        m_SlotNumber    (SlotNumber),
        m_CommandCode    (CommandCode)
    {
    }

    ArpResetLocalMessage::~ArpResetLocalMessage ()
    {
    }

    void  ArpResetLocalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4Address(&m_IpAddress,"IpAddress"));
        addSerializableAttribute (new AttributeString(&m_MacAdress,"MacAdress"));
        addSerializableAttribute (new AttributeUI32(&m_InterfaceType,"InterfaceType"));
        addSerializableAttribute (new AttributeString(&m_InterfaceValue,"InterfaceValue"));
        addSerializableAttribute (new AttributeUI32(&m_SlotNumber,"SlotNumber"));
        addSerializableAttribute (new AttributeUI32(&m_CommandCode,"CommandCode"));
    }

    void  ArpResetLocalMessage::setIpAddress(const IpV4Address &IpAddress)
    {
        m_IpAddress  =  IpAddress;
    }

    IpV4Address  ArpResetLocalMessage::getIpAddress() const
    {
        return (m_IpAddress);
    }

    void  ArpResetLocalMessage::setMacAdress(const string &MacAdress)
    {
        m_MacAdress  =  MacAdress;
    }

    string  ArpResetLocalMessage::getMacAdress() const
    {
        return (m_MacAdress);
    }

    void  ArpResetLocalMessage::setInterfaceType(const UI32 &InterfaceType)
    {
        m_InterfaceType  =  InterfaceType;
    }

    UI32  ArpResetLocalMessage::getInterfaceType() const
    {
        return (m_InterfaceType);
    }

    void  ArpResetLocalMessage::setInterfaceValue(const string &InterfaceValue)
    {
        m_InterfaceValue  =  InterfaceValue;
    }

    string  ArpResetLocalMessage::getInterfaceValue() const
    {
        return (m_InterfaceValue);
    }

    void  ArpResetLocalMessage::setSlotNumber(const UI32 &SlotNumber)
    {
        m_SlotNumber  =  SlotNumber;
    }

    UI32  ArpResetLocalMessage::getSlotNumber() const
    {
        return (m_SlotNumber);
    }

    void  ArpResetLocalMessage::setCommandCode(const UI32 &CommandCode)
    {
        m_CommandCode  =  CommandCode;
    }

    UI32  ArpResetLocalMessage::getCommandCode() const
    {
        return (m_CommandCode);
    }

}
