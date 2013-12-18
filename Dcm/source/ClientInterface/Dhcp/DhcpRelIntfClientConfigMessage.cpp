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

#include "Framework/Utils/TraceUtils.h"

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "ClientInterface/Dhcp/DhcpRelIntfClientConfigMessage.h"

namespace DcmNs
{

DhcpRelIntfClientConfigMessage::DhcpRelIntfClientConfigMessage ()
    : DcmManagementInterfaceMessage (IPHELPCONFIG),
    m_serverAddress     ("0.0.0.0"),
    m_ifName            (""),
    m_ifType            (0),
    m_vlanValue         (0),
    m_negation          (false)
{ 
}

DhcpRelIntfClientConfigMessage::DhcpRelIntfClientConfigMessage (const IpV4Address &serverAddress, const string &ifName, const UI32 &ifType, const UI32 &vlanValue, const bool &negation)
    : DcmManagementInterfaceMessage (IPHELPCONFIG),
    m_serverAddress    (serverAddress),
    m_ifName    (ifName),
    m_ifType    (ifType),
    m_vlanValue    (vlanValue),
    m_negation  (negation)
{
    WaveNs::trace(TRACE_LEVEL_INFO, __FUNCTION__);
}

DhcpRelIntfClientConfigMessage::~DhcpRelIntfClientConfigMessage ()
{
}

void  DhcpRelIntfClientConfigMessage::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeIpV4Address(&m_serverAddress,"serverAddress"));
    addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    addSerializableAttribute (new AttributeUI32(&m_vlanValue,"vlanValue"));
    addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
}

void  DhcpRelIntfClientConfigMessage::setServerAddress(const IpV4Address &serverAddress)
{
    m_serverAddress  =  serverAddress;
}

IpV4Address  DhcpRelIntfClientConfigMessage::getServerAddress() const
{
    return (m_serverAddress);
}

void  DhcpRelIntfClientConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  DhcpRelIntfClientConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  DhcpRelIntfClientConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  DhcpRelIntfClientConfigMessage::getIfType() const
{
    return (m_ifType);
}

void  DhcpRelIntfClientConfigMessage::setVlanValue(const UI32 &vlanValue)
{
    m_vlanValue  =  vlanValue;
}

UI32  DhcpRelIntfClientConfigMessage::getVlanValue() const
{
    return (m_vlanValue);
}

void  DhcpRelIntfClientConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  DhcpRelIntfClientConfigMessage::getNegation() const
{
    return (m_negation);
}

const void *DhcpRelIntfClientConfigMessage::getCStructureForInputs ()
{
    string str_ipaddr;
    iphelp_dhcprelay_config_in_msg_t *pInput = 
        (iphelp_dhcprelay_config_in_msg_t *)malloc (sizeof(iphelp_dhcprelay_config_in_msg_t));

    pInput->cmd = IPHELP_CONFIG_DRADDR;
    strncpy (pInput->ip_address, (m_serverAddress.toString()).c_str(), DHCP_MAX_IPV4ADDR_STR);
    strncpy (pInput->if_name, m_ifName.c_str(), DHCP_MAX_IFNAME_STR);
    pInput->interface_type = m_ifType;
    pInput->interface_id = m_vlanValue;
    pInput->negation = m_negation;

    return (pInput);
}

void DhcpRelIntfClientConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
}
}
