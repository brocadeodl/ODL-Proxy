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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationDeleteNetworkLocalWorker.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationDeleteNetworkLocalMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManagerTypes.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "Framework/Utils/Integer.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Types/UI32Range.h"
#include "Framework/Utils/Integer.h"

#include <stdio.h>

using namespace DcmNs;

namespace NspNs
{

NetworkConfigurationDeleteNetworkLocalWorker::NetworkConfigurationDeleteNetworkLocalWorker (NetworkConfigurationLocalObjectManager *pNetworkConfigurationLocalObjectManager)
    : WaveWorker (pNetworkConfigurationLocalObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_LOCAL_DELETE_NETWORK, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalMessageHandler));
}

NetworkConfigurationDeleteNetworkLocalWorker::~NetworkConfigurationDeleteNetworkLocalWorker ()
{
}

PrismMessage *NetworkConfigurationDeleteNetworkLocalWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_LOCAL_DELETE_NETWORK :
            pPrismMessage = new NetworkConfigurationDeleteNetworkLocalMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalMessageHandler (NetworkConfigurationDeleteNetworkLocalMessage* pNetworkConfigurationDeleteNetworkLocalMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalDeactivateInHardWareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkLocalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationDeleteNetworkLocalWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNetworkConfigurationDeleteNetworkLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalValidateStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalValidateStep : Entering ...");

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalDeactivateInHardWareStep(PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalActivateInHardWareStep : Entering ...");

    NetworkConfigurationDeleteNetworkLocalMessage *pNetworkConfigurationDeleteNetworkLocalMessage = reinterpret_cast<NetworkConfigurationDeleteNetworkLocalMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationDeleteNetworkLocalMessage, __FILE__, __LINE__);

    string  networkName      = pNetworkConfigurationDeleteNetworkLocalMessage->getNetworkName ();
    UI32    networkId        = pNetworkConfigurationDeleteNetworkLocalMessage->getNetworkId   ();
    Integer networkIdInteger   (networkId);

    trace (TRACE_LEVEL_DEVEL, string ("NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalDeactivateInHardWareStep : De-Activating Network in local hardware : Name ") + networkName + string (" Network Id : ") + networkId + string ("."));

    ValClientSynchronousConnection *pValClientSynchronousConnection = NetworkConfigurationLocalObjectManager::getPValClientSynchronousConnection ();

    prismAssert (NULL != pValClientSynchronousConnection, __FILE__, __LINE__);

    WaveConnectionStatus connectionstatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;
    ResourceId           status           = WAVE_MESSAGE_ERROR;

    if (false == (pValClientSynchronousConnection->isCurrentlyConnected ()))
    {
        connectionstatus = pValClientSynchronousConnection->connect ("", 15);
    }

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionstatus)
    {
        string portProfileName = string ("slm-profile-") + networkId;

        status = pValClientSynchronousConnection->deactivatePortProfile (portProfileName);
        status = pValClientSynchronousConnection->deletePortProfile (portProfileName);
        status = pValClientSynchronousConnection->deleteVlanInterface (string ("") + networkId);
    }
    else
    {
        status = connectionstatus;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // For now, simply log a message and move on.

        trace (TRACE_LEVEL_WARN, "NetworkConfigurationDeleteNetworkLocalWorker::deleteNetworkLocalDeactivateInHardWareStep : Could not de-activate the configuration successfully in H/W.  Status : " + FrameworkToolKit::localize (status));

        status = WAVE_MESSAGE_SUCCESS;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

}
