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

#ifndef NSMSETRXLIMMESSAGE_H
#define NSMSETRXLIMMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmSetRxlimMessage: public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmSetRxlimMessage ();
            NsmSetRxlimMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc);
            virtual    ~NsmSetRxlimMessage ();
			void setRxlim(const UI32 &rxlim);
			UI32 getRxlim()  const;
			void setOpCode(const UI32 &opCode);
			UI32 getOpCode()  const;
			void setHelloInt(const UI32 &helloint);
			UI32 getHelloInt()  const;
			void setShutdnTime(const UI32 &shutdntime);
			UI32 getShutdnTime()  const;
// Now the data members

        private:
        protected:
        public:
			UI32  m_rxlim;
			UI32  m_opCode;
			UI32  m_hello_int;
			UI32  m_shutdn_time;
			string  m_poId;
            string  m_poName;
            UI32  m_poIfIndex;
            string  m_desc;
    };
}
#endif                                            //NSMSETRXLIMMESSAGE_H
