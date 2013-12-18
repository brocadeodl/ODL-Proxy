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
 **************************************************************************/

#ifndef BPRATELIMITLOCAlCONFIGMESSAGE_H
#define BPRATELIMITLOCALCONFIGMESSAGE_H


#include "Framework/Messaging/Local/PrismMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class BpratelimitLocalConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            BpratelimitLocalConfigMessage ();
            BpratelimitLocalConfigMessage (const UI32 &slotId,const UI32 &opCode);
            virtual    ~BpratelimitLocalConfigMessage();
            void setSlotId(const UI32 &sampleRate);
            UI32 getSlotId()  const;
            void setMode(const UI32 &sampleRate);
            UI32 getMode()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_slotId;
            UI32  m_opCode;
            UI32  m_mode ; 
    };
}
#endif                                            //BpratelimitLocalCREATEBpratelimitLocalCONFIGMESSAGE_H
