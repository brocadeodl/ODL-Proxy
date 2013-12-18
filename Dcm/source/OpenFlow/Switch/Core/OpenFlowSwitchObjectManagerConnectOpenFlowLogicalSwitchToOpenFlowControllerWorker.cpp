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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage.h"
#include "OpenFlow/Switch/Core/ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext.h"
#include "Framework/Utils/Integer.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchInstance/OpenFlowLogicalSwitchInstanceLocalObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchInstance/OpenFlowLogicalSwitchInstanceRepository.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker (OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager)
    : WaveWorker (pOpenFlowSwitchObjectManager)
{
    addOperationMap (OPENFLOW_OBJECT_MANAGER_CONNECT_OPENFLOW_LOGICAL_SWITCH_TO_OPENFLOW_CONTROLLER, reinterpret_cast<PrismMessageHandler> (&OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerMessageHandler));
}

OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::~OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker ()
{
}

PrismMessage *OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case OPENFLOW_OBJECT_MANAGER_CONNECT_OPENFLOW_LOGICAL_SWITCH_TO_OPENFLOW_CONTROLLER :
            pPrismMessage = new OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage;
            break;

        default :
            pPrismMessage = NULL;
            break;
    }

    return (pPrismMessage);
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerMessageHandler (OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage *pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::prismLinearSequencerFailedStep)
    };

    ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext *pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext = new ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext (pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->setOpenFlowLogicalSwitchName                          (pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage->getOpenFlowLogicalSwitchName                          ());
    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->setOpenFlowLogicalSwitchUri                           (pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage->getOpenFlowLogicalSwitchUri                           ());
    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->setOpenFlowControllerIpAddress                        (pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage->getOpenFlowControllerIpAddress                        ());
    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->setOpenFlowControllerPort                             (pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage->getOpenFlowControllerPort                             ());
    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->setOpenFlowcontrollerChannelConnectionDefaultProtocol (pOpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage->getOpenFlowcontrollerChannelConnectionDefaultProtocol ());

    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->holdAll ();
    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->start   ();
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep (ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext *pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep : Entering ..");

          ResourceId                        status                            = WAVE_MESSAGE_SUCCESS;
    const string                            openFlowLogicalSwitchName         = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowLogicalSwitchName                          ();
    const Uri                               openFlowLogicalSwitchUri          = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowLogicalSwitchUri                           ();
          string                            openFlowLogicalSwitchUriString;
    const string                            openFlowControllerIpAddress       = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowcontrollerIpAddress                        ();
    const UI32                              openFlowControllerPort            = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowControllerPort                             ();
    const OpenFlowChannelConnectionProtocol openFlowChannelConnectionProtocol = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowcontrollerChannelConnectionDefaultProtocol ();

    openFlowLogicalSwitchUri.toString (openFlowLogicalSwitchUriString);

    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep : Open Flow Logical Switch Name   : " + openFlowLogicalSwitchName);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep : Open Flow Logical Switch URI    : " + openFlowLogicalSwitchUriString);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep : Open Flow Controller Ip Address : " + openFlowControllerIpAddress);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep : Open Flow Controller Port       : " + (Integer (openFlowControllerPort)).toString ());
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerValidateStep : Open Flow Controller Protocol   : " + FrameworkToolKit::localize (openFlowChannelConnectionProtocol));


    WaveManagedObjectSynchronousQueryContext queryContext (OpenFlowLogicalSwitchManagedObject::getClassName ());

    if ("" != openFlowLogicalSwitchName)
    {
        queryContext.addAndAttribute (new AttributeString (openFlowLogicalSwitchName, "openFlowLogicalSwitchName"));
    }
    else
    {
        if ("" != openFlowLogicalSwitchUriString)
        {
            queryContext.addAndAttribute (new AttributeUri (openFlowLogicalSwitchUri,  "openFlowLogicalSwitchUri"));
        }
        else
        {
            status = OPEN_FLOW_LOGICAL_SWITCH_ERROR_INVALID_IDENTIFIERS;
        }
    }

    vector<WaveManagedObject *> *pQueryResults = querySynchronously (&queryContext);

    prismAssert (NULL!= pQueryResults, __FILE__, __LINE__);

    UI32 numberOfQueryResults = pQueryResults->size ();

    prismAssert (1 >= numberOfQueryResults, __FILE__, __LINE__);

    if (1 == numberOfQueryResults)
    {
        WaveManagedObject *pWaveManagedObject = (*pQueryResults)[0];

        prismAssert (NULL != pWaveManagedObject , __FILE__, __LINE__);

        OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject = dynamic_cast<OpenFlowLogicalSwitchManagedObject *> (pWaveManagedObject);

        prismAssert (NULL != pOpenFlowLogicalSwitchManagedObject, __FILE__, __LINE__);

        pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->setPOpenFlowLogicalSwitchManagedObject (pOpenFlowLogicalSwitchManagedObject);

        pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->addManagedObjectForGarbageCollection (pOpenFlowLogicalSwitchManagedObject);

        delete pQueryResults;

        status = WAVE_MESSAGE_SUCCESS;
    }
    else // 0 results
    {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);

        status = OPEN_FLOW_LOGICAL_SWITCH_ERROR_DOES_NOT_EXIST;
    }

    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->executeNextStep (status);
    return;
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep (ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext *pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep : Entering ..");

    OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getPOpenFlowLogicalSwitchManagedObject ();

    prismAssert (NULL != pOpenFlowLogicalSwitchManagedObject, __FILE__, __LINE__);

    const string                            openFlowLogicalSwitchName         = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowLogicalSwitchName   ();
    const Uri                               openFlowLogicalSwitchUri          = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowLogicalSwitchUri    ();
          string                            openFlowLogicalSwitchUriString;
    const string                            openFlowControllerIpAddress       = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowcontrollerIpAddress                        ();
    const UI32                              openFlowControllerPort            = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowControllerPort                             ();
    const OpenFlowChannelConnectionProtocol openFlowChannelConnectionProtocol = pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->getOpenFlowcontrollerChannelConnectionDefaultProtocol ();

    openFlowLogicalSwitchUri.toString (openFlowLogicalSwitchUriString);

    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep : Open Flow Logical Switch Name   : " + openFlowLogicalSwitchName);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep : Open Flow Logical Switch URI    : " + openFlowLogicalSwitchUriString);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep : Open Flow Controller Ip Address : " + openFlowControllerIpAddress);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep : Open Flow Controller Port       : " + (Integer (openFlowControllerPort)).toString ());
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerWorker::connectOpenFlowLogicalSwitchToOpenFlowControllerConnectStep : Open Flow Controller Protocol   : " + FrameworkToolKit::localize (openFlowChannelConnectionProtocol));

    bool       isKnown = pOpenFlowLogicalSwitchManagedObject->isAKnownOpenFlowController (openFlowControllerIpAddress, openFlowControllerPort);
    ResourceId status  = WAVE_MESSAGE_SUCCESS;

    if (true == isKnown)
    {
        status = WAVE_MESSAGE_SUCCESS;

         bool                                             isKnownLogicalSwitchInstance                     = OpenFlowLogicalSwitchInstanceRepository::isAKnownOpenFlowLogicalSwitch (openFlowLogicalSwitchName);
         OpenFlowLogicalSwitchInstanceLocalObjectManager *pOpenFlowLogicalSwitchInstanceLocalObjectManager = NULL;

        if (false == isKnownLogicalSwitchInstance)
        {
            pOpenFlowLogicalSwitchInstanceLocalObjectManager = new OpenFlowLogicalSwitchInstanceLocalObjectManager (openFlowLogicalSwitchName);
        }
        else
        {
            pOpenFlowLogicalSwitchInstanceLocalObjectManager = OpenFlowLogicalSwitchInstanceRepository::getOpenFlowLogicalSwitchInstanceLocalObjectManager (openFlowLogicalSwitchName);
        }

        prismAssert (NULL != pOpenFlowLogicalSwitchInstanceLocalObjectManager, __FILE__, __LINE__);

        //PrismServiceId serviceId = pOpenFlowLogicalSwitchInstanceLocalObjectManager->getServiceId ();
    }
    else
    {

        status = OPEN_FLOW_LOGICAL_SWITCH_ERROR_OPEN_CONTROLLER_DOES_NOT_EXIST;
    }

    pConnectOpenFlowLogicalSwitchToOpenFlowControllerContext->executeNextStep (status);
    return;
}

}
