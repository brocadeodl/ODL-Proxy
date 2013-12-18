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


/************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.       *
 *   All rights reserved.                                               *
 *   Author : pprakash							*
 ************************************************************************/
#include "string"
#include "Ipadm/Local/IpadmLocalObjectManager.h"
#include "Ipadm/Local/IpadmIpv6RouteconfigMessage.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Types/IpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
namespace DcmNs
{

    IpadmIpv6RouteconfigMessage::IpadmIpv6RouteconfigMessage ()
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6ROUTECONFIG)
    {
    }

    IpadmIpv6RouteconfigMessage::IpadmIpv6RouteconfigMessage (const IpV6AddressNetworkMask &dest,const IpV6Address &nexthop,const string &type)
        : PrismMessage (IpadmLocalObjectManager::getPrismServiceId (), IPADMIPV6ROUTECONFIG),
        m_dest    (dest),
        m_nexthop    (nexthop),
	m_type (type)
    {
    }

    IpadmIpv6RouteconfigMessage::~IpadmIpv6RouteconfigMessage ()
    {
    }

    void  IpadmIpv6RouteconfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV6AddressNetworkMask(&m_dest,"dest"));
        addSerializableAttribute (new AttributeIpV6Address(&m_nexthop,"nexthop"));
        addSerializableAttribute (new AttributeString(&m_type,"type"));
    }

    void  IpadmIpv6RouteconfigMessage::setDest(const IpV6AddressNetworkMask &dest)
	{		    
        m_dest  =  dest;
    }

    IpV6AddressNetworkMask  IpadmIpv6RouteconfigMessage::getDest() const
    {
        return (m_dest);
    }

    void  IpadmIpv6RouteconfigMessage::setNhIp(const IpV6Address &nexthop)
    {
        m_nexthop  =  nexthop;
    }

    IpV6Address  IpadmIpv6RouteconfigMessage::getNhIp() const
    {
        return (m_nexthop);
    }

    void  IpadmIpv6RouteconfigMessage::setType(const string &type)
	{		    
        m_type  =  type;
    }

   string IpadmIpv6RouteconfigMessage::getType() const
    {
        return (m_type);
    }
    void  IpadmIpv6RouteconfigMessage::setRouteType(const UI32 &type)
	{		    
        m_routeType  =  type;
    }

   UI32 IpadmIpv6RouteconfigMessage::getRouteType() const
    {
        return (m_routeType);
	}
    
}
