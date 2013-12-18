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

#ifndef IPADMIPV6PREFIXCONFIGMESSAGE_H
#define IPADMIPV6PREFIXCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmIpv6PrefixConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpadmIpv6PrefixConfigMessage ();
            IpadmIpv6PrefixConfigMessage (const string &switchid,const IpV6AddressNetworkMask &ipv6_prefix);
            virtual    ~IpadmIpv6PrefixConfigMessage ();
            void setSwitchid(const string &switchid);
            string getSwitchid()  const;
            void setIpv6Prefix(const IpV6AddressNetworkMask &ipv6_prefix);
            IpV6AddressNetworkMask getIpv6Prefix()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_switchid;
            IpV6AddressNetworkMask  m_ipv6_prefix;
    };
}
#endif                                            //IPADMIPV6PREFIXCONFIGMESSAGE_H
