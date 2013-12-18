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

#include "Span/Local/SpanLocalLocalObjectManager.h"
#include "Span/Local/SpanLocalConfigWorker.h"
#include "Span/Local/SpanLocalTypes.h"
#include "Span/Global/SpanGlobalConfigManagedObject.h"
#include "Span/Global/SpanGlobalSpanGlobalConfigMessage.h"
#include "SpanLocalConfigLocalManagedObject.h"
#include "ClientInterface/Span/SpanClientLocalSpanLocalConfigMessage.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"


namespace DcmNs
{

    SpanLocalLocalObjectManager::SpanLocalLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pSpanLocalConfigWorker = new SpanLocalConfigWorker (this);
        prismAssert (NULL != m_pSpanLocalConfigWorker, __FILE__, __LINE__);
    }

    SpanLocalLocalObjectManager::~SpanLocalLocalObjectManager ()
    {
    }

    SpanLocalLocalObjectManager  *SpanLocalLocalObjectManager::getInstance()
    {
        static SpanLocalLocalObjectManager *pSpanLocalLocalObjectManager = new SpanLocalLocalObjectManager ();

        WaveNs::prismAssert (NULL != pSpanLocalLocalObjectManager, __FILE__, __LINE__);

        return (pSpanLocalLocalObjectManager);
    }

    string  SpanLocalLocalObjectManager::getClassName()
    {
        return ("SpanLocal");
    }

    PrismServiceId  SpanLocalLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *SpanLocalLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SpanLocalLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void SpanLocalLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string pass_name = pWaveAsynchronousContextForPostBootPhase->getPassName();
        SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();

		switch(PassNum)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			trace (TRACE_LEVEL_INFO, string("SpanLocalObjectManager::postboot Entered with pass:") + pWaveAsynchronousContextForPostBootPhase->getPassNum());
       		//ConfigReplaySpanGlobalConfig();
			break;
		default:
			break;
		}
		if (pass_name == "DCM_POSTBOOT_GLOBAL_STAGE1")
		{
            trace (TRACE_LEVEL_INFO,("SpanLocalLocalObjectManager:: postboot for span sessions with just description\n"));
            ConfigReplaySpanGlobalConfig();
		}
		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback();	
    }

   void SpanLocalLocalObjectManager::ConfigReplaySpanGlobalConfig()
    {
		SpanGlobalConfigManagedObject *pSpanGlobalConfigManagedObject = NULL;
       	ResourceId status = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = NULL;
		pResults =querySynchronously(&syncQueryCtxt);
		string sourcestr_twoTupleIfName;
		string deststr_twoTupleIfName;

		trace (TRACE_LEVEL_INFO,"Entered Span config replay\n");
		
		if(pResults == NULL || pResults->size() == 0)
			trace (TRACE_LEVEL_ERROR,"Not possible case\n");

		if (pResults != NULL && pResults->size() != 0)
        {
            UI32 numberOfResults = pResults->size();
			trace (TRACE_LEVEL_INFO,string("pResults for span is not NULL\n") + numberOfResults);

            for (UI32 i=0; i< numberOfResults; i++)
            {
                pSpanGlobalConfigManagedObject = dynamic_cast<SpanGlobalConfigManagedObject *>((*pResults)[i]);
				 
                if (pSpanGlobalConfigManagedObject)
                {
               	trace (TRACE_LEVEL_INFO,string("SpanLocalLocalObjectManager::ConfigReplaySpanGlobalConfig:Session ID:") + pSpanGlobalConfigManagedObject->getSession_id());
               	trace (TRACE_LEVEL_INFO,string("SpanLocalLocalObjectManager::ConfigReplaySpanGlobalConfig:Description:") + pSpanGlobalConfigManagedObject->getDescription());
               	trace (TRACE_LEVEL_INFO,string("SpanLocalLocalObjectManager::ConfigReplaySpanGlobalConfig:Source If Name:") + pSpanGlobalConfigManagedObject->getSource_ifName());
               	   //Only description/session id is configured with no source/destination.
               	   //For VCS (Management Cluster), all sessions should be replayed on all Nodes. For FC, only local sessions will be there in the db
                   if(pSpanGlobalConfigManagedObject->getSource_ifName() != "None" && WAVE_PERSISTENCE_CLUSTER_ENABLED != DcmToolKit::isVcsEnabled() ) {
                	   trace (TRACE_LEVEL_INFO,string("SpanLocalLocalObjectManager::ConfigReplaySpanGlobalConfig:Session ") + pSpanGlobalConfigManagedObject->getSession_id() + string("will be played back by Nsm"));
                   }
                   else {

                	  SpanClientLocalSpanLocalConfigMessage *n = new SpanClientLocalSpanLocalConfigMessage();
                	  SpanClientLocalSpanLocalConfigMessage *m = new SpanClientLocalSpanLocalConfigMessage();
					  m->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
					  m->setOpCode(SPAN_MSG_SET);
					  m->setMessageType(SPAN_LOCAL_MSG_SESSION);

					  trace (TRACE_LEVEL_INFO,string("Session Id:")+m->getSession_id());

					  status = sendSynchronouslyToWaveClient ("nsm", m);
					  if (status == WAVE_MESSAGE_SUCCESS) {
						  trace(TRACE_LEVEL_SUCCESS, string("Span Config successful for the session: ")+m->getSession_id());
						  if(pSpanGlobalConfigManagedObject->getDescription() != "") {
							  n->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
							  n->setDescription(pSpanGlobalConfigManagedObject->getDescription());
							  n->setOpCode(SPAN_MSG_SET);
							  n->setMessageType(SPAN_LOCAL_MSG_DESC);
							  status = sendSynchronouslyToWaveClient ("nsm", n);
							  if (status == WAVE_MESSAGE_SUCCESS) {
								  trace(TRACE_LEVEL_SUCCESS, string("Span Description configured "));
							  }
							  else {
								  trace(TRACE_LEVEL_ERROR, string("Failed to configure the Span Description"));
							  }
						  }
					  }
					  else
					  {
						  trace(TRACE_LEVEL_ERROR, string("Span Config failed"));
					  }
					  delete m;
					  delete n;

					  //To fix the defect TR000454561. In case of span over VCS when intermediate nodes get rebooted,
					  //those nodes will not have the EXM entry to have the span path information. The only option is to trigger
					  //the config replay on those nodes even if the src/destination does not belong to that node.
					  //The config replay on interface will not trigger this info to these nodes because the src/destination does not
					  //belong to this node.
					  SI32 localMappedId = (SI32)(DcmToolKit::getLocalMappedId());
					  SI32 srcMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalConfigManagedObject->getSource_ifName());
					  SI32 dstMappedId = NsmUtils::getMappedIdFromIfName(pSpanGlobalConfigManagedObject->getDest_ifName());
					  UI32 destIfcType = pSpanGlobalConfigManagedObject->getDest_tengiga();
					  //We need to trigger this replay only if both src and dest do not belong to the local node for a span over vcs session.
					  //When src/dest is from the local node, the replay is triggered while interface config replay.
					  NSM_PLUG_INFO_S(string("Check whether the src/dest config replay needs to be trigerred. Local Rbridge Id=")+localMappedId+";Src Rbridge="+srcMappedId+";Dst Rbridge="+dstMappedId);

					  if(localMappedId != srcMappedId && localMappedId != dstMappedId && srcMappedId != dstMappedId && destIfcType != DESTINATION_TYPE_RSPAN) {

						    NSM_PLUG_INFO_S(string("Need to replay the session so that the span path info will be available in the backend"));

					        SpanClientLocalSpanLocalConfigMessage *msg = new SpanClientLocalSpanLocalConfigMessage();
					        msg->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
					        UI32 rspanVlanId = 0;
						    if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalConfigManagedObject->getSource_ifName()))
						    {
							    string str_twoTupleIfName;
							    NsmUtils::getTwoTupleIfName(pSpanGlobalConfigManagedObject->getSource_ifName(), str_twoTupleIfName);
							    msg->setSource_ifName(str_twoTupleIfName);
							    NSM_PLUG_INFO_S(string("Src If Name= ")+str_twoTupleIfName);
						    }
						    else {
							    msg->setSource_ifName(pSpanGlobalConfigManagedObject->getSource_ifName());
						    }

						    if(true  == NsmUtils::isValidThreeTuple(pSpanGlobalConfigManagedObject->getDest_ifName()))
						    {
							    string str_twoTupleIfName;
							    NsmUtils::getTwoTupleIfName(pSpanGlobalConfigManagedObject->getDest_ifName(), str_twoTupleIfName);
							    msg->setDest_ifName(str_twoTupleIfName);
							    NSM_PLUG_INFO_S(string("Dst If Name= ")+str_twoTupleIfName);
						    }
						    else {
							    msg->setDest_ifName(pSpanGlobalConfigManagedObject->getDest_ifName());
						    }
					        msg->setDirection(pSpanGlobalConfigManagedObject->getDirection());
						    msg->setSrcethernet(pSpanGlobalConfigManagedObject->getSource_tengiga());
					        msg->setDestethernet(pSpanGlobalConfigManagedObject->getDest_tengiga());
					        msg->setDescription(pSpanGlobalConfigManagedObject->getDescription());
						    msg->setOpCode( SPAN_MSG_SET );
						    msg->setMessageType( SPAN_LOCAL_MSG_CONFIG );
					   	    msg->setTrillSA(srcMappedId);
					   	    msg->setTrillDA(dstMappedId);
					   	    msg->setRspanVlanId(rspanVlanId);

					   	    NSM_PLUG_INFO_S(string("OP Code=")+msg->getOpCode()+":Msg Type="+msg->getMessageType()+":Src Ifc="+msg->getSource_ifName()+":Destethernet="+msg->getDestethernet()+":Dest Ifc="+msg->getDest_ifName()+":Rspan Vlan="+msg->getRspanVlanId()+":Trill SA="+msg->getTrillSA()+":Trill DA="+msg->getTrillDA()+":Direction="+msg->getDirection());

						    status = sendSynchronouslyToWaveClient ("nsm", msg);
						    if (status == WAVE_MESSAGE_SUCCESS) {
						       NSM_PLUG_INFO_S(string("Span Src/Dest Config successful for the session: ")+msg->getSession_id());
						    }
						    else
						    {
								if (WAVE_MESSAGE_SUCCESS != msg->getCompletionStatus()) {
									NSM_PLUG_DBG_ERR_S(string("Span config - WAVE_MESSAGE_SUCCESS != CompletionStatus; Completion status = ")+msg->getCompletionStatus());
								}
								else
								{
									NSM_PLUG_DBG_ERR_S(string("Span config client Status =")+m->getClientStatus());
								}
						    }
						    delete msg;
					  }

                   }
                }
            }
			if(pResults)
			{
	     //To Cleanup the query results outside the commit block
		   		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
       }
		trace (TRACE_LEVEL_INFO,"Exiting Span config replay\n");
   }

void SpanLocalLocalObjectManager::slotFailover (WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
{
        string func_name = "SpanLocalLocalObjectManager::slotFailover() ";
        int32_t slotId = pWaveAsynchronousContextForSlotFailover->getSlotNumber();
        
        trace (TRACE_LEVEL_INFO, func_name + " Entered with " +" SlotId = " + slotId );
        
        SpanGlobalConfigManagedObject *pSpanGlobalConfigManagedObject = NULL;
	    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(SpanGlobalConfigManagedObject::getClassName());
	    vector<WaveManagedObject *> *pResults = NULL;
	    pResults = querySynchronously(&syncQueryCtxt);
	    string sourcestr_IfName;
	    string deststr_IfName;
		ResourceId status = WAVE_MESSAGE_ERROR;

        if (pResults == NULL || pResults->size() == 0)
		{
            trace (TRACE_LEVEL_INFO, func_name + "No Span MOs found\n");
			goto EXIT;
		}

        trace (TRACE_LEVEL_INFO, func_name + " Found " + pResults->size() + " Span MOs ");

        //startTransaction();
        for (UI32 i = 0; i < pResults->size(); i++)
		{
                 pSpanGlobalConfigManagedObject = dynamic_cast<SpanGlobalConfigManagedObject *>(( *pResults)[i]);
				 sourcestr_IfName = pSpanGlobalConfigManagedObject->getSource_ifName();
				 deststr_IfName = pSpanGlobalConfigManagedObject->getDest_ifName();
				 
				 // if Span MO matching interface and this MO is not deleted
                 if (pSpanGlobalConfigManagedObject && 
				     ((NsmUtils::getSlotId(sourcestr_IfName) == slotId) ||
				      (NsmUtils::getSlotId(deststr_IfName) == slotId ) ) )
                 {
                     SpanClientLocalSpanLocalConfigMessage *m = new SpanClientLocalSpanLocalConfigMessage();
					 SpanGlobalSpanGlobalConfigMessage *n = new SpanGlobalSpanGlobalConfigMessage();

                     SI32 srcMappedId = 0;
                     SI32 dstMappedId = 0;

                     trace (TRACE_LEVEL_INFO, func_name + "Span MO " +
					    pSpanGlobalConfigManagedObject->getSession_id() +
					    " found with slot: " + slotId + " source: " + 
					    sourcestr_IfName + " dest: " + deststr_IfName);

					 // send msg to back-end
					 m->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
					 m->setOpCode(SPAN_MSG_NO_SET);
					 m->setMessageType(SPAN_LOCAL_MSG_SESSION);

					 if (NsmUtils::isValidThreeTuple(m->getSource_ifName())) {
					   srcMappedId = NsmUtils::getMappedIdFromIfName(m->getSource_ifName());
					 }
					 if (NsmUtils::isValidThreeTuple(m->getDest_ifName())) {
					   dstMappedId = NsmUtils::getMappedIdFromIfName(m->getDest_ifName());
					 }
					 trace(TRACE_LEVEL_SUCCESS, string("Trill SA ")+srcMappedId);
					 trace(TRACE_LEVEL_SUCCESS, string("Trill DA ")+dstMappedId);
					 m->setTrillSA(srcMappedId);
					 m->setTrillDA(dstMappedId);
					 m->setRspanVlanId(pSpanGlobalConfigManagedObject->getDestVlanId());

					 status = sendSynchronouslyToWaveClient("nsm", m);
					 if (status == WAVE_MESSAGE_SUCCESS)
                          trace (TRACE_LEVEL_SUCCESS, func_name + "delete backend Span Session ID " + 
                             pSpanGlobalConfigManagedObject->getSession_id());
					 else
					 	  trace (TRACE_LEVEL_ERROR, func_name + " Error: delete backend Span Session ID " + 
		                     pSpanGlobalConfigManagedObject->getSession_id());

					 // send to msg to global plugin to delete Span global MO
                     //delete pSpanGlobalConfigManagedObject;
                     n->setOpCode(SPAN_NO_LINECARD);
                     n->setSession_id(pSpanGlobalConfigManagedObject->getSession_id());
					 n->setMessageType(SPAN_LOCAL_MSG_CONFIG);
                     status = sendSynchronously(n); 
                     trace (TRACE_LEVEL_INFO, func_name + "delete Span global MO: status from Global Plugin is " +
                        status);

					 delete m;
					 delete n;
				 }
		}
		//commitTransaction();
        		
		if (pResults) {
			pResults->clear();
			delete pResults;
		}
		
EXIT:
        pWaveAsynchronousContextForSlotFailover->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForSlotFailover->callback();

        trace (TRACE_LEVEL_INFO, func_name + " DONE! " + " SlotId = " + slotId);
}


}
