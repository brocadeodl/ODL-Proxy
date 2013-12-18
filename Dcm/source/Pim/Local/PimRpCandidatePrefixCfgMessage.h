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
 *   Author : Chirag Taunk                                                 *
 **************************************************************************/

#ifndef PIMRPCANDIDATEPREFIXCFGMESSAGE_H
#define PIMRPCANDIDATEPREFIXCFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimRpCandidatePrefixCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimRpCandidatePrefixCfgMessage ();
            PimRpCandidatePrefixCfgMessage (const UI8 &msgType,
				const UI8 &prefixAction,const IpV4Address &prefixIP,
				const UI16 &prefixLen);
            virtual    ~PimRpCandidatePrefixCfgMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
			void setAction(const UI8 &prefixAction);
            UI8 getAction()  const;
            void setPrefixIP(const IpV4Address &prefixIP);
            IpV4Address getPrefixIP()  const;
            void setPrefixLen(const UI16 &prefixLen);
            UI16 getPrefixLen()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_msgType;
			UI8  m_prefixAction;
            IpV4Address  m_prefixIP;
            UI16  m_prefixLen;
    };
}
#endif                                            //PIMRPCANDIDATEPREFIXCFGMESSAGE_H

