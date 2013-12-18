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

#ifndef L3ssArpShowMessage_H
#define L3ssArpShowMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
using namespace WaveNs;

namespace DcmNs
{

    class L3ssArpShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L3ssArpShowMessage ();
            virtual    ~L3ssArpShowMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
			void setMac(const string &mac);
			string getMac() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIp_addr(const IpV4Address &ip_addr);
            IpV4Address getIp_addr()  const;
            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void setLastPrinted(const UI32 &lastPrinted);
            UI32 getLastPrinted() const;
            void copyAllBuffers(const WaveNs::PrismMessage &);
            void setVrf_name(const string &);
            string getVrf_name() const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
			string m_mac;
            UI32    m_cmdCode;
            IpV4Address m_ip_addr;
            UI32    m_lastPrinted;
            string m_vrf_name;
    };
}
#endif                                            //L3ssArpShowMessage_H



