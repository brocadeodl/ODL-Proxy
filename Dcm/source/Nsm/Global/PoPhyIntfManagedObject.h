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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef POPHYINTFMANAGEDOBJECT_H
#define POPHYINTFMANAGEDOBJECT_H

#include "Nsm/Global/IntfManagedObject.h"
#include "Nsm/Local/PVlanTrunkAssocManagedObject.h"
#include "Nsm/Local/PVlanMappingManagedObject.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Policer/Global/PolicyClassMapManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

  	class QosCosToProfileIdManagedObject;
	class QosCosToTailDropManagedObject;
	class PfcFlowControlLocalManagedObject;
    class XstpPortConfigManagedObject;
    class XstpPortInstanceConfigManagedObject;
	class PortSecurityLocalManagedObject;
	class MacAddressVlanClassificationMO;
	class MacGroupVlanClassificationMO;
	class CtagVlanClassificationMO;
	class CtagVlanClassificationRemoveMO;
	class CtagPVlanClassificationMO;
	class CtagPVlanClassificationRemoveMO;
	class AMPPProfileCompositeManagedObject;
	class AMPPProfileDomainCompositeManagedObject;

    class PoPhyIntfManagedObject : virtual public IntfManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PoPhyIntfManagedObject (WaveObjectManager *pWaveObjectManager);
            PoPhyIntfManagedObject (WaveObjectManager *pWaveObjectManager,const bool &swModeFlag,const swModeEnum &swMode,const bool &isAllowedVlanAll,const UI32Range &vlanRangeAdd, const UI32Range &vlanRangeRemove, const UI32Range &vlanRangeExcept, const vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> > &portVlanXstpConfig,  const vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> > &portInstanceXstpConfig, const bool &isPortProfiled,const bool &pVlanModePromiscuous, const bool &pVlanModeHost, const UI32 &pVlanNativeVlanId, const bool &pVlanIsAllowedVlanAll, const bool &pVlanIsAllowedVlanNone, const UI32Range &pVlanRangeAdd, const UI32Range &pVlanRangeRemove, const UI32Range &pVlanRangeExcept);
            virtual    ~PoPhyIntfManagedObject ();
            static string getClassName();
            void setSwModeFlag(const bool &swModeFlag);
            bool getSwModeFlag()  const;
            void setSwMode(const swModeEnum &swMode);
            swModeEnum getSwMode()  const;
            void setPvlanModePromiscuous(const bool &pVlanModePromiscuous);
            bool getPvlanModePromiscuous()  const;
            void setPvlanModeHost(const bool &pVlanModeHost);
            bool getPvlanModeHost()  const;
            void setPvlanModeTrunkBas(const bool &pVlanModeTrunkBas);
            bool getPvlanModeTrunkBas()  const;
            void setPvlanModeTrunkPromis(const bool &pVlanModeTrunkPromis);
            bool getPvlanModeTrunkPromis()  const;
            void setPvlanModeTrunkHost(const bool &pVlanModeTrunkHost);
            bool getPvlanModeTrunkHost()  const;
            void setPvlanNativeVlanId(const UI32 &pVlanNativeVlanId);
            UI32 getPvlanNativeVlanId()  const;
			void setPvlanIsAllowedVlanAll(const bool &pVlanIsAllowedVlanAll);
            bool getPvlanIsAllowedVlanAll()  const;
			void setPvlanIsAllowedVlanNone (const bool &pVlanIsAllowedVlanNone);
            bool getPvlanIsAllowedVlanNone ()  const;
			void setPvlanRangeAdd (const UI32Range &pVlanRangeAdd);
			UI32Range getPvlanRangeAdd ()  const;
			void setPvlanRangeRemove(const UI32Range &pVlanRangeRemove);
			UI32Range getPvlanRangeRemove ()  const;
			void setPvlanRangeExcept (const UI32Range &pVlanRangeExcept);
			UI32Range getPvlanRangeExcept ()  const;
			void setPvlanHostPrimVlanId(const UI32 &pVlanHostPrimVlanId);
			UI32 getPvlanHostPrimVlanId()  const;
			void setPvlanHostSecVlanId(const UI32 &pVlanHostSecVlanId);
			UI32 getPvlanHostSecVlanId()  const;

			void setPVlanTrunkAssoc(const vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> > &pVlanTrunkAssoc);
			vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> > getPVlanTrunkAssoc()  const;
			void addPVlanTrunkAssoc(PVlanTrunkAssocManagedObject *pVlanTrunkAssocMO);
            SI32 deletePVlanTrunkAssocPrimSec(const UI32 &primId, const UI32 &secId);
            SI32 deletePVlanTrunkAssocPrim(const UI32 &primId);
            SI32 deletePVlanTrunkAssocAll();

			void setPVlanMapping(const vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > &pVlanMapping);
			vector<WaveManagedObjectPointer<PVlanMappingManagedObject> > getPVlanMapping()  const;
			void addPVlanMapping(PVlanMappingManagedObject *pVlanMappingMO);
			SI32 deletePVlanMappingPrim(const UI32 &primId);
			SI32 deletePVlanMappingAll();
			void cleanUpPvlanAll();
			void cleanUpAccessTrunkAll();
            void setIsAllowedVlanAll(const bool &isAllowedVlanAll);
            bool getIsAllowedVlanAll()  const;
            void setIgnoreSplit(const bool &flag);
            bool getIgnoreSplit()  const;
            void setIsAllowedVlanNone(const bool &isAllowedVlanNone);
            bool getIsAllowedVlanNone()  const;

            void setVlanRangeAdd(const UI32Range &vlanRangeAdd);
            void setVlanRangeRemove(const UI32Range &vlanRangeRemove);
            void setVlanRangeExcept(const UI32Range &vlanRangeExcept);
            UI32Range getVlanRangeAdd()  const;
            UI32Range getVlanRangeRemove()  const;
            UI32Range getVlanRangeExcept()  const;
            void  setAccessVlanObjectId(const ObjectId &accessVlanObjectId);
            ObjectId  getAccessVlanObjectId() const;

			void setFlowControlTx(const SI32 &flowControlTx);
            SI32 getFlowControlTx()  const;
            void setFlowControlRx(const SI32 &flowControlRx);
            SI32 getFlowControlRx()  const;
            void setQosTrust(const bool &qosTrust);
            bool getQosTrust()  const;
			void setQosDscpTrust(const bool &qosDscpTrust);
            bool getQosDscpTrust()  const;
            void setPriorityTag(const bool &priorityTag);
            bool getPriorityTag()  const;
            void setDefaultCos(const SI32 &defaultCos);
            SI32 getDefaultCos()  const;
			void setQosCosMap(const ObjectId &qosCosMap);
            ObjectId getQosCosMap()  const;
            void setQosTrafficClassMap(const ObjectId &qosTrafficClassMap);
            ObjectId getQosTrafficClassMap()  const;
            void setCeeMap(const ObjectId &ceeMap);
            ObjectId getCeeMap()  const;
            
			void setQosDscpMap(const ObjectId &qosDscpMap);
            ObjectId getQosDscpMap()  const;
            void setQosDscpTrafficClassMap(const ObjectId &qosDscpTrafficClassMap);
            ObjectId getQosDscpTrafficClassMap()  const;
            void setQosDscpCosMap(const ObjectId &dscpCosMap);
            ObjectId getQosDscpCosMap()  const;

			void setPfcFlowControl(const vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > &pfcFlowControl);
            vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> > getPfcFlowControl()  const;
            void addPfc(PfcFlowControlLocalManagedObject *pfcMO);
            SI32 deletePfc(const SI32 &pfcCos);
            SI32 deletePfcAll();
			void setCosProfile(const vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > &cosToProfile);
            vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > getCosProfile()  const;
            void addCosProfile(QosCosToProfileIdManagedObject *cosProfileMO);
            SI32 deleteCosProfile(const UI32 &cosProfile);
            SI32 deleteCosProfileAll();
			void setCosTailDrop(const vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> > &cosToTailDrop);
            vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> > getCosTailDrop()  const;
            void addCosTailDrop(QosCosToTailDropManagedObject *cosTailDropMO);
            SI32 deleteCosTailDrop(const UI32 &cosTailDrop);
            SI32 deleteCosTailDropAll();



            void  addPortVlanXstpConfig(XstpPortVlanConfigManagedObject* portVlanXstpConfig);
            vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> > getPortVlanXstpConfig() const;
            void  deletePortVlanXstpConfig(const ObjectId &vlanOid);
            void  deletePortVlanXstpConfigAll();

            void  addPortInstanceXstpConfig(XstpPortInstanceConfigManagedObject* portInstanceXstpConfig);
            vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> > getPortInstanceXstpConfig() const;
            void  deletePortInstanceXstpConfig(const ObjectId &instanceOid);
            void  deletePortInstanceXstpConfigAll();
            bool  isInstanceAllow(ObjectId &oid, UI32 id);

            WaveManagedObjectPointer<XstpPortConfigManagedObject> getPortStpConfig() const;
            void  setPortStpConfig(WaveManagedObjectPointer<XstpPortConfigManagedObject> portStpConfig);
            void  updatePortStpConfig(XstpPortConfigManagedObject *portStpConfig);
            bool  isVlanAllow(ObjectId &oid, UI32 id);
            SI32 getPfcFlowControlRow(const SI32 &pfcCos, UI32 &tx, UI32 &rx);
			SI32 getCosProfileRow(const UI32 &cos, UI32 &red_profile);
			SI32 getCosTailDropRow(const UI32 &cos, UI32 &tail_drop);
            void setIsPortProfiled(const bool &isPortProfiled);
            bool getIsPortProfiled() const;
            void setRaGuard(const bool &raGuard);
            bool getRaGuard() const;

			//QoS Policy Map
            void setInputPolicyMapId(const ObjectId &mapId);
			ObjectId getInputPolicyMapId() const;
			void setOutputPolicyMapId(const ObjectId &mapId);
            ObjectId getOutputPolicyMapId() const;
			void setPortSecConfig(const WaveManagedObjectPointer<PortSecurityLocalManagedObject> &PortSecConfig);
			WaveManagedObjectPointer<PortSecurityLocalManagedObject>  getPortSecConfig() const;
   			void deleteMacAddressVlanClassificationAll();
   			void deleteMacGroupVlanClassificationAll();
   			void deleteCtagVlanClassificationAll();
   			void deleteCtagPVlanClassificationAll();
            vector<WaveManagedObjectPointer<MacAddressVlanClassificationMO> > getMacAddressVlanClassification() const;
            vector<WaveManagedObjectPointer<MacGroupVlanClassificationMO> > getMacGroupVlanClassification() const;
            vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > getCtagVlanClassification() const;
            vector<WaveManagedObjectPointer<CtagPVlanClassificationMO> > getCtagPVlanClassification() const;
			
			/*1-n composition of port-profiles in interface*/
            void addPortProfileToInterface
                (const WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> &portProfile);
            void removePortProfileFromInterface
                (const string &portProfile);

            vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > 
				getAssociatedPortProfiles() const;
            void setAssociatedPortProfiles(const 
				vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > 
				&associatedPortProfiles);

			/*1-1 composition of port-profile-domain in interface*/
            void setAssociatedPortProfileDomain
				(const WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> &portProfileDomain);
			void updatePortProfileDomain
				(const WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> &portProfileDomain);
            WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject>
				getAssociatedPortProfileDomain() const;			

            string getAssociatedPortProfileDomainName() const;
            vector<string> getAssociatedPortProfileNames() const;

// Now the data members

        private:
        protected:
        public:
            bool  m_swModeFlag;
            swModeEnum  m_swMode;
            bool  m_pVlanModePromiscuous;
            bool  m_pVlanModeHost;
            bool  m_pVlanModeTrunkBas;
            bool  m_pVlanModeTrunkPromis;
            bool  m_pVlanModeTrunkHost;
			UI32  m_pVlanNativeVlanId;
			bool  m_pVlanIsAllowedVlanAll;
            bool  m_pVlanIsAllowedVlanNone;
            UI32Range  m_pVlanRangeAdd;
            UI32Range  m_pVlanRangeRemove;
            UI32Range  m_pVlanRangeExcept;
            UI32  m_pVlanHostPrimVlanId;
            UI32  m_pVlanHostSecVlanId;
            vector<WaveManagedObjectPointer<PVlanTrunkAssocManagedObject> >  m_pVlanTrunkAssoc;
            vector<WaveManagedObjectPointer<PVlanMappingManagedObject> >  m_pVlanMapping;
            bool m_ignore_split;
            bool  m_isAllowedVlanAll;
            bool  m_isAllowedVlanNone;
            ObjectId  m_accessVlanObjectId;
            UI32Range  m_vlanRangeAdd;
            UI32Range  m_vlanRangeRemove;
            UI32Range  m_vlanRangeExcept;
			SI32  m_flowControlTx;
            SI32  m_flowControlRx;
            bool  m_qosTrust;
            bool  m_priorityTag;
            SI32  m_defaultCos;
			bool  m_qosDscpTrust;
			
			// port vlan xstp config
            vector<WaveManagedObjectPointer<XstpPortVlanConfigManagedObject> > m_portVlanXstpConfig;
			// port instance xstp config
            vector<WaveManagedObjectPointer<XstpPortInstanceConfigManagedObject> > m_portInstanceXstpConfig;

			ObjectId  m_qosCosMap;
            ObjectId  m_qosTrafficClassMap;
            ObjectId  m_ceeMap;
            
			ObjectId  m_qosDscpMap;
            ObjectId  m_qosDscpTrafficClassMap;
            ObjectId  m_qosDscpCosMap;

			vector<WaveManagedObjectPointer<PfcFlowControlLocalManagedObject> >  m_pfcFlowControl;
			//vector<WaveManagedObjectPointer<LocalManagedObject> >  m_pfcFlowControl;
			vector<WaveManagedObjectPointer<QosCosToProfileIdManagedObject> > m_cosProfile;
			vector<WaveManagedObjectPointer<QosCosToTailDropManagedObject> > m_cosTailDrop;			

            WaveManagedObjectPointer<XstpPortConfigManagedObject> m_portStpConfig;
            bool m_isPortProfiled;
            vector<WaveManagedObjectPointer<AMPPProfileCompositeManagedObject> > m_associatedPortProfiles;
            WaveManagedObjectPointer<AMPPProfileDomainCompositeManagedObject> m_associatedPortProfileDomain;

            //QoS Policy Map
			ObjectId m_inputPolicyMapId;
			ObjectId m_outputPolicyMapId;
			bool m_raGuard;
			WaveManagedObjectPointer<PortSecurityLocalManagedObject>  m_portSecConfig;
			vector<WaveManagedObjectPointer<MacAddressVlanClassificationMO> > m_macAddressVlanClassification;
			vector<WaveManagedObjectPointer<MacGroupVlanClassificationMO> > m_macGroupVlanClassification;
			vector<WaveManagedObjectPointer<CtagVlanClassificationMO> > m_ctagVlanClassification;
			vector<WaveManagedObjectPointer<CtagVlanClassificationRemoveMO> > m_ctagVlanClassificationRemove;
			vector<WaveManagedObjectPointer<CtagPVlanClassificationMO> > m_ctagPVlanClassification;
			vector<WaveManagedObjectPointer<CtagPVlanClassificationRemoveMO> > m_ctagPVlanClassificationRemove;
    };
}
#endif                                            //POPHYINTFMANAGEDOBJECT_H
