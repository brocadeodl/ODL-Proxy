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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita, Shivanand Tatpatti                                 *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Ssm/Local/StormControlLocalWorker.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/StormControlLocalShowMessage.h"
#include "Ssm/Local/StormControlLocalClearMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include <sstream>
#include "ClientInterface/StormControl/StormControlClientShowMessage.h"
#include "ClientInterface/StormControl/StormControlClientClearMessage.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Utils/Capabilities.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Ssm/Global/SsmPlugUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"
#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    StormControlLocalWorker::StormControlLocalWorker (SsmLocalObjectManager *pSsmLocalObjectManager)
        : WaveWorker  (pSsmLocalObjectManager)
    {
		addOperationMap (STORMCONTROLLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&StormControlLocalWorker::StormControlLocalShowMessageHandler));
		addOperationMap (STORMCONTROLLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&StormControlLocalWorker::StormControlLocalClearMessageHandler));
    }

    StormControlLocalWorker::~StormControlLocalWorker ()
    {
    }

    PrismMessage  *StormControlLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

			case STORMCONTROLLOCALSHOW :
				pPrismMessage = new StormControlLocalShowMessage ();
				break;
            case STORMCONTROLLOCALCLEAR :
                pPrismMessage = new StormControlLocalClearMessage();
                break;
			default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *StormControlLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return(0) ;
    }

    void StormControlLocalWorker::StormControlLocalShowMessageHandler 
		(StormControlLocalShowMessage *pStormControlLocalShowMessage)
    {
        if (pStormControlLocalShowMessage->getIsGwRequest())
        {
             PrismLinearSequencerStep sequencerSteps[] =
             {
                  reinterpret_cast<PrismLinearSequencerStep> 
					  (&StormControlLocalWorker::sendShowStormControlCommandToCluster),
                  reinterpret_cast<PrismLinearSequencerStep> 
					  (&StormControlLocalWorker::prismLinearSequencerSucceededStep),
                  reinterpret_cast<PrismLinearSequencerStep> 
					  (&StormControlLocalWorker::prismLinearSequencerFailedStep)
             };

             PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext
			 (pStormControlLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
             pPrismLinearSequencerContext->start ();
             return;
        }
        else
        {
             PrismSynchronousLinearSequencerStep sequencerSteps[] =
             {
                 reinterpret_cast<PrismSynchronousLinearSequencerStep> 
					 (&StormControlLocalWorker::sendShowStormControlToSsmDaemon),
                 reinterpret_cast<PrismSynchronousLinearSequencerStep> 
					 (&StormControlLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                 reinterpret_cast<PrismSynchronousLinearSequencerStep> 
					 (&StormControlLocalWorker::prismSynchronousLinearSequencerFailedStep)
             };
             PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pStormControlLocalShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
             pPrismSynchronousLinearSequencerContext->execute ();
        }
    }

    void StormControlLocalWorker::sendShowStormControlCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        StormControlLocalShowMessage        	*msg_p 		= NULL;
        StormControlLocalShowMessage        	*loc_msg_p 	= NULL;
        WaveSendToClusterContext    	      	*send_ctx_p = NULL;
        UI32                            	  	locationId 	= 0;
        vector<UI32>                      		locations;

        msg_p = dynamic_cast<StormControlLocalShowMessage *>(seq_ctx_p->getPPrismMessage ());
        
		SSM_PLUG_ASSERT_PTR(msg_p);

        loc_msg_p = new StormControlLocalShowMessage();
        
		SSM_PLUG_ASSERT_PTR(loc_msg_p);

        loc_msg_p->setIfName(msg_p->getIfName());
        loc_msg_p->setIfType(msg_p->getIfType());
        loc_msg_p->setOpCode(msg_p->getOpCode());
        loc_msg_p->setIsGwRequest(false);
        loc_msg_p->setProtocol(msg_p->getProtocol());
        
        if (QOS_BUM_INVALID_LOCATION_ID != msg_p->getLocationId()) {
        	locationId = msg_p->getLocationId();
        } else {
        	locationId = NsmUtils::getLocalLocationId();
        }

		tracePrintf(
			TRACE_LEVEL_DEBUG,
			"In StormControlLocalWorker: Sending StormControlLocalShowMessage to the locationId <%d> rbridgeid<%d> Opcode<%d> Protocol<%d> IfType<%d> IfName<%s> IsGwRequest<%d>",
			locationId, DcmToolKit::getMappedIdFromLocationId(locationId),
			loc_msg_p->getOpCode(), loc_msg_p->getProtocol(),
			loc_msg_p->getIfType(), loc_msg_p->getIfName().c_str(),
			loc_msg_p->getIsGwRequest());

        locations.push_back(locationId);

        send_ctx_p = new WaveSendToClusterContext(this,reinterpret_cast
			<PrismAsynchronousCallback>(&StormControlLocalWorker::
			StormControlLocalShowSendToClusterCallback), seq_ctx_p);
        SSM_PLUG_ASSERT_PTR(send_ctx_p);

        if (!send_ctx_p)
            return;

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        send_ctx_p->setLocationsToSendToForPhase1(locations);

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

        sendToWaveCluster(send_ctx_p);
        return;
    }

    void StormControlLocalWorker::StormControlLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p) {

        PrismLinearSequencerContext      *seq_ctx_p 	= NULL;
        StormControlLocalShowMessage     *req_msg_p 	= NULL,
                                         *resp_msg_p 	= NULL,
                                         *sw_resp_p 	= NULL;

        seq_ctx_p 	= reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p 	= dynamic_cast<StormControlLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p 	= dynamic_cast<StormControlLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

        prismAssert (req_msg_p != NULL, __FILE__, __LINE__);
        prismAssert (resp_msg_p != NULL, __FILE__, __LINE__);
        
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            
            vector<LocationId> 	locations 	=  	ctx_p->getLocationsToSendToForPhase1();
            UI32    			i 			= 	0;
            
            for (i = 0; i < locations.size(); i++) {
                
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
                
                if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    sw_resp_p = dynamic_cast<StormControlLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
                    prismAssert (sw_resp_p != NULL, __FILE__, __LINE__);
                    req_msg_p->copyAllBuffers(*sw_resp_p);
                } else {
                    trace(TRACE_LEVEL_INFO, string("Response processing: The location: ") 
                        + locations[i] + string(" returned an error: ")
                        + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        } else {
            trace(TRACE_LEVEL_INFO, string("StormControlLocalShowSendToClusterCallback returned error: ")
							+ FrameworkToolKit::localize(
									sendToClusterCompletionStatus));
        }
        
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        
        delete resp_msg_p;
        delete ctx_p;
        return;
    }
	
    ResourceId StormControlLocalWorker::sendShowStormControlToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
    {
        StormControlLocalShowMessage *msg_p     = dynamic_cast<StormControlLocalShowMessage *> (ctx_p->getPPrismMessage ());
		StormControlClientShowMessage *cl_msg_p	= NULL;
        ResourceId      			 status;
        SI32            			 mappedId 	= 0;
        string          			 ifName 	= "";

        // Validate 1: Check if the command is supported on the platform.
        if (!(Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_STORMCONTROL))) {
            trace (TRACE_LEVEL_DEBUG, "STORMCONTROL commands not supported on this platform.");
			status = WRC_NSM_ERR_DCM_STORMCONTROL_NOT_SUPPORTED;
			msg_p->setCompletionStatus(status);
            return status;
        }

        cl_msg_p 		= new StormControlClientShowMessage();
		
        if (cl_msg_p != NULL)
        {
            char buffer[25]	= "";
            buffer[24]      = '\0';
            UI32 bufferSize = sizeof (buffer);

            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
				|| (msg_p->getIfType() == IF_TYPE_FO))
            {
                /* Three tuple to two tuple conversion */
                if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))
                    NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
				else 
					ifName = msg_p->getIfName();

				if (msg_p->getIfType() == IF_TYPE_TE)
                	snprintf(buffer, bufferSize, "te%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_GI)
					snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_FO)
					snprintf(buffer, bufferSize, "fo%s", ifName.c_str());
            }

            mappedId = DcmToolKit::getLocalMappedId();

            if (mappedId == (-1))
                mappedId = 0;

            cl_msg_p->setIfName(buffer);
            cl_msg_p->setOpCode(msg_p->getOpCode());
            cl_msg_p->setMappedId(mappedId);
            cl_msg_p->setProtocol(msg_p->getProtocol());

    		tracePrintf(
				TRACE_LEVEL_DEBUG,
				"Sending StormControlLocalShowMessage to the qosd MappedId<%d> Opcode<%d> Protocol<%d> IfName<%s>",
				cl_msg_p->getMappedId(), cl_msg_p->getOpCode(),
				cl_msg_p->getProtocol(), cl_msg_p->getIfName().c_str());

            status = sendSynchronouslyToWaveClient ("qos", cl_msg_p);

            if (status == WAVE_MESSAGE_SUCCESS)
            {
                if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                {
                    trace(TRACE_LEVEL_INFO, string("Resp: Client Completion Error MsgId: ") 
						+ cl_msg_p->getMessageId() 
						+ FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                    msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }

                if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS)
                {
					trace(TRACE_LEVEL_INFO, string("cl_msg_p->getClientStatus() :") + cl_msg_p->getClientStatus());
                    trace(TRACE_LEVEL_INFO, string("Resp:  Client Error MsgId: ") 
						+ cl_msg_p->getMessageId() 
						+ FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                    msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }
            }

            if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") 
					+  cl_msg_p->getMessageId() 
					+  string("reason:") 
					+  FrameworkToolKit::localize(status));
                if(cl_msg_p)
                    delete cl_msg_p;
                return status;
            }

            trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());
            msg_p->copyAllBuffers(*cl_msg_p);
            trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");

            status = cl_msg_p->getCompletionStatus();
            if(cl_msg_p)
                delete cl_msg_p;
            return status;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
        }
        if(cl_msg_p)
            delete cl_msg_p;
        return status;
	}	

    void StormControlLocalWorker::StormControlLocalClearSendToClusterCallback ( WaveSendToClusterContext *ctx_p) {

        PrismLinearSequencerContext             *seq_ctx_p  = NULL;
        StormControlLocalClearMessage            *req_msg_p  = NULL,
                                                *resp_msg_p = NULL,
                                                *sw_resp_p  = NULL;

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<StormControlLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<StormControlLocalClearMessage *> (ctx_p->getPPrismMessageForPhase1());

        SSM_PLUG_ASSERT_PTR(req_msg_p);
        SSM_PLUG_ASSERT_PTR(resp_msg_p);
        
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            
            vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
            UI32    i = 0;
            
            for (i = 0; i < locations.size(); i++)  {
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
                
                if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    sw_resp_p = dynamic_cast<StormControlLocalClearMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
                    SSM_PLUG_ASSERT_PTR(sw_resp_p);
                } else {
                    SSM_PLUG_ERR_LVL_S(string("Response processing: The location: ") + locations[i] + 
                        string("returned error: ") + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        } else {
            trace(TRACE_LEVEL_ERROR,string("PolicerLocalClearSendToClusterCallback : returned error: ")
							+ FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }
        seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);

        delete resp_msg_p;
        delete ctx_p;

        return;
    }

    void StormControlLocalWorker::sendClearStormControlCommandToCluster(
		PrismLinearSequencerContext *seq_ctx_p) {

	StormControlLocalClearMessage *msg_p = NULL;
	StormControlLocalClearMessage *loc_msg_p = NULL;
	WaveSendToClusterContext *send_ctx_p = NULL;
	UI32 locationId = QOS_BUM_INVALID_LOCATION_ID;
	vector<UI32> locations;

	msg_p =
			dynamic_cast<StormControlLocalClearMessage *>(seq_ctx_p->getPPrismMessage());

	SSM_PLUG_ASSERT_PTR(msg_p);
	loc_msg_p = new StormControlLocalClearMessage();

	loc_msg_p->setIfName(msg_p->getIfName());
	loc_msg_p->setIfType(msg_p->getIfType());
	loc_msg_p->setOpCode(msg_p->getOpCode());
	loc_msg_p->setIsGwRequest(false);
	loc_msg_p->setProtocol(msg_p->getProtocol());

	SSM_PLUG_ASSERT_PTR(loc_msg_p);

	if (QOS_BUM_INVALID_LOCATION_ID != msg_p->getLocationId()) {
		locationId = msg_p->getLocationId();
	} else {
		locationId = NsmUtils::getLocalLocationId();
	}

	locations.push_back(locationId);

	tracePrintf(
			TRACE_LEVEL_DEBUG,
			"In StormControlLocalWorker: Sending StormControlLocalClearMessage to the locationId <%d> rbridgeid<%d> Opcode<%d> Protocol<%d> IfType<%d> IfName<%s> IsGwRequest<%d>",
			locationId, DcmToolKit::getMappedIdFromLocationId(locationId),
			loc_msg_p->getOpCode(), loc_msg_p->getProtocol(),
			loc_msg_p->getIfType(), loc_msg_p->getIfName().c_str(),
			loc_msg_p->getIsGwRequest());

	send_ctx_p =
			new WaveSendToClusterContext(
					this,
					reinterpret_cast<PrismAsynchronousCallback>(&StormControlLocalWorker::StormControlLocalClearSendToClusterCallback),
					seq_ctx_p);
	SSM_PLUG_ASSERT_PTR(send_ctx_p);

	if (!send_ctx_p)
		return;

	send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
	send_ctx_p->setLocationsToSendToForPhase1(locations);
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);

	sendToWaveCluster(send_ctx_p);
	return;
}

    ResourceId StormControlLocalWorker::sendClearStormControlToSsmDaemon ( PrismSynchronousLinearSequencerContext *ctx_p)
    {
        StormControlLocalClearMessage 	*msg_p 		= 
			dynamic_cast<StormControlLocalClearMessage *> (ctx_p->getPPrismMessage());
        StormControlClientClearMessage 	*cl_msg_p  	= NULL;
        ResourceId      			 	status;
        SI32            			 	mappedId   	= 0;
        string          			 	ifName     	= "";

        // Validate 1: Check if the command is supported on the platform.
        if (!(Capabilities::instance()->isCapabilityPresent(FEAT_CAPABILITY_STORMCONTROL))) {

            trace (TRACE_LEVEL_DEBUG, "STORMCONTROL commands not supported on this platform.");
            status = WRC_NSM_ERR_DCM_STORMCONTROL_NOT_SUPPORTED;
            msg_p->setCompletionStatus(status);
            return status;
        }

        cl_msg_p 									= new StormControlClientClearMessage ();

        if (cl_msg_p != NULL) 
		{
            char buffer[25] = "";
            buffer[24]      = '\0';
            UI32 bufferSize = sizeof (buffer);

            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
				|| (msg_p->getIfType() == IF_TYPE_FO))
			{
                /* Three tuple to two tuple conversion */
                if (NsmUtils::isValidThreeTuple(msg_p->getIfName())) 
                    NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
                else 
                    ifName = msg_p->getIfName();
                
                if(msg_p->getIfType() == IF_TYPE_TE)
                    snprintf(buffer, bufferSize, "te%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_GI)
					snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
				else if (msg_p->getIfType() == IF_TYPE_FO)
					snprintf(buffer, bufferSize, "fo%s", ifName.c_str());
            } 
            
            mappedId = DcmToolKit::getLocalMappedId();

            if (mappedId == (-1)) 
                mappedId = 0;

            cl_msg_p->setIfName(buffer);
            cl_msg_p->setOpCode(msg_p->getOpCode());
            cl_msg_p->setMappedId(mappedId);
			cl_msg_p->setProtocol(msg_p->getProtocol());
																			
            trace (TRACE_LEVEL_INFO, "sendToLocalNode: Message being sent to Ssm \n"
                + cl_msg_p->getIfName() + "\n"
                + cl_msg_p->getOpCode() + "\n"
                + cl_msg_p->getMappedId() + "\n"
                + cl_msg_p->getProtocol() + "\n"
				);

            status = sendSynchronouslyToWaveClient ("qos", cl_msg_p);

            if (status == WAVE_MESSAGE_SUCCESS) 
			{
                if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) 
				{
                    trace(TRACE_LEVEL_INFO, string("Resp: Client Completion Error MsgId: ") +  cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));

                    msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
						delete cl_msg_p;
                    
					return status;
                }
				
				if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS)
				{
                    trace(TRACE_LEVEL_INFO, string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                    trace(TRACE_LEVEL_INFO, string("Value of error message: ") + cl_msg_p->getClientStatus());
					msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }
            }

            if (status != WAVE_MESSAGE_SUCCESS) 
			{
                trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
                if(cl_msg_p)
                    delete cl_msg_p;
                return status;
            }
            trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());

            trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");

            status = cl_msg_p->getCompletionStatus();
            if(cl_msg_p)
                delete cl_msg_p;
            return status;
        } 
		else 
		{
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
        }

        if(cl_msg_p)
            delete cl_msg_p;
        
        return status;
    }

    void StormControlLocalWorker::StormControlLocalClearMessageHandler ( StormControlLocalClearMessage *pStormControlLocalClearMessage)
    {
        if (pStormControlLocalClearMessage->getIsGwRequest()) 
		{ 
			PrismLinearSequencerStep sequencerSteps[] =
        	{
            	reinterpret_cast<PrismLinearSequencerStep> 
					(&StormControlLocalWorker::sendClearStormControlCommandToCluster),
	            reinterpret_cast<PrismLinearSequencerStep> 
					(&StormControlLocalWorker::prismLinearSequencerSucceededStep),
    	        reinterpret_cast<PrismLinearSequencerStep> 
					(&StormControlLocalWorker::prismLinearSequencerFailedStep)
        	};

	        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pStormControlLocalClearMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
    	    pPrismLinearSequencerContext->start ();
        	return;
        } 
		else 
		{
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
	        {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> 
					(&StormControlLocalWorker::sendClearStormControlToSsmDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> 
					(&StormControlLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> 
					(&StormControlLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

	        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pStormControlLocalClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();
        }
	}
}


