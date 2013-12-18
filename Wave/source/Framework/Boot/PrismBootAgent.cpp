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

#include "Framework/Boot/PrismBootAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Database/DatabaseStandaloneTransaction.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerInitializeWorker.h"
#include "Framework/Core/WaveNodeBackendSyncUpAgent.h"

namespace WaveNs
{

PrismBootAgent::PrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator)
    : WaveWorker (pWaveObjectManager),
      m_currentFrameworkSequenceGenerator (currentFrameworkSequenceGenerator)
{
}

PrismBootAgent::~PrismBootAgent ()
{
}

ResourceId PrismBootAgent::execute (const WaveBootPhase &waveBootPhase)
{
    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::initializePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::enablePrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::installPrismServicesDuringPrePhaseStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::bootPrismServicesDuringPrePhaseStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::initializeLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::enableLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::listenForEventsLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::installLocalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::bootLocalPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::initializeGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::enableGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::listenForEventsGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::installGlobalPrismServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::bootGlobalPrismServicesStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&PrismBootAgent::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (reinterpret_cast<PrismMessage *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pPrismSynchronousLinearSequencerContext->execute ();

    return (status);
}

ResourceId PrismBootAgent::initializePrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequenceDuringPrePhase (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == isToBeExcludedFromInitializeDuringPrePhase (serviceIdsToInitialize[i]))
        {
            continue;
        }

        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToInitialize[i])))
            {
                continue;
            }
        }
    
        PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getReason ());

        ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializePrismServicesDuringPrePhaseStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismInitializeObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializePrismServicesDuringPrePhaseStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }

        PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnable[i]);

        ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep : Could not send a message to Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsPrismServicesDuringPrePhaseStep : Could not Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Listen For Events " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::enablePrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }

        PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnable[i], getReason ());

        ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enablePrismServicesDuringPrePhaseStep : Could not send a message to Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismEnableObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enablePrismServicesDuringPrePhaseStep : Could not Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::installPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInstall;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInstallSequenceDuringPrePhase (serviceIdsToInstall);
    numberOfServices = serviceIdsToInstall.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToInstall[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedFromInstallDuringPrePhase (serviceIdsToInstall[i])))
        {
            continue;
        }

        PrismInstallObjectManagerMessage prismInstallObjectManagerMessage (serviceIdsToInstall[i], getReason ());

        ResourceId status = sendSynchronously (&prismInstallObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installPrismServicesDuringPrePhaseStep : Could not send a message to Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismInstallObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installPrismServicesDuringPrePhaseStep : Could not Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Installed " + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::bootPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequenceDuringPrePhase (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToBoot[i])))
        {
            continue;
        }

        PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getReason (), getRollBackFlag ());

        ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootPrismServicesDuringPrePhaseStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismBootObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootPrismServicesDuringPrePhaseStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::initializeGlobalPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequenceDuringPrePhase (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == isToBeExcludedFromInitializeDuringPrePhase (serviceIdsToInitialize[i]))
        {
            continue;
        }

        if (false == (FrameworkToolKit::isALocalService (serviceIdsToInitialize[i])))
        {
            PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getReason ());

            ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializeGlobalPrismServicesDuringPrePhaseStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ",  Status : " + FrameworkToolKit::localize (status));

                return (status);
            }

            status = prismInitializeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializeGlobalPrismServicesDuringPrePhaseStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));

                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
            }
        }
    }    

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::listenForEventsGlobalServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }

        if (false == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnable[i]);

            ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsGlobalServicesDuringPrePhaseStep : Could not send a message to Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsGlobalServicesDuringPrePhaseStep : Could not Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Listen For Events  " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::enableGlobalPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }

        if (false == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnable[i], getReason ());

            ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enableGlobalPrismServicesDuringPrePhaseStep : Could not send a message to Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismEnableObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enableGlobalPrismServicesDuringPrePhaseStep : Could not Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);

}

ResourceId PrismBootAgent::bootGlobalPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequenceDuringPrePhase (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToBoot[i])))
        {
            continue;
        }
        
        if (false == (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
        {
            PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getReason (), getRollBackFlag ());

            ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootGlobalPrismServicesDuringPrePhaseStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismBootObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootGlobalPrismServicesDuringPrePhaseStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
            }
        }        
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::initializeLocalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequence (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (isToBeExcludedFromInitializePhase (serviceIdsToInitialize[i])))
        {
            continue;
        }

        if (true == (isToBeExcludedFromCurrentBootPhase (serviceIdsToInitialize[i])))
        {
            continue;
        }

        if (true == (FrameworkToolKit::isALocalService (serviceIdsToInitialize[i])))
        {
            PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getReason ());

            ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializeLocalPrismServicesStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ",  Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismInitializeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializeLocalPrismServicesStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::initializeGlobalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequence (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (isToBeExcludedFromInitializePhase (serviceIdsToInitialize[i])))
        {
            continue;
        }

        if (false == (FrameworkToolKit::isALocalService (serviceIdsToInitialize[i])))
        {
            PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getReason ());

            ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializeGlobalPrismServicesStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ",  Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismInitializeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializeGlobalPrismServicesStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::initializePrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInitialize;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInitializeSequence (serviceIdsToInitialize);
    numberOfServices = serviceIdsToInitialize.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (isToBeExcludedFromInitializePhase (serviceIdsToInitialize[i])))
        {
            continue;
        }

        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToInitialize[i])))
            {
                continue;
            }
        }
    
        PrismInitializeObjectManagerMessage prismInitializeObjectManagerMessage (serviceIdsToInitialize[i], getReason ());

        ResourceId status = sendSynchronously (&prismInitializeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializePrismServicesStep : Could not send a message to Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ",  Status : " + FrameworkToolKit::localize (status));
            return (status);
        }

        status = prismInitializeObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::initializePrismServicesStep : Could not Initialize a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Initialized " + FrameworkToolKit::getServiceNameById (serviceIdsToInitialize[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::listenForEventsLocalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
            {
                continue;
            }

            PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnable[i]);

            ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsLocalPrismServicesStep : Could not send a message to Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsLocalPrismServicesStep : Could not Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Listen For Events " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}


ResourceId PrismBootAgent::enableLocalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
            {
                continue;
            }

            PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnable[i], getReason ());

            ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enableLocalPrismServicesStep : Could not send a message to Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismEnableObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enableLocalPrismServicesStep : Could not Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::listenForEventsGlobalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
            {
                continue;
            }

            PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnable[i]);

            ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsGlobalPrismServicesStep : Could not send a message to Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsGlobalPrismServicesStep : Could not Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Listen For Events  " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::enableGlobalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
            {
                continue;
            }

            PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnable[i], getReason ());

            ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enableGlobalPrismServicesStep : Could not send a message to Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismEnableObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enableGlobalPrismServicesStep : Could not Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::listenForEventsPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }

        // Excluding the Applicaiton service in phase 2 and framework service in phase 3
        if (true == (isToBeExcludedFromCurrentBootPhase (serviceIdsToEnable[i])))
        {
            continue;
        }

        PrismListenForEventsObjectManagerMessage prismListenForEventsObjectManagerMessage (serviceIdsToEnable[i]);

        ResourceId status = sendSynchronously (&prismListenForEventsObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsPrismServicesStep : Could not send a message to Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }

        status = prismListenForEventsObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::listenForEventsPrismServicesStep : Could not Listen For Events from a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Listen For Events " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::enablePrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }

        if (true == (isToBeExcludedFromCurrentBootPhase (serviceIdsToEnable[i])))
        {
            continue;
        }

        PrismEnableObjectManagerMessage prismEnableObjectManagerMessage (serviceIdsToEnable[i], getReason ());

        ResourceId status = sendSynchronously (&prismEnableObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enablePrismServicesStep : Could not send a message to Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }

        status = prismEnableObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::enablePrismServicesStep : Could not Enable a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Enabled " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::installLocalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInstall;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInstallSequence (serviceIdsToInstall);
    numberOfServices = serviceIdsToInstall.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (FrameworkToolKit::isALocalService (serviceIdsToInstall[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToInstall[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedFromInstall (serviceIdsToInstall[i])))
            {
                continue;
            }

            if (true == (isToBeExcludedFromCurrentBootPhase (serviceIdsToInstall[i])))
            {
                continue;
            }

            PrismInstallObjectManagerMessage prismInstallObjectManagerMessage (serviceIdsToInstall[i], getReason ());

            ResourceId status = sendSynchronously (&prismInstallObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installLocalPrismServicesStep : Could not send a message to Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismInstallObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installLocalPrismServicesStep : Could not Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Installed " + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::installGlobalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInstall;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInstallSequence (serviceIdsToInstall);
    numberOfServices = serviceIdsToInstall.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (FrameworkToolKit::isALocalService (serviceIdsToInstall[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToInstall[i])))
                {
                    continue;
                }
            }

           if (true == (isToBeExcludedFromInstall (serviceIdsToInstall[i])))
           {
               continue;
           }

            PrismInstallObjectManagerMessage prismInstallObjectManagerMessage (serviceIdsToInstall[i], getReason ());

            ResourceId status = sendSynchronously (&prismInstallObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installGlobalPrismServicesStep : Could not send a message to Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismInstallObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installGlobalPrismServicesStep : Could not Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Installed " + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::installPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToInstall;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getInstallSequence (serviceIdsToInstall);
    numberOfServices = serviceIdsToInstall.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToInstall[i])))
            {
                continue;
            }
        }

        PrismInstallObjectManagerMessage prismInstallObjectManagerMessage (serviceIdsToInstall[i], getReason ());

        ResourceId status = sendSynchronously (&prismInstallObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installPrismServicesStep : Could not send a message to Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }

        status = prismInstallObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::installPrismServicesStep : Could not Install a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Installed " + FrameworkToolKit::getServiceNameById (serviceIdsToInstall[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::bootLocalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToBoot[i])))
            {
                continue;
            }

            PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getReason (), getRollBackFlag ());

            ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootLocalPrismServicesStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismBootObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootLocalPrismServicesStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    if ((false == FrameworkToolKit::getDetectSchemaChange ()) && (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus))
    {   
        trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep : exiting as no need for ForeignKeyConstraints application.");
        return (WAVE_MESSAGE_SUCCESS);
    }

    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert(NULL != pOrmRepository, __FILE__, __LINE__);

    ManagedObjectSchemaInfoRepository & moSchemaInfoRepository  = pOrmRepository->getMOSchemaInfoRepository ();

    if (false == moSchemaInfoRepository.getIfUserDefinedKeyCombinationChanged ())
    {
        trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep: no changes for UserDefinedKeyCombination. exiting..");
        return (WAVE_MESSAGE_SUCCESS);
    }
    
    string alterSqlForCurrentSchema;
    string alterSqlForStartupSchema;
    string alterSql;
    
    // 1. Sql to update userDefinedKeyCombinationValue, KeyString and OwnerKeyString.
    moSchemaInfoRepository.getSqlToUpdateUserDefinedKeyCombinationForUpgrade (alterSqlForCurrentSchema, alterSqlForStartupSchema);

    trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep: sql to update UDKC values for current schema- \n" + alterSqlForCurrentSchema);

    alterSql = alterSqlForCurrentSchema + alterSqlForStartupSchema;

    ResourceId      status  = WAVE_MESSAGE_SUCCESS;
    
    if (alterSql != "")
    { 
        DatabaseObjectManagerExecuteTransactionMessage message (alterSql);
        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep : Could not send message to update values for udkc. Status : " + FrameworkToolKit::localize (status));
            //prismAssert (false, __FILE__, __LINE__);
        }
        else
        {   
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {   
                trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep:: failed to update udkc values. Status : " + FrameworkToolKit::localize (status));
                //prismAssert (false, __FILE__, __LINE__);
            }
            else
            {   
                trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep: successfully updated the corresponding udkc values.");
            }
        }
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        /* This indicates DB is not in proper state. remove cfg file and reboot and come back with First time boot */
        handleUpgradeFailure (getServiceId ());
    }

    // 2.Sql to apply the constraint.
    alterSql = alterSqlForCurrentSchema = alterSqlForStartupSchema = "";

    moSchemaInfoRepository.getSqlToAddUserDefinedKeyUniqueConstraint (alterSqlForCurrentSchema, alterSqlForStartupSchema);

    trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep: sql to add udkc constraints for current schema- \n" + alterSqlForCurrentSchema);

    alterSql = alterSqlForCurrentSchema + alterSqlForStartupSchema;

    status  = WAVE_MESSAGE_SUCCESS;
    
    if (alterSql != "")
    { 
        DatabaseObjectManagerExecuteTransactionMessage message (alterSql);
        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep : Could not send message to add constraint for udkc. Status : " + FrameworkToolKit::localize (status));
            //prismAssert (false, __FILE__, __LINE__);
        }
        else
        {   
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {   
                trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateUserDefinedKeyCombinationConstraintsAndKeysStep:: failed to add udkc constraints. Status : " + FrameworkToolKit::localize (status));
                //prismAssert (false, __FILE__, __LINE__);
            }
            else
            {   
                trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep: successfully added udkc constraints.");
            }
        }
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        /* This indicates DB is not in proper state. remove cfg file and reboot and come back with First time boot */
        handleUpgradeFailure (getServiceId ());
    }

    return (status);
}

ResourceId PrismBootAgent::updateDatabaseForeignKeyConstraintsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    //  FrameworkToolKit::getDetectSchemaChange () : helps in identifying the Persistent boot during FWDL case. This flag will be false during "standby Schema conversion".
    //  During standby schema conversion, dbConversionStatus is set to NOT_ATTEMPTED in first step "configureStandbyValidateVersionStep" and will be set to 
    //  DATABASE_SCHEMA_CONVERSION_SUCCEEDED only after successful conversion of schema. Hence, (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus) check to have 
    //  ForeignKeyConstraint application on successful standby schema conversion.

    if ((false == FrameworkToolKit::getDetectSchemaChange ()) && (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus))
    {
        trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep : exiting as no need for ForeignKeyConstraints application.");
        return (WAVE_MESSAGE_SUCCESS);
    }

    string alterSqlForCurrentSchema;
    string alterSqlForStartupSchema;
    string alterSql;

    OrmRepository *pOrmRepository = OrmRepository::getInstance ();
    prismAssert(NULL != pOrmRepository, __FILE__, __LINE__);

    ManagedObjectSchemaInfoRepository & moSchemaInfoRepository  = pOrmRepository->getMOSchemaInfoRepository ();

    map<string, ModifiedManagedObjectSchemaDifference> modifiedTablesSchemaDifference = moSchemaInfoRepository.getModifiedTablesSchemaDifference ();
    map<string, ModifiedManagedObjectSchemaDifference>::iterator mIterator;

    trace (TRACE_LEVEL_DEBUG, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep : Number of Modified Managed Objects : " + modifiedTablesSchemaDifference.size ());

    for (mIterator = modifiedTablesSchemaDifference.begin(); mIterator != modifiedTablesSchemaDifference.end(); mIterator++)
    {   
        //(mIterator->second).printContentsForDebugging ();

        string modifiedManagedObjectName = mIterator->first;

        //Iterate through the vector of all added relationships 
        vector<RelationshipInfoFromSchemaDifference> addedRelationsVector = (mIterator->second).getAddedRelations ();
        vector<RelationshipInfoFromSchemaDifference>::const_iterator relationsIterator = addedRelationsVector.begin();

	    //---generate Alter statements to add NOT NULL constraints for 1:1 relationships 
        for(;relationsIterator != addedRelationsVector.end(); ++relationsIterator) 
        {
            if(ORM_RELATION_TYPE_ONE_TO_ONE == relationsIterator->getRelationType ()) 
            {
                OrmTable* pOrmTable = OrmRepository::getTableByName (modifiedManagedObjectName);
                alterSqlForCurrentSchema += pOrmTable->getAlterSqlToAddNotNullForOneToOneRelationship (OrmRepository::getWaveCurrentSchema() ,relationsIterator->getRelationName());
                alterSqlForStartupSchema += pOrmTable->getAlterSqlToAddNotNullForOneToOneRelationship (OrmRepository::getWaveStartSchema(), relationsIterator->getRelationName());
            }
        }

        //Iterate through the vector of all changed relationships 
        vector<RelationshipInfoFromSchemaDifference> changedRelationsVector = (mIterator->second).getChangedRelations ();
        vector<RelationshipInfoFromSchemaDifference>::const_iterator changedRelationsIterator = changedRelationsVector.begin();

	    //---generate Alter statements to add NOT NULL constraints for 1:1 relationships 
        for(;changedRelationsIterator != changedRelationsVector.end(); ++changedRelationsIterator) 
        {
            if (changedRelationsIterator->isCanBeEmptyChanged ())
            {
                if (changedRelationsIterator->getCanBeEmpty ())
                {
                    /* If canBeEmpty is true, Dropping NOT NULL is done during DB conversion only*/
                }
                else
                {
                    OrmTable* pOrmTable = OrmRepository::getTableByName (modifiedManagedObjectName);
                    alterSqlForCurrentSchema += pOrmTable->getAlterSqlToAddNotNullForOneToOneRelationship (OrmRepository::getWaveCurrentSchema() ,changedRelationsIterator->getRelationName());
                    alterSqlForStartupSchema += pOrmTable->getAlterSqlToAddNotNullForOneToOneRelationship (OrmRepository::getWaveStartSchema(), changedRelationsIterator->getRelationName());
                }
            }

            if (changedRelationsIterator->isDisableValidationChanged ())
            {
                trace (TRACE_LEVEL_INFO, "disableValidationChanged : " + changedRelationsIterator->isDisableValidationChanged ());
                if (changedRelationsIterator->getDisableValidations ())
                {
                    /* If Validations are disabled, we remove Foriegn Key Constraints during DB conversion only*/
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, "disableValidation : " + changedRelationsIterator->getDisableValidations ());
                    OrmTable* pOrmTable = OrmRepository::getTableByName (modifiedManagedObjectName);
                    alterSqlForCurrentSchema += pOrmTable->getAlterSqlToEnableValidationForOneToManyRelationship (OrmRepository::getWaveCurrentSchema() ,changedRelationsIterator->getRelationName());
                    alterSqlForStartupSchema += pOrmTable->getAlterSqlToEnableValidationForOneToManyRelationship (OrmRepository::getWaveStartSchema(), changedRelationsIterator->getRelationName());
                }
            }
        }
    }

    alterSql = alterSqlForCurrentSchema + alterSqlForStartupSchema;

    trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep : Alter SQL to add NOT NULL constraint is : " + alterSqlForCurrentSchema);

    ResourceId                                     status  = WAVE_MESSAGE_SUCCESS;
    
    if (alterSql != "")
    { 
        DatabaseObjectManagerExecuteTransactionMessage message (alterSql);
        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {   
            trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep : Could not send message to alter 1:1 relationship constraints. Status : " + FrameworkToolKit::localize (status));
            //prismAssert (false, __FILE__, __LINE__);
        }
        else
        {   
            status = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {   
                trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep:: failed to update 1:1 relationship constraints. Status : " + FrameworkToolKit::localize (status));
                //prismAssert (false, __FILE__, __LINE__);
            }
            else
            {   
                trace (TRACE_LEVEL_INFO, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep: updated 1:1 relationship constraints.");
            }
        }
        /*
        DatabaseStandaloneTransaction dbStandaloneTransaction;
        if (!dbStandaloneTransaction.execute (alterSql))
        {   
            trace (TRACE_LEVEL_ERROR, "PrismBootAgent::updateDatabaseForeignKeyConstraintsStep : Error in applying SQL to database");
            status = WAVE_MESSAGE_ERROR;
        }
        */
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {   
        /* This indicates DB is not in proper state. remove cfg file and reboot and come back with First time boot */
        handleUpgradeFailure (getServiceId ());
    }

    return (status);
}

ResourceId PrismBootAgent::bootGlobalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                if (true != (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
                {
                    continue;
                }
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToBoot[i])))
            {
                continue;
            }

            PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getReason (), getRollBackFlag ());

            ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootGlobalPrismServicesStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }

            status = prismBootObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootGlobalPrismServicesStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::bootPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    vector<PrismServiceId> serviceIdsToBoot;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIdsToBoot);
    numberOfServices = serviceIdsToBoot.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToBoot[i])))
            {
                continue;
            }
        }

        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToBoot[i])))
        {
            continue;
        }

        if (true == (isToBeExcludedFromCurrentBootPhase (serviceIdsToBoot[i])))
        {
            continue;
        }

        PrismBootObjectManagerMessage prismBootObjectManagerMessage (serviceIdsToBoot[i], getReason (), getRollBackFlag ());

        ResourceId status = sendSynchronously (&prismBootObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootPrismServicesStep : Could not send a message to Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }

        status = prismBootObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::bootPrismServicesStep : Could not Boot a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Booted " + FrameworkToolKit::getServiceNameById (serviceIdsToBoot[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId  PrismBootAgent::upgradePrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    if (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
    
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
            {
                continue;
            }
        }

        /* DO WE NEED THIS ?? SINCE UPGRADE IS CALLED ONLY FOR PERSISTENT SYSTEM BOOT UP CASE AND NOT THE CLUSTER BASED PHASES 
        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }*/

        PrismUpgradeObjectManagerMessage prismUpgradeObjectManagerMessage (serviceIdsToEnable[i], getReason ());

        ResourceId status = sendSynchronously (&prismUpgradeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradePrismServicesDuringPrePhaseStep : Could not send a message to Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismUpgradeObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradePrismServicesDuringPrePhaseStep : Could not Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Upgraded " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}
 
void PrismBootAgent::handleUpgradeFailure (const PrismServiceId &serviceId)
{
    trace (TRACE_LEVEL_WARN, "PrismBootAgent::handleUpgradeFailure: upgrade failed for service : [" + FrameworkToolKit::getServiceNameById (serviceId) + "]");

    // 1. Configuration file will be removed and 2. system will be rebooted. 
    // This will bring system in first time boot and configuration will be regained by config replay.");

    // 1.
    vector<string>  output;
    SI32            cmdStatus                  = 0;
    const string    prismConfigurationfileName = (PrismFrameworkObjectManager::getInstance ())->getConfigurationFileName ();

    trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleUpgradeFailure: deleting file " + prismConfigurationfileName);

    cmdStatus = FrameworkToolKit::systemCommandOutput ((string ("/bin/rm -rf ") + prismConfigurationfileName).c_str(), output);
    
    if (0 != cmdStatus)
    {
        if (0 < output.size())
        {
            trace (TRACE_LEVEL_ERROR, string("DatabaseObjectManager::handleUpgradeFailure: cmd to deleted file ")+ prismConfigurationfileName + string(" failed with error message : ") + output[0]);
        }
    }
    

    // 2.
    //Wave::logOperationStatus (REBOOT_FOR_SERVICE_UPGRADE_FAILURE_AUTO_RECOVERY);
    // As RAS OM will not be ready by now, cant use raslog here.
    FrameworkToolKit::initializeConsolePrintFunction ();
    FrameworkToolKit::consolePrint (string("System going down for auto recovery as upgrade has failed for [" + FrameworkToolKit::getServiceNameById (serviceId) + "]"));

    trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleUpgradeFailure: System going down for auto recovery as upgrade has failed for [" + FrameworkToolKit::getServiceNameById (serviceId) + "]");

    output.clear();
    FrameworkToolKit::systemCommandOutput ("/fabos/sbin/reboot -s -r AutoRecoveryForUpgradeFailure", output);
    prismSleep(30);

    trace (TRACE_LEVEL_WARN, "DatabaseObjectManager::handleUpgradeFailure: Reboot is triggered but system has not yet rebooted.");
    prismAssert (false, __FILE__, __LINE__);
}

ResourceId  PrismBootAgent::upgradeGlobalPrismServicesDuringPrePhaseStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    if (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequenceDuringPrePhase (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        /* DO WE NEED THIS ?? SINCE UPGRADE IS CALLED ONLY FOR PERSISTENT SYSTEM BOOT UP CASE AND NOT THE CLUSTER BASED PHASES 
        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }*/

        if (false == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            PrismUpgradeObjectManagerMessage prismUpgradeObjectManagerMessage (serviceIdsToEnable[i], getReason ());

            ResourceId status = sendSynchronously (&prismUpgradeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradeGlobalPrismServicesDuringPrePhaseStep : Could not send a message to Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                return (status);
            }
    
            status = prismUpgradeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradeGlobalPrismServicesDuringPrePhaseStep : Could not Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                if (false == FrameworkToolKit::isSchemaConversionDisabledByUser ())
                {
                    handleUpgradeFailure (serviceIdsToEnable[i]);
                }

                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Upgraded " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);

}
ResourceId  PrismBootAgent::upgradeLocalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    if (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus)
    {
        trace (TRACE_LEVEL_INFO, string("getDbConversionStatus") + FrameworkToolKit::localize (dbConversionStatus));
        return (WAVE_MESSAGE_SUCCESS);
    }

    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (true == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                    continue;
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
            {
                continue;
            }
        
            PrismUpgradeObjectManagerMessage prismUpgradeObjectManagerMessage (serviceIdsToEnable[i], getReason ());

            ResourceId status = sendSynchronously (&prismUpgradeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradeLocalPrismServicesStep : Could not send a message to Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                return (status);
            }

            status = prismUpgradeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradeLocalPrismServicesStep : Could not Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                if (false == FrameworkToolKit::isSchemaConversionDisabledByUser ())
                {
                    handleUpgradeFailure (serviceIdsToEnable[i]);
                }

                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Upgraded " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId  PrismBootAgent::upgradeGlobalPrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    if (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus)
    {
        trace (TRACE_LEVEL_INFO, string("getDbConversionStatus") + FrameworkToolKit::localize (dbConversionStatus));
        return (WAVE_MESSAGE_SUCCESS);
    }

    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if (false == (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
        {
            if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
            {
                    continue;
            }

            if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
            {
                continue;
            }
    
            PrismUpgradeObjectManagerMessage prismUpgradeObjectManagerMessage (serviceIdsToEnable[i], getReason ());

            ResourceId status = sendSynchronously (&prismUpgradeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradeGlobalPrismServicesStep : Could not send a message to Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                return (status);
            }

            status = prismUpgradeObjectManagerMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradeGlobalPrismServicesStep : Could not Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

                
                if (false == FrameworkToolKit::isSchemaConversionDisabledByUser ())
                {
                    handleUpgradeFailure (serviceIdsToEnable[i]);
                }

                return (status);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "Upgraded " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
            }
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId  PrismBootAgent::upgradePrismServicesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId dbConversionStatus = FrameworkToolKit::getDbConversionStatus ();

    if (DATABASE_SCHEMA_CONVERSION_SUCCEEDED != dbConversionStatus)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }

    vector<PrismServiceId> serviceIdsToEnable;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    m_currentFrameworkSequenceGenerator.getEnableSequence (serviceIdsToEnable);
    numberOfServices = serviceIdsToEnable.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        if ((true == (isAPersistentBoot ())) && (true != (willBeAPrimaryLocation ())))
        {
            if (true != (FrameworkToolKit::isALocalService (serviceIdsToEnable[i])))
            {
                continue;
            }
        }

        /* DO WE NEED THIS ?? SINCE UPGRADE IS CALLED ONLY FOR PERSISTENT SYSTEM BOOT UP CASE AND NOT THE CLUSTER BASED PHASES 
        if (true == (isToBeExcludedForEnableAndBoot (serviceIdsToEnable[i])))
        {
            continue;
        }*/

        PrismUpgradeObjectManagerMessage prismUpgradeObjectManagerMessage (serviceIdsToEnable[i], getReason ());

        ResourceId status = sendSynchronously (&prismUpgradeObjectManagerMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradePrismServicesStep: Could not send a message to Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            return (status);
        }

        status = prismUpgradeObjectManagerMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::upgradePrismServicesStep: Could not Upgrade a service : ") + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]) + ", Status : " + FrameworkToolKit::localize (status));

            if (false == FrameworkToolKit::isSchemaConversionDisabledByUser ())
            {
                handleUpgradeFailure (serviceIdsToEnable[i]);
            }

            return (status);
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "Upgraded " + FrameworkToolKit::getServiceNameById (serviceIdsToEnable[i]));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);

}

ResourceId  PrismBootAgent::databaseSanityCheckStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    //This step will send a message(WAVE_OBJECT_MANAGER_DATABASE_SANITY_CHECK) to all Local and Global services.
    vector<PrismServiceId> serviceIds;
    UI32                   i                       = 0;
    UI32                   numberOfServices        = 0;

    /*
     * TBD - Do this Sanity Check only if the last reboot was an abrupt reboot(kernel panic/power cycle)
     */

    if (false == FrameworkToolKit::getIsAbruptReboot ())
    {
        return (WAVE_MESSAGE_SUCCESS);
    }

    m_currentFrameworkSequenceGenerator.getBootSequence (serviceIds);
    numberOfServices = serviceIds.size ();

    for (i = 0; i < numberOfServices; i++)
    {
        WaveObjectManagerDatabaseSanityCheckMessage waveObjectManagerDatabaseSanityCheckMessage (serviceIds[i], getReason ());

        ResourceId status = sendSynchronously (&waveObjectManagerDatabaseSanityCheckMessage, FrameworkToolKit::getThisLocationId ());

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::databaseSanityCheckStep: Could not send a message to check Database Sanity to service : ") + FrameworkToolKit::getServiceNameById (serviceIds[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (status);
        }

        status = waveObjectManagerDatabaseSanityCheckMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::databaseSanityCheckStep: Database Sanity Check failed for service : ") + FrameworkToolKit::getServiceNameById (serviceIds[i]) + ", Status : " + FrameworkToolKit::localize (status));
            return (WAVE_MESSAGE_ERROR_DATABASE_INCONSISTENT);
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "DB Sanity Check Done : " + FrameworkToolKit::getServiceNameById (serviceIds[i]));
        }
    }
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId PrismBootAgent::backendSyncUpStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "PrismBootAgent::backendSyncUpStep: Send back end sync up to all plug-ins .... ");

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    WaveNodeBackendSyncUpAgent *pWaveNodeBackendSyncUpAgent = new WaveNodeBackendSyncUpAgent (m_pWaveObjectManager);

    status = pWaveNodeBackendSyncUpAgent->execute ();

    delete pWaveNodeBackendSyncUpAgent;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
       trace (TRACE_LEVEL_FATAL, string ("PrismBootAgent::backendSyncUpStep Failed : Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

bool PrismBootAgent::isAPersistentBoot ()
{
    return (false);
}

bool PrismBootAgent::isToBeExcludedForEnableAndBoot (const PrismServiceId &prismServiceId)
{
    return (false);
}

bool PrismBootAgent::willBeAPrimaryLocation ()
{
    return ((FrameworkToolKit::isStandAloneLocation ()) || (FrameworkToolKit::isPrimaryLocation ()));
}

bool PrismBootAgent::isToBeExcludedFromInstallDuringPrePhase (const PrismServiceId &prismServiceId)
{
    return (false);
}

bool PrismBootAgent::isToBeExcludedFromInstall (const PrismServiceId &prismServiceId)
{
    return (false);
}

bool PrismBootAgent::isToBeExcludedFromCurrentBootPhase (PrismServiceId prismServiceId)
{
    return (false);
}

bool PrismBootAgent::isToBeExcludedFromInitializeDuringPrePhase (const PrismServiceId &prismServiceId)
{
    return (false);
}

bool PrismBootAgent::isToBeExcludedFromInitializePhase (const PrismServiceId &prismServiceId)
{
    return (false);
}

WaveBootReason PrismBootAgent::getReason () const
{
    return (WAVE_BOOT_FIRST_TIME_BOOT);
}

bool PrismBootAgent::getRollBackFlag () const
{
    return (false);
}

}
