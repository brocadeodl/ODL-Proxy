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


#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "OpenFlow/Switch/Core/CreateOpenFlowLogicalSwitchContext.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "DcmResourceIdEnums.h"

using namespace DcmNs;
using namespace DcmNs;

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker (OpenFlowSwitchObjectManager *pOpenFlowSwitchObjectManager)
    : WaveWorker (pOpenFlowSwitchObjectManager)
{
    OpenFlowLogicalSwitchManagedObject openFlowLogicalSwitchManagedObject (pOpenFlowSwitchObjectManager);

    openFlowLogicalSwitchManagedObject.setupOrm ();

    addManagedClass (OpenFlowLogicalSwitchManagedObject::getClassName (), this);

    addOperationMap (OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_LOGICAL_SWITCH, reinterpret_cast<PrismMessageHandler> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowLogicalSwitchMessageHandler));
}

OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::~OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker ()
{
}

PrismMessage *OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_LOGICAL_SWITCH :
            pPrismMessage = new OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage;
            break;

        default :
            pPrismMessage = NULL;
            break;
    }

    return (pPrismMessage);
}


WaveManagedObject *OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((OpenFlowLogicalSwitchManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new OpenFlowLogicalSwitchManagedObject (reinterpret_cast<OpenFlowSwitchObjectManager *> (getPWaveObjectManager ()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }

    return (pWaveManagedObject);
}

void OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowLogicalSwitchMessageHandler (OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage *pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowLogicalSwitchValidateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowLogicalSwitchCreateStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::prismLinearSequencerFailedStep)
    };

    CreateOpenFlowLogicalSwitchContext *pCreateOpenFlowLogicalSwitchContext = new CreateOpenFlowLogicalSwitchContext (pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pCreateOpenFlowLogicalSwitchContext->setOpenFlowLogicalSwitchName (pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage->getOpenFlowLogicalSwitchName ());
    pCreateOpenFlowLogicalSwitchContext->setOpenFlowLogicalSwitchUri  (pOpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage->getOpenFlowLogicalSwitchUri  ());

    pCreateOpenFlowLogicalSwitchContext->holdAll ();
    pCreateOpenFlowLogicalSwitchContext->start   ();
}

void OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowLogicalSwitchValidateStep (CreateOpenFlowLogicalSwitchContext *pCreateOpenFlowLogicalSwitchContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowSwitchValidateStep : Entering ...");

    ResourceId status                   = WAVE_MESSAGE_SUCCESS;
    string     openFlowLogicalSwitchName       = pCreateOpenFlowLogicalSwitchContext->getOpenFlowLogicalSwitchName ();
    Uri        openFlowLogicalSwitchUri        = pCreateOpenFlowLogicalSwitchContext->getOpenFlowLogicalSwitchUri  ();
    string     openFlowLogicalSwitchUriString;
    UI32       numberOfResults          = 0;

    openFlowLogicalSwitchUri.toString (openFlowLogicalSwitchUriString);

    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowSwitchValidateStep : Open Flow Logical Switch Name : " + openFlowLogicalSwitchName);
    trace (TRACE_LEVEL_DEBUG, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowSwitchValidateStep : Open Flow Logical Switch URI  : " + openFlowLogicalSwitchUriString);

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (OpenFlowLogicalSwitchManagedObject::getClassName ());

    synchronousQueryContext.addOrAttribute (new AttributeString (openFlowLogicalSwitchName, "openFlowLogicalSwitchName"));
    synchronousQueryContext.addOrAttribute (new AttributeUri    (openFlowLogicalSwitchUri,  "openFlowLogicalSwitchUri"));

    status = querySynchronouslyForCount (&synchronousQueryContext, numberOfResults);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowSwitchValidateStep : Query failed : Status" + FrameworkToolKit::localize (status));
    }
    else
    {
        if (0 != numberOfResults)
        {
            trace (TRACE_LEVEL_ERROR, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowSwitchValidateStep : An Open Flow Logical Switch with the given parameters already exists.");

            status = OPEN_FLOW_LOGICAL_SWITCH_ERROR_ALREADY_EXISTS;
        }
    }

    pCreateOpenFlowLogicalSwitchContext->executeNextStep (status);
    return;
}

void OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowLogicalSwitchCreateStep (CreateOpenFlowLogicalSwitchContext *pCreateOpenFlowLogicalSwitchContext)
{
    trace (TRACE_LEVEL_DEVEL, "OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchWorker::createOpenFlowSwitchCreateStep : Entering ...");

    string                              openFlowLogicalSwitchName                  = pCreateOpenFlowLogicalSwitchContext->getOpenFlowLogicalSwitchName ();
    Uri                                 openFlowLogicalSwitchUri                   = pCreateOpenFlowLogicalSwitchContext->getOpenFlowLogicalSwitchUri  ();

    OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject = new OpenFlowLogicalSwitchManagedObject (getPWaveObjectManager ());

    pOpenFlowLogicalSwitchManagedObject->setOpenFlowLogicalSwitchName (openFlowLogicalSwitchName);
    pOpenFlowLogicalSwitchManagedObject->setOpenFlowLogicalSwitchUri  (openFlowLogicalSwitchUri);

    prismAssert (NULL != pOpenFlowLogicalSwitchManagedObject, __FILE__, __LINE__);

    pCreateOpenFlowLogicalSwitchContext->addManagedObjectForGarbageCollection (pOpenFlowLogicalSwitchManagedObject);

    pCreateOpenFlowLogicalSwitchContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    return;
}

}

