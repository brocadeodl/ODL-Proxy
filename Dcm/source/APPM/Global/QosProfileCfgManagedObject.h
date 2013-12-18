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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef QOSPROFILECFGMANAGEDOBJECT_H
#define QOSPROFILECFGMANAGEDOBJECT_H

#include "APPM/Global/SubProfileManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "vcs.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class PfcRxTxMapManagedObject;
    class QosProfileCfgManagedObject : virtual public SubProfileManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            QosProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            QosProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const ObjectId &ceeMapId,const SI32 &defaultCosValue,const bool &qosTrust,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> > &flowControlPfcRxTxMap,const ObjectId &cosTrafficClass,const ObjectId &cosMutation);
            virtual    ~QosProfileCfgManagedObject ();
            static string getClassName();
            void setCeeMapId(const ObjectId &ceeMapId);
            ObjectId getCeeMapId()  const;
            void setDefaultCosValue(const SI32 &defaultCosValue);
            SI32 getDefaultCosValue()  const;
            void setQosTrust(const bool &qosTrust);
            bool getQosTrust()  const;
            void setFlowControlTxEnable(const SI32 &flowControlTxEnable);
            SI32 getFlowControlTxEnable()  const;
            void setFlowControlRxEnable(const SI32 &flowControlRxEnable);
            SI32 getFlowControlRxEnable()  const;
            void setFlowControlPfcRxTxMap(const vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> > &flowControlPfcRxTxMap);
            vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> > getFlowControlPfcRxTxMap()  const;
            void setCosTrafficClass(const ObjectId &cosTrafficClass);
            ObjectId getCosTrafficClass()  const;
            void setCosMutation(const ObjectId &cosMutation);
            ObjectId getCosMutation()  const;
            Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);
            void addPfcTxRxToVector(PfcRxTxMapManagedObject *PfcTxRxMap);
            void deletePfcTxRxToVector(int index);
            void setDefaultFlowControlVals();
            UI8 sendBulkRequestToClient(const string profile_name, const UI32 profile_id);
            bool verifyCosMutMap(const string mapName);
            bool verifyCosTcMap(const string mapName);
            bool verifyCeeMap(const string mapName);
            void searchResetCosMutMap(const ObjectId cosMutation);
            void searchResetCosTcMap(const ObjectId cosTc);
            void searchResetCeeMap(const ObjectId ceeMap);
            void applyCeeMap();
            virtual UI32 activationValidate();
            UI32 activationValidateQosFcoeConflict();
            bool isDefaultConfig();
            

// Now the data members

        private:
        protected:
        public:
            ObjectId  m_ceeMapId;
            SI32  m_defaultCosValue;
            bool  m_qosTrust;
            SI32  m_flowControlTxEnable;
            SI32  m_flowControlRxEnable;
            vector<WaveManagedObjectPointer<PfcRxTxMapManagedObject> >  m_flowControlPfcRxTxMap;
            ObjectId  m_cosTrafficClass;
            ObjectId  m_cosMutation;
    };
}
#endif                                            //QOSPROFILECFGMANAGEDOBJECT_H
