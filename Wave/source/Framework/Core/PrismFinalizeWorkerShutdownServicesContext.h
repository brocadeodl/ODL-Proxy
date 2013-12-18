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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Geetha Aparna                                                *
 ***************************************************************************/

#ifndef PRISMFINALIZEWORKERSHUTDOWNSERVICESCONTEXT_H
#define PRISMFINALIZEWORKERSHUTDOWNSERVICESCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Shutdown/PrismFinalizeWorker.h"

namespace WaveNs
{

class PrismFinalizeWorkerShutdownServicesContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                        PrismFinalizeWorkerShutdownServicesContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                       ~PrismFinalizeWorkerShutdownServicesContext ();
        void                setPrismShutdownMode                               (const PrismShutdownMode &prismShutdownMode);
        PrismShutdownMode   getPrismShutdownMode                               () const;

    // Now the data members

    private :
        PrismShutdownMode   m_prismShutdownMode;

    protected :
    public :
};

}

#endif // PRISMFINALIZEWORKERSHUTDOWNSERVICESCONTEXT_H
