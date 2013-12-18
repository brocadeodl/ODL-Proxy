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
 *   Copyright (C) 2011-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Rahul Jain                                                   *
 **************************************************************************/

#ifndef _PIMINTFCFGGLOBALWORKER_H_
#define _PIMINTFCFGGLOBALWORKER_H_

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Global/Pim/PimIntfCfgGlobalMsg.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmGlobalObjectManager;
    class PimIntfCfgGlobalMsg;

    class PimIntfCfgGlobalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const 
                string &managedClassName);
            void PimIntfCfgGlobalMsgHandler(PimIntfCfgGlobalMsg 
                *pPimIntfCfgGlobalMsg);
            ResourceId sendPimCfgGlbStep(PrismSynchronousLinearSequencerContext
                *pSeqCtx);
            void ClusterGetFirstFailureLocation(WaveSendToClusterContext *ctx_p,
                LocationId &locationid);
        protected:
        public:
            PimIntfCfgGlobalWorker (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~PimIntfCfgGlobalWorker ();

// Now the data members
        private:
        protected:
        public:
    };

}
#endif /* _PIMINTFCFGGLOBALWORKER_H_ */
