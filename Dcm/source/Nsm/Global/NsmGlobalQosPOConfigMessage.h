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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef NSMGLOBALQOSPOCONFIGMESSAGE_H
#define NSMGLOBALQOSPOCONFIGMESSAGE_H 

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalQosPOConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalQosPOConfigMessage ();
            NsmGlobalQosPOConfigMessage (const UI32&poId,const UI32 &flowControlTx,const UI32 &flowControlRx,const bool &qosTrust, const bool &qosDscpTrust,const SI32 &defaultCosValue,const SI32 &eldOpCode,const SI32 &OpCode, const string &qosPolicyMapName);
            virtual    ~NsmGlobalQosPOConfigMessage ();
            void setPoId(const UI32 &poId);
            UI32 getPoId()  const;
            void setFlowControlTx(const UI32 &flowControlTx);
            UI32 getFlowControlTx()  const;
            void setFlowControlRx(const UI32 &flowControlRx);
            UI32 getFlowControlRx()  const;
            void setQosTrust(const bool &qosTrust);
            bool getQosTrust()  const;
            void setQosDscpTrust(const bool &qosDscpTrust);
            bool getQosDscpTrust()  const;
			void setDefaultCosValue(const SI32 &defaultCosValue);
            SI32 getDefaultCosValue()  const;
            void setCmdCode(const SI32 &fieldOpCode);
            SI32 getCmdCode()  const;
            void setOpCode(const SI32 &OpCode);
            SI32 getOpCode()  const;
			void setCosToCosMutationMapName(const string &cosToCosMutationMapName);
            string getCosToCosMutationMapName()  const;
            void setCosToTrafficClassMapName(const string &cosToTrafficClassMapName);
            string getCosToTrafficClassMapName()  const;
			void setDscpToDscpMutationMapName(const string &dscpToDscpMutationMapName);
            string getDscpToDscpMutationMapName()  const;
            void setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName);
            string getDscpToTrafficClassMapName()  const;
            void setDscpToCosMapName(const string &dscpToCosMapName);
            string getDscpToCosMapName()  const;
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
			void setQosPolicyMapName(const string &qosPolicyMapName);
			string getQosPolicyMapName() const;


// Now the data members

        private:
        protected:
        public:
            UI32  m_poId;
            UI32  m_flowControlTx;
            UI32  m_flowControlRx;
            bool  m_qosTrust;
            bool m_qosDscpTrust;
			SI32  m_defaultCosValue;
            SI32  m_cmdCode;
            SI32  m_OpCode;
			string  m_cosToCosMutationMapName;
            string  m_cosToTrafficClassMapName;
            string  m_dscpToDscpMutationMapName;
            string  m_dscpToTrafficClassMapName;
            string  m_dscpToCosMapName;
			string  m_ceeMapName;
			string  m_qosPolicyMapName;

    };
}
#endif 											//NSMGLOBALQOSPOCONFIGMESSAGE_H
