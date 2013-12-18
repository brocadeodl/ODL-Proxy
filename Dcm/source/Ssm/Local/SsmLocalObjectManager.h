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
 *   Author : pasu                                                     *
 **************************************************************************/

#ifndef SSMLOCALOBJECTMANAGER_H
#define SSMLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Utils/DceLinearSeqContext.h"
using namespace WaveNs;

namespace DcmNs
{
	class DcmManagementInterfaceMessage;
    class SsmLocalServiceAddMacAclPolicyMessage;
    class SsmLocalServiceRemMacAclPolicyMessage;
    class SsmLocalServiceAssocRuleToPolicyMessage;
    class SsmLocalServiceAssocPolicyToInterfaceMessage;
    class SsmLocalServiceResqMacAclPolicyMessage;
    class SsmMacAclShowLocalSPMessage;
	class SsmLocalIpAclWorker;
    class SsmBpratelimitLocalWorker;
	class PhyIntfLocalManagedObject;
	class NsmLocalIngressEgressMacAclPolicyManagedObject;
	class SsmMacAclIntfAssocBulkMessage;
    class StormControlLocalShowMessage;
    class StormControlLocalClearMessage;
    class StormControlLocalWorker;

    class SsmLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void SsmLocalServiceAddMacAclPolicyMessageHandler( SsmLocalServiceAddMacAclPolicyMessage *pSsmLocalServiceAddMacAclPolicyMessage);
            void SsmLocalServiceAssocPolicyToInterfaceMessageHandler( SsmLocalServiceAssocPolicyToInterfaceMessage *pSsmLocalServiceAssocPolicyToInterfaceMessage);
            void SsmLocalServiceAssocRuleToPolicyMessageHandler( SsmLocalServiceAssocRuleToPolicyMessage *pSsmLocalServiceAssocRuleToPolicyMessage);
	        void ssmLocalAssocMacAclRuleToPolicy (DceLinearSeqContext *seqContext);
            void SsmLocalServiceRemMacAclPolicyMessageHandler( SsmLocalServiceRemMacAclPolicyMessage *pSsmLocalServiceRemMacAclPolicyMessage);
            void SsmMacAclShowLocalSPMessageHandler(SsmMacAclShowLocalSPMessage *pSsmMacAclShowLocalSPMessage);
            void SsmLocalServiceResqMacAclPolicyMessageHandler(SsmLocalServiceResqMacAclPolicyMessage *pSsmLocalServiceResqMacAclPolicyMessage);
			void SsmLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			void sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
			ResourceId sendShowToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p);

            void ConfigReplayMacAclPolicy();
            void ConfigReplayStdMacAclPolicy();
            void ConfigReplayExtMacAclPolicy();
			void ConfigReplayMacAclIntf(SI32 slotNum);
            void ConfigReplayMacAclGlobalIntf(const UI32 ifType);
            void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);

			void ConfigReplayStormControlIntf(SI32 slotNum);
			ResourceId getErrorStatusPerPlatform(ResourceId status);

        protected:
        public:
            SsmLocalObjectManager ();
            virtual    ~SsmLocalObjectManager ();
            static SsmLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			static DcmManagementInterfaceMessage *ConfigReplayMacAclIntfAssoc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
			static SsmMacAclIntfAssocBulkMessage *ConfigReplayMacAclGlobalIntfAssoc(const UI32 ifType, WaveManagedObject *pWaveManagedObject);
			static SsmMacAclIntfAssocBulkMessage *ConfigReplayMacAclGlobalIntfAssoc(const UI32 ifType,
					NsmLocalIngressEgressMacAclPolicyManagedObject* nsmIngressEgressMacAclManagedObject, UI32 poVlanId);
			static const string getPolicyNameByObjId(const ObjectId objId);
            static void ConfigReplayStormControlPerInterface(PhyIntfLocalManagedObject *phyIntfLocalManagedObject);
            /*
             * ifType - one of PO_INTF_TYPE or VLAN_INTF_TYPE
             * pPoVlanResults - Vector of PoIntfManagedObject or
             *      VlanIntfManagedObject depending on ifType
             */
            static void configReplayMacAclGlobalIntf(const UI32 ifType, vector<WaveManagedObject *> *&pPoVlanResults);


// Now the data members

        private:

        protected:
        public:
			SsmLocalIpAclWorker   *m_pSsmLocalIpAclWorker;
            StormControlLocalWorker   *m_pStormControlLocalWorker;
			SsmBpratelimitLocalWorker   *m_pSsmBpratelimitLocalWorker;
    };
}
#endif                                            //SSMLOCALOBJECTMANAGER_H
