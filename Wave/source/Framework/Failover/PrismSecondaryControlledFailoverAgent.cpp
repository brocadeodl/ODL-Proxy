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

#include "Framework/Failover/PrismSecondaryControlledFailoverAgent.h"
#include "Framework/Failover/PrismFailoverAgentContext.h"
#include "Framework/Failover/PrismFailoverAgentSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

PrismSecondaryControlledFailoverAgent::PrismSecondaryControlledFailoverAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismFailoverAgent (pWaveObjectManager, currentFrameworkSequenceGenerator),m_failoverDueToPrimaryRemoval (false)
{
}

PrismSecondaryControlledFailoverAgent::~PrismSecondaryControlledFailoverAgent ()
{
}

FrameworkObjectManagerFailoverReason PrismSecondaryControlledFailoverAgent::getFailoverReason () const
{
    return (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED);
}

void PrismSecondaryControlledFailoverAgent::setFailoverDueToPrimaryRemoval ()
{
     m_failoverDueToPrimaryRemoval = true;
}

bool  PrismSecondaryControlledFailoverAgent::getFailoverDueToPrimaryRemoval () const
{
    return m_failoverDueToPrimaryRemoval;
}

void PrismSecondaryControlledFailoverAgent::execute (PrismFailoverAgentContext *pPrismFailoverAgentContext)
{

    //call the base class execute if it is not the case of primary removal
    if(!getFailoverDueToPrimaryRemoval()) 
    {
        PrismFailoverAgent::execute (pPrismFailoverAgentContext);
        return;
    }

    WaveNs::PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::resetThisLocationToPrimaryStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::initializePrismGlobalServicesDuringPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::enablePrismGlobalServicesDuringPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::bootPrismGlobalServicesDuringPrePhaseStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::initializePrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::enablePrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::listenForEventsPrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::bootPrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::pausePersistenceForPrimaryChangeStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::informSecondaryLocationsToChangePrimaryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::resumeDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::backendSyncUpStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::broadcastPrimaryChangedEventForPlugins), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::failoverPrismServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::removeOldPrimaryLocationStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::startHeartBeatToSecondariesStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::savePrismConfiguration), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::failoverForUnknownWaveNodePrismServicesStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryControlledFailoverAgent::prismLinearSequencerFailedStep)
    };

    PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext = new PrismFailoverAgentSequencerContext (pPrismFailoverAgentContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismFailoverAgentSequencerContext->setFailedLocationIds                    (pPrismFailoverAgentContext->getFailedLocationIds());
    pPrismFailoverAgentSequencerContext->setServiceToBeIgnored                   (pPrismFailoverAgentContext->getServiceToBeIgnored());
    pPrismFailoverAgentSequencerContext->setOlderPrimaryLocationId               (FrameworkToolKit::getClusterPrimaryLocationId());
    pPrismFailoverAgentSequencerContext->setAnyConfigurationChangeTrackingNumber (PersistenceLocalObjectManager::getAnyConfigurationChangeTrackingNumber ());

    trace (TRACE_LEVEL_DEBUG, string("PrismSecondaryControlledFailoverAgent::execute : set OlderPrimaryLocationId = ") + FrameworkToolKit::getClusterPrimaryLocationId());
    pPrismFailoverAgentSequencerContext->setIsPrincipalChangedWithThisFailover (true);
    
    pPrismFailoverAgentSequencerContext->holdAll ();
    pPrismFailoverAgentSequencerContext->start ();
}

bool PrismSecondaryControlledFailoverAgent::isToBeExcludedForFailover (const PrismServiceId &prismServiceId)
{
    if (((PrismFrameworkObjectManager::getPrismServiceId ()) == prismServiceId) ||
        (true == (FrameworkToolKit::isALocalService (prismServiceId))) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}


WaveBootReason PrismSecondaryControlledFailoverAgent::getBootReason () const
{
    return (WAVE_BOOT_SECONDARY_CONTROLLED_FAILOVER_BOOT);
}

}
