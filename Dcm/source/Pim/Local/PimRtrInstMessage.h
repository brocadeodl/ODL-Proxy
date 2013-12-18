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

#ifndef PIMRTRINSTMESSAGE_H
#define PIMRTRINSTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeUI32UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimRtrInstMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimRtrInstMessage ();
            virtual    ~PimRtrInstMessage ();
            void setMsgType(const UI8 &msgType);
            UI8 getMsgType()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setVrfId(const string &vrfId);
            string getVrfId()  const;
            void setMaxMcache(const UI32 &maxMcache);
            UI32 getMaxMcache()  const;
            void setHelloTimer(const SI16 &helloTimer);
            SI16 getHelloTimer()  const;
            void setNbrTimeout(const SI16 &NbrTimeout);
            SI16 getNbrTimeout()  const;
            void setInactivityTimer(const SI16 &inactivityTimer);
            SI16 getInactivityTimer()  const;
            void setMsgInterval(const UI32 &msgInterval);
            UI32 getMsgInterval()  const;
            void setSptThreshold(const UI32 &sptThreshold);
            UI32 getSptThreshold()  const;

// Now the data members
        private:
        protected:
        public:
            UI8  m_msgType;
            UI8  m_cmdCode;
            string  m_vrfId;
            UI32  m_maxMcache;
            SI16  m_helloTimer;
            SI16  m_NbrTimeout;
            SI16  m_inactivityTimer;
            UI32  m_msgInterval;
            UI32  m_sptThreshold;
    };
}
#endif                                            //PIMRTRINSTMESSAGE_H
