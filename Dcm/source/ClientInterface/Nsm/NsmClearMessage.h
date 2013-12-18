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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Navin
 **************************************************************************/

#ifndef NSM_CLEAR_MESSAGE_H
#define NSM_CLEAR_MESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClearMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClearMessage ();
            NsmClearMessage (const string &ifName, const UI32 &slot, const UI32 &poId, const UI32 &vlan, const UI32 &cmdCode);
            virtual ~NsmClearMessage ();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setSlotId(const UI32 &slot);
            UI32 getSlotId() const;
            void setPortChannelId(const UI32 &poId);
            UI32 getPortChannelId() const;
            void setVlanId(const UI32 &vlan);
            UI32 getVlanId() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_ifName;
            UI32 m_slot;
            UI32 m_vlan;
            UI32 m_po_id;
            UI32 m_cmdCode;
    };
}
#endif  //NSM_CLEAR_MESSAGE_H
