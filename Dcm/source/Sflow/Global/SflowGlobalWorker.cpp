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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h" 
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Utils/Capabilities.h"

#include "Policer/Global/PoClassMapManagedObject.h"

#include "Sflow/Global/SflowGlobalObjectManager.h"
#include "Sflow/Global/SflowGlobalWorker.h"
#include "Sflow/Global/SflowGlobalCreateSflowGlobalConfigMessage.h"
#include "Sflow/Global/SflowGlobalDeleteSflowGlobalConfigMessage.h"
#include "Sflow/Global/SflowGlobalSflowCollectorMessage.h"
#include "Sflow/Global/SflowGlobalSflowProfileMessage.h"

#include "Sflow/Local/SflowCreateSflowGlobalConfigSPMessage.h"
#include "Sflow/Local/SflowDeleteSflowGlobalConfigSPMessage.h"
#include "Sflow/Local/SflowProfilePrismMessage.h"
#include "Sflow/Global/SflowGlobalConfigManagedObject.h"
#include "Sflow/Global/SflowCollectorManagedObject.h"
#include "Sflow/Global/SflowProfileManagedObject.h"
#include "Sflow/Global/SflowGlobalTypes.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"


namespace DcmNs
{

    SflowGlobalWorker::SflowGlobalWorker ( SflowGlobalObjectManager *pSflowGlobalObjectManager) : WaveWorker  (pSflowGlobalObjectManager)
    {
        SflowGlobalConfigManagedObject    SflowGlobalConfigManagedObject    (pSflowGlobalObjectManager);
        SflowGlobalConfigManagedObject.setupOrm ();
        addManagedClass (SflowGlobalConfigManagedObject::getClassName (), this);
        SflowCollectorManagedObject    SflowCollectorManagedObject    (pSflowGlobalObjectManager);
        SflowCollectorManagedObject.setupOrm ();
        addManagedClass (SflowCollectorManagedObject::getClassName (), this);
        SflowProfileManagedObject    SflowProfileManagedObject    (pSflowGlobalObjectManager);
        SflowProfileManagedObject.setupOrm ();
        addManagedClass (SflowProfileManagedObject::getClassName (), this);
        addOperationMap (SFLOWGLOBALCREATESFLOWGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&SflowGlobalWorker::SflowGlobalCreateSflowGlobalConfigMessageHandler));
        addOperationMap (SFLOWGLOBALDELETESFLOWGLOBALCONFIG, reinterpret_cast<PrismMessageHandler> (&SflowGlobalWorker::SflowGlobalDeleteSflowGlobalConfigMessageHandler));
        addOperationMap (SFLOWGLOBALSFLOWCOLLECTOR, reinterpret_cast<PrismMessageHandler> (&SflowGlobalWorker::SflowGlobalSflowCollectorMessageHandler));
        addOperationMap (SFLOWGLOBALSFLOWPROFILE, reinterpret_cast<PrismMessageHandler> (&SflowGlobalWorker::SflowGlobalSflowProfileMessageHandler));
    }

    SflowGlobalWorker::~SflowGlobalWorker ()
    {
    }

    PrismMessage  *SflowGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SFLOWGLOBALCREATESFLOWGLOBALCONFIG :
                pPrismMessage = new SflowGlobalCreateSflowGlobalConfigMessage ();
                break;
            case SFLOWGLOBALDELETESFLOWGLOBALCONFIG :
                pPrismMessage = new SflowGlobalDeleteSflowGlobalConfigMessage ();
                break;
            case SFLOWGLOBALSFLOWCOLLECTOR :
                pPrismMessage = new SflowGlobalSflowCollectorMessage ();
                break;
            case SFLOWGLOBALSFLOWPROFILE :
                pPrismMessage = new SflowGlobalSflowProfileMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SflowGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((SflowGlobalConfigManagedObject::getClassName ()) == managedClassName && NULL != getPWaveObjectManager())
        {
            pWaveManagedObject = new SflowGlobalConfigManagedObject(dynamic_cast<SflowGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if((SflowCollectorManagedObject::getClassName ()) == managedClassName && NULL != getPWaveObjectManager()) {
        	pWaveManagedObject = new SflowCollectorManagedObject(dynamic_cast<SflowGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if((SflowProfileManagedObject::getClassName ()) == managedClassName && NULL != getPWaveObjectManager()) {
        	pWaveManagedObject = new SflowProfileManagedObject(dynamic_cast<SflowGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SflowGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }


    void SflowGlobalWorker::SflowGlobalSendToClusterCallback (WaveSendToClusterContext *ctx_p)
    {
        NSM_PLUG_DBG_TRACE();
        LocationId locationid;
        NsmUtils::ClusterGetFirstFailureLocation(ctx_p, locationid);
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p); 
        NSM_PLUG_DBG_S(string("sendToClusterCompletionStatus = ") + sendToClusterCompletionStatus);
        PrismLinearSequencerContext *seq_ctx_p;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }


    void SflowGlobalWorker::createSflowGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "SflowGlobalWorker::createSflowGlobalConfigStep: Entered....");
        if(NULL==pPrismLinearSequencerContext->getPPrismMessage())
        {
            return;
        }
        SflowGlobalCreateSflowGlobalConfigMessage *pSflowGlobalCreateSflowGlobalConfigMessage = dynamic_cast<SflowGlobalCreateSflowGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;

        SflowCreateSflowGlobalConfigSPMessage *m = new SflowCreateSflowGlobalConfigSPMessage();

        m->setEnable(pSflowGlobalCreateSflowGlobalConfigMessage->getEnable());
        m->setPollingInterval(pSflowGlobalCreateSflowGlobalConfigMessage->getPollingInterval());
        m->setSampleRate(pSflowGlobalCreateSflowGlobalConfigMessage->getSampleRate());
        m->setOpCode(pSflowGlobalCreateSflowGlobalConfigMessage->getOpCode());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&SflowGlobalWorker::SflowGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

		send_ctx_p->setPartialSuccessFlag(true);
        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId SflowGlobalWorker::createSflowGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        int createFlag = 1;
        //int status = WAVE_MESSAGE_ERROR;
        IpV4Address defaultCollector(SFLOW_DEFAULT_COLLECTOR);

        SflowGlobalConfigManagedObject *pSflowGlobalConfigManagedObject = NULL;
        SflowGlobalCreateSflowGlobalConfigMessage *pSflowGlobalCreateSflowGlobalConfigMessage = dynamic_cast<SflowGlobalCreateSflowGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
                createFlag = 1;
                //status = WAVE_MESSAGE_SUCCESS;
            } else if (1 == numberOfResults) {
                if((NULL==(*pResults)[0])) {
                    //status = WAVE_MESSAGE_ERROR;
                }
                else {
                    pSflowGlobalConfigManagedObject  = dynamic_cast<SflowGlobalConfigManagedObject*>((*pResults)[0]);
                    trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
                    createFlag = 0;
                    //status = WAVE_MESSAGE_SUCCESS;
                }
            } else {
                    //status = WAVE_MESSAGE_ERROR;
                }
            }
        if (createFlag == 0) {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
            updateWaveManagedObject (pSflowGlobalConfigManagedObject);
            prismAssert (NULL != pSflowGlobalConfigManagedObject, __FILE__, __LINE__);
        } else {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
            pSflowGlobalConfigManagedObject = new SflowGlobalConfigManagedObject(dynamic_cast<SflowGlobalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != pSflowGlobalConfigManagedObject, __FILE__, __LINE__);
        }

        switch(pSflowGlobalCreateSflowGlobalConfigMessage->getOpCode())
        {
            case SFLOW_MSG_ENABLE: 
                trace (TRACE_LEVEL_DEVEL, "SflowGlobalWorkercase case SFLOW_MSG_ENABLE ");
                pSflowGlobalConfigManagedObject->setEnable(pSflowGlobalCreateSflowGlobalConfigMessage->getEnable());
                if (createFlag == 1) {
                    pSflowGlobalConfigManagedObject->setPollingInterval(SFLOW_DEFAULT_POLLING_INTERVAL);
                    pSflowGlobalConfigManagedObject->setSampleRate(SFLOW_DEFAULT_SAMPLE_RATE);
                }
                break;
            case SFLOW_MSG_POLLING_INTERVAL: 
                trace (TRACE_LEVEL_DEVEL, "SflowGlobalWorkercase case SFLOW_MSG_POLLING_INTERVAL");
                pSflowGlobalConfigManagedObject->setPollingInterval(pSflowGlobalCreateSflowGlobalConfigMessage->getPollingInterval());
                if (createFlag == 1) {
                    pSflowGlobalConfigManagedObject->setEnable(SFLOW_DEFAULT_ENABLE);
                    pSflowGlobalConfigManagedObject->setSampleRate(SFLOW_DEFAULT_SAMPLE_RATE);
                }
                break;
            case SFLOW_MSG_SAMPLE_RATE: 
                trace (TRACE_LEVEL_DEVEL, "SflowGlobalWorkercase case SFLOW_MSG_SAMPLE_RATE");
                pSflowGlobalConfigManagedObject->setSampleRate(pSflowGlobalCreateSflowGlobalConfigMessage->getSampleRate());
                if (createFlag == 1) {
                    pSflowGlobalConfigManagedObject->setEnable(SFLOW_DEFAULT_ENABLE);
                    pSflowGlobalConfigManagedObject->setPollingInterval(SFLOW_DEFAULT_POLLING_INTERVAL);
                }
                break;
            default:
                break;
        }
        //        return pSflowGlobalCreateSflowGlobalConfigMessage->getCompletionStatus ();

        if ( (createFlag) && (pSflowGlobalConfigManagedObject)) {
            //Sanjeev: mark the MO for GC
            pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
                    pSflowGlobalConfigManagedObject);
        }
		// Sanjeev: mark the query results for GC.
		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
				*pResults);
            delete pResults;
		}

        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return WAVE_MESSAGE_SUCCESS;
    }

    void  SflowGlobalWorker::SflowGlobalCreateSflowGlobalConfigMessageHandler( SflowGlobalCreateSflowGlobalConfigMessage *pSflowGlobalCreateSflowGlobalConfigMessage)
    {
        // Validations and Queries should go here
        trace (TRACE_LEVEL_DEBUG, "SflowGlobalWorker::SflowGlobalCreateSflowGlobalConfigMessageHandler: Entered....");
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::createSflowGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::createSflowGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSflowGlobalCreateSflowGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }


    void SflowGlobalWorker::deleteSflowGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        if(NULL==pPrismLinearSequencerContext->getPPrismMessage())
        {
            return;
        }

        SflowGlobalDeleteSflowGlobalConfigMessage *pSflowGlobalDeleteSflowGlobalConfigMessage = dynamic_cast<SflowGlobalDeleteSflowGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;

        trace (TRACE_LEVEL_DEBUG, "SflowGlobalWorker::deleteSflowGlobalConfigStep: Entered....");
        SflowDeleteSflowGlobalConfigSPMessage *m = new SflowDeleteSflowGlobalConfigSPMessage();

        m->setEnable(pSflowGlobalDeleteSflowGlobalConfigMessage->getEnable());
        m->setPollingInterval(pSflowGlobalDeleteSflowGlobalConfigMessage->getPollingInterval());
        m->setSampleRate(pSflowGlobalDeleteSflowGlobalConfigMessage->getSampleRate());
        m->setOpCode(pSflowGlobalDeleteSflowGlobalConfigMessage->getOpCode());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&SflowGlobalWorker::SflowGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

		send_ctx_p->setPartialSuccessFlag(true);
        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId SflowGlobalWorker::deleteSflowGlobalConfigMOStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        //int status = 0;
        IpV4Address defaultCollector(SFLOW_DEFAULT_COLLECTOR);

        SflowGlobalConfigManagedObject *pSflowGlobalConfigManagedObject = NULL;
        SflowGlobalDeleteSflowGlobalConfigMessage *pSflowGlobalDeleteSflowGlobalConfigMessage = dynamic_cast<SflowGlobalDeleteSflowGlobalConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "SflowGlobalWorker::deleteSflowGlobalConfigMOStep MO not found");
                //status = WAVE_MESSAGE_ERROR;
            } else if (1 == numberOfResults) {
                pSflowGlobalConfigManagedObject  = dynamic_cast<SflowGlobalConfigManagedObject*>((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "SflowGlobalWorker::deleteSflowGlobalConfigMOStep MO found" );
                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                trace (TRACE_LEVEL_DEBUG, "SflowGlobalWorker::deleteSflowGlobalConfigMOStep More than one MO found" );
                //status = WAVE_MESSAGE_ERROR;
            }
            pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        if (pSflowGlobalConfigManagedObject == NULL) {
            return(WAVE_MESSAGE_ERROR);
        }	
        else
        {
            prismAssert (NULL != pSflowGlobalConfigManagedObject, __FILE__, __LINE__);
            updateWaveManagedObject (pSflowGlobalConfigManagedObject);
            switch(pSflowGlobalDeleteSflowGlobalConfigMessage->getOpCode())
            {
                case SFLOW_MSG_NO_ENABLE: 
                    trace (TRACE_LEVEL_DEVEL, "SflowGlobalWorkercase case SFLOW_MSG_NO_ENABLE");
                    pSflowGlobalConfigManagedObject->setEnable(SFLOW_DEFAULT_ENABLE);
                    break;
                case SFLOW_MSG_NO_POLLING_INTERVAL: 
                    trace (TRACE_LEVEL_DEVEL, "SflowGlobalWorkercase case SFLOW_MSG_NO_POLLING_INTERVAL");
                    pSflowGlobalConfigManagedObject->setPollingInterval(SFLOW_DEFAULT_POLLING_INTERVAL);
                    break;
                case SFLOW_MSG_NO_SAMPLE_RATE: 
                    trace (TRACE_LEVEL_DEVEL, "SflowGlobalWorkercase case SFLOW_MSG_NO_SAMPLE_RATE");
                    pSflowGlobalConfigManagedObject->setSampleRate(SFLOW_DEFAULT_SAMPLE_RATE);
                    break;
                default:
                    break;
            }
        }
        //        return pSflowGlobalDeleteSflowGlobalConfigMessage->getCompletionStatus ();
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS); 
        return WAVE_MESSAGE_SUCCESS;
    }

    void  SflowGlobalWorker::SflowGlobalDeleteSflowGlobalConfigMessageHandler( SflowGlobalDeleteSflowGlobalConfigMessage *pSflowGlobalDeleteSflowGlobalConfigMessage)
    {
        // Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::deleteSflowGlobalConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::deleteSflowGlobalConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSflowGlobalDeleteSflowGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }

    void  SflowGlobalWorker::SflowGlobalSflowCollectorMessageHandler(SflowGlobalSflowCollectorMessage *pMessage)
    {

    	NSM_PLUG_DBG_S("Entered....");


        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::SflowCollectorUpdateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::SflowCollectorUpdateMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerFailedStep)
        };


        DceLinearSeqContext *pPrismLinearSequencerContext = new DceLinearSeqContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));


		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
        NSM_PLUG_DBG_S("Exiting....");
    }

    ResourceId SflowGlobalWorker::SflowCollectorValidateStep (DceLinearSeqContext *pPrismLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entered....");
    	SflowGlobalSflowCollectorMessage *pMessage = dynamic_cast<SflowGlobalSflowCollectorMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    	IpVxAddress ipAddress = pMessage->getCollectorIP();
    	UI32 portnumber = pMessage->getCollectorPort();
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

		//Validation 1 - Check whether sflow Gloabl MO is present
        SflowGlobalConfigManagedObject *pSflowGlobalConfigManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObjectPointer<SflowCollectorManagedObject> >configuredCollectors;
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        int existinCollectorCount = 0;
        if (NULL != pResults) {
			UI32 numberOfResults = pResults->size();
			if (0 == numberOfResults) {
				NSM_PLUG_DBG_ERR_S(
						"Sflow is not enabled. Enable sflow before configuring collector");
				status = WRC_SFLOW_SET_ERR_PORT_NOT_EXIST;
			}
			else if (1 == numberOfResults) {
				if ((NULL == (*pResults)[0])) {
					NSM_PLUG_DBG_ERR_S(
							"Sflow is not enabled. Enable sflow before configuring collector");
					status = WRC_SFLOW_SET_ERR_PORT_NOT_EXIST;
				}
				else {
					pSflowGlobalConfigManagedObject =
							dynamic_cast<SflowGlobalConfigManagedObject*>((*pResults)[0]);
					//Put the SflowGlobalConfigMO in the cache to be used in the subsequent steps
					pPrismLinearSequencerContext->getCache()->put(SflowGlobalConfigManagedObject::getClassName(), pSflowGlobalConfigManagedObject);
					pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pSflowGlobalConfigManagedObject);
					NSM_PLUG_DBG_S(
							"Sflow is enabled. Continue with validation")
				}
			}
			else {
				NSM_PLUG_DBG_ERR_S("*** More than 1 Sflow Instance!!!??? ***");
				status = WAVE_MESSAGE_ERROR;
			}
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
        	NSM_PLUG_DBG_ERR_S("ERROR: NOT proceeding with validation. Failed to retrieve SflowGlobalConfigManagedObject!");
            return status;
        }


        if(pSflowGlobalConfigManagedObject) {
        	configuredCollectors = pSflowGlobalConfigManagedObject->getCollectorList();
        	existinCollectorCount = configuredCollectors.size();
        	NSM_PLUG_DBG_S("Number of existing collectors:"+existinCollectorCount);
        }

        switch(pMessage->getOpCode())
        {
			case SFLOW_MSG_COLLECTOR:
			{
				string   tempcollector;
				tempcollector = ipAddress.toString();
				NSM_PLUG_DBG_S(string ("Checking whether the given IP is valid"));
				if((SFLOW_ZERO_COLLECTOR_IP  == tempcollector)||
						((SFLOW_MULTICAST_BEGIN_IP<= tempcollector)&&
						 (SFLOW_MULTICAST_END_IP  >= tempcollector))||
						(SFLOW_BROADCAST_ALL_IP   == tempcollector) ||
						(SFLOW_LOOPBACK_IP        == tempcollector))
				{
					status = WRC_SFLOW_SET_ERR_INVALID_COLLECTOR_IP;
					NSM_PLUG_DBG_ERR_S(string ("Invalid collector IP: ")+tempcollector);
					return status;
				}

        		if(existinCollectorCount > 0) {
        			NSM_PLUG_DBG_S("Checking whether the maximum number of collectors are already configured");
        			//Validation 2 - Check whether maximum number of collectors are configured
        			//TODO: Use constant rather than the magic number for max sflow collectors
        			if(existinCollectorCount < 5) {
        				//Validation 3 - Check whether the collector is already configured
						string ip_str;
						for (int i = 0; i < existinCollectorCount; i++) {
							ip_str = configuredCollectors[i]->getCollectorIP().toString();
							//configuredCollectors[i]->getCollectorIP().fromString(ip_str);
							NSM_PLUG_DBG_S(string("Checking Collector with ip =")+ip_str+" and port="+configuredCollectors[i]->getCollectorPort());
							if(ipAddress.operator ==(configuredCollectors[i]->getCollectorIP()) && portnumber == configuredCollectors[i]->getCollectorPort()) {
								NSM_PLUG_DBG_ERR_S("Given collector already configured");
								status = WAVE_MESSAGE_ERROR;
								break;
							}
						}
        			}
        			else {
        				NSM_PLUG_DBG_ERR_S(string("Maximum number of collectors already configured!"));
	        			status = WRC_SFLOW_SET_ERR_MAX_COLLECTORS_CONFIGURED;
        			}
        		}
        		break;
			}
			case SFLOW_MSG_NO_COLLECTOR:
			{
        		if(existinCollectorCount > 0) {
        			NSM_PLUG_DBG_S(string("Checking whether the collector is already configured!"));
					//Validation 3 - Check whether the collector is already configured
					string ip_str;
					bool collectorExists = false;
					for (int i = 0; i < existinCollectorCount; i++) {
						//configuredCollectors[i]->getCollectorIP().fromString(ip_str);
						ip_str = configuredCollectors[i]->getCollectorIP().toString();
						NSM_PLUG_DBG_S(string("Checking Collector with ip =")+ip_str+" and port="+configuredCollectors[i]->getCollectorPort());
						if(ipAddress.operator ==(configuredCollectors[i]->getCollectorIP()) && portnumber == configuredCollectors[i]->getCollectorPort()) {
							NSM_PLUG_DBG_S(string("Given collector is configured. Proceed with the deletion."));
							collectorExists = true;
							break;
						}
					}
					if(false == collectorExists) {
						NSM_PLUG_DBG_ERR_S(string("ERROR: Given collector is NOT configured!"));
						status = WRC_SFLOW_SET_ERR_COLLECTOR_NOT_CONFIGURED;
					}
        		}
        		else {
        			NSM_PLUG_DBG_ERR_S(string("No collector configured!"));
        			status = WRC_SFLOW_SET_ERR_COLLECTOR_NOT_CONFIGURED;
        		}
        		break;
			}
			default:
			{
				NSM_PLUG_DBG_ERR_S(string("ERROR: The message should not reach here!"));
				break;
			}
        }
    	NSM_PLUG_DBG_S("Exiting....");
		return status;
    }

    void SflowGlobalWorker::SflowCollectorUpdateStep (DceLinearSeqContext *pPrismLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entered....");

        if(NULL==pPrismLinearSequencerContext->getPPrismMessage())
        {
            return;
        }

    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	status = SflowCollectorValidateStep(pPrismLinearSequencerContext);
    	if (status != WAVE_MESSAGE_SUCCESS) {
    		NSM_PLUG_DBG_S("Sflow Collector Config validations FAILED");
    		pPrismLinearSequencerContext->executeNextStep(status);
    		return;
    	}
    	else {
    		NSM_PLUG_DBG_S("Sflow Collector Config validations successful");
    	}

        SflowGlobalSflowCollectorMessage *pMessage = dynamic_cast<SflowGlobalSflowCollectorMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;

        SflowCreateSflowGlobalConfigSPMessage *m = new SflowCreateSflowGlobalConfigSPMessage();
        m->setCollector(pMessage->getCollectorIP());
        m->setPortNumber(pMessage->getCollectorPort());
        m->setCollectorAddressType(pMessage->getCollectorAddressType());
        m->setOpCode(pMessage->getOpCode());


        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&SflowGlobalWorker::SflowGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

		send_ctx_p->setPartialSuccessFlag(true);
        sendToWaveCluster(send_ctx_p);


    	NSM_PLUG_DBG_S("Exiting....");
		return ;
    }

    ResourceId SflowGlobalWorker::SflowCollectorUpdateMOStep (DceLinearSeqContext *pSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entered...");

        SflowGlobalConfigManagedObject *pSflowMO = dynamic_cast<SflowGlobalConfigManagedObject *> (pSequencerContext->
        		getCache()->getWaveManagedObject(SflowGlobalConfigManagedObject::getClassName()));


    	if(!pSflowMO) {
    		NSM_PLUG_DBG_ERR_S(string("ERROR: The SflowGlobalConfigManagedObject  is NULL"));
    		return WAVE_MESSAGE_ERROR;
    	}
    	else {
    		NSM_PLUG_DBG_S("Retrieved the SflowGlobalConfigManagedObject from the cache");
    	}

        SflowCollectorManagedObject *pCollectorMO = NULL;
        SflowGlobalSflowCollectorMessage *pCollectorMsg = dynamic_cast<SflowGlobalSflowCollectorMessage*> (pSequencerContext->getPPrismMessage ());
        IpVxAddress ipAddress = pCollectorMsg->getCollectorIP();
        UI32 portnumber = pCollectorMsg->getCollectorPort();
        NSM_PLUG_INFO_S(string("Updating the collector MO for the IP Address ")+ipAddress.toString()+" and Port Number "+portnumber);


        switch(pCollectorMsg->getOpCode())
        {
			case SFLOW_MSG_COLLECTOR:
			{
				NSM_PLUG_DBG_S("Case Create - SFLOW_MSG_COLLECTOR");

		        pCollectorMO = new SflowCollectorManagedObject(dynamic_cast<SflowGlobalObjectManager*>(getPWaveObjectManager()));
		        prismAssert (NULL != pCollectorMO, __FILE__, __LINE__);
		        pCollectorMO->setCollectorIP(ipAddress);
		        pCollectorMO->setCollectorPort(portnumber);
		        pCollectorMO->setOwnerManagedObjectId(pSflowMO->getObjectId());

        	    addToComposition(SflowGlobalConfigManagedObject::getClassName(), SflowCollectorManagedObject::getClassName(),
            	    "collectorList", pSflowMO->getObjectId(), pCollectorMO->getObjectId());

        	    pSequencerContext->addManagedObjectForGarbageCollection(pCollectorMO);

	            NSM_PLUG_DBG_S("Created SflowGlobalConfigManagedObject");
				break;
			}
			case SFLOW_MSG_NO_COLLECTOR:
			{
				NSM_PLUG_DBG_S("Case Delete - SFLOW_MSG_NO_COLLECTOR");
				UI32 numberOfResults = 0;

    	        WaveManagedObjectSynchronousQueryContext collectorSyncQueryCtxt(SflowCollectorManagedObject::getClassName());

    	        collectorSyncQueryCtxt.addSelectFieldForObjectId("objectId");
    	        collectorSyncQueryCtxt.addAndAttribute (new AttributeIpVxAddress(&ipAddress, "ipAddress"));
    	        collectorSyncQueryCtxt.addAndAttribute (new AttributeUI32(&portnumber, "portNumber"));
    	        collectorSyncQueryCtxt.addAndAttribute (new AttributeObjectId(pSflowMO->getObjectId(), "ownerManagedObjectId"));
            	vector<WaveManagedObject *> *pCollectors = querySynchronously (&collectorSyncQueryCtxt);

	            if (NULL != pCollectors) {
    	            numberOfResults = pCollectors->size ();
        	        if (numberOfResults == 0) {
            	        NSM_PLUG_DBG_S("Failed to find the Sflow Collector MO in the DB");
                	} else if (1 == numberOfResults) {
                		pCollectorMO= dynamic_cast<SflowCollectorManagedObject *>((*pCollectors)[0]);
	                    deleteFromComposition(SflowGlobalConfigManagedObject::getClassName(), SflowCollectorManagedObject::getClassName(),
    	                    "collectorList", pSflowMO->getObjectId(), pCollectorMO->getObjectId());
        	        }
        	        pSequencerContext->addManagedObjectsForGarbageCollection(*pCollectors);
                	delete pCollectors;
                }

				break;
			}
			default:
			{
				NSM_PLUG_DBG_S("ERROR: Should not reach here!");
				break;
			}
        }


        NSM_PLUG_DBG_S("Exiting....");
        pSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return WAVE_MESSAGE_SUCCESS;

    }

    void  SflowGlobalWorker::SflowGlobalSflowProfileMessageHandler(SflowGlobalSflowProfileMessage *pMessage)
    {
    	NSM_PLUG_DBG_S("Entered....");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::SflowProfileUpdateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::SflowProfileUpdateMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&SflowGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
        NSM_PLUG_DBG_S("Exiting....");
    }

    ResourceId SflowGlobalWorker::SflowProfileValidateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entered....");
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

		//sflow-profile is used along with the policy-map. So if policer capability not present in the platform, return the error code.
    	//In case of SA, if the platform does not support this, throw error
    	//In case of VCS, even if the local platform does not support sflow-profile, we'll create the profile, but the restriction is imposed while applying it to an interface
		if(WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled() && !Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_POLICER)) {
			return WRC_SFLOW_PROFILE_ERR_NOT_SUPPORTED_PLATFORM;
		}


    	SflowGlobalSflowProfileMessage *pMessage = dynamic_cast<SflowGlobalSflowProfileMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
    	if(pMessage->getOpCode() == SFLOW_MSG_PROFILE_CREATE) {
			//Validation 1 - Check whether the sampling rate is a power of two
			UI32 samplingRate = pMessage->getSamplingRate();
			while (((samplingRate % 2) == 0) && samplingRate > 1) { /* While samplingRate is even and > 1 */
				samplingRate /= 2;
			}
			bool isSamplingRatePowerOfTwo = (samplingRate == 1);

			NSM_PLUG_DBG_S(string("samplingRate :")+samplingRate);
			NSM_PLUG_DBG_S(string("Original samplingRate :")+pMessage->getSamplingRate());
			NSM_PLUG_DBG_S(string("isSamplingRatePowerOfTwo :")+isSamplingRatePowerOfTwo);

			if(false == isSamplingRatePowerOfTwo) {
				NSM_PLUG_DBG_S(string("Sflow profile sampling rate is NOT a power of 2"));
				return WRC_SFLOW_PROFILE_SAMPLING_RATE_NOT_POWER_OF_TWO;
			}


			//Validation 2 - Check whether the maximum number of sflow profiles is already configured
			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowProfileManagedObject::getClassName());
			UI32 existingProfileCount = 0;
			status = querySynchronouslyForCount(&syncQueryCtxt, existingProfileCount);
			if(WAVE_MESSAGE_SUCCESS == status) {
				NSM_PLUG_DBG_S(string("Number of existing sflow profiles:")+existingProfileCount);
				if(existingProfileCount == SFLOW_DCM_MAX_PROFILE_COUNT) {
					NSM_PLUG_DBG_S(string("Maximum number of profiles already configured"));
					return WRC_SFLOW_PROFILE_MAX_PROFILES_CONFIGURED;
				}
			}
			else {
				NSM_PLUG_DBG_S(string("Failed to query the existing profile count!!"));
			}
    	}
    	else {
    		//Verify if the sflow profile is mapped to a policy. If so do not allow the deletion.
    		ObjectId sflowProfileObjectId = ObjectId::NullObjectId;
    		string profileName = pMessage->getProfileName();
    		NSM_PLUG_DBG_S(string("Validations for the Delete operation for the profile ")+profileName);
			WaveManagedObjectSynchronousQueryContext sflowProfileMOQueryCtxt(SflowProfileManagedObject::getClassName());
			sflowProfileMOQueryCtxt.addAndAttribute (new AttributeString(&profileName, "profileName"));
			sflowProfileMOQueryCtxt.addSelectField("objectId");
			vector<WaveManagedObject *> *pResults = querySynchronously(&sflowProfileMOQueryCtxt);
			if (pResults && pResults->size() == 1) {
				sflowProfileObjectId = (*pResults)[0]->getObjectId();
				NSM_PLUG_DBG_S(string("ObjectId of SflowProfileMO:")+sflowProfileObjectId.toString());
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoClassMapManagedObject::getClassName());
				syncQueryCtxt.addAndAttribute(new AttributeObjectId(&sflowProfileObjectId, "sflowMapId"));
				UI32 policyMapcount = 0;
				status = querySynchronouslyForCount(&syncQueryCtxt, policyMapcount);
				if(WAVE_MESSAGE_SUCCESS == status) {
					NSM_PLUG_DBG_S(string("Number of Policy Maps associated with this sflow profile:")+policyMapcount);
					if(policyMapcount >= 1) {
						NSM_PLUG_DBG_S(string("The sflow profile is bound to some policy maps. Hence cannot be deleted"));
						return WRC_SFLOW_PROFILE_BOUND_TO_CLASSMAP;
					}
				}
				else {
					NSM_PLUG_DBG_S(string("Failed to query the policy maps associated with the profile!!"));
				}
			}
			else {
				NSM_PLUG_DBG_S(string("Failed to find Sflow profile :")+profileName);
			}
    	}
    	NSM_PLUG_DBG_S("Exiting....");
		return status;
    }

    void SflowGlobalWorker::SflowProfileUpdateStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entered....");

        if(NULL==pPrismLinearSequencerContext->getPPrismMessage())
        {
            return;
        }

    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	status = SflowProfileValidateStep(pPrismLinearSequencerContext);
    	if (status != WAVE_MESSAGE_SUCCESS) {
    		NSM_PLUG_DBG_S("Sflow Profile Config validations FAILED");
    		pPrismLinearSequencerContext->executeNextStep(status);
    		return;
    	}
    	else {
    		NSM_PLUG_DBG_S("Sflow Profile Config validations successful");
    	}

        SflowGlobalSflowProfileMessage *pMessage = dynamic_cast<SflowGlobalSflowProfileMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;

        SflowProfilePrismMessage *m = new SflowProfilePrismMessage();
        m->setProfileName(pMessage->getProfileName());
        m->setSamplingRate(pMessage->getSamplingRate());
        m->setOpCode(pMessage->getOpCode());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&SflowGlobalWorker::SflowGlobalSendToClusterCallback), pPrismLinearSequencerContext);
        prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(m);

		send_ctx_p->setPartialSuccessFlag(true);
        sendToWaveCluster(send_ctx_p);

    	NSM_PLUG_DBG_S("Exiting....");
		return ;
    }

    ResourceId SflowGlobalWorker::SflowProfileUpdateMOStep (PrismLinearSequencerContext *pSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entered...");

        SflowProfileManagedObject *pProfileMO = NULL;
        SflowGlobalSflowProfileMessage *pProfileMsg = dynamic_cast<SflowGlobalSflowProfileMessage*> (pSequencerContext->getPPrismMessage ());
        string profileName = pProfileMsg->getProfileName();
        UI32 samplingRate = pProfileMsg->getSamplingRate();
        NSM_PLUG_INFO_S(string("Updating the profile MO for the Profile Name ")+profileName+" and Sampling Rate "+samplingRate);


        switch(pProfileMsg->getOpCode())
        {
			case SFLOW_MSG_PROFILE_CREATE:
			{
				NSM_PLUG_DBG_S(string("Case Create/Update - SFLOW_MSG_PROFILE_CREATE"));

				WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SflowProfileManagedObject::getClassName());
				syncQueryCtxt.addAndAttribute (new AttributeString(&profileName, "profileName"));
				vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);
				if (NULL != pResults) {
					//pSequencerContext->addManagedObjectsForGarbageCollection(*pResults);

					UI32 numberOfResults = pResults->size ();
					if (0 == numberOfResults) {
						NSM_PLUG_DBG_S(string("No match found. Hence create the MO"));
						pProfileMO = new SflowProfileManagedObject(dynamic_cast<SflowGlobalObjectManager*>(getPWaveObjectManager()));
				        prismAssert (NULL != pProfileMO, __FILE__, __LINE__);
				        pProfileMO->setProfileName(profileName);
				        pProfileMO->setSamplingRate(samplingRate);
					}

					else if (1 == numberOfResults) {
						NSM_PLUG_DBG_S(string("The profile with the name already exists. Hence update the sampling rate"));
						pProfileMO  = dynamic_cast<SflowProfileManagedObject*>((*pResults)[0]);
		                NSM_PLUG_ASSERT_PTR (pProfileMO);
		                pProfileMO->setSamplingRate(samplingRate);
					}
					else {
						NSM_PLUG_DBG_S(string("Error! numberOfResults = ")+numberOfResults);
						return (WAVE_MESSAGE_ERROR);
					}
				}
				else {
					NSM_PLUG_DBG_S(string("NULL == pResults. Hence create the MO"));
					pProfileMO = new SflowProfileManagedObject(dynamic_cast<SflowGlobalObjectManager*>(getPWaveObjectManager()));
			        prismAssert (NULL != pProfileMO, __FILE__, __LINE__);
			        pProfileMO->setProfileName(profileName);
			        pProfileMO->setSamplingRate(samplingRate);
				}

	            updateWaveManagedObject (pProfileMO);
	            prismAssert (NULL != pProfileMO, __FILE__, __LINE__);
	            pSequencerContext->addManagedObjectForGarbageCollection(pProfileMO);
	            NSM_PLUG_DBG_S("Created SflowProfileManagedObject");
				break;
			}
			case SFLOW_MSG_PROFILE_DELETE:
			{
				NSM_PLUG_DBG_S("Case Delete - SFLOW_MSG_PROFILE_DELETE");
				UI32 numberOfResults = 0;

    	        WaveManagedObjectSynchronousQueryContext profileSyncQueryCtxt(SflowProfileManagedObject::getClassName());

    	        profileSyncQueryCtxt.addSelectFieldForObjectId("objectId");
    	        profileSyncQueryCtxt.addAndAttribute (new AttributeString(&profileName, "profileName"));

            	vector<WaveManagedObject *> *pProfiles = querySynchronously (&profileSyncQueryCtxt);

	            if (NULL != pProfiles) {
    	            numberOfResults = pProfiles->size ();
        	        if (numberOfResults == 0) {
            	        NSM_PLUG_DBG_S("Failed to find the Sflow Profile MO in the DB");
                	} else if (1 == numberOfResults) {
                		pProfileMO= dynamic_cast<SflowProfileManagedObject *>((*pProfiles)[0]);
	                    deleteWaveManagedObject(pProfileMO->getObjectId());
	                    NSM_PLUG_DBG_S("Deleted SflowProfileManagedObject");
        	        }
        	        pSequencerContext->addManagedObjectsForGarbageCollection(*pProfiles);
                	delete pProfiles;
                }
				break;
			}
			default:
			{
				NSM_PLUG_DBG_S("ERROR: Should not reach here!");
				break;
			}
        }
        NSM_PLUG_DBG_S("Exiting....");
        pSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return WAVE_MESSAGE_SUCCESS;
    }

    /**
     * Prior to Nos 4.0.0, the sflow collector was part of SflowGlobalConfigMO.
     * By default, the SflowGlobalConfigMO was not created and while configuring sflow collector
     * (or any other field in the old SflowGlobalConfigMO), the MO was getting created.
     * So we could configure sflow collector, even if sflow is disabled.
     * From 4.0.0 onwards, we support multiple sflow collectors. So the sflow collector is taken out from
     * the SflowGlobalConfigMO and anew MO, SflowCollectorMO is formed.
     * The SflowCollectorMO has a composition relation with SflowGlobalConfigMO. So now, if the SflowGlobalConfigMO
     * is not there, we cannot configure Sflow Collector.
     * This has backward compatibility issues with Netconf clients like BNA.
     *
     * Hence using the install to create the default SflowGlobalConfigManagedObject
     */
	void SflowGlobalWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
		string default_val;
        trace(TRACE_LEVEL_INFO, string("Entering SflowGlobalWorker::install () ..."));

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT)
        		&& (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

        trace(TRACE_LEVEL_INFO, string("Entering SflowGlobalWorker::install () ... Create Default Sflow Config MO ..."));

		startTransaction();

        SflowGlobalConfigManagedObject *pWaveManagedObject = new SflowGlobalConfigManagedObject(dynamic_cast<SflowGlobalObjectManager *>(getPWaveObjectManager()));
        tracePrintf (TRACE_LEVEL_INFO, "SflowGlobalWorker::defaultConfMOStep : SflowGlobalConfigManagedObject %p ", pWaveManagedObject);

		status = commitTransaction ();

        if (status == FRAMEWORK_SUCCESS) {
        	trace(TRACE_LEVEL_INFO, string("Created Default Sflow Config MO successfully..."));
        	status = WAVE_MESSAGE_SUCCESS;
		}else{
			trace(TRACE_LEVEL_ERROR, "Failed to create SflowGlobalConfigMO");
		}

        delete pWaveManagedObject;


        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
        return;

    }


}
