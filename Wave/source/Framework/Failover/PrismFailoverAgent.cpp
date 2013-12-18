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

#include "Framework/Failover/PrismFailoverAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"
#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Framework/Failover/PrismFailoverAgentContext.h"
#include "Framework/Failover/PrismFailoverAgentSequencerContext.h"
#include "Framework/Database/DatabaseObjectManagerBackupMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/PrimaryChangedEvent.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Core/WaveNodeBackendSyncUpAgent.h"

#include <string>
#include <set>
#include <fstream>
#include <algorithm>

using namespace std;

namespace WaveNs
{
static const UI32       s_primaryChangedPhase0TimeoutInMilliseconds = 900000; // 15 * 60 * 1000
static const UI32       s_primaryChangedPhase1TimeoutInMilliseconds = 900000; // 15 * 60 * 1000

PrismFailoverAgent::PrismFailoverAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : WaveWorker (pWaveObjectManager),
      m_currentFrameworkSequenceGenerator (currentFrameworkSequenceGenerator)
{
}

PrismFailoverAgent::~PrismFailoverAgent ()
{
}

void PrismFailoverAgent::execute (PrismFailoverAgentContext *pPrismFailoverAgentContext)
{
    WaveNs::PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFailoverAgent::failoverPrismServicesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFailoverAgent::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PrismFailoverAgent::prismLinearSequencerFailedStep)
    };

    PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext = new PrismFailoverAgentSequencerContext (pPrismFailoverAgentContext, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismFailoverAgentSequencerContext->setFailedLocationIds (pPrismFailoverAgentContext->getFailedLocationIds());
    pPrismFailoverAgentSequencerContext->setServiceToBeIgnored (pPrismFailoverAgentContext->getServiceToBeIgnored());
    pPrismFailoverAgentSequencerContext->setIsConfigurationChange (pPrismFailoverAgentContext->getIsConfigurationChange ());

    pPrismFailoverAgentSequencerContext->holdAll ();
    pPrismFailoverAgentSequencerContext->start ();
}

void PrismFailoverAgent::failoverPrismServicesStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::failoverPrismServicesStep");
    vector<PrismServiceId> serviceIdsToFailover;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;
    vector<LocationId>     failedLocationIds       = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    //PrismServiceId         serviceToBeIgnored      = pPrismFailoverAgentContext->getServiceToBeIgnored ();
    UI32                   index                   = 0;
    bool                   isConfigurationChange   = pPrismFailoverAgentSequencerContext->getIsConfigurationChange ();
    bool                   isPrincipalChangedWithThisFailover = pPrismFailoverAgentSequencerContext->getIsPrincipalChangedWithThisFailover ();

    m_currentFrameworkSequenceGenerator.getFailoverSequence (serviceIdsToFailover);
    numberOfServices = serviceIdsToFailover.size ();

    trace (TRACE_LEVEL_DEBUG, string ("PrismFailoverAgent::failoverPrismServicesStep : Number of services : ") + numberOfServices);

    for (i = 0; i < numberOfServices; i++)
    {
        index = numberOfServices - 1 - i;

        //if ((false == (isToBeExcludedForFailover (serviceIdsToFailover[index]))) && (serviceToBeIgnored != serviceIdsToFailover[index]))
        if (false == (isToBeExcludedForFailover (serviceIdsToFailover[index])))
        {
            PrismFailoverObjectManagerMessage prismFailoverObjectManagerMessage (serviceIdsToFailover[index], getFailoverReason (), isPrincipalChangedWithThisFailover);

            (prismFailoverObjectManagerMessage.getFailedLocationIds ()) = failedLocationIds;

            prismFailoverObjectManagerMessage.setIsConfigurationChange (isConfigurationChange);
            
            ResourceId status = sendSynchronously (&prismFailoverObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::failoverPrismServicesStep : Could not Failover a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToFailover[index]));
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::failoverPrismServicesStep : Status                       : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
                return;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Failed-Over " + FrameworkToolKit::getServiceNameById (serviceIdsToFailover[index]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "Skipped " + FrameworkToolKit::getServiceNameById (serviceIdsToFailover[index]));
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::initializePrismGlobalServicesDuringPrePhaseStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::initializePrismGlobalServicesDuringPrePhaseStep Entering");

    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequenceDuringPrePhase (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();


    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (isToBeExcludedDuringPrePhase (serviceIdsToInitialize [i])))
        {
            PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getBootReason ());

            ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::initializePrismGlobalServicesDuringPrePhaseStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
            }

            status = prismInitializeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::initializePrismGlobalServicesDuringPrePhaseStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
            }
        }
    }    

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::enablePrismGlobalServicesDuringPrePhaseStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::enablePrismGlobalServicesDuringPrePhaseStep");

    vector<PrismServiceId> serviceIdsToEnableDuringPrePhase;
    UI32                   i                                 = 0;
    UI32                   numberOfServices                  = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnableDuringPrePhase);
    numberOfServices = serviceIdsToEnableDuringPrePhase.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (isToBeExcludedDuringPrePhase (serviceIdsToEnableDuringPrePhase[i])))
        {
            PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnableDuringPrePhase[i], getBootReason ());

            ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::enablePrismGlobalServicesStep : Could not Enable a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::enablePrismGlobalServicesStep : Status                     : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
                return;
            }
            else
            {
                status = prismEnableObjectManagerMessage.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, "PrismBootAgent::enablePrismGlobalServicesStep : Could not Enable a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
                    trace (TRACE_LEVEL_FATAL, "PrismBootAgent::enablePrismGlobalServicesStep : Status                     : " + FrameworkToolKit::localize (status));
                    prismAssert (false, __FILE__, __LINE__);
                    pPrismFailoverAgentSequencerContext->executeNextStep(status);
                    return;
                }

                trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "Skipped " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep Entering");

    vector<PrismServiceId> serviceIdsToEnableDuringPrePhase;
    UI32                   i                                 = 0;
    UI32                   numberOfServices                  = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnableDuringPrePhase);
    numberOfServices = serviceIdsToEnableDuringPrePhase.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (isToBeExcludedDuringPrePhase (serviceIdsToEnableDuringPrePhase[i])))
        {
            PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnableDuringPrePhase[i]);

            ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep : Could not send Listen for Event message a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
                trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep : Status                     : " + FrameworkToolKit::localize (status));

                prismAssert (false, __FILE__, __LINE__);

                pPrismFailoverAgentSequencerContext->executeNextStep(status);
                return;
            }
            else
            {
                status  = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, "PrismBootAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep : Could not send Listen for Event message a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
                    trace (TRACE_LEVEL_FATAL, "PrismBootAgent::listenForEventsPrismGlobalServicesDuringPrePhaseStep : Status                     : " + FrameworkToolKit::localize (status));

                    prismAssert (false, __FILE__, __LINE__);

                    pPrismFailoverAgentSequencerContext->executeNextStep(status);
                    return;
                }
                
                trace (TRACE_LEVEL_INFO, "Listen For Event Done " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "Skipped " + FrameworkToolKit::getServiceNameById (serviceIdsToEnableDuringPrePhase[i]));
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::bootPrismGlobalServicesDuringPrePhaseStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequenceDuringPrePhase (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == isToBeExcludedDuringPrePhase (serviceIdsToBoot[i]))
        {
            PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getBootReason ());

            ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::bootPrismGlobalServicesDuringPrePhaseStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            }

            status = prismBootObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::bootPrismGlobalServicesDuringPrePhaseStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
            }
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);      
}

void PrismFailoverAgent::initializePrismGlobalServicesStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequence (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == isToBeExcluded (serviceIdsToInitialize[i]))
        {
            PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getBootReason ());

            ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("mFailoverAgent::initializePrismGlobalServicesStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ",  Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
            }

            status = prismInitializeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::initializePrismGlobalServicesStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
            }
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::enablePrismGlobalServicesStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::enablePrismGlobalServicesStep");
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (isToBeExcluded (serviceIdsToEnable[i])))
        {
            PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnable[i], getBootReason ());

            ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::enablePrismGlobalServicesStep : Could not Enable a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::enablePrismGlobalServicesStep : Status                     : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
                return;
            }
            else
            {
                status = prismEnableObjectManagerMessage.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::enablePrismGlobalServicesStep : Could not Enable a service ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                    prismAssert (false, __FILE__, __LINE__);

                    pPrismFailoverAgentSequencerContext->executeNextStep(status);
                    return;
                }

                trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "Skipped " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::listenForEventsPrismGlobalServicesStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::listenForEventsPrismGlobalServicesStep");

    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (isToBeExcluded (serviceIdsToEnable[i])))
        {
            PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnable[i]);

            ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::listenForEventsPrismGlobalServicesStep : Could not send a Message for Listen For Event to a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
                trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::listenForEventsPrismGlobalServicesStep : Status                     : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
                return;
            }
            else
            {
                status = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::listenForEventsPrismGlobalServicesStep : Listen for Event failed to Completion for service :") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                    prismAssert (false, __FILE__, __LINE__);

                    pPrismFailoverAgentSequencerContext->executeNextStep(status);
                    return;
                }

                trace (TRACE_LEVEL_INFO, "Listen For Service " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "Skipped " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::bootPrismGlobalServicesStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (isToBeExcluded (serviceIdsToBoot[i])))
        {
            PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getBootReason ());

            ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::bootPrismGlobalServicesStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
            }

            status = prismBootObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::bootPrismGlobalServicesStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
            }
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}


void PrismFailoverAgent::removeOldPrimaryLocationStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
         trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::removeOldPrimaryLocationStep");
         //If it is a controlled failover set the flag is the message
         if(FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == getFailoverReason ()) 
         {
             PrismFrameworkObjectManager *pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
             prismAssert (NULL != pPrismFrameworkObjectManager, __FILE__, __LINE__);

             LocationId oldPrimaryLocationId = pPrismFailoverAgentSequencerContext->getOlderPrimaryLocationId ();
             pPrismFrameworkObjectManager->removeKnownLocation (oldPrimaryLocationId);
             tracePrintf (TRACE_LEVEL_INFO, "PrismFaioverAgent::removeOldPrimaryLocationStep::Removing the old primary location id %u due to controlled failover", oldPrimaryLocationId);
            
         }
         pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::resetThisLocationToPrimaryStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::resetThisLocationToPrimaryStep");
    PrismFrameworkObjectManager *pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();

    prismAssert (NULL != pPrismFrameworkObjectManager, __FILE__, __LINE__);

    

    pPrismFrameworkObjectManager->resetLocationToPrimary ();

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

bool PrismFailoverAgent::isToBeExcludedForFailover (const PrismServiceId &prismServiceId)
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

void PrismFailoverAgent::createDatabaseDump ()
{
    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName ()); 

    DatabaseObjectManagerBackupMessage message (backupFileName);

    message.setDataOnlyBackup (true);

    ResourceId                         status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {    
        trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::createDatabaseDump : Could not send message to backup database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::createDatabaseDump : Message to backup database failed.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }    
        else 
        {    
             trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::createDatabaseDump : Successfully backed up the database.");
        }    
    }
}

void PrismFailoverAgent::pausePersistenceForPrimaryChangeStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    pausePersistence ();

    pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryStep");
    vector<LocationId>  connectedLocationIds;
    UI32                numberOfConnectedLocations = 0;
    UI32                i                          = 0;
    LocationId          thisLocationId             = FrameworkToolKit::getThisLocationId ();
    string              thisLocationIpAddress      = FrameworkToolKit::getThisLocationIpAddress ();
    UI32                thisLocationPort           = FrameworkToolKit::getThisLocationPort ();
    LocationId          locationId                 = FrameworkToolKit::getThisLocationId ();
    string              ipAddress                  = FrameworkToolKit::getThisLocationIpAddress ();
    UI32                port                       = FrameworkToolKit::getThisLocationPort ();

    vector<LocationId>  failedLocationIds          = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    UI32                j                          = 0;
    UI32                found                      = 0;

    FrameworkToolKit::getConnectedLocations (connectedLocationIds);
    numberOfConnectedLocations = connectedLocationIds.size ();

    pPrismFailoverAgentSequencerContext->setConnectedLocationVector (connectedLocationIds);

    ++(*pPrismFailoverAgentSequencerContext);
    
    for (i = 0; i < numberOfConnectedLocations; i++)
    {
        found = 0;
        locationId = connectedLocationIds[i];
        ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryStep : Processing Location: ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[0] + string ("."));

        for (j = 0; j < failedLocationIds.size(); j++) 
        {
            if(failedLocationIds[j] == locationId) 
            {
                trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryStep : Do not Inform Location ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));
                found = 1;
                break;
            }
        }

        if(found) 
        {
            continue;
        }

        FrameworkObjectManagerPrimaryChangedMessage *pMessage = new FrameworkObjectManagerPrimaryChangedMessage(thisLocationId, thisLocationIpAddress, thisLocationPort);

        pMessage->setAnyConfigurationChangeTrackingNumber (pPrismFailoverAgentSequencerContext->getAnyConfigurationChangeTrackingNumber ());
        pMessage->setSchemaChangeVectors ();

         //If it is a controlled failover set the flag is the message
        if(FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == getFailoverReason ()) 
        {
            pMessage->setPrimaryChangeDueToControlledFailover ();
        }

        WaveMessageStatus status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFailoverAgent::informSecondaryLocationsToChangePrimaryCallback), pPrismFailoverAgentSequencerContext, s_primaryChangedPhase0TimeoutInMilliseconds, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryStep: Not able to inform secondary location : ") + locationId + string (" Error Code : ") + status);
            pPrismFailoverAgentSequencerContext->setSecondaryNodeStatus (ipAddress, port, status);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
        else
        {
             ++(*pPrismFailoverAgentSequencerContext);
        }

    }

    createDatabaseDump ();

    --(*pPrismFailoverAgentSequencerContext);

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        /* We need to advance to next steps here as all sends may be failed or completed within the same stack */
        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryCallback");
    PrismFailoverAgentSequencerContext  *pPrismFailoverAgentSequencerContext = reinterpret_cast<PrismFailoverAgentSequencerContext *> (pContext);
    LocationId                           locationId;
    string                               ipAddress;
    SI32                                 port;

    --(*pPrismFailoverAgentSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();
        locationId                  = pPrismMessage->getReceiverLocationId ();
        ipAddress                   = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port                        = FrameworkToolKit::getPortForLocationId (locationId);

        pPrismFailoverAgentSequencerContext->setSecondaryNodeStatus (ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryCallback : Case 1 - Informing ") + ipAddress + " failed : " + completionStatus);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
    }
    else
    {
        if (NULL != pPrismMessage)
        {
            locationId = pPrismMessage->getReceiverLocationId ();
            ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            port       = FrameworkToolKit::getPortForLocationId (locationId);

            pPrismFailoverAgentSequencerContext->setSecondaryNodeStatus (ipAddress, port, frameworkStatus);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);

            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryCallback : Case 2 - Informing ") + ipAddress + " failed : " + frameworkStatus);
        }
    }

    if (NULL != pPrismMessage)
    {
        delete pPrismMessage;
    }

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        LocationId          locationId1 = 0;
        string              ipAddress1  = "";
        SI32                port1       = 0;
        vector<LocationId>  connectedLocationIds;
        UI32                numberOfConnectedLocations = 0;

        pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
        numberOfConnectedLocations = connectedLocationIds.size ();
    
        for (UI32 i = 0; i < numberOfConnectedLocations; i++)
        {
            locationId1 = connectedLocationIds[i];
            ipAddress1  = FrameworkToolKit::getIpAddressForLocationId (locationId1);
            port1       = FrameworkToolKit::getPortForLocationId      (locationId1);

            if (true != (pPrismFailoverAgentSequencerContext->isSecondaryNodeStatusSet (ipAddress1, port1)))
            {
                pPrismFailoverAgentSequencerContext->setSecondaryNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_TIMED_OUT);
                (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId1);

                trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryCallback : Case 3 - Informing ") + ipAddress1 + " Timeout ");
            }
        }

        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step: ");
    vector<LocationId>  connectedLocationIds;
    UI32                numberOfConnectedLocations = 0;

    vector<LocationId>  failedLocationIds          = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    UI32                found                      = 0;

    ifstream            databaseBackupFile;
    SI32                sizeOfBackupFile           = 0;
    char               *pDatabaseBackupBuffer      = NULL;
    string backupFileName = string (FrameworkToolKit::getProcessInitialWorkingDirectory () + "/" + FrameworkToolKit::getDatabaseBackupFileName ());

    // Collect the Database backup buffer to be sent over to all of the known and connected secondaries.

    databaseBackupFile.open (backupFileName.c_str (), ios::binary);
    databaseBackupFile.seekg (0, ios::end);
    sizeOfBackupFile = databaseBackupFile.tellg ();

    prismAssert (0 != sizeOfBackupFile, __FILE__, __LINE__);

    if (0 < sizeOfBackupFile)
    {
        pDatabaseBackupBuffer = new char[sizeOfBackupFile];

        prismAssert (NULL != pDatabaseBackupBuffer, __FILE__, __LINE__);

        databaseBackupFile.seekg (0, ios::beg);
        databaseBackupFile.read (pDatabaseBackupBuffer, sizeOfBackupFile);
    }

    databaseBackupFile.close ();

    pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
    numberOfConnectedLocations = connectedLocationIds.size ();

    ++(*pPrismFailoverAgentSequencerContext);

    for (UI32 i = 0; i < numberOfConnectedLocations; i++)
    {
        found = 0;
        LocationId  locationId = connectedLocationIds[i];
        string      ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        UI32        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step : Processing Location: ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[0] + string ("."));


        // We will send primary changed phase1 message only to the locations that are known to us at this point.
        // Only to those locations that are succeeded as part of last phase.

        if (WAVE_MESSAGE_SUCCESS != (pPrismFailoverAgentSequencerContext->getSecondaryNodeStatus (ipAddress, port)))
        {
            trace (TRACE_LEVEL_DEVEL, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step : Skipping failed secondary Location : ") + ipAddress + string (":") + port + string (":") + locationId + string ("."));
            continue;
        }

        for (UI32 j = 0; j < failedLocationIds.size(); j++) 
        {
            if(failedLocationIds[j] == locationId) 
            {
                trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step : Do not Inform Location ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));
                found = 1;
                break;
            }
        }

        if(found) 
        {
            continue;
        }

        FrameworkObjectManagerPrimaryChangedPhase1Message *pMessage = new FrameworkObjectManagerPrimaryChangedPhase1Message();

        pMessage->setAnyConfigurationChangeTrackingNumber (pPrismFailoverAgentSequencerContext->getAnyConfigurationChangeTrackingNumber ());

        if (NULL != pDatabaseBackupBuffer)
        {
            pMessage->addBuffer (pMessage->getDatabaseBackupBufferTag (), sizeOfBackupFile, pDatabaseBackupBuffer, false);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step : No Database Backup is being sent to the lcoation.");
        }

        ResourceId status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Callback), pPrismFailoverAgentSequencerContext, s_primaryChangedPhase1TimeoutInMilliseconds, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Step: Not able to inform secondary location : ") + locationId + string (" Error Code : ") + status);
            pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, status);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
        else
        {
             pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_PHASE1_UNKNOWN);
             ++(*pPrismFailoverAgentSequencerContext);
        }

    }

    --(*pPrismFailoverAgentSequencerContext);

    if (NULL != pDatabaseBackupBuffer)
    {
        delete [] pDatabaseBackupBuffer;
    }

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        /* We need to advance to next steps here as all sends may be failed or completed within the same stack */
        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Callback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Callback");

    PrismFailoverAgentSequencerContext  *pPrismFailoverAgentSequencerContext = reinterpret_cast<PrismFailoverAgentSequencerContext *> (pContext);
    LocationId                           locationId;
    string                               ipAddress;
    SI32                                 port;

    --(*pPrismFailoverAgentSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();
        locationId                  = pPrismMessage->getReceiverLocationId ();
        ipAddress                   = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port                        = FrameworkToolKit::getPortForLocationId (locationId);

        pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Callback : Case 1 - Informing ") + ipAddress + " failed : " + completionStatus);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
    }
    else
    {
        if (NULL != pPrismMessage)
        {
            locationId = pPrismMessage->getReceiverLocationId ();
            ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            port       = FrameworkToolKit::getPortForLocationId (locationId);

            pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, frameworkStatus);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);

            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Callback : Case 2 - Informing ") + ipAddress + " failed : " + frameworkStatus);
        }
    }

    if (NULL != pPrismMessage)
    {
        delete pPrismMessage;
    }

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        LocationId          locationId1 = 0;
        string              ipAddress1  = "";
        SI32                port1       = 0;
        vector<LocationId>  connectedLocationIds;
        UI32                numberOfConnectedLocations = 0;

        pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
        numberOfConnectedLocations = connectedLocationIds.size ();

        for (UI32 i = 0; i < numberOfConnectedLocations; i++)
        {
            locationId1 = connectedLocationIds[i];
            ipAddress1  = FrameworkToolKit::getIpAddressForLocationId (locationId1);
            port1       = FrameworkToolKit::getPortForLocationId      (locationId1);

            if (FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_PHASE1_UNKNOWN == (pPrismFailoverAgentSequencerContext->getSecondaryNodeStatus (ipAddress1, port1)))
            {
                pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_PHASE1_TIMED_OUT);
                (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId1);

                trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase1Callback : Case 3 - Informing ") + ipAddress1 + " Timeout ");
            }
        }

        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::resumeDatabaseDumpStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::resumeDatabaseDumpStep");
    PrismResumeObjectManagerMessage message (DatabaseObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::resumeDatabaseDumpStep : Could not resume Database.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::resumeDatabaseDumpStep : Could not resume Database.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::resumeDatabaseDumpStep : Successfully resumed Database.");
        }
    }

    resumePersistence ();

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step: ");
    vector<LocationId>  connectedLocationIds;
    UI32                numberOfConnectedLocations = 0;
    UI32                i                          = 0;

    vector<LocationId>  failedLocationIds          = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    UI32                j                          = 0;
    UI32                found                      = 0;

    pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
    numberOfConnectedLocations = connectedLocationIds.size ();

    ++(*pPrismFailoverAgentSequencerContext);
    
    for (i = 0; i < numberOfConnectedLocations; i++)
    {
        found = 0;
        LocationId  locationId = connectedLocationIds[i];
        string      ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        UI32        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step : Processing Location: ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));

        if (WAVE_MESSAGE_SUCCESS != (pPrismFailoverAgentSequencerContext->getSecondaryNodeStatus (ipAddress, port)))
        {
            trace (TRACE_LEVEL_DEVEL, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step : Skipping failed secondary Location : ") + ipAddress + string (":") + port + string (":") + locationId + string ("."));
            continue;
        }

        for (j = 0; j < failedLocationIds.size(); j++) {
            if(failedLocationIds[j] == locationId) {
                trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step : Do not Inform Location ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));
                found = 1;
                break;
            }
        }
        
        if(found) {
            continue;
        }

        FrameworkObjectManagerPrimaryChangedPhase2Message *pMessage = new FrameworkObjectManagerPrimaryChangedPhase2Message();

        ResourceId status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Callback), pPrismFailoverAgentSequencerContext, 300000, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, status);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);

            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step: Not able to inform secondary location : ") + locationId + string (" Error Code : ") + status);
        }
        else
        {
            pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_PHASE2_UNKNOWN);
            ++(*pPrismFailoverAgentSequencerContext);
        }

    }
    
    --(*pPrismFailoverAgentSequencerContext);

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        /* We need to advance to next steps here as all sends may be failed or completed within the same stack */
        trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Step: go next step ");
        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Callback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Callback");

    PrismFailoverAgentSequencerContext  *pPrismFailoverAgentSequencerContext = reinterpret_cast<PrismFailoverAgentSequencerContext *> (pContext);
    LocationId                           locationId;
    string                               ipAddress;
    SI32                                 port;

    --(*pPrismFailoverAgentSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();
        locationId                  = pPrismMessage->getReceiverLocationId ();
        ipAddress                   = FrameworkToolKit::getIpAddressForLocationId (locationId);
        port                        = FrameworkToolKit::getPortForLocationId (locationId);

        pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, completionStatus);

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Callback : Case 1 - Informing ") + ipAddress + " failed : " + completionStatus);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
    }
    else
    {
        if (NULL != pPrismMessage)
        {
            locationId = pPrismMessage->getReceiverLocationId ();
            ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            port       = FrameworkToolKit::getPortForLocationId (locationId);

            pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress, port, frameworkStatus);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);

            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Callback : Case 2 - Informing ") + ipAddress + " failed : " + frameworkStatus);
        }
    }

    if (NULL != pPrismMessage)
    {
        delete pPrismMessage;
    }

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        LocationId          locationId1 = 0;
        string              ipAddress1  = "";
        SI32                port1       = 0;
        vector<LocationId>  connectedLocationIds;
        UI32                numberOfConnectedLocations = 0;

        pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
        numberOfConnectedLocations = connectedLocationIds.size ();

        for (UI32 i = 0; i < numberOfConnectedLocations; i++)
        {
            locationId1 = connectedLocationIds[i];
            ipAddress1  = FrameworkToolKit::getIpAddressForLocationId (locationId1);
            port1       = FrameworkToolKit::getPortForLocationId      (locationId1);

            if (FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_PHASE2_UNKNOWN == (pPrismFailoverAgentSequencerContext->getSecondaryNodeStatus (ipAddress1, port1)))
            {
                (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId1);
                pPrismFailoverAgentSequencerContext->updateSecondaryNodeStatus (ipAddress1, port1, FRAMEWORK_ERROR_FAILOVER_INFORM_SECONDARY_LOCATIONS_PHASE2_TIMED_OUT);
                trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase2Callback : Case 3 - Informing ") + ipAddress1 + " Timeout ");
            }
        }

        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step: ");

    vector<LocationId>  connectedLocationIds;
    UI32                numberOfConnectedLocations = 0;
    UI32                i                          = 0;

    vector<LocationId>  failedLocationIds          = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    UI32                j                          = 0;
    UI32                found                      = 0;

    pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
    numberOfConnectedLocations = connectedLocationIds.size ();

    ++(*pPrismFailoverAgentSequencerContext);

    for (i = 0; i < numberOfConnectedLocations; i++)
    {
        found = 0;
        LocationId  locationId = connectedLocationIds[i];
        string      ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        UI32        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step : Processing Location: ") + ipAddress + string (":") + port + string (":") + locationId);

        if (WAVE_MESSAGE_SUCCESS != (pPrismFailoverAgentSequencerContext->getSecondaryNodeStatus (ipAddress, port)))
        {
            trace (TRACE_LEVEL_DEVEL, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step : Skipping failed secondary Location : ") + ipAddress + string (":") + port + string (":") + locationId + string ("."));
            continue;
        }

        for (j = 0; j < failedLocationIds.size(); j++) 
        {
            if(failedLocationIds[j] == locationId) 
            {
                trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step : Do not Inform Location ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));
                found = 1;
                break;
            }
        }

        if(found) 
        {
            continue;
        }

        FrameworkObjectManagerPrimaryChangedPhase3Message *pMessage = new FrameworkObjectManagerPrimaryChangedPhase3Message();

        ResourceId status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Callback), pPrismFailoverAgentSequencerContext, 0, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step: Not able to inform secondary location : ") + locationId + string (" Error Code : ") + status);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
        else
        {
             ++(*pPrismFailoverAgentSequencerContext);
        }

    }

    --(*pPrismFailoverAgentSequencerContext);

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        /* We need to advance to next steps here as all sends may be failed or completed within the same stack */
        trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Step: go next step ");
        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Callback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::informSecondaryLocationsToChangePrimaryPhase3Callback");

    PrismFailoverAgentSequencerContext  *pPrismFailoverAgentSequencerContext = reinterpret_cast<PrismFailoverAgentSequencerContext *> (pContext);

    --(*pPrismFailoverAgentSequencerContext);

	prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();
        LocationId locationId       = pPrismMessage->getReceiverLocationId ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
        }
    }
    else
    {
        LocationId  locationId = pPrismMessage->getReceiverLocationId ();

        (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);
    }

    if (NULL != pPrismMessage)
    {
        delete pPrismMessage;
    }

    if (0 == (pPrismFailoverAgentSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }
} 

void PrismFailoverAgent::broadcastPrimaryChangedEventForPlugins (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::broadcastPrimaryChangedEventForPlugins Entering....");

    PrimaryChangedEvent *pPrimaryChangedEvent = new PrimaryChangedEvent ();
    LocationId           newPrimaryLocationId = FrameworkToolKit::getClusterPrimaryLocationId ();

    pPrimaryChangedEvent->setNewPrimaryLocationId (newPrimaryLocationId);

    broadcast (pPrimaryChangedEvent);

    trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::broadcastPrimaryChangedEventForPlugins : broadcasted an event PrimaryChangedEvent with  New Primary LocationId is ") + newPrimaryLocationId);
    pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS); 
}

void PrismFailoverAgent::savePrismConfiguration (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    FrameworkToolKit::savePrismConfiguration(true);

    pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);    
}

void PrismFailoverAgent::startHeartBeatToSecondariesStep(PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
   trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::startHeartBeatToSecondariesStep: ");

    vector<LocationId>              knownLocationIds;
    vector<LocationId>              failedLocationIds = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    WaveNode                        *pNode    = NULL;
    set<LocationId>                 locationIdSet;
    set<LocationId>::iterator       it;
    string                          ipAddress;
    UI32                            port = 0;
    UI32                            i = 0;

    FrameworkToolKit::getKnownLocations (knownLocationIds);

    UI32 numberOfKnownLocations = knownLocationIds.size ();
    
    vector<WaveManagedObject *> *pResults = querySynchronously (WaveNode::getClassName ());

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32 totalWaveNodes = pResults->size ();

    if ((totalWaveNodes - 1)!= numberOfKnownLocations)
    {
        trace (TRACE_LEVEL_WARN, string ("PrismFailoverAgent::startHeartBeatToSecondariesStep: Total wave nodes (") + totalWaveNodes + string (") does not match with the known locations (") + numberOfKnownLocations + string (")"));
    }

    if (1 > totalWaveNodes)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::startHeartBeatToSecondariesStep: Wave node does not have itself in the database");
        prismAssert (false, __FILE__, __LINE__);
    }

    //Perform a union of locationId from known location and wave node 
    //set will not allow duplicate entries, so it is ok to insert the same locationId twice
    for (i = 0; i < numberOfKnownLocations; i++)
    {
        locationIdSet.insert (knownLocationIds[i]);
    }

    vector<LocationId> unknownWaveNodeLocationIds;

    for (i = 0; i < totalWaveNodes; i++)
    {
        pNode       = dynamic_cast<WaveNode *> ((*pResults)[i]);

        LocationId  waveNodeLocationId = pNode->getLocationId ();
        bool        foundWaveNode = false;

        if (waveNodeLocationId != FrameworkToolKit::getThisLocationId ())
        {
            for (UI32 n = 0; n < numberOfKnownLocations; n++)
            {
                if (waveNodeLocationId == knownLocationIds [n])
                {
                    foundWaveNode = true;
                    break;
                }
            }

            if (false == foundWaveNode)
            {
                unknownWaveNodeLocationIds.push_back (waveNodeLocationId);
            }
        }
    }

    LocationId locationId = 0;    

    for (it = locationIdSet.begin (); it != locationIdSet.end (); it++)
    {
        bool found = false;
        
        locationId = *it;

        //If the locationId corresponds to itself do not start heart beat

        if (locationId == FrameworkToolKit::getThisLocationId ())
        {
            continue;
        }

        //If the location is in the failed location vector, do not start the heart beat

        for (UI32 j = 0; j < failedLocationIds.size(); j++) 
        {
            if(failedLocationIds[j] == locationId) 
            {
                trace (TRACE_LEVEL_INFO, string("PrismFailoverAgent::startHeartBeatToSecondariesStep: Do not start Heartbeat with Location ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));
    
                found = 1;
                break;
            }
        }

        if(found) 
        {
            continue;
        }

        if (FrameworkToolKit::isAKnownLocation (locationId))
        {
            ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            port       = FrameworkToolKit::getPortForLocationId      (locationId);
        }
        else
        {
            prismAssert (false , __FILE__, __LINE__);
        }
        

        StartHeartBeatMessage* pMessage = new StartHeartBeatMessage(); 
        pMessage->setDstIpAddress               (ipAddress);
        pMessage->setDstPortNumber              (port);
        pMessage->setHeartBeatInterval          (10000);
        pMessage->setHeartBeatFailureThreshold  (3);

        ResourceId status = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
                  trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::startHeartBeatToSecondaryNodes: Could not start heart beating with Node : ") + ipAddress + string (", Port : ") + port + string (", Status = ") + FrameworkToolKit::localize (status));

                  prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
           status = pMessage->getCompletionStatus ();

           if (WAVE_MESSAGE_SUCCESS != status)
           {
                      trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::startHeartBeatToSecondaryNodes : Could not start heart beating with Node : ") + ipAddress + string (", Port : ") + port + string (", Completion Status = ") + FrameworkToolKit::localize (status));
                      prismAssert (false, __FILE__, __LINE__);
           }

           trace (TRACE_LEVEL_INFO, string ("PrismFailoverAgent::startHeartBeatToSecondaryNodes :Started HeartBeat with node ") + ipAddress + string (":") + port);
        }

              delete pMessage;
      }
  
    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
 
    // Disconnect from the node which got connected between the InformSecondaryLocationPhase 0 and Starting HeartBeat to secondary nodes.
    vector<LocationId>              connectedLocationIds;
    vector<LocationId>              actuallyConnectedLocationIds;
    UI32                            numberOfActuallyConnectedLocations;
    vector<LocationId>::iterator    locationIterator;

    pPrismFailoverAgentSequencerContext->getConnectedLocationVector (connectedLocationIds);
    FrameworkToolKit::getConnectedLocations (actuallyConnectedLocationIds);

    numberOfActuallyConnectedLocations = actuallyConnectedLocationIds.size ();

    for (UI32 location = 0; location < numberOfActuallyConnectedLocations; location ++)
    {
        locationIterator = find (connectedLocationIds.begin (), connectedLocationIds.end (), actuallyConnectedLocationIds [location]);
    
        if (locationIterator == connectedLocationIds.end ())
        {
            trace (TRACE_LEVEL_INFO, string ("disconnect from location in failover") + actuallyConnectedLocationIds [location]);
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (actuallyConnectedLocationIds [location]);            
        }
    }

    // set the uknown location to trigger the failover in the next step

    pPrismFailoverAgentSequencerContext->setUnknownWaveNodeLocationIds (unknownWaveNodeLocationIds);

    trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::startHeartBeatToSecondaryNodes: Exit ..");
    pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);


}

void PrismFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep: ");
    vector<LocationId>  connectedLocationIds;
    UI32                numberOfConnectedLocations = 0;
    UI32                i                          = 0;

    vector<LocationId>  failedLocationIds          = pPrismFailoverAgentSequencerContext->getFailedLocationIds ();
    UI32                j                          = 0;
    UI32                found                      = 0;

    pPrismFailoverAgentSequencerContext->getConnectedLocationVector(connectedLocationIds);
    numberOfConnectedLocations = connectedLocationIds.size ();

    
    for (i = 0; i < numberOfConnectedLocations; i++)
    {
        found = 0;
        LocationId  locationId = connectedLocationIds[i];
        string      ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
        UI32        port       = FrameworkToolKit::getPortForLocationId      (locationId);

        trace (TRACE_LEVEL_DEBUG, string("PrismFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep : Processing Location: ") + ipAddress + string (":") + port + string (":") + locationId + string (".")); 

        if (WAVE_MESSAGE_SUCCESS != (pPrismFailoverAgentSequencerContext->getSecondaryNodeStatus (ipAddress, port)))
        {
            trace (TRACE_LEVEL_DEVEL, string("PrismFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep : Skipping failed secondary Location : ") + ipAddress + string (":") + port + string (":") + locationId + string ("."));
            continue;
        }

        for (j = 0; j < failedLocationIds.size(); j++) {
            if(failedLocationIds[j] == locationId) {
                trace (TRACE_LEVEL_DEBUG, string("PrismFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep : Do not Inform Location ") + ipAddress + string (":") + port + string (":") + locationId + string(" Failed Loc: ") + failedLocationIds[j] + string ("."));
                found = 1;
                break;
            }
        }
        
        if(found) {
            continue;
        }

        FrameworkObjectManagerRemoveKnownLocationsMessage *pMessage = new FrameworkObjectManagerRemoveKnownLocationsMessage();

        pMessage->setFailedLocations (failedLocationIds);
        ResourceId status = sendOneWay (pMessage, locationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismFailoverAgent::informSecondaryLocationsToRemoveFailedFromKnownLocationsStep: Not able to inform secondary location : ") + locationId + string (" Error Code : ") + status);
        }

    }
    

    pPrismFailoverAgentSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

bool PrismFailoverAgent::isToBeExcluded (const PrismServiceId &prismServiceId)
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

bool PrismFailoverAgent::isToBeExcludedDuringPrePhase (const PrismServiceId &prismServiceId)
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

void PrismFailoverAgent::pausePersistence ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::pausePersistence : Starting ...");

    PrismPauseObjectManagerMessage message (PersistenceObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::pausePersistence : Could not pause Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::pausePersistence : Could not pause Persistence.  Completion Status : " + FrameworkToolKit::localize (status));            
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::pausePersistence : Successfully paused Persistence.");
        }
    }
}

void PrismFailoverAgent::resumePersistence ()
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::resumePersistence : Starting ...");

    PrismResumeObjectManagerMessage message (PersistenceObjectManager::getPrismServiceId ());
    ResourceId                      status  = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::resumePersistence : Could not resume Persistence.  Status : " + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismFailoverAgent::resumePersistence : Could not resume Persistence.  Completion Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::resumePersistence : Successfully resumed Persistence.");
        }
    }
}

void PrismFailoverAgent::failoverForUnknownWaveNodePrismServicesStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PrismFailoverAgent::failoverPrismServicesStep");
    vector<PrismServiceId> serviceIdsToFailover;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;
    vector<LocationId>     failedLocationIds       = pPrismFailoverAgentSequencerContext->getUnknownWaveNodeLocationIds ();
    //PrismServiceId         serviceToBeIgnored      = pPrismFailoverAgentContext->getServiceToBeIgnored ();
    UI32                   index                   = 0;
    bool                   isConfigurationChange   = pPrismFailoverAgentSequencerContext->getIsConfigurationChange ();
    bool                   isPrincipalChangedWithThisFailover = false;

    if (0 == failedLocationIds.size ())
    {
        pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

        return;
    }

    m_currentFrameworkSequenceGenerator.getFailoverSequence (serviceIdsToFailover);
    numberOfServices = serviceIdsToFailover.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        index = numberOfServices - 1 - i;

        //if ((false == (isToBeExcludedForFailover (serviceIdsToFailover[index]))) && (serviceToBeIgnored != serviceIdsToFailover[index]))
        if (false == (isToBeExcludedForFailover (serviceIdsToFailover[index])))
        {
            PrismFailoverObjectManagerMessage prismFailoverObjectManagerMessage (serviceIdsToFailover[index], FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED, isPrincipalChangedWithThisFailover);

            (prismFailoverObjectManagerMessage.getFailedLocationIds ()) = failedLocationIds;

            prismFailoverObjectManagerMessage.setIsConfigurationChange (isConfigurationChange);

            ResourceId status = sendSynchronously (&prismFailoverObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::failoverPrismServicesStep : Could not Failover a service : " + FrameworkToolKit::getServiceNameById (serviceIdsToFailover[index]));
                trace (TRACE_LEVEL_FATAL, "PrismBootAgent::failoverPrismServicesStep : Status                       : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
                pPrismFailoverAgentSequencerContext->executeNextStep(status);
                return;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "failoverForUnknownWaveNodePrismServicesStep Failed-Over " + FrameworkToolKit::getServiceNameById (serviceIdsToFailover[index]));
            }
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "Skipped " + FrameworkToolKit::getServiceNameById (serviceIdsToFailover[index]));
        }
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void PrismFailoverAgent::backendSyncUpStep (PrismFailoverAgentSequencerContext *pPrismFailoverAgentSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismFailoverAgent::backendSyncUpStep: Send back end sync up to all plug-ins .... ");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveNodeBackendSyncUpAgent *pWaveNodeBackendSyncUpAgent = new WaveNodeBackendSyncUpAgent (m_pWaveObjectManager);

    status = pWaveNodeBackendSyncUpAgent->execute ();

    delete pWaveNodeBackendSyncUpAgent;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
       trace (TRACE_LEVEL_FATAL, string ("PrismFailoverAgent::backendSyncUpStep Failed : Status : ") + FrameworkToolKit::localize (status));
       prismAssert (false, __FILE__, __LINE__);
       pPrismFailoverAgentSequencerContext->executeNextStep(status);
    }

    pPrismFailoverAgentSequencerContext->executeNextStep(status);
    return;
}

}
