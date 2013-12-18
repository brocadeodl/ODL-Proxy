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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Dhcp/DhcpRelIntfLocalConfigMessage.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

DhcpRelIntfLocalConfigMessage::DhcpRelIntfLocalConfigMessage ()
    : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), DHCPRELINTFLOCALCONFIG),
	m_serverAddress(""),
	m_ifName(""),
	m_vlanValue(0),
	m_ifType(IF_TYPE_INVALID),
	m_negation(false)
{
}

DhcpRelIntfLocalConfigMessage::DhcpRelIntfLocalConfigMessage (const IpV4Address &serverAddress, const string &ifName, const UI32 &vlanValue, const UI32 &ifType, const bool &negation)
    : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), DHCPRELINTFLOCALCONFIG),
    m_serverAddress    (serverAddress),
    m_ifName    	   (ifName),
    m_vlanValue   	   (vlanValue),
    m_ifType     	   (ifType),
    m_negation  	   (negation)
{
}

DhcpRelIntfLocalConfigMessage::~DhcpRelIntfLocalConfigMessage ()
{
}

void  DhcpRelIntfLocalConfigMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeIpV4Address(&m_serverAddress,"serverAddress"));
    addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    addSerializableAttribute (new AttributeUI32(&m_vlanValue,"vlanValue"));
    addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
}

void  DhcpRelIntfLocalConfigMessage::setServerAddress(const IpV4Address &serverAddress)
{
    m_serverAddress  =  serverAddress;
}

IpV4Address  DhcpRelIntfLocalConfigMessage::getServerAddress() const
{
    return (m_serverAddress);
}

void  DhcpRelIntfLocalConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  DhcpRelIntfLocalConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  DhcpRelIntfLocalConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  DhcpRelIntfLocalConfigMessage::getIfType() const
{
    return (m_ifType);
}

void  DhcpRelIntfLocalConfigMessage::setVlanValue(const UI32 &vlanValue)
{
    m_vlanValue  =  vlanValue;
}

UI32  DhcpRelIntfLocalConfigMessage::getVlanValue() const
{
    return (m_vlanValue);
}

void  DhcpRelIntfLocalConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  DhcpRelIntfLocalConfigMessage::getNegation() const
{
    return (m_negation);
}

}
