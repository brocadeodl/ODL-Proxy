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

#include "Fcoe/Common/FcoeCommonWorker.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"

namespace DcmNs
{
    FcoeCommonWorker::FcoeCommonWorker (WaveObjectManager *pWaveObjectManager)
        : WaveWorker(pWaveObjectManager)
    {
    }

    FcoeCommonWorker::~FcoeCommonWorker()
    {
    }

    PrismMessage *
    FcoeCommonWorker::createMessageInstance(const UI32 &operationCode)
    {
        trace (TRACE_LEVEL_ERROR, "FcoeCommonWorker::createMessageInstance NOT IMPLEMENTED");
        assert(0);
        return NULL;
    }

    WaveManagedObject *
    FcoeCommonWorker::createManagedObjectInstance(const string &managedClassName)
    {
        trace (TRACE_LEVEL_ERROR, "FcoeCommonWorker::createManagedObjectInstance NOT IMPLEMENTED");
        assert(0);
        return NULL;
    }

    void
    FcoeCommonWorker::FcoeLinearSequencerCommitTransactionStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeCommonWorker::prismLinearSequencerCommitTransactionStep(seq_ctx_p);
    }

    void
    FcoeCommonWorker::FcoeLinearSequencerSucceededStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeCommonWorker::prismLinearSequencerSucceededStep(seq_ctx_p);
    }

    void
    FcoeCommonWorker::FcoeLinearSequencerStartTransactionStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        seq_ctx_p->holdAll();
        seq_ctx_p->setTransactionStartedStatus(true);
        FcoeCommonWorker::prismLinearSequencerStartTransactionStep(seq_ctx_p);
    }

    void
    FcoeCommonWorker::FcoeLinearSequencerFailedStep (FcoeLinearSequencerContext *seq_ctx_p)
    {
        FcoeCommonWorker::prismLinearSequencerFailedStep(seq_ctx_p);
    }
}   
