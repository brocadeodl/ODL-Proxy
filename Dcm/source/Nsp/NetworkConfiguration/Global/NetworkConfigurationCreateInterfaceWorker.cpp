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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceMessage.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceContext.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"
#include "uuid/uuid.h"

using namespace DcmNs;

namespace NspNs
{

NetworkConfigurationCreateInterfaceWorker::NetworkConfigurationCreateInterfaceWorker (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : WaveWorker (pNetworkConfigurationObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_CREATE_INTERFACE, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationCreateInterfaceWorker::createInterfaceMessageHandler));
}

NetworkConfigurationCreateInterfaceWorker::~NetworkConfigurationCreateInterfaceWorker ()
{
}

PrismMessage *NetworkConfigurationCreateInterfaceWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_CREATE_INTERFACE :
            pPrismMessage = new NetworkConfigurationCreateInterfaceMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void NetworkConfigurationCreateInterfaceWorker::enable (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback            ();
}

void NetworkConfigurationCreateInterfaceWorker::createInterfaceMessageHandler (NetworkConfigurationCreateInterfaceMessage* pNetworkConfigurationCreateInterfaceMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateInterfaceWorker::createInterfaceValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateInterfaceWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateInterfaceWorker::createInterfaceCreateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateInterfaceWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateInterfaceWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateInterfaceWorker::prismLinearSequencerFailedStep)
    };

    NetworkConfigurationCreateInterfaceContext *pNetworkConfigurationCreateInterfaceContext = new NetworkConfigurationCreateInterfaceContext (pNetworkConfigurationCreateInterfaceMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pNetworkConfigurationCreateInterfaceContext->holdAll ();
    pNetworkConfigurationCreateInterfaceContext->start ();
}

void NetworkConfigurationCreateInterfaceWorker::createInterfaceValidateStep (NetworkConfigurationCreateInterfaceContext* pNetworkConfigurationCreateInterfaceContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateInterfaceWorker::createInterfaceValidateStep : Entering ...");

    NetworkConfigurationCreateInterfaceMessage *pNetworkConfigurationCreateInterfaceMessage = reinterpret_cast<NetworkConfigurationCreateInterfaceMessage *> (pNetworkConfigurationCreateInterfaceContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationCreateInterfaceMessage, __FILE__, __LINE__);

    string uuid = pNetworkConfigurationCreateInterfaceMessage->getNetworkUuid ();

    WaveManagedObjectSynchronousQueryContext queryContext (NetworkManagedObject::getClassName ());

    queryContext.addAndAttribute (new AttributeString (uuid, "uuid"));

    queryContext.addSelectFieldForObjectId ("objectId");

    vector<WaveManagedObject *> *pQueryResults = querySynchronously (&queryContext);

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    UI32       numberOfExistingNetworksWithThisId = pQueryResults->size ();
    ResourceId status                             = WAVE_MESSAGE_SUCCESS;

    if (0 == numberOfExistingNetworksWithThisId)
    {
        status = NSP_CREATE_INTERFACE_ERROR_NETWORK_DOES_NOT_EXIST;
    }
    else
    {
        prismAssert (1 == numberOfExistingNetworksWithThisId, __FILE__, __LINE__);

        NetworkManagedObject *pNetworkManagedObject = dynamic_cast<NetworkManagedObject *> ((*pQueryResults)[0]);

        prismAssert (NULL != pNetworkManagedObject, __FILE__, __LINE__);

        ObjectId networkObjectId = pNetworkManagedObject->getObjectId ();

        pNetworkConfigurationCreateInterfaceContext->setNetworkObjectId (networkObjectId);

        WaveManagedObjectSynchronousQueryContext queryContextForNetworkSegment (NetworkSegmentManagedObject::getClassName ());

        queryContextForNetworkSegment.addSelectFieldForObjectId ("objectId");
        queryContextForNetworkSegment.addAndAttribute (new AttributeObjectId (networkObjectId, "ownerManagedObjectId"));

        vector<WaveManagedObject *> *pQueryResultsForNetworkSegment = querySynchronously (&queryContextForNetworkSegment);

        prismAssert (NULL != pQueryResultsForNetworkSegment, __FILE__, __LINE__);

        UI32 numberOfNetworkSegments = pQueryResultsForNetworkSegment->size ();

        prismAssert (1 == numberOfNetworkSegments, __FILE__, __LINE__);

        NetworkSegmentManagedObject *pNetworkSegmentManagedObject = dynamic_cast<NetworkSegmentManagedObject *> ((*pQueryResultsForNetworkSegment)[0]);

        prismAssert (NULL != pNetworkSegmentManagedObject, __FILE__, __LINE__);

        ObjectId networkSegmentObjectId = pNetworkSegmentManagedObject->getObjectId ();

        pNetworkConfigurationCreateInterfaceContext->setNetworkSegmentObjectId (networkSegmentObjectId);
        pNetworkConfigurationCreateInterfaceContext->setPNetworkSegmentManagedObject (pNetworkSegmentManagedObject);

        pNetworkConfigurationCreateInterfaceContext->addManagedObjectsForGarbageCollection (*pQueryResultsForNetworkSegment);
    }

    pNetworkConfigurationCreateInterfaceContext->addManagedObjectsForGarbageCollection (*pQueryResults);

    pNetworkConfigurationCreateInterfaceContext->executeNextStep (status);
}

void NetworkConfigurationCreateInterfaceWorker::createInterfaceCreateStep (NetworkConfigurationCreateInterfaceContext* pNetworkConfigurationCreateInterfaceContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateInterfaceWorker::createInterfaceCreateStep : Entering ...");

    NetworkConfigurationCreateInterfaceMessage *pNetworkConfigurationCreateInterfaceMessage = reinterpret_cast<NetworkConfigurationCreateInterfaceMessage *> (pNetworkConfigurationCreateInterfaceContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationCreateInterfaceMessage, __FILE__, __LINE__);

    NetworkSegmentManagedObject *pNetworkSegmentManagedObject = pNetworkConfigurationCreateInterfaceContext->getPNetworkSegmentManagedObject ();

    prismAssert (NULL != pNetworkSegmentManagedObject, __FILE__, __LINE__);

    //ObjectId networkObjectId        = pNetworkConfigurationCreateInterfaceContext->getNetworkObjectId        ();
    ObjectId networkSegmentObjectId = pNetworkConfigurationCreateInterfaceContext->getNetworkSegmentObjectId ();

    VnicManagedObject *pVnicManagedObject = new VnicManagedObject (reinterpret_cast<NetworkConfigurationObjectManager *> (getPWaveObjectManager()));

    prismAssert (NULL != pVnicManagedObject, __FILE__, __LINE__);

    uuid_t                 uuid;
    char                   uuidInStringFormat[64];
    string                 networkuuid             = pNetworkConfigurationCreateInterfaceMessage->getNetworkUuid ();
    NetworkInterfaceStatus networkInterfaceStatus  = pNetworkConfigurationCreateInterfaceMessage->getInterfaceStatus ();

    uuid_generate (uuid);
    uuid_unparse  (uuid, uuidInStringFormat);

    pVnicManagedObject->setUuid                   (uuidInStringFormat);
    pVnicManagedObject->setNetworkUuid            (networkuuid);
    pVnicManagedObject->setNetworkInterfaceStatus (networkInterfaceStatus);

    pVnicManagedObject->setOwnerManagedObjectId (networkSegmentObjectId);

    addToComposition (NetworkSegmentManagedObject::getClassName (), NetworkInterfaceManagedObject::getClassName (), "networkInterfaces", networkSegmentObjectId, pVnicManagedObject->getObjectId ());

    pNetworkConfigurationCreateInterfaceMessage->setInterfaceUuid (uuidInStringFormat);

    pNetworkConfigurationCreateInterfaceContext->addManagedObjectForGarbageCollection (pVnicManagedObject);

    pNetworkConfigurationCreateInterfaceContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
