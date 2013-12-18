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

#ifndef MSTPBRIDGECONFIGMESSAGE_H
#define MSTPBRIDGECONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/MacAddress.h"

using namespace WaveNs;

namespace DcmNs
{
	class MstpUpdateBridgeConfigMessage;

    class MstpBridgeConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();

        public:
            MstpBridgeConfigMessage ();
            MstpBridgeConfigMessage (MstpUpdateBridgeConfigMessage & msg);
            virtual    ~MstpBridgeConfigMessage ();

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
            void setAgingTime(const UI32 &AgingTime);
            UI32 getAgingTime()  const;
            void setCiscoInterop(const bool &ciscoInterop);
            bool getCiscoInterop()  const;
            void setMacReduction(const bool &MacReduction);
            bool getMacReduction()  const;
            void setStpDisable(const bool &StpDisable);
            bool getStpDisable()  const;
            void setErrDisableEnable(const bool &ErrDisableEnable);
            bool getErrDisableEnable()  const;
            void setPoPathcost(const bool &PoPathcost);
            bool getPoPathcost()  const;
            void setVcsBridgeMac(const MacAddress &mac);
            const MacAddress &getVcsBridgeMac() const;

            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure) {
				return ;
			}

// Now the data members
        private:
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
            UI32  m_mode;
            UI32  m_agingTime;
			bool  m_ciscoInterop;
			bool  m_macReduction;
			bool  m_stpDisable;
			bool  m_errDisableEnable;
			bool  m_poPathcost;
            UI32  m_cmdCode;
            MacAddress m_vcsBridgeMac;

    };
}
#endif                                            //MSTPBRIDGECONFIGMESSAGE_H
