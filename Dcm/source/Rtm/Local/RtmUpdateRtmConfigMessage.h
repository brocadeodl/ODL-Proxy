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

#ifndef RTMUPDATERTMCONFIGMESSAGE_H
#define RTMUPDATERTMCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class RtmUpdateRtmConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RtmUpdateRtmConfigMessage ();
            virtual    ~RtmUpdateRtmConfigMessage ();
            void setNhProto(const UI32 &nhProto);
            UI32 getNhProto()  const;
            void setNhDefault(const bool &nhDefault);
            bool getNhDefault()  const;
            void setNhRecur(const UI32 &nhRecurr);
            UI32 getNhRecur()  const;
            void setLoadSharing(const UI32 &loadSharing);
            UI32 getLoadSharing()  const;
            void setRouterId(const IpV4Address &routerID);
            IpV4Address getRouterId()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_nhProto;
            bool  m_nhDefault;
            UI32  m_nhRecur;
            UI32  m_loadSharing;
            IpV4Address  m_routerID;
            UI32  m_opCode;
    };
}
#endif                                            //RTMUPDATERTMCONFIGMESSAGE_H
