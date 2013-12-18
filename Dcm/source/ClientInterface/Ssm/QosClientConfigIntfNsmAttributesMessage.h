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

#ifndef QOSCLIENTCONFIGINTFNSMATTRIBUTESMESSAGE_H 
#define QOSCLIENTCONFIGINTFNSMATTRIBUTESMESSAGE_H 

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosClientConfigIntfNsmAttributesMessage : public DcmManagementInterfaceMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosClientConfigIntfNsmAttributesMessage();
            QosClientConfigIntfNsmAttributesMessage(const string &ifName,const SI32 &flowControlTx,const SI32 &flowControlRx,const bool &qosTrust,const bool &qosDscpTrust, const SI32 &defaultCosValue,const SI32 &cmdCode,const SI32 &OpCode);
            virtual    ~QosClientConfigIntfNsmAttributesMessage();
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
            void setDscpToCosMapName(const string &dscpToCosMutationMapName);
            string getDscpToCosMapName()  const;
	
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
			void setPfcFlowControlCos(const SI32 &pfcFlowControlCos);
            SI32 getPfcFlowControlCos()  const;
            void setPfcFlowControlTx(const UI32 &pfcFlowControlTx);
            UI32 getPfcFlowControlTx()  const;
            void setPfcFlowControlRx(const UI32 &pfcFlowControlRx);
            UI32 getPfcFlowControlRx()  const;
			void setCos0Threshold(const UI32 &threshold);
			UI32 getCos0Threshold() const;
			void setCos1Threshold(const UI32 &threshold);
			UI32 getCos1Threshold() const;
			void setCos2Threshold(const UI32 &threshold);
			UI32 getCos2Threshold() const;
			void setCos3Threshold(const UI32 &threshold);
			UI32 getCos3Threshold() const;
			void setCos4Threshold(const UI32 &threshold);
			UI32 getCos4Threshold() const;
			void setCos5Threshold(const UI32 &threshold);
			UI32 getCos5Threshold() const;
			void setCos6Threshold(const UI32 &threshold);
			UI32 getCos6Threshold() const;
			void setCos7Threshold(const UI32 &threshold);
			UI32 getCos7Threshold() const;
			void setDistance(const LongDistance &distance);
			LongDistance getDistance() const;
			void setCos(const UI32 &cos);
            UI32 getCos()  const;
            void setRedProfile(const UI32 &redProfile);
            UI32 getRedProfile()  const;
            void setTailDrop(const UI32 &tailDrop);
            UI32 getTailDrop()  const;
	
			virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);


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
            string  m_ceeMapName;
			string  m_dscpToDscpMutationMapName;
			string  m_dscpToTrafficClassMapName;
			string  m_dscpToCosMapName;
			SI32  m_pfcFlowControlCos;
            UI32  m_pfcFlowControlTx;
            UI32  m_pfcFlowControlRx;
			UI32 m_cos0Threshold;
			UI32 m_cos1Threshold;
			UI32 m_cos2Threshold;
			UI32 m_cos3Threshold;
			UI32 m_cos4Threshold;
			UI32 m_cos5Threshold;
			UI32 m_cos6Threshold;
			UI32 m_cos7Threshold;
			LongDistance  m_distance;
			UI32  m_cos;
			UI32  m_redProfile;
			UI32  m_tailDrop;
    };
}
#endif 												//QOSCLIENTCONFIGINTFNSMATTRIBUTESMESSAGE_H
