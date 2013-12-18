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
 *   Author : Abhay Gidwani                                                     *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"

#include "ClientInterface/Igmp/IgmpMessageDef.h"
#include "ClientInterface/Igmp/IgmpClientSgIntfLocalConfigMessage.h"

namespace DcmNs
{

IgmpClientSgIntfLocalConfigMessage::IgmpClientSgIntfLocalConfigMessage ()
    : DcmManagementInterfaceMessage (IGMPCLIENTSGINTFLOCALCONFIG),
    m_ifName            (""),
    m_ifType            (0),
    m_negation          (false)
{ 

}

IgmpClientSgIntfLocalConfigMessage::IgmpClientSgIntfLocalConfigMessage (const IpV4Address &groupAddress, const string &ifName, const UI32 &ifType, const bool &negation)
    : DcmManagementInterfaceMessage (IGMPCLIENTSGINTFLOCALCONFIG),
    m_groupAddress    (groupAddress),
    m_ifName    (ifName),
    m_ifType    (ifType),
    m_negation  (negation)
{
    WaveNs::trace(TRACE_LEVEL_INFO, __FUNCTION__);
}

IgmpClientSgIntfLocalConfigMessage::~IgmpClientSgIntfLocalConfigMessage ()
{
}

void  IgmpClientSgIntfLocalConfigMessage::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
    addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
}

void  IgmpClientSgIntfLocalConfigMessage::setGroupAddress(const IpV4Address &groupAddress)
{
    m_groupAddress  =  groupAddress;
}

IpV4Address  IgmpClientSgIntfLocalConfigMessage::getGroupAddress() const
{
    return (m_groupAddress);
}

void  IgmpClientSgIntfLocalConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  IgmpClientSgIntfLocalConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  IgmpClientSgIntfLocalConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  IgmpClientSgIntfLocalConfigMessage::getIfType() const
{
    return (m_ifType);
}
void  IgmpClientSgIntfLocalConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  IgmpClientSgIntfLocalConfigMessage::getNegation() const
{
    return (m_negation);
}

const void *IgmpClientSgIntfLocalConfigMessage::getCStructureForInputs ()
{
    string str_ipaddr;
    igmp_sg_intf_cfg_msg_t *pInput = 
        (igmp_sg_intf_cfg_msg_t *)malloc (sizeof (struct igmp_sg_intf_cfg_msg_));

    strncpy (pInput->group, (m_groupAddress.toString()).c_str(), IGMP_MAX_STR);
    strncpy (pInput->if_name, m_ifName.c_str(), IGMP_MAX_STR);
    pInput->if_type = m_ifType;
    pInput->negation = m_negation;

    return (pInput);
}

void IgmpClientSgIntfLocalConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
}
}
