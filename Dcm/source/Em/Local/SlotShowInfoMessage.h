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

#ifndef SLOTSHOWINFOMESSAGE_H
#define SLOTSHOWINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;



namespace DcmNs
{
	#define MAX_SLOT 16
	#define SLOT_SHOW_LINECARD 1
	#define SLOT_SHOW_SFM 2
	#define SLOT_SHOW_MM 3
	#define SLOT_SHOW_ALL 4

    class SlotShowInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SlotShowInfoMessage ();
            SlotShowInfoMessage (const UI32 &slotnum,const UI32 &cmdcode, const SI32 &result);
            virtual    ~SlotShowInfoMessage ();
            void setSlotnum(const UI32 &slotnum);
            UI32 getSlotnum()  const;
            void setCmdCode(const UI32 &cmdcode);
            UI32 getCmdCode()  const;
            void setResult(const SI32 &result);
            SI32 getResult()  const;
			void setOutput(const string &output);
	        string getOutput() const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_slotnum;
            UI32  m_cmdcode;
            SI32  m_result;
			string m_output;
    };
}
#endif
