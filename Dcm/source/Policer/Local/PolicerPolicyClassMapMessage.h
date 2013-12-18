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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#ifndef POLICERPOLICYCLASSMAPMESSAGE_H
#define POLICERPOLICYCLASSMAPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerPolicyClassMapMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PolicerPolicyClassMapMessage ();
            PolicerPolicyClassMapMessage(const string &policyClassMapName,
			const UI64 &cir, const UI64 &cbs, const UI64 &eir, const UI64 &ebs,
			const UI32 &conformDscp, const UI32 &conformPrecedence,
			const UI32 &conformType, const UI32 &conformTc,
			const UI32 &exceedDscp, const UI32 &exceedPrecedence,
			const UI32 &exceedType, const UI32 &exceedTc,
			const string &classMapId, const string &priorityMapId,
			const UI32 &opcode,
			const UI8 &cos, const UI8 &trafficClass, const UI16 &dscp,
			const string &CoSMutationMapName,
			const string &CoSTrafficClassMapName, const string &dscpCoSMapName,
			const string &dscpMutationMapName,
			const string &dscpTrafficClassMapName, const string &sflowMapName,
			const string &ceeMapName, const UI64 &shapingRate,

			const UI8 &strictPriority,

			const UI32 &dwrrTrafficClass0, const UI32 &dwrrTrafficClass1,
			const UI32 &dwrrTrafficClass2, const UI32 &dwrrTrafficClass3,
			const UI32 &dwrrTrafficClass4, const UI32 &dwrrTrafficClass5,
			const UI32 &dwrrTrafficClass6, const UI32 &dwrrTrafficClass7,

			const UI64 &shapingRateTrafficClass0,
			const UI64 &shapingRateTrafficClass1,
			const UI64 &shapingRateTrafficClass2,
			const UI64 &shapingRateTrafficClass3,
			const UI64 &shapingRateTrafficClass4,
			const UI64 &shapingRateTrafficClass5,
			const UI64 &shapingRateTrafficClass6,
			const UI64 &shapingRateTrafficClass7);

            virtual    ~PolicerPolicyClassMapMessage ();
            void setPolicyClassMapName(const string &policyClassMapName);
            string getPolicyClassMapName()  const;
            void setCir(const UI64 &cir);
            UI64 getCir()  const;
            void setCbs(const UI64 &cbs);
            UI64 getCbs()  const;
            void setEir(const UI64 &eir);
            UI64 getEir()  const;
            void setEbs(const UI64 &ebs);
            UI64 getEbs()  const;
            void setConformDscp(const UI32 &conformDscp);
            UI32 getConformDscp()  const;
            void setConformPrecedence(const UI32 &conformPrecedence);
            UI32 getConformPrecedence()  const;
			void setConformType(const UI32 &conformType);
            UI32 getConformType()  const;			
            void setConformTc(const UI32 &conformTc);
            UI32 getConformTc()  const;
            void setExceedDscp(const UI32 &exceedDscp);
            UI32 getExceedDscp()  const;
            void setExceedPrecedence(const UI32 &exceedPrecedence);
            UI32 getExceedPrecedence()  const;
            void setExceedType(const UI32 &exceedType);
            UI32 getExceedType()  const;
			void setExceedTc(const UI32 &exceedTc);
            UI32 getExceedTc()  const;
            void setClassMapId(const string &classMapId);
            string getClassMapId()  const;
            void setPriorityMapId(const string &priorityMapId);
            string getPriorityMapId()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;

            void setCoS(const UI8 &cos);
            UI8 getCoS()  const;

            void setTrafficClass(const UI8 &trafficClass);
            UI8 getTrafficClass()  const;

            void setDscp(const UI16 &dscp);
            UI16 getDscp()  const;

            void setCoSMutationMapName( const string &cosMutationMapName);
            string getCoSMutationMapName() const;
            void setCoSTrafficClassMapName( const string &cosTrafficClassMapName);
            string getCoSTrafficClassMapName() const;
            void setDscpCoSMapName( const string &dscpCoSMapName);
            string getDscpCoSMapName() const;
            void setDscpMutationMapName( const string &dscpMutationMapName);
            string getDscpMutationMapName() const;
            void setDscpTrafficClassMapName( const string &dscpTrafficClassMapName);
            string getDscpTrafficClassMapName() const;
            void setSflowMapName( const string &sflowMapName);
            string getSflowMapName() const;
            void setCeeMapName( const string &ceeMapName);
            string getCeeMapName() const;

            void setShapingRate(const UI64 &shapingRate);
            UI64 getShapingRate()  const;

            void setStrictPriority(const UI8 &strictPriority);
            UI8 getStrictPriority()  const;

            void setDwrrTrafficClass0(const UI32 &dwrrTrafficClass0);
            UI32 getDwrrTrafficClass0()  const;
            void setDwrrTrafficClass1(const UI32 &dwrrTrafficClass1);
            UI32 getDwrrTrafficClass1()  const;
            void setDwrrTrafficClass2(const UI32 &dwrrTrafficClass2);
            UI32 getDwrrTrafficClass2()  const;
            void setDwrrTrafficClass3(const UI32 &dwrrTrafficClass3);
            UI32 getDwrrTrafficClass3()  const;
            void setDwrrTrafficClass4(const UI32 &dwrrTrafficClass4);
            UI32 getDwrrTrafficClass4()  const;
            void setDwrrTrafficClass5(const UI32 &dwrrTrafficClass5);
            UI32 getDwrrTrafficClass5()  const;
            void setDwrrTrafficClass6(const UI32 &dwrrTrafficClass6);
            UI32 getDwrrTrafficClass6()  const;
            void setDwrrTrafficClass7(const UI32 &dwrrTrafficClass7);
            UI32 getDwrrTrafficClass7()  const;

            void setShapingRateTrafficClass0(const UI64 &shapingRateTrafficClass0);
            UI64 getShapingRateTrafficClass0()  const;
            void setShapingRateTrafficClass1(const UI64 &shapingRateTrafficClass1);
            UI64 getShapingRateTrafficClass1()  const;
            void setShapingRateTrafficClass2(const UI64 &shapingRateTrafficClass2);
            UI64 getShapingRateTrafficClass2()  const;
            void setShapingRateTrafficClass3(const UI64 &shapingRateTrafficClass3);
            UI64 getShapingRateTrafficClass3()  const;
            void setShapingRateTrafficClass4(const UI64 &shapingRateTrafficClass4);
            UI64 getShapingRateTrafficClass4()  const;
            void setShapingRateTrafficClass5(const UI64 &shapingRateTrafficClass5);
            UI64 getShapingRateTrafficClass5()  const;
            void setShapingRateTrafficClass6(const UI64 &shapingRateTrafficClass6);
            UI64 getShapingRateTrafficClass6()  const;
            void setShapingRateTrafficClass7(const UI64 &shapingRateTrafficClass7);
            UI64 getShapingRateTrafficClass7()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_policyClassMapName;
            UI64  m_cir;
            UI64  m_cbs;
            UI64  m_eir;
            UI64  m_ebs;
            UI32  m_conformDscp;
            UI32  m_conformPrecedence;
			UI32 m_conformType;
            UI32  m_conformTc;
            UI32  m_exceedDscp;
            UI32  m_exceedPrecedence;
			UI32 m_exceedType;
            UI32  m_exceedTc;
            string  m_classMapId;
            string  m_priorityMapId;
            UI32  m_opcode;

            UI8   m_cos;
            UI8   m_trafficClass;
            UI16  m_dscp;

            string m_CoSMutationMapName;
            string m_CoSTrafficClassMapName;
            string m_dscpCoSMapName;
            string m_dscpMutationMapName;
            string m_dscpTrafficClassMapName;
            string m_sflowMapName;
            string m_ceeMapName;

            UI64 m_shapingRate;

            UI8  m_strictPriority;

            UI32  m_dwrrTrafficClass0;
            UI32  m_dwrrTrafficClass1;
            UI32  m_dwrrTrafficClass2;
            UI32  m_dwrrTrafficClass3;
            UI32  m_dwrrTrafficClass4;
            UI32  m_dwrrTrafficClass5;
            UI32  m_dwrrTrafficClass6;
            UI32  m_dwrrTrafficClass7;

            UI64  m_shapingRateTrafficClass0;
            UI64  m_shapingRateTrafficClass1;
            UI64  m_shapingRateTrafficClass2;
            UI64  m_shapingRateTrafficClass3;
            UI64  m_shapingRateTrafficClass4;
            UI64  m_shapingRateTrafficClass5;
            UI64  m_shapingRateTrafficClass6;
            UI64  m_shapingRateTrafficClass7;


    };
}
#endif                                            //POLICERPOLICYCLASSMAPMESSAGE_H
