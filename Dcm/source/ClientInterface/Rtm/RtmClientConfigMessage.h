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

#ifndef RTMCLIENTCONFIGMESSAGE_H
#define RTMCLIENTCONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "Rtm/Local/RtmUpdateRtmConfigMessage.h"
#include "Rtm/Local/RtmConfigLocalManagedObject.h"
#include "Rtm/Local/RtmNhProtoLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
    class VrfLocalAddVrfMessage;

    class RtmClientConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RtmClientConfigMessage ();
            RtmClientConfigMessage (RtmUpdateRtmConfigMessage& msg);
            RtmClientConfigMessage (VrfLocalAddVrfMessage *msg);
            virtual    ~RtmClientConfigMessage ();

            void setRtmConfig (RtmConfigLocalManagedObject* pMo, string vrfName = "");
            void setRtmConfig (RtmNhProtoLocalManagedObject* pMo, string vrfName = "");

            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);
            const void * getCStructureForInputs();
// Now the data members

        private:
        protected:
        public:

            UI32  m_nhProto;
            bool  m_nhDefault;
            UI32  m_nhRecur;
            UI32  m_loadSharing;
            IpV4Address  m_routerID;
            UI32  m_cfgBitMask;
            UI32  m_opCode;
            string m_rd;
            UI32 m_rtType;
            string m_rt;
            bool m_vrfafIpv4Flag;
            UI32 m_maxRoutes;
            string m_vrf;/*current vrf cli vrf mode*/
    };
}
#endif                                            //RTMUPDATERTMCONFIGMESSAGE_H
