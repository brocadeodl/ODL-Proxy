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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Cluster/MultiPartition/Global/MultiPartitionGlobalCleanupAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Cluster/MultiPartition/Global/MultiPartitionObjectManager.h"
#include "Cluster/MultiPartition/Global/MultiPartitionGlobalCleanupAgentContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

MultiPartitionGlobalCleanupAgent::MultiPartitionGlobalCleanupAgent (WaveObjectManager *pWaveObjectManager, string &partitionName, UI32 senderServiceId, ObjectId &ownerPartitionManagedObjectId) 
    : WaveWorker                        (pWaveObjectManager),
      m_partitionName                   (partitionName),
      m_senderServiceId                 (senderServiceId),
      m_ownerPartitionManagedObjectId   (ownerPartitionManagedObjectId),
      m_isPartialCleanup                (false),
      m_partialCleanupTag               (FRAMEWORK_ERROR)
{
}

MultiPartitionGlobalCleanupAgent::~MultiPartitionGlobalCleanupAgent ()
{
}

bool MultiPartitionGlobalCleanupAgent::getIsPartialCleanup () const
{
    return m_isPartialCleanup;
}

void MultiPartitionGlobalCleanupAgent::setPartialCleanupTag (const ResourceId &partialCleanupTag)
{
    m_isPartialCleanup  = true;

    m_partialCleanupTag = partialCleanupTag;
}

ResourceId MultiPartitionGlobalCleanupAgent::getPartialCleanupTag ()
{
    prismAssert (true == m_isPartialCleanup, __FILE__, __LINE__);

    return m_partialCleanupTag;
}

ResourceId MultiPartitionGlobalCleanupAgent::execute ()
{
    trace (TRACE_LEVEL_INFO, "PrismMultiPartitionGlobalCleanupAgent::execute: called.");

    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionGlobalCleanupAgent::getListOfEnabledServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionGlobalCleanupAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionGlobalCleanupAgent::prismSynchronousLinearSequencerFailedStep)
    };

    MultiPartitionGlobalCleanupAgentContext *pMultiPartitionGlobalCleanupAgentContext = new MultiPartitionGlobalCleanupAgentContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pMultiPartitionGlobalCleanupAgentContext->execute ();

    return (status);
}

ResourceId MultiPartitionGlobalCleanupAgent::getListOfEnabledServicesStep (MultiPartitionGlobalCleanupAgentContext *pMultiPartitionGlobalCleanupAgentContext)
{
    trace (TRACE_LEVEL_INFO, "PrismMultiPartitionGlobalCleanupAgent::getListOfEnabledServicesStep: called.");
    vector<PrismServiceId> &enabledServices = pMultiPartitionGlobalCleanupAgentContext->getEnabledServices ();

    WaveObjectManager::getListOfEnabledServices (enabledServices);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId MultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep (MultiPartitionGlobalCleanupAgentContext *pMultiPartitionGlobalCleanupAgentContext)
{
    vector<PrismServiceId> &serviceIdsToSendMultiPartitionCleanup   = pMultiPartitionGlobalCleanupAgentContext->getEnabledServices ();
    UI32                   i                                        = 0;
    UI32                   numberOfServices                         = serviceIdsToSendMultiPartitionCleanup.size();

    trace (TRACE_LEVEL_INFO, string ("PrismMultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep: called. Total Services = [ ") + (serviceIdsToSendMultiPartitionCleanup.size())+ string("]"));

    for (i = 0; i < numberOfServices; i++)
    {
        trace (TRACE_LEVEL_SUCCESS, "PrismMultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep: send PartitionCleanup to service: " + FrameworkToolKit::getServiceNameById (serviceIdsToSendMultiPartitionCleanup[i]));

        if (false == (requiresMultiPartitionGlobalCleanupNotification(serviceIdsToSendMultiPartitionCleanup[i])))
        {
            continue;
        }

        if (m_senderServiceId == serviceIdsToSendMultiPartitionCleanup[i])
        {
            // Don't send the cleanup to the caller service. 
            continue;
        }

        ObjectId ownerPartitionManagedObjectId = getOwnerPartitionManagedObjectId ();

        PrismMultiPartitionCleanupObjectManagerMessage *prismMultiPartitionCleanupObjectManagerMessage = new PrismMultiPartitionCleanupObjectManagerMessage (serviceIdsToSendMultiPartitionCleanup[i], m_partitionName, ownerPartitionManagedObjectId );

        if (true == getIsPartialCleanup ())
        {
            ResourceId partialCleanupTag = getPartialCleanupTag ();
            prismMultiPartitionCleanupObjectManagerMessage->setPartialCleanupTag (partialCleanupTag);
        }

        ResourceId status = sendSynchronously (prismMultiPartitionCleanupObjectManagerMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismMultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep: Could not send PartitionCleanup to service: " + FrameworkToolKit::getServiceNameById (serviceIdsToSendMultiPartitionCleanup[i]));

            delete prismMultiPartitionCleanupObjectManagerMessage;

            return (status);
        }
        else
        {
            status = prismMultiPartitionCleanupObjectManagerMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismMultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep: Not able to update Instance Id  Completion Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "PrismMultiPartitionGlobalCleanupAgent::sendMultiPartitionGlobalCleanupStep : Successfully sent MultiPartitionCleanup to Service: " + FrameworkToolKit::getServiceNameById (serviceIdsToSendMultiPartitionCleanup[i]));
            }
        }

        delete prismMultiPartitionCleanupObjectManagerMessage;
    }

    return (WAVE_MESSAGE_SUCCESS);
}

bool MultiPartitionGlobalCleanupAgent::requiresMultiPartitionGlobalCleanupNotification (const PrismServiceId &prismServiceId)
{
    // 1. Exlude PrismFrameworkObjectManager (in general.)
    // 2. Exclude MultiTenencyLocalObjectManager (It is running this Agent)
    // 3. Exclude Local services
    // 4. Exclude User Specific Local services.

    if (((PrismFrameworkObjectManager::getPrismServiceId()) == prismServiceId) ||
        ((MultiPartitionObjectManager::getPrismServiceId ()) == prismServiceId) ||
        (true == (FrameworkToolKit::isALocalService (prismServiceId))) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

ObjectId  MultiPartitionGlobalCleanupAgent::getOwnerPartitionManagedObjectId () const
{
    return (m_ownerPartitionManagedObjectId);
}

}
