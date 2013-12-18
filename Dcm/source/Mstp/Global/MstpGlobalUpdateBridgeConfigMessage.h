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

#ifndef MSTPGLOBALUPDATEBRIDGECONFIGMESSAGE_H
#define MSTPGLOBALUPDATEBRIDGECONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalUpdateBridgeConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpGlobalUpdateBridgeConfigMessage ();
            MstpGlobalUpdateBridgeConfigMessage (const UI32 &priority,const UI32 &forwardDelay,const UI32 &helloTime,const UI32 &maxAge,const UI32 &maxHop,const string &desc,const UI32 &errDisableInterval,const UI32 &txHoldCount,const UI16 &revision,const string &region,const UI32 &cmdCode, const UI32 &mode);
            virtual    ~MstpGlobalUpdateBridgeConfigMessage ();
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setForwardDelay(const UI32 &forwardDelay);
            UI32 getForwardDelay()  const;
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setMaxAge(const UI32 &maxAge);
            UI32 getMaxAge()  const;
            void setMaxHop(const UI32 &maxHop);
            UI32 getMaxHop()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setErrDisableInterval(const UI32 &errDisableInterval);
            UI32 getErrDisableInterval()  const;
            void setTxHoldCount(const UI32 &txHoldCount);
            UI32 getTxHoldCount()  const;
            void setRevision(const UI16 &revision);
            UI16 getRevision()  const;
            void setRegion(const string &region);
            string getRegion()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setMode(const UI32 &mode);
            UI32 getMode()  const;
			void printMsg();

// Now the data members

        private:
        protected:
        public:
            UI32  m_priority;
            UI32  m_forwardDelay;
            UI32  m_helloTime;
            UI32  m_maxAge;
            UI32  m_maxHop;
            string  m_desc;
            UI32  m_errDisableInterval;
            UI32  m_txHoldCount;
            UI16  m_revision;
            string  m_region;
            UI32  m_cmdCode;
            UI32  m_mode;
    };
}
#endif                                            //MSTPGLOBALUPDATEBRIDGECONFIGMESSAGE_H
