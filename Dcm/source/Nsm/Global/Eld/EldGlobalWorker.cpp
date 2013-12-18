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
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/Eld/NsmGlobalEldUpdateConfigMessage.h"
#include "Nsm/Global/Eld/NsmGlobalSetRxlimMessage.h"
#include "Nsm/Global/Eld/EldGlobalWorker.h"
#include "Nsm/Global/Eld/EldConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
//#include "Nsm/Global/Eld/EldGlobalTypes.h"
#include "Nsm/Local/Eld/NsmSetRxlimMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    EldGlobalWorker::EldGlobalWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        EldConfigManagedObject    EldConfigManagedObject    (pNsmGlobalObjectManager);
        EldConfigManagedObject.setupOrm ();
        addManagedClass (EldConfigManagedObject::getClassName (), this);
		addOperationMap (NSMGLOBALELDUPDATECONFIG, reinterpret_cast<PrismMessageHandler> (&EldGlobalWorker::NsmGlobalEldRxlimConfigMessageHandler));
    }

    EldGlobalWorker::~EldGlobalWorker ()
    {
    }

    PrismMessage  *EldGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMGLOBALELDUPDATECONFIG :
                pPrismMessage = new NsmGlobalSetRxlimMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *EldGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((EldConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new EldConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "EldGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void
    EldGlobalWorker::EldConfGlobalSendToClusterCallback (WaveSendToClusterContext  *ctx_p)
    {
        trace (TRACE_LEVEL_DEBUG, "Inside EldGlobalSendToClusterCallback");
        NSM_PLUG_INFO_S ("Inside EldGlobalSendToClusterCallback");
		LocationId locationid;
		NsmUtils::ClusterGetFirstFailureLocation(ctx_p, locationid);
		ResourceId sendToClusterCompletionStatus = ctx_p->getCompletionStatusForPhase1(locationid);
        PrismLinearSequencerContext             *seq_ctx_p;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        trace (TRACE_LEVEL_DEBUG, string("EldGlobalSendToClusterCallback:") + sendToClusterCompletionStatus);
        return;
    }   

void EldGlobalWorker::updateEldRxlimStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NsmGlobalSetRxlimMessage *pNsmGlobalSetRxlimMessage = dynamic_cast<NsmGlobalSetRxlimMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

		if (NULL == pNsmGlobalSetRxlimMessage) {
        	trace (TRACE_LEVEL_DEBUG, string("EldGlobalWorker:updateEldRxlimStep:: pNsmGlobalSetRxlimMessage is NULL"));
			return;
		}

       if((WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) && (!pNsmGlobalSetRxlimMessage->getSendToLocal()))
        {
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }
		
		NsmSetRxlimMessage *m = new NsmSetRxlimMessage();

        WaveSendToClusterContext    *send_ctx_p;

		m->setOpCode(pNsmGlobalSetRxlimMessage->getCmdCode());

		if (pNsmGlobalSetRxlimMessage->getCmdCode() == ELD_GLOBAL_CONF_VCS_RX_LIMIT) {
			m->setRxlim (pNsmGlobalSetRxlimMessage->getRxlim());
		}
		
		if (pNsmGlobalSetRxlimMessage->getCmdCode() == ELD_GLOBAL_CONF_HELLO_TIME) {
			m->setHelloInt (pNsmGlobalSetRxlimMessage->getHelloInt());
		}
		
		if (pNsmGlobalSetRxlimMessage->getCmdCode() == ELD_GLOBAL_CONF_SHUTDOWN_TIME) {
			m->setShutdnTime (pNsmGlobalSetRxlimMessage->getShutdnTime());
		}

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&EldGlobalWorker::EldConfGlobalSendToClusterCallback),pPrismLinearSequencerContext);

        prismAssert(NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);
		
		pNsmGlobalSetRxlimMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

		return;
	}

	void EldGlobalWorker::updateEldGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int createFlag = 1;
		//int status = 0;

		EldConfigManagedObject *pEldConfigManagedObject = NULL;
		NsmGlobalSetRxlimMessage *pNsmGlobalSetRxlimMessage = dynamic_cast<NsmGlobalSetRxlimMessage*> (pPrismLinearSequencerContext->getPPrismMessage ()); 
        NSM_PLUG_ASSERT_PTR (pNsmGlobalSetRxlimMessage); 

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(EldConfigManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, "NO PREVIOUS MO: numberOfResults = 0");
				createFlag = 1;
				//status = WAVE_MESSAGE_SUCCESS;
			} else if (1 == numberOfResults) {
				pEldConfigManagedObject  = dynamic_cast<EldConfigManagedObject*>((*pResults)[0]);
                NSM_PLUG_ASSERT_PTR (pEldConfigManagedObject);

				trace (TRACE_LEVEL_DEBUG, "PREVIOUS MO exists: numberOfResults = 1");
				createFlag = 0;
				//status = WAVE_MESSAGE_SUCCESS;
			} else {
				//status = WAVE_MESSAGE_ERROR;
			}
		}
		if (createFlag == 0) {
			trace (TRACE_LEVEL_DEBUG, "createFlag = 0");
			updateWaveManagedObject (pEldConfigManagedObject);
			prismAssert (NULL != pEldConfigManagedObject, __FILE__, __LINE__);
		} else {
			trace (TRACE_LEVEL_DEBUG, "createFlag = 1");
			pEldConfigManagedObject = new EldConfigManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
			prismAssert (NULL != pEldConfigManagedObject, __FILE__, __LINE__);
		}

				if (pNsmGlobalSetRxlimMessage->getCmdCode() == ELD_GLOBAL_CONF_VCS_RX_LIMIT) {
	        		pEldConfigManagedObject->setRxlim (pNsmGlobalSetRxlimMessage->getRxlim());
			trace (TRACE_LEVEL_DEBUG, "EldConfigManagedObject MSG_SET_RXLIM");
					NSM_PLUG_DBG_S(string("ELD:Rxlim: ") + pNsmGlobalSetRxlimMessage->getRxlim());
				} else if (pNsmGlobalSetRxlimMessage->getCmdCode() == ELD_GLOBAL_CONF_HELLO_TIME) {
		        	pEldConfigManagedObject->setHello (pNsmGlobalSetRxlimMessage->getHelloInt());
					NSM_PLUG_DBG_S(string("ELD:Hello: ") + pNsmGlobalSetRxlimMessage->getHelloInt()); 
				} else if (pNsmGlobalSetRxlimMessage->getCmdCode() == ELD_GLOBAL_CONF_SHUTDOWN_TIME){
	        		pEldConfigManagedObject->setShutdown (pNsmGlobalSetRxlimMessage->getShutdnTime());
					NSM_PLUG_DBG_S(string("ELD:Shutdown: ") + pNsmGlobalSetRxlimMessage->getShutdnTime()); 
				}
        if (pResults) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete pResults;
        }
        if (createFlag) {
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection (pEldConfigManagedObject);
        }

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		return;
	}

	void EldGlobalWorker::NsmGlobalEldRxlimConfigMessageHandler(NsmGlobalSetRxlimMessage *pNsmGlobalSetRxlimMessage)
{
 	PrismLinearSequencerStep sequencerSteps[] =
    {
// Programming Protocol Daemons goes here
    	reinterpret_cast<PrismLinearSequencerStep> (&EldGlobalWorker::updateEldRxlimStep),
		reinterpret_cast<PrismLinearSequencerStep>(&EldGlobalWorker::prismLinearSequencerStartTransactionStep),

// Your configuration change code goes here
		reinterpret_cast<PrismLinearSequencerStep>(&EldGlobalWorker::updateEldGlobalConfigMOStep),

		reinterpret_cast<PrismLinearSequencerStep>(&EldGlobalWorker::prismLinearSequencerCommitTransactionStep),

		reinterpret_cast<PrismLinearSequencerStep>(&EldGlobalWorker::prismLinearSequencerSucceededStep),
		reinterpret_cast<PrismLinearSequencerStep>(&EldGlobalWorker::prismLinearSequencerFailedStep)
	};
    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pNsmGlobalSetRxlimMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
	pPrismLinearSequencerContext->holdAll();
    pPrismLinearSequencerContext->start ();
}
}
