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
#include "Arp/Local/ArpClearLocalMessage.h"
#include "Arp/Local/ArpTypes.h"

namespace DcmNs
{

    ArpClearLocalMessage::ArpClearLocalMessage ()
        : PrismMessage(ArpLocalObjectManager::getPrismServiceId (), ARPCLEARLOCAL)
    {
    }

    ArpClearLocalMessage::ArpClearLocalMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr, const string &mac_addr)
    :PrismMessage(ArpLocalObjectManager::getPrismServiceId (), ARPCLEARLOCAL),
		m_ifName    (ifName),
		m_cmdCode   (cmdCode),
		m_ip_addr	(ip_addr),
		m_mac_addr  (mac_addr)
    {
    }


    ArpClearLocalMessage::~ArpClearLocalMessage ()
    {
    }

    void  ArpClearLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		    addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    		addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    		addSerializableAttribute (new AttributeIpV4Address(&m_ip_addr, "ip_addr"));
		    addSerializableAttribute (new AttributeString(&m_mac_addr,"mac_addr"));
    }

    void  ArpClearLocalMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  ArpClearLocalMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  ArpClearLocalMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  ArpClearLocalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void  ArpClearLocalMessage::setIp_addr(const IpV4Address &ip_addr)
    {
        m_ip_addr  =  ip_addr;
    }

    IpV4Address  ArpClearLocalMessage::getIp_addr() const
    {
        return (m_ip_addr);
    }

    void  ArpClearLocalMessage::setMac_addr(const string &mac_addr)
    {
        m_mac_addr  =  mac_addr;
    }

    string  ArpClearLocalMessage::getMac_addr() const
    {
        return (m_mac_addr);
    }


}
