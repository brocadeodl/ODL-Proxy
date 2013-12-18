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

#ifndef RTMCLIENTSTATICROUTEMESSAGE_H
#define RTMCLIENTSTATICROUTEMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Rtm/Local/RtmUpdateStaticRouteMessage.h"
#include "Rtm/Local/IfStaticRouteLocalManagedObject.h"
#include "Rtm/Local/NhStaticRouteLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class RtmClientStaticRouteMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RtmClientStaticRouteMessage ();
            RtmClientStaticRouteMessage (RtmUpdateStaticRouteMessage& msg);
            virtual    ~RtmClientStaticRouteMessage ();

			void setStaticRoute (IfStaticRouteLocalManagedObject *pMo);
			void setStaticRoute (NhStaticRouteLocalManagedObject *pMo);

            virtual const void *getCStructureForInputs    ();
			virtual void loadOutputsFromCStructure (const void *pOutputCStructure);


// Now the data members

        private:
        protected:
        public:
            IpV4AddressNetworkMask  m_dest;
            IpV4Address  m_nhIp;
            UI32  m_ifType;
            string  m_ifName;
            UI32  m_cost;
            UI32  m_distance;
            UI32  m_tag;
            UI32  m_opCode;
            string m_nextHopVrf;
    };
}
#endif                                            //RTMCLIETNSTATICROUTEMESSAGE_H

