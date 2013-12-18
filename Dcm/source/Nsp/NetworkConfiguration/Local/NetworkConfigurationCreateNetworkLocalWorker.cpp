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

#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationCreateNetworkLocalWorker.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationCreateNetworkLocalMessage.h"
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

NetworkConfigurationCreateNetworkLocalWorker::NetworkConfigurationCreateNetworkLocalWorker (NetworkConfigurationLocalObjectManager *pNetworkConfigurationLocalObjectManager)
    : WaveWorker (pNetworkConfigurationLocalObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_LOCAL_CREATE_NETWORK, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalMessageHandler));
}

NetworkConfigurationCreateNetworkLocalWorker::~NetworkConfigurationCreateNetworkLocalWorker ()
{
}

PrismMessage *NetworkConfigurationCreateNetworkLocalWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_LOCAL_CREATE_NETWORK :
            pPrismMessage = new NetworkConfigurationCreateNetworkLocalMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalMessageHandler (NetworkConfigurationCreateNetworkLocalMessage* pNetworkConfigurationCreateNetworkLocalMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalActivateInHardWareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkLocalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationCreateNetworkLocalWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNetworkConfigurationCreateNetworkLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalValidateStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalValidateStep : Entering ...");

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalActivateInHardWareStep(PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalActivateInHardWareStep : Entering ...");

    NetworkConfigurationCreateNetworkLocalMessage *pNetworkConfigurationCreateNetworkLocalMessage = reinterpret_cast<NetworkConfigurationCreateNetworkLocalMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationCreateNetworkLocalMessage, __FILE__, __LINE__);

    string  networkName      = pNetworkConfigurationCreateNetworkLocalMessage->getNetworkName ();
    UI32    networkId        = pNetworkConfigurationCreateNetworkLocalMessage->getNetworkId   ();
    Integer networkIdInteger   (networkId);

    trace (TRACE_LEVEL_DEVEL, string ("NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalActivateInHardWareStep : Activating Network in local hardware : Name ") + networkName + string (" Network Id : ") + networkId + string ("."));

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
        status = pValClientSynchronousConnection->createVlanInterface (networkIdInteger.toString (), networkId, 0, networkName);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            string portProfileName = string ("slm-profile-") + networkId;

            status = pValClientSynchronousConnection->createPortProfile (portProfileName);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                status = pValClientSynchronousConnection->createVlanProfileForPortProfile (portProfileName);

                if (WAVE_MESSAGE_SUCCESS == status)
                {
                    status = pValClientSynchronousConnection->setSwitchPortForVlanProfileForPortProfile (portProfileName);

                    if (WAVE_MESSAGE_SUCCESS == status)
                    {
                        status = pValClientSynchronousConnection->setSwitchPortModeTrunkForVlanProfileForPortProfile (portProfileName);

                        if (WAVE_MESSAGE_SUCCESS == status)
                        {
                            UI32Range allowedVlanRange ((Integer (networkId)).toString ());

                            status = pValClientSynchronousConnection->setSwitchPortAllowedVlansForVlanProfileForPortProfile (portProfileName, allowedVlanRange);

                            if (WAVE_MESSAGE_SUCCESS == status)
                            {
                                status = pValClientSynchronousConnection->activatePortProfile (portProfileName);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        status = connectionstatus;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // For now, simply log a message and move on.

        trace (TRACE_LEVEL_WARN, "NetworkConfigurationCreateNetworkLocalWorker::createNetworkLocalActivateInHardWareStep : Could not activate the configuration successfully in H/W.  Status : " + FrameworkToolKit::localize (status));

        status = WAVE_MESSAGE_SUCCESS;
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

}
