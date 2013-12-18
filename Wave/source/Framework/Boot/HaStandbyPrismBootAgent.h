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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef STANDBYPRISMBOOTAGENT_H
#define STANDBYPRISMBOOTAGENT_H

#include "Framework/Boot/PrismBootAgent.h"

namespace WaveNs
{

class HaStandbyPrismBootAgent : public PrismBootAgent
{
    private :
        virtual bool           isAPersistentBoot                            ();
        virtual bool           isToBeExcludedForEnableAndBoot               (const PrismServiceId& prismServiceId);
        virtual bool           isToBeExcludedFromInstallDuringPrePhase      (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInstall                    (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInitializeDuringPrePhase   (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInitializePhase            (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromCurrentBootPhase           (const PrismServiceId &prismServiceId);
        virtual WaveBootReason getReason                                    () const;

    protected :
        ResourceId haInstallPrismServicesDuringPrePhaseStep       (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId haInstallPrismServicesStep                     (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId haBootPrismServicesDuringPrePhaseStep          (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId haBootLocalPrismServicesStep                   (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId haBootGlobalPrismServicesStep                  (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId haBootPrismServicesStep                        (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId resetLocationRole                              (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

    public :
                            HaStandbyPrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual            ~HaStandbyPrismBootAgent ();
        virtual ResourceId  execute                              (const WaveBootPhase &waveBootPhase);

    // now the data members

    private :
                            WaveBootPhase m_waveBootPhase;
    protected :
    public :
};

}

#endif // STANDBYPRISMBOOTAGENT_H
