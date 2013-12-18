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

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClientsContext.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Utils/Capabilities.h"
#include "Utils/DceClusterUtils.h"
#include "Sflow/Local/SflowLocalObjectManager.h"
#include "Sflow/Local/SflowLocalConfigWorker.h"
#include "Sflow/Local/SflowCreateSflowGlobalConfigSPMessage.h"
#include "Sflow/Local/SflowDeleteSflowGlobalConfigSPMessage.h"
#include "Sflow/Local/SflowProfilePrismMessage.h"
#include "Sflow/Local/SflowLocalShowMessage.h"
#include "Sflow/Local/SflowTypes.h"
#include "ClientInterface/Sflow/SflowClientCreateSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowClientDeleteSflowGlobalConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowClientSflowProfileConfigSPMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"
#include "ClientInterface/Sflow/SflowShowMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "sflow_dcmd.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"



namespace DcmNs
{

    SflowLocalConfigWorker::SflowLocalConfigWorker ( SflowLocalObjectManager *pSflowLocalObjectManager) 
            : WaveWorker  (pSflowLocalObjectManager)
    {
        addOperationMap (SFLOWCREATESFLOWGLOBALCONFIGSP, reinterpret_cast<PrismMessageHandler> (&SflowLocalConfigWorker::SflowCreateSflowGlobalConfigSPMessageHandler));
        addOperationMap (SFLOWDELETESFLOWGLOBALCONFIGSP, reinterpret_cast<PrismMessageHandler> (&SflowLocalConfigWorker::SflowDeleteSflowGlobalConfigSPMessageHandler));
        addOperationMap (SFLOWLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&SflowLocalConfigWorker::SflowLocalShowMessageHandler));
        addOperationMap (SFLOWPROFILECONFIGSP, reinterpret_cast<PrismMessageHandler> (&SflowLocalConfigWorker::SflowProfileConfigMessageHandler));
    WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient ("Sflagt", &SflowLocalConfigWorker::createManagementInterfaceMessageInstance);
    }

    SflowLocalConfigWorker::~SflowLocalConfigWorker ()
    {
    }

    PrismMessage  *SflowLocalConfigWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SFLOWCREATESFLOWGLOBALCONFIGSP :
                pPrismMessage = new SflowCreateSflowGlobalConfigSPMessage ();
                break;
            case SFLOWDELETESFLOWGLOBALCONFIGSP :
                pPrismMessage = new SflowDeleteSflowGlobalConfigSPMessage ();
                break;
            case SFLOWLOCALSHOW :
                pPrismMessage = new SflowLocalShowMessage ();
                break;
            case SFLOWPROFILECONFIGSP:
                pPrismMessage = new SflowProfilePrismMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

   ManagementInterfaceMessage *SflowLocalConfigWorker::createManagementInterfaceMessageInstance (const UI32 &operationCode)
    {
        ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;

        switch (operationCode) {
            case SFLOWSHOWMESSAGE:
                pManagementInterfaceMessage = new SflowShowMessage();
                break;
            default:
                pManagementInterfaceMessage = NULL;
        }                                                                                                                
    return (pManagementInterfaceMessage);
}
    WaveManagedObject  *SflowLocalConfigWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId SflowLocalConfigWorker::createSflowLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S(string("Entering"));
        int status = WAVE_MESSAGE_ERROR;
        if(NULL==pPrismSynchronousLinearSequencerContext->getPPrismMessage ()) {
            return status;
        }

        SflowCreateSflowGlobalConfigSPMessage *pSflowCreateSflowGlobalConfigSPMessage = dynamic_cast<SflowCreateSflowGlobalConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        SflowClientCreateSflowGlobalConfigSPMessage *m = new SflowClientCreateSflowGlobalConfigSPMessage();

        m->setEnable(pSflowCreateSflowGlobalConfigSPMessage->getEnable());
        /*
        //To test just the dcm db
        if(m->getCollectorAddressType() == SFLOW_COLLECTOR_ADDRESSTYPE_IPV6) {
            IpVxAddress collector;
            collector.fromString("10.10.10.10");
            m->setCollector(collector);
        }
        else {
        	m->setCollector(pSflowCreateSflowGlobalConfigSPMessage->getCollector());
        }
        */
        m->setCollector(pSflowCreateSflowGlobalConfigSPMessage->getCollector());
        m->setPortNumber(pSflowCreateSflowGlobalConfigSPMessage->getPortNumber());
        m->setCollectorAddressType(pSflowCreateSflowGlobalConfigSPMessage->getCollectorAddressType());
        m->setPollingInterval(pSflowCreateSflowGlobalConfigSPMessage->getPollingInterval());
        m->setSampleRate(pSflowCreateSflowGlobalConfigSPMessage->getSampleRate());
        m->setOpCode(pSflowCreateSflowGlobalConfigSPMessage->getOpCode());

        NSM_PLUG_DBG_S(string("Sflow Collector to backend:")+m->getCollector().toString());
        NSM_PLUG_DBG_S(string("Sflow Collector Address Type to backend:")+m->getCollectorAddressType());

        status = sendSynchronouslyToWaveClient ("Sflow", m);
        NSM_PLUG_DBG_S(string("Exiting with status=")+status);
        return errorCode(pSflowCreateSflowGlobalConfigSPMessage, m, status);
    }

    void  SflowLocalConfigWorker::SflowCreateSflowGlobalConfigSPMessageHandler( SflowCreateSflowGlobalConfigSPMessage *pSflowCreateSflowGlobalConfigSPMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::createSflowLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSflowCreateSflowGlobalConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId SflowLocalConfigWorker::deleteSflowLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S(string("Entering.."));
        int status = WAVE_MESSAGE_ERROR;
        if(NULL == pPrismSynchronousLinearSequencerContext->getPPrismMessage ()) {
            return status;
        }

        SflowDeleteSflowGlobalConfigSPMessage *pSflowDeleteSflowGlobalConfigSPMessage = dynamic_cast<SflowDeleteSflowGlobalConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        SflowClientDeleteSflowGlobalConfigSPMessage *m = new SflowClientDeleteSflowGlobalConfigSPMessage();

        m->setEnable(pSflowDeleteSflowGlobalConfigSPMessage->getEnable());
        m->setCollector(pSflowDeleteSflowGlobalConfigSPMessage->getCollector());
        m->setPollingInterval(pSflowDeleteSflowGlobalConfigSPMessage->getPollingInterval());
        m->setSampleRate(pSflowDeleteSflowGlobalConfigSPMessage->getSampleRate());
        m->setOpCode(pSflowDeleteSflowGlobalConfigSPMessage->getOpCode());

        status = sendSynchronouslyToWaveClient ("Sflow", m);
        NSM_PLUG_DBG_S(string("Exiting with status=")+status);
        return errorCode(pSflowDeleteSflowGlobalConfigSPMessage, m, status);
    }

    void  SflowLocalConfigWorker::SflowDeleteSflowGlobalConfigSPMessageHandler( SflowDeleteSflowGlobalConfigSPMessage *pSflowDeleteSflowGlobalConfigSPMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::deleteSflowLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            //            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::deleteSflowLocalConfigMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSflowDeleteSflowGlobalConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void SflowLocalConfigWorker::SflowLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p)
    {

    	NSM_PLUG_DBG_S(string("Entering : "));
        PrismLinearSequencerContext				*seq_ctx_p = NULL;
        SflowLocalShowMessage					*req_msg_p = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p = NULL;

        NSM_PLUG_DBG_TRACE();

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p = dynamic_cast<SflowLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p =  dynamic_cast<SflowLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

        NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        NSM_PLUG_DBG_S(string("Number of locations : ")+locations.size());
        UI32	i = 0;
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus )
        {
			for (i = 0; i < locations.size(); i++)
			{
				ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
				if(WAVE_MESSAGE_SUCCESS == statusPerLocationId)
				{
					sw_resp_p = dynamic_cast<SflowLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
					NSM_PLUG_ASSERT_PTR(sw_resp_p);
					if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
					{
						//  req_msg_p->copyBuffersFrom(*sw_resp_p);
						NSM_PLUG_DBG_S(string("Calling   copyAllBuffers(): "));
						req_msg_p->copyAllBuffers(*sw_resp_p);
					}
					else
					{
						NSM_PLUG_DBG_ERR_S(string("Response processing: The location: ") + locations[i] + string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
					}
				}
				else
				{
					NSM_PLUG_DBG_ERR_S(string("Response processing: The location: ")+ locations[i] + string("returned error: ")+FrameworkToolKit::localize(statusPerLocationId));
				}
			}
        }
        else
        {
        	NSM_PLUG_DBG_ERR_S(string("returned error: ")  + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }
        NSM_PLUG_INFO_S(string(": MC return status: ") + sendToClusterCompletionStatus);
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        delete resp_msg_p;
        delete ctx_p;
        return;
    }

    void SflowLocalConfigWorker::SflowSendToClientsCallback (WaveSendToClientsContext *ctx_p)
    {
        PrismLinearSequencerContext				*seq_ctx_p = NULL;
        SflowLocalShowMessage					*req_msg_p = NULL;
        SflowShowMessage                      *sw_resp_p = NULL;
        //SflowShowMessage                      *context_msg_p = NULL;
        trace (TRACE_LEVEL_INFO, string ("SflowLocalConfigWorker::SflowSendToClientsCallback"));  
        NSM_PLUG_DBG_TRACE();

        ResourceId   sendToClientsCompletionStatus = ctx_p->getCompletionStatus ();
        NSM_PLUG_DBG_S(string("sendToClientsCompletionStatus:")+sendToClientsCompletionStatus);
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext ());
        NSM_PLUG_DBG_S(string("Got seq_ctx_p:"));


        prismAssert (NULL != seq_ctx_p, __FILE__, __LINE__);
        NSM_PLUG_DBG_S(string("About to retrieve the message"));

        if(seq_ctx_p->getPPrismMessage ()) {
        	NSM_PLUG_DBG_S(string("seq_ctx_p->getPPrismMessage () is not NULL"));
        	req_msg_p = dynamic_cast<SflowLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        	NSM_PLUG_INFO_S(string("Retrieved SflowShowMessage"));
        }
        else {
        	NSM_PLUG_DBG_ERR_S(string("seq_ctx_p->getPPrismMessage () is NULL"));
        	seq_ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
        	return;
        }
        //bool gwReq = false;
        //req_msg_p = new SflowLocalShowMessage(context_msg_p->getIfName(), context_msg_p->getCmdCode(), gwReq);

        NSM_PLUG_DBG_S(string("Retrieved the message"));


        vector<SI32>              instancesToSendToForPhase1 = ctx_p->getInstancesToSendToForPhase1 ();
        UI32                      numberOfInstancesToSendTo = instancesToSendToForPhase1.size ();
        NSM_PLUG_DBG_S(string("numberOfInstancesToSendTo = ") + numberOfInstancesToSendTo);
        UI32 i = 0;
        for (i = 0; i < numberOfInstancesToSendTo; i++)
        {
            WaveMessageStatus sendStatus = ctx_p->getSendStatusForPhase1 (instancesToSendToForPhase1[i]);
            trace (TRACE_LEVEL_INFO, string ("SflowLocalConfigWorker::SflowSendToClientsCallback: Response from instance ")+i);
            
            if (WAVE_MESSAGE_SUCCESS == sendStatus) {
            	NSM_PLUG_INFO_S(string("WAVE_MESSAGE_SUCCESS == sendStatus"));
                sw_resp_p = dynamic_cast<SflowShowMessage *> (ctx_p->getResultingMessageForPhase1 (instancesToSendToForPhase1[i]));
              //  NSM_PLUG_ASSERT_PTR(sw_resp_p);
                if(sw_resp_p)
                {
                if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
                {
                //  req_msg_p->copyBuffersFrom(*sw_resp_p);
                	NSM_PLUG_DBG_S(string("Calling copyAllBuffers"));
                    req_msg_p->copyAllBuffers(*sw_resp_p);
                }
                else
                {
                	NSM_PLUG_DBG_ERR_S(string("Response processing: The location: ") + instancesToSendToForPhase1[i] + string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
                }
               }
	        }
            else {
            	NSM_PLUG_DBG_ERR_S(string("Send Status is not WAVE_MESSAGE_SUCCESS!"));
            }
        }

        delete (ctx_p->getPManagementInterfaceMessageForPhase1());
        delete ctx_p;

       if(numberOfInstancesToSendTo == 0)
      {
    	   NSM_PLUG_DBG_ERR_S(string("numberOfInstancesToSendTo == 0?. Might result in error"));
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
      }else {
        seq_ctx_p->executeNextStep(sendToClientsCompletionStatus);
	  }

        return;
    }
    void SflowLocalConfigWorker::sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
    	NSM_PLUG_DBG_S(string("Entering: "));
        SflowLocalShowMessage		*msg_p = NULL;
        SflowLocalShowMessage		*loc_msg_p = NULL;
        WaveSendToClusterContext	*send_ctx_p = NULL;
        bool						isGwRequest = false;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal;

        msg_p = dynamic_cast<SflowLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR(msg_p);

        mappedId = DcmToolKit::getLocalMappedId();
        NSM_PLUG_DBG_S(string("Setting mappedId=")+mappedId);


        loc_msg_p = new SflowLocalShowMessage(msg_p->getIfName(), msg_p->getCmdCode(), isGwRequest);
        NSM_PLUG_ASSERT_PTR(loc_msg_p);
        NSM_PLUG_DBG_S(string("interface = ") +  msg_p->getInterface() );
		loc_msg_p->setInterface(msg_p->getInterface());
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&SflowLocalConfigWorker::SflowLocalShowSendToClusterCallback), seq_ctx_p);
        NSM_PLUG_ASSERT_PTR(send_ctx_p);
        if (!send_ctx_p)
        {
            NSM_PLUG_DBG_S(string("The send_ctx_p is NULL."));
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }

        loc_msg_p->setMappedId(mappedId);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        if (NsmUtils::isValidTwoTuple(msg_p->getIfName()))  
        {
        	NSM_PLUG_DBG_S(string("Valid Two Tuple. sendToLocal = true"));
            sendToLocal = true;
        } else if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) 
        {
        	NSM_PLUG_DBG_S(string("sendToLocal = false. Send  to the targeted LocationId pointed to by the mappedId"));
            /* Send  to the targetted LocationId pointed to by the mappedId */
            sendToLocal = false;
            locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
            locations.push_back(locationId);
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
        else
        {
        	NSM_PLUG_DBG_S(string("sendToLocal = false"));
            sendToLocal = false;
        }
        if (sendToLocal)
        {
            locationId = NsmUtils::getLocalLocationId();
            NSM_PLUG_DBG_S(string("locationId = ")+locationId);
            if (locationId)
            {
                locations.push_back(locationId);
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }

        sendToWaveCluster(send_ctx_p);
        return;
    }

    ResourceId SflowLocalConfigWorker::sendShowToSflowDaemon (PrismLinearSequencerContext *ctx_p)
    {
    	NSM_PLUG_DBG_S(string("Entering..."));
        SflowShowMessage *cl_msg_p = NULL;
        ResourceId      status = WAVE_MESSAGE_ERROR;
        SI32            mappedId = 0;
        string          ifName = "";
        SflowLocalShowMessage *msg_p = NULL;

        char bpStr[4]= "";
        int bpId = 0;
        char *str1,*str2;


        if(NULL==ctx_p->getPPrismMessage())
        {
            NSM_PLUG_DBG_S(string("The send_ctx_p is NULL."));
            ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
            return status;
        }

        msg_p = dynamic_cast<SflowLocalShowMessage *> (ctx_p->getPPrismMessage ());

        mappedId = DcmToolKit::getLocalMappedId();

        NSM_PLUG_DBG_S(string("Setting in the SflowLocalShowMessage - Mapped Id:")+mappedId);;
        msg_p->setMappedId(mappedId);

        if (mappedId == (-1))
        { 
            mappedId = 0;
        }


		bool isLinecardCapabiltyPresent = Capabilities::isCapabilityPresent(FEAT_CAPABILITY_LINECARD);
		NSM_PLUG_DBG_S(string("Is platform Linecard Capable=")+isLinecardCapabiltyPresent);
        if(isLinecardCapabiltyPresent && ( msg_p->getCmdCode() ==  SFLOWSHOWSFLOWALL || msg_p->getCmdCode() == SFLOW_CLEAR_ALL_STATS))
        {
        	NSM_PLUG_DBG_S(string("Sending Show Command to linecard clients") + string(" CmdCode =") + msg_p->getCmdCode() + string ("ifName =")  + msg_p->getIfName());
        	NSM_PLUG_DBG_S(string("CmdCode is SFLOWSHOWSFLOWALL or SFLOW_CLEAR_ALL_STATS"));
        	bool isGwReq = false;
		    SflowShowMessage *m = new SflowShowMessage();
		    m->setCmdCode( msg_p->getCmdCode());
		    m->setIsGwRequest(isGwReq);
		    m->setIfName(msg_p->getIfName());
		    m->setMappedId(mappedId);
            WaveSendToClientsContext *pWaveSendToClientsContext = new WaveSendToClientsContext (this,
                                                                        reinterpret_cast<PrismAsynchronousCallback> (&SflowLocalConfigWorker::SflowSendToClientsCallback),
                                                                        ctx_p , "Sflagt", true);
            pWaveSendToClientsContext->setPManagementInterfaceMessageForPhase1 (m);
            sendToWaveClients (pWaveSendToClientsContext);
            NSM_PLUG_INFO_S(string("Successfully processed the show/clear message response from the agents in the linecard"));
			return WAVE_MESSAGE_SUCCESS;
        }



        if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))
        {
            NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
            NSM_PLUG_DBG_S(string("The three tuple is .")+ string(msg_p->getIfName()) + string(" and the two tuple is .") + string(ifName));
        }
        else
        {
            ifName = msg_p->getIfName();
            NSM_PLUG_DBG_S(string("The two tuple is .")+ string(ifName));
        }
        cl_msg_p = new SflowShowMessage(ifName, msg_p->getCmdCode(), mappedId);
		NSM_PLUG_DBG_S(string("interface type = ") +  msg_p->getInterface() );
		NSM_PLUG_DBG_S(string("interface name = ") +  msg_p->getIfName() );
		cl_msg_p->setInterface(msg_p->getInterface());

		const char* ifcNameStr = ifName.c_str();
        if((str1 = strstr((char *) ifcNameStr, "/")))
        {
            if((str2 =strstr(str1+1,"/")))
            {
                strncpy(bpStr,str1+1,str2 - str1 );
                bpId = atoi(bpStr);
            }
            else
            {
                strncpy(bpStr,ifcNameStr,str1 - ifcNameStr);
                bpId = atoi(bpStr);
            }
        }
        NSM_PLUG_DBG_S(string("linecard id of the interface = ") +  bpId );

		if((msg_p->getCmdCode() == SFLOWSHOWSFLOWALL || msg_p->getCmdCode() == SFLOWSHOWSFLOWINTERFACE || msg_p->getCmdCode() == SFLOW_CLEAR_INTERFACE_STATS || msg_p->getCmdCode() == SFLOW_CLEAR_ALL_STATS)) {
		  int partionId = -1;
		  char clientName[16] = "sflagt";
		  partionId =  DcmToolKit::getActivePartitionIdBySlot(bpId);
		  sprintf(clientName, "%s%d", "Sflagt_p", partionId);
		  NSM_PLUG_DBG_S(string("Calling  sendSynchronouslyToWaveClient to Sflagt_p") + partionId);
		  status = sendSynchronouslyToWaveClient (clientName,cl_msg_p,bpId);
		}
		else {
			NSM_PLUG_DBG_S(string("Calling  sendSynchronouslyToWaveClient to Sflow"));
			status = sendSynchronouslyToWaveClient("Sflow", cl_msg_p);
		}

        NSM_PLUG_DBG_S(string("status:")+status);

        if(NULL==cl_msg_p)
        {
            NSM_PLUG_DBG_S(string("The SflowShowMessage is NULL."));
            ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
            return WAVE_MESSAGE_ERROR;
        }

        NSM_PLUG_DBG_S(string("Sending Show Command to Cluster") + string(" MsgId =")  + cl_msg_p->getMessageId()  + string(" CmdCode =") + cl_msg_p->getCmdCode() + string(" ifName =")  + cl_msg_p->getIfName() + string(" mappedId =") + cl_msg_p->getMappedId());

        if (status == WAVE_MESSAGE_SUCCESS)
        {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
            {
                NSM_PLUG_DBG_S(string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                //bpatel: To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;

				NSM_PLUG_DBG_S(string("The Completion Status != WAVE_MESSAGE_SUCCESS"));
				ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
				return WAVE_MESSAGE_ERROR;

            }

            if (cl_msg_p->getClientStatus() != 0)
            { 
                NSM_PLUG_DBG_S(string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                //bpatel: To avoid memory leak
                status = msg_p->getCompletionStatus();
                delete cl_msg_p;

				NSM_PLUG_DBG_S(string("The Client Status != 0"));
				ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
				return WAVE_MESSAGE_ERROR;

            }
        }

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            NSM_PLUG_DBG_S(string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
            //bpatel: To avoid memory leak
            delete cl_msg_p;

			NSM_PLUG_DBG_S(string("The status != WAVE_MESSAGE_SUCCESS"));
			ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
			return WAVE_MESSAGE_ERROR;

        }

        NSM_PLUG_DBG_S(string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());

        msg_p->copyAllBuffers(*cl_msg_p);
        status = cl_msg_p->getCompletionStatus();
        NSM_PLUG_DBG_S(string("completion status: ") + status);
        delete cl_msg_p;
        ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return WAVE_MESSAGE_SUCCESS;
    }

    void SflowLocalConfigWorker::SflowLocalShowMessageHandler (SflowLocalShowMessage *pSflowLocalShowMessage)
    {
    	NSM_PLUG_DBG_S(string("Entering: "));
        if (pSflowLocalShowMessage->getIsGwRequest())
        {
        	NSM_PLUG_DBG_S(string("Message is GwRequest: "));
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&SflowLocalConfigWorker::sendShowCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&SflowLocalConfigWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&SflowLocalConfigWorker::prismLinearSequencerFailedStep)
            };
            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pSflowLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        }
        else
        { 
        	NSM_PLUG_DBG_S(string("Message is NOT GwRequest: "));
        	PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&SflowLocalConfigWorker::sendShowToSflowDaemon),
                reinterpret_cast<PrismLinearSequencerStep> (&SflowLocalConfigWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&SflowLocalConfigWorker::prismLinearSequencerSucceededStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pSflowLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;

        }
    }

    ResourceId SflowLocalConfigWorker::errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status)
    {
        NSM_PLUG_DBG_TRACE();
        loc_msg_p -> setCompletionStatus(status);
        if(WAVE_MESSAGE_SUCCESS == status)
        {
            if(WAVE_MESSAGE_SUCCESS != c_msg_p -> getCompletionStatus())
            {
                status = c_msg_p -> getCompletionStatus();
                loc_msg_p->setCompletionStatus(status);
            }
            else if(WAVE_MESSAGE_SUCCESS != c_msg_p -> getClientStatus())
            {
                status =  c_msg_p -> getClientStatus();
                loc_msg_p -> setCompletionStatus(status);
                NSM_PLUG_DBG_S(string("Error From Sflow Client") + status);
            }
        }
        delete c_msg_p;
        return status;
    }	

    ResourceId SflowLocalConfigWorker::mapBackEndErrorToDcmdError(int rc)
    {
        ResourceId errorId = WAVE_MESSAGE_SUCCESS;
        switch (rc)
        {
            case SFLOW_DCM_SET_ERR_PORT_NOT_EXIST:
                errorId = WRC_SFLOW_SET_ERR_PORT_NOT_EXIST;
                break;
            case SFLOW_DCM_SET_ERR_COLLECTOR_NOT_CONFIGURED:
                errorId = WRC_SFLOW_SET_ERR_COLLECTOR_NOT_CONFIGURED;
                break;
            case SFLOW_DCM_SET_ERR_MAX_COLLECTORS_CONFIGURED:
                errorId = WRC_SFLOW_SET_ERR_MAX_COLLECTORS_CONFIGURED;
                break;
            default:
                errorId = WAVE_MESSAGE_SUCCESS;
                break;
        }
        return (errorId);
    }

    void SflowLocalConfigWorker::SflowProfileConfigMessageHandler(SflowProfilePrismMessage *pMessage)
    {
    	NSM_PLUG_DBG_S("Entering");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::sflowProfileLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SflowLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();

    }

    ResourceId SflowLocalConfigWorker::sflowProfileLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
    	NSM_PLUG_DBG_S("Entering");
        ResourceId status = WAVE_MESSAGE_ERROR;
        if(NULL==pPrismSynchronousLinearSequencerContext->getPPrismMessage ()) {
            return status;
        }

        SflowProfilePrismMessage *pMsg = dynamic_cast<SflowProfilePrismMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        SflowClientSflowProfileConfigSPMessage *m = new SflowClientSflowProfileConfigSPMessage();
        m->setProfileName(pMsg->getProfileName());
        m->setSamplingRate(pMsg->getSamplingRate());
        m->setOpCode(pMsg->getOpCode());
        NSM_PLUG_DBG_S("Sending the message to Sflow daemon");
        status = sendSynchronouslyToWaveClient ("Sflow", m);
        NSM_PLUG_DBG_S(string("Exiting with status=")+status);
        return errorCode(pMsg, m, status);
    }
}
