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

#ifndef NHVRFLOCALMANAGEDOBJECT_H
#define NHVRFLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class RtmLocalObjectManager;
    class NhVrfLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            NhVrfLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager);
            virtual    ~NhVrfLocalManagedObject ();
            static string getClassName();
            void setDest(const IpV4AddressNetworkMask &dest);
            IpV4AddressNetworkMask getDest()  const;
            void setNhIp(const IpV4Address &nhIp);
            IpV4Address getNhIp()  const;
            void setInterfaceType(const UI32 &interfaceType);
            UI32 getInterfaceType()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCost(const UI32 &cost, bool userConfig = false);
            UI32UC getCost()  const;
            void setDistance(const UI32 &distance, bool userConfig = false);
            UI32UC getDistance()  const;
            void setTag(const UI32 &tag, bool userConfig = false);
            UI32UC getTag()  const;
            ObjectId getNextHopVrf() const;
            void setNextHopVrf(ObjectId &vrf);
            string getVrf() const { return m_currVrf;}
            void setVrf(const string &vrf) { m_currVrf = vrf;}

// Now the data members

        private:
        protected:
        public:
            IpV4AddressNetworkMask  m_dest;
            IpV4Address  m_nhIp;
            UI32UC  m_cost;
            UI32UC  m_distance;
            UI32UC  m_tag;
            ObjectId m_nextHopVrf;
            string m_currVrf;
    };
}
#endif                                            //NHSTATICROUTELOCALMANAGEDOBJECT_H
