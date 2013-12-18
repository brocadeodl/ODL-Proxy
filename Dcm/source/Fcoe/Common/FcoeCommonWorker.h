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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#ifndef FCOE_COMMON_WORKER_H
#define FCOE_COMMON_WORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

#include "Fcoe/Common/FcoeLinearSequencerContext.h"

using namespace WaveNs;

namespace DcmNs
{
    class FcoeCommonWorker : public WaveNs::WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

        public:
            FcoeCommonWorker(WaveObjectManager *pWaveObjectManager);
            virtual ~FcoeCommonWorker();

            virtual void FcoeLinearSequencerCommitTransactionStep (FcoeLinearSequencerContext *seq_ctx_p);
            virtual void FcoeLinearSequencerSucceededStep (FcoeLinearSequencerContext *seq_ctx_p);
            virtual void FcoeLinearSequencerStartTransactionStep (FcoeLinearSequencerContext *seq_ctx_p);
            virtual void FcoeLinearSequencerFailedStep (FcoeLinearSequencerContext *seq_ctx_p);

        protected:
        
    };
}
#endif
