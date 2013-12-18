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

#ifndef PRISMFAILOVERAGENT_H
#define PRISMFAILOVERAGENT_H

#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include <vector>
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"

namespace WaveNs
{

class PrismFailoverAgentSequencerContext;
class PrismFailoverAgentContext;

class PrismFailoverAgent : protected WaveWorker
{
    private :
        virtual bool           isToBeExcludedForFailover                    (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedDuringPrePhase                 (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcluded                               (const PrismServiceId &prismServiceId);
        virtual WaveBootReason getBootReason                                () const = 0;

        virtual FrameworkObjectManagerFailoverReason getFailoverReason      () const = 0;

                void           pausePersistence                             ();
                void           resumePersistence                            ();

    protected :
             PrismFailoverAgent                                             (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        void failoverPrismServicesStep                                      (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);

        void initializePrismGlobalServicesDuringPrePhaseStep                (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext); 
        void enablePrismGlobalServicesDuringPrePhaseStep                    (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void listenForEventsPrismGlobalServicesDuringPrePhaseStep           (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void bootPrismGlobalServicesDuringPrePhaseStep                      (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);

        void initializePrismGlobalServicesStep                              (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void enablePrismGlobalServicesStep                                  (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void listenForEventsPrismGlobalServicesStep                         (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void bootPrismGlobalServicesStep                                    (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);

        void createDatabaseDump                                             ();
        void pausePersistenceForPrimaryChangeStep                           (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToChangePrimaryStep                    (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToChangePrimaryCallback                (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
        void informSecondaryLocationsToChangePrimaryPhase1Step              (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToChangePrimaryPhase1Callback          (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
        void resumeDatabaseDumpStep                                         (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToChangePrimaryPhase2Step              (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToChangePrimaryPhase2Callback          (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
        void informSecondaryLocationsToChangePrimaryPhase3Step              (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToChangePrimaryPhase3Callback          (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
        void removeOldPrimaryLocationStep                                   (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void resetThisLocationToPrimaryStep                                 (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void startHeartBeatToSecondariesStep                                (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void broadcastPrimaryChangedEventForPlugins                         (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void savePrismConfiguration                                         (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void informSecondaryLocationsToRemoveFailedFromKnownLocationsStep   (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void failoverForUnknownWaveNodePrismServicesStep                    (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);
        void backendSyncUpStep                                              (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext);

    public :
        virtual            ~PrismFailoverAgent                              ();
        virtual  void      execute                                          (PrismFailoverAgentContext *pPrismFailoverAgentContext);        

    // Now the data members

    private :
    protected :
        FrameworkSequenceGenerator &m_currentFrameworkSequenceGenerator;

    public :
};

}

#endif // PRISMFAILOVERAGENT_H
