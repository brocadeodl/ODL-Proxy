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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMINITIALIZEWORKERSTARTPRISMSERVICESCONTEXT_H
#define PRISMINITIALIZEWORKERSTARTPRISMSERVICESCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"

namespace WaveNs
{

class PrismBootAgent;

class PrismFrameworkInitializeWorkerStartServicesContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                        PrismFrameworkInitializeWorkerStartServicesContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                       ~PrismFrameworkInitializeWorkerStartServicesContext ();
        void            setPrismBootMode                                   (PrismBootMode prismBootMode);
        PrismBootMode   getPrismBootMode                                   () const;
        void            setWaveBootPhase                                   (const WaveBootPhase &waveBootPhase);
        WaveBootPhase   getWaveBootPhase                                   () const;
        void            setPPrismBootAgent                                 (PrismBootAgent *pPrismBootAgent);
        PrismBootAgent *getPPrismBootAgent                                 ();
        bool            isSchemaChangedOnFwdl                              () const;
        void            setSchemaChangedOnFwdl                             (bool hasChanged);
        bool            getIsSchemaUpgradedSuccessfully                    () const;
        void            setIsSchemaUpgradedSuccessfully                    (const bool &isSchemaUpgradedSuccessfully);

    // Now the data members

    private :
        PrismBootMode   m_prismBootMode;
        WaveBootPhase   m_waveBootPhase;
        PrismBootAgent *m_pPrismBootAgent;
        bool            m_isSchemaChangedOnFwdl;
        bool            m_isSchemaUpgradedSuccessfully;

    protected :
    public :
};

}

#endif // PRISMINITIALIZEWORKERSTARTPRISMSERVICESCONTEXT_H
