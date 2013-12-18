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

#ifndef INTFXSTPLOCALCONFWORKER_H
#define INTFXSTPLOCALCONFWORKER_H

#include "Nsm/Common/IntfXstpBaseWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmUpdatePhyStpConfigMessage;
	class NsmUpdatePhyVlanStpConfigMessage;
    class NsmUpdatePoStpConfigSPMessage;
    class NsmUpdatePoVlanStpConfigSPMessage;
	class PoPhyIntfManagedObject;
	class PhyIntfLocalManagedObject;
	class NsmUpdatePoInstanceStpConfigSPMessage;
	class NsmUpdatePhyInstanceStpConfigMessage;
    class NsmDeleteBridgeXstpConfigMessage;
    class DceSynchronousLinearSeqContext;

    class IntfXstpLocalConfWorker: public IntfXstpBaseWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmUpdatePhyStpConfigMessageHandler( NsmUpdatePhyStpConfigMessage *pNsmUpdatePhyStpConfigMessage);
            void NsmUpdatePhyVlanStpConfigMessageHandler(NsmUpdatePhyVlanStpConfigMessage *pNsmUpdatePhyVlanStpConfigMessage);
            void NsmUpdatePoStpConfigSPMessageHandler( NsmUpdatePoStpConfigSPMessage *pNsmUpdatePoStpConfigSPMessage);
            void NsmUpdatePoVlanStpConfigSPMessageHandler( NsmUpdatePoVlanStpConfigSPMessage *pNsmUpdatePoVlanStpConfigSPMessage);
            void NsmUpdatePoInstanceStpConfigSPMessageHandler( NsmUpdatePoInstanceStpConfigSPMessage *pNsmUpdatePoInstanceStpConfigSPMessage);
            void NsmUpdatePhyInstanceStpConfigMessageHandler(NsmUpdatePhyInstanceStpConfigMessage *pNsmUpdatePhyInstanceStpConfigMessage);

            // Port Vlan STP process
            ResourceId updatePhyVlanStpConfigMO(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePhyVlanStpConfig(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePoVlanStpConfigSP(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId updatePoStpConfig (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePhyStpConfig (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePoInstanceStpConfigSP(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePhyInstanceStpConfig(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePhyInstanceStpConfigMO(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

            void NsmDeleteBridgeXstpConfigMessageHandler(NsmDeleteBridgeXstpConfigMessage *pNsmDeleteBridgeXstpConfigMessage);
            ResourceId deleteBridgeConfigMO (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

            ResourceId updatePhyStpConfigMO (DceSynchronousLinearSeqContext *pSequencerContext);
            ResourceId updatePhyPortUidStep (DceSynchronousLinearSeqContext *pSequencerContext);

            ResourceId validateIntfXstpConfigStep(DceSynchronousLinearSeqContext *pContext);

        protected:
            ResourceId fillMo(XstpPortConfigManagedObject *pPortXstpMo, NsmPortStpConfigMessage *pMessage);
            ResourceId fillMo(XstpPortInstanceConfigManagedObject *pPortInstMo, NsmPortStpConfigMessage *pMessage);
            ResourceId fillMo(XstpPortVlanConfigManagedObject *pPortVlanMo, NsmPortStpConfigMessage *pMessage);

            string getIntfClassName () const;

        public:
            IntfXstpLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~IntfXstpLocalConfWorker ();

			ResourceId deleteVlanIntfXstp(UI32 vid, DceSynchronousLinearSeqContext * pPrismSynchronousLinearSequencerContext);

			ResourceId createPortXstp(PhyIntfLocalManagedObject *pMo);
			ResourceId deletePortXstp(PhyIntfLocalManagedObject *pMo, DceSynchronousLinearSeqContext *pSequencerContext);
    };
}
#endif                                            //INTFXSTPLOCALCONFWORKER_H
