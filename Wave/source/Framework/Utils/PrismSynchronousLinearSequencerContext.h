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
 *   Author Vidyasagara Reddy Guntaka                                      *
 ***************************************************************************/

#ifndef PRISMSYNCHRONOUSLINEARSEQUENCERCONTEXT_H
#define PRISMSYNCHRONOUSLINEARSEQUENCERCONTEXT_H

#include "Framework/Types/Types.h"
#include "Framework/MultiThreading/PrismPosixThread.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class PrismMessage;
class PrismElement;
class PrismSynchronousLinearSequencerContext;
class PrismAsynchronousContext;
class WaveManagedObject;

typedef ResourceId (PrismElement::* PrismSynchronousLinearSequencerStep) (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

class PrismSynchronousLinearSequencerContext
{
    private :
        void       advanceCurrentStep ();
        ResourceId executeCurrentStep ();
        void       garbageCollect     ();

    protected :
    public :
                                                        PrismSynchronousLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                                        PrismSynchronousLinearSequencerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                                        PrismSynchronousLinearSequencerContext (const PrismSynchronousLinearSequencerContext &prismSynchronousLinearSequencerContext);
        virtual                                        ~PrismSynchronousLinearSequencerContext ();
                PrismSynchronousLinearSequencerContext &operator =                             (const PrismSynchronousLinearSequencerContext &prismSynchronousLinearSequencerContext);
                ResourceId                              getCompletionStatus                    ();
                ResourceId                              execute                                ();
                PrismMessage                           *getPPrismMessage                       ();
                void                                    setPPrismMessage                       (PrismMessage *pPrismMessage);
                PrismAsynchronousContext               *getPPrismAsynchronousContext           ();
                bool                                    getIsTransactionStartedByMe            () const;
                void                                    setIsTransactionStartedByMe            (const bool &isTransactionStartedByMe);

                void                                    addManagedObjectsForGarbageCollection  (const vector<WaveManagedObject *> &managedObjectsForGarbageCollection);
                void                                    addManagedObjectForGarbageCollection   (WaveManagedObject *pWaveManagedObjectForGarbageCollection);

                bool                                    getIsADelayedCommitTransaction         () const;
                void                                    setIsADelayedCommitTransaction         (const bool &isADelayedCommitTransaction);

    // Now the data  members

    private :
        PrismMessage                        *m_pPrismMessage;
        PrismAsynchronousContext            *m_pPrismAsynchronousContext;
        PrismElement                        *m_pPrismElement;
        PrismSynchronousLinearSequencerStep *m_pSteps;
        UI32                                 m_numberOfSteps;
        UI32                                 m_currentStep;
        ResourceId                           m_completionStatus;
        bool                                 m_isTransactionStartedByMe;

        vector<WaveManagedObject *>          m_managedObjectsForGarbageCollection;

        bool                                 m_isADelayedCommitTransaction;

        clockid_t                            m_clockId;

    protected :
    public:
};

}

#endif // PRISMSYNCHRONOUSLINEARSEQUENCERCONTEXT_H
