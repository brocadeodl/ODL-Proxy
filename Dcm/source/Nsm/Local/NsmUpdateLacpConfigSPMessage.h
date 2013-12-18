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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef NSMUPDATELACPCONFIGSPMESSAGE_H
#define NSMUPDATELACPCONFIGSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmUpdateLacpConfigSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdateLacpConfigSPMessage ();
            NsmUpdateLacpConfigSPMessage (const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable,
														const UI8 &cmdCode);
            NsmUpdateLacpConfigSPMessage (const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable, const UI32 dot1xtesttimeout,														const UI8 &cmdCode);
            virtual    ~NsmUpdateLacpConfigSPMessage ();
            void setSystemPriority(const UI32 &systemPriority);
            UI32 getSystemPriority()  const;
			void setDot1xenable(const bool &dot1xenable);
			bool getDot1xenable()  const;
            void setNativevlanDisable(const bool &nativevlanDisable);
            bool getNativevlanDisable()  const;
			void setDot1xtesttimeout(const UI32 &dot1xtesttimeout);
			UI32 getDot1xtesttimeout()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;

        private:
        protected:
        public:
            UI32  m_systemPriority;
			bool  m_dot1xenable;
            bool  m_nativevlanDisable;
            UI32  m_dot1xtesttimeout;
            UI8  m_cmdCode;
    };
}
#endif  //NSMUPDATELACPCONFIGSPMESSAGE_H
