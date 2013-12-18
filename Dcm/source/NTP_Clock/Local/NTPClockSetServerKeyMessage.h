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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : bperuma                                                     *
 **************************************************************************/

#ifndef NTPCLOCKSETSERVERKEYMESSAGE_H
#define NTPCLOCKSETSERVERKEYMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockSetServerKeyMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NTPClockSetServerKeyMessage ();
            NTPClockSetServerKeyMessage (const string &IpAddrList,const string &server,const UI32 &Key_ID,const string &Key_Value);
            virtual    ~NTPClockSetServerKeyMessage ();
            void setIpAddrList(const string &IpAddrList);
            string getIpAddrList()  const;
            void setServer(const string &server);
            string getServer()  const;
            void setKey_ID(const UI32 &Key_ID);
            UI32 getKey_ID()  const;
            void setKey_Value(const string &Key_Value);
            string getKey_Value()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_IpAddrList;
            string  m_server;
            UI32  m_Key_ID;
            string  m_Key_Value;
    };
}
#endif                                            //NTPCLOCKSETSERVERKEYMESSAGE_H
