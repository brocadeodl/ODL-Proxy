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
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#ifndef MSG_ELD_INTF_CONFMESSAGE_H
#define MSG_ELD_INTF_CONFMESSAGE_H

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h" 

using namespace WaveNs;

namespace DcmNs
{

    class EldClientUpdateRxlimConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            EldClientUpdateRxlimConfigMessage ();
            EldClientUpdateRxlimConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation);
            virtual    ~EldClientUpdateRxlimConfigMessage ();
            virtual const void *getCStructureForInputs();
            virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
			void setRxlim(const UI32 &rxlim);
			UI32 getRxlim()  const;
			void setHelloInt(const UI32 &helloint);
			UI32 getHelloInt()  const;
			void setShutdnTime(const UI32 &shutdntime);
			UI32 getShutdnTime()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_opCode;
			UI32  m_rxlim;
			UI32  m_hello_int;
			UI32  m_shutdn_time;
    };
}
#endif                                            //MSG_ELD_INTF_CONFMESSAGE_H
