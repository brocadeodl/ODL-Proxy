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
#include "Ipadm/Local/IpadmIpv6PrefixConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIpv6PrefixConfigMessage::IpadmIpv6PrefixConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6PREFIXCONFIG)
    {
    }

    IpadmIpv6PrefixConfigMessage::IpadmIpv6PrefixConfigMessage (const string &switchid,const IpV6AddressNetworkMask &ipv6_prefix)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6PREFIXCONFIG),
        m_switchid    (switchid),
        m_ipv6_prefix    (ipv6_prefix)
    {
    }

    IpadmIpv6PrefixConfigMessage::~IpadmIpv6PrefixConfigMessage ()
    {
    }

    void  IpadmIpv6PrefixConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeIpV6AddressNetworkMask(&m_ipv6_prefix,"ipv6_prefix"));
    }

    void  IpadmIpv6PrefixConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIpv6PrefixConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIpv6PrefixConfigMessage::setIpv6Prefix(const IpV6AddressNetworkMask &ipv6_prefix)
    {
        m_ipv6_prefix  =  ipv6_prefix;
    }

    IpV6AddressNetworkMask  IpadmIpv6PrefixConfigMessage::getIpv6Prefix() const
    {
        return (m_ipv6_prefix);
    }

}
