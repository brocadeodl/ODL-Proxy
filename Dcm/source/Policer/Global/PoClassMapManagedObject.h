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

#ifndef POCLASSMAPMANAGEDOBJECT_H
#define POCLASSMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeUI64UC.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerGlobalObjectManager;
    class PoClassMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PoClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager);
            PoClassMapManagedObject (PolicerGlobalObjectManager *pPolicerGlobalObjectManager,const UI64UC &cir,const UI64UC &cbs,const UI64UC &eir,const UI64UC &ebs,const UI32UC &conformDscp,const UI32UC &conformPrecedence,const UI32 &conformType, const UI32UC &conformTc,const UI32UC &exceedDscp,const UI32UC &exceedPrecedence,const UI32 &exceedType,const UI32UC &exceedTc,const ObjectId &classMapId,const ObjectId &priorityMapId,
            		const UI8 &cos, const UI8 &traffic_class, const UI16 &dscp,const ObjectId &sflowMapId, const ObjectId &cosMutationMapId,const ObjectId &cosTrafficClassMapId,
            		const ObjectId &dscpMutationMapId,
            		const ObjectId &dscpCosMapId,
            		const ObjectId &dscpTrafficClassMapId,
            		const ObjectId &ceeMapId,

            		const UI64 &shapingRate,

            		const UI8 &strictPriority,

            		const UI32 &dwrrTrafficClass0,
            		const UI32 &dwrrTrafficClass1,
            		const UI32 &dwrrTrafficClass2,
            		const UI32 &dwrrTrafficClass3,
            		const UI32 &dwrrTrafficClass4,
            		const UI32 &dwrrTrafficClass5,
            		const UI32 &dwrrTrafficClass6,
            		const UI32 &dwrrTrafficClass7,

            		const UI64 &shapingRateTrafficClass0,
            		const UI64 &shapingRateTrafficClass1,
            		const UI64 &shapingRateTrafficClass2,
            		const UI64 &shapingRateTrafficClass3,
            		const UI64 &shapingRateTrafficClass4,
            		const UI64 &shapingRateTrafficClass5,
            		const UI64 &shapingRateTrafficClass6,
            		const UI64 &shapingRateTrafficClass7);
            virtual    ~PoClassMapManagedObject ();
            static string getClassName();
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
            void setClassMapId(const ObjectId &classMapId);
            ObjectId getClassMapId()  const;
            void setPriorityMapId(const ObjectId &priorityMapId);
            ObjectId getPriorityMapId()  const;
			UI32 getCase (const UI32 &userChoiceWyserTag);

			UI8 getCoS() const;
			void setCoS(const UI8 &cos);

			UI8 getTrafficClass() const;
			void setTrafficClass(const UI8 &trafficClass);

			UI16 getDscp() const;
			void setDscp(const UI16 &dscp);

			UI64 getShapingRate() const;
			void setShapingRate(const UI64 &shapingRate);

			ObjectId getSflowMap() const;
			void setSflowMap(const ObjectId &sflowMapId);

			ObjectId getCoSMutationMap() const;
			void setCoSMutationMap(const ObjectId &cosMutationMapId);

			ObjectId getCoSTrafficClassMap() const;
			void setCoSTrafficClassMap(const ObjectId &cosToTrafficClassMapId);

			ObjectId getDscpCoSMap() const;
			void setDscpCoSMap(const ObjectId &dscpToCosMapId);

			ObjectId getDscpMutationMap() const;
			void setDscpMutationMap(const ObjectId &dscpMutationMapId);

			ObjectId getCeeMap() const;
			void setCeeMap(const ObjectId &ceeMapId);

			ObjectId getDscpTrafficClassMap() const;
			void setDscpTrafficClassMap(const ObjectId &dscpToTrafficClassMapId);

			UI8 getStrictPriority() const;
			void setStrictPriority(const UI8 &strictPriority);

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
            void setDwrrTrafficClassLast(const UI32 &dwrrTrafficClassLast);
            UI32 getDwrrTrafficClassLast()  const;


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
            UI64UC  m_cir;
            UI64UC  m_cbs;
            UI64UC  m_eir;
            UI64UC  m_ebs;
            UI32UC  m_conformDscp;
            UI32UC  m_conformPrecedence;
            UI32 m_conformType;
			UI32UC  m_conformTc;
            UI32UC  m_exceedDscp;
            UI32UC  m_exceedPrecedence;
			UI32 m_exceedType;
            UI32UC  m_exceedTc;
            ObjectId  m_classMapId;
            ObjectId  m_priorityMapId;

            UI8  m_cos;
            UI8  m_traffic_class;
            UI16  m_dscp;

            ObjectId  m_sflowMapId;
            ObjectId  m_cosMutationMapId;
            ObjectId  m_cosTrafficClassMapId;
            ObjectId  m_dscpMutationMapId;
            ObjectId  m_dscpCosMapId;
            ObjectId  m_dscpTrafficClassMapId;
            ObjectId  m_ceeMapId;

            UI64  m_shapingRate;

            UI8  m_strictPriority;
            UI32  m_dwrrTrafficClass0;
            UI32  m_dwrrTrafficClass1;
            UI32  m_dwrrTrafficClass2;
            UI32  m_dwrrTrafficClass3;
            UI32  m_dwrrTrafficClass4;
            UI32  m_dwrrTrafficClass5;
            UI32  m_dwrrTrafficClass6;
            UI32  m_dwrrTrafficClassLast;

            UI64  m_shapingRateTrafficClass0;
            UI64  m_shapingRateTrafficClass1;
            UI64  m_shapingRateTrafficClass2;
            UI64  m_shapingRateTrafficClass3;
            UI64  m_shapingRateTrafficClass4;
            UI64  m_shapingRateTrafficClass5;
            UI64  m_shapingRateTrafficClass6;
            UI64  m_shapingRateTrafficClass7;

            UI32  m_scheduler_type;

    };
}
#endif                                            //PoClassMapManagedObject_H
