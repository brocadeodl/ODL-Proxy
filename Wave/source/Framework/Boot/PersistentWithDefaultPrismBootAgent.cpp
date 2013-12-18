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

#include "Framework/Boot/PersistentWithDefaultPrismBootAgent.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableMessage.h"

#include <vector>

using namespace std;

namespace WaveNs
{

PersistentWithDefaultPrismBootAgent::PersistentWithDefaultPrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismBootAgent (pWaveObjectManager, currentFrameworkSequenceGenerator)
{
}

PersistentWithDefaultPrismBootAgent::~PersistentWithDefaultPrismBootAgent ()
{
}

ResourceId PersistentWithDefaultPrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::initializePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::enablePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::installPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::bootPrismServicesDuringPrePhaseStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::databaseSanityCheckStep), 

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::initializeLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::enableLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::listenForEventsLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::installLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::bootLocalPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::connectToKnownLocationsStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::initializeGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::enableGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::listenForEventsGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::installGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::bootGlobalPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::updateDatabaseForeignKeyConstraintsStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PersistentWithDefaultPrismBootAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

    return (status);
}

ResourceId PersistentWithDefaultPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    string                      prismConfigurationfileName   = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();
    PrismFrameworkConfiguration prismFrameworkConfiguration;
    ResourceId                  status                       = WAVE_MESSAGE_ERROR;

    status = prismFrameworkConfiguration.loadConfiguration (prismConfigurationfileName);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PersistentWithDefaultPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : How can this be.  We detected that there was a configuration file.  And now it is not available???");
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
        pThisLocation->setClusterPrimaryLocationId (prismFrameworkConfiguration.m_clusterPrimaryLocationId);
        pThisLocation->setClusterPrimaryPort       (prismFrameworkConfiguration.m_clusterPrimaryPort);

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

        status = WAVE_MESSAGE_SUCCESS;
    }

    return (status);
}

ResourceId PersistentWithDefaultPrismBootAgent::connectToKnownLocationsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
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

            trace (TRACE_LEVEL_ERROR, string ("PersistentWithDefaultPrismBootAgent::createFrameworkConfigurationFromPersistedConfigurationStep : Failed to connect to ") + ipAddress + ":" + port + ".");
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

bool PersistentWithDefaultPrismBootAgent::isAPersistentBoot ()
{
    return (true);
}

WaveBootReason PersistentWithDefaultPrismBootAgent::getReason () const
{
    return (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT);
}

}
