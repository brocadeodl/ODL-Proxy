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
 *   Author : Brunda                                                       *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIds.h"
#include "DcmCore/DcmToolKit.h"


#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpContextMappingWorker.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigSetContextMappingMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetContextMappingIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveContextMappingIntMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigRemoveContextMappingMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigAddContextMappingMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddContextMappingIntMessage.h"
#include "SnmpServer/Global/ContextMappingManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "snmp/snmplib.h"

#include "AAA/Global/AAASequencerContexts.h"
namespace DcmNs
{

    SnmpContextMappingWorker::SnmpContextMappingWorker ( SnmpServerObjectManager *pSnmpServerObjectManager)
        : WaveWorker  (pSnmpServerObjectManager)
    {
        ContextMappingManagedObject    ContextMappingManagedObject    (pSnmpServerObjectManager);
        ContextMappingManagedObject.setupOrm ();
        addManagedClass (ContextMappingManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERSNMPCONFIGSETCONTEXTMAPPING, reinterpret_cast<PrismMessageHandler> (&SnmpContextMappingWorker::SnmpServerSnmpConfigSetContextMappingMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGREMOVECONTEXTMAPPING, reinterpret_cast<PrismMessageHandler> (&SnmpContextMappingWorker::SnmpServerSnmpConfigRemoveContextMappingMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGADDCONTEXTMAPPING, reinterpret_cast<PrismMessageHandler> (&SnmpContextMappingWorker::SnmpServerSnmpConfigAddContextMappingMessageHandler));
    }

    SnmpContextMappingWorker::~SnmpContextMappingWorker ()
    {
    }

    PrismMessage  *SnmpContextMappingWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERSNMPCONFIGSETCONTEXTMAPPING :
                pPrismMessage = new SnmpServerSnmpConfigSetContextMappingMessage ();
                break;
            case SNMPSERVERSNMPCONFIGREMOVECONTEXTMAPPING :
                pPrismMessage = new SnmpServerSnmpConfigRemoveContextMappingMessage ();
                break;
            case SNMPSERVERSNMPCONFIGADDCONTEXTMAPPING :
                pPrismMessage = new SnmpServerSnmpConfigAddContextMappingMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SnmpContextMappingWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((ContextMappingManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ContextMappingManagedObject(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SnmpContextMappingWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	/*
	** This function is the global message handler to add new ContextMapping string.
	** Called from: ContextMapping WyserEaConfiguration Worker.
	** Functions:	Sends messages to local services and copies the new ContextMapping data to DB.
	** Inputs:		ContextName(Required), vrfName
	** Errors: 		If message sending to local services fail or ContextMappingit to DB fails.
	** On success:	Return WAVE_MESSAGE_SUCCESS.
	*/

    void  SnmpContextMappingWorker::SnmpServerSnmpConfigAddContextMappingMessageHandler( SnmpServerSnmpConfigAddContextMappingMessage *pSnmpServerSnmpConfigAddContextMappingMessage)
    {
		int resContextSize = 0, i = 0, isLastCmd = 0;

		trace (TRACE_LEVEL_DEBUG, "Entering SNMP ContextMapping Add Message handler");
		if (pSnmpServerSnmpConfigAddContextMappingMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Add ContextMapping Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;
		
        string contextName   	= pSnmpServerSnmpConfigAddContextMappingMessage->getContextName();
        string vrfName       	= pSnmpServerSnmpConfigAddContextMappingMessage->getVrfName();

		if ((contextName.length() < 2) || (contextName.length() > AGT_MAX_CONTEXT_SIZE) ) {
            pSnmpServerSnmpConfigAddContextMappingMessage->setCompletionStatus(WRC_SNMP_INVALID_CONTEXT_LENGTH);
            reply (pSnmpServerSnmpConfigAddContextMappingMessage);
            return;
        }
	
		if (vrfName.length() > AGT_MAX_CONTEXT_SIZE) {
            pSnmpServerSnmpConfigAddContextMappingMessage->setCompletionStatus(WRC_SNMP_INVALID_VRFNAME_LENGTH);
            reply (pSnmpServerSnmpConfigAddContextMappingMessage);
            return;
        }
       
		/* In case of last replay CLI configuration, check if the userName 
	     * is already present in the DB. If so, then return success without updating the DB.
    	 */ 
		
		if (pSnmpServerSnmpConfigAddContextMappingMessage->getIsALastConfigReplay() == true) {
            trace (TRACE_LEVEL_INFO,
                "Add Context Global::getIsALastConfigReplay");
	        WaveManagedObjectSynchronousQueryContext *syncQueryContext = new
    	        WaveManagedObjectSynchronousQueryContext(
        	    ContextMappingManagedObject::getClassName());
	        vector<WaveManagedObject *> *pContexts = querySynchronously (syncQueryContext);

			if ((NULL == pContexts) || (pContexts->size() == 0)) {
    	        trace (TRACE_LEVEL_INFO,
        	        "Add Context Global::No Context MO"); 
	        } else {
    	        resContextSize = pContexts->size();
        	    trace (TRACE_LEVEL_INFO,
                string (" Add Context Global::resContextSize : ") +resContextSize);
				for (i = 0; i < resContextSize; i ++) {
		        	ContextMappingManagedObject *pContextManagedObject = dynamic_cast<ContextMappingManagedObject *> ((*pContexts)[i]);
        		    if (pContextManagedObject == NULL) {
                    	trace (TRACE_LEVEL_ERROR,
	                        "Add Context Global:: pContextManagedObject dynamic_cast failed");
    		         }
		            if (contextName.compare(pContextManagedObject->getContextName()) == 0) {
						isLastCmd = 1;
            		}
		        delete pContextManagedObject;
            	}
        	}
        delete pContexts;
        delete syncQueryContext;

			if (isLastCmd == 1) {
        	    trace (TRACE_LEVEL_INFO,
	                string (" Add Context Global::Context Mapping already exists: ") +contextName);
    	        pSnmpServerSnmpConfigAddContextMappingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
				reply (pSnmpServerSnmpConfigAddContextMappingMessage);
            	return;
			}	
		} // End of Last replay

        SnmpServerIntSnmpConfigAddContextMappingIntMessage *pMessage =
					new SnmpServerIntSnmpConfigAddContextMappingIntMessage (contextName, vrfName);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&SnmpContextMappingWorker::addContextMappingSTCCB), pSnmpServerSnmpConfigAddContextMappingMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
 
    }

    void SnmpContextMappingWorker::addContextMappingSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        
        SnmpServerSnmpConfigAddContextMappingMessage *pSnmpServerSnmpConfigAddContextMappingMessage = 
                                (SnmpServerSnmpConfigAddContextMappingMessage *)pWaveSendToClusterContext->getPCallerContext();


        string errorMessage;
        bool       allFailed  = false;
        ResourceId status     = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus    = WAVE_MESSAGE_SUCCESS;

        string contextName   	= pSnmpServerSnmpConfigAddContextMappingMessage->getContextName();
        string vrfName       	= pSnmpServerSnmpConfigAddContextMappingMessage->getVrfName();
        ContextMappingManagedObject *pContextMappingManagedObject = NULL;

		FILE *fp = NULL;
		// Added for testing purpose to simulate last replay usecase
        if (pSnmpServerSnmpConfigAddContextMappingMessage->getIsALastConfigReplay() == false 
				&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered Last Replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigAddContextMappingMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigAddContextMappingMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding snmp ContextMapping operation failed on one or more of the connected nodes.");
            else
            {
                trace (TRACE_LEVEL_ERROR, "FAILURE - Adding snmp ContextMapping operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "SnmpContextMappingWorker::addContextMappingSTCCB", errorMessage);
#if 0
                if (errorMessage.length() != 0)
                    status = CLUSTER_ERROR_1001;
#endif
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

		/*
		* By now messages to local handler have succeeded.
		* Prepare to commit to db.
		* Use the input  (default ro) to commit to db.
		*/
        if (!allFailed)
        {
            startTransaction ();
            pContextMappingManagedObject = dynamic_cast<ContextMappingManagedObject *>(createManagedObjectInstance(ContextMappingManagedObject::getClassName()));

            pContextMappingManagedObject->setContextName(contextName);
            pContextMappingManagedObject->setVrfName(vrfName);

            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_DEBUG, 
                    "SNMP GlobalAdd ContextMapping MessageHandler: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                        "SNMP GlobalAdd ContextMapping MessageHandler:Commiting failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
            delete pContextMappingManagedObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigAddContextMappingIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigAddContextMappingIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigAddContextMappingMessage->setCompletionStatus (status);
        pSnmpServerSnmpConfigAddContextMappingMessage->setMessageString (errorMessage);
        reply (pSnmpServerSnmpConfigAddContextMappingMessage);
    }

	/*
	** This function is the global message handler to delete ContextMapping string.
	** Called from: ContextMapping WyserEaConfiguration Worker.
	** Inputs:		ContextName(Required) and  vrfName.
	** Functions:	Sends messages to local services and removes the context from DB.
	** Errors: 		If message sending to local services fail or commit to DB fails.
	** On success:	Return WAVE_MESSAGE_SUCCESS.
	*/

    void  SnmpContextMappingWorker::SnmpServerSnmpConfigRemoveContextMappingMessageHandler( SnmpServerSnmpConfigRemoveContextMappingMessage *pSnmpServerSnmpConfigRemoveContextMappingMessage)
    {
		int resContextSize = 0, isExist = 0, i = 0;
		trace (TRACE_LEVEL_DEBUG, "Entering SNMP ContextMapping remove handler");

		if (pSnmpServerSnmpConfigRemoveContextMappingMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Remove ContextMappingunity Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;

		string contextName   = pSnmpServerSnmpConfigRemoveContextMappingMessage->getContextName();
        string delFieldName  = pSnmpServerSnmpConfigRemoveContextMappingMessage->getDelFieldName();

		 /* In case of last replay CLI configuration, check if the Context name
    	  * is already deleted in the DB. If so, then return success without updating the DB.
	     */     
        if (pSnmpServerSnmpConfigRemoveContextMappingMessage->getIsALastConfigReplay() == true) {
            trace (TRACE_LEVEL_INFO,
                "Remove Context Global:: getIsALastConfigReplay");
		    WaveManagedObjectSynchronousQueryContext *synchronousQueryContextMapping = new
        	    WaveManagedObjectSynchronousQueryContext(
            		ContextMappingManagedObject::getClassName());
		    vector<WaveManagedObject *> *pContexts = querySynchronously (synchronousQueryContextMapping);
        
        	if ((NULL == pContexts) || (pContexts->size() == 0)) {
            	trace (TRACE_LEVEL_INFO,
    	            "Remove Context Global:: No Context MO Exists");        
			} else {
        	    resContextSize = pContexts->size();
            	trace (TRACE_LEVEL_INFO,
                string ("Remove Context Global:: resContextSize : ") +resContextSize);
		        for (i = 0; i < resContextSize; i ++) {
        		   ContextMappingManagedObject *pContextManagedObject = dynamic_cast<ContextMappingManagedObject *> ((*pContexts)[i]);
		           if (pContextManagedObject == NULL) {
            	        trace (TRACE_LEVEL_ERROR,
        	                "Remove Context Global: pContextManagedObject dynamic_cast failed");
             		}	
		            if (contextName.compare(pContextManagedObject->getContextName()) == 0) {  
                		isExist = 1;
		            }
       			    delete pContextManagedObject;
            	}
        	}
	        delete pContexts;
    	    delete synchronousQueryContextMapping;

        if (isExist == 0) {
            trace (TRACE_LEVEL_INFO,
                    string ("Remove Context Global:: ContextName already exists: ") +contextName);
            pSnmpServerSnmpConfigRemoveContextMappingMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigRemoveContextMappingMessage);
            return;
        }
    } // End of Last replay

		
		SnmpServerIntSnmpConfigRemoveContextMappingIntMessage *pMessage =
					new SnmpServerIntSnmpConfigRemoveContextMappingIntMessage ();

         pMessage->setContextName(contextName);
         if (delFieldName.length()!=0)
            pMessage->setDelFieldName(delFieldName);    

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&SnmpContextMappingWorker::delContextMappingSTCCB), pSnmpServerSnmpConfigRemoveContextMappingMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void SnmpContextMappingWorker::delContextMappingSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   

		trace (TRACE_LEVEL_DEBUG, "Entering SNMP ContextMappingGlobal:delContextMappingSTCCB"); 
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigRemoveContextMappingMessage *pSnmpServerSnmpConfigRemoveContextMappingMessage = 
                                (SnmpServerSnmpConfigRemoveContextMappingMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool allFailed      = false;
        ResourceId status   = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus  = WAVE_MESSAGE_SUCCESS; 
		string contextName  = pSnmpServerSnmpConfigRemoveContextMappingMessage->getContextName();
        string delFieldName = pSnmpServerSnmpConfigRemoveContextMappingMessage->getDelFieldName();
        string errorMessage;
        ContextMappingManagedObject *pContextMappingManagedObject = NULL;

		FILE *fp = NULL;
		// Added for testing purpose to simulate last replay usecase
        if (pSnmpServerSnmpConfigRemoveContextMappingMessage->getIsALastConfigReplay() == false 
					&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered Last Replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigRemoveContextMappingMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigRemoveContextMappingMessage);
            return;
        }

        trace(TRACE_LEVEL_DEBUG, "Entering SNMP ContextMapping delTCCB" + contextName);
        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting snmp ContextMapping operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting snmp ContextMapping operation failed on all nodes");
            }
            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delContextMappingSTCCB", errorMessage);
#if 0
                if (errorMessage.length() != 0)
                    status = CLUSTER_ERROR_1001;
#endif
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
            WaveManagedObjectSynchronousQueryContext *syncQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(ContextMappingManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (contextName, "contextName"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) 
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +contextName);
                pSnmpServerSnmpConfigRemoveContextMappingMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_CONTEXT_NAME_STR_ERROR);
				// clean up allocated messages
				SnmpServerIntSnmpConfigRemoveContextMappingIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigRemoveContextMappingIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigRemoveContextMappingMessage);
                delete syncQueryContext;
                return;
            }

            pContextMappingManagedObject = dynamic_cast<ContextMappingManagedObject *> ((*pResultsRollback)[0]);
            delete pResultsRollback;
            delete syncQueryContext;

            /*
            * By now messages send to local handlers have succeeded.
            * Prepare to commit to DB.
            * Query the string inputted and remove that MO.
            */
            startTransaction ();
			if(delFieldName.length()==0)
            	delete pContextMappingManagedObject;
			else { 
				if(delFieldName.compare("vrfName")==0)
                               pContextMappingManagedObject->setVrfName("");
                    updateWaveManagedObject (pContextMappingManagedObject);
			}
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_DEBUG,
                        "SNMP ContextMappingDelete MessageHandler: Successful Cluster-Wide commit ContextMappingConfig");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, 
                        "SNMP ContextMapping Delete MessageHandler:Commiting ContextMapping config failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigRemoveContextMappingIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigRemoveContextMappingIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigRemoveContextMappingMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigRemoveContextMappingMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigRemoveContextMappingMessage);
    }

	/*
	** This function is the global message handler to change access of a ContextMapping string.
	** Called from: ContextMapping WyserEaConfiguration Worker.
	** Inputs:		ContextName(Required), vrfName is optional.
	** Functions:	Sends messages to local services and changes access of community in DB.
	** Errors: 		If message sending to local services fail or commit to DB fails.
	** On success:	Return WAVE_MESSAGE_SUCCESS.
	*/


    void  SnmpContextMappingWorker::SnmpServerSnmpConfigSetContextMappingMessageHandler( SnmpServerSnmpConfigSetContextMappingMessage *pSnmpServerSnmpConfigSetContextMappingMessage)
	{
		trace (TRACE_LEVEL_DEBUG, "Entering SNMP ContextMapping Set message handler");

		if (pSnmpServerSnmpConfigSetContextMappingMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Set ContextMapping global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;

		string contextName   = pSnmpServerSnmpConfigSetContextMappingMessage->getContextName();
		string vrfName       = pSnmpServerSnmpConfigSetContextMappingMessage->getVrfName();

		if ((contextName.length() < 2) || (contextName.length() > AGT_MAX_CONTEXT_SIZE) ) {
            pSnmpServerSnmpConfigSetContextMappingMessage->setCompletionStatus(WRC_SNMP_INVALID_CONTEXT_LENGTH);
            reply (pSnmpServerSnmpConfigSetContextMappingMessage);
            return;
        }
	
		if (vrfName.length() > AGT_MAX_CONTEXT_SIZE) {
            pSnmpServerSnmpConfigSetContextMappingMessage->setCompletionStatus(WRC_SNMP_INVALID_VRFNAME_LENGTH);
            reply (pSnmpServerSnmpConfigSetContextMappingMessage);
            return;
        }
        trace(TRACE_LEVEL_DEBUG,string("setContextMappingMesasageHandler")+contextName);

		SnmpServerIntSnmpConfigSetContextMappingIntMessage *pMessage =
					new SnmpServerIntSnmpConfigSetContextMappingIntMessage (contextName, vrfName);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&SnmpContextMappingWorker::setContextMappingSTCCB), pSnmpServerSnmpConfigSetContextMappingMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void SnmpContextMappingWorker::setContextMappingSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
		trace (TRACE_LEVEL_DEBUG, "Entering SNMP ContextMapping Set TCCB");
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigSetContextMappingMessage *pSnmpServerSnmpConfigSetContextMappingMessage = 
                                (SnmpServerSnmpConfigSetContextMappingMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS; 
		
		string contextName   = pSnmpServerSnmpConfigSetContextMappingMessage->getContextName();
		string vrfName       = pSnmpServerSnmpConfigSetContextMappingMessage->getVrfName();
        string errorMessage;
		FILE *fp = NULL;

	   if (pSnmpServerSnmpConfigSetContextMappingMessage->getIsALastConfigReplay() == false                     						&& (fp = fopen("/tmp/unsync.txt", "r"))) {
		    trace (TRACE_LEVEL_INFO, "Triggered Last Replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigSetContextMappingMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigSetContextMappingMessage);
            return;
        }

        trace(TRACE_LEVEL_DEBUG,string("setContextMappingTCCB")+contextName);
        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting  snmp community operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Setting snmp community operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::setontextMappingSTCCB", errorMessage);
#if 0
                if (errorMessage.length() != 0)
                    status = CLUSTER_ERROR_1001;
#endif
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
            ContextMappingManagedObject   *pSetObject     = NULL;

            WaveManagedObjectSynchronousQueryContext *syncQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(ContextMappingManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (contextName, "contextName"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) 
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +contextName);
                pSnmpServerSnmpConfigSetContextMappingMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_CONTEXT_NAME_STR_ERROR);
				
				// clean up allocated messages
				SnmpServerIntSnmpConfigSetContextMappingIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigSetContextMappingIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigSetContextMappingMessage);
                delete syncQueryContext;
                return;
            }

            pSetObject = dynamic_cast<ContextMappingManagedObject *> ((*pResultsRollback)[0]);
            delete syncQueryContext;
            delete pResultsRollback;

            /*
            * Messages sent to local handlers have succedeed. Prepare to commit to DB.
            * Query the community MO with provided string and change the access.
            */
            
            //pSetObject->setContextName(contextName);
            if(vrfName.length() !=0) {
                pSetObject->setVrfName(vrfName);
            }

            startTransaction ();
            updateWaveManagedObject (pSetObject);
            
            fstatus = commitTransaction ();

            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_DEBUG, " SNMP ContextMapping SET: Successfully Cluster-Wide committed the contextMapping Configuration");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL,
                    "SNMP ContextMapping SET : Commiting Context Mapping config failed.Status:"
                    + FrameworkToolKit::localize (fstatus));
            }

            delete pSetObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigSetContextMappingIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigSetContextMappingIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
		pSnmpServerSnmpConfigSetContextMappingMessage->setMessageString (errorMessage);
		pSnmpServerSnmpConfigSetContextMappingMessage->setCompletionStatus (status);
		reply (pSnmpServerSnmpConfigSetContextMappingMessage);
    }  
}
 
