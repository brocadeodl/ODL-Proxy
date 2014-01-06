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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef NSMGLOBALQOSPFCCONFIGMESSAGE_H
#define NSMGLOBALQOSPFCCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalQosPfcConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalQosPfcConfigMessage ();
            NsmGlobalQosPfcConfigMessage (const SI32 &pfcFlowControlCos,const SI32 &pfcFlowControlTx,const SI32 &pfcFlowControlRx);
            virtual    ~NsmGlobalQosPfcConfigMessage ();
            void setPfcFlowControlCos(const SI32 &pfcFlowControlCos);
            SI32 getPfcFlowControlCos()  const;
            void setPfcFlowControlTx(const UI32 &pfcFlowControlTx);
            UI32 getPfcFlowControlTx()  const;
            void setPfcFlowControlRx(const UI32 &pfcFlowControlRx);
            UI32 getPfcFlowControlRx()  const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;
            void setOpCode(const SI32 &OpCode);
            SI32 getOpCode()  const;
			void setPoId(const UI32 &poId);
            UI32 getPoId()  const;


// Now the data members

        private:
        protected:
        public:
			UI32  m_poId;
            SI32  m_pfcFlowControlCos;
            UI32  m_pfcFlowControlTx;
            UI32  m_pfcFlowControlRx;
            SI32  m_cmdCode;
            SI32  m_OpCode;
    };
}
#endif                                            //NSMLOCALQOSPFCCONFIGMESSAGE_H