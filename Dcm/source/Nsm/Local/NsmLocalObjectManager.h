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

#ifndef NSMLOCALOBJECTMANAGER_H
#define NSMLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Nsm/Local/FcoeWorker/NsmFcoeLocalWorker.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForMultiPartitionCleanup.h"
#include "Framework/Core/NodeReadyForAllCommandsEvent.h"
#include "Nsm/Local/InterfaceStateReplayServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

typedef enum
{
    NSM_SERVICE_READY = 0,
    NSM_LINECARD_READY,
    NSM_CHASSIS_READY,
    NSM_LINECARD_ONLINE
} NsmEvents;

	class EldLocalWorker;
    class LldpLocalWorker;
    class ConfigMgmtLocalWorker;
    class IntfConfLocalWorker;
    class VlanClassifierLocalConfWorker;	
    class PVlanLocalConfWorker;
    class IntfXstpLocalConfWorker;
    class Dot1xLocalWorker;
    class VRRPLocalConfWorker;
    class NsmFcoeLocalWorker;
	class QosLocalInterfaceWorker;
	class XstpPortInstanceConfigManagedObject;
	class XstpPortConfigManagedObject;
    class VlanIntfIgmpsLocalConfWorker;
    class OSPFInterfaceLocalConfWorker;
	class PhyIntfLocalManagedObject;
	class VlanIntfManagedObject;
	class PoIntfManagedObject;
	class EcmpConfigManagedObject;
	class IntfConfLocalWorker;
	class PortSecurityLocalConfWorker;
	class PortSecurityLocalManagedObject;
	class PortSecAllowedMacsLocalManagedObject;
	class IpLocalConfigWorker;
	class NsmRmonLocalWorker; 
	class CosToCosMutationMapManagedObject;
	class CosToTrafficClassMapManagedObject;
	class DscpToTrafficClassMapManagedObject;
	class DscpToDscpMutationMapManagedObject;
	class DscpToCosMapManagedObject;
	class CeeMapManagedObject;
    class NsmLocalL3IpWorker;
    class NsmLocalIntfLoopbackWorker;
    class PimLocalIntfWorker;
    class IntfUdldConfLocalWorker;

    class NsmL3Util;
    class IgmpLocalWorker;
    class DhcpRelayLocalWorker;

    class IntIpConfigLocalManagedObject;
    class NsmLocalL3IpWorker;
    class NsmLocalIntfLoopbackWorker;
    class PimLocalIntfWorker;
    class NsmIpConfigManagedObject;
    class NsmIpAddrConfigManagedObject;
    class PoPhyIntfManagedObject;
    class NsmIntfSviManagedObject;
    class NsmIntfLoopbackManagedObject;

    class NsmLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void configReplayForVlan(const SI32 slotId = -1);
            ResourceId provisionVlansForPoPhy(
                        const PoPhyIntfManagedObject *pPoPhyMo);
        protected:
        public:
            NsmLocalObjectManager ();
            virtual    ~NsmLocalObjectManager ();
            static NsmLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

            friend class NsmL3Util;

// Now the data members

		private:
            EldLocalWorker     *m_pEldLocalWorker;
			LldpLocalWorker  *m_pLldpLocalWorker;
			ConfigMgmtLocalWorker  *m_pConfigMgmtLocalWorker;
			IntfConfLocalWorker  *m_pIntfConfLocalWorker;
			VlanClassifierLocalConfWorker  *m_pVlanClassifierLocalConfWorker;
			PVlanLocalConfWorker *m_pPVlanLocalConfWorker;
			IntfXstpLocalConfWorker  *m_pIntfXstpLocalConfWorker;
			NsmFcoeLocalWorker *m_pNsmFcoeLocalWorker;
			Dot1xLocalWorker *m_pDot1xLocalWorker;
			PortSecurityLocalConfWorker *m_pPortSecurityLocalConfWorker;
			QosLocalInterfaceWorker *m_pQosLocalInterfaceWorker;
			VlanIntfIgmpsLocalConfWorker  *m_pVlanIntfIgmpsLocalConfWorker;
			OSPFInterfaceLocalConfWorker  *m_pOSPFInterfaceLocalConfWorker;
			IpLocalConfigWorker  *m_pIpLocalConfigWorker;			
			NsmRmonLocalWorker *m_pNsmRmonLocalWorker;
            IntfUdldConfLocalWorker *m_pIntfUdldConfLocalWorker;

            VRRPLocalConfWorker *m_pVRRPLocalConfWorker;
            NsmLocalL3IpWorker *m_pNsmLocalL3IpWorker;
            NsmLocalIntfLoopbackWorker *m_pNsmLocalIntfLoopbackWorker;
            PimLocalIntfWorker *m_pPimLocalIntfWorker;
            IgmpLocalWorker  *m_pIgmpLocalWorker;
            DhcpRelayLocalWorker  *m_pDhcpRelLocalWorker;

			void postBootConfigureVlanClassifierRules();
			void postBootConfigureVlanClassifierGroup();
			void postBootConfigureVlanClassifierGroupToPo(vector<PoIntfManagedObject *> &poMOs);
			void postBootConfigureDot1xIntefaceConfig();
			void postBootConfigureDot1xGlobalConfig();
			void configReplayRmonStats();
			void configReplayRmonHistory();
			void postBootConfigureLacpGlobalConfig();
			void postBootConfigureEcmpGlobalConfig();
			void postBootConfigureNativeVlanGlobalConfig();
			void slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForFailover);
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
			void warmRecoveryPostBoot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
            void multiPartitionCleanup (WaveAsynchronousContextForMultiPartitionCleanup *cleanupContext); 
            void multiPartitionPartialCleanup(WaveAsynchronousContextForMultiPartitionCleanup *cleanupContext);
			void cleanupInterfaceVRFBind(WaveAsynchronousContextForMultiPartitionCleanup *cleanupContext, bool fullCleanup = true);
			void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd);
            void nodeReadyForAllCommandsEventHandler (const NodeReadyForAllCommandsEvent *&pNodeReadyForAllCommandsEvent);
			void bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent);

			void postBootPass1Phy1Configuration();
			/* ELD */
			void ConfigReplayEldGlobalScalars();
			void postBoot3PoEldConfiguration(vector<PoIntfManagedObject *> &poMOs);
			/* ELD */
			void postBootPass1PoConfiguration();

			//void postBootPass1PhyConfiguration();

			ResourceId Pass1VlanConfiguration(VlanIntfManagedObject *pVlanIntfManagedObject,bool createVlanFlag);

			void QueryNsmToCreatePhyIntfMO();
			void postBootPassLongDistanceConfiguration();
			void postBootPassShutdownPhyConfiguration();


			/*
			 * Sudhanshu:Hercules support for postboot
			 */
            void ConfigReplayIgmpsIntfConfig();

		protected:
		public:
            bool m_allowIslToEdge;
			ResourceId Pass1PoConfiguration(PoIntfManagedObject *pPoIntfManagedObject,bool createPoFlag);
			ResourceId Pass1PhyConfiguration(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject,CfgReplayModes CfgReplayMode);
			ResourceId PassShutdownPhyConfiguration(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);

			ResourceId PassLongDistancePhyConfiguration(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			IntfXstpLocalConfWorker* getIntfXstpLocalConfWorker();

			void configReplayXstpVlanConfig(vector<WaveManagedObject *>* pVlanResults  = NULL);
			void configReplayBpduGuardConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL,
										PoIntfManagedObject *pPoIntfLocalManagedObject = NULL );
			//ResourceId getPhyIntfByName(string & ifName, PhyIntfLocalManagedObject * &pIntfMo);
			ResourceId postBootFcoeProvisioningConfig();
			const string getPolicyNameByObjId(const ObjectId objId);
			void configReplayVlanClassifierGroupToPhy(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayVlanClassifierGroupToPo(PoIntfManagedObject *pPoIntfManagedObject);
			void configReplayRmonStats(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
		    void configReplayRmonHistory(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayRmonAlarm(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayRmonCmds(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			bool isPhyPortInISLMode(const string ifName, bool isMarkForDeletion ,PrismSynchronousLinearSequencerContext *seq_ctx_p = NULL);
			void configReplayPhyQosMaps(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPhyQosCosThresholds(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPoQosMaps(PoIntfManagedObject *pPoIntfManagedObject);
			void configReplayPhyQosDscpMaps(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPhyQosPfc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPoQosPfc(PoIntfManagedObject *pPoIntfManagedObject);
			void configReplayPhyQosCosProfile(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPhyQosCosTailDrop(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPoQosCosProfile(PoIntfManagedObject *pPoIntfManagedObject);
			void configReplayPoQosCosTailDrop(PoIntfManagedObject *pPoIntfManagedObject);
			void configReplayPhyCee(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPoCee(PoIntfManagedObject *pPoIntfManagedObject);
            void configReplayPhyPriorityTag(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
            void configReplayPoPriorityTag(PoIntfManagedObject *pPoIntfManagedObject);
			void configReplaySflowInterface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayInterfaceDesc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayDot1xInteface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayEldInteface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayEldPcInteface(PoIntfManagedObject *pPoIntfManagedObject);
			CosToCosMutationMapManagedObject *getCosToCosMutationMapMOByObjectId(const ObjectId &oId);
			CosToTrafficClassMapManagedObject *getCosToTrafficClassMapMOByObjectId(const ObjectId &oId);
			DscpToDscpMutationMapManagedObject *getDscpToDscpMutationMapMOByObjectId(const ObjectId &oId);
            DscpToTrafficClassMapManagedObject *getDscpToTrafficClassMapMOByObjectId(const ObjectId &oId);
			DscpToCosMapManagedObject *getDscpToCosMapMOByObjectId(const ObjectId &oId);
			CeeMapManagedObject *getCeeMapMOByObjectId(const ObjectId &oId);
			void ConfigReplayPerIntfLacpConfig(PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject);
			void configReplayChannelGroupToPhy(PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject);
			void ConfigReplayPerIntfLldpConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			/* APPM */ 
			bool isProfileSpecificFcoeApplied(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			bool isProfileSpecificFcoeApplied(string ifName);
			bool isFcoeConfiguredOnAnyProfiledPort();
			/* APPM over loaded funtions for reply config */
			void ConfigReplayAppmPortProfileConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			
			void configReplaySpanInterface(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
            ResourceId postBootFcoeProvisioningConfigForInterface(PhyIntfLocalManagedObject *if_mo_p);
            ResourceId postBootFcoeProvisioningConfigForPoInterface(PoIntfManagedObject *if_mo_p, bool nsm_send_override = false);
            ResourceId FcoeLocalSendEthConfigMessageToDaemon(NsmFcoeportAttrMessage *msg_p);

			FcoeMapManagedObject* fcoeMapGetMoByName (string &map_name, FcoeLinearSequencerContext *ctx_p = NULL);
			ObjectId fcoeMapGetObjectIdByName (string &map_name, FcoeLinearSequencerContext *ctx_p = NULL);
            void setAllowIslToEdge(bool allowIslToEdge);
            bool getAllowIslToEdge();

            void postbootXstpConfig(vector<PoIntfManagedObject *> &pos);

            void postBootPass1Configuration (vector<WaveManagedObject *>* pVlanResults, const bool poCreateFlag = true);
            void postBootPass1PhyConfiguration (int32_t slotId,
                LocationId locId);

            void configReplayXstpPortConfig (PoPhyIntfManagedObject *pPoPhyMo);
            void configReplayXstpPortInstanceConfig (
                PoPhyIntfManagedObject *pPoPhyMo);
            void configReplayXstpPortVlanConfig (
                PoPhyIntfManagedObject *pPoPhyMo);
            void configReplayPhyOSPFConfig(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
            void postBootPhyConfiguration (int32_t slotId, LocationId locId);
            void configReplayVlanPVlan(VlanIntfManagedObject *pVlanIntfManagedObject);
            void configReplayPhyPVlan(PhyIntfLocalManagedObject *pPhyMO, const string &ifName);
            void configReplayPoPVlan(PoIntfManagedObject *pPoMO, const string &ifName);
			void configReplayPortSecurity(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			void configReplayPoPortSecurity (PoIntfManagedObject *pPoIntfMO);
			void configReplayAllowedMacs(PortSecurityLocalManagedObject *pPortSecMO);
			void configReplayOuis(PortSecurityLocalManagedObject *pPortSecMO);
			void configReplayStickyMacs(PortSecurityLocalManagedObject *pPortSecMO);
            void configReplaySwitchportRspanVlan(PhyIntfLocalManagedObject *pPhyMO,const string &ifName);
            void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            bool isEnabled();
            void postBootPass1VlanConfiguration(vector<WaveManagedObject *>* &pVlanResults, const bool createFlag = true);
            void ConfigReplayIgmpsVlanIntfConfig(vector<WaveManagedObject *>* &pResultsPMO);
            void ConfigReplayIgmpsMrtrVlanConfig(vector<WaveManagedObject *>* &pResultsPMO);
            void ConfigReplayIgmpsSgVlanConfig(vector<WaveManagedObject *>* &pResultsPMO);
            void configReplayIgmpsIntfConfig(vector<WaveManagedObject *>* &pResultsPMO);
			void configReplayPoVlanClassification(PoIntfManagedObject *pMO, const string &ifName);
			void configReplayPhyVlanClassification(PhyIntfLocalManagedObject *pMO, const string &ifName);
            void configReplayPoIfToPortProfileAssociation(PoIntfManagedObject *pMO, const string &ifName);
            void configReplayPhyIfToPortProfileAssociation(PhyIntfLocalManagedObject *pMO, const string &ifName);

			void configReplayMacGroup();

      	};

    inline string  NsmLocalObjectManager::getClassName()
    {
        return ("NSM");
    }

class NsmServiceReadyEvent : public PrismEvent
{
	private :
	protected :
	public :
		NsmServiceReadyEvent ();
		~NsmServiceReadyEvent ();

		// Now the data members

	private :
	protected :
	public :
};

class NsmChassisReadyEvent : public PrismEvent
{
    private :
    protected :
    public :
        NsmChassisReadyEvent ();
        ~NsmChassisReadyEvent ();

        // Now the data members

    private :
    protected :
    public :
};

class NsmLinecardReadyEvent : public PrismEvent
{
    private :
        virtual void setupAttributesForSerialization ();
    protected :
    public :
        NsmLinecardReadyEvent ();
        ~NsmLinecardReadyEvent ();

        UI32    getSlotId               () const;
        void    setSlotId               (const UI32 &slotId);

        // Now the data members

    private :
        UI32   m_slotId;
    protected :
    public :
};

class NsmLinecardOnlineEvent : public PrismEvent
{
    private :
        virtual void setupAttributesForSerialization ();
    protected :
    public :
        NsmLinecardOnlineEvent ();
        ~NsmLinecardOnlineEvent ();

        UI32    getSlotId               () const;
        void    setSlotId               (const UI32 &slotId);

        // Now the data members

    private :
        UI32   m_slotId;
    protected :
    public :
};


}
#endif                                            //NSMLOCALOBJECTMANAGER_H
