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
 *   Author : hzhu                                                     *
 **************************************************************************/

#ifndef RTMUPDATESTATICROUTEMESSAGE_H
#define RTMUPDATESTATICROUTEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class RtmUpdateStaticRouteMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RtmUpdateStaticRouteMessage ();
            virtual    ~RtmUpdateStaticRouteMessage ();
            void setDest(const IpV4AddressNetworkMask &dest);
            IpV4AddressNetworkMask getDest()  const;
            void setNhIp(const IpV4Address &nhIp);
            IpV4Address getNhIp()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCost(const UI32 &cost);
            UI32UC getCost()  const;
            void setDistance(const UI32 &distance);
            UI32UC getDistance()  const;
            void setTag(const UI32 &tag);
            UI32UC getTag()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
            string getNextHopVrf() const;
            void setNextHopVrf(string &vrf);
            

// Now the data members

        private:
        protected:
        public:
            IpV4AddressNetworkMask  m_dest;
            IpV4Address  m_nhIp;
            UI32  m_ifType;
            string  m_ifName;
            UI32UC  m_cost;
            UI32UC  m_distance;
            UI32UC  m_tag;
            UI32  m_opCode;
            string m_vrf;
    };
}
#endif                                            //RTMUPDATESTATICROUTEMESSAGE_H

