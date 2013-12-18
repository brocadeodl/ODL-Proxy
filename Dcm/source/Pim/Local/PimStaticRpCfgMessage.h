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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMSTATICRPCFGMESSAGE_H
#define PIMSTATICRPCFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimStaticRpCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimStaticRpCfgMessage ();
            PimStaticRpCfgMessage (const UI8 &msgType,const IpV4Address &rpAddr,const string &prefixList);
            virtual    ~PimStaticRpCfgMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
            void setRpAddr(const IpV4Address &rpAddr);
            IpV4Address getRpAddr()  const;
            void setPrefixList(const string &prefixList);
            string getPrefixList()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_msgType;
            IpV4Address  m_rpAddr;
            string  m_prefixList;
    };
}
#endif                                            //PIMSTATICRPCFGMESSAGE_H
