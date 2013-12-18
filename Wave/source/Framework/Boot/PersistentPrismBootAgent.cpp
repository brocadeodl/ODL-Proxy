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

#include "Framework/Boot/PersistentPrismBootAgent.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableMessage.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Database/DatabaseObjectManagerCleanPreparedTransactionsMessage.h"
#include "Cluster/Local/WaveNode.h"
#include <vector>
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Cluster/PrismCluster.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Persistence/PersistenceObjectManagerUpdateInstanceIdMesssage.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

using namespace std;

namespace WaveNs
{

PersistentPrismBootAgent::PersistentPrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismBootAgent (pWaveObjectManager, currentFrameworkSequenceGenerator),
      m_isRollBackDataBaseValid (false),
      m_isRollCfgFileIsValid (true),
      m_waveBootPhase (WAVE_BOOT_PHASE_ALL_PHASES)
{
}

PersistentPrismBootAgent::~PersistentPrismBootAgent ()
{
}

ResourceId PersistentPrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    m_waveBootPhase = waveBootPhase;

    if (WAVE_BOOT_PHASE_ALL_PHASES == m_waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::upgradePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::rollBackDataBase),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::databaseSanityCheckStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::upgradeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootLocalPrismServicesStep),

            //reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::connectToKnownLocationsStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::upgradeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::updateDatabaseForeignKeyConstraintsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };
    
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();
        
        // Cleanup Upgrade data structures after Persistent boot 

        /* FIXME: Commenting it for now as this will be required while executing BIST for db_conversion. Can be uncommented once Unit testing is done.
                  Moreover this celanup should happen in all boot mode cases when getDetectSchemaChange is true and not only in persistent boot.
        if (true ==  FrameworkToolKit::getDetectSchemaChange ())
        {
            OrmRepository *pOrmRepository = OrmRepository::getInstance ();

            pOrmRepository->getMOSchemaInfoRepository().cleanupSchemaDifferenceInfo ();
            vector<string> tableNames = pOrmRepository->getTableNames (); 
        
            for (unsigned int i=0; i < tableNames.size (); i++)
            {
                (pOrmRepository->getTableByName (tableNames[i]))->cleanupManagedObjectAttributes ();
            }
        }*/

        return (status);
    }
    else if (WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE == m_waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::resetLocationRole),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::rollBackDataBase),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else if (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 == m_waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::rollBackDataBase),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::backendSyncUpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else if (WAVE_BOOT_ROLL_BACK_BOOT_PHASE == m_waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::resetLocationRole),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::disconnectFromAllConnectedLocations),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::rollBackDataBase),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootLocalPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::backendSyncUpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };
    
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else if (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2 == m_waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::resetLocationRole),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::disconnectFromAllConnectedLocations),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::rollBackDataBase),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsLocalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootLocalPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::backendSyncUpStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    
    }
    else if (WAVE_BOOT_RESET_NODE_TO_UNCONFIRM_ROLE == m_waveBootPhase)
    {
        WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::resetLocationRole),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::disconnectFromAllConnectedLocations),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalServicesDuringPrePhaseStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesDuringPrePhaseStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::initializeGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::enableGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::listenForEventsGlobalPrismServicesStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::bootGlobalPrismServicesStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

        return (status);
    }
    else
    {
        return (WAVE_MESSAGE_ERROR);
    }

}

ResourceId PersistentPrismBootAgent::disconnectFromAllConnectedLocations (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    (PrismFrameworkObjectManager::getInstance())->disconnectFromAllKnownLocations ();

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PersistentPrismBootAgent::resetLocationRole (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    LocationBase *pThisLocation = (PrismFrameworkObjectManager::getInstance ())->getThisLocation ();

    prismAssert (NULL != pThisLocation, __FILE__, __LINE__);

    LocationRole    thisLocationRole = pThisLocation->getLocationRole ();
 
    if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == thisLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_2 == thisLocationRole) || (LOCATION_SECONDARY_CLUSTER_PHASE_3 == thisLocationRole))
    {
        pThisLocation->setLocationRole (LOCATION_STAND_ALONE);
        pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
        pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());

        m_isRollBackDataBaseValid = true;
    }
    else if ((LOCATION_SECONDARY_REJOIN_PHASE_0 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_2 == thisLocationRole) || (LOCATION_SECONDARY_REJOIN_PHASE_3 == thisLocationRole))
    {
        m_isRollBackDataBaseValid = true;

        pThisLocation->setLocationRole (LOCATION_SECONDARY_UNCONFIRMED);
        pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
        pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());
    }
    else if ((LOCATION_SECONDARY == thisLocationRole) || (LOCATION_SECONDARY_UNCONFIRMED == thisLocationRole) || (LOCATION_PRIMARY_UNCONFIRMED == thisLocationRole))
    {
        pThisLocation->setLocationRole (LOCATION_SECONDARY_UNCONFIRMED);    

        pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
        pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());
    }
    else if (LOCATION_PRIMARY == thisLocationRole)
    {
        pThisLocation->setLocationRole (LOCATION_PRIMARY_UNCONFIRMED);    

        pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
        pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());
    }
    else if (LOCATION_STAND_ALONE == thisLocationRole)
    {
        pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
        pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "There is no other Phase Possible for timer timeout");

        prismAssert (false, __FILE__, __LINE__);

        return (WAVE_MESSAGE_ERROR);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    string                      prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
    PrismFrameworkConfiguration prismFrameworkConfiguration;
    ResourceId                  status                       = WAVE_MESSAGE_ERROR;

    status = prismFrameworkConfiguration.loadConfiguration (prismConfigurationfileName);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : How can this be.  We detected that there was a configuration file.  And now it is not available???");
        prismAssert (false, __FILE__, __LINE__);

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        LocationBase *pThisLocation = (PrismFrameworkObjectManager::getInstance ())->getThisLocation ();

        prismAssert (NULL != pThisLocation, __FILE__, __LINE__);

        FrameworkToolKit::setLastUsedLocationId (prismFrameworkConfiguration.m_lastUsedLocationId);

        pThisLocation->setLocationRole             (static_cast<LocationRole> (prismFrameworkConfiguration.m_thisLocationRole));
        pThisLocation->setLocationId               (prismFrameworkConfiguration.m_thisLocationLocationId);
        pThisLocation->resetIpAddress              (prismFrameworkConfiguration.m_thisLocationIpAddress);

        FrameworkToolKit::setIsDbRestoreIncomplete (prismFrameworkConfiguration.m_dbRestoreIncomplete);

        if (LOCATION_STAND_ALONE == (pThisLocation->getLocationRole ()))
        {
            trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : Setting Location Role To : " + FrameworkToolKit::localize (LOCATION_STAND_ALONE));

            pThisLocation->setClusterPrimaryLocationId (prismFrameworkConfiguration.m_clusterPrimaryLocationId);
            pThisLocation->setClusterPrimaryPort       (prismFrameworkConfiguration.m_clusterPrimaryPort);
        }
        else
        {
            if (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 == m_waveBootPhase)
            {
                // this can happen only when in unconfirmed state this node receive rejoin request and node failed in phase 0 before setting state as phase 0
                m_isRollBackDataBaseValid = false;

                (PrismFrameworkObjectManager::getInstance ())->setIsStartupValid (prismFrameworkConfiguration.m_isStartupValid);
                (PrismFrameworkObjectManager::getInstance ())->setStartupFileType (prismFrameworkConfiguration.m_startupFileType);
                (PrismFrameworkObjectManager::getInstance ())->setStartupFileName (prismFrameworkConfiguration.m_startupFileName);

                pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
                pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());

                pThisLocation->setLocationRole (LOCATION_SECONDARY_UNCONFIRMED);

                return (WAVE_MESSAGE_SUCCESS);
            }
            else if (LOCATION_PRIMARY == (pThisLocation->getLocationRole ()))
            {
                trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : Re-Setting Location Role To : " + FrameworkToolKit::localize (LOCATION_PRIMARY_UNCONFIRMED));

                pThisLocation->setLocationRole (LOCATION_PRIMARY_UNCONFIRMED);
            }
            else if (LOCATION_SECONDARY == (pThisLocation->getLocationRole ()))
            {
                trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : Re-Setting Location Role To : " + FrameworkToolKit::localize (LOCATION_SECONDARY_UNCONFIRMED));

                pThisLocation->setLocationRole (LOCATION_SECONDARY_UNCONFIRMED);
            }
            else if ((LOCATION_SECONDARY_CLUSTER_PHASE_0 == (pThisLocation->getLocationRole ())) || (LOCATION_SECONDARY_CLUSTER_PHASE_1 == (pThisLocation->getLocationRole ())) || (LOCATION_SECONDARY_CLUSTER_PHASE_2 == (pThisLocation->getLocationRole ())) || (LOCATION_SECONDARY_CLUSTER_PHASE_3 == (pThisLocation->getLocationRole ())))
            {
                pThisLocation->setLocationRole (LOCATION_STAND_ALONE);

                // if dbRestoreIncomplete flag is set then restoring the db will be taken care in PersistenceOM::boot. Hence, no need to execute rollBackDataBase again here. 

                 m_isRollBackDataBaseValid = (true == prismFrameworkConfiguration.m_dbRestoreIncomplete) ? false : true;    

                (PrismFrameworkObjectManager::getInstance ())->setIsStartupValid (prismFrameworkConfiguration.m_isStartupValid);
                (PrismFrameworkObjectManager::getInstance ())->setStartupFileType (prismFrameworkConfiguration.m_startupFileType);
                (PrismFrameworkObjectManager::getInstance ())->setStartupFileName (prismFrameworkConfiguration.m_startupFileName);

                pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
                pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());                
            
                return (WAVE_MESSAGE_SUCCESS);    
            }
            else if ((LOCATION_SECONDARY_REJOIN_PHASE_0 == pThisLocation->getLocationRole ()) || (LOCATION_SECONDARY_REJOIN_PHASE_1 == pThisLocation->getLocationRole ()) || (LOCATION_SECONDARY_REJOIN_PHASE_2 == pThisLocation->getLocationRole ()) || (LOCATION_SECONDARY_REJOIN_PHASE_3 == pThisLocation->getLocationRole ()))
            {
               trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : Handling Reboot in the middle of Rejoin Phase , Re-Setting Location Role To : " + FrameworkToolKit::localize (LOCATION_SECONDARY_UNCONFIRMED));

                m_isRollBackDataBaseValid = true;

                pThisLocation->setLocationRole (LOCATION_SECONDARY_UNCONFIRMED);
            }
    
            pThisLocation->setClusterPrimaryLocationId (pThisLocation->getLocationId ());
            pThisLocation->setClusterPrimaryPort       (pThisLocation->getPort ());
        }

        UI32 numberOfKnownLocations = prismFrameworkConfiguration.m_knownLocationIpAddresses.size ();
        UI32 i                      = 0;

        for (i = 0; i < numberOfKnownLocations; i++)
        {
            pThisLocation->addKnownLocation (prismFrameworkConfiguration.m_knownLocationLocationIds[i], prismFrameworkConfiguration.m_knownLocationIpAddresses[i], prismFrameworkConfiguration.m_knownLocationPorts[i]);
        }

        UI32 numberOfKnownSubLocations = prismFrameworkConfiguration.m_knownSubLocationIpAddresses.size ();

        for (i = 0; i < numberOfKnownSubLocations; i++)
        {
            pThisLocation->addSubLocation (prismFrameworkConfiguration.m_knownSubLocationLocationIds[i], prismFrameworkConfiguration.m_knownSubLocationIpAddresses[i], prismFrameworkConfiguration.m_knownSubLocationPorts[i]);
        }

        (PrismFrameworkObjectManager::getInstance ())->setIsStartupValid (prismFrameworkConfiguration.m_isStartupValid);
        (PrismFrameworkObjectManager::getInstance ())->setStartupFileType (prismFrameworkConfiguration.m_startupFileType);
        (PrismFrameworkObjectManager::getInstance ())->setStartupFileName (prismFrameworkConfiguration.m_startupFileName);


        // if dbRestoreIncomplete flag is set then restoring the db will be taken care in PersistenceOM::boot. Hence, no need to execute rollBackDataBase again here. 
        if (true == prismFrameworkConfiguration.m_dbRestoreIncomplete)
        {
            m_isRollBackDataBaseValid = false;    
        }

        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
}

ResourceId PersistentPrismBootAgent::connectToKnownLocationsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    LocationBase       *pThisLocation                = (PrismFrameworkObjectManager::getInstance ())->getThisLocation ();
    vector<LocationId>  knownRemoteLocations;
    UI32                numberOfKnownRemoteLocations = 0;
    UI32                i                            = 0;
    string              ipAddress                    = "";
    SI32                port                         = 0;

    prismAssert (NULL != pThisLocation, __FILE__, __LINE__);

    pThisLocation->getKnownRemoteLocations (knownRemoteLocations);
    numberOfKnownRemoteLocations = knownRemoteLocations.size ();

    for (i = 0; i < numberOfKnownRemoteLocations; i++)
    {
        ResourceId connectionStatus = pThisLocation->connectToRemoteLocation (knownRemoteLocations[i], 60, 60);

        if (FRAMEWORK_SUCCESS != connectionStatus)
        {
            ipAddress = FrameworkToolKit::getIpAddressForLocationId (knownRemoteLocations[i]);
            port      = FrameworkToolKit::getPortForLocationId (knownRemoteLocations[i]);

            trace (TRACE_LEVEL_ERROR, string ("PersistentPrismBootAgent::connectToKnownLocationsStep : Failed to connect to ") + ipAddress + ":" + port + ".");
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PersistentPrismBootAgent::rollBackDataBase (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, string("PersistentPrismBootAgent::rollBackDataBase: m_isRollBackDataBaseValid = ") + m_isRollBackDataBaseValid);
    if (false == m_isRollBackDataBaseValid)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }

    // removing the cfg file to make sure if node rebooted in the middle of rollback.It should come clean.

    const string    prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
    vector<string>  output;
    SI32            cmdStatus                    = 0;

    trace (TRACE_LEVEL_WARN, string ("PersistentPrismBootAgent::rollBackDataBase: deleting file ") + prismConfigurationfileName);

    cmdStatus = FrameworkToolKit::systemCommandOutput ((string ("/bin/rm -rf ") + prismConfigurationfileName).c_str(), output);

    if ( cmdStatus != 0 )
    {        
        trace (TRACE_LEVEL_ERROR, string("PersistentPrismBootAgent::rollBackDataBase: Cmd to delete file ")+ prismConfigurationfileName + string(" failed with error message : ") + output[0]);
    }

    cleanPreparedTransactions ();
    emptyDatabaseToRollBack ();
    reloadPreviousDataBase  ();

    if ( (WAVE_MESSAGE_SUCCESS == FrameworkToolKit::isConnectedToHaPeerLocation ()) && (true == FrameworkToolKit::getIsLiveSyncEnabled ()) )
    {
        trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::rollBackDatabase: Rollback database on standby is triggered");

        FrameworkObjectManagerRollbackStandbyOnActiveRollbackMessage rollbackStandbyOnActiveRollbackMessage;
        ResourceId                                                   status = WAVE_MESSAGE_SUCCESS;

        status = sendSynchronously (&rollbackStandbyOnActiveRollbackMessage, FrameworkToolKit::getHaPeerLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PersistentPrismBootAgent::rollBackDataBase: Unable to perform rollback on STANDBY. Status: " + FrameworkToolKit::localize (status));
            FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
        }
        else
        {
            status = rollbackStandbyOnActiveRollbackMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::rollBackDataBase: Succeeded in performing rollback on STANDBY.");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "PersistentPrismBootAgent::rollBackDataBase: Failed to perform rollback on STANDBY. Status: " + FrameworkToolKit::localize (status));
                FrameworkToolKit::notifySyncUpdateFailAndStopSync(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_FAILED);
            }
        }
    }
    else if ((FrameworkToolKit::getSyncState () == IN_SYNC) && (false == FrameworkToolKit::getIsLiveSyncEnabled ()))
    {
        // DB previously in sync from sync dump. but live transaction sync not supported. So set out of sync and notify on sync update fail
        trace (TRACE_LEVEL_WARN, "PersistentPrismBootAgent::rollBackDataBase: Live sync disabled. Notify sync update failure on first update after sync dump.");
        FrameworkToolKit::notifySyncUpdateFailure(WAVE_MESSAGE_ERROR_CLUSTER_DB_HASYNC_NOT_SUPPORTED);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::rollBackDatabase: Rollback db on standby will not be initiated as Peer is disconnected.");
    }

    if (true == m_isRollCfgFileIsValid)
    {
        FrameworkToolKit::rollbackCfgFile ();
    }
    else
    {
        m_isRollCfgFileIsValid = true;
    }

    FrameworkToolKit::savePrismConfiguration (true);

    PersistenceObjectManagerUpdateInstanceIdMesssage message;

    ResourceId  status  = sendSynchronously (&message); 

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::rollBackDataBase : Could not send message to update instance Id.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::rollBackDataBase : Not able to update Instance Id  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void PersistentPrismBootAgent::cleanPreparedTransactions ()
{
    trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::cleanPreparedTransactions: Entering ...");

    DatabaseObjectManagerCleanPreparedTransactionsMessage databaseObjectManagerCleanPreparedTransactionsMessage;
    ResourceId                                            cleanPreparedTransactionsStatus                        = WAVE_MESSAGE_SUCCESS;

    cleanPreparedTransactionsStatus = sendSynchronously (&databaseObjectManagerCleanPreparedTransactionsMessage); 

    if (WAVE_MESSAGE_SUCCESS != cleanPreparedTransactionsStatus)
    {
        trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::cleanPreparedTransactions: Could not send message to Clean Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        cleanPreparedTransactionsStatus = databaseObjectManagerCleanPreparedTransactionsMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == cleanPreparedTransactionsStatus)
        {
            trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::cleanPreparedTransactions: Succeeded in cleaning previously Prepared Transactions.");
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::cleanPreparedTransactions: Failed to clean previously Prepared Transactions. Status : " + FrameworkToolKit::localize (cleanPreparedTransactionsStatus));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void PersistentPrismBootAgent::emptyDatabaseToRollBack ()
{
    trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::emptyDatabaseToRollBack Entering ...");

    DatabaseObjectManagerEmptyMessage message;
    ResourceId                        status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {         trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::emptyDatabaseToRollBack : Could not send message to empty database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::emptyDatabaseToRollBack : Message to empty database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void PersistentPrismBootAgent::reloadPreviousDataBase ()
{
    trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::reloadPreviousDataBase Entering....");

    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName2 ());

    DatabaseObjectManagerRestoreMessage message (backupFileName);
    message.setDataOnlyRestore (true);

    ResourceId                          status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::reloadPreviousDataBase : Could not send message to restore database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_ERROR_DB_RESTORE_FAILED == status)
        {
            trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::reloadPreviousDataBase : Not Able to Restore DB from Backed Up File. Completion Status : " + FrameworkToolKit::localize (status));
            trace (TRACE_LEVEL_FATAL, "PersistentPrismBootAgent::reloadPreviousDataBase : Going For Extreme Recovery By going to default/startup configuration");

            status = FrameworkToolKit::changePrismConfigurationValidity ( false );
            if ( status != WAVE_MESSAGE_SUCCESS  )
            {
                trace (TRACE_LEVEL_FATAL, ("PersistentPrismBootAgent::reloadPreviousDataBase : Removing CFG file failed in extreme Recoverycase "));
            }
            system ("/sbin/reboot -f");

            sleep (300);
    
            trace (TRACE_LEVEL_FATAL, ("System is not rebooting, Manual recovery is required"));
        
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PersistentPrismBootAgent::reloadPreviousDataBase : SUCCESSFULLY rollback to the last known valid previous DB configuration.");
        }
    }
}

bool PersistentPrismBootAgent::isAPersistentBoot ()
{
    return (true);
}

WaveBootReason PersistentPrismBootAgent::getReason () const
{
    return (WAVE_BOOT_PERSISTENT_BOOT);
}

bool PersistentPrismBootAgent::isToBeExcludedForEnableAndBoot (const PrismServiceId& prismServiceId)
{
    if ((WAVE_BOOT_ROLL_BACK_BOOT_PHASE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2 == m_waveBootPhase) || (WAVE_BOOT_RESET_NODE_TO_UNCONFIRM_ROLE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 == m_waveBootPhase) || (WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE == m_waveBootPhase))
    {
        if (((PrismFrameworkObjectManager::getPrismServiceId ()) == prismServiceId) || (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

bool PersistentPrismBootAgent::isToBeExcludedFromInitializeDuringPrePhase (const PrismServiceId &prismServiceId)
{
    if ((WAVE_BOOT_ROLL_BACK_BOOT_PHASE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2 == m_waveBootPhase) || (WAVE_BOOT_RESET_NODE_TO_UNCONFIRM_ROLE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 == m_waveBootPhase) || (WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE == m_waveBootPhase))
    {
        if (((PrismFrameworkObjectManager::getPrismServiceId ()) == prismServiceId) || (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

bool PersistentPrismBootAgent::isToBeExcludedFromInitializePhase (const PrismServiceId &prismServiceId)
{
    if ((WAVE_BOOT_ROLL_BACK_BOOT_PHASE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2 == m_waveBootPhase) || (WAVE_BOOT_RESET_NODE_TO_UNCONFIRM_ROLE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 == m_waveBootPhase) || (WAVE_BOOT_PREPARE_FOR_HA_FAILOVER_PHASE == m_waveBootPhase))
    {
        if (((PrismFrameworkObjectManager::getPrismServiceId ()) == prismServiceId) || (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

bool PersistentPrismBootAgent::getRollBackFlag () const
{
    if ((WAVE_BOOT_ROLL_BACK_BOOT_PHASE == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_AFTER_PHASE_2 == m_waveBootPhase) || (WAVE_BOOT_ROLL_BACK_BOOT_PHASE_BEFORE_PHASE_0 == m_waveBootPhase))
    {
        return (true);
    }

    return (false);
}

}
