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

#ifndef L3ssClearMessage_H
#define L3ssClearMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
using namespace WaveNs;

namespace DcmNs
{

    class L3ssClearMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L3ssClearMessage ();
            virtual    ~L3ssClearMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setIp_addr(const IpV4Address &ip_addr);
            IpV4Address getIp_addr()  const;
            void setIp_addr_mask(const IpV4AddressNetworkMask &ip_addr_mask);
            IpV4AddressNetworkMask getIp_addr_mask() const;
            void setVrf_name(const string &vrf_name);
            string getVrf_name() const;
            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

// Now the data members

        private:
        protected:
        public:
            UI32    m_cmdCode;
            IpV4Address m_ip_addr;
            IpV4AddressNetworkMask m_ip_addr_mask;
            string  m_vrf_name;

    };
}
#endif                                            //L3ssClearMessage_H


