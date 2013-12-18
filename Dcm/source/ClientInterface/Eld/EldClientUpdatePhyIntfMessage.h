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
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#ifndef MSG_ELD_INTF_ENABLE_MESSAGE_H
#define MSG_ELD_INTF_ENABLE_MESSAGE_H

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h" 
#include "Framework/Attributes/AttributeUI32Range.h"

using namespace WaveNs;

namespace DcmNs
{

    class EldClientUpdatePhyIntfMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            EldClientUpdatePhyIntfMessage ();
            EldClientUpdatePhyIntfMessage (const UI32 &lmqi,const UI32 &prio,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation);
            virtual    ~EldClientUpdatePhyIntfMessage ();
            void setOpCode(const UI32 &opCode);
            UI32 getOpCode()  const;
			void setIfName(const string &ifName);
			string getIfName()  const;
            void setPrio(const UI32 &prio);
            UI32 getPrio()  const;
            void setVlanId(const UI32 &vlanId);
			UI32 getVlanId()  const;
			void setVlanidList(const vector<UI32> &vlanidList);
			vector<UI32> getVlanidList()  const;
			void setVlanIdRange(const UI32Range &vlanRange);
			UI32Range getVlanIdRange()  const;
			void setIfType(const UI32 &ifType);
			UI32 getIfType()  const;

			virtual const void *getCStructureForInputs();
            virtual void  loadOutputsFromCStructure(const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
			string  m_IfName;
            UI32  m_lmqi;
            UI32  m_prio;
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
			vector<UI32>  m_vlanidList;
			UI32Range     m_vlanIdRange;
			UI32  m_ifType;
    };
}
#endif                                            //MSG_ELD_INTF_ENABLE_MESSAGE_H
