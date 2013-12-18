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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#ifndef ARPCLEARLOCALMESSAGE_H
#define ARPCLEARLOCALMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpClearLocalMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ArpClearLocalMessage ();
            ArpClearLocalMessage (const string &ifName,const UI32 &cmdCode, const IpV4Address &ip_addr, const string &mac_addr);
            virtual    ~ArpClearLocalMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIp_addr(const IpV4Address &ip_addr);
            IpV4Address getIp_addr()  const;
            void setMac_addr(const string &mac_addr);
            string getMac_addr()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32    m_cmdCode;
            IpV4Address	m_ip_addr;
            string m_mac_addr;

    };
}
#endif                                            //ARPCLEARLOCALMESSAGE_H
