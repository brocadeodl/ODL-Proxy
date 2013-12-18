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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#ifndef NSMGLOBALUPDATEIGMPSVLANCONFIGMESSAGE_H
#define NSMGLOBALUPDATEIGMPSVLANCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdateIgmpsVlanConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdateIgmpsVlanConfigMessage ();
            NsmGlobalUpdateIgmpsVlanConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation);
            virtual    ~NsmGlobalUpdateIgmpsVlanConfigMessage ();
            void setLmqi(const UI32 &lmqi);
            UI32 getLmqi()  const;
            void setQmrt(const UI32 &qmrt);
            UI32 getQmrt()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setFastLeave(const bool &fastLeave);
            bool getFastLeave()  const;
            void setQueryInterval(const UI32 &queryInterval);
            UI32 getQueryInterval()  const;
            void setQuerierEnable(const bool &querierEnable);
            bool getQuerierEnable()  const;
            void setMrouterTimeout(const UI32 &mrouterTimeout);
            UI32 getMrouterTimeout()  const;
            void setVlanId(const UI32 &vlanId);
            UI32 getVlanId()  const;
            void setVlanIfName(const string &vlanIfName);
            string getVlanIfName()  const;
            void setVlanIfIndex(const UI64 &vlanIfIndex);
            UI64 getVlanIfIndex()  const;
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
            void setNegation(const bool &negation);
            bool getNegation()  const;
            bool getRestrictUnknownMcast() const;
            void setRestrictUnknownMcast(const bool &restrictUnknownMcast);
// Now the data members

        private:
        protected:
        public:
            UI32  m_lmqi;
            UI32  m_qmrt;
            bool  m_enable;
            bool  m_fastLeave;
            UI32  m_queryInterval;
            bool  m_querierEnable;
            UI32  m_mrouterTimeout;
            UI32  m_vlanId;
            string  m_vlanIfName;
            UI64  m_vlanIfIndex;
            UI32  m_opCode;
            bool  m_negation;
            bool  m_restrictUnknownMcast;
    };
}
#endif                                            //NSMGLOBALUPDATEIGMPSVLANCONFIGMESSAGE_H
