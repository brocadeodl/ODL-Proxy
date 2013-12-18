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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationCreateNetworkLocalMessage.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"
#include "Framework/Types/BitMap.h"

#include "uuid/uuid.h"

using namespace DcmNs;

namespace NspNs
{

static BitMap s_networkIdBitMap (4096);

NetworkConfigurationCreateNetworkWorker::NetworkConfigurationCreateNetworkWorker (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : WaveWorker (pNetworkConfigurationObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_CREATE_NETWORK, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationCreateNetworkWorker::createNetworkMessageHandler));
}

NetworkConfigurationCreateNetworkWorker::~NetworkConfigurationCreateNetworkWorker ()
{
}

PrismMessage *NetworkConfigurationCreateNetworkWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_CREATE_NETWORK :
            pPrismMessage = new NetworkConfigurationCreateNetworkMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void NetworkConfigurationCreateNetworkWorker::enable (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    // For now this code to set the reserved bits is in enable.  But, move this to install later when we support persistence for bitmap.

    s_networkIdBitMap.set (0);
    s_networkIdBitMap.set (1);
    s_networkIdBitMap.set (1001);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback            ();
}

void NetworkConfigurationCreateNetworkWorker::createNetworkMessageHandler (NetworkConfigurationCreateNetworkMessage* pNetworkConfigurationCreateNetworkMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::createNetworkValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::createNetworkCreateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::createNetworkActivateInAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNetworkConfigurationCreateNetworkMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void NetworkConfigurationCreateNetworkWorker::createNetworkValidateStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateNetworkWorker::createNetworkValidateStep : Entering ...");

    NetworkConfigurationCreateNetworkMessage *pNetworkConfigurationCreateNetworkMessage = reinterpret_cast<NetworkConfigurationCreateNetworkMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationCreateNetworkMessage, __FILE__, __LINE__);

    string networkName = pNetworkConfigurationCreateNetworkMessage->getNetworkName ();
    UI32   networkId   = pNetworkConfigurationCreateNetworkMessage->getNetworkId   ();

    WaveManagedObjectSynchronousQueryContext queryContext (NetworkManagedObject::getClassName ());

    queryContext.addOrAttribute (new AttributeString (networkName, "name"));

    if (0 != networkId)
    {
        queryContext.addOrAttribute (new AttributeUI32 (networkId, "networkId"));
    }

    vector<WaveManagedObject *> *pQueryResults = querySynchronously (&queryContext);

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    UI32       numberOfExistingNetworksWithThisName = pQueryResults->size ();
    ResourceId status                               = WAVE_MESSAGE_SUCCESS;

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);

    if (0 < numberOfExistingNetworksWithThisName)
    {
        status = NSP_CREATE_NETWORK_ERROR_DUPLICATE_NAME_OR_ID;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void NetworkConfigurationCreateNetworkWorker::createNetworkCreateStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateNetworkWorker::createNetworkCreateStep : Entering ...");

    NetworkConfigurationCreateNetworkMessage *pNetworkConfigurationCreateNetworkMessage = reinterpret_cast<NetworkConfigurationCreateNetworkMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationCreateNetworkMessage, __FILE__, __LINE__);

    string networkName = pNetworkConfigurationCreateNetworkMessage->getNetworkName ();
    UI32   networkId   = pNetworkConfigurationCreateNetworkMessage->getNetworkId   ();

    if (0 == networkId)
    {
        networkId = getNextAvailableNetworkId ();

        if (0 == networkId)
        {
            pPrismLinearSequencerContext->executeNextStep (NSP_CREATE_NETWORK_ERROR_OUT_OF_NETWORKS);
            return;
        }

        pNetworkConfigurationCreateNetworkMessage->setNetworkId (networkId);
    }

    NetworkManagedObject *pNetworkManagedObject = new NetworkManagedObject (reinterpret_cast<NetworkConfigurationObjectManager *> (getPWaveObjectManager()));

    prismAssert (NULL != pNetworkManagedObject, __FILE__, __LINE__);

    pNetworkManagedObject->setName      (networkName);
    pNetworkManagedObject->setNetworkId (networkId);

    // Later this one has to go into persistable configuration.

    s_networkIdBitMap.set (networkId);

    uuid_t uuid;
    char   uuidInStringFormat[64];

    uuid_generate (uuid);
    uuid_unparse (uuid, uuidInStringFormat);

    pNetworkManagedObject->setUuid (uuidInStringFormat);

    pNetworkConfigurationCreateNetworkMessage->setNetworkUuid (uuidInStringFormat);

    NetworkSegmentManagedObject *pNetworkSegmentManagedObject = new NetworkSegmentManagedObject (reinterpret_cast<NetworkConfigurationObjectManager *> (getPWaveObjectManager()));

    prismAssert (NULL != pNetworkSegmentManagedObject, __FILE__, __LINE__);

    pNetworkManagedObject->addNetworkSegment (pNetworkSegmentManagedObject);

    pPrismLinearSequencerContext->addManagedObjectForGarbageCollection (pNetworkManagedObject);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void NetworkConfigurationCreateNetworkWorker::createNetworkActivateInAllNodesStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateNetworkWorker::createNetworkActivateInAllNodesStep : Entering ...");

    NetworkConfigurationCreateNetworkMessage *pNetworkConfigurationCreateNetworkMessage = reinterpret_cast<NetworkConfigurationCreateNetworkMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationCreateNetworkMessage, __FILE__, __LINE__);

    UI32 networkId = pNetworkConfigurationCreateNetworkMessage->getNetworkId ();

    prismAssert (0 != networkId, __FILE__, __LINE__);

    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&NetworkConfigurationCreateNetworkWorker::createNetworkActivateInAllNodesCallback), pPrismLinearSequencerContext);

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    NetworkConfigurationCreateNetworkLocalMessage *pNetworkConfigurationCreateNetworkLocalMessage = new NetworkConfigurationCreateNetworkLocalMessage (pNetworkConfigurationCreateNetworkMessage->getNetworkName (), networkId);

    prismAssert (NULL != pNetworkConfigurationCreateNetworkLocalMessage, __FILE__, __LINE__);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pNetworkConfigurationCreateNetworkLocalMessage);

    sendToWaveCluster (pWaveSendToClusterContext);

}

void NetworkConfigurationCreateNetworkWorker::createNetworkActivateInAllNodesCallback (WaveSendToClusterContext* pWaveSendToClusterContext)
{
    ResourceId                   sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pPrismLinearSequencerContext->executeNextStep (sendToClusterCompletionStatus);
}

UI32 NetworkConfigurationCreateNetworkWorker::getNextAvailableNetworkId ()
{
    UI32 i                  = 0;
    UI32 nextAvailableIndex = 0;

    for (i = 0; i < 4096; i++)
    {
        if (0 == s_networkIdBitMap.getValue (i))
        {
            nextAvailableIndex = i;
            break;
        }
    }

    return (nextAvailableIndex);
}

}
