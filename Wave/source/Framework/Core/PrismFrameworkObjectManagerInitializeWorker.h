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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKOBJECTMANAGERINITIALIZEWORKER_H
#define PRISMFRAMEWORKOBJECTMANAGERINITIALIZEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Boot/PrismBootAgent.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Postboot/PrismPostbootDebug.h"
#include "Framework/ObjectRelationalMapping/DatabaseSchema.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"

namespace WaveNs
{

typedef enum
{
    WAVE_BOOT_UNKNOWN = 0,
    WAVE_BOOT_FIRST_TIME,
    WAVE_BOOT_PERSISTENT,
    WAVE_BOOT_PERSISTENT_WITH_DEFAULT,
    WAVE_BOOT_SECONDARY_CONFIGURE,
    WAVE_BOOT_SECONDARY_UNCONFIGURE,
    WAVE_BOOT_SECONDARY_REJOIN,
    WAVE_BOOT_HASTANDBY,
    WAVE_BOOT_PREPARE_FOR_HA_BOOT
} PrismBootMode;

class PrismFrameworkInitializeWorkerStartServicesContext;

class PrismFrameworkObjectManagerInitializeWorker : public WaveWorker
{
    private :
        ResourceId determineNodeBootModeStep (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);
        ResourceId chooseABootAgentStep      (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);
        ResourceId runTheBootAgentStep       (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);
        ResourceId saveConfigurationStep     (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);
        ResourceId destroyABootAgentStep     (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);
        ResourceId informServicesToInitializeBeforeBootComplete (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);
        ResourceId declareBootCompleteStep   (PrismFrameworkInitializeWorkerStartServicesContext *pPrismFrameworkInitializeWorkerStartServicesContext);

        ResourceId performSchemaConversion             (const PrismFrameworkConfiguration &prismFrameworkConfiguration, bool ignoreConfigFile = false);
    	void	   FrameworkUpdateDatabaseSchema	   (FrameworkObjectManagerUpdateDatabaseSchema *pFrameworkObjectManagerUpdateDatabaseSchema);
        //void       buildDatabaseSchemaInfoRepository   (const PrismFrameworkConfiguration &prismFrameworkConfiguration, ManagedObjectSchemaInfoRepository &moRepository, vector<pair<string, UI32> >&tableClassIdTuples);
        ResourceId buildDatabaseSchemaInfoRepository   (const PrismFrameworkConfiguration &prismFrameworkConfiguration, ManagedObjectSchemaInfoRepository &moRepository);

    protected :
    public :
                           PrismFrameworkObjectManagerInitializeWorker (WaveObjectManager *pWaveObjectManager);
                          ~PrismFrameworkObjectManagerInitializeWorker ();

               ResourceId  startPrismServices                          (const PrismBootMode &prismBootMode = WAVE_BOOT_UNKNOWN, const WaveBootPhase &waveBootPhase = WAVE_BOOT_PHASE_ALL_PHASES);

        static bool        getInitialBootCompleted                     ();
        static void        setInitialBootCompleted                     (const bool &initialBootCompleted);
        static void        setInitialSchemaInfoObjectSaved             (bool schemaInfoObjectSaved);
        static bool        getInitialSchemaInfoObjectSaved             ();

        void               populateSchemaUpgradeTableInDb                           ();
    // Now the data members

    private :
               PrismBootAgent      *m_pPrismBootAgent;
        static bool                 m_initialBootcompleted;
        static PrismMutex           m_initialBootcompletedMutex;
               PrismPostbootDebug   m_postbootDebug; // to allow invocation of intialize function
        static bool                 m_initialSchemaInfoObjectSaved;
        static PrismMutex           m_initialSchemaInfoObjectSavedMutex;

    protected :
    public :

    friend class PrismFrameworkObjectManager;
};

}

#endif //PRISMFRAMEWORKOBJECTMANAGERINITIALIZEWORKER_H
