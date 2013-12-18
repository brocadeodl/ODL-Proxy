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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef VLANPROFILECFGMANAGEDOBJECT_H
#define VLANPROFILECFGMANAGEDOBJECT_H

#include "APPM/Global/SubProfileManagedObject.h"
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Global/APPMNativeVlanManagedObject.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

#include "vcs.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "APPM/Global/APPMCtagBasedVlanClassificationMO.h"
#include "APPM/Global/APPMMacBasedVlanClassificationMO.h"


using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class APPMVlanManagedObject;
    class APPMVlanClassifierManagedObject;
    class APPMGlobalConfWorker;
    class APPMNativeVlanManagedObject;
    class VlanProfileCfgManagedObject : virtual public SubProfileManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
						
        public:
            VlanProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            VlanProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const bool &swModeFlag,const  VlanSwMode &swMode,const bool &isAllowedVlanAll,const bool &isAllowedVlanAllV2, const bool &isAllowedVlanNone,const UI32 &accessVlan,const UI32Range &vlanRangeAdd,const UI32Range &vlanRangeRemove,const UI32Range &vlanRangeExcept,const vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> > &vlanClassifierInfo);
            virtual    ~VlanProfileCfgManagedObject ();
            static string getClassName();
            void setSwModeFlag(const bool &swModeFlag);
            bool getSwModeFlag()  const;
            void setSwMode(const VlanSwMode &swMode);
            VlanSwMode getSwMode()  const;
            void setIsAllowedVlanAll(const bool &isAllowedVlanAll);
            bool getIsAllowedVlanAll()  const;
            void setIsAllowedVlanAllV2(const bool &isAllowedVlanAllV2);
            bool getIsAllowedVlanAllV2()  const;
            void setIsAllowedVlanNone(const bool &isAllowedVlanNone);
            bool getIsAllowedVlanNone()  const;
            void setAccessVlan(const UI32 &accessVlan);
            UI32 getAccessVlan()  const;
            void setVlanRangeAdd(const UI32Range &vlanRangeAdd);
            void setVlanRangeRemove(const UI32Range &vlanRangeRemove);
            void setVlanRangeExcept(const UI32Range &vlanRangeExcept);
            UI32Range getVlanRangeAdd()  const;
            UI32Range getVlanRangeRemove()  const;
            UI32Range getVlanRangeExcept()  const;
            void setVlanClassifierInfo(const vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> > &vlanClassifierInfo);
            vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> > getVlanClassifierInfo()  const;
			Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);
			void  addGroupIdToVector(APPMVlanClassifierManagedObject *vlanClassifierMO);
            void delGroupIdFromVector(int index);
            AppmReturnCode updateVlanClassifierInfo(APPMGlobalAPPMVlanMessage *pMsg, bool flag);
            UI8 sendBulkRequestToClient(const string profile_name, const UI32 profile_id);
			void setNativeVlanIntfConfig(const WaveManagedObjectPointer<APPMNativeVlanManagedObject> &nativeVlanIntfConfig);
			WaveManagedObjectPointer<APPMNativeVlanManagedObject> getNativeVlanIntfConfig()  const;
            void getVlans(vector<UI32>& vlans);
            bool isVlanPresent(const UI32 vlanId);
            virtual string customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext);

            bool doesVlanContainMacClassification(UI32 &vlanId);
            bool doesVlanContainCtagClassification(UI32 &vlanId);

            void deleteAllMacVlanClassifications();
            void deleteAllMacGroupVlanClassifications();
            void deleteAllCtagVlanClassifications();

            void deleteCtagVlanClassificationsOfVlan(UI32 &vlanId);
            void deleteMacVlanClassificationsOfVlan(UI32 &vlanId);
            void deleteMacGroupVlanClassificationsOfVlan(UI32 &vlanId);

            vector<WaveManagedObjectPointer<APPMMacAddressVlanClassificationMO> > getMacVlanClassifications () const;
            vector<WaveManagedObjectPointer<APPMMacGroupVlanClassificationMO> > getMacGroupVlanClassifications() const;
            vector<WaveManagedObjectPointer<APPMCtagVlanClassificationMO> > getCtagVlanClassifications() const;
// Now the data members

        private:
        protected:
        public:
            bool  m_swModeFlag;
            VlanSwMode  m_swMode;
            bool  m_isAllowedVlanAll;
            bool  m_isAllowedVlanAllV2;
            bool  m_isAllowedVlanNone;
            UI32  m_accessVlan;
            UI32Range  m_vlanRangeAdd;
            UI32Range  m_vlanRangeRemove;
            UI32Range  m_vlanRangeExcept;
            vector<WaveManagedObjectPointer<APPMVlanClassifierManagedObject> >  m_vlanClassifierInfo;
			WaveManagedObjectPointer<APPMNativeVlanManagedObject>  m_nativeVlanIntfConfig;

            vector<WaveManagedObjectPointer<APPMMacAddressVlanClassificationMO> >   m_macVlanClassifications;
            vector<WaveManagedObjectPointer<APPMMacGroupVlanClassificationMO> >     m_macGroupVlanClassifications;
            vector<WaveManagedObjectPointer<APPMCtagVlanClassificationMO> >         m_ctagVlanClassifications;
            vector<WaveManagedObjectPointer<APPMCtagVlanClassificationRemoveMO> >   m_removeCtagVlanClassifications;

    };
}
#endif                                            //VLANPROFILECFGMANAGEDOBJECT_H
