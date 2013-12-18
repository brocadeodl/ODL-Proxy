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

#ifndef PIMBSRCANDIDATECFGMESSAGE_H
#define PIMBSRCANDIDATECFGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimBsrCandidateCfgMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimBsrCandidateCfgMessage ();
            PimBsrCandidateCfgMessage (const UI8 &msgType,const UI8 &intfType,const string &intfName,const UI16 &mask,const UI16 &priority);
            virtual    ~PimBsrCandidateCfgMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
            void setIntfType(const UI8 &intfType);
            UI8 getIntfType()  const;
            void setIntfName(const string &intfName);
            string getIntfName()  const;
            void setMask(const UI16 &mask);
            UI16 getMask()  const;
            void setPriority(const UI16 &priority);
            UI16 getPriority()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_msgType;
            UI8  m_intfType;
            string  m_intfName;
            UI16  m_mask;
            UI16  m_priority;
    };
}
#endif                                            //PIMBSRCANDIDATECFGMESSAGE_H
