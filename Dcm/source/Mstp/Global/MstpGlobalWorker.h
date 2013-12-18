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

#ifndef MSTPGLOBALWORKER_H
#define MSTPGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalObjectManager;
    class MstpGlobalUpdateBridgeConfigMessage;
    class MstpGlobalDeleteVlanBridgeConfigMessage;
    class MstpGlobalUpdateVlanBridgeConfigMessage;
    class MstpGlobalUpdateMstpInstanceConfigMessage;
    class VlanIntfManagedObject;
    class XstpBridgeConfigManagedObject;
    class DceLinearSeqContext;
    class MstiUpdateSequencerContext;


    class MstpGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void MstpGlobalDeleteVlanBridgeConfigMessageHandler( MstpGlobalDeleteVlanBridgeConfigMessage *pMstpGlobalDeleteVlanBridgeConfigMessage);
            void MstpGlobalUpdateBridgeConfigMessageHandler( MstpGlobalUpdateBridgeConfigMessage *pMstpGlobalUpdateBridgeConfigMessage);
            void MstpGlobalUpdateMstpInstanceConfigMessageHandler( MstpGlobalUpdateMstpInstanceConfigMessage *pMstpGlobalUpdateMstpInstanceConfigMessage);
            void MstpGlobalUpdateVlanBridgeConfigMessageHandler( MstpGlobalUpdateVlanBridgeConfigMessage *pMstpGlobalUpdateVlanBridgeConfigMessage);

            // Process the bridge configure update
            void validateBridgeConfigStep(DceLinearSeqContext *pSequencerContext);
            void updateGlobalBridgeConfigStep (DceLinearSeqContext *pSequencerContext);
            void deleteGlobalBridgeConfigNsmStep (DceLinearSeqContext *pSequencerContext);
            void releaseLogicalPortIdsStep (DceLinearSeqContext *pSequencerContext);
            void updateGlobalBridgeConfigMOStep (DceLinearSeqContext *pSequencerContext);

            void validateMstpInstanceConfigStep (MstiUpdateSequencerContext *pSequencerContext);
            void notifyNsmOnMstpInstanceUpdateStep (MstiUpdateSequencerContext *pSequencerContext);
            void updateGlobalMstpInstanceConfigStep (MstiUpdateSequencerContext *pSequencerContext);
            void updateGlobalMstpInstanceConfigMOStep (MstiUpdateSequencerContext *pSequencerContext);


            ResourceId updateBridgeConfigMO (MstpGlobalUpdateBridgeConfigMessage *pGlobalMessage, DceLinearSeqContext *pSequencerContext);
            ResourceId createBridgeConfigMO (MstpGlobalUpdateBridgeConfigMessage *pGlobalMessage, DceLinearSeqContext *pSequencerContext);
            ResourceId deleteBridgeConfigMO (int mode, DceLinearSeqContext *pSequencerContext);

            // Process the vlan bridge configure update
            void validateVlanBridgeConfigStep (DceLinearSeqContext *pSequencerContext);
            void updateGlobalVlanBridgeConfigStep (DceLinearSeqContext *pSequencerContext);
            void updateVlanBridgeConfigMOStep (DceLinearSeqContext *pSequencerContext);
            ResourceId updateVlanBridgeConfigMO (MstpGlobalUpdateVlanBridgeConfigMessage *pMessage, DceLinearSeqContext *pSequencerContext);

            // delete Vlan bridge
            ResourceId deleteVlanBridgeConfigMO(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId deleteVlanFromInstanceBaseConfigMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            void sendToCluster (PrismMessage *pMessage, DceLinearSeqContext *pSequencerContext, vector<LocationId> *pLocations = NULL);
            void sendToClusterCallback (WaveSendToClusterContext  *pClusterContext);

        protected:
        public:
            MstpGlobalWorker (MstpGlobalObjectManager *pMstpGlobalObjectManager);
            virtual    ~MstpGlobalWorker ();

    };

}
#endif                                            //MSTPGLOBALWORKER_H
