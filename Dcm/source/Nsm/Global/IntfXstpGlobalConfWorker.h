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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef INTFXSTPGLOBALCONFWORKER_H
#define INTFXSTPGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Nsm/Common/IntfXstpBaseWorker.h"
#include <bitset>

using namespace WaveNs;

namespace DcmNs
{

    class PoIntfManagedObject;
    class DceLinearSeqContext;
    class DceSynchronousLinearSeqContext;
    class NsmGlobalUpdatePoStpConfigMessage;
    class NsmGlobalUpdatePoVlanStpConfigMessage;
    class NsmGlobalUpdatePoInstanceStpConfigMessage;
    class NsmGlobalDeleteBridgeXstpConfigMessage;

    class IntfXstpGlobalConfWorker: public IntfXstpBaseWorker
    {
        public:
            IntfXstpGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~IntfXstpGlobalConfWorker ();

        protected:
            ResourceId fillMo(XstpPortConfigManagedObject *pPortXstpMo, NsmPortStpConfigMessage *pMessage);
            ResourceId fillMo(XstpPortInstanceConfigManagedObject *pPortInstMo, NsmPortStpConfigMessage *pMessage);
            ResourceId fillMo(XstpPortVlanConfigManagedObject *pPortVlanMo, NsmPortStpConfigMessage *pMessage);

        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmGlobalUpdatePoStpConfigMessageHandler( NsmGlobalUpdatePoStpConfigMessage *pNsmGlobalUpdatePoStpConfigMessage);
            void NsmGlobalUpdatePoVlanStpConfigMessageHandler( NsmGlobalUpdatePoVlanStpConfigMessage *pNsmGlobalUpdatePoVlanStpConfigMessage);
            void NsmGlobalUpdatePoInstanceStpConfigMessageHandler( NsmGlobalUpdatePoInstanceStpConfigMessage *pNsmGlobalUpdatePoInstanceStpConfigMessage);
            void updateGlobalPoStpConfig (DceLinearSeqContext *pPrismLinearSequencerContext);
            void IntfXstpGlobalConfSendToClusterCallback(WaveSendToClusterContext  *ctx_p);
            void updateGlobalPoStpConfigMO (DceLinearSeqContext *pSequencerContext);

            // Port Vlan Stp process
            void updatePoVlanStpConfigMO (DceLinearSeqContext *pContext);
            void updatePoVlanStpConfig (DceLinearSeqContext *pContext);

	    void NsmGlobalDeleteBridgeXstpConfigMessageHandler(NsmGlobalDeleteBridgeXstpConfigMessage *pNsmGlobalDeleteBridgeXstpConfigMessage);
			ResourceId deleteBridgeConfigMO (DceSynchronousLinearSeqContext *pSequencerContext);

            void updatePoInstanceStpConfigMO(DceLinearSeqContext *pContext);
	    void updatePoInstanceStpConfig(DceLinearSeqContext *pContext);

            void validateIntfXstpConfigStep (DceLinearSeqContext *pContext);
            void sendPoConfigMessageToCluster (const string &poId, PrismMessage *pMessage, PrismLinearSequencerContext *pContext);
            void sendToCluster(PrismMessage *pMessage, PrismLinearSequencerContext *pSequencerContext);
            void cleanupAndExecuteNextStep(WaveSendToClusterContext *pClusterContext, PrismLinearSequencerContext *pContext, ResourceId execStatus); 
        public:

			ResourceId deleteVlanIntfXstp(UI32 vid, DceLinearSeqContext *pSequencerContext);

			ResourceId createPortXstp(PoPhyIntfManagedObject *pMo);

			ResourceId deletePortXstp(PoIntfManagedObject *pMo,  DceLinearSeqContext *pSequencerContext = NULL);

			void updateVlanIntfXstp(UI32 vlanId, bool shut, PrismLinearSequencerContext *pSequencerContext);

    };

}

#endif // INTFXSTPGLOBALCONFWORKER_H
