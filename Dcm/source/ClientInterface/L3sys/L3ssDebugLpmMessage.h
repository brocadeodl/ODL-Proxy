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
 *   Author :                                                 *
 **************************************************************************/

#ifndef L3ssDebugLpmMessage_H
#define L3ssDebugLpmMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
using namespace WaveNs;

namespace DcmNs
{

    class L3ssDebugLpmMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L3ssDebugLpmMessage ();
            virtual    ~L3ssDebugLpmMessage ();
			void  setCmdCode(const UI32 &cmdCode);
			UI32  getCmdCode() const;
			void  setSlot(const UI32 &slotno);
			UI32  getSlot() const;
			void  setChip(const UI32 &chipno);
			UI32  getChip() const;
			void  setLastPrinted(const UI32 &last_printed);
			UI32  getLastPrinted() const;
			const void *getCStructureForInputs ();
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
			void loadOutputsFromCStructure (const void *pOutputCStructure);
            void copyAllBuffers(const WaveNs::PrismMessage &);

// Now the data members

        private:
        protected:
        public:
            UI32    m_cmdCode;
            UI32    m_slot;
            UI32    m_chip;
            UI32    m_last_printed;
    };
}
#endif                                            //L3ssDebugLpmMessage_H



