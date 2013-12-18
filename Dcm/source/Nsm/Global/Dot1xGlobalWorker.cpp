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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/Dot1xGlobalWorker.h"
#include "Nsm/Global/NsmGlobalDot1xUpdateConfigMessage.h"
#include "Nsm/Global/Dot1xConfigGlobalManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
namespace DcmNs
{

    Dot1xGlobalWorker::Dot1xGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        Dot1xConfigGlobalManagedObject    Dot1xConfigGlobalManagedObject    (pNsmGlobalObjectManager);
        Dot1xConfigGlobalManagedObject.setupOrm ();
        addManagedClass (Dot1xConfigGlobalManagedObject::getClassName (), this);
        addOperationMap (NSMGLOBALDOT1XUPDATECONFIG, reinterpret_cast<PrismMessageHandler> (&Dot1xGlobalWorker::NsmGlobalDot1xUpdateConfigMessageHandler));
    }

    Dot1xGlobalWorker::~Dot1xGlobalWorker ()
    {
    }

    PrismMessage  *Dot1xGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALDOT1XUPDATECONFIG :
                pPrismMessage = new NsmGlobalDot1xUpdateConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *Dot1xGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((Dot1xConfigGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new Dot1xConfigGlobalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "Dot1xGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  Dot1xGlobalWorker::NsmGlobalDot1xUpdateConfigMessageHandler( NsmGlobalDot1xUpdateConfigMessage *pNsmGlobalDot1xUpdateConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xGlobalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmGlobalDot1xUpdateConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

}
