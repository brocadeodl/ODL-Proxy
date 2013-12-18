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

#ifndef PIMRPCANDIDATECFGMESSAGE_H
#define PIMRPCANDIDATECFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimRpCandidateCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimRpCandidateCfgMessage ();
            PimRpCandidateCfgMessage (const UI8 &msgType,const UI8 &intfType,const string &intfName,const string &prefixList);
            virtual    ~PimRpCandidateCfgMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
            void setIntfType(const UI8 &intfType);
            UI8 getIntfType()  const;
            void setIntfName(const string &intfName);
            string getIntfName()  const;
            void setPrefixList(const string &prefixList);
            string getPrefixList()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_msgType;
            UI8  m_intfType;
            string  m_intfName;
            string  m_prefixList;
    };
}
#endif                                            //PIMRPCANDIDATECFGMESSAGE_H
