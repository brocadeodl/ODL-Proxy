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
 *   Copyright (C) 2011-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kverma                                                       *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/Pim/PimIntfCfgGlobalWorker.h"
#include "Nsm/Global/Pim/PimIntfCfgGlobalMsg.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Local/Pim/PimIntfCfgMessage.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    PimIntfCfgGlobalWorker::PimIntfCfgGlobalWorker(NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        addOperationMap (NSMGLOBALPIMINTFCFG, 
            reinterpret_cast<PrismMessageHandler>
            (&PimIntfCfgGlobalWorker::PimIntfCfgGlobalMsgHandler));
    }

    PimIntfCfgGlobalWorker::~PimIntfCfgGlobalWorker ()
    {
    }

    PrismMessage* PimIntfCfgGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMGLOBALPIMINTFCFG:
                pPrismMessage = new PimIntfCfgGlobalMsg ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject* PimIntfCfgGlobalWorker::createManagedObjectInstance(const 
        string &managedClassName)
    {
        return NULL;
    }
    
    void PimIntfCfgGlobalWorker::PimIntfCfgGlobalMsgHandler(
        PimIntfCfgGlobalMsg *pPimIntfCfgGlobalMsg)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimIntfCfgGlobalWorker::sendPimCfgGlbStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimIntfCfgGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimIntfCfgGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimIntfCfgGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(
                &PimIntfCfgGlobalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pSeqCtx = 
            new PrismSynchronousLinearSequencerContext(pPimIntfCfgGlobalMsg, 
                this, sequencerSteps, 
                sizeof(sequencerSteps)/sizeof(sequencerSteps[0]));

        pSeqCtx->execute();
    }
    
    ResourceId PimIntfCfgGlobalWorker::sendPimCfgGlbStep(
        PrismSynchronousLinearSequencerContext *pSeqCtx)
    {
        trace (TRACE_LEVEL_DEBUG, string("PimIntfCfgGlobalWorker::sendPimCfgGlbStep: ")
            + string("Entered...."));

        PimIntfCfgGlobalMsg *pPimIntfCfgGlobalMsg = 
            dynamic_cast<PimIntfCfgGlobalMsg*> (pSeqCtx->getPPrismMessage ());
        int status = WAVE_MESSAGE_ERROR;
        PimIntfCfgMessage *pMsg = new PimIntfCfgMessage();

        pMsg->setMsgType(pPimIntfCfgGlobalMsg->getMsgType());
        pMsg->setCmdCode(pPimIntfCfgGlobalMsg->getCmdCode());
        pMsg->setIntfType(pPimIntfCfgGlobalMsg->getIntfType());
        pMsg->setIntfName(pPimIntfCfgGlobalMsg->getIntfName());
        pMsg->setEnable(pPimIntfCfgGlobalMsg->getEnable());
        pMsg->setDrPriority(pPimIntfCfgGlobalMsg->getDrPriority());
        pMsg->setMultBoundaryAcl(pPimIntfCfgGlobalMsg->getMultBoundaryAcl());
        pMsg->setPimBorder(pPimIntfCfgGlobalMsg->getPimBorder());
        pMsg->setTtlThold(pPimIntfCfgGlobalMsg->getTtlThold());

        status = sendSynchronously(pMsg);
        pPimIntfCfgGlobalMsg->setCompletionStatus(pMsg->getCompletionStatus());

        if(status == WAVE_MESSAGE_SUCCESS) {
            if(pPimIntfCfgGlobalMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) 
                status = pPimIntfCfgGlobalMsg->getCompletionStatus();
            else 
                trace(TRACE_LEVEL_DEBUG, string("PimIntfCfgGlobalWorker::sendPimCfgGlbStep: ")
                    + string("Successfully sent message to local worker."));
        }
        else {
            trace(TRACE_LEVEL_DEBUG, string("PimIntfCfgGlobalWorker::sendPimCfgGlbStep: ")
                + string("Failed to send message to local worker. Status : ") 
                + FrameworkToolKit::localize (status));
        }
        
        delete pMsg;
        return status;
    }
}
