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

#ifndef IPADMIPV6ROUTECONFIGMESSAGE_H
#define IPADMIPV6ROUTECONFIGMESSAGE_H
#include "string"
#include "Framework/Types/IpV6Address.h"
#include "Framework/Types/IpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmIpv6RouteconfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
	    IpadmIpv6RouteconfigMessage ();
	    IpadmIpv6RouteconfigMessage (const IpV6AddressNetworkMask &dest,const IpV6Address &nexthop,const string &type);
	    IpadmIpv6RouteconfigMessage (const IpV6AddressNetworkMask &dest,const IpV6Address &nexthop);
            virtual    ~IpadmIpv6RouteconfigMessage ();
	    void setDest(const IpV6AddressNetworkMask &dest);
            IpV6AddressNetworkMask getDest()  const;
            void setNhIp(const IpV6Address &nexthop);
            IpV6Address getNhIp()  const;
	    string getType() const;
	    void setType(const string &type);	    
	    UI32 getRouteType() const;
	    void setRouteType(const UI32 &type);	    
// Now the data members

        private:
        protected:
        IpV6AddressNetworkMask m_dest;
 	IpV6Address  m_nexthop;
    UI32 m_routeType;
	string m_type;
    };
}
#endif                                            //IPADMIPV6ROUTECONFIGMESSAGE_H
