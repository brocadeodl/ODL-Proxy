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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Utils/Capabilities.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Igmps/Global/IgmpsGlobalObjectManager.h"
#include "Igmps/Global/IgmpsGlobalWorker.h"
#include "Igmps/Global/IgmpsGlobalUpdateIgmpsGlobalConfigMessage.h"
#include "Igmps/Global/IgmpsGlobalConfigManagedObject.h"
#include "Igmps/Global/IgmpsGlobalTypes.h"
#include "Igmps/Local/IgmpsUpdateIgmpsConfigMessage.h"
#include "Nsm/Global/Igmps/NsmGlobalUpdateIgmpsVlanConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"
namespace DcmNs
{

    IgmpsGlobalWorker::IgmpsGlobalWorker ( IgmpsGlobalObjectManager *pIgmpsGlobalObjectManager)
        : WaveWorker  (pIgmpsGlobalObjectManager)
    {
        IgmpsGlobalConfigManagedObject    IgmpsGlobalConfigManagedObject    (pIgmpsGlobalObjectManager);
        IgmpsGlobalConfigManagedObject.setupOrm ();
        addManagedClass (IgmpsGlobalConfigManagedObject::getClassName (), this);
        addOperationMap (IGMPSGLOBALUPDATEIGMPSGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpsGlobalWorker::IgmpsGlobalUpdateIgmpsGlobalConfigMessageHandler));
    }

    IgmpsGlobalWorker::~IgmpsGlobalWorker ()
    {
    }

    PrismMessage  *IgmpsGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case IGMPSGLOBALUPDATEIGMPSGLOBALCONFIG :
                pPrismMessage = new IgmpsGlobalUpdateIgmpsGlobalConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IgmpsGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((IgmpsGlobalConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IgmpsGlobalConfigManagedObject(dynamic_cast<IgmpsGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IgmpsGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void
    IgmpsGlobalWorker::IgmpsGlobalConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace (TRACE_LEVEL_DEVEL, "IgmpsGlobalConfigSendToClusterCallback::Entering");
        ResourceId sendToClusterCompletionStatus = pWaveSendToClusterContext->getCompletionStatus ();
        PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
        IgmpsGlobalUpdateIgmpsGlobalConfigMessage *pIgmpsGlobalUpdateIgmpsGlobalConfigMessage
                        = dynamic_cast<IgmpsGlobalUpdateIgmpsGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        NSM_PLUG_ASSERT_PTR (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage);

        if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS)
            trace (TRACE_LEVEL_DEVEL, "Phase 1 message completely/partially succeeded on all locations");
		else
		    trace (TRACE_LEVEL_DEVEL, "Phase 1 message failed on all locations");

        NsmGlobalUpdateIgmpsVlanConfigMessage *pIgmpsVlanConfigMessage = new NsmGlobalUpdateIgmpsVlanConfigMessage();
        pIgmpsVlanConfigMessage->setOpCode (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getOpCode());
        pIgmpsVlanConfigMessage->setEnable (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getEnable());
        pIgmpsVlanConfigMessage->setRestrictUnknownMcast(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getRestrictUnknownMcast());
        ResourceId status = sendSynchronously (pIgmpsVlanConfigMessage);
        /* No errors expected. */
        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, "IgmpsGlobalWorker::IgmpsGlobalConfigSendToClusterCallback:Failed to send IgmpsVlanConfigMessage");
        }
        delete pIgmpsVlanConfigMessage;

        ResourceId         returnStatus = WAVE_MESSAGE_SUCCESS;

		/*
		 * Sudhanshu : Changes for partial success(Hercules)  
		 */

        vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        for (UI32 i = 0; i < locations.size(); i++) 
		{
            returnStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locations[i]); 	
			trace (TRACE_LEVEL_INFO, string("IgmpsGlobalConfigSendToClusterCallback::getCompletionStatus: ") 
						+ returnStatus + (" for location: ") + locations[i]);
		}

		returnStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext); 
	     
		delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
		delete pWaveSendToClusterContext->getPPrismMessageForPhase2();
		delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_DEVEL, "IgmpsGlobalConfigSendToClusterCallback::Done");

        pPrismLinearSequencerContext->executeNextStep(returnStatus);

		return;
    }

	void IgmpsGlobalWorker::updateIgmpsGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		IgmpsGlobalUpdateIgmpsGlobalConfigMessage *pIgmpsGlobalUpdateIgmpsGlobalConfigMessage
            = dynamic_cast<IgmpsGlobalUpdateIgmpsGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage);

        IgmpsUpdateIgmpsConfigMessage *m = new IgmpsUpdateIgmpsConfigMessage();

		trace (TRACE_LEVEL_DEBUG, "IgmpsGlobalWorker::updateIgmpsGlobalConfigStep: Entered....");

        /* create clustercontext and initialize with callback func */
               m->setOpCode(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getOpCode());
            switch (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getOpCode()) {
            case IGMPS_GLOBAL_SNOOP_ENABLE:                
                 m->setEnable(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getEnable());
                break;
            case IGMPS_GLOBAL_RESTRICT_UNKNOWN_MCAST_ENABLE:
                 m->setRestrictUnknownMcast(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getRestrictUnknownMcast());
                break;
            default:
             if(m)
                delete m;
               return;// WRC_IGMP_ERR_INVALID_COMMAND; 
               break;
            }
        /* Set Phase 1 message */
        WaveSendToClusterContext    *pWaveSendToClusterContext;
        pWaveSendToClusterContext = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&IgmpsGlobalWorker::IgmpsGlobalConfigSendToClusterCallback),
            pPrismLinearSequencerContext);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(m);

		/* Sudhanshu start: Changes for partial success(Hercules) */
		bool partialFlag = true;
		pWaveSendToClusterContext->setPartialSuccessFlag(partialFlag);
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(partialFlag);

		/* Sudhanshu end :  Changes for partial success(Hercules) */

        /* Send the message to all nodes in the cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

		/* Sudhanshu start: Changes for partial success(Hercules) */
		trace (TRACE_LEVEL_DEBUG, "IgmpsGlobalWorker::updateIgmpsGlobalConfigStep: Ended....");

		/* Sudhanshu end: Changes for partial success(Hercules) */
        return;
	}

	void IgmpsGlobalWorker::validateIgmpsGlobalConfigStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		ResourceId         returnStatus = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEVEL, "ValidateIgmpsGlobalConfigStep:Entering");
		IgmpsGlobalUpdateIgmpsGlobalConfigMessage *pIgmpsGlobalUpdateIgmpsGlobalConfigMessage
        	    = dynamic_cast<IgmpsGlobalUpdateIgmpsGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		NSM_PLUG_ASSERT_PTR (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage);
		trace (TRACE_LEVEL_DEVEL, string("validateIgmpsGlobalConfigStep:Opcode") +pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getOpCode());
		trace (TRACE_LEVEL_DEVEL, string("validateIgmpsGlobalConfigStep:restrict") +pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getRestrictUnknownMcast());
		switch (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getOpCode()) {
			case IGMPS_GLOBAL_RESTRICT_UNKNOWN_MCAST_ENABLE:
			if(!(Capabilities::isCapabilityPresent(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST)))
				returnStatus = WRC_IGMP_ERR_GLOBAL_SNOOP_RESTRICT_NOT_SUPPORTED_PLATFORM;
			else
				trace (TRACE_LEVEL_DEVEL, string("validateIgmpsGlobalConfigStep:capabilities returned true") );
		}
        	pPrismLinearSequencerContext->executeNextStep(returnStatus);
	}
 
	void IgmpsGlobalWorker::updateIgmpsGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int createFlag = 1;
		int status = 0;

		IgmpsGlobalConfigManagedObject *pIgmpsGlobalConfigManagedObject = NULL;
		IgmpsGlobalUpdateIgmpsGlobalConfigMessage *pIgmpsGlobalUpdateIgmpsGlobalConfigMessage = dynamic_cast<IgmpsGlobalUpdateIgmpsGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage);
                int cmdCode = pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getOpCode();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IgmpsGlobalConfigManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
				createFlag = 1;
				status = WAVE_MESSAGE_SUCCESS;
			} else if (1 == numberOfResults) {
				pIgmpsGlobalConfigManagedObject  = dynamic_cast<IgmpsGlobalConfigManagedObject*>((*pResults)[0]);
                NSM_PLUG_ASSERT_PTR (pIgmpsGlobalConfigManagedObject);

				trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
				createFlag = 0;
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}
		if (createFlag == 0) {
			trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
            if (IS_CMDCODE_SET(cmdCode, IGMPS_GLOBAL_SNOOP_ENABLE)) {
                pIgmpsGlobalConfigManagedObject->addAttributeToBeUpdated("enable");
                pIgmpsGlobalConfigManagedObject->setEnable(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getEnable());
			    trace (TRACE_LEVEL_DEVEL, "IgmpsGlobalWorkercase MSG_ENABLE ");
            } 
            if (IS_CMDCODE_SET(cmdCode, IGMPS_GLOBAL_RESTRICT_UNKNOWN_MCAST_ENABLE)) {
                pIgmpsGlobalConfigManagedObject->addAttributeToBeUpdated("restrictunknownmcast");
                pIgmpsGlobalConfigManagedObject->setRestrictUnknownMcast(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getRestrictUnknownMcast());
                trace (TRACE_LEVEL_DEVEL, "IgmpsGlobalWorkercase MSG_RESTRICT_UNKNOWN_MCAST ");
            }
			updateWaveManagedObject (pIgmpsGlobalConfigManagedObject);
			prismAssert (NULL != pIgmpsGlobalConfigManagedObject, __FILE__, __LINE__);
		} else {
			trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
			pIgmpsGlobalConfigManagedObject = new IgmpsGlobalConfigManagedObject(dynamic_cast<IgmpsGlobalObjectManager*>(getPWaveObjectManager()));
			prismAssert (NULL != pIgmpsGlobalConfigManagedObject, __FILE__, __LINE__);
			trace (TRACE_LEVEL_DEVEL, "IgmpsGlobalWorkercase MSG_ENABLE ");
			if (IS_CMDCODE_SET(cmdCode, IGMPS_GLOBAL_SNOOP_ENABLE)) {
				pIgmpsGlobalConfigManagedObject->setEnable(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getEnable());
			}
			if (IS_CMDCODE_SET(cmdCode, IGMPS_GLOBAL_RESTRICT_UNKNOWN_MCAST_ENABLE)) {
				pIgmpsGlobalConfigManagedObject->setRestrictUnknownMcast(pIgmpsGlobalUpdateIgmpsGlobalConfigMessage->getRestrictUnknownMcast());
			}
		}

        if (pResults) {
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
			delete pResults;
        }
        if (createFlag) {
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection (pIgmpsGlobalConfigManagedObject);
        }

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // Nothing to do for now.
            // FIXME : Re-evaluate what needs to be done.
        }

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		return;
	}

    void  IgmpsGlobalWorker::IgmpsGlobalUpdateIgmpsGlobalConfigMessageHandler( IgmpsGlobalUpdateIgmpsGlobalConfigMessage *pIgmpsGlobalUpdateIgmpsGlobalConfigMessage)
    {
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::validateIgmpsGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::updateIgmpsGlobalConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::updateIgmpsGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IgmpsGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pIgmpsGlobalUpdateIgmpsGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start ();
    }

}


