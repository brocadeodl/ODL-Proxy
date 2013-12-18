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
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

#include "Fcoe/Common/FcoeDbTransactionLinearSequencerContext.h"

using namespace std;

namespace DcmNs
{
    
    FcoeDbTransactionLinearSequencerContext::FcoeDbTransactionLinearSequencerContext(PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
        :   PrismLinearSequencerContext(pPrismMessage, pPrismElement, pSteps, numberOfSteps),
            m_TransactionStartedStatus (false)
    {
    }

    FcoeDbTransactionLinearSequencerContext::FcoeDbTransactionLinearSequencerContext(PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
        :   PrismLinearSequencerContext(pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
            m_TransactionStartedStatus (false)
    {
    }

    FcoeDbTransactionLinearSequencerContext::FcoeDbTransactionLinearSequencerContext(const PrismLinearSequencerContext &prismLinearSequencerContext)
        :   PrismLinearSequencerContext(prismLinearSequencerContext),
            m_TransactionStartedStatus (false)
    {
    }

    FcoeDbTransactionLinearSequencerContext::~FcoeDbTransactionLinearSequencerContext()
    {
        UI32    i;
        for (i = 0; i < m_memMoReleaseVector.size(); i++) {
            delete m_memMoReleaseVector[i];
        }

        for (i = 0; i < m_memPResultsReleaseVector.size(); i++) {
            delete m_memPResultsReleaseVector[i];
        }
    }
    
    bool
    FcoeDbTransactionLinearSequencerContext::getTransactionStartedStatus()
    {
        return m_TransactionStartedStatus;
    }

    void
    FcoeDbTransactionLinearSequencerContext::setTransactionStartedStatus(const bool &status)
    {
        m_TransactionStartedStatus = status;
    }

    void
    FcoeDbTransactionLinearSequencerContext::addMOPointerToReleaseVector (WaveManagedObject *ptr_p)
    {
        if (ptr_p) {
            m_memMoReleaseVector.push_back(ptr_p);
        }
    }
    
    void    
    FcoeDbTransactionLinearSequencerContext::addMOVectorToReleaseVector (vector<WaveManagedObject *> *pResults, const bool addVectorPointer)
    {
        UI32 i;
        if (pResults == NULL) {
            return;
        }

        for (i = 0; i < pResults->size(); i++) {
            m_memMoReleaseVector.push_back((*pResults)[i]);
        }

        if (addVectorPointer == true) {
            m_memPResultsReleaseVector.push_back(pResults);
        }
    }
}

