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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef LSYSLOCALOBJECTMANAGER_H
#define LSYSLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalClearSPMessage.h"
#include "L2sys/Local/L2sysLocalShowPortSecurityMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"

#define INTERFACE_TYPE_GIGAETHERNET 1
#define INTERFACE_TYPE_TENGIGAETHERNET 2
#define INTERFACE_TYPE_FORTYGIGAETHERNET 3
#define INTERFACE_TYPE_PORTCHANNEL 4

using namespace WaveNs;

namespace DcmNs
{

    class L2sysUpdateL2sysGlobalStaticMacSPMessage;
    class L2sysUpdateL2sysAgingTimeoutSPMessage;
    class L2sysUpdateL2sysClearSPMessage;
    class L2sysLocalShowMacProfileMessage;
    class L2sysShowRbridgeMacMessage;

    class L2sysLocalObjectManager: public WaveLocalObjectManager
    {
        private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void L2sysUpdateL2sysAgingTimeoutSPMessageHandler( L2sysUpdateL2sysAgingTimeoutSPMessage *pL2sysUpdateL2sysAgingTimeoutSPMessage);
			void L2sysUpdateL2sysGlobalStaticMacSPMessageHandler( L2sysUpdateL2sysGlobalStaticMacSPMessage *pL2sysUpdateL2sysGlobalStaticMacSPMessage);
			void L2sysUpdateL2sysGlobalClearSPMessageHandler( L2sysUpdateL2sysGlobalClearSPMessage *pL2sysUpdateL2sysGlobalClearSPMessage);
			void L2sysLocalShowMacProfileMessageHandler (L2sysLocalShowMacProfileMessage *pL2sysLocalShowMacProfileMessage);
			ResourceId L2sysUpdateMac(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateL2sysAging (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId L2sysClearMac(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId getPhyIntfIfindexByPoIfName (string phyName, UI64 *ifindex);
			ResourceId showMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			void showMessageClusterStepCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
			ResourceId showMessageClusterStep(PrismLinearSequencerContext *pPrismLinearSequencerContext);	
			void postBootConfigureL2sysIntefaceConfig();
			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase);
			void postBootConfigureL2sysAgingConfig();
			ResourceId sendL2sysShowMacCommandToL2sysDaemon(PrismLinearSequencerContext *seq_ctx_p);
			void  sendL2sysShowMacCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
			void L2sysShowRbridgeMacMessageHandler(L2sysShowRbridgeMacMessage* pL2sysShowRbridgeMacMessage);
			void L2sysShowMacSendToClusterCallback(WaveSendToClusterContext *ctx_p);
			void slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForFailover);
			void L2sysLocalShowPortSecurityMessageHandler (L2sysLocalShowPortSecurityMessage *pL2sysLocalShowPortSecurityMessage);
			ResourceId showPortSecurityMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        protected:
        public:
            L2sysLocalObjectManager ();
            virtual    ~L2sysLocalObjectManager ();
            static L2sysLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //LSYSLOCALOBJECTMANAGER_H
