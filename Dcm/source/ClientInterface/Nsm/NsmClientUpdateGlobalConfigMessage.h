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

/******************************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	      *
 *   All rights reserved.                                                     *
 *   Author : Mobin Mohan                                                     *
 *****************************************************************************/

#ifndef NSMCLIENTGLOBALUPDATECONFIGMESSAGE_H
#define NSMCLIENTGLOBALUPDATECONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmClientUpdateGlobalConfigMessage : public 
                                             DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmClientUpdateGlobalConfigMessage ();
            NsmClientUpdateGlobalConfigMessage (bool nativevlanDisable, 
														const UI8 &cmdCode, const UI32 &slotId);
            virtual    ~NsmClientUpdateGlobalConfigMessage ();
            void setSystemPriority(const UI32 &systemPriority);
            UI32 getSystemPriority()  const;
            void setDot1xenable(const bool &enable);
            bool getDot1xenable()  const;
       		void setSlotId(const UI32 &slotId);
			UI32 getSlotId()  const;
            void setNativevlanDisable(const bool &nativevlanDisable);
            bool getNativevlanDisable()  const;
            void setDot1xtesttimeout(const UI32 &dot1xtesttimeout);
            UI32 getDot1xtesttimeout()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            
			virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (
                                             const void *pOutputCStructure);

        private:
        protected:
        public:
            UI32  m_systemPriority;
            bool  m_dot1xenable;
            bool  m_nativevlanDisable;
            UI32  m_dot1xtesttimeout;
            UI8  m_cmdCode;
			UI32 m_slotId;
    };
}
#endif //NSMCLIENTGLOBALUPDATECONFIGMESSAGE_H
