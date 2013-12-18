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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationDeleteNetworkWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationDeleteNetworkMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationDeleteNetworkLocalMessage.h"
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

NetworkConfigurationDeleteNetworkWorker::NetworkConfigurationDeleteNetworkWorker (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : WaveWorker (pNetworkConfigurationObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_DELETE_NETWORK, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationDeleteNetworkWorker::deleteNetworkMessageHandler));
}

NetworkConfigurationDeleteNetworkWorker::~NetworkConfigurationDeleteNetworkWorker ()
{
}

PrismMessage *NetworkConfigurationDeleteNetworkWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_DELETE_NETWORK :
            pPrismMessage = new NetworkConfigurationDeleteNetworkMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void NetworkConfigurationDeleteNetworkWorker::deleteNetworkMessageHandler (NetworkConfigurationDeleteNetworkMessage* pNetworkConfigurationDeleteNetworkMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::deleteNetworkValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::deleteNetworkDeleteStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::deleteNetworkDeactivateInAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNetworkConfigurationDeleteNetworkMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void NetworkConfigurationDeleteNetworkWorker::deleteNetworkValidateStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationDeleteNetworkWorker::deleteNetworkValidateStep : Entering ...");

    NetworkConfigurationDeleteNetworkMessage *pNetworkConfigurationDeleteNetworkMessage = reinterpret_cast<NetworkConfigurationDeleteNetworkMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationDeleteNetworkMessage, __FILE__, __LINE__);

    string networkName = pNetworkConfigurationDeleteNetworkMessage->getNetworkName ();
    UI32   networkId   = pNetworkConfigurationDeleteNetworkMessage->getNetworkId   ();
    string networkUuid = pNetworkConfigurationDeleteNetworkMessage->getNetworkUuid ();

    WaveManagedObjectSynchronousQueryContext queryContext (NetworkManagedObject::getClassName ());

    if ("" != networkName)
    {
        queryContext.addOrAttribute (new AttributeString (networkName, "name"));
    }

    if (0 != networkId)
    {
        queryContext.addOrAttribute (new AttributeUI32 (networkId, "networkId"));
    }

    if ("" != networkUuid)
    {
        queryContext.addOrAttribute (new AttributeString (networkUuid, "uuid"));
    }

    vector<WaveManagedObject *> *pQueryResults = querySynchronously (&queryContext);

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    const UI32       numberOfExistingNetworksWithThisName = pQueryResults->size ();
          ResourceId status                               = WAVE_MESSAGE_SUCCESS;

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);

    if (0 == numberOfExistingNetworksWithThisName)
    {
        status = NSP_DELETE_NETWORK_ERROR_NO_NETWORK_WITH_NAME_OR_ID;
    }
    else
    {
        prismAssert (1 == numberOfExistingNetworksWithThisName, __FILE__, __LINE__);
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

void NetworkConfigurationDeleteNetworkWorker::deleteNetworkDeleteStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationDeleteNetworkWorker::deleteNetworkDeleteStep : Entering ...");

    NetworkConfigurationDeleteNetworkMessage *pNetworkConfigurationDeleteNetworkMessage = reinterpret_cast<NetworkConfigurationDeleteNetworkMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationDeleteNetworkMessage, __FILE__, __LINE__);

    string networkName = pNetworkConfigurationDeleteNetworkMessage->getNetworkName ();
    UI32   networkId   = pNetworkConfigurationDeleteNetworkMessage->getNetworkId   ();
    string networkUuid = pNetworkConfigurationDeleteNetworkMessage->getNetworkUuid ();

    WaveManagedObjectSynchronousQueryContext queryContext (NetworkManagedObject::getClassName ());

    if ("" != networkName)
    {
        queryContext.addOrAttribute (new AttributeString (networkName, "name"));
    }

    if (0 != networkId)
    {
        queryContext.addOrAttribute (new AttributeUI32 (networkId, "networkId"));
    }

    if ("" != networkUuid)
    {
        queryContext.addOrAttribute (new AttributeString (networkUuid, "uuid"));
    }

    vector<WaveManagedObject *> *pQueryResults         = querySynchronously (&queryContext);
    NetworkManagedObject        *pNetworkManagedObject = NULL;

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    const UI32 numberOfExistingNetworksWithThisName = pQueryResults->size ();

    prismAssert (1 == numberOfExistingNetworksWithThisName, __FILE__, __LINE__);

    pNetworkManagedObject = dynamic_cast<NetworkManagedObject *> ((*pQueryResults)[0]);

    prismAssert (NULL != pNetworkManagedObject, __FILE__, __LINE__);

    pNetworkConfigurationDeleteNetworkMessage->setNetworkId   (pNetworkManagedObject->getNetworkId ());
    pNetworkConfigurationDeleteNetworkMessage->setNetworkName (pNetworkManagedObject->getName ());

    delete pNetworkManagedObject;

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void NetworkConfigurationDeleteNetworkWorker::deleteNetworkDeactivateInAllNodesStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationDeleteNetworkWorker::deleteNetworkDeactivateInAllNodesStep : Entering ...");

    NetworkConfigurationDeleteNetworkMessage *pNetworkConfigurationDeleteNetworkMessage = reinterpret_cast<NetworkConfigurationDeleteNetworkMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationDeleteNetworkMessage, __FILE__, __LINE__);

    UI32 networkId = pNetworkConfigurationDeleteNetworkMessage->getNetworkId ();

    prismAssert (0 != networkId, __FILE__, __LINE__);

    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&NetworkConfigurationDeleteNetworkWorker::deleteNetworkActivateInAllNodesCallback), pPrismLinearSequencerContext);

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    NetworkConfigurationDeleteNetworkLocalMessage *pNetworkConfigurationDeleteNetworkLocalMessage = new NetworkConfigurationDeleteNetworkLocalMessage (pNetworkConfigurationDeleteNetworkMessage->getNetworkName (), networkId);

    prismAssert (NULL != pNetworkConfigurationDeleteNetworkLocalMessage, __FILE__, __LINE__);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pNetworkConfigurationDeleteNetworkLocalMessage);

    sendToWaveCluster (pWaveSendToClusterContext);

}

void NetworkConfigurationDeleteNetworkWorker::deleteNetworkActivateInAllNodesCallback (WaveSendToClusterContext* pWaveSendToClusterContext)
{
    ResourceId                   sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
    PrismLinearSequencerContext *pPrismLinearSequencerContext  = reinterpret_cast<PrismLinearSequencerContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pPrismLinearSequencerContext->executeNextStep (sendToClusterCompletionStatus);
}

}
