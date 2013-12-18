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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef PERSISTENTWITHDEFAULTFORHABOOTAGENT_H
#define PERSISTENTWITHDEFAULTFORHABOOTAGENT_H

#include "Framework/Boot/PrismBootAgent.h"

namespace WaveNs
{

class PersistentWithDefaultForHABootAgent : public PrismBootAgent
{
    private :
        virtual bool           isAPersistentBoot ();
        virtual WaveBootReason getReason         () const;

    protected :
    public :
                            PersistentWithDefaultForHABootAgent                        (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual            ~PersistentWithDefaultForHABootAgent                        ();
        virtual ResourceId  execute                                                    (const WaveBootPhase &waveBootPhase);

                ResourceId  createFrameworkConfigurationFromPersistedConfigurationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId  connectToKnownLocationsStep                                (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        virtual bool        isToBeExcludedFromInstallDuringPrePhase                    (const PrismServiceId &prismServiceId);
        virtual bool        isToBeExcludedFromInstall                                  (const PrismServiceId &prismServiceId);
        virtual bool        isToBeExcludedFromInitializeDuringPrePhase                 (const PrismServiceId &prismServiceId);
        virtual bool        isToBeExcludedFromInitializePhase                          (const PrismServiceId &prismServiceId);
        virtual bool        isToBeExcludedForEnableAndBoot                             (const PrismServiceId& prismServiceId);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // PERSISTENTWITHDEFAULTFORHABOOTAGENT_H
