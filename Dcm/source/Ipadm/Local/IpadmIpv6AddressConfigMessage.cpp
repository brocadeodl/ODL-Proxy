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
 *   Author : cshah                                                     *
 **************************************************************************/

#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmIpv6AddressConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIpv6AddressConfigMessage::IpadmIpv6AddressConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6ADDRESSCONFIG)
    {
    }

    IpadmIpv6AddressConfigMessage::IpadmIpv6AddressConfigMessage (const string &switchid,const string &ipv6_address)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6ADDRESSCONFIG),
        m_switchid    (switchid),
        m_ipv6_address    (ipv6_address)
    {
    }

    IpadmIpv6AddressConfigMessage::~IpadmIpv6AddressConfigMessage ()
    {
    }

    void  IpadmIpv6AddressConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeString(&m_ipv6_address,"ipv6_address"));
    }

    void  IpadmIpv6AddressConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIpv6AddressConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIpv6AddressConfigMessage::setIpv6Address(const string &ipv6_address)
    {
        m_ipv6_address  =  ipv6_address;
    }

    string  IpadmIpv6AddressConfigMessage::getIpv6Address() const
    {
        return (m_ipv6_address);
    }

}
