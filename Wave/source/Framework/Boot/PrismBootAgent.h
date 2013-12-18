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

#ifndef PRISMBOOTAGENT_H
#define PRISMBOOTAGENT_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"

#include <vector>

using namespace std;

namespace WaveNs
{

typedef enum
{
    WAVE_BOOT_PHASE_PRE_PHASE                     = 0,
    WAVE_BOOT_PHASE_POST_PHASE                    = 1,
    WAVE_BOOT_PHASE_AFTER_POST_PHASE              = 2,
    WAVE_BOOT_ROLL_BACK_BOOT_PHASE                = 3,    
    WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2  = 4,
    WAVE_BOOT_RESET_NODE_TO_UNCONFIRM_ROLE        = 5,
    WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 = 6,
    WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE       = 7,
    WAVE_BOOT_PHASE_ALL_PHASES 
} WaveBootPhase;

class PrismBootAgent : protected WaveWorker
{
    private :
        virtual bool           isAPersistentBoot                            ();
        virtual bool           isToBeExcludedForEnableAndBoot               (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInstallDuringPrePhase      (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInstall                    (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromCurrentBootPhase           (PrismServiceId prismServiceId);
        virtual bool           isToBeExcludedFromInitializeDuringPrePhase   (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInitializePhase            (const PrismServiceId &prismServiceId);

        virtual WaveBootReason getReason                                    () const;
        virtual bool           getRollBackFlag                              () const;
        void                   handleUpgradeFailure                         (const PrismServiceId& service);

    protected :
                   PrismBootAgent                                 (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        void       addAStepToPrismBootAgent                       (PrismSynchronousLinearSequencerStep prismBootAgentStep);
        virtual bool           willBeAPrimaryLocation                       ();

        ResourceId initializePrismServicesDuringPrePhaseStep      (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId listenForEventsPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId enablePrismServicesDuringPrePhaseStep          (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId upgradePrismServicesDuringPrePhaseStep         (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId installPrismServicesDuringPrePhaseStep         (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId bootPrismServicesDuringPrePhaseStep            (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId initializeGlobalPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId listenForEventsGlobalServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId enableGlobalPrismServicesDuringPrePhaseStep     (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId upgradeGlobalPrismServicesDuringPrePhaseStep    (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId bootGlobalPrismServicesDuringPrePhaseStep       (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId initializeLocalPrismServicesStep               (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId listenForEventsLocalPrismServicesStep          (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId enableLocalPrismServicesStep                   (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId upgradeLocalPrismServicesStep                  (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId installLocalPrismServicesStep                  (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId bootLocalPrismServicesStep                     (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId initializeGlobalPrismServicesStep              (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId listenForEventsGlobalPrismServicesStep         (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId enableGlobalPrismServicesStep                  (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId upgradeGlobalPrismServicesStep                 (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId installGlobalPrismServicesStep                 (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId bootGlobalPrismServicesStep                    (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId initializePrismServicesStep                    (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId listenForEventsPrismServicesStep               (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId enablePrismServicesStep                        (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId upgradePrismServicesStep                       (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId installPrismServicesStep                       (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId bootPrismServicesStep                          (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId updateDatabaseForeignKeyConstraintsStep        (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId databaseSanityCheckStep                        (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId updateUserDefinedKeyCombinationConstraintsAndKeysStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

        ResourceId backendSyncUpStep                              (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

    public :
        virtual            ~PrismBootAgent ();
        virtual ResourceId  execute        (const WaveBootPhase &waveBootPhase);

    // Now the data members

    private :
    protected :
        FrameworkSequenceGenerator &m_currentFrameworkSequenceGenerator;

    public :
};

}

#endif // PRISMBOOTAGENT_H
