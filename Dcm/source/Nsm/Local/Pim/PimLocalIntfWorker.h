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
 *   Author : Rahul Jain                                                   *
 **************************************************************************/

#ifndef _PIMLOCALINTFWORKER_H_
#define _PIMLOCALINTFWORKER_H_

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/Pim/PimIntfCfgMessage.h"
#include "Nsm/Local/Pim/PimIntfCfgLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class PimIntfCfgMessage;

    class PimLocalIntfWorker: public WaveWorker
    {
        /* Function declarations */
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void  NsmLocalPimIntfCfgMessageHandler(PimIntfCfgMessage *pPimIntfLocalMsg);
            
            /* Per MO, client and DB related function declarations */
            ResourceId validatePimIntfCfgStep(DceSynchronousLinearSeqContext *pSeqCtx);
            ResourceId sendPimPhyIntfCfgStep(DceSynchronousLinearSeqContext *pSeqCtx);
            ResourceId updPimPhyIntfCfgDBStep(DceSynchronousLinearSeqContext *pSeqCtx);
           
			ResourceId updatePIMNoRtrPimConfigSessionMO(PrismSynchronousLinearSequencerContext *pSeqCtx);
	 
            /* Utility function declaraions */
            void pimIntfFillMO(PimIntfCfgLocalManagedObject *pMo, 
                NsmL3ReadyManagedObject *pL3RdyMo, PimIntfCfgMessage *pMsg, 
                bool enable_val);
            NsmL3ReadyManagedObject* pimGetL3RdyMo(
                PrismSynchronousLinearSequencerContext *pSeqCtx, 
                PimIntfCfgMessage *pMsg);
            void pimIntfCfgMarkMoForUpd(PimIntfCfgLocalManagedObject *pMo, 
                NsmL3ReadyManagedObject *pL3RdyMo, PimIntfCfgMessage *pMsg);
        protected:
        public:
            PimLocalIntfWorker (NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual ~PimLocalIntfWorker ();
            static PimIntfCfgLocalManagedObject* getPimIntfCfgLclMo(const 
                NsmL3ReadyManagedObject *pL3RdyMo);

        /* Now the data members */
        private:
        protected:
        public:
    };
}
#endif /* _PIMLOCALINTFWORKER_H_ */
