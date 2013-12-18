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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKFAILOVERWORKER_H
#define PRISMFRAMEWORKFAILOVERWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

namespace WaveNs
{

class PrismFrameworkFailoverWorkerContext;
class PrismFrameworkFailoverWorkerSequencerContext;
class PrismFailoverAgentContext;

class PrismFrameworkFailoverWorker : public WaveWorker
{
    private :
    protected :
    public :
                      PrismFrameworkFailoverWorker            (WaveObjectManager *pWaveObjectManager);
        virtual      ~PrismFrameworkFailoverWorker            ();

                void  executeFailover                         (PrismFrameworkFailoverWorkerContext *pPrismFrameworkFailoverWorkerContext);
                void  executeFailoverSelectFailoverAgentStep  (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext);
                void  executeFailoverRunFailoverAgentStep     (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext);
                void  executeFailoverRunFailoverAgentStepCallback (PrismFailoverAgentContext *pPrismFailoverAgentContext);
                void  executeFailoverDestroyFailoverAgentStep (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext);
                void  executeFailoverCompleteEventStep        (PrismFrameworkFailoverWorkerSequencerContext *pPrismFrameworkFailoverWorkerSequencerContext);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // PRISMFRAMEWORKFAILOVERWORKER_H
