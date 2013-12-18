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

#ifndef MSTPUPDATEVLANBRIDGECONFIGMESSAGE_H
#define MSTPUPDATEVLANBRIDGECONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class MstpGlobalUpdateVlanBridgeConfigMessage;

    class MstpUpdateVlanBridgeConfigMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpUpdateVlanBridgeConfigMessage ();
            MstpUpdateVlanBridgeConfigMessage (MstpGlobalUpdateVlanBridgeConfigMessage & global);
            MstpUpdateVlanBridgeConfigMessage (const UI32 &id,const UI32 &priority,const UI32 &forwardDelay,const UI32 &helloTime,const UI32 &maxAge,const UI32 &agingTime, const UI32 &cmdCode, const UI32 &mode);
            virtual    ~MstpUpdateVlanBridgeConfigMessage ();
            void setId(const UI32 &id);
            UI32 getId()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setForwardDelay(const UI32 &forwardDelay);
            UI32 getForwardDelay()  const;
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setMaxAge(const UI32 &maxAge);
            UI32 getMaxAge()  const;
            void setAgingTime(const UI32 &agingTime);
            UI32 getAgingTime()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setMode(const UI32 &mode);
            UI32 getMode()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_id;
            UI32  m_priority;
            UI32  m_forwardDelay;
            UI32  m_helloTime;
            UI32  m_maxAge;
            UI32  m_agingTime;
            UI32  m_cmdCode;
            UI32  m_mode;
    };
}
#endif                                            //MSTPUPDATEVLANBRIDGECONFIGMESSAGE_H
