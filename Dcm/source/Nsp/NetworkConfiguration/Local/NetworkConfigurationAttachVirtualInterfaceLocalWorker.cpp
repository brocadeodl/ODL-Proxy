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

#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationAttachVirtualInterfaceLocalWorker.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationLocalObjectManager.h"
#include "Nsp/NetworkConfiguration/Local/NetworkConfigurationAttachVirtualInterfaceLocalMessage.h"
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

NetworkConfigurationAttachVirtualInterfaceLocalWorker::NetworkConfigurationAttachVirtualInterfaceLocalWorker (NetworkConfigurationLocalObjectManager *pNetworkConfigurationLocalObjectManager)
    : WaveWorker (pNetworkConfigurationLocalObjectManager)
{
    addOperationMap (NETWORK_CONFIGURATION_LOCAL_ATTACH_VIRTUAL_INTERFACE, reinterpret_cast<PrismMessageHandler> (&NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalMessageHandler));
}

NetworkConfigurationAttachVirtualInterfaceLocalWorker::~NetworkConfigurationAttachVirtualInterfaceLocalWorker ()
{
}

PrismMessage *NetworkConfigurationAttachVirtualInterfaceLocalWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case NETWORK_CONFIGURATION_LOCAL_ATTACH_VIRTUAL_INTERFACE :
            pPrismMessage = new NetworkConfigurationAttachVirtualInterfaceLocalMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalMessageHandler (NetworkConfigurationAttachVirtualInterfaceLocalMessage* pNetworkConfigurationAttachVirtualInterfaceLocalMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceLocalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&NetworkConfigurationAttachVirtualInterfaceLocalWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNetworkConfigurationAttachVirtualInterfaceLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalValidateStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalValidateStep : Entering ...");

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep (PrismLinearSequencerContext* pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep : Entering ...");

    NetworkConfigurationAttachVirtualInterfaceLocalMessage *pNetworkConfigurationAttachVirtualInterfaceLocalMessage = reinterpret_cast<NetworkConfigurationAttachVirtualInterfaceLocalMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pNetworkConfigurationAttachVirtualInterfaceLocalMessage, __FILE__, __LINE__);

    string     networkName      = pNetworkConfigurationAttachVirtualInterfaceLocalMessage->getNetworkName ();
    UI32       networkId        = pNetworkConfigurationAttachVirtualInterfaceLocalMessage->getNetworkId   ();
    Integer    networkIdInteger   (networkId);
    string     profileName      = "slm-profile-" + networkIdInteger.toString ();
    MacAddress macAddress       = pNetworkConfigurationAttachVirtualInterfaceLocalMessage->getMacAddress ();
    string     targetHost       = pNetworkConfigurationAttachVirtualInterfaceLocalMessage->getTargetHost ();

    trace (TRACE_LEVEL_DEVEL, string ("NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep : attaching Mac " + macAddress.toString2 () + " to  : Name ") + networkName + string (" Network Id : ") + networkId + string (" Using Profile ") + profileName + string ("."));

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
        status = pValClientSynchronousConnection->associateMacToPortProfile (profileName, macAddress.toString2 ());
    }
    else
    {
        status = connectionstatus;
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // For now, simply log a message and move on.

        trace (TRACE_LEVEL_WARN, "NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep : Could not activate the configuration successfully in H/W.  Status : " + FrameworkToolKit::localize (status));

        status = WAVE_MESSAGE_SUCCESS;
    }

#if 0
    trace (TRACE_LEVEL_INFO, "NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep : Configuring Target Host : " + targetHost);

    vector<string> targetHostParameters;
    UI32           numberOfTargetHostParamters;
    string         host;
    string         port;
    string         bridgeName;
    string         vnicName;

    StringUtils::tokenize (targetHost, targetHostParameters, ':');

    numberOfTargetHostParamters = targetHostParameters.size ();

    if (1 <= numberOfTargetHostParamters)
    {
        host = targetHostParameters[0];
    }

    if (2 <= numberOfTargetHostParamters)
    {
        port = targetHostParameters[1];
    }

    if (2 <= numberOfTargetHostParamters)
    {
        bridgeName = targetHostParameters[2];
    }

    if (2 <= numberOfTargetHostParamters)
    {
        vnicName = targetHostParameters[3];
    }

    string ovsCommandString1 = string ("ovs-vsctl --db=tcp:") + host + string (":") + port + string (" -- --if-exists del-port ") + bridgeName + string (" ") + vnicName;
    string ovsCommandString2 = string ("ovs-vsctl --db=tcp:") + host + string (":") + port + string (" add-port ") + bridgeName + string (" ") + vnicName + string (" tag=") + networkId;

    trace (TRACE_LEVEL_INFO, "NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep : Configuring Target Host with : " + ovsCommandString1);
    trace (TRACE_LEVEL_INFO, "NetworkConfigurationAttachVirtualInterfaceLocalWorker::attachVirtualInterfaceLocalActivateInHardWareStep : Configuring Target Host with : " + ovsCommandString2);

    vector<string> output;

    FrameworkToolKit::systemCommandOutput(ovsCommandString1, output);
    FrameworkToolKit::systemCommandOutput(ovsCommandString2, output);
#endif

    pPrismLinearSequencerContext->executeNextStep (status);
}

}
