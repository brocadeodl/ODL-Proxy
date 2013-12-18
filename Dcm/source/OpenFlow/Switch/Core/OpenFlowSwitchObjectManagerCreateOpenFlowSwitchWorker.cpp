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

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "OpenFlow/Switch/Core/CreateOpenFlowSwitchContext.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowCapableSwitchManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"


namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker (OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager)
    : WaveWorker (pOpenFlowSwitchObjectManager)
{
    OpenFlowCapableSwitchManagedObject openFlowCapableSwitchManagedObject (pOpenFlowSwitchObjectManager);

    openFlowCapableSwitchManagedObject.setupOrm ();

    addManagedClass (OpenFlowCapableSwitchManagedObject::getClassName (), this);

    addOperationMap (OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_SWITCH, reinterpret_cast<PrismMessageHandler> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchMessageHandler));
}

OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::~OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker ()
{
}

PrismMessage *OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_SWITCH :
            pPrismMessage = new OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage;
            break;

        default :
            pPrismMessage = NULL;
            break;
    }

    return (pPrismMessage);
}

void OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchMessageHandler (OpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage *pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchCreateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::prismLinearSequencerFailedStep)
    };

    CreateOpenFlowSwitchContext *pCreateOpenFlowSwitchContext = new CreateOpenFlowSwitchContext (pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pCreateOpenFlowSwitchContext->setOpenFlowSwitchName (pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage->getOpenFlowSwitchName ());
    pCreateOpenFlowSwitchContext->setOpenFlowSwitchUri  (pOpenFlowSwitchObjectManagerCreateOpenFlowSwitchMessage->getOpenFlowSwitchUri  ());

    pCreateOpenFlowSwitchContext->holdAll ();
    pCreateOpenFlowSwitchContext->start   ();
}

void OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep (CreateOpenFlowSwitchContext *pCreateOpenFlowSwitchContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep : Entering ...");

    ResourceId status                   = WAVE_MESSAGE_SUCCESS;
    string     openFlowSwitchName       = pCreateOpenFlowSwitchContext->getOpenFlowSwitchName ();
    Uri        openFlowSwitchUri        = pCreateOpenFlowSwitchContext->getOpenFlowSwitchUri  ();
    string     openFlowSwitchUriString;
    UI32       numberOfResults          = 0;

    openFlowSwitchUri.toString (openFlowSwitchUriString);

    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep : Open Flow Switch Name : " + openFlowSwitchName);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep : Open Flow Switch URI  : " + openFlowSwitchUriString);

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (OpenFlowCapableSwitchManagedObject::getClassName ());

    synchronousQueryContext.addOrAttribute (new AttributeString (openFlowSwitchName, "openFlowSwitchName"));
    synchronousQueryContext.addOrAttribute (new AttributeUri    (openFlowSwitchUri,  "openFlowSwitchUri"));

    status = querySynchronouslyForCount (&synchronousQueryContext, numberOfResults);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep : Query failed : Status" + FrameworkToolKit::localize (status));
    }
    else
    {
        if (0 != numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchValidateStep : An Open Flow Capable Switch with the given parameters already exists.");

            status = OPEN_FLOW_CAPABLE_SWITCH_ERROR_ALREADY_EXISTS;
        }
    }

    pCreateOpenFlowSwitchContext->executeNextStep (status);
    return;
}

void OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchCreateStep (CreateOpenFlowSwitchContext *pCreateOpenFlowSwitchContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerCreateOpenFlowSwitchWorker::createOpenFlowSwitchCreateStep : Entering ...");

    string                              openFlowSwitchName                  = pCreateOpenFlowSwitchContext->getOpenFlowSwitchName ();
    Uri                                 openFlowSwitchUri                   = pCreateOpenFlowSwitchContext->getOpenFlowSwitchUri  ();

    OpenFlowCapableSwitchManagedObject *pOpenFlowCapableSwitchManagedObject = new OpenFlowCapableSwitchManagedObject (getPWaveObjectManager ());

    pOpenFlowCapableSwitchManagedObject->setOpenFlowSwitchName (openFlowSwitchName);
    pOpenFlowCapableSwitchManagedObject->setOpenFlowSwitchUri  (openFlowSwitchUri);

    prismAssert (NULL != pOpenFlowCapableSwitchManagedObject, __FILE__, __LINE__);

    pCreateOpenFlowSwitchContext->addManagedObjectForGarbageCollection (pOpenFlowCapableSwitchManagedObject);

    pCreateOpenFlowSwitchContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

}

