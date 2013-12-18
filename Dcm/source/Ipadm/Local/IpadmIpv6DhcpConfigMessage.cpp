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
 *   Author :  rjohn                                                       *
 **************************************************************************/

#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmIpv6DhcpConfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"

namespace DcmNs
{

    IpadmIpv6DhcpConfigMessage::IpadmIpv6DhcpConfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6DHCPCONFIG)
    {
    }

    IpadmIpv6DhcpConfigMessage::IpadmIpv6DhcpConfigMessage (const string &switchid,const bool &dhcpv6)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6DHCPCONFIG),
        m_switchid    (switchid),
        m_dhcpv6    (dhcpv6)
    {
    }

    IpadmIpv6DhcpConfigMessage::~IpadmIpv6DhcpConfigMessage ()
    {
    }

    void  IpadmIpv6DhcpConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchid,"switchid"));
        addSerializableAttribute (new AttributeBool(&m_dhcpv6,"dhcpv6"));
    }

    void  IpadmIpv6DhcpConfigMessage::setSwitchid(const string &switchid)
    {
        m_switchid  =  switchid;
    }

    string  IpadmIpv6DhcpConfigMessage::getSwitchid() const
    {
        return (m_switchid);
    }

    void  IpadmIpv6DhcpConfigMessage::setIpv6Dhcp(const bool &dhcpv6)
    {
        m_dhcpv6  =  dhcpv6;
    }

    bool IpadmIpv6DhcpConfigMessage::getIpv6Dhcp() const
    {
        return (m_dhcpv6);
    }

}

