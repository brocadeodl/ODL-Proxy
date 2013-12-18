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

#ifndef PERSISTENTPRISMBOOTAGENT_H
#define PERSISTENTPRISMBOOTAGENT_H

#include "Framework/Boot/PrismBootAgent.h"

namespace WaveNs
{

class PersistentPrismBootAgent : public PrismBootAgent
{
    private :
        virtual bool           isAPersistentBoot                                          ();
        virtual WaveBootReason getReason                                                  () const;
        virtual bool           getRollBackFlag                                            () const;

                void           cleanPreparedTransactions                                  ();
                void           emptyDatabaseToRollBack                                    ();
                void           reloadPreviousDataBase                                     ();
//                void           rollbackCfgFile                                            ();

        virtual bool           isToBeExcludedForEnableAndBoot                             (const PrismServiceId& prismServiceId);
        virtual bool           isToBeExcludedFromInitializeDuringPrePhase                 (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInitializePhase                          (const PrismServiceId &prismServiceId);

    protected :
    public :
                            PersistentPrismBootAgent                                   (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual            ~PersistentPrismBootAgent                                   ();
        virtual ResourceId  execute                                                    (const WaveBootPhase &waveBootPhase);

                ResourceId  createFrameworkConfigurationFromPersistedConfigurationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId  connectToKnownLocationsStep                                (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId  rollBackDataBase                                           (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId  resetLocationRole                                          (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                ResourceId  disconnectFromAllConnectedLocations                        (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

    // Now the data members

    private :
                bool          m_isRollBackDataBaseValid;
                bool          m_isRollCfgFileIsValid;
                WaveBootPhase m_waveBootPhase;
    protected :
    public :
};

}

#endif // PERSISTENTPRISMBOOTAGENT_H
