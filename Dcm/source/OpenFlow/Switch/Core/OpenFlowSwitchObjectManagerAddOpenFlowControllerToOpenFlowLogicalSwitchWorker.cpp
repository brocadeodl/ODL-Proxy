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

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage.h"
#include "OpenFlow/Switch/Core/AddOpenFlowControllerToOpenFlowLogicalSwitchContext.h"
#include "Framework/Utils/Integer.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIdEnums.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker (OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager)
    : WaveWorker (pOpenFlowSwitchObjectManager)
{
    addOperationMap (OPENFLOW_OBJECT_MANAGER_ADD_OPENFLOW_CONTROLLER_TO_OPENFLOW_LOGICAL_SWITCH, reinterpret_cast<PrismMessageHandler> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchMessageHandler));

    OpenFlowControllerManagedObject openFlowControllerManagedObject (pOpenFlowSwitchObjectManager);

    openFlowControllerManagedObject.setupOrm ();

    addManagedClass (OpenFlowControllerManagedObject::getClassName (), this);
}

OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::~OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker ()
{
}

PrismMessage *OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case OPENFLOW_OBJECT_MANAGER_ADD_OPENFLOW_CONTROLLER_TO_OPENFLOW_LOGICAL_SWITCH :
            pPrismMessage = new OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage;
            break;

        default :
            pPrismMessage = NULL;
            break;
    }

    return (pPrismMessage);
}

WaveManagedObject *OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((OpenFlowControllerManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new OpenFlowControllerManagedObject (reinterpret_cast<OpenFlowSwitchObjectManager *> (getPWaveObjectManager ()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

void OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchMessageHandler (OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage *pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::prismLinearSequencerFailedStep)
    };

    AddOpenFlowControllerToOpenFlowLogicalSwitchContext *pAddOpenFlowControllertoOpenFlowLogicalSwitchContext = new AddOpenFlowControllerToOpenFlowLogicalSwitchContext (pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->setOpenFlowLogicalSwitchName                          (pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage->getOpenFlowLogicalSwitchName                          ());
    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->setOpenFlowLogicalSwitchUri                           (pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage->getOpenFlowLogicalSwitchUri                           ());
    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->setOpenFlowControllerIpAddress                        (pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage->getOpenFlowControllerIpAddress                        ());
    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->setOpenFlowControllerPort                             (pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage->getOpenFlowControllerPort                             ());
    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->setOpenFlowcontrollerChannelConnectionDefaultProtocol (pOpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage->getOpenFlowcontrollerChannelConnectionDefaultProtocol ());

    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->holdAll ();
    pAddOpenFlowControllertoOpenFlowLogicalSwitchContext->start   ();
}

void OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep (AddOpenFlowControllerToOpenFlowLogicalSwitchContext *pAddOpenFlowControllerToOpenFlowLogicalSwitchContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep : Entering ..");

    ResourceId status                           = WAVE_MESSAGE_SUCCESS;
    string     openFlowLogicalSwitchName        = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowLogicalSwitchName   ();
    Uri        openFlowLogicalSwitchUri         = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowLogicalSwitchUri    ();
    string     openFlowLogicalSwitchUriString;
    string     openFlowControllerIpAddress      = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowcontrollerIpAddress ();
    UI32       openFlowControllerPort           = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowControllerPort      ();

    openFlowLogicalSwitchUri.toString (openFlowLogicalSwitchUriString);

    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep : Open Flow Logical Switch Name   : " + openFlowLogicalSwitchName);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep : Open Flow Logical Switch URI    : " + openFlowLogicalSwitchUriString);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep : Open Flow Controller Ip Address : " + openFlowControllerIpAddress);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchValidateStep : Open Flow Controller Port       : " + (Integer (openFlowControllerPort)).toString ());

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

        pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->setPOpenFlowLogicalSwitchManagedObject (pOpenFlowLogicalSwitchManagedObject);

        pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->addManagedObjectForGarbageCollection (pOpenFlowLogicalSwitchManagedObject);

        delete pQueryResults;

        status = WAVE_MESSAGE_SUCCESS;
    }
    else // 0 results
    {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pQueryResults);

        status = OPEN_FLOW_LOGICAL_SWITCH_ERROR_DOES_NOT_EXIST;
    }

    pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->executeNextStep (status);
    return;
}

void OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep (AddOpenFlowControllerToOpenFlowLogicalSwitchContext *pAddOpenFlowControllerToOpenFlowLogicalSwitchContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep : Entering ..");

    OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getPOpenFlowLogicalSwitchManagedObject ();

    prismAssert (NULL != pOpenFlowLogicalSwitchManagedObject, __FILE__, __LINE__);

    const string                            openFlowControllerIpAddress       = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowcontrollerIpAddress                        ();
    const UI32                              openFlowControllerPort            = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowControllerPort                             ();
    const OpenFlowChannelConnectionProtocol openFlowChannelConnectionProtocol = pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->getOpenFlowcontrollerChannelConnectionDefaultProtocol ();

    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep : Open Flow Controller Ip Address : " + openFlowControllerIpAddress);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep : Open Flow Controller Port       : " + (Integer (openFlowControllerPort)).toString ());
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchWorker::addOpenFlowControllerToOpenFlowLogicalSwitchCreateStep : Open Flow Controller Protocol   : " + FrameworkToolKit::localize (openFlowChannelConnectionProtocol));

    bool       isKnown = pOpenFlowLogicalSwitchManagedObject->isAKnownOpenFlowController (openFlowControllerIpAddress, openFlowControllerPort);
    ResourceId status  = WAVE_MESSAGE_SUCCESS;

    if (true == isKnown)
    {
        status = OPEN_FLOW_LOGICAL_SWITCH_ERROR_OPEN_CONTROLLER_ALREADY_EXISTS;
    }
    else
    {
        pOpenFlowLogicalSwitchManagedObject->addOpenFlowController (openFlowControllerIpAddress, openFlowControllerPort, openFlowChannelConnectionProtocol);

        updateWaveManagedObject (pOpenFlowLogicalSwitchManagedObject);

        status = WAVE_MESSAGE_SUCCESS;
    }

    pAddOpenFlowControllerToOpenFlowLogicalSwitchContext->executeNextStep (status);
    return;
}

}
