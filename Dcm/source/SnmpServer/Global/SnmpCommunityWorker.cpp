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
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "DcmResourceIds.h"
#include "DcmCore/DcmToolKit.h"


#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpCommunityWorker.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigSetCommMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetCommIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveCommIntMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigRemoveCommMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigAddCommMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddCommIntMessage.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/HostsManagedObject.h"
#include <snmp/snmplib.h>

#include "AAA/Global/AAASequencerContexts.h"
namespace DcmNs
{

    SnmpCommunityWorker::SnmpCommunityWorker ( SnmpServerObjectManager *pSnmpServerObjectManager)
        : WaveWorker  (pSnmpServerObjectManager)
    {
        CommunityManagedObject    CommunityManagedObject    (pSnmpServerObjectManager);
        CommunityManagedObject.setupOrm ();
        addManagedClass (CommunityManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERSNMPCONFIGSETCOMM, reinterpret_cast<PrismMessageHandler> (&SnmpCommunityWorker::SnmpServerSnmpConfigSetCommMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGREMOVECOMM, reinterpret_cast<PrismMessageHandler> (&SnmpCommunityWorker::SnmpServerSnmpConfigRemoveCommMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGADDCOMM, reinterpret_cast<PrismMessageHandler> (&SnmpCommunityWorker::SnmpServerSnmpConfigAddCommMessageHandler));
    }

    SnmpCommunityWorker::~SnmpCommunityWorker ()
    {
    }

    PrismMessage  *SnmpCommunityWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERSNMPCONFIGSETCOMM :
                pPrismMessage = new SnmpServerSnmpConfigSetCommMessage ();
                break;
            case SNMPSERVERSNMPCONFIGREMOVECOMM :
                pPrismMessage = new SnmpServerSnmpConfigRemoveCommMessage ();
                break;
            case SNMPSERVERSNMPCONFIGADDCOMM :
                pPrismMessage = new SnmpServerSnmpConfigAddCommMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SnmpCommunityWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((CommunityManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CommunityManagedObject(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "SnmpCommunityWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	/*
	** This function is the global message handler to add new community string.
	** Called from: Community WyserEaConfiguration Worker.
	** Inputs:		Community string and access (optional)
	** Functions:	Sends messages to local services and commits the new community to DB.
	** Errors: 		If message sending to local services fail or commit to DB fails.
	** On success:	Return WAVE_MESSAGE_SUCCESS.
	*/

    void  SnmpCommunityWorker::SnmpServerSnmpConfigAddCommMessageHandler( SnmpServerSnmpConfigAddCommMessage *pSnmpServerSnmpConfigAddCommMessage)
    {
		int resSize = 0, i = 0, isLastCmd = 0;
		if (pSnmpServerSnmpConfigAddCommMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Add Community Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		
		vector<UI32> locations;
        string commStr   	= pSnmpServerSnmpConfigAddCommMessage->getCommunity();
		Access commAccess 	= pSnmpServerSnmpConfigAddCommMessage->getAccess();

		if ((commStr.length() < 2) || (commStr.length() > 16) ) {
			pSnmpServerSnmpConfigAddCommMessage->setCompletionStatus(WRC_SNMP_INVALID_COMMUNITY_LENGTH);
			reply (pSnmpServerSnmpConfigAddCommMessage);
			return;
		}

		if ((commAccess != rw) && (commAccess != ro)) {
			pSnmpServerSnmpConfigAddCommMessage->setCompletionStatus(WRC_SNMP_INVALID_ACCESS_ERROR);
			reply (pSnmpServerSnmpConfigAddCommMessage);
			return;
		}

	/* In case of last replay CLI configuration, check if the community string
     * is already exist in the DB. If so, then return success without updating the DB. 
     */

    if (pSnmpServerSnmpConfigAddCommMessage->getIsALastConfigReplay() == true) {
        trace (TRACE_LEVEL_INFO, "Add Community Global:: isALastConfigReplay");
	     WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new
    	        WaveManagedObjectSynchronousQueryContext(
                CommunityManagedObject::getClassName());
	       vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

	        if (pResults == NULL || pResults->size() == 0) {
    	        trace (TRACE_LEVEL_INFO, "Add Community Global:: No MO exists");
        	} else {
		        resSize = pResults->size();
        		trace (TRACE_LEVEL_INFO,
	               string ("Add Community Global:: Community resSize : ") +resSize);

    		    for (i = 0; i < resSize; i ++) {
			    	CommunityManagedObject *pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResults)[i]);
            	    if (pCommManagedObject == NULL) {
                    	trace (TRACE_LEVEL_ERROR,
                        "Add Community Global:: pCommManagedObject dynamic_cast failed");
                	}
                	if (commStr.compare(pCommManagedObject->getCommunity()) == 0) {
						isLastCmd = 1;
        	        }
                	delete pCommManagedObject;
            	}
        	}
	        delete pResults;
    	    delete synchronousQueryContext;

			if (isLastCmd == 1) {
		        trace (TRACE_LEVEL_INFO,
        	           			string ("Add Community Global:: Community String already exists: ") +commStr);
				pSnmpServerSnmpConfigAddCommMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
				reply(pSnmpServerSnmpConfigAddCommMessage);
				return;
			}
		} // End of isLast Replay

        SnmpServerIntSnmpConfigAddCommIntMessage *pMessage =
					new SnmpServerIntSnmpConfigAddCommIntMessage (commStr, commAccess);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&SnmpCommunityWorker::addCommSTCCB), pSnmpServerSnmpConfigAddCommMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void SnmpCommunityWorker::addCommSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigAddCommMessage *pSnmpServerSnmpConfigAddCommMessage = 
                                (SnmpServerSnmpConfigAddCommMessage *)pWaveSendToClusterContext->getPCallerContext();

        string errorMessage;
        bool       allFailed  = false;
        ResourceId status     = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus    = WAVE_MESSAGE_SUCCESS;
        string     commStr    = pSnmpServerSnmpConfigAddCommMessage->getCommunity();
		Access     commAccess = pSnmpServerSnmpConfigAddCommMessage->getAccess();
        CommunityManagedObject *pCommManagedObject = NULL;

		// Added for testing purpose to simulate lastconfig replay usecase
		FILE *fp = NULL;
        if (pSnmpServerSnmpConfigAddCommMessage->getIsALastConfigReplay() == false 
					&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "isLast Replay event triggered");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigAddCommMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigAddCommMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding snmp community operation failed on one or more of the connected nodes.");
            else
            {
                trace (TRACE_LEVEL_ERROR, "FAILURE - Adding snmp community operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "SnmpCommunityWorker::addCommSTCCB", errorMessage);
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
		* Use the input community string and access (default ro) to commit to db.
		*/
        if (!allFailed)
        {
            startTransaction ();
            pCommManagedObject = dynamic_cast<CommunityManagedObject *>(createManagedObjectInstance(CommunityManagedObject::getClassName()));

            pCommManagedObject->setCommunity(commStr);
            pCommManagedObject->setAccess(commAccess);

            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO, 
                    "SNMP GlobalAdd Comm MessageHandler: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                        "SNMP GlobalAdd Comm MessageHandler:Commiting failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
            delete pCommManagedObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigAddCommIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigAddCommIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigAddCommMessage->setCompletionStatus (status);
        pSnmpServerSnmpConfigAddCommMessage->setMessageString (errorMessage);
        reply (pSnmpServerSnmpConfigAddCommMessage);
    }

	/*
	** This function is the global message handler to delete community string.
	** Called from: Community WyserEaConfiguration Worker.
	** Inputs:		Community string and access (optional)
	** Functions:	Sends messages to local services and removes the community from DB.
	** Note: 		Only those community strings that are not reference by any host can be deleted.
	** Errors: 		If message sending to local services fail or commit to DB fails.
	** On success:	Return WAVE_MESSAGE_SUCCESS.
	*/

    void  SnmpCommunityWorker::SnmpServerSnmpConfigRemoveCommMessageHandler( SnmpServerSnmpConfigRemoveCommMessage *pSnmpServerSnmpConfigRemoveCommMessage)
    {
		int i = 0, resSize = 0, isExist = 0, chkHost = 0;
		int version1 = 0, version2c = 1, v2hostSize = 0, v1hostSize = 0;
		vector<UI32> locations;
		ObjectId ObjId;

		trace (TRACE_LEVEL_INFO, "Entering SNMP community remove handler");

		if (pSnmpServerSnmpConfigRemoveCommMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Remove Community Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		string commStr   = pSnmpServerSnmpConfigRemoveCommMessage->getCommunity();

		/* Before deleting community string check if any host entry is
		 * associated with it */

		WaveManagedObjectSynchronousQueryContext *synchronousQueryCommunity = new
            WaveManagedObjectSynchronousQueryContext(CommunityManagedObject::getClassName());

		vector<WaveManagedObject *> *pResultsCom = querySynchronously (synchronousQueryCommunity);
        if ((NULL == pResultsCom) || (pResultsCom->size() == 0)) {
            trace (TRACE_LEVEL_INFO, "Del Community Global:: No community MO");
        } else {
            resSize = pResultsCom->size();
         	trace (TRACE_LEVEL_INFO,
	               string ("Del Community Global:: Community resSize : ") +resSize);
            for (i = 0; i < resSize; i ++) {
	        	CommunityManagedObject *pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResultsCom)[i]);
                if (pCommManagedObject == NULL) {
                    trace (TRACE_LEVEL_ERROR,
                        "Del Host Global:: pCommManagedObject dynamic_cast failed");
                }
                if (commStr.compare(pCommManagedObject->getCommunity()) == 0) {
					isExist = 1;
        	        ObjId  = pCommManagedObject->getObjectId();
                }
            	delete pCommManagedObject;
			}

	        WaveManagedObjectSynchronousQueryContext *syncQueryHost = new
    	        WaveManagedObjectSynchronousQueryContext(HostsManagedObject::getClassName());
	        syncQueryHost->addAndAttribute (new AttributeEnum ((UI32 *) &version1, "version"));
    	    vector<WaveManagedObject *> *pResultsHost = querySynchronously (syncQueryHost);

        	if ((NULL == pResultsHost) || (pResultsHost->size() == 0))
       		{
            	trace (TRACE_LEVEL_INFO, "Del Community Global::No host MO");
        	} else {
            	v1hostSize = pResultsHost->size();
        		trace (TRACE_LEVEL_INFO,
		               string ("Del Community Global:: v2Host resSize : ") +v1hostSize);
	            for (i = 0; i < v1hostSize; i ++) {
    	            HostsManagedObject *pHostManagedObject = dynamic_cast <HostsManagedObject *> ((*pResultsHost)[i]);
	                if (pHostManagedObject == NULL) {
    	                trace (TRACE_LEVEL_ERROR,
                        "Del Community Global:: pHostManagedObject dynamic_cast failed");
                	}
	                if (ObjId == pHostManagedObject->getCommunity()) {
    	                 chkHost = 1;
		 			}
	            	delete pHostManagedObject;
    	        }
			}
        	delete pResultsHost;
        	delete syncQueryHost;

	        if (!chkHost) {
    		    WaveManagedObjectSynchronousQueryContext *syncQueryHostv2 = new
            	WaveManagedObjectSynchronousQueryContext(HostsManagedObject::getClassName());

		        syncQueryHostv2->addAndAttribute (new AttributeEnum ((UI32 *)&version2c, "version"));

		        vector<WaveManagedObject *> *pResultsHostv2 = querySynchronously (syncQueryHostv2);

	            if ((NULL == pResultsHostv2) || (pResultsHostv2->size() == 0)) {
    	    	    trace (TRACE_LEVEL_INFO, "Del Community Global::No host v2c MO");
            	} else {
		            v2hostSize = pResultsHostv2->size();
        			trace (TRACE_LEVEL_INFO,
		               string ("Del Community Global:: v2Host resSize : ") +v2hostSize);
		            for (i = 0; i < v2hostSize; i ++) {
        		        HostsManagedObject *pHostv2ManagedObject = dynamic_cast <HostsManagedObject *> ((*pResultsHostv2)[i]);
		                if (pHostv2ManagedObject == NULL) {
        		            trace (TRACE_LEVEL_ERROR,
		                        "Del Community Global:: pHostManagedObject dynamic_cast failed");
						}
                		if (ObjId == pHostv2ManagedObject->getCommunity()) {
				            chkHost = 1;
        		        }
	            		delete pHostv2ManagedObject;
		            }
		        }
            	delete pResultsHostv2;
	            delete syncQueryHostv2;
    	    }
		}
		delete pResultsCom;
	    delete synchronousQueryCommunity;

        if (chkHost) {
            pSnmpServerSnmpConfigRemoveCommMessage->setCompletionStatus(WRC_SNMP_COMMUNITY_ASSOCIATED_ERROR);
    	   	trace (TRACE_LEVEL_INFO, "Del Community Global:: Delete Community Failed");
            reply (pSnmpServerSnmpConfigRemoveCommMessage);
            return;
        }
     

	/* In case of last replay CLI configuration, check if the community string
	 * is already deleted in the DB. If so, then return success without
	 * deleting it from the DB.
	 */

    if (pSnmpServerSnmpConfigRemoveCommMessage->getIsALastConfigReplay() == true) {
        trace (TRACE_LEVEL_INFO, "Remove Community Global:: isALastConfigReplay");
        if (isExist == 0) {
            trace (TRACE_LEVEL_INFO,
               string ("Remove Community Global:: Community already deleted ") +commStr);
            pSnmpServerSnmpConfigRemoveCommMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigRemoveCommMessage);
            return;
        }
    } // End of IsLast Replay

		SnmpServerIntSnmpConfigRemoveCommIntMessage *pMessage =
					new SnmpServerIntSnmpConfigRemoveCommIntMessage (commStr);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&SnmpCommunityWorker::delCommSTCCB), pSnmpServerSnmpConfigRemoveCommMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void SnmpCommunityWorker::delCommSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigRemoveCommMessage *pSnmpServerSnmpConfigRemoveCommMessage = 
                                (SnmpServerSnmpConfigRemoveCommMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool allFailed      = false;
        ResourceId status   = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus  = WAVE_MESSAGE_SUCCESS; 
		string commStr      = pSnmpServerSnmpConfigRemoveCommMessage->getCommunity();
        string errorMessage;
        CommunityManagedObject *pCommManagedObject = NULL;

		// Added for testing purpose to simulate Last Replay CLI usecase
		FILE *fp = NULL;
        if (pSnmpServerSnmpConfigRemoveCommMessage->getIsALastConfigReplay() == false 
				&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered isLast Replay");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigRemoveCommMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigRemoveCommMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting snmp community operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting snmp community operation failed on all nodes");
            }
            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delCommSTCCB", errorMessage);
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
                new WaveManagedObjectSynchronousQueryContext(CommunityManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (commStr, "community"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) 
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +commStr);
                pSnmpServerSnmpConfigRemoveCommMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
				// clean up allocated messages
				SnmpServerIntSnmpConfigRemoveCommIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigRemoveCommIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigRemoveCommMessage);
                delete syncQueryContext;
                return;
            }

            pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResultsRollback)[0]);
            delete pResultsRollback;
            delete syncQueryContext;

            /*
            * By now messages send to local handlers have succeeded.
            * Prepare to commit to DB.
            * Query the string inputted and remove that MO.
            */

            startTransaction ();
            delete pCommManagedObject;
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO,
                        "SNMP CommDelete MessageHandler: Successful Cluster-Wide commit CommunityConfig");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, 
                        "SNMP Comm Delete MessageHandler:Commiting Community config failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigRemoveCommIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigRemoveCommIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigRemoveCommMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigRemoveCommMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigRemoveCommMessage);
    }

	/*
	** This function is the global message handler to change access of a community string.
	** Called from: Community WyserEaConfiguration Worker.
	** Inputs:		Community string and access (optional)
	** Functions:	Sends messages to local services and changes access of community in DB.
	** Note: 		Only 3 community strings can be configured as ro or rw. 
	** Backend commit will fail if more than 3 are configured in each access group.
	** Errors: 		If message sending to local services fail or commit to DB fails.
	** On success:	Return WAVE_MESSAGE_SUCCESS.
	*/


    void  SnmpCommunityWorker::SnmpServerSnmpConfigSetCommMessageHandler( SnmpServerSnmpConfigSetCommMessage *pSnmpServerSnmpConfigSetCommMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering SNMP Community Set message handler");

		if (pSnmpServerSnmpConfigSetCommMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Set Community global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
		string commStr   = pSnmpServerSnmpConfigSetCommMessage->getCommunity();
		Access accessStr = pSnmpServerSnmpConfigSetCommMessage->getAccess();
		SnmpServerIntSnmpConfigSetCommIntMessage *pMessage =
					new SnmpServerIntSnmpConfigSetCommIntMessage (commStr, accessStr);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&SnmpCommunityWorker::setCommSTCCB), pSnmpServerSnmpConfigSetCommMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void SnmpCommunityWorker::setCommSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {   
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigSetCommMessage *pSnmpServerSnmpConfigSetCommMessage = 
                                (SnmpServerSnmpConfigSetCommMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS; 
		string     commStr   = pSnmpServerSnmpConfigSetCommMessage->getCommunity();
		Access     accessStr = pSnmpServerSnmpConfigSetCommMessage->getAccess();
        string errorMessage;

		// Added for testing purpose to simulate lastconfig replay usecase
        FILE *fp = NULL;
        if (pSnmpServerSnmpConfigSetCommMessage->getIsALastConfigReplay() == false 
                    && (fp = fopen("/tmp/unsync.txt", "r"))) {           
			trace (TRACE_LEVEL_INFO, "isLast Replay event triggered");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigSetCommMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigSetCommMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting snmp community operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting snmp community operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delCommSTCCB", errorMessage);
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
            CommunityManagedObject   *pSetObject     = NULL;

            WaveManagedObjectSynchronousQueryContext *syncQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(CommunityManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (commStr, "community"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) 
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +commStr);
                pSnmpServerSnmpConfigSetCommMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
				
				// clean up allocated messages
				SnmpServerIntSnmpConfigSetCommIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigSetCommIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigSetCommMessage);
                delete syncQueryContext;
                return;
            }

            pSetObject = dynamic_cast<CommunityManagedObject *> ((*pResultsRollback)[0]);
            delete syncQueryContext;
            delete pResultsRollback;

            /*
            * Messages sent to local handlers have succedeed. Prepare to commit to DB.
            * Query the community MO with provided string and change the access.
            */

            startTransaction ();
            updateWaveManagedObject (pSetObject);

            if ((ro <= accessStr) &&  (accessStr <= rw))
            {
                trace (TRACE_LEVEL_INFO, "SNMP Community SET: Setting elementName " + accessStr);
                pSetObject->setAccess (accessStr);
            }
            fstatus = commitTransaction ();

            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO, " SNMP Community SET: Successfully Cluster-Wide committed the Community Configuration");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL,
                    "SNMP Community SET : Commiting Community config failed.Status:"
                    + FrameworkToolKit::localize (fstatus));
            }

            delete pSetObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigSetCommIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigSetCommIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
		pSnmpServerSnmpConfigSetCommMessage->setMessageString (errorMessage);
		pSnmpServerSnmpConfigSetCommMessage->setCompletionStatus (status);
		reply (pSnmpServerSnmpConfigSetCommMessage);
    }  
}
 
