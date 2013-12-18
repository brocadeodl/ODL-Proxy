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

#ifndef PRISMLINEARSEQUENCERCONTEXT_H
#define PRISMLINEARSEQUENCERCONTEXT_H

#include "Framework/Types/Types.h"

#include <vector>

#include <pthread.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;

namespace WaveNs
{

class PrismMessage;
class PrismElement;
class PrismLinearSequencerContext;
class PrismAsynchronousContext;
class WaveManagedObject;

typedef void (PrismElement::* PrismLinearSequencerStep) (PrismLinearSequencerContext *pPrismLinearSequencerContext);

class PrismLinearSequencerContext
{
    private :
        void advanceCurrentStep ();
        void executeCurrentStep ();

        void garbageCollect     ();

    protected :
    public :
                                             PrismLinearSequencerContext                   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                             PrismLinearSequencerContext                   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                             PrismLinearSequencerContext                   (const PrismLinearSequencerContext &prismLinearSequencerContext);
        virtual                             ~PrismLinearSequencerContext                   ();
                PrismLinearSequencerContext &operator =                                    (const PrismLinearSequencerContext &prismLinearSequencerContext);
                ResourceId                   getCompletionStatus                           ();
                void                         start                                         ();
                void                         executeNextStep                               (const ResourceId &currentStepStatus);
                void                         executeSuccessStep                            ();
                PrismMessage                *getPPrismMessage                              ();
                void                         setPPrismMessage                              (PrismMessage *pPrismMessage);
                PrismAsynchronousContext    *getPPrismAsynchronousContext                  ();
                PrismLinearSequencerContext &operator ++                                   ();
                PrismLinearSequencerContext &operator --                                   ();
                UI32                         getNumberOfCallbacksBeforeAdvancingToNextStep ();
                void                         holdAll                                       ();
                void                         unholdAll                                     ();
                bool                         getIsHoldAllRequested                         ();
                bool                         getIsTransactionStartedByMe                   () const;
                void                         setIsTransactionStartedByMe                   (const bool &isTransactionStartedByMe);

                UI32                         getNumberOfFailures                           () const;
                void                         setNumberOfFailures                           (const UI32 &numberOfFailures);
                void                         incrementNumberOfFailures                     ();
                void                         incrementNumberOfFailures                     (const UI32 &numberOfFailures);

                vector<WaveManagedObject *> *getPQueryResults                              () const;
                void                         setPQueryResults                              (vector<WaveManagedObject *> *pQueryResults);

        virtual void                         addManagedObjectsForGarbageCollection         (const vector<WaveManagedObject *> &managedObjectsForGarbageCollection);
        virtual void                         addManagedObjectForGarbageCollection          (WaveManagedObject *pWaveManagedObjectForGarbageCollection);

                bool                         getIsADelayedCommitTransaction                () const;
                void                         setIsADelayedCommitTransaction                (const bool &isADelayedCommitTransaction);

    // Now the data members

    private :
        PrismMessage                *m_pPrismMessage;
        PrismAsynchronousContext    *m_pPrismAsynchronousContext;
        PrismElement                *m_pPrismElement;
        PrismLinearSequencerStep    *m_pSteps;
        UI32                         m_numberOfSteps;
        UI32                         m_currentStep;
        UI32                         m_numberOfCallbacksBeforeAdvancingToNextStep;
        ResourceId                   m_completionStatus;
        bool                         m_isHoldAllRequested;
        bool                         m_isTransactionStartedByMe;

        UI32                         m_numberOfFailures;

        vector<WaveManagedObject *> *m_pQueryResults;
        vector<WaveManagedObject *>  m_managedObjectsForGarbageCollection;

        struct timespec              m_ts1;
        struct timespec              m_ts2;
        struct timespec              m_tsr1;
        struct timespec              m_tsr2;
        SI32                         m_returnValue;
        SI32                         m_returnValueR;
        UI32                         m_operationCode;
    
        bool                         m_isADelayedCommitTransaction;

        clockid_t                    m_clockId;

    protected :
    public :
};

}

#endif //PRISMLINEARSEQUENCERCONTEXT_H
