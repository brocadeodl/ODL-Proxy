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
 *   Author : ugodavar                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Utils/Capabilities.h"
#include "Utils/DceClusterUtils.h"
#include "Utils/ProvisioningUtil.h"
#include "Nsm/Local/VLAN/GvlanConfigUtils.h"
#include "Span/Local/SpanLocalLocalObjectManager.h"
#include "Span/Local/SpanLocalConfigWorker.h"
#include "Span/Local/SpanLocalSpanLocalConfigMessage.h"
#include "Span/Local/SpanLocalShowMessage.h"
#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "Span/Local/SpanLocalTypes.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "ClientInterface/Span/SpanShowMessage.h"
#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/LldpIntfConfLocalManagedObject.h"
#include "Qos/Common/QosUtils.h"
#include "APPM/Utils/APPMUtils.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "DcmCStatus.h"
#include "nsm_pmirror.h"



namespace DcmNs
{

    SpanLocalConfigWorker::SpanLocalConfigWorker ( SpanLocalLocalObjectManager *pSpanLocalLocalObjectManager) : WaveWorker  (pSpanLocalLocalObjectManager)
    {
        addOperationMap (SPANLOCALSPANLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&SpanLocalConfigWorker::SpanLocalSpanLocalConfigMessageHandler));
		addOperationMap (SPANLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&SpanLocalConfigWorker::SpanLocalShowMessageHandler));
    }

    SpanLocalConfigWorker::~SpanLocalConfigWorker ()
    {
	}

    PrismMessage  *SpanLocalConfigWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case SPANLOCALSPANLOCALCONFIG :
                pPrismMessage = new SpanLocalSpanLocalConfigMessage ();
                break;
			case SPANLOCALSHOW :
				pPrismMessage = new SpanLocalShowMessage ();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SpanLocalConfigWorker::createManagedObjectInstance(const string &managedClassName)
    {
		return NULL;
    }

    ResourceId SpanLocalConfigWorker::validateConfigMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
    	trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::validateConfigMessageStep : Entering"));
        ResourceId status = WAVE_MESSAGE_SUCCESS;

		if(NULL==pPrismSynchronousLinearSequencerContext->getPPrismMessage ()) {
		     return WAVE_MESSAGE_ERROR;
		}

		UI32 localMappedId = DcmToolKit::getLocalMappedId();
						
		SpanLocalSpanLocalConfigMessage *pSpanLocalSpanLocalConfigMessage = dynamic_cast<SpanLocalSpanLocalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		UI32 trillSA = pSpanLocalSpanLocalConfigMessage->getTrillSA();
		UI32 trillDA = pSpanLocalSpanLocalConfigMessage->getTrillDA();
		NSM_PLUG_DBG_S(string("Local Rbridge Id:")+localMappedId+" ; Source Rbridge Id:"+trillSA+" ; Destination Rbridge Id:"+trillDA);

        if(SPAN_MSG_SET == pSpanLocalSpanLocalConfigMessage->getOpCode() && (localMappedId == trillSA || localMappedId == trillDA))
        {
            string ifName = pSpanLocalSpanLocalConfigMessage->getDest_ifName();
        	string threeTupleIfName = "";
        	NsmUtils::getThreeTupleIfName(ifName,threeTupleIfName,0);
        	NSM_PLUG_DBG_S(string("Retrieving the L3ReadyMO for the interface ")+threeTupleIfName);
            NsmL3ReadyManagedObject *pL3ReadyMo =
                NsmL3Util::PhyIfNameGetL3ReadyMO( threeTupleIfName, pPrismSynchronousLinearSequencerContext);

            if (pL3ReadyMo) {
                if (!(pL3ReadyMo->getCanDeleteL3ReadyMO())) {
                	NSM_PLUG_DBG_ERR_S(string("L3 Configs present. Cannot configure the interface as span destination! "));
                    status = WRC_NSM_ERR_DST_MIR_PORT_REMOVE_L3_CONFIG;
                }
            }
            else {
            	NSM_PLUG_INFO_S(string("NO L3ReadyMO for the interface ")+threeTupleIfName);
            }

            if (pL3ReadyMo) {
                pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
            }
            if(pSpanLocalSpanLocalConfigMessage->getDestethernet() == DESTINATION_TYPE_RSPAN) {
            	string dstRemoteVlanIntf = pSpanLocalSpanLocalConfigMessage->getDest_ifName();
            	UI32 vlanId = atoi(dstRemoteVlanIntf.c_str());
            	trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::validateConfigMessageStep : Destination RSPAN Vlan ")
            			+dstRemoteVlanIntf);
            	//The source interface cannot be a callisto interface

				if(!Capabilities::isCapabilityPresent(FEAT_CAPABILITY_RSPAN_SOURCE)) {
					NSM_PLUG_DBG_ERR_S(string("RSPAN source config is not supported on this platform:"));
					status = WRC_RSPAN_SRC_SESSION_NOTSUPPORTED_ON_PLATFORM;
				}
				if(false == NsmUtils::isRemoteSpanVlan(vlanId)) {
					NSM_PLUG_DBG_ERR_S(string("The configured destination VLAN ")+vlanId+" is not RSPAN vlan");
				   status = WRC_NSM_VLAN_NOT_RSPAN_VLAN;
				}
            }
            else if(pSpanLocalSpanLocalConfigMessage->getTrillDA() != pSpanLocalSpanLocalConfigMessage->getTrillSA()) {

            	//Span over VCS. If the source node is callisto, the config should not be supported
            	NSM_PLUG_DBG_S(string("Span over VCS. Check if the source node is callisto. Local Mapped Id=")+localMappedId);
            	//The source interface cannot be a callisto interface

				if(pSpanLocalSpanLocalConfigMessage->getTrillSA() == localMappedId && !Capabilities::isCapabilityPresent(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE)) {
					NSM_PLUG_DBG_ERR_S(string("Span over VCS -  source config is not supported on this platform:"));
					status = WRC_SPAN_VCS_SRC_NOTSUPPORTED_ON_PLATFORM;
				}
            }



            NSM_PLUG_DBG_S(string("Destination Ifc Type:")+pSpanLocalSpanLocalConfigMessage->getDestethernet());
            if(pSpanLocalSpanLocalConfigMessage->getDestethernet() != DESTINATION_TYPE_RSPAN && localMappedId == trillDA) {
            	//Destination interface is not remote-span vlan. Check if any L2/L3 config is present on the interface
            	PhyIntfLocalManagedObject *pPhyIntfMo =
            	        NsmL3Util::PhyIfNameGetPhyIfLocalMO(threeTupleIfName);
                pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pPhyIntfMo);

                if(pPhyIntfMo) {

                	NSM_PLUG_INFO_S(string("Check if there is any L2/L3 config present on the interface ")+ifName);

					//1. Check if switchport configuration is present on the destination interface
    			    if (pPhyIntfMo->getSwModeFlag()) {
    			    	NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. switchport configuration present on the interface ")+ifName);
    					status = WRC_NSM_SPAN_DEST_PORT_IN_L2;
    			    }

					//2. Check if any qos configuration is present on the destination interface
					if (QosUtils::isInterfaceQosCeeConfigured(pPhyIntfMo) ||
						QosUtils::isInterfaceQosNonCeeConfigured (pPhyIntfMo) ||
						QosUtils::isInterfaceMQCConfigured(pPhyIntfMo))
					{
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. Qos configurations present on the interface ")+threeTupleIfName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_QoS;
					}

					//3. Check if any FCoE configuration is present on the destination interface
					if (QosUtils::isInterfaceFcoeConfigured(pPhyIntfMo))
					{
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. FCoE configurations present on the interface ")+ifName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_FCOE;
					}

					//4. Check if any dot1x configuration is present on the destination interface
	                WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> pDot1xPortConfigLocalManagedObject = WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (pPhyIntfMo->getDot1xIntfConfig());
					Dot1xPortConfigLocalManagedObject *dot1xConfigMO =  pDot1xPortConfigLocalManagedObject.operator-> ();
					if(dot1xConfigMO) {
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. dot1x configurations present on the interface ")+ifName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_DOT1X;
					}

					//5. Check if any rmon configuration is present on the destination interface
					vector<WaveManagedObjectPointer<RmonHistoryLocalManagedObject> > rmonHistoryMOVector =  pPhyIntfMo->getRmonHistory();
					unsigned int size = rmonHistoryMOVector.size();
					if(size > 0) {
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. rmon configurations present on the interface ")+ifName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_RMON;
					}

					//6. Check if any sflow configuration is present on the destination interface
					if(true == pPhyIntfMo->getEnable()) {
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. sflow is enabled on the interface ")+ifName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_SFLOW;
					}

					//7. Check if lldp is enabled on the destination interface
					WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> pLldpIntfConfLocalManagedObject = WaveManagedObjectPointer<LldpIntfConfLocalManagedObject> (pPhyIntfMo->getLldpIntfConfig());
					if (pLldpIntfConfLocalManagedObject->getDisable() != true) {
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. lldp is enabled on the interface ")+ifName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_LLDP;
					}

					//8. Check if lacp is configured on the destination interface
					if(pPhyIntfMo->getLacpDefaultUp() != LACP_DEFAULT_DEFAULT_UP) {
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. lacp is configured on the interface ")+ifName);
						status = WRC_NSM_SPAN_DEST_PORT_IN_LACP;
					}

					//9. Check if any acl configuration is present on the destination interface
					if(pPhyIntfMo->getIngressEgressIpAclPolicyDirectioncount(SSMACL_BINDING_DIRECTION_BOTH_E) > 0)
					{
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. ACL configurations present on the interface ")+ifName);
						status  = WRC_NSM_SPAN_DEST_PORT_IN_ACL;
					}

					//10. Check if any UDLD configuration is present on the destination interface
					if(true == pPhyIntfMo->getUdldEnable())
					{
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. UDLD configurations present on the interface ")+ifName);
						status  = WRC_UDLD_ENABLED_PORT_CANNOT_BE_DEST_MIRROR_PORT;
					}

					NSM_PLUG_INFO_S(string("Default Value of MTU:")+NSM_MTU_DEF_VAL+" : Configured Value of MTU"+pPhyIntfMo->getMtu());
					//11. Check if the mtu of the the destination interface is not default
					if(NSM_MTU_DEF_VAL != pPhyIntfMo->getMtu())
					{
						NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. Non default MTU configuration present on the interface ")+ifName);
						status  = WRC_NSM_SPAN_DEST_PORT_IN_MTU;
					}

					//12. Check if the destination interface is a port-profile port
                    bool isPortProfiled = false;
                    APPMUtils::isPortProfilePort(pPhyIntfMo, isPortProfiled);
                    if(isPortProfiled)
                    {
                        NSM_PLUG_DBG_ERR_S(string("Cannot be made as port mirroring detination. port-profile configuration present on the interface ")+ifName);
                        status  = WRC_NSM_SPAN_DEST_PORT_PORTPROFILE_PORT;
                    }
            	}
            	else {
            		NSM_PLUG_DBG_ERR_S(string("ERROR: Failed to retrieve the physical interface ")+ifName);
            	}

            }
            else {
            	NSM_PLUG_INFO_S(string("Destination Ifc Type is RSPAN. No need to validate for L2/L3 configs"));
            }
        }
        
        pSpanLocalSpanLocalConfigMessage->setCompletionStatus(status); 
        trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::validateConfigMessageStep : Exiting with status=")+status);
        return status;
    }

    ResourceId SpanLocalConfigWorker::ConfigMessageStep(DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
    {
    	trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::ConfigMessageStep : Entering "));
		int status = WAVE_MESSAGE_ERROR;
		UI32 rspanVlanId = 0;
		if(NULL==pPrismSynchronousLinearSequencerContext->getPPrismMessage ()) {
		     return status;
		}
						
		SpanLocalSpanLocalConfigMessage *pSpanLocalSpanLocalConfigMessage = dynamic_cast<SpanLocalSpanLocalConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
		
		//Provision the vlan on the source rbridge
		UI32 trillSA = pSpanLocalSpanLocalConfigMessage->getTrillSA();
		UI32 localMappedId = DcmToolKit::getLocalMappedId();
		if(SPAN_MSG_SET == pSpanLocalSpanLocalConfigMessage->getOpCode() && localMappedId == trillSA && pSpanLocalSpanLocalConfigMessage->getDestethernet() == DESTINATION_TYPE_RSPAN) {
            string rspanVlanStr = pSpanLocalSpanLocalConfigMessage->getDest_ifName();
            UI32 vlanId = atoi(rspanVlanStr.c_str());
            NSM_PLUG_INFO_S(string("Need to provision the vlan ")+vlanId+" at the source Rbridge");
            PROVISION_VLAN(pPrismSynchronousLinearSequencerContext,vlanId);
		}

    	SpanClientLocalSpanLocalConfigMessage *m = new SpanClientLocalSpanLocalConfigMessage();
    	string str_twoTupleIfName;

    	m->setSession_id(pSpanLocalSpanLocalConfigMessage->getSession_id());      
		if(true  == NsmUtils::isValidThreeTuple(pSpanLocalSpanLocalConfigMessage->getSource_ifName()))
		{
			NsmUtils::getTwoTupleIfName(pSpanLocalSpanLocalConfigMessage->getSource_ifName(), str_twoTupleIfName);
			m->setSource_ifName(str_twoTupleIfName);
			trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::ConfigMessageStep : Src Valid Three Tuple - If Name= ")+str_twoTupleIfName);
		}
		else
			m->setSource_ifName(pSpanLocalSpanLocalConfigMessage->getSource_ifName());
		
		if(true  == NsmUtils::isValidThreeTuple(pSpanLocalSpanLocalConfigMessage->getDest_ifName()))
		{	
			NsmUtils::getTwoTupleIfName(pSpanLocalSpanLocalConfigMessage->getDest_ifName(), str_twoTupleIfName);
			m->setDest_ifName(str_twoTupleIfName);
			trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::ConfigMessageStep : Dst Valid Three Tuple - If Name= ")+str_twoTupleIfName);
		}
		else	
			m->setDest_ifName(pSpanLocalSpanLocalConfigMessage->getDest_ifName());


		if(pSpanLocalSpanLocalConfigMessage->getDestethernet() == DESTINATION_TYPE_RSPAN) {
			rspanVlanId = atoi(pSpanLocalSpanLocalConfigMessage->getDest_ifName().c_str());
			NSM_PLUG_DBG_S(string("Destination type is vlan. Hence set the vlan Id=")+rspanVlanId);
		}

    	trace(TRACE_LEVEL_DEBUG, string("SpanLocalConfigWorker::ConfigMessageStep -: Source and Dest ports in Local_worker:") + pSpanLocalSpanLocalConfigMessage->getSource_ifName().c_str() + "-"+pSpanLocalSpanLocalConfigMessage->getDest_ifName().c_str());

       	m->setDirection(pSpanLocalSpanLocalConfigMessage->getDirection());
		m->setSrcethernet(pSpanLocalSpanLocalConfigMessage->getSrcethernet());
		m->setDestethernet(pSpanLocalSpanLocalConfigMessage->getDestethernet());
       	m->setDescription(pSpanLocalSpanLocalConfigMessage->getDescription());
		m->setOpCode(pSpanLocalSpanLocalConfigMessage->getOpCode());	
   		m->setMessageType(pSpanLocalSpanLocalConfigMessage->getMessageType());
   		m->setTrillSA(pSpanLocalSpanLocalConfigMessage->getTrillSA());
   		m->setTrillDA(pSpanLocalSpanLocalConfigMessage->getTrillDA());
   		m->setRspanVlanId(rspanVlanId);

   		NSM_PLUG_INFO_S(string("OP Code=")+m->getOpCode()+":Msg Type="+m->getMessageType()+":Src Ifc="+m->getSource_ifName()+":Destethernet="+m->getDestethernet()+":Dest Ifc="+m->getDest_ifName()+":Rspan Vlan="+m->getRspanVlanId()+":Trill SA="+m->getTrillSA()+":Trill DA="+m->getTrillDA()+":Direction="+m->getDirection());


       	status = sendSynchronouslyToWaveClient ("nsm", m);
		trace(TRACE_LEVEL_INFO, string("SpanLocalConfigWorker::ConfigMessageStep -:  Client Status ::") + m->getClientStatus());
		if(pSpanLocalSpanLocalConfigMessage->getMessageType() == SPAN_LOCAL_MSG_CONFIG && 
			pSpanLocalSpanLocalConfigMessage->getOpCode() == SPAN_MSG_SET)
		{
			pSpanLocalSpanLocalConfigMessage->copyBufferFromClient(m);
			trace(TRACE_LEVEL_INFO, string("SpanLocalConfigWorker::ConfigMessageStep -:  Copy buffer from client occurred\n"));
		}
		
 		pSpanLocalSpanLocalConfigMessage->setCompletionStatus(status);

    	if (WAVE_MESSAGE_SUCCESS == status) 
		{
    		NSM_PLUG_DBG_S(string("WAVE_MESSAGE_SUCCESS == status"));
		    if (WAVE_MESSAGE_SUCCESS != pSpanLocalSpanLocalConfigMessage->getCompletionStatus()) 
			{
		    	NSM_PLUG_DBG_ERR_S(string("Error: WAVE_MESSAGE_SUCCESS != CompletionStatus"));
	        	status = pSpanLocalSpanLocalConfigMessage->getCompletionStatus();
	        }
			else 
			{
	       		status = mapBackEndErrorToDcmdError(m->getClientStatus());
	       		trace(TRACE_LEVEL_INFO, string("Client status ") + status + "\n"+  m->getClientStatus());
	       		pSpanLocalSpanLocalConfigMessage->setCompletionStatus(status);

	       		if(SPAN_MSG_NO_SET == m->getOpCode() && m->getDestethernet() == DESTINATION_TYPE_RSPAN
	       				&& localMappedId == trillSA && status == WAVE_MESSAGE_SUCCESS) {
	       			//Need to see if unprovision needs to be handled
	       			NSM_PLUG_INFO_S(string("Need to see if unprovision is triggered for the vlan ")+m->getRspanVlanId());
	       			HANDLE_PROVISION_RESPONSE(m);
	       		}
			}
		}
    	else {
    		trace(TRACE_LEVEL_ERROR, string("SpanLocalConfigWorker::ConfigMessageStep :  WAVE_MESSAGE_SUCCESS != status"));
    	}

        //To avoid memory leak
		if (m)
		{
			delete m;
		}
		return status;
   	}

    void  SpanLocalConfigWorker::SpanLocalSpanLocalConfigMessageHandler( SpanLocalSpanLocalConfigMessage *pSpanLocalSpanLocalConfigMessage)
    {
// Validations and Queries should go here
    	trace(TRACE_LEVEL_DEVEL, string("SpanLocalConfigWorker::SpanLocalSpanLocalConfigMessageHandler : Entering"));

    	if(pSpanLocalSpanLocalConfigMessage->isCapabilityProbeMessage()) {
    		NSM_PLUG_DBG_S(string("Capability Probe Message. Just perform the validations and no config"));
			PrismSynchronousLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::validateConfigMessageStep),
				reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
				reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
			};
	        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pSpanLocalSpanLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	        pPrismSynchronousLinearSequencerContext->execute ();
    	}
    	else {
    		PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
    	  	    //reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::validateConfigMessageStep),
    	  	    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::ConfigMessageStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep>(&SpanLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
            };
    		DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pSpanLocalSpanLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();
    	}
    }

	void SpanLocalConfigWorker::SpanLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p)
	{
	    PrismLinearSequencerContext             *seq_ctx_p = NULL;
	    SpanLocalShowMessage                   *req_msg_p = NULL,
	                                            *resp_msg_p = NULL,
	                                           *sw_resp_p = NULL;
        NSM_PLUG_DBG_TRACE();
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p = dynamic_cast<SpanLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p =  dynamic_cast<SpanLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());
 
		NSM_PLUG_ASSERT_PTR(req_msg_p);
        NSM_PLUG_ASSERT_PTR(resp_msg_p);
		
		vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
		UI32    i = 0;
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus )
        {
			for (i = 0; i < locations.size(); i++)
			{
				ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);
				if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
					sw_resp_p = dynamic_cast<SpanLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
					NSM_PLUG_ASSERT_PTR(sw_resp_p);
					if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
					{
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

	void SpanLocalConfigWorker::sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        SpanLocalShowMessage       *msg_p = NULL;
		SpanLocalShowMessage       *loc_msg_p = NULL;
        WaveSendToClusterContext    *send_ctx_p = NULL;
		bool                        isGwRequest = false;
        vector<UI32>                locations;
        
		msg_p = dynamic_cast<SpanLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        NSM_PLUG_ASSERT_PTR(msg_p);

		loc_msg_p = new SpanLocalShowMessage(msg_p->getSession_id(),msg_p->getMessageType(), msg_p->getCmdCode(), isGwRequest);
		NSM_PLUG_ASSERT_PTR(loc_msg_p);
				
        NSM_PLUG_DBG_S(string("Sending Show Command to Cluster") + string(" CmdCode =") + msg_p->getCmdCode());
	
        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&SpanLocalConfigWorker::SpanLocalShowSendToClusterCallback), seq_ctx_p);
        NSM_PLUG_ASSERT_PTR(send_ctx_p);

		if (!send_ctx_p)
		{
		     NSM_PLUG_DBG_S(string("The send_ctx_p is NULL."));
		     seq_ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
		     return;
	    }

		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
		sendToWaveCluster(send_ctx_p);
		return;
	}
	
	ResourceId SpanLocalConfigWorker::sendShowToSpanDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
    {
        SpanShowMessage *cl_msg_p = NULL;
        ResourceId      status = WAVE_MESSAGE_ERROR;
		SI32            mappedId = 0;
        SpanLocalShowMessage *msg_p = NULL;
        if(NULL==ctx_p->getPPrismMessage())
        {
            return status;
        }
        msg_p = dynamic_cast<SpanLocalShowMessage *> (ctx_p->getPPrismMessage ());
		
		mappedId = DcmToolKit::getLocalMappedId();
		if (mappedId == (-1))
		{
		    mappedId = 0;
		}
		

	    cl_msg_p = new SpanShowMessage(msg_p->getSession_id(), msg_p->getMessageType() , msg_p->getCmdCode(), mappedId);
        status = sendSynchronouslyToWaveClient("nsm", cl_msg_p);
		
		if(NULL==cl_msg_p)
		    return status;

		NSM_PLUG_DBG_S(string("Sending Show Command to Cluster") + string(" Session_Id =")  + cl_msg_p->getSession_id()  + string(" CmdCode =") + cl_msg_p->getCmdCode() + string("MessageType =")  + cl_msg_p->getMessageType()+ string(" mappedId =") + cl_msg_p->getMappedId() );

		if (status == WAVE_MESSAGE_SUCCESS)
		{
		    if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
		    {
		        NSM_PLUG_DBG_S(string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getSession_id() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
		        msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
		     	status = msg_p->getCompletionStatus();
		        delete cl_msg_p;
				return status;
			 }
			if (cl_msg_p->getClientStatus() != 0)
			{
				NSM_PLUG_DBG_S(string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
				msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}
		}

		if (status != WAVE_MESSAGE_SUCCESS)
		{
		     NSM_PLUG_DBG_S(string("Client Message sending failed for Session_Id: ") + cl_msg_p->getSession_id() + string("reason:") +  FrameworkToolKit::localize(status));
			 delete cl_msg_p;
	         return status;
	    }

	    NSM_PLUG_DBG_S(string("Client Message processing succesful for Session_Id: ") + cl_msg_p->getSession_id());

	    msg_p->copyAllBuffers(*cl_msg_p);
	    status = cl_msg_p->getCompletionStatus();
	    delete cl_msg_p;
	    return status;
	}
	
	void SpanLocalConfigWorker::SpanLocalShowMessageHandler (SpanLocalShowMessage *pSpanLocalShowMessage)
	{
	    if (pSpanLocalShowMessage->getIsGwRequest())
	    {
	         PrismLinearSequencerStep sequencerSteps[] =
	         {
	              reinterpret_cast<PrismLinearSequencerStep> (&SpanLocalConfigWorker::sendShowCommandToCluster),
	              reinterpret_cast<PrismLinearSequencerStep> (&SpanLocalConfigWorker::prismLinearSequencerSucceededStep),
	              reinterpret_cast<PrismLinearSequencerStep> (&SpanLocalConfigWorker::prismLinearSequencerFailedStep)
	         };
	
			 PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pSpanLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
			 pPrismLinearSequencerContext->start ();
			 return;
		}
		else
		{
		     PrismSynchronousLinearSequencerStep sequencerSteps[] =
		     {
		         reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SpanLocalConfigWorker::sendShowToSpanDaemon),
		         reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SpanLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
		         reinterpret_cast<PrismSynchronousLinearSequencerStep> (&SpanLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
		     };
             PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pSpanLocalShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
             pPrismSynchronousLinearSequencerContext->execute ();
        }
	}
	
	ResourceId SpanLocalConfigWorker::mapBackEndErrorToDcmdError(int rc)	
	{
		ResourceId errorId = WAVE_MESSAGE_SUCCESS;
		switch (rc)
		{
			case NSM_SPAN_PORT_MIRROR_NOT_DELETE:
		         errorId = WRC_NSM_SPAN_PORT_MIRROR_NOT_DELETE;
				 break;
			case NSM_SPAN_INTERFACE_DOES_NOT_EXIST:
				 errorId = WRC_NSM_SPAN_INTERFACE_DOES_NOT_EXIST;
				 break;
			case NSM_SPAN_SOURCE_DEST_PORT_SAME:
				 errorId = WRC_NSM_SPAN_SOURCE_DEST_PORT_SAME;
				 break;
			case NSM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG;
				 break;
			case NSM_SPAN_INCORRECT_DIRECTION:
				 errorId = WRC_NSM_SPAN_INCORRECT_DIRECTION;
				 break;
			case NSM_SPAN_ARGUMENTS_NOT_MATCH_CONFIGURATION:
				 errorId = WRC_NSM_SPAN_ARGUMENTS_NOT_MATCH_CONFIGURATION;
				 break;
			case NSM_SPAN_MONITOR_SESSION_ALREADY_CONFIGURED:
				 errorId = WRC_NSM_SPAN_MONITOR_SESSION_ALREADY_CONFIGURED;
				 break;
			case NSM_RSPAN_MONITOR_SESSION_SOURCE_ALREADY_CONFIGURED:
				 errorId = WRC_NSM_RSPAN_MONITOR_SESSION_SOURCE_ALREADY_CONFIGURED;
				 break;
			case NSM_SPAN_SOURCE_ALREADY_CONFIGURED_SAME_DIR:
				 errorId = WRC_NSM_SPAN_SOURCE_ALREADY_CONFIGURED_SAME_DIR;
				 break;
			case NSM_SPAN_SOURCE_ALREADY_CONFIGURED_AS_DESTINATION:
				 errorId = WRC_NSM_SPAN_SOURCE_ALREADY_CONFIGURED_AS_DESTINATION;
				 break;
			case NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_SOURCE_PORT:
				 errorId = WRC_NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_SOURCE_PORT;
				 break;
			case NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_DESTINATION_ANOTHER_SESSION:
				 errorId = WRC_NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_DESTINATION_ANOTHER_SESSION;
				 break;
			case NSM_SPAN_HARDWARE_PORT_CONFIGURATION_FAILED:
				 errorId = WRC_NSM_SPAN_HARDWARE_PORT_CONFIGURATION_FAILED;
				 break;
			case NSM_SPAN_DESCRIPTION_LENGTH_MORE_THAN_LIMIT:
				 errorId = WRC_NSM_SPAN_DESCRIPTION_LENGTH_MORE_THAN_LIMIT;
				 break;
			case NSM_SPAN_SESSION_DOESNOT_EXIST:
				 errorId = WRC_NSM_SPAN_SESSION_DOESNOT_EXIST;
				 break;
			case SPAN_MIRRORING_ISL_INTERFACE_COMMAND_ERROR:
				 errorId = WRC_SPAN_MIRRORING_ISL_INTERFACE_COMMAND_ERROR;
				 break;
			case DCM_SPAN_DEST_PORT_IN_L2:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_L2;
				 break;
			case DCM_SPAN_DEST_PORT_IN_L3:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_L3;
				 break;
			case DCM_SPAN_DEST_PORT_IN_QoS:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_QoS;
				 break;
			case DCM_SPAN_DEST_PORT_IN_ACL:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_ACL;
				 break;
			case DCM_SPAN_DEST_PORT_IN_DOT1X:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_DOT1X;
				 break;
			case NSM_SPAN_DEST_PORT_IN_LAG:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_LAG;
				 break;
			case DCM_SPAN_DEST_PORT_IN_LLDP:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_LLDP;
				 break;
			case NSM_SPAN_DEST_PORT_IN_PORTPROFILE:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_PORTPROFILE;
				 break;
			case DCM_SPAN_DEST_PORT_IN_MTU:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_MTU;
				 break; 
			case NSM_SPAN_DEST_PORT_IN_STORMCONTROL:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_STORMCONTROL;
				 break;
			case DCM_SPAN_DEST_PORT_IN_RMON:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_RMON;
				 break;
			case DCM_SPAN_DEST_PORT_IN_SFLOW:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_SFLOW;
				 break;
			case DCM_SPAN_DEST_PORT_IN_PIM:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_PIM;
				 break;
			case DCM_SPAN_DEST_PORT_IN_OSPF:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_OSPF;
				 break;
			case DCM_SPAN_DEST_PORT_IN_IGMP:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_IGMP;
				 break;
			case DCM_SPAN_DEST_PORT_IN_FCOE:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_FCOE;
				 break;
			case DCM_SPAN_DEST_PORT_IN_LACP:
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_LACP;
				 break;

			//TODO: Fix the issue with Capabilities.h inclusion
			 /*
			 * This is a work around to fix the problem while using the Capabilities API.
			 * We have to use the Capabilities API for checking some port mirroring capabilities of the platform.
			 * When I include the Utils/Capabilities.h , the error codes from the backend are somehow getting hidden.
			 * The backend error codes are made available by including the backend file nsm_pmirror.h

			 * Now the backend error codes are converted to dcm error codes With prefix WRC_ using this function
			 * But after including the Capabilities header file, this function is not able to do this conversion properly and it goes to the default case in the function.

			 * To me it looks like, when I include Capabilities.h, somehow the enum defined in nsm_pmirror.h is getting hidden.
			 * Probably this could be due to some improper includes in Capabilities.h, but presently redefining the enum in the SpanMessageDef also, as a temporary fix.
			 */


			case DCM_SPAN_PORT_MIRROR_NOT_DELETE:
		         errorId = WRC_NSM_SPAN_PORT_MIRROR_NOT_DELETE;
				 break;
			case DCM_SPAN_INTERFACE_DOES_NOT_EXIST:
				 errorId = WRC_NSM_SPAN_INTERFACE_DOES_NOT_EXIST;
				 break;
			case DCM_SPAN_SOURCE_DEST_PORT_SAME:
				 errorId = WRC_NSM_SPAN_SOURCE_DEST_PORT_SAME;
				 break;
			case DCM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG: {
				 NSM_PLUG_DBG_S(string("Case: DCM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG"));
				 errorId = WRC_NSM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG;
				 break;
			}
			case DCM_SPAN_INCORRECT_DIRECTION:
				 errorId = WRC_NSM_SPAN_INCORRECT_DIRECTION;
				 break;
			case DCM_SPAN_ARGUMENTS_NOT_MATCH_CONFIGURATION:
				 errorId = WRC_NSM_SPAN_ARGUMENTS_NOT_MATCH_CONFIGURATION;
				 break;
			case DCM_SPAN_MONITOR_SESSION_ALREADY_CONFIGURED:
				 errorId = WRC_NSM_SPAN_MONITOR_SESSION_ALREADY_CONFIGURED;
				 break;
			case DCM_RSPAN_MONITOR_SESSION_SOURCE_ALREADY_CONFIGURED:
				 errorId = WRC_NSM_RSPAN_MONITOR_SESSION_SOURCE_ALREADY_CONFIGURED;
				 break;
			case DCM_SPAN_SOURCE_ALREADY_CONFIGURED_SAME_DIR:
				 errorId = WRC_NSM_SPAN_SOURCE_ALREADY_CONFIGURED_SAME_DIR;
				 break;
			case DCM_SPAN_SOURCE_ALREADY_CONFIGURED_AS_DESTINATION:
				 errorId = WRC_NSM_SPAN_SOURCE_ALREADY_CONFIGURED_AS_DESTINATION;
				 break;
			case DCM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_SOURCE_PORT:
				 errorId = WRC_NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_SOURCE_PORT;
				 break;
			case DCM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_DESTINATION_ANOTHER_SESSION:
				 errorId = WRC_NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_DESTINATION_ANOTHER_SESSION;
				 break;
			case DCM_SPAN_HARDWARE_PORT_CONFIGURATION_FAILED:
				 errorId = WRC_NSM_SPAN_HARDWARE_PORT_CONFIGURATION_FAILED;
				 break;
			case DCM_SPAN_DESCRIPTION_LENGTH_MORE_THAN_LIMIT:
				 errorId = WRC_NSM_SPAN_DESCRIPTION_LENGTH_MORE_THAN_LIMIT;
				 break;
			case DCM_SPAN_SESSION_DOESNOT_EXIST:
				 errorId = WRC_NSM_SPAN_SESSION_DOESNOT_EXIST;
				 break;
			case DCM_SPAN_MIRRORING_ISL_INTERFACE_COMMAND_ERROR:
				 errorId = WRC_SPAN_MIRRORING_ISL_INTERFACE_COMMAND_ERROR;
				 break;
			case DCM_NSM_SLOT_NOT_UP:
				 errorId = WRC_NSM_SLOT_NOT_UP;
				 break;
			default:
				 errorId = WAVE_MESSAGE_SUCCESS;
				 break;
		}
		return errorId;
	}
}

