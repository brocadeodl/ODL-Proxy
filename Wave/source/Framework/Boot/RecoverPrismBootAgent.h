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

#ifndef RECOVERPRISMBOOTAGENT_H
#define RECOVERPRISMBOOTAGENT_H

#include "Framework/Boot/FirstTimePrismBootAgent.h"
#include "Framework/Database/DatabaseObjectManagerShutdownMessage.h"

namespace WaveNs
{

class RecoverPrismBootAgent : public FirstTimePrismBootAgent
{
    private :

        virtual bool           isToBeExcludedFromInitializeDuringPrePhase   (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedForEnableAndBoot               (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInstallDuringPrePhase      (const PrismServiceId &prismServiceId);

    protected :
    public :
                 RecoverPrismBootAgent   (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual ~RecoverPrismBootAgent   ();

        virtual ResourceId  execute                              (const WaveBootPhase &waveBootPhase);
                ResourceId  shutdownDatabaseStep                 (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId  updateFrameworkConfigurationToDefaultStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

    // Now the data members

    private :
                            WaveBootPhase m_waveBootPhase;
    protected :
    public :
};

}

#endif // RECOVERPRISMBOOTAGENT_H
