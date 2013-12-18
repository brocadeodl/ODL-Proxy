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
#ifndef FCOE_SEQUENCER_CONTEXT_H
#define FCOE_SEQUENCER_CONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{
    class FcoeLinearSequencerContext : public WaveNs::PrismLinearSequencerContext
    {
        private:
        protected:
        public:
                    FcoeLinearSequencerContext(PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                    FcoeLinearSequencerContext(PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                    FcoeLinearSequencerContext(const PrismLinearSequencerContext &prismLinearSequencerContext);
                    ~FcoeLinearSequencerContext();
            bool    getTransactionStartedStatus();
            void    setTransactionStartedStatus(const bool &status);
            void    addMOPointerToReleaseVector(WaveManagedObject *ptr_p);
            void    addMOVectorToReleaseVector(vector<WaveManagedObject *> *moVec,
                                               const bool addVectorPointer = true);

        private:
            bool                                    m_TransactionStartedStatus;
            vector<WaveManagedObject *>             m_memMoReleaseVector;
            vector < vector <WaveManagedObject *> *>   m_memPResultsReleaseVector;
        
        public:
        private:
    };
}

#endif
