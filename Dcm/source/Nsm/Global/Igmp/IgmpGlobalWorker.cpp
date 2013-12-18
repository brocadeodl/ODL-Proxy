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
 *   Author : agidwani                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/Igmp/IgmpGlobalWorker.h"
#include "Nsm/Global/Igmp/IgmpPoSviIntfGlobalConfigMessage.h"
#include "Nsm/Global/Igmp/IgmpSgIntfGlobalConfigMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/Igmp/IgmpPoSviIntfLocalConfigMessage.h"
#include "Nsm/Local/Igmp/IgmpSgIntfLocalConfigMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Igmp/IgmpToolkit.h"
#include "ClientInterface/Igmps/IgmpsUtils.h"

namespace DcmNs
{

IgmpGlobalWorker::IgmpGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
    : WaveWorker  (pNsmGlobalObjectManager)
{
    addOperationMap (IGMPPOSVIINTFGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpGlobalWorker::IgmpPoSviIntfGlobalConfigMessageHandler));
    addOperationMap (IGMPSGINTFGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpGlobalWorker::IgmpSgIntfGlobalConfigMessageHandler));
}

IgmpGlobalWorker::~IgmpGlobalWorker ()
{
}

PrismMessage  *IgmpGlobalWorker::createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {

        case IGMPPOSVIINTFGLOBALCONFIG :
            pPrismMessage = new IgmpPoSviIntfGlobalConfigMessage ();
            break;
        case IGMPSGINTFGLOBALCONFIG :
            pPrismMessage = new IgmpSgIntfGlobalConfigMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject  *IgmpGlobalWorker::createManagedObjectInstance(const string &managedClassName)
{
    return NULL;
}

/***************************************************************************/
/*     Functions for handling config(except SG) on SVIs & port-channels    */
/***************************************************************************/

void  IgmpGlobalWorker::IgmpPoSviIntfGlobalConfigMessageHandler(IgmpPoSviIntfGlobalConfigMessage *pIgmpPoSviIntfGlobalConfigMessage)
{
// Validations and Queries should go here

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::UpdateIgmpPoSviIntfGlobalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIgmpPoSviIntfGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}

/* Function to send the config to the local plugin */
ResourceId IgmpGlobalWorker::UpdateIgmpPoSviIntfGlobalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    IgmpPoSviIntfGlobalConfigMessage *pIgmpPoSviIntfGlobalConfigMessage = 
        dynamic_cast<IgmpPoSviIntfGlobalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
    IgmpPoSviIntfLocalConfigMessage *m = new IgmpPoSviIntfLocalConfigMessage();

    m->setLmqi(pIgmpPoSviIntfGlobalConfigMessage->getLmqi());
    m->setQmrt(pIgmpPoSviIntfGlobalConfigMessage->getQmrt());
    m->setImmediateLeave(pIgmpPoSviIntfGlobalConfigMessage->getImmediateLeave());
    m->setQueryInterval(pIgmpPoSviIntfGlobalConfigMessage->getQueryInterval());
    m->setIfName(pIgmpPoSviIntfGlobalConfigMessage->getIfName());
    m->setIfType(pIgmpPoSviIntfGlobalConfigMessage->getIfType());
    m->setOpCode(pIgmpPoSviIntfGlobalConfigMessage->getOpCode());

    // FIXME We're not supporting IGMP on VCS in the first release of
    // Hydra so we should return an error if VCS is enabled
    status = sendSynchronously(m);
    pIgmpPoSviIntfGlobalConfigMessage->setCompletionStatus(m->getCompletionStatus());

    delete m;

    if (status == WAVE_MESSAGE_SUCCESS && pIgmpPoSviIntfGlobalConfigMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__) + string("   ") + toString(pIgmpPoSviIntfGlobalConfigMessage->getCompletionStatus()));
        return pIgmpPoSviIntfGlobalConfigMessage->getCompletionStatus();
    }
    return status;
}

/*******************************************************************************/
/*       Functions for handling SG config on SVIs and port-channels            */
/*******************************************************************************/

void  IgmpGlobalWorker::IgmpSgIntfGlobalConfigMessageHandler( IgmpSgIntfGlobalConfigMessage *pIgmpSgIntfGlobalConfigMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::UpdateIgmpSgIntfGlobalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpGlobalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIgmpSgIntfGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}

/* Function to send the SG config to the local plugin */
ResourceId IgmpGlobalWorker::UpdateIgmpSgIntfGlobalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    IgmpSgIntfGlobalConfigMessage *pIgmpSgIntfGlobalConfigMessage = 
        dynamic_cast<IgmpSgIntfGlobalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
    IgmpSgIntfLocalConfigMessage *m = new IgmpSgIntfLocalConfigMessage();

    m->setIfName(pIgmpSgIntfGlobalConfigMessage->getIfName());
    m->setIfType(pIgmpSgIntfGlobalConfigMessage->getIfType());
    m->setGroupAddress(pIgmpSgIntfGlobalConfigMessage->getGroupAddress());
    m->setNegation(pIgmpSgIntfGlobalConfigMessage->getNegation());

    status = sendSynchronously(m);
    pIgmpSgIntfGlobalConfigMessage->setCompletionStatus(m->getCompletionStatus());

    delete m;

    if (status == WAVE_MESSAGE_SUCCESS && pIgmpSgIntfGlobalConfigMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__) + string("   ") + toString(pIgmpSgIntfGlobalConfigMessage->getCompletionStatus()));
        return pIgmpSgIntfGlobalConfigMessage->getCompletionStatus();
    }
    return status;
}

}
