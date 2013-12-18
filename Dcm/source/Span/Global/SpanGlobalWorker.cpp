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
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
//#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Span/Global/SpanGlobalObjectManager.h"
#include "Span/Global/SpanGlobalWorker.h"
#include "Span/Global/SpanGlobalSpanGlobalConfigMessage.h"
#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "Span/Global/SpanGlobalTypes.h"
#include "Span/Local/SpanLocalSpanLocalConfigMessage.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "nsm_pmirror.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Utils/DceClusterUtils.h"
#include "Nsm/Local/Span/NsmLocaldestinationMirrorportMessage.h"
namespace DcmNs
{
	SpanGlobalWorker::SpanGlobalWorker ( SpanGlobalObjectManager *pSpanGlobalObjectManager) 
		: WaveWorker  (pSpanGlobalObjectManager)
	{
		SpanGlobalConfigManagedObject    SpanGlobalConfigManagedObject    (pSpanGlobalObjectManager);
	    SpanGlobalConfigManagedObject.setupOrm ();																			
		addManagedClass (SpanGlobalConfigManagedObject::getClassName (), this);
		addOperationMap (SPANGLOBALSPANGLOBALCONFIG, 
			reinterpret_cast<PrismMessageHandler> (&SpanGlobalWorker::SpanGlobalSpanGlobalConfigMessageHandler));
	}

    SpanGlobalWorker::~SpanGlobalWorker ()
	{
	}
	
	PrismMessage  *SpanGlobalWorker::createMessageInstance(const UI32 &operationCode)
	{
	     PrismMessage *pPrismMessage = NULL;
         switch (operationCode)
         {
            case SPANGLOBALSPANGLOBALCONFIG :
                pPrismMessage = new SpanGlobalSpanGlobalConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
         }
         return (pPrismMessage);
    }
	    
	WaveManagedObject  *SpanGlobalWorker::createManagedObjectInstance(const string &managedClassName)
	{
	     WaveManagedObject *pWaveManagedObject = NULL;
         if ((SpanGlobalConfigManagedObject::getClassName ()) == managedClassName  && NULL != getPWaveObjectManager())
         {
              pWaveManagedObject = new SpanGlobalConfigManagedObject(dynamic_cast<SpanGlobalObjectManager*>(getPWaveObjectManager()));
         }
         else
         {
              trace (TRACE_LEVEL_FATAL, "SpanGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
         }
	     return (pWaveManagedObject);
	}

	void SpanGlobalWorker::validateSpanGlobalConfigStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
	//     ResourceId status = WAVE_MESSAGE_SUCCESS;
	}

	ResourceId ClusterGetStatusFromContext_Span(WaveSendToClusterContext  *ctx_p)
	{
		NSM_PLUG_DBG_S(string("Entering"));
		LocationId locationid;
    	ResourceId sendToClusterCompletionStatus = ctx_p->getCompletionStatus ();
    	NSM_PLUG_DBG_S(string("sendToClusterCompletionStatus=")+sendToClusterCompletionStatus);
    	PrismLinearSequencerContext *seq_ctx_p;
		vector< vector <Attribute *> > ErrArguments;
		vector <Attribute *> errArgs;
    	string errString;
    	seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
		
		SpanGlobalSpanGlobalConfigMessage *pMsg = NULL;
		pMsg = reinterpret_cast<SpanGlobalSpanGlobalConfigMessage *>(seq_ctx_p->getPPrismMessage());
		
		UI32 session_id = pMsg->getSession_id();

    	if (sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS && ctx_p->getIsPartialSuccessCaseFlag())
    	{
			ResourceId errCode ;
			vector<LocationId> sendLocations = ctx_p->getLocationsToSendToForPhase1();
			for (UI32 i = 0; i < sendLocations.size(); i++)
			{
				errCode = ctx_p->getCompletionStatusForPhase1(sendLocations[i]);
				errArgs.clear();

				if(errCode == WRC_NSM_SPAN_SESSION_DOESNOT_EXIST)
					errArgs.push_back (new AttributeUI32(session_id,"session_id"));
				ErrArguments.push_back(errArgs);
			}

			ClusterGetErrorStringWithArgsForFailedLocation(ctx_p, errString, ErrArguments);

			if (errString.size())
				(seq_ctx_p->getPPrismMessage())->setMessageString(errString);
    	} 
		else if (sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES ) 
		{
       		ClusterGetFirstFailureLocation(ctx_p, locationid);
    	   	sendToClusterCompletionStatus = ctx_p->getCompletionStatusForPhase1(locationid);
    	}
   		ctx_p->setCompletionStatus(sendToClusterCompletionStatus);
		return sendToClusterCompletionStatus;
	}

	void SpanGlobalWorker::SpanGlobalSendToClusterCallback (WaveSendToClusterContext *ctx_p)
	{
		NSM_PLUG_INFO_S("Process the cluster callback");

        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext_Span(ctx_p);
        ResourceId sendToLocalCompletionStatus = WAVE_MESSAGE_SUCCESS;
        NSM_PLUG_INFO_S(string("sendToClusterCompletionStatus = ") + sendToClusterCompletionStatus);
		
        PrismLinearSequencerContext *seq_ctx_p;
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
	
        SpanGlobalSpanGlobalConfigMessage *pMsg = NULL;
        pMsg = reinterpret_cast<SpanGlobalSpanGlobalConfigMessage *>(seq_ctx_p->getPPrismMessage());

		if(pMsg->getMessageType() == SPAN_LOCAL_MSG_CONFIG)
		{
	        LocationId locationid;
    	    NsmUtils::ClusterGetFirstFailureLocation(ctx_p, locationid);
       	 	sendToLocalCompletionStatus = ctx_p->getCompletionStatusForPhase1(locationid);
       	    NSM_PLUG_INFO_S(string("sendToLocalCompletionStatus_SPAN_LOCAL_MSG_CONFIG = ") + sendToLocalCompletionStatus);
			
			if(sendToLocalCompletionStatus != WAVE_MESSAGE_SUCCESS)
			{
				NSM_PLUG_DBG_ERR_S(string("sendToLocalCompletionStatus != WAVE_MESSAGE_SUCCESS"));
				SpanLocalSpanLocalConfigMessage *phase1Msg = 
					reinterpret_cast<SpanLocalSpanLocalConfigMessage *>(ctx_p->getResultingMessageForPhase1(locationid));
		    	prismAssert (NULL != phase1Msg, __FILE__, __LINE__);
				pMsg->copyAllBuffers(*phase1Msg);
			}
		}
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p->getPPrismMessageForPhase2();
        delete ctx_p;
        if(pMsg->isCapabilityProbeMessage()) {
        	NSM_PLUG_DBG_S(string("Capability Probe message. Send the errors without actually performing the configs"));
        	seq_ctx_p->executeNextStep(sendToLocalCompletionStatus);
        }
        else {
        	seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        }
        return;																						
	}
	
	void SpanGlobalWorker::SpanGlobalConfigMessageStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		trace (TRACE_LEVEL_DEVEL, "SpanGlobalWorker::SpanGlobalConfigMessageStep..Entering");
		if(NULL==pPrismLinearSequencerContext->getPPrismMessage())
        {
            return;
        }
		
	    SpanGlobalSpanGlobalConfigMessage *pSpanGlobalSpanGlobalConfigMessage = dynamic_cast<SpanGlobalSpanGlobalConfigMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		if (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_NO_LINECARD)
		{
			trace (TRACE_LEVEL_INFO, "SpanGlobalWorker::SpanGlobalConfigMessageStepMessage from Local ObjectManager for slotFailover().So returning ");
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}
	    
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		string str_twoTupleIfName;

		if ((((pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET) 
				|| (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET)) 
				&& (pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_CONFIG)) 
			|| ((pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET) 
				&& (pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_SESSION)) ) {
			/* Send a message to Nsm LocalObject Manager about destination mirrored port*/
			
			NSM_PLUG_DBG_S(string("Destination Interface Type:")+pSpanGlobalSpanGlobalConfigMessage->getDestethernet());
			//If the destination interface is a physical port, then only send the message.
            // For remote_span, vlan interface, it is configured through the NsmGlobalUpdateVlanIntfMessage and IntfConfGlobalWorker
			//TODO Use enums rather than magic number for the dest intf type
			if(pSpanGlobalSpanGlobalConfigMessage->getDestethernet() != DESTINATION_TYPE_RSPAN) {
				NsmLocaldestinationMirrorportMessage *pNsmLocaldestinationMirrorportMessage = new NsmLocaldestinationMirrorportMessage();
				LocationId DestportlocationId = 0;
				ResourceId vcs_status = DcmToolKit::isVcsEnabled();
				if (WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status)
				{
					string threeTupleIfName;
					threeTupleIfName = pSpanGlobalSpanGlobalConfigMessage->getDest_ifName();
					SI32 DestportmappedId = NsmUtils::getMappedIdFromIfName(threeTupleIfName);
					DestportlocationId = DcmToolKit::getLocationIdFromMappedId(DestportmappedId);
					trace (TRACE_LEVEL_DEBUG, string("SpanGlobalWorker::SpanGlobalConfigMessageStep LocationID : ") +  DestportlocationId);
				}
				else
				{
					DestportlocationId = NsmUtils::getLocalLocationId();
				}


				if (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET) {
					pNsmLocaldestinationMirrorportMessage->setOpcode(SPAN_ADD_MIRRORED_PORT);
				}
				else if (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET) {
					pNsmLocaldestinationMirrorportMessage->setOpcode(SPAN_DEL_MIRRORED_PORT);
				}
				trace(TRACE_LEVEL_DEVEL, string("SpanGlobalWorker::SpanGlobalConfigMessageStep: Sending the NsmLocaldestinationMirrorportMessage"));
				pNsmLocaldestinationMirrorportMessage->setDestifName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName());
				//status = sendSynchronously (pNsmLocaldestinationMirrorportMessage, DestportlocationId);
				status = sendSynchronously (pNsmLocaldestinationMirrorportMessage);
				if (status == WAVE_MESSAGE_SUCCESS &&
					pNsmLocaldestinationMirrorportMessage->getCompletionStatus() == WAVE_MESSAGE_SUCCESS) {
					NSM_PLUG_INFO_S(string("Successfully configured the destination interface MO"));
					status = WAVE_MESSAGE_SUCCESS;
				} else {
					NSM_PLUG_DBG_ERR_S(string("Some failure occurred during destination config"));
					if (status == WAVE_MESSAGE_SUCCESS) {
						status = pNsmLocaldestinationMirrorportMessage->getCompletionStatus();
						NSM_PLUG_DBG_ERR_S(string("Completion Status during destination config:")+status);
					} else {
						NSM_PLUG_DBG_ERR_S(string("Setting status to  WAVE_MESSAGE_ERROR"));
						status = WAVE_MESSAGE_ERROR;
					}
					delete pNsmLocaldestinationMirrorportMessage;
					pPrismLinearSequencerContext->executeNextStep(status);
					return;
				}

				if(pNsmLocaldestinationMirrorportMessage)
				{
					delete pNsmLocaldestinationMirrorportMessage;
				}
			}
			else {
				trace(TRACE_LEVEL_DEBUG, string("SpanGlobalWorker::SpanGlobalConfigMessageStep: Dest Interface is a remote_span vlan interface"));
			}
		}
		
		if(pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_SFP_BREAKOUT_CLEAR_LOCAL_MSG_CONFIG)
		{
			pSpanGlobalSpanGlobalConfigMessage->setMessageType(SPAN_LOCAL_MSG_CONFIG);
		}

		WaveSendToClusterContext    *send_ctx_p;
		//UI32                  		locationId = 0;
        SI32                        srcMappedId = 0;
        SI32                        dstMappedId = 0;
		vector<UI32> 				locations;
		
	    SpanLocalSpanLocalConfigMessage *m = new SpanLocalSpanLocalConfigMessage();
	    m->setSession_id(pSpanGlobalSpanGlobalConfigMessage->getSession_id());
		if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName()))
		{
			NsmUtils::getTwoTupleIfName(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName(), str_twoTupleIfName);
			m->setSource_ifName(str_twoTupleIfName);
		}
		else
			m->setSource_ifName(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName());
	    if(pSpanGlobalSpanGlobalConfigMessage->getDestethernet() != DESTINATION_TYPE_RSPAN) {
	    	trace(TRACE_LEVEL_DEVEL, string("SpanGlobalWorker::SpanGlobalConfigMessageStep: Dest Interface is a Physical interface"));
			if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName()))
			{
				NsmUtils::getTwoTupleIfName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName(), str_twoTupleIfName);
				m->setDest_ifName(str_twoTupleIfName);
			}
			else
				m->setDest_ifName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName());
	    }
	    else {
	    	trace(TRACE_LEVEL_DEVEL, string("SpanGlobalWorker::SpanGlobalConfigMessageStep: Dest Interface is a remote_span vlan interface"));
	    	stringstream vlanif_str;
	    	vlanif_str << pSpanGlobalSpanGlobalConfigMessage->getDestVlanId();
	    	string dstVlanIdString = vlanif_str.str();

	    	trace(TRACE_LEVEL_DEBUG, string("SpanGlobalWorker::SpanGlobalConfigMessageStep: Setting the Dest Intf as the Vlan Intf ")+dstVlanIdString);
	    	m->setDest_ifName(dstVlanIdString);
	    }

		trace(TRACE_LEVEL_DEBUG, string("SpanGlobalWorker::SpanGlobalConfigMessageStep:Source and Dest ports in Global_worker:")
			+ m->getSource_ifName().c_str()
			+ "-"
			+ m->getDest_ifName().c_str());
		
        m->setDirection(pSpanGlobalSpanGlobalConfigMessage->getDirection());
		m->setSrcethernet(pSpanGlobalSpanGlobalConfigMessage->getSrcethernet());
		m->setDestethernet(pSpanGlobalSpanGlobalConfigMessage->getDestethernet());
        m->setDescription(pSpanGlobalSpanGlobalConfigMessage->getDescription());
        m->setOpCode(pSpanGlobalSpanGlobalConfigMessage->getOpCode());
        m->setMessageType(pSpanGlobalSpanGlobalConfigMessage->getMessageType());


		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
			(&SpanGlobalWorker::SpanGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

		if (NsmUtils::isValidTwoTuple(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName()))
        {
			//locationId = NsmUtils::getLocalLocationId();
        }
		else if (NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName()))
		{
			srcMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName());
			NSM_PLUG_DBG_S(string("Rbridge Id of the source interface:")+srcMappedId);
			//locationId = DcmToolKit::getLocationIdFromMappedId(srcMappedId);
		}
		if (NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName())) {
			dstMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName());
			NSM_PLUG_DBG_S(string("Rbridge Id of the destination interface:")+dstMappedId);
		}
		m->setTrillSA(srcMappedId);
		m->setTrillDA(dstMappedId);

		/*
		//SA and RSPAN case only. VCS case should send it to all nodes.
		if (( (srcMappedId == dstMappedId) ||
				(pSpanGlobalSpanGlobalConfigMessage->getDestethernet() == DESTINATION_TYPE_RSPAN))
			&& (locationId !=0))
		{
			NSM_PLUG_INFO_S(string("For Phase1 send, add the locationId:")+locationId);
			locations.push_back(locationId);
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		}
		*/
	    send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return;
	}
	
	void SpanGlobalWorker::SpanGlobalConfigfailedStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NSM_PLUG_INFO_S("Entering");
		SpanGlobalSpanGlobalConfigMessage *pSpanGlobalSpanGlobalConfigMessage = dynamic_cast<SpanGlobalSpanGlobalConfigMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
		ResourceId status = pPrismLinearSequencerContext->getCompletionStatus();

        /* First Check for status if it is error thrown from NSMLOCALOM 
		 	then return from here only */
		if (status == NSM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG) {
			trace(TRACE_LEVEL_DEBUG, string("SpanGlobalWorker::SpanGlobalConfigfailedStep Returning due to error in 1st Step from NSM"));
			prismLinearSequencerFailedStep (pPrismLinearSequencerContext);
			return;
		}
			
		/* First get count of span MO specific destination port name */
		UI32 count = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName(),"dest_ifName"));
		querySynchronouslyForCount (&syncQueryCtxt, count);
		
		/* If count is greater then 0 then no need to send any message to nsm to revert any previous opration*/
		if ((count > 0) && (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET)) {
			trace(TRACE_LEVEL_ERROR, string("SpanGlobalWorker::SpanGlobalConfigfailedStep Returning as port is already a mirrored destination port"));
			prismLinearSequencerFailedStep (pPrismLinearSequencerContext);
			return;
		}
	
		string str_twoTupleIfName;
		
		if (((pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET)
                || (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET))
                && (pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_CONFIG)) {
			/* Send a message to Nsm LocalObject Manager about destination mirrored port
			 * and Revert What we did in 1st Step*/
			
			NsmLocaldestinationMirrorportMessage *pNsmLocaldestinationMirrorportMessage = new NsmLocaldestinationMirrorportMessage();
            LocationId locationId = 0;
            ResourceId vcs_status = DcmToolKit::isVcsEnabled();
            if (WAVE_PERSISTENCE_CLUSTER_ENABLED == vcs_status)
            {
                string threeTupleIfName;
                threeTupleIfName = pSpanGlobalSpanGlobalConfigMessage->getDest_ifName();
                SI32 mappedId = NsmUtils::getMappedIdFromIfName(threeTupleIfName);
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
                trace (TRACE_LEVEL_ERROR, string( "SpanGlobalWorker::SpanGlobalConfigfailedStep: : LocationID : ") +  locationId);
            }
            else
            {
                locationId = NsmUtils::getLocalLocationId();
            }
			
			pNsmLocaldestinationMirrorportMessage->setDestifName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName());
			
			/* Set opcode to reverse of wht we sent previously */
			if (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET) {
				pNsmLocaldestinationMirrorportMessage->setOpcode(SPAN_ADD_MIRRORED_PORT);
			}
			else if (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET) {
				pNsmLocaldestinationMirrorportMessage->setOpcode(SPAN_DEL_MIRRORED_PORT);
			}
			
			status = sendSynchronously (pNsmLocaldestinationMirrorportMessage, locationId);
			if (status != FRAMEWORK_SUCCESS) {
				trace (TRACE_LEVEL_ERROR, string("SpanGlobalWorker::SpanGlobalConfigfailedStep: Message sent to NSM Failed"));
				status = WAVE_MESSAGE_ERROR;
			} else {
				trace (TRACE_LEVEL_DEBUG, string("SpanGlobalWorker::SpanGlobalConfigfailedStep: Message sent to NSM Succeeded"));
				status = WAVE_MESSAGE_SUCCESS;
			}
            if(pNsmLocaldestinationMirrorportMessage) {
                delete pNsmLocaldestinationMirrorportMessage;
            }
		}	
	    prismLinearSequencerFailedStep (pPrismLinearSequencerContext);
		return;
	}
			
		
	ResourceId SpanGlobalWorker::SpanGlobalConfigMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)		
	{
		SpanGlobalSpanGlobalConfigMessage *pSpanGlobalSpanGlobalConfigMessage = 
			dynamic_cast<SpanGlobalSpanGlobalConfigMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
		SpanGlobalConfigManagedObject *PSpanGlobalConfigManagedObject = NULL;
		vector<WaveManagedObject *> *pResults = NULL;
		int createFlag = 1;
		string default_description = "";
		string default_source_ifname = "None";
		string default_dest_ifname = "None";
		UI32 enum_default = ENUM_DEFAULT_VALUE;
		UI32 default_rspan_vlanid = RSPAN_VLAN_DEFAULT_VALUE;
		
		int session_id = pSpanGlobalSpanGlobalConfigMessage->getSession_id();
		string classname = "SpanGlobalConfigManagedObject";
		
		if((pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_SHOW_SESSION) 
			|| (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_SHOW_ALL) )
			return pSpanGlobalSpanGlobalConfigMessage->getCompletionStatus ();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32 (session_id, "session_id"));
		pResults = querySynchronously (&syncQueryCtxt);
		
		if (NULL != pResults) 
		{
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
				createFlag = 1;
			} else if (1 == numberOfResults) {
				PSpanGlobalConfigManagedObject  = dynamic_cast<SpanGlobalConfigManagedObject*>((*pResults)[0]);
				trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
				createFlag = 0;
			}	
		}

		if ( (createFlag == 0) && (pSpanGlobalSpanGlobalConfigMessage->getOpCode() != SPAN_MSG_NO_SET ) )
    	{
			updateWaveManagedObject (PSpanGlobalConfigManagedObject);
			prismAssert (NULL != PSpanGlobalConfigManagedObject, __FILE__, __LINE__);
		}
		else if( (createFlag) && (pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_SESSION ) )
        {
            PSpanGlobalConfigManagedObject = new SpanGlobalConfigManagedObject
				(dynamic_cast<SpanGlobalObjectManager*>(getPWaveObjectManager()), 
				 session_id, default_source_ifname, default_dest_ifname, 
				 enum_default, default_description,enum_default,enum_default,enum_default,enum_default,default_rspan_vlanid);
            prismAssert (NULL != PSpanGlobalConfigManagedObject, __FILE__, __LINE__);
        }
        else if( ((pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_CONFIG) 
				&& (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET)) 
			|| ((pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_DESC) 
				&& (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET)) 
			|| ((pSpanGlobalSpanGlobalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_CONFIG)
				&& (pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_NO_LINECARD)) )
		{
            updateWaveManagedObject (PSpanGlobalConfigManagedObject);
		}
                                                                                                
		switch( pSpanGlobalSpanGlobalConfigMessage->getMessageType() )
		{
	        case SPAN_LOCAL_MSG_DESC:
			{
				NSM_PLUG_DBG_S(string("case SPAN_LOCAL_MSG_DESC "));
                 if(pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET)
                      PSpanGlobalConfigManagedObject->setDescription( pSpanGlobalSpanGlobalConfigMessage->getDescription() );
                 else
                      PSpanGlobalConfigManagedObject->setDescription( default_description );
		
				 if(pResults)
                      pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                 break;
			}
			case SPAN_LOCAL_MSG_CONFIG:
			{
				NSM_PLUG_DBG_S(string("case SPAN_LOCAL_MSG_CONFIG"));
                 if(pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_SET)
	             {
                	 NSM_PLUG_DBG_S(string("opCode == SPAN_MSG_SET"));
	                  if(PSpanGlobalConfigManagedObject->getSource_ifName() != default_source_ifname)
					  {
					      break;
					  }
					  PSpanGlobalConfigManagedObject->setSource_ifName( pSpanGlobalSpanGlobalConfigMessage->getSource_ifName() );
					  PSpanGlobalConfigManagedObject->setDest_ifName( pSpanGlobalSpanGlobalConfigMessage->getDest_ifName() );
					  PSpanGlobalConfigManagedObject->setDirection( pSpanGlobalSpanGlobalConfigMessage->getDirection() );
					  PSpanGlobalConfigManagedObject->setSource_tengiga(pSpanGlobalSpanGlobalConfigMessage->getSrcethernet() );
					  PSpanGlobalConfigManagedObject->setDest_tengiga(pSpanGlobalSpanGlobalConfigMessage->getDestethernet() );
					  PSpanGlobalConfigManagedObject->unsetEnumleaves_default();
					  PSpanGlobalConfigManagedObject->setDestVlanId(pSpanGlobalSpanGlobalConfigMessage->getDestVlanId());
					  trace (TRACE_LEVEL_INFO,"Updation of MO happened");
				 }
				 else
				 {
					 NSM_PLUG_DBG_S(string("opCode != SPAN_MSG_SET"));
					 PSpanGlobalConfigManagedObject->setSource_ifName( default_source_ifname );
					 PSpanGlobalConfigManagedObject->setDest_ifName( default_dest_ifname );
					 PSpanGlobalConfigManagedObject->setDirection( enum_default );
					 PSpanGlobalConfigManagedObject->setSource_tengiga( enum_default );
					 PSpanGlobalConfigManagedObject->setDest_tengiga( enum_default );
					 PSpanGlobalConfigManagedObject->setDestVlanId(default_rspan_vlanid);
					 PSpanGlobalConfigManagedObject->setEnumleaves_default();
				 }
				 if(pResults)
					 pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
				 break;
			}
			case SPAN_LOCAL_MSG_SESSION:
			{
				 trace (TRACE_LEVEL_INFO,"This is SPAN_LOCAL_MSG_SESSION");
				 
				 if( pSpanGlobalSpanGlobalConfigMessage->getOpCode() == SPAN_MSG_NO_SET)
				 {
					 trace (TRACE_LEVEL_INFO,"This is SPAN_MSG_NO_SET");
					 if(PSpanGlobalConfigManagedObject)
					 {
						 delete PSpanGlobalConfigManagedObject;
					 }
				 }
				 break;
			}
			default:
				 break;
		}
		
		if( (createFlag) && (PSpanGlobalConfigManagedObject) )
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(PSpanGlobalConfigManagedObject);
		
		//return pSpanGlobalSpanGlobalConfigMessage->getCompletionStatus ();
		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
		NSM_PLUG_INFO_S(string("Successfully updated the SpanglobalconfigManagedObject"));
		return WAVE_MESSAGE_SUCCESS;
	}
    																			
	void  SpanGlobalWorker::SpanGlobalSpanGlobalConfigMessageHandler
		( SpanGlobalSpanGlobalConfigMessage *pSpanGlobalSpanGlobalConfigMessage)
	{
		if(pSpanGlobalSpanGlobalConfigMessage->isCapabilityProbeMessage()) {
			NSM_PLUG_DBG_S(string("Capability Probe Message. Just perform the span capability and no config"));
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::SpanSendCapabilityProbeToClusterStep),
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::prismLinearSequencerFailedStep)
			 };
			 PrismLinearSequencerContext *pPrismLinearSequencerContext =
				 new PrismLinearSequencerContext (pSpanGlobalSpanGlobalConfigMessage, this, sequencerSteps,
					 sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

			 pPrismLinearSequencerContext->holdAll ();
			 pPrismLinearSequencerContext->start ();
		}
		else {
	// Validations and Queries should go here
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::SpanGlobalConfigMessageStep),
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::prismLinearSequencerStartTransactionStep),

	// Your configuration change code goes here
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::SpanGlobalConfigMOStep),
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::prismLinearSequencerCommitTransactionStep),
	// Programming Protocol Daemons goes here
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&SpanGlobalWorker::SpanGlobalConfigfailedStep)
			 };
			 PrismLinearSequencerContext *pPrismLinearSequencerContext =
				 new PrismLinearSequencerContext (pSpanGlobalSpanGlobalConfigMessage, this, sequencerSteps,
					 sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

			 pPrismLinearSequencerContext->holdAll ();
			 pPrismLinearSequencerContext->start ();
		}

	 }

	void SpanGlobalWorker::SpanSendCapabilityProbeToClusterStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		NSM_PLUG_DBG_S(string("Entering"));
		WaveSendToClusterContext    *send_ctx_p;
		UI32                  		locationId = 0;
        SI32                        srcMappedId = 0;
        SI32                        dstMappedId = 0;
		vector<UI32> 				locations;

		SpanGlobalSpanGlobalConfigMessage *pSpanGlobalSpanGlobalConfigMessage = dynamic_cast<SpanGlobalSpanGlobalConfigMessage *> (pPrismLinearSequencerContext->getPPrismMessage());
		string str_twoTupleIfName;

	    SpanLocalSpanLocalConfigMessage *m = new SpanLocalSpanLocalConfigMessage();
	    m->setCapabilityProbeMessage(true);
	    m->setSession_id(pSpanGlobalSpanGlobalConfigMessage->getSession_id());
		if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName()))
		{
			NsmUtils::getTwoTupleIfName(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName(), str_twoTupleIfName);
			m->setSource_ifName(str_twoTupleIfName);
		}
		else
			m->setSource_ifName(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName());
	    if(pSpanGlobalSpanGlobalConfigMessage->getDestethernet() != DESTINATION_TYPE_RSPAN) {
	    	NSM_PLUG_DBG_S(string("Dest Interface is a Physical interface"));
			if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName()))
			{
				NsmUtils::getTwoTupleIfName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName(), str_twoTupleIfName);
				m->setDest_ifName(str_twoTupleIfName);
			}
			else
				m->setDest_ifName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName());
	    }
	    else {
	    	NSM_PLUG_DBG_S(string("Dest Interface is a remote_span vlan interface"));
	    	stringstream vlanif_str;
	    	vlanif_str << pSpanGlobalSpanGlobalConfigMessage->getDestVlanId();
	    	string dstVlanIdString = vlanif_str.str();
	    	NSM_PLUG_DBG_S(string("Setting the Dest Intf as the Vlan Intf ")+dstVlanIdString);
	    	m->setDest_ifName(dstVlanIdString);
	    }

	    NSM_PLUG_DBG_S( string("Source and Dest ports in Global_worker:")
			+ m->getSource_ifName().c_str()
			+ "-"
			+ m->getDest_ifName().c_str());

        m->setDirection(pSpanGlobalSpanGlobalConfigMessage->getDirection());
		m->setSrcethernet(pSpanGlobalSpanGlobalConfigMessage->getSrcethernet());
		m->setDestethernet(pSpanGlobalSpanGlobalConfigMessage->getDestethernet());
        m->setDescription(pSpanGlobalSpanGlobalConfigMessage->getDescription());
        m->setOpCode(pSpanGlobalSpanGlobalConfigMessage->getOpCode());
        m->setMessageType(pSpanGlobalSpanGlobalConfigMessage->getMessageType());


		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>
			(&SpanGlobalWorker::SpanGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert ((send_ctx_p) != NULL, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);

		if (NsmUtils::isValidTwoTuple(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName()))
			locationId = NsmUtils::getLocalLocationId();
		else if (NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName()))
		{
			srcMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalSpanGlobalConfigMessage->getSource_ifName());
			NSM_PLUG_DBG_S(string("Rbridge Id of the source interface:")+srcMappedId);
			locationId = DcmToolKit::getLocationIdFromMappedId(srcMappedId);
		}
		if (NsmUtils::isValidThreeTuple(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName())) {
			dstMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalSpanGlobalConfigMessage->getDest_ifName());
			NSM_PLUG_DBG_S(string("Rbridge Id of the destination interface:")+dstMappedId);
		}
		m->setTrillSA(srcMappedId);
		m->setTrillDA(dstMappedId);

		if (( (srcMappedId == dstMappedId) ||
				(pSpanGlobalSpanGlobalConfigMessage->getDestethernet() == DESTINATION_TYPE_RSPAN))
			&& (locationId !=0))
			/* SA and RSPAN case only. VCS case should send it to all nodes. */
		{
			NSM_PLUG_INFO_S(string("For the phase 1 message, adding the locationId:")+locationId);
			locations.push_back(locationId);
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		}
	    send_ctx_p->setPartialSuccessFlag(true);
		send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return;

	}
}

							
		
