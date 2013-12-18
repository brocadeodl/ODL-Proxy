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
 *   Author : hungn                                                     *
 **************************************************************************/

#ifndef LINECARDPOWERSETMESSAGE_H
#define LINECARDPOWERSETMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LinecardPowerSetMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LinecardPowerSetMessage ();
            LinecardPowerSetMessage (const UI32 &slotnum,const UI32 &mode,const SI32 &result);
            virtual    ~LinecardPowerSetMessage ();
            void setSlotnum(const UI32 &slotnum);
            UI32 getSlotnum()  const;
            void setMode(const UI32 &mode);
            UI32 getMode()  const;
            void setResult(const SI32 &result);
            SI32 getResult()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_slotnum;
            UI32  m_mode;
            SI32  m_result;
    };
}
#endif                                            //LINECARDPOWERSETMESSAGE_H
