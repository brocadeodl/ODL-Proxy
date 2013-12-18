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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIds.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/DceClusterUtils.h"


#include "SystemMonitor/Global/GlobalRelayIpWorker.h"
#include "SystemMonitor/Global/RelayIpConfigMessage.h"
#include "SystemMonitor/Global/RelayIpSetMessage.h"
#include "SystemMonitor/Global/RelayIpDelMessage.h"
#include "SystemMonitor/Local/RelayIpLocalConfigMessage.h"
#include "SystemMonitor/Local/RelayIpLocalSetMessage.h"
#include "SystemMonitor/Local/RelayIpLocalDelMessage.h"
#include "SystemMonitor/Global/RelayIpManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

#include "AAA/Global/AAASequencerContexts.h"
namespace DcmNs
{

    GlobalRelayIpWorker::GlobalRelayIpWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        RelayIpManagedObject    RelayIpManagedObject    (pSystemMonitorGlobalObjectManager);
        RelayIpManagedObject.setupOrm ();
        addManagedClass (RelayIpManagedObject::getClassName (), this);
        addOperationMap (RELAYIPCONFIG, reinterpret_cast<PrismMessageHandler> (&GlobalRelayIpWorker::RelayIpConfigMessageHandler));
        addOperationMap (RELAYIPSET, reinterpret_cast<PrismMessageHandler> (&GlobalRelayIpWorker::RelayIpSetMessageHandler));
        addOperationMap (RELAYIPDEL, reinterpret_cast<PrismMessageHandler> (&GlobalRelayIpWorker::RelayIpDelMessageHandler));
    }

    GlobalRelayIpWorker::~GlobalRelayIpWorker ()
    {
    }

    PrismMessage  *GlobalRelayIpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case RELAYIPCONFIG :
                pPrismMessage = new RelayIpConfigMessage ();
                break;
			case RELAYIPSET :
                pPrismMessage = new RelayIpSetMessage ();
                break;
            case RELAYIPDEL :
                pPrismMessage = new RelayIpDelMessage ();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalRelayIpWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((RelayIpManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RelayIpManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalRelayIpWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
	void  GlobalRelayIpWorker::RelayIpDelMessageHandler(RelayIpDelMessage *pRelayIpDelMessage)
    {
        trace (TRACE_LEVEL_INFO, "GlobalRelayIpWorker::RelayIpDelMessageHandler::Del RelayIp Msg Handler:Entering ");

		if (pRelayIpDelMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "GlobalRelayIpWorker::RelayIpDelMessageHandler:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;
		HostUC	hostip	  = pRelayIpDelMessage->getHostip();
        string domainStr      = pRelayIpDelMessage->getDomainname();
		RelayIpLocalDelMessage *pMessage =
					new RelayIpLocalDelMessage (domainStr, hostip);
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalRelayIpWorker::delRelaySTCCB), pRelayIpDelMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}
        sendToWaveCluster (pWaveSendToClusterContext);
    }
	void GlobalRelayIpWorker::delRelaySTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
    AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
	vector< vector <Attribute *> > ErrArguments;
	string errString;
        RelayIpDelMessage *pRelayIpDelMessage = 
                                (RelayIpDelMessage *)pWaveSendToClusterContext->getPCallerContext();
//	 RelayIpLocalDelMessage *sw_resp_p = NULL;
        bool       allFailed   = false;
        ResourceId status      = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus     = WAVE_MESSAGE_SUCCESS; 
		// HostUC	hostip	   = pRelayIpLocalDelMessage->getHostip();
        string errorMessage;
/*	UI32 num_of_locations = 0;
	UI32 i = 0, j = 0;
	vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
	num_of_locations = locations.size();
	trace (TRACE_LEVEL_ERROR, string("No of locations") + num_of_locations);
	for (i = 0; i < num_of_locations; i++)
	{
		trace (TRACE_LEVEL_ERROR, string("Del relay Entering here:") + num_of_locations + locations[i]);
		sw_resp_p =  dynamic_cast<RelayIpLocalDelMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));
		status = sw_resp_p->getCompletionStatus();
		 trace (TRACE_LEVEL_ERROR, string("Del relayip getCompletionStatus: ") + status + (" for location: ") + locations[i]);
		 if (status != WAVE_MESSAGE_SUCCESS) {
		 	j++;
			trace(TRACE_LEVEL_INFO, string("Del relayip Response processing: The location: ") + locations[i] + string("returned error: ") + FrameworkToolKit::localize(status));
		}	
	}
	if ( j == num_of_locations ) {
		allFailed = true;
	}
*/

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Delting relayip failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Delting relayip failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "GlobalRelayIpWorker::DelRelaySTCCB", errorMessage);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        { 
            RelayIpManagedObject *pRelayIpManagedObject = NULL;

            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(RelayIpManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL != pResults) && (pResults->size() != 0))
            {
				pRelayIpManagedObject = dynamic_cast<RelayIpManagedObject *> ((*pResults)[0]);
            	startTransaction ();
				delete pRelayIpManagedObject;
				fstatus = commitTransaction ();
	            if (FRAMEWORK_SUCCESS == fstatus)
    	        {
        	        trace (TRACE_LEVEL_INFO, 
            	        "GlobalRelayIpWorker::DelRelaySTCCB: Successful Cluster-Wide commit");
                	fstatus = WAVE_MESSAGE_SUCCESS;
            	}
	            else
    	        {
        	        trace (TRACE_LEVEL_ERROR,
            	        "GlobalRelayIpWorker::delRelaySTCCB:Commiting failed.Status:"
                	    + FrameworkToolKit::localize (fstatus));
            	}
	            delete synchronousQueryContext;
    	        delete pResults;
            } 
		}
        // clean up allocated messages
        RelayIpLocalDelMessage *pLocalMsgForPhase1 =
				(RelayIpLocalDelMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pRelayIpDelMessage->setCompletionStatus (status);
        pRelayIpDelMessage->setMessageString (errorMessage);
        reply (pRelayIpDelMessage);
    }
    void  GlobalRelayIpWorker::RelayIpSetMessageHandler(RelayIpSetMessage *pRelayIpSetMessage)
    {
        trace (TRACE_LEVEL_INFO, "GlobalRelayIpWorker::RelayIpSetMessageHandler::Set RelayIp Msg Handler:Entering ");

		if (pRelayIpSetMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "GlobalRelayIpWorker::RelayIpSetMessageHandler:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;
		HostUC	hostip	  = pRelayIpSetMessage->getHostip();
        string domainStr      = pRelayIpSetMessage->getDomainname();
		RelayIpLocalSetMessage *pMessage =
					new RelayIpLocalSetMessage (domainStr, hostip);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalRelayIpWorker::setRelaySTCCB), pRelayIpSetMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}	
        sendToWaveCluster (pWaveSendToClusterContext);
    }
	void GlobalRelayIpWorker::setRelaySTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        RelayIpSetMessage *pRelayIpSetMessage = 
                                (RelayIpSetMessage *)pWaveSendToClusterContext->getPCallerContext();
	vector< vector <Attribute *> > ErrArguments;
	string errString;
//	RelayIpLocalSetMessage *sw_resp_p = NULL;
        bool       allFailed   = false;
        ResourceId status      = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus     = WAVE_MESSAGE_SUCCESS; 
		string     domainStr   = pRelayIpSetMessage->getDomainname();
		HostUC	hostip	   = pRelayIpSetMessage->getHostip();
        string errorMessage;
/*	UI32 num_of_locations = 0;
	UI32 i = 0, j = 0;
	vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
	num_of_locations = locations.size();
	trace (TRACE_LEVEL_ERROR, string("No of locations") + num_of_locations);
	for (i = 0; i < num_of_locations; i++)
	{
		trace (TRACE_LEVEL_ERROR, string("Set relay Entering here:") + num_of_locations + locations[i]);
		sw_resp_p =  dynamic_cast<RelayIpLocalSetMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));
		status = sw_resp_p->getCompletionStatus();
		trace (TRACE_LEVEL_ERROR, string("Set relayip getCompletionStatus: ") + status + (" for location: ") + locations[i]);
		if (status != WAVE_MESSAGE_SUCCESS) {
			j++;
			trace(TRACE_LEVEL_INFO, string("Set relayip Response processing: The location: ") + locations[i] + string("returned error: ")+ FrameworkToolKit::localize(status));
		}
	}
	if ( j == num_of_locations ) {
		allFailed = true;
	}
*/
	
        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting relayip failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Setting relayip failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "GlobalRelayIpWorker::setRelaySTCCB", errorMessage);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }
	

        if (!allFailed)
        { 
            RelayIpManagedObject *pRelayIpManagedObject = NULL;

            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(RelayIpManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "GlobalRelayIpWorker::setRelaySTCCB No such managed object" +domainStr);
            	startTransaction ();
				pRelayIpManagedObject = dynamic_cast<RelayIpManagedObject *>
					(createManagedObjectInstance(RelayIpManagedObject::getClassName()));
				pRelayIpManagedObject->setDomainname (domainStr);
				pRelayIpManagedObject->setHostip (hostip);
				fstatus = commitTransaction ();
	            if (FRAMEWORK_SUCCESS == fstatus)
    	        {
        	        trace (TRACE_LEVEL_INFO, 
            	        "GlobalRelayIpWorker::setRelaySTCCB: Successful Cluster-Wide commit");
                	fstatus = WAVE_MESSAGE_SUCCESS;
            	}
	            else
    	        {
        	        trace (TRACE_LEVEL_ERROR,
            	        "GlobalRelayIpWorker::setRelaySTCCB:Commiting failed.Status:"
                	    + FrameworkToolKit::localize (fstatus));
            	}
	            delete synchronousQueryContext;
    	        delete pResults;
				delete pRelayIpManagedObject;
            } else {
				pRelayIpManagedObject = dynamic_cast<RelayIpManagedObject *> ((*pResults)[0]);
				delete pResults;
				delete synchronousQueryContext;
				startTransaction ();
//				if (domainStr.length() != 0)
				pRelayIpManagedObject->setDomainname(domainStr);
				//pRelayIpManagedObject->setHostip (hostip);
				updateWaveManagedObject (pRelayIpManagedObject);
				fstatus = commitTransaction ();
				if (FRAMEWORK_SUCCESS == fstatus)
				{             
					trace (TRACE_LEVEL_INFO, 
							"GlobalRelayIpWorker::setRelaySTCCB: Successful Cluster-Wide commit");
					fstatus = WAVE_MESSAGE_SUCCESS;
				}
				else
				{             
					trace (TRACE_LEVEL_FATAL, 
							"GlobalRelayIpWorker::setRelaySTCCB:Commiting Seturation failed.Status : " 
							+ FrameworkToolKit::localize (fstatus));
				}
				delete pRelayIpManagedObject;
			}
		}
        // clean up allocated messages
        RelayIpLocalSetMessage *pLocalMsgForPhase1 =
				(RelayIpLocalSetMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pRelayIpSetMessage->setCompletionStatus (status);
        pRelayIpSetMessage->setMessageString (errorMessage);
        reply (pRelayIpSetMessage);
    }

    void  GlobalRelayIpWorker::RelayIpConfigMessageHandler(RelayIpConfigMessage *pRelayIpConfigMessage)
    {
        trace (TRACE_LEVEL_INFO, "GlobalRelayIpWorker::RelayIpConfigMessageHandler::Congig RelayIp Msg Handler:Entering ");

		if (pRelayIpConfigMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "GlobalRelayIpWorker::RelayIpConfigMessageHandler:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;
		HostUC	hostip	  = pRelayIpConfigMessage->getHostip();
        string domainStr      = pRelayIpConfigMessage->getDomainname();
		RelayIpLocalConfigMessage *pMessage =
					new RelayIpLocalConfigMessage (domainStr, hostip);
	FrameworkToolKit::getFullyConnectedLocations(locations);
	locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalRelayIpWorker::addRelaySTCCB), pRelayIpConfigMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
	if(!locations.empty()) {
		pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
	}	
        sendToWaveCluster (pWaveSendToClusterContext);
    }
	void GlobalRelayIpWorker::addRelaySTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
 	 vector< vector <Attribute *> > ErrArguments;
	 string errString;
        RelayIpConfigMessage *pRelayIpConfigMessage = 
                                (RelayIpConfigMessage *)pWaveSendToClusterContext->getPCallerContext();
//	RelayIpLocalConfigMessage *sw_resp_p = NULL;
        bool       allFailed   = false;
        ResourceId status      = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus     = WAVE_MESSAGE_SUCCESS; 
		string     domainStr   = pRelayIpConfigMessage->getDomainname();
		HostUC	   hostip	   = pRelayIpConfigMessage->getHostip();
        string errorMessage;
/*	UI32 num_of_locations = 0;
	UI32 i = 0, j = 0;
	vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
	num_of_locations = locations.size();
	trace (TRACE_LEVEL_ERROR, string("No of locations") + num_of_locations);
	for (i = 0; i < num_of_locations; i++)
	{
		 trace (TRACE_LEVEL_ERROR, string("Add relay Entering here:") + num_of_locations + locations[i]);
		sw_resp_p =  dynamic_cast<RelayIpLocalConfigMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));
		
	//	status = pWaveSendToClusterContext->getCompletionStatusForPhase1(locations[i]);
		status = sw_resp_p->getCompletionStatus();
		trace (TRACE_LEVEL_ERROR, string("Add relayip getCompletionStatus: ") + status + (" for location: ") + locations[i]);
		if (status != WAVE_MESSAGE_SUCCESS) {
			j++;
			trace(TRACE_LEVEL_INFO, string("Add relayip Response processing: The location: ") + locations[i] + string("returned error: ") + FrameworkToolKit::localize(status));
		}	
	}
	if ( j == num_of_locations ) {
		allFailed = true;
	}
*/
	
        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding relayip failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Adding relayip failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "GlobalRelayIpWorker::addRelaySTCCB", errorMessage);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }
	

        if (!allFailed)
        { 
            RelayIpManagedObject *pRelayIpManagedObject = NULL;

            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(RelayIpManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "GlobalRelayIpWorker::addRelaySTCCB No such managed object" +domainStr);
            	startTransaction ();
				pRelayIpManagedObject = dynamic_cast<RelayIpManagedObject *>
					(createManagedObjectInstance(RelayIpManagedObject::getClassName()));
				pRelayIpManagedObject->setHostip (hostip);
				if (domainStr.length() != 0)
					pRelayIpManagedObject->setDomainname (domainStr);
				fstatus = commitTransaction ();
	            if (FRAMEWORK_SUCCESS == fstatus)
    	        {
        	        trace (TRACE_LEVEL_INFO, 
            	        "GlobalRelayIpWorker::addRelaySTCCB: Successful Cluster-Wide commit");
                	fstatus = WAVE_MESSAGE_SUCCESS;
            	}
	            else
    	        {
        	        trace (TRACE_LEVEL_ERROR,
            	        "GlobalRelayIpWorker::addRelaySTCCB:Commiting failed.Status:"
                	    + FrameworkToolKit::localize (fstatus));
            	}
	            delete synchronousQueryContext;
    	        delete pResults;
				delete pRelayIpManagedObject;
            } else {
				pRelayIpManagedObject = dynamic_cast<RelayIpManagedObject *> ((*pResults)[0]);
				delete pResults;
				delete synchronousQueryContext;
				startTransaction ();
				if (domainStr.length() != 0)
					pRelayIpManagedObject->setDomainname(domainStr);
				pRelayIpManagedObject->setHostip (hostip);
				updateWaveManagedObject (pRelayIpManagedObject);
				fstatus = commitTransaction ();
				if (FRAMEWORK_SUCCESS == fstatus)
				{             
					trace (TRACE_LEVEL_INFO, 
							"GlobalRelayIpWorker::addRelaySTCCB: Successful Cluster-Wide commit");
					fstatus = WAVE_MESSAGE_SUCCESS;
				}
				else
				{             
					trace (TRACE_LEVEL_FATAL, 
							"GlobalRelayIpWorker::addRelaySTCCB:Commiting configuration failed.Status : " 
							+ FrameworkToolKit::localize (fstatus));
				}
				delete pRelayIpManagedObject;
			}
		}
        // clean up allocated messages
        RelayIpLocalConfigMessage *pLocalMsgForPhase1 =
				(RelayIpLocalConfigMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pRelayIpConfigMessage->setCompletionStatus (status);
        pRelayIpConfigMessage->setMessageString (errorMessage);
        reply (pRelayIpConfigMessage);
    }
}
