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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef NSMUPDATEPOVLANSTPCONFIGSPMESSAGE_H
#define NSMUPDATEPOVLANSTPCONFIGSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmGlobalUpdatePoVlanStpConfigMessage;
    class NsmUpdatePoVlanStpConfigSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmUpdatePoVlanStpConfigSPMessage ();
			NsmUpdatePoVlanStpConfigSPMessage(NsmGlobalUpdatePoVlanStpConfigMessage &msg);
            NsmUpdatePoVlanStpConfigSPMessage (const string &ifName,const UI32 &vid,const UI16 &priority,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~NsmUpdatePoVlanStpConfigSPMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setVid(const UI32 &vid);
            UI32 getVid()  const;
            void setPriority(const UI16 &priority);
            UI16 getPriority()  const;
            void setCost(const UI32 &cost);
            UI32 getCost()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_vid;
            UI16  m_priority;
            UI32  m_cost;
            UI32  m_cmdCode;
    };
}
#endif                                            //NSMUPDATEPOVLANSTPCONFIGSPMESSAGE_H
