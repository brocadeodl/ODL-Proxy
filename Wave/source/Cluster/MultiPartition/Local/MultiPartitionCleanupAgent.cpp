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

#include "Cluster/MultiPartition/Local/MultiPartitionCleanupAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/MultiPartition/Local/MultiPartitionLocalObjectManager.h"
#include "Cluster/MultiPartition/Local/MultiPartitionCleanupAgentContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveLocalObjectManagerForUserSpecificTasks.h"

namespace WaveNs
{

MultiPartitionCleanupAgent::MultiPartitionCleanupAgent (WaveObjectManager *pWaveObjectManager, string &partitionName, UI32 senderServiceId, ObjectId &ownerPartitionManagedObjectId) 
    : WaveWorker (pWaveObjectManager),
      m_partitionName (partitionName),
      m_senderServiceId (senderServiceId),
      m_ownerPartitionManagedObjectId (ownerPartitionManagedObjectId),
      m_isPartialCleanup              (false),
      m_partialCleanupTag             (FRAMEWORK_ERROR)

{
}

MultiPartitionCleanupAgent::~MultiPartitionCleanupAgent ()
{
}

bool MultiPartitionCleanupAgent::getIsPartialCleanup () const
{
    return m_isPartialCleanup;
}

void MultiPartitionCleanupAgent::setPartialCleanupTag (const ResourceId &partialCleanupTag)
{
    m_isPartialCleanup  = true;

    m_partialCleanupTag = partialCleanupTag;
}

ResourceId MultiPartitionCleanupAgent::getPartialCleanupTag ()
{
    prismAssert (true == m_isPartialCleanup, __FILE__, __LINE__);

    return m_partialCleanupTag;
}

ResourceId MultiPartitionCleanupAgent::execute ()
{
    trace (TRACE_LEVEL_INFO, "PrismMultiPartitionCleanupAgent::execute: called.");

    WaveNs::PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionCleanupAgent::getListOfEnabledServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionCleanupAgent::sendMultiPartitionCleanupStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionCleanupAgent::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionCleanupAgent::prismSynchronousLinearSequencerFailedStep)
    };

    MultiPartitionCleanupAgentContext *pMultiPartitionCleanupAgentContext = new MultiPartitionCleanupAgentContext (reinterpret_cast<PrismAsynchronousContext *> (NULL), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    ResourceId status = pMultiPartitionCleanupAgentContext->execute ();

    return (status);
}

ResourceId MultiPartitionCleanupAgent::getListOfEnabledServicesStep (MultiPartitionCleanupAgentContext *pMultiPartitionCleanupAgentContext)
{
    trace (TRACE_LEVEL_INFO, "PrismMultiPartitionCleanupAgent::getListOfEnabledServicesStep: called.");
    vector<PrismServiceId> &enabledServices = pMultiPartitionCleanupAgentContext->getEnabledServices ();

    WaveObjectManager::getListOfEnabledServices (enabledServices);

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId MultiPartitionCleanupAgent::sendMultiPartitionCleanupStep (MultiPartitionCleanupAgentContext *pMultiPartitionCleanupAgentContext)
{
    vector<PrismServiceId> &serviceIdsToSendMultiPartitionCleanup = pMultiPartitionCleanupAgentContext->getEnabledServices ();
    UI32       i                    = 0;
    UI32       numberOfServices     = serviceIdsToSendMultiPartitionCleanup.size();

    trace (TRACE_LEVEL_INFO, string ("PrismMultiPartitionCleanupAgent::sendMultiPartitionCleanupStep: called. Total Services = [ ") + (serviceIdsToSendMultiPartitionCleanup.size())+ string("]"));
    for (i = 0; i < numberOfServices; i++)
    {
        trace (TRACE_LEVEL_INFO, "PrismMultiPartitionCleanupAgent::sendMultiPartitionCleanupStep: send PartitionCleanup to service: " + FrameworkToolKit::getServiceNameById (serviceIdsToSendMultiPartitionCleanup[i]));
        if (false == (requiresMultiPartitionCleanupNotification(serviceIdsToSendMultiPartitionCleanup[i])))
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
            trace (TRACE_LEVEL_FATAL, "PrismMultiPartitionCleanupAgent::sendMultiPartitionCleanupStep: Could not send PartitionCleanup to service: " + FrameworkToolKit::getServiceNameById (serviceIdsToSendMultiPartitionCleanup[i]));

            delete prismMultiPartitionCleanupObjectManagerMessage;

            return (status);
        }
        else
        {
            status = prismMultiPartitionCleanupObjectManagerMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "PrismMultiPartitionCleanupAgent::sendMultiPartitionCleanupStep: Not able to update Instance Id  Completion Status : " + FrameworkToolKit::localize (status));
                prismAssert (false, __FILE__, __LINE__);
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "PrismMultiPartitionCleanupAgent::sendMultiPartitionCleanupStep : Successfully sent MultiPartitionCleanup to Service: " + FrameworkToolKit::getServiceNameById (serviceIdsToSendMultiPartitionCleanup[i]));
            }
        }

        delete prismMultiPartitionCleanupObjectManagerMessage;
    }

    return (WAVE_MESSAGE_SUCCESS);
}

bool MultiPartitionCleanupAgent::requiresMultiPartitionCleanupNotification(const PrismServiceId &prismServiceId)
{
    // 1. Exlude PrismFrameworkObjectManager (in general.)
    // 2. Exclude MultiTenencyLocalObjectManager (It is running this Agent)
    // 3. Exclude Global services
    // 4. Exclude User Specific Local services.

    if (((PrismFrameworkObjectManager::getPrismServiceId()) == prismServiceId) ||
        ((MultiPartitionLocalObjectManager::getPrismServiceId ()) == prismServiceId) ||
        ((ClusterLocalObjectManager::getPrismServiceId ()) == prismServiceId) ||
        (false == (FrameworkToolKit::isALocalService (prismServiceId))) ||
        (true == (WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))))
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

ObjectId  MultiPartitionCleanupAgent::getOwnerPartitionManagedObjectId () const
{
    return (m_ownerPartitionManagedObjectId);
}

}
