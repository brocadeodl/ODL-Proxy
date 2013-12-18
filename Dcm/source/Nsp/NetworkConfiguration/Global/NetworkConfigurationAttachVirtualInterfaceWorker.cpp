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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceWorker.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManager.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceMessage.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationObjectManagerTypes.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceContext.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/VirtualMachineManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationAttachVirtualInterfaceLocalMessage.h"

namespace NspNs
{

NetworkConfigurationAttachVirtualInterfaceWorker::NetworkConfigurationAttachVirtualInterfaceWorker (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager)
    : WaveWorker (pNetworkConfigurationObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_ATTACH_INTERFACE, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceMessageHandler));
}

NetworkConfigurationAttachVirtualInterfaceWorker::~NetworkConfigurationAttachVirtualInterfaceWorker ()
{
}

PrismMessage *NetworkConfigurationAttachVirtualInterfaceWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_ATTACH_INTERFACE :
            pPrismMessage = new NetworkConfigurationAttachVirtualInterfaceMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}
void NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceMessageHandler (NetworkConfigurationAttachVirtualInterfaceMessage *pNetworkConfigurationAttachVirtualInterfaceMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceAttachStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceActivateInAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceWorker::prismLinearSequencerFailedStep)
    };

    NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext = new NetworkConfigurationAttachVirtualInterfaceContext (pNetworkConfigurationAttachVirtualInterfaceMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pNetworkConfigurationAttachVirtualInterfaceContext->holdAll ();
    pNetworkConfigurationAttachVirtualInterfaceContext->start ();
}

void NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceValidateStep (NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext)
{
    NetworkConfigurationAttachVirtualInterfaceMessage *pNetworkConfigurationAttachVirtualInterfaceMessage = reinterpret_cast<NetworkConfigurationAttachVirtualInterfaceMessage*> (pNetworkConfigurationAttachVirtualInterfaceContext->getPPrismMessage ());
    WaveManagedObjectSynchronousQueryContext           interfaceQueryContext                                (VnicManagedObject::getClassName ());
    WaveManagedObjectSynchronousQueryContext           networkQueryContext                                  (NetworkManagedObject::getClassName ());
    WaveManagedObjectSynchronousQueryContext           virtualMachineQueryContext                           (VirtualMachineManagedObject::getClassName ());
    NetworkManagedObject                              *pNetworkManagedObject                              = NULL;
    VnicManagedObject                                 *pVnicManagedObject                                 = NULL;
    string                                             networkUuid;
    string                                             interfaceUuid;
    MacAddress                                         macAddress;
    vector<WaveManagedObject *>                       *pNetworkQueryResults                               = NULL;
    vector<WaveManagedObject *>                       *pInterfaceQueryResults                             = NULL;
    UI32                                               numberOfQueryResults                               = 0;
    ResourceId                                         status                                             = WAVE_MESSAGE_SUCCESS;
    ObjectId                                           virtualMachine;
    UI32                                               virtualMachineCount                                = 0;
    ResourceId                                         queryStatus                                        = WAVE_MESSAGE_SUCCESS;
    string                                             networkName;
    UI32                                               networkId                                          = 0;

    prismAssert (NULL != pNetworkConfigurationAttachVirtualInterfaceMessage, __FILE__, __LINE__);

    networkUuid   = pNetworkConfigurationAttachVirtualInterfaceMessage->getNetworkUuid   ();
    interfaceUuid = pNetworkConfigurationAttachVirtualInterfaceMessage->getInterfaceUuid ();
    macAddress    = pNetworkConfigurationAttachVirtualInterfaceMessage->getMacAddress    ();

    networkQueryContext.addAndAttribute (new AttributeString (networkUuid, "uuid"));
    networkQueryContext.addSelectField ("name");
    networkQueryContext.addSelectField ("networkId");
    networkQueryContext.addSelectField ("uuid");

    pNetworkQueryResults = querySynchronously (&networkQueryContext);

    prismAssert (NULL != pNetworkQueryResults, __FILE__, __LINE__);

    pNetworkConfigurationAttachVirtualInterfaceContext->addManagedObjectsForGarbageCollection (*pNetworkQueryResults);

    numberOfQueryResults = pNetworkQueryResults->size ();

    if (0 == numberOfQueryResults)
    {
        status = NSP_ATTACH_INTERFACE_ERROR_NETWORK_DOES_NOT_EXIST;
    }
    else if (1 == numberOfQueryResults)
    {
        pNetworkManagedObject = dynamic_cast<NetworkManagedObject *> ((*pNetworkQueryResults)[0]);

        prismAssert (NULL != pNetworkManagedObject, __FILE__, __LINE__);

        networkName = pNetworkManagedObject->getName ();
        networkId   = pNetworkManagedObject->getNetworkId ();

        pNetworkConfigurationAttachVirtualInterfaceContext->setNetworkName (networkName);
        pNetworkConfigurationAttachVirtualInterfaceContext->setNetworkId   (networkId);

        interfaceQueryContext.addAndAttribute (new AttributeString (networkUuid,   "networkUuid"));
        interfaceQueryContext.addAndAttribute (new AttributeString (interfaceUuid, "uuid"));

        pInterfaceQueryResults = querySynchronously (&interfaceQueryContext);

        prismAssert (NULL != pInterfaceQueryResults, __FILE__, __LINE__);

        pNetworkConfigurationAttachVirtualInterfaceContext->addManagedObjectsForGarbageCollection (*pInterfaceQueryResults);

        numberOfQueryResults = pInterfaceQueryResults->size ();

        if (0 == numberOfQueryResults)
        {
            status = NSP_ATTACH_INTERFACE_ERROR_INTERFACE_DOES_NOT_EXIST;
        }
        else if (1 == numberOfQueryResults)
        {
            pVnicManagedObject = dynamic_cast<VnicManagedObject *> ((*pInterfaceQueryResults)[0]);

            prismAssert (NULL != pVnicManagedObject, __FILE__, __LINE__);

            pNetworkConfigurationAttachVirtualInterfaceContext->setPVnicManagedObject (pVnicManagedObject);

            virtualMachine = pVnicManagedObject->getVirtualMachine ();

            if (ObjectId::NullObjectId != virtualMachine)
            {
                status = NSP_ATTACH_INTERFACE_ERROR_INTERFACE_ALREADY_USED;
            }
            else
            {
                virtualMachineQueryContext.addAndAttribute (new AttributeMacAddress (macAddress, "macAddress"));

                queryStatus = querySynchronouslyForCount (&virtualMachineQueryContext, virtualMachineCount);

                if (WAVE_MESSAGE_SUCCESS != queryStatus)
                {
                    prismAssert (false, __FILE__, __LINE__);
                }

                if (0 == virtualMachineCount)
                {
                    status = WAVE_MESSAGE_SUCCESS;
                }
                else if (1 == virtualMachineCount)
                {
                    status = NSP_ATTACH_INTERFACE_ERROR_VM_ALREADY_ATTACHED;
                }
                else
                {
                    prismAssert (false, __FILE__, __LINE__);
                }
            }
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    pNetworkConfigurationAttachVirtualInterfaceContext->executeNextStep (status);
}

void NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceAttachStep (NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext)
{
    NetworkConfigurationAttachVirtualInterfaceMessage *pNetworkConfigurationAttachVirtualInterfaceMessage = reinterpret_cast<NetworkConfigurationAttachVirtualInterfaceMessage*> (pNetworkConfigurationAttachVirtualInterfaceContext->getPPrismMessage ());
    VnicManagedObject                                 *pVnicManagedObject                                 = pNetworkConfigurationAttachVirtualInterfaceContext->getPVnicManagedObject ();
    string                                             networkUuid;
    string                                             interfaceUuid;
    MacAddress                                         macAddress;
    string                                             targetHost;

    prismAssert (NULL != pNetworkConfigurationAttachVirtualInterfaceMessage, __FILE__, __LINE__);

    networkUuid   = pNetworkConfigurationAttachVirtualInterfaceMessage->getNetworkUuid   ();
    interfaceUuid = pNetworkConfigurationAttachVirtualInterfaceMessage->getInterfaceUuid ();
    macAddress    = pNetworkConfigurationAttachVirtualInterfaceMessage->getMacAddress    ();
    targetHost    = pNetworkConfigurationAttachVirtualInterfaceMessage->getTargetHost    ();

    prismAssert (NULL != pVnicManagedObject, __FILE__, __LINE__);

    VirtualMachineManagedObject *pVirtualMachineManagedObject = new VirtualMachineManagedObject (reinterpret_cast<NetworkConfigurationObjectManager *> (getPWaveObjectManager()));

    prismAssert (NULL != pVirtualMachineManagedObject, __FILE__, __LINE__);

    pVirtualMachineManagedObject->setMacAddress (macAddress);
    pVirtualMachineManagedObject->setTargetHost (targetHost);

    updateWaveManagedObject (pVnicManagedObject);

    pVnicManagedObject->setVirtualMachine (pVirtualMachineManagedObject->getObjectId ());

    pNetworkConfigurationAttachVirtualInterfaceContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceActivateInAllNodesStep (NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateNetworkWorker::createNetworkActivateInAllNodesStep : Entering ...");

    NetworkConfigurationAttachVirtualInterfaceMessage *pNetworkConfigurationAttachVirtualInterfaceMessage = reinterpret_cast<NetworkConfigurationAttachVirtualInterfaceMessage*> (pNetworkConfigurationAttachVirtualInterfaceContext->getPPrismMessage ());
    string                                             networkUuid;
    MacAddress                                         macAddress;
    string                                             targetHost;
    string                                             networkName;
    UI32                                               networkId                                          = 0;

    prismAssert (NULL != pNetworkConfigurationAttachVirtualInterfaceMessage, __FILE__, __LINE__);

    networkUuid   = pNetworkConfigurationAttachVirtualInterfaceMessage->getNetworkUuid ();
    macAddress    = pNetworkConfigurationAttachVirtualInterfaceMessage->getMacAddress  ();
    targetHost    = pNetworkConfigurationAttachVirtualInterfaceMessage->getTargetHost  ();
    networkName   = pNetworkConfigurationAttachVirtualInterfaceContext->getNetworkName ();
    networkId     = pNetworkConfigurationAttachVirtualInterfaceContext->getNetworkId   ();

    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceActivateInAllNodesCallback), pNetworkConfigurationAttachVirtualInterfaceContext);

    prismAssert (NULL != pWaveSendToClusterContext, __FILE__, __LINE__);

    NetworkConfigurationAttachVirtualInterfaceLocalMessage *pNetworkConfigurationAttachVirtualInterfaceLocalMessage = new NetworkConfigurationAttachVirtualInterfaceLocalMessage (networkName, networkId, macAddress, targetHost);

    prismAssert (NULL != pNetworkConfigurationAttachVirtualInterfaceLocalMessage, __FILE__, __LINE__);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pNetworkConfigurationAttachVirtualInterfaceLocalMessage);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void NetworkConfigurationAttachVirtualInterfaceWorker::attachVirtualInterfaceActivateInAllNodesCallback (WaveSendToClusterContext* pWaveSendToClusterContext)
{
    ResourceId                                         sendToClusterCompletionStatus                       = pWaveSendToClusterContext->getCompletionStatus ();
    NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext  = reinterpret_cast<NetworkConfigurationAttachVirtualInterfaceContext *> (pWaveSendToClusterContext->getPCallerContext ());

    prismAssert (NULL != pNetworkConfigurationAttachVirtualInterfaceContext, __FILE__, __LINE__);

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;

    pNetworkConfigurationAttachVirtualInterfaceContext->executeNextStep (sendToClusterCompletionStatus);
}


}
