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

#ifndef MSTPLOCALWORKER_H
#define MSTPLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpLocalObjectManager;
    class MstpCreateBridgeConfigMessage;
    //class MstpDeleteBridgeConfigMessage;
    class MstpUpdateBridgeConfigMessage;
    class MstpUpdateVlanConfigMessage;
    class MstpCreateVlanBridgeConfigMessage;
    class MstpDeleteVlanBridgeConfigMessage;
    class MstpUpdateVlanBridgeConfigMessage;
    class MstpUpdateMstpInstanceConfigMessage;
    class MstpLocalShowSTPMessage;
    class MstpLocalClearMessage;

    class MstpLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void MstpCreateBridgeConfigMessageHandler( MstpCreateBridgeConfigMessage *pMstpCreateBridgeConfigMessage);
            void MstpCreateVlanBridgeConfigMessageHandler( MstpCreateVlanBridgeConfigMessage *pMstpCreateVlanBridgeConfigMessage);
      //      void MstpDeleteBridgeConfigMessageHandler( MstpDeleteBridgeConfigMessage *pMstpDeleteBridgeConfigMessage);
            void MstpDeleteVlanBridgeConfigMessageHandler( MstpDeleteVlanBridgeConfigMessage *pMstpDeleteVlanBridgeConfigMessage);
            void MstpUpdateBridgeConfigMessageHandler( MstpUpdateBridgeConfigMessage *pMstpUpdateBridgeConfigMessage);
            void MstpUpdateMstpInstanceConfigMessageHandler( MstpUpdateMstpInstanceConfigMessage *pMstpUpdateMstpInstanceConfigMessage);
            void MstpUpdateVlanBridgeConfigMessageHandler( MstpUpdateVlanBridgeConfigMessage *pMstpUpdateVlanBridgeConfigMessage);
	   		ResourceId updateBridgeConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId updateVlanBridgeConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	    	ResourceId updateMstpInstanceConfigStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	   		ResourceId updateMstpInstanceConfigMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateLogicalPortIdConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId allocateLogicalPortIds(vector<mstp_msg_port_id_info> &portIdList, int brMode);

			void  MstpUpdateVlanConfigMessageHandler( MstpUpdateVlanConfigMessage *pMstpUpdateBridgeConfigMessage);
			ResourceId updateVlanConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);



			/* Bpdu-drop cmd */
			void MstpLocalShowSTPMessageHandler(MstpLocalShowSTPMessage* pMstpLocalShowSTPMessage);
			void sendMstpShowSTPCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);
			ResourceId sendMstpShowSTPCommandToDaemon(PrismLinearSequencerContext *seq_ctx_p);

            // Clear commands..
            void MstpLocalClearMessageHandler (MstpLocalClearMessage *pMessage);
            void processClearCommandStep (PrismLinearSequencerContext *pSequencerContext);

			void MstpLocalSendToClusterCallback (WaveSendToClusterContext *ctx_p);

        protected:
        public:
            MstpLocalWorker ( MstpLocalObjectManager *pMstpLocalObjectManager);
            virtual    ~MstpLocalWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //MSTPLOCALWORKER_H
