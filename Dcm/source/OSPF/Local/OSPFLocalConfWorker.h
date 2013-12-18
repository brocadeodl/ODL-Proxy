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

#ifndef OSPFLOCALCONFWORKER_H
#define OSPFLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "OSPF/Local/OSPFAreaLocalMessage.h"
#include "OSPF/Local/OSPFVirtualLinkLocalMessage.h"
#include "ClientInterface/OSPF/OSPFAreaMessage.h"
#include "OSPF/Local/OSPFAreaRangeLocalMessage.h"
#include "OSPF/Local/OSPFLocalShowMessage.h"
#include "OSPF/Local/OSPFLocalClearMessage.h"
#include "ClientInterface/OSPF/OSPFAreaRangeMessage.h"
#include "ClientInterface/OSPF/OSPFShowMessage.h"
#include "ClientInterface/OSPF/OSPFClearMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
    class OSPFRouterConfigLocalMessage;
    class OSPFPermitDenyRedistributeLocalMessage;
	class OSPFConfigLocalManagedObject;
	class OSPFPermitDenyRedistributeLocalManagedObject;
    class OSPFVirtualLinkLocalManagedObject;

    class OSPFLocalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void OSPFRouterConfigLocalMessageHandler( OSPFRouterConfigLocalMessage *pOSPFRouterConfigLocalMessage);
		    void OSPFPermitDenyRedistributeLocalMessageHandler (OSPFPermitDenyRedistributeLocalMessage *pOSPFPermitDenyLocalMessage);

			ResourceId ospfRouterConfigStep
				(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId deleteOSPFIntfConfig
				(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId ospfRouterConfigMOStep
				(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

		    ResourceId OSPFPermitDenyRedistributeStep
			    (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId OSPFPermitDenyRedistributeMOStep
			    (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

            void OSPFVirtualLinkLocalMessageHandler(OSPFVirtualLinkLocalMessage *pOSPFVirtualLinkLocalMessage);
            OSPFVirtualLinkLocalManagedObject *OSPFGetVirtualLinkLocalManagedObject(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId OSPFUpdateVirtualLinkStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId OSPFUpdateVirtualLinkMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            OSPFVirtualLinkLocalManagedObject *OSPFGetVirtualLinkMO(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            void ospfUpdateLocalMessage(OSPFVirtualLinkLocalMessage*, OSPFAreaMessage*);
            void OSPFLocalShowMessageHandler(OSPFLocalShowMessage * OSPFLocalShowMessage);        
            void OSPFLocalClearMessageHandler(OSPFLocalClearMessage * OSPFLocalClearMessage);        
            void OSPFAreaLocalMessageHandler(OSPFAreaLocalMessage *pOSPFAreaLocalMessage);
			ResourceId OSPFUpdateAreaStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId OSPFUpdateAreaMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
            string OSPFGetOtherAreaFormatString(string area);

            void OSPFAreaRangeLocalMessageHandler(OSPFAreaRangeLocalMessage *pOSPFAreaRangeLocalMessage);
			ResourceId OSPFUpdateAreaRangeStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId OSPFUpdateAreaRangeMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);

			string  getManagedObjectClassNameFromId(const OSPFManagedObjectId &managedObjectId);
			vector<WaveManagedObject *> *queryManagedObject(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			OSPFConfigLocalManagedObject *getOSPFConfigManagedObject(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
                        OSPFConfigLocalManagedObject *getOSPFConfigManagedObject(string vrf);
			void deleteManagedObject(WaveManagedObject *pMO, OSPFRouterConfigLocalMessage *pMsg,
					DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			void deletePermitDenyLocalManagedObject(OSPFPermitDenyRedistributeLocalManagedObject *pMO, OSPFPermitDenyRedistributeLocalMessage *pMsg,DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			OSPFPermitDenyRedistributeLocalManagedObject *createPermitDenyManagedObject (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext,
																	OSPFPermitDenyRedistributeLocalMessage *pMsg);
		    void updateOSPFPermitDenyManagedObject(OSPFPermitDenyRedistributeLocalManagedObject *pOSPFPermitDenyLocalManagedObject);
            ResourceId validateKeyLength(OSPFVirtualLinkLocalMessage *);
			WaveManagedObject *createLocalManagedObjectFromId(OSPFManagedObjectId managedObjectId,
					DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);
			ResourceId validateVRF(OSPFRouterConfigLocalMessage *);
            ResourceId sendShowToOSPFDaemon (PrismSynchronousLinearSequencerContext *ctx_p);
            void OSPFClearCmdClusterCallback(WaveSendToClusterContext *ctx_p);
            ResourceId sendOSPFClearCommandToOSPFDaemon(DceSynchronousLinearSeqContext *seq_ctx_p);
            void sendOSPFClearCommandToCluster(PrismLinearSequencerContext *seq_ctx_p);

            void updateMoForAllVrf(WaveManagedObject *pMO, OSPFRouterConfigLocalMessage *pMsg,DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext);


        protected:
        public:
            OSPFLocalConfWorker ( OSPFLocalObjectManager *pOSPFLocalObjectManager);
            virtual    ~OSPFLocalConfWorker ();
			void updateManagedObjectFromLocalMessage(WaveManagedObject *pMO, OSPFRouterConfigLocalMessage *pMsg);


// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //OSPFLOCALCONFWORKER_H
