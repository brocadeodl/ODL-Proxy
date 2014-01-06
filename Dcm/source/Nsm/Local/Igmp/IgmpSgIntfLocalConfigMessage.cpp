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
 *   Author : agidwani                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Igmp/IgmpSgIntfLocalConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

IgmpSgIntfLocalConfigMessage::IgmpSgIntfLocalConfigMessage ()
    : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),IGMPSGINTFLOCALCONFIG)
{
}

IgmpSgIntfLocalConfigMessage::IgmpSgIntfLocalConfigMessage (const IpV4Address &groupAddress,const string &ifName,const UI64 &ifIndex, const UI32 &ifType, const bool &negation)
    : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),IGMPSGINTFLOCALCONFIG),
    m_groupAddress    (groupAddress),
    m_ifName    (ifName),
    m_ifIndex   (ifIndex),
    m_ifType    (ifType),
    m_negation  (negation)
{
}

IgmpSgIntfLocalConfigMessage::~IgmpSgIntfLocalConfigMessage ()
{
}

void  IgmpSgIntfLocalConfigMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
    addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
    addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
}

void  IgmpSgIntfLocalConfigMessage::setGroupAddress(const IpV4Address &groupAddress)
{
    m_groupAddress  =  groupAddress;
}

IpV4Address  IgmpSgIntfLocalConfigMessage::getGroupAddress() const
{
    return (m_groupAddress);
}

void  IgmpSgIntfLocalConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  IgmpSgIntfLocalConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  IgmpSgIntfLocalConfigMessage::setIfIndex(const UI64 &ifIndex)
{
    m_ifIndex  =  ifIndex;
}

UI64  IgmpSgIntfLocalConfigMessage::getIfIndex() const
{
    return (m_ifIndex);
}

void  IgmpSgIntfLocalConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  IgmpSgIntfLocalConfigMessage::getIfType() const
{
    return (m_ifType);
}

void  IgmpSgIntfLocalConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  IgmpSgIntfLocalConfigMessage::getNegation() const
{
    return (m_negation);
}

}