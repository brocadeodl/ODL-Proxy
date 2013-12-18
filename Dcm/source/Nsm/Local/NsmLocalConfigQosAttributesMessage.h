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

#ifndef NSMLOCALCONFIGQOSATTRIBUTESMESSAGE_H
#define NSMLOCALCONFIGQOSATTRIBUTESMESSAGE_H 

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalConfigQosAttributesMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalConfigQosAttributesMessage ();
            NsmLocalConfigQosAttributesMessage (const string &ifName,const SI32 &flowControlTx,const SI32 &flowControlRx,const bool &qosTrust,const bool &qosdscpTrust, const SI32 &defaultCosValue,const SI32 &cmdCode,const SI32 &OpCode);
            virtual    ~NsmLocalConfigQosAttributesMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setFlowControlTx(const SI32 &flowControlTx);
            SI32 getFlowControlTx()  const;
            void setFlowControlRx(const SI32 &flowControlRx);
            SI32 getFlowControlRx()  const;
            void setQosTrust(const bool &qosTrust);
            bool getQosTrust()  const;
			void setQosDscpTrust(const bool &qosDscpTrust);
            bool getQosDscpTrust()  const;
            void setDefaultCosValue(const SI32 &defaultCosValue);
            SI32 getDefaultCosValue()  const;
            void setCmdCode(const SI32 &cmdCode);
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
            void setIfType(const UI32 &ifType);
            UI32 getIfType() const;
			void setDistance(const LongDistance &distance);
			LongDistance getDistance() const;
			

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            SI32  m_flowControlTx;
            SI32  m_flowControlRx;
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
            UI32 m_ifType;
			LongDistance m_distance;

    };
}
#endif 												//NSMLOCALCONFIGQOSATTRIBUTESMESSAGE_H
