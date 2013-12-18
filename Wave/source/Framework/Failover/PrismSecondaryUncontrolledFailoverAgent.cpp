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

#include "Framework/Failover/PrismSecondaryUncontrolledFailoverAgent.h"
#include "Framework/Failover/PrismFailoverAgentContext.h"
#include "Framework/Failover/PrismFailoverAgentSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

PrismSecondaryUncontrolledFailoverAgent::PrismSecondaryUncontrolledFailoverAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : PrismFailoverAgent (pWaveObjectManager, currentFrameworkSequenceGenerator)
{
}

PrismSecondaryUncontrolledFailoverAgent::~PrismSecondaryUncontrolledFailoverAgent ()
{
}

FrameworkObjectManagerFailoverReason PrismSecondaryUncontrolledFailoverAgent::getFailoverReason () const
{
    return (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);
}

void PrismSecondaryUncontrolledFailoverAgent::execute (PrismFailoverAgentContext *pPrismFailoverAgentContext)
{
    WaveNs::PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::resetThisLocationToPrimaryStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::initializePrismGlobalServicesDuringPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::enablePrismGlobalServicesDuringPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::bootPrismGlobalServicesDuringPrePhaseStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::initializePrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::enablePrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::listenForEventsPrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::bootPrismGlobalServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::pausePersistenceForPrimaryChangeStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::informSecondaryLocationsToChangePrimaryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::resumeDatabaseDumpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::backendSyncUpStep),

        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::broadcastPrimaryChangedEventForPlugins),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::failoverPrismServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::startHeartBeatToSecondariesStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::savePrismConfiguration), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::failoverForUnknownWaveNodePrismServicesStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismSecondaryUncontrolledFailoverAgent::prismLinearSequencerFailedStep)
    };

    PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext = new PrismFailoverAgentSequencerContext (pPrismFailoverAgentContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismFailoverAgentSequencerContext->setFailedLocationIds                    (pPrismFailoverAgentContext->getFailedLocationIds());
    pPrismFailoverAgentSequencerContext->setServiceToBeIgnored                   (pPrismFailoverAgentContext->getServiceToBeIgnored());
    pPrismFailoverAgentSequencerContext->setIsPrincipalChangedWithThisFailover   (true);
    pPrismFailoverAgentSequencerContext->setAnyConfigurationChangeTrackingNumber (PersistenceLocalObjectManager::getAnyConfigurationChangeTrackingNumber ());
    pPrismFailoverAgentSequencerContext->setOlderPrimaryLocationId               (FrameworkToolKit::getClusterPrimaryLocationId());

    trace (TRACE_LEVEL_DEBUG, string("PrismSecondaryUncontrolledFailoverAgent::execute: set OlderPrimaryLocationId = ") + FrameworkToolKit::getClusterPrimaryLocationId());

    pPrismFailoverAgentSequencerContext->holdAll ();
    pPrismFailoverAgentSequencerContext->start ();
}

bool PrismSecondaryUncontrolledFailoverAgent::isToBeExcludedForFailover (const PrismServiceId &prismServiceId)
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

WaveBootReason PrismSecondaryUncontrolledFailoverAgent::getBootReason () const
{
    return (WAVE_BOOT_SECONDARY_UNCONTROLLED_FAILOVER_BOOT);
}

}
