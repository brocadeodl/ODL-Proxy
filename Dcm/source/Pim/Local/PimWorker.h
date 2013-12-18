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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMWORKER_H
#define PIMWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Pim/Local/MultRtrCfgLocalManagedObject.h"
#include "Pim/Local/StaticRpLocalManagedObject.h"
#include "Pim/Local/RpCandidateLocalManagedObject.h"
#include "Pim/Local/RpCandidatePrefixLocalManagedObject.h"
#include "Pim/Local/BsrCandidateLocalManagedObject.h"
#include "Pim/Local/PimRtrCfgLocalManagedObject.h"
#include "ClientInterface/Pim/PimRtrCfgClientMsg.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimLocalObjectManager;
    class PimRtrCfgLocalManagedObject;
    class PimBsrCandidateCfgMessage;
    class PimRpCandidateCfgMessage;
	class PimRpCandidatePrefixCfgMessage;
    class PimStaticRpCfgMessage;
    class PimRtrInstMessage;
    class PimMultRtrCfgMessage;

    class PimWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance
					(const string &managedClassName);
            void PimBsrCandidateCfgMessageHandler( PimBsrCandidateCfgMessage 
					*pPimBsrCandidateCfgMessage);
            void PimMultRtrCfgMessageHandler( PimMultRtrCfgMessage 
					*pPimMultRtrCfgMessage);
            void PimRpCandidateCfgMessageHandler( PimRpCandidateCfgMessage 
					*pPimRpCandidateCfgMessage);
			void PimRpCandidatePrefixCfgMessageHandler( 
					PimRpCandidatePrefixCfgMessage 
					*pPimRpCandidatePrefixCfgMessage);
            void PimRtrInstMessageHandler( PimRtrInstMessage 
					*pPimRtrInstMessage);
            void PimStaticRpCfgMessageHandler( PimStaticRpCfgMessage 
					*pPimStaticRpCfgMessage);
            
            /* Per MO, client and DB related function declarations */
			ResourceId validationStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId sendPimRtrCfgStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId updPimRtrCfgDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId sendPimBsrCandidateStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId updatePimBsrCandidateDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId sendPimIpMultCfgStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId updPimIpMultCfgDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCxt);
            ResourceId sendPimStaticRpCfgStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);
            ResourceId updPimStaticRpCfgDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);
            ResourceId sendPimCandRpCfgStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);
            ResourceId updPimCandRpCfgDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);
			ResourceId sendPimCandRpPrefixCfgStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);
            ResourceId updPimCandRpPrefixCfgDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);
			ResourceId validatePimCandRpPrefixCfgDbStep(
					PrismSynchronousLinearSequencerContext *pSeqCtx);

            /* Forward declarations of utility functions */
            PimRtrCfgLocalManagedObject* getPimRtrCfgMo(
                PrismSynchronousLinearSequencerContext  *pContext, string MOAttr);
            void copyPimDfltVals(PimRtrInstMessage *pMsg, 
                PimRtrCfgClientMsg *pClientMsg);
            void pimIpMultMOAdd(MultRtrCfgLocalManagedObject *pMo, 
                PimMultRtrCfgMessage *pMsg);
            ResourceId pimIpMultMODel(MultRtrCfgLocalManagedObject *pMo, 
                PimMultRtrCfgMessage *pMsg);
            void pimStaticRpMOAdd(StaticRpLocalManagedObject *pMo, 
                PimRtrCfgLocalManagedObject *pPimRtrMo, 
                PimStaticRpCfgMessage *pMsg, int addInVector);
            void pimStaticRpMODel(StaticRpLocalManagedObject *pMo, 
                PimRtrCfgLocalManagedObject *pPimRtrMo, 
                PimStaticRpCfgMessage *pMsg);
            void pimCandRpMOAdd(RpCandidateLocalManagedObject *pMo, 
                PimRtrCfgLocalManagedObject *pPimRtrMo, 
                PimRpCandidateCfgMessage *pMsg, int addInVector);
            void pimCandRpMODel(RpCandidateLocalManagedObject *pMo, 
                PimRtrCfgLocalManagedObject *pPimRtrMo, 
                PimRpCandidateCfgMessage *pMsg);
			void pimCandRpPrefixMOAdd(RpCandidatePrefixLocalManagedObject *pMo,
                PimRtrCfgLocalManagedObject *pPimRtrMo,
                PimRpCandidatePrefixCfgMessage *pMsg, int addInVector);
            void pimCandRpPrefixMODel(RpCandidatePrefixLocalManagedObject *pMo,
                PimRtrCfgLocalManagedObject *pPimRtrMo,
                PimRpCandidatePrefixCfgMessage *pMsg);
            ResourceId pimRtrCfgFillMO(PimRtrCfgLocalManagedObject *pMo,
                PimRtrInstMessage *pMsg, bool enable_val);
            ResourceId pimRtrCfgMarkMoForUpd(PimRtrCfgLocalManagedObject *pMo,
                PimRtrInstMessage *pMsg);
            void pimBsrCandMOAdd(BsrCandidateLocalManagedObject *pMo, 
                PimRtrCfgLocalManagedObject *pPimRtrMo, 
                PimBsrCandidateCfgMessage *pMsg, int addInVector);
            void pimBsrCandMODel(BsrCandidateLocalManagedObject *pMo, 
                PimRtrCfgLocalManagedObject *pPimRtrMo, 
                PimBsrCandidateCfgMessage *pMsg);
            
        protected:
        public:
            PimWorker ( PimLocalObjectManager *pPimLocalObjectManager);
            virtual    ~PimWorker ();
            int32_t pimExtractSlot(string ifName, UI32 ifType);

// Now the data members
        private:
        protected:
        public:
    };
}
#endif                                            //PIMWORKER_H
