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
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"      
#include "DcmResourceIds.h"

#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/ConfigV3HostWorker.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigAddV3HostMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigDelV3HostMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigChangeV3HostMessage.h"
#include "SnmpServer/Global/SnmpUserManagedObject.h"
#include "SnmpServer/Global/SystemGroupManagedObject.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigChangeV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigDelV3HostIntMessage.h"
#include "SnmpServer/Local/SNMPLocalBootMessage.h"
#include "SnmpServer/Global/V3HostsManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/enumdatatype.h"

#include "snmp/snmplib.h"

#include "AAA/Global/AAASequencerContexts.h"
namespace DcmNs
{

    ConfigV3HostWorker::ConfigV3HostWorker ( SnmpServerObjectManager *pSnmpServerObjectManager)
        : WaveWorker  (pSnmpServerObjectManager)
    {
        V3HostsManagedObject    snmpV3HostsManagedObject    (pSnmpServerObjectManager);
        snmpV3HostsManagedObject.setupOrm ();
        addManagedClass (V3HostsManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERSNMPCONFIGADDV3HOST, reinterpret_cast<PrismMessageHandler> (&ConfigV3HostWorker::SnmpServerSnmpConfigAddV3HostMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGDELV3HOST, reinterpret_cast<PrismMessageHandler> (&ConfigV3HostWorker::SnmpServerSnmpConfigDelV3HostMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGCHANGEV3HOST, reinterpret_cast<PrismMessageHandler> (&ConfigV3HostWorker::SnmpServerSnmpConfigChangeV3HostMessageHandler));
    }

    ConfigV3HostWorker::~ConfigV3HostWorker ()
    {
    }

    PrismMessage  *ConfigV3HostWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERSNMPCONFIGADDV3HOST :
                pPrismMessage = new SnmpServerSnmpConfigAddV3HostMessage ();
                break;
            case SNMPSERVERSNMPCONFIGDELV3HOST :
                pPrismMessage = new SnmpServerSnmpConfigDelV3HostMessage ();
                break;
            case SNMPSERVERSNMPCONFIGCHANGEV3HOST :
                pPrismMessage = new SnmpServerSnmpConfigChangeV3HostMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigV3HostWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((V3HostsManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new V3HostsManagedObject(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "ConfigV3HostWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
/*
*	This function is the Global handler to add new v3 host.
*	Inputs: 	IPAddress, Username string added by user, optional version, Port number.
*	Functions: 	Sends message to Local Add v3Host handler, if success commit the DB with new V3host.
*	Error Case:	Message sending to Local failed or commit to DB failed.	
*/

    void  ConfigV3HostWorker::SnmpServerSnmpConfigAddV3HostMessageHandler( SnmpServerSnmpConfigAddV3HostMessage *pSnmpServerSnmpConfigAddV3HostMessage)
    {
		int i = 0, resV3Size = 0; 
		int retUserSize = 0, userExist = 0, isLastCmd = 0;
		ObjectId userObjId;
		string ipStr;
		HostUC ipVal;
	
		trace (TRACE_LEVEL_INFO, "Entering SNMP V3Host Add handler");
		if (pSnmpServerSnmpConfigAddV3HostMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Add V3Host Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
		string      userName   = pSnmpServerSnmpConfigAddV3HostMessage->getUsername();
		HostUC		ipAddr     = pSnmpServerSnmpConfigAddV3HostMessage->getHostUC();
		UI32        udpPort    = pSnmpServerSnmpConfigAddV3HostMessage->getUdpPort();
		NotifyType  nType      = pSnmpServerSnmpConfigAddV3HostMessage->getNotify();
        string      engineId   = pSnmpServerSnmpConfigAddV3HostMessage->getEngineid();
		SevLevel	sevLevel   = pSnmpServerSnmpConfigAddV3HostMessage->getSevLevel();
    
		ipStr = ipAddr.getHostValue();

        if ((engineId.length() < 10) || (engineId.length() > 65) )
        {
            pSnmpServerSnmpConfigAddV3HostMessage->setCompletionStatus (WRC_SNMP_V3_INVALID_ENGINEID_ERROR);
            reply (pSnmpServerSnmpConfigAddV3HostMessage);
            return;
        }

		trace (TRACE_LEVEL_INFO, string("Add V3Host Global::Host ") +ipStr);
		trace (TRACE_LEVEL_INFO, string("Add V3Host Global::userName ") +userName);

		/* Before adding any v3host entry check if the associated username is exits or not */

		WaveManagedObjectSynchronousQueryContext *syncQueryContextUser = new
                WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());

		vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContextUser);

        if ((NULL == pResults) || (pResults->size() == 0)) {
        	trace (TRACE_LEVEL_INFO, "Add V3Host Global:: no User MO");
         } else {
         	retUserSize = pResults->size();
			trace (TRACE_LEVEL_INFO, string("Add V3Host Global::retUserSize ") +retUserSize);
            for (i = 0; i < retUserSize; i ++) {
            	SnmpUserManagedObject *pUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResults)[i]);
                if (pUserManagedObject == NULL) {
		        	trace (TRACE_LEVEL_ERROR,
		                        "Add V3Host Global:: pUserManagedObject dynamic_cast failed");
                }
				if (userName.compare(pUserManagedObject->getUsername()) == 0) {
					userObjId = pUserManagedObject->getObjectId();
					userExist = 1;
				}
				delete pUserManagedObject;
			}
		}
		delete pResults;
		delete syncQueryContextUser;

		if (!userExist) {
			pSnmpServerSnmpConfigAddV3HostMessage->setCompletionStatus (WRC_SNMP_INVALID_USER_ERROR);
        	trace (TRACE_LEVEL_INFO, "Add V3Host Global:: Add v3Host Failed");
	        reply (pSnmpServerSnmpConfigAddV3HostMessage);
	        return;
		}	
	
		/* In case of last replay CLI configuration, check if the v3Host 
	     * is already exist in the DB. If so, then return Success without updating the DB.
    	 */

		if (pSnmpServerSnmpConfigAddV3HostMessage->getIsALastConfigReplay() == true) {
            trace (TRACE_LEVEL_INFO, "Add V3Host Global:: getIsALastConfigReplay");
			WaveManagedObjectSynchronousQueryContext *syncQueryV3Host = new
    	        WaveManagedObjectSynchronousQueryContext(
        	    V3HostsManagedObject::getClassName());
	        vector<WaveManagedObject *> *pResultsV3Host = querySynchronously (syncQueryV3Host);

    	    if ((NULL == pResultsV3Host) || (pResultsV3Host->size() == 0)) {
        	    trace (TRACE_LEVEL_INFO,
            	    "Add V3Host Global:: No V3Host MO");
	        } else {
    		    resV3Size = pResultsV3Host->size();
		        trace (TRACE_LEVEL_INFO,
                string ("Add V3Host Global::resV3Size : ") +resV3Size);
        		for (i = 0; i < resV3Size; i ++) {
					V3HostsManagedObject *pV3HostManagedObject = dynamic_cast
        	            <V3HostsManagedObject *> ((*pResultsV3Host)[i]);
					 if (pV3HostManagedObject == NULL) {
                    	trace (TRACE_LEVEL_ERROR,
	                        "Add V3Host Global: pV3HostManagedObject dynamic_cast failed");
    		         }
						ipVal = pV3HostManagedObject->getHostUC();
					if ((ipStr.compare(ipVal.getHostValue()) == 0) &&
						(userObjId == pV3HostManagedObject->getUsername())) {
								isLastCmd = 1;
					}
	        		delete pV3HostManagedObject;
				}
			}
        delete pResultsV3Host;
		delete syncQueryV3Host;

			if (isLastCmd == 1) {
			    trace (TRACE_LEVEL_INFO, string ("Add V3Host Global:: V3Host already exists: ") +ipStr);
        		trace (TRACE_LEVEL_INFO, string ("Add V3Host userName Global:: V3Host userName already exists: ") +userName);
			    pSnmpServerSnmpConfigAddV3HostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
			    reply (pSnmpServerSnmpConfigAddV3HostMessage);
				return;
			}
		} // End of IsLast Replay

        SnmpServerIntSnmpConfigAddV3HostIntMessage *pMessage = 
					new SnmpServerIntSnmpConfigAddV3HostIntMessage 
					(ipAddr, userName, nType, udpPort, engineId, sevLevel);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigV3HostWorker::addV3HostSTCCB), 
             pSnmpServerSnmpConfigAddV3HostMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigV3HostWorker::addV3HostSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigAddV3HostMessage *pSnmpServerSnmpConfigAddV3HostMessage = 
                                (SnmpServerSnmpConfigAddV3HostMessage *)pWaveSendToClusterContext->getPCallerContext();
		
		// Added for testing purpose to simulate LastReplay usecase
		FILE *fp = NULL;
        if (pSnmpServerSnmpConfigAddV3HostMessage->getIsALastConfigReplay() == false 
				&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered isLast Replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigAddV3HostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigAddV3HostMessage);
            return;
        }

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS;
        string errorMessage;

 		string      userName   = pSnmpServerSnmpConfigAddV3HostMessage->getUsername();
		HostUC		ipAddr     = pSnmpServerSnmpConfigAddV3HostMessage->getHostUC();
		UI32        udpPort    = pSnmpServerSnmpConfigAddV3HostMessage->getUdpPort();
		NotifyType  nType      = pSnmpServerSnmpConfigAddV3HostMessage->getNotify();
        string      engineId   = pSnmpServerSnmpConfigAddV3HostMessage->getEngineid();
		SevLevel	sevLevel   = pSnmpServerSnmpConfigAddV3HostMessage->getSevLevel();


        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding snmp v3 host operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Adding snmp v3 host operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigV3HostWorker::addV3HostSTCCB", errorMessage);
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
		* By now messages to local services have succeeded.
		* Prepare to commit to DB.
		* Query user MO, get Object Id for passed user string.
		* Use this Id to query host MO and add the Id as user field in Host MO.
		*/

        if (!allFailed)
        {
            SnmpUserManagedObject *pSnmpUserManagedObject = NULL;
            V3HostsManagedObject *pV3HostManagedObject = NULL;


            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());
             synchronousQueryContext->addAndAttribute (new AttributeString (userName, "username"));
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +userName);
                pSnmpServerSnmpConfigAddV3HostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_USER_ERROR);
				
		        // clean up allocated messages
        		SnmpServerIntSnmpConfigAddV3HostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigAddV3HostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		        delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigAddV3HostMessage);
                delete synchronousQueryContext;
                delete pWaveSendToClusterContext;
                return;
            }
            pSnmpUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResults)[0]);
            delete synchronousQueryContext;
            delete pResults;
           
            startTransaction ();
            pV3HostManagedObject = dynamic_cast<V3HostsManagedObject *>(createManagedObjectInstance(V3HostsManagedObject::getClassName()));

            pV3HostManagedObject->setNotify(nType);         
            pV3HostManagedObject->setUsername(pSnmpUserManagedObject->getObjectId());
            pV3HostManagedObject->setHostUC(ipAddr);
            pV3HostManagedObject->setUdpPort (udpPort);
            pV3HostManagedObject->setEngineid (engineId);
            pV3HostManagedObject->setSevLevel (sevLevel);

            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO, 
                    "SNMP GlobalAdd v3Host MessageHandler: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                    "SNMP GlobalAdd v3Host MessageHandler:Commiting failed.Status:"
                    + FrameworkToolKit::localize (fstatus));
            }
            delete pSnmpUserManagedObject;
            delete pV3HostManagedObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigAddV3HostIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigAddV3HostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete pWaveSendToClusterContext;
        pSnmpServerSnmpConfigAddV3HostMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigAddV3HostMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigAddV3HostMessage);       
    }

/*
*   This function is the Global handler to change attributes (version, port) of v1 or v2c V3host.
*   Inputs:     IPAddress, Community string of existing V3host, version and/or Port number.
*   Functions:  Sends message to Local Add V3Host handler, if success commit the DB with change.
*   Error Case: Message sending to Local failed or commit to DB failed.
*   On success: Return WAVE_MESSAGE_SUCCESS.
*/

    void  ConfigV3HostWorker::SnmpServerSnmpConfigChangeV3HostMessageHandler( SnmpServerSnmpConfigChangeV3HostMessage *pSnmpServerSnmpConfigChangeV3HostMessage)
    {
        
		trace (TRACE_LEVEL_INFO, "Entering SNMP Change V3host message handler");
		if (pSnmpServerSnmpConfigChangeV3HostMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Change V3Host Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
		string      userName   = pSnmpServerSnmpConfigChangeV3HostMessage->getUsername();
		HostUC		ipAddr     = pSnmpServerSnmpConfigChangeV3HostMessage->getHostUC();
		int         udpPort    = pSnmpServerSnmpConfigChangeV3HostMessage->getUdpPort();
		NotifyType  nType      = pSnmpServerSnmpConfigChangeV3HostMessage->getNotify();
        string      engineId   = pSnmpServerSnmpConfigChangeV3HostMessage->getEngineid();
        SevLevel    sevLevel   = pSnmpServerSnmpConfigChangeV3HostMessage->getSevLevel();

        if ((engineId.length() < 10) || (engineId.length() > 65) )
        {
            pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus (WRC_SNMP_V3_INVALID_ENGINEID_ERROR);
            reply (pSnmpServerSnmpConfigChangeV3HostMessage);
            return;
        }
		if ((nType != traps) && (nType != informs))
		{
			trace (TRACE_LEVEL_INFO, "SNMP V3Hostworker: Invalid notify type " + nType);
			pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus
                                (WRC_SNMP_INVALID_NOTIFY_ERROR);
            reply (pSnmpServerSnmpConfigChangeV3HostMessage);
            return;
		}

		if (udpPort < 0) {
			trace (TRACE_LEVEL_INFO, string ("SNMP V3Hostworker: Invalid port") +udpPort);
			pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus
                                (WRC_SNMP_INVALID_PORT_NUMBER);
            reply (pSnmpServerSnmpConfigChangeV3HostMessage);
            return;
		}
 		SnmpServerIntSnmpConfigChangeV3HostIntMessage *pMessage =
            new SnmpServerIntSnmpConfigChangeV3HostIntMessage (ipAddr, userName, 
                    nType, udpPort, engineId, sevLevel);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigV3HostWorker::setV3HostSTCCB),
                        pSnmpServerSnmpConfigChangeV3HostMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigV3HostWorker::setV3HostSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {

	    AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;
        SnmpServerSnmpConfigChangeV3HostMessage *pSnmpServerSnmpConfigChangeV3HostMessage = 
                        (SnmpServerSnmpConfigChangeV3HostMessage *)pWaveSendToClusterContext->getPCallerContext();

		string      userName     = pSnmpServerSnmpConfigChangeV3HostMessage->getUsername();
		HostUC		ipAddr       = pSnmpServerSnmpConfigChangeV3HostMessage->getHostUC();
		int         udpPort      = pSnmpServerSnmpConfigChangeV3HostMessage->getUdpPort();
		NotifyType  nType        = pSnmpServerSnmpConfigChangeV3HostMessage->getNotify();
        string      engineId     = pSnmpServerSnmpConfigChangeV3HostMessage->getEngineid();
        SevLevel    sevLevel     = pSnmpServerSnmpConfigChangeV3HostMessage->getSevLevel();

        string errorMessage;
        bool        allFailed    = false;
        ResourceId  status       = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId  fstatus      = WAVE_MESSAGE_SUCCESS;
     	V3HostsManagedObject   *pV3HostObject         = NULL;
		SnmpUserManagedObject  *pUserObject           = NULL;

		FILE *fp = NULL;                // Added for testing purpose to simulate lastconfig replay usecase
        if (pSnmpServerSnmpConfigChangeV3HostMessage->getIsALastConfigReplay() == false
                    && (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "triggered last replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigChangeV3HostMessage);
            return;
        }     

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Changing snmp host operation failed on one or more of the connected nodes.");
            else 
            {
                trace (TRACE_LEVEL_ERROR, "FAILURE - Changing snmp host operation failed on all nodes");
                allFailed = true;
            }
            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::changeHostSTCCB", errorMessage);
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
                new WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (userName, "username"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +userName);
                pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_USER_ERROR);
		        // clean up allocated messages
        		SnmpServerIntSnmpConfigChangeV3HostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigChangeV3HostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		        delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigChangeV3HostMessage);
                delete syncQueryContext;
                delete pWaveSendToClusterContext;
                return;
            }

            pUserObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsRollback)[0]);
            delete syncQueryContext;
            delete pResultsRollback;
            ObjectId userOIRB = pUserObject->getObjectId();
            delete pUserObject;
            WaveManagedObjectSynchronousQueryContext *syncQueryV3Host = new 
                    WaveManagedObjectSynchronousQueryContext(V3HostsManagedObject::getClassName());
            syncQueryV3Host->addAndAttribute (new AttributeHostUC(&ipAddr, true, "hostip", snmp_server_ip));
            syncQueryV3Host->addAndAttribute (new AttributeObjectIdAssociation( 
                    &userOIRB,"username", SnmpUserManagedObject::getClassName ()));

            vector<WaveManagedObject *> *pResultsV3Host = querySynchronously (syncQueryV3Host);

            if ((NULL == pResultsV3Host) || (pResultsV3Host->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such V3host managed object");
                pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus
                                    (WRC_SNMP_V3_INVALID_HOSTNAME_ERROR);
				
    		    // clean up allocated messages
	        	SnmpServerIntSnmpConfigChangeV3HostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigChangeV3HostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		        delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigChangeV3HostMessage);
                delete syncQueryV3Host;
                delete pWaveSendToClusterContext;
                return;
            }
            
            pV3HostObject = dynamic_cast<V3HostsManagedObject *> ((*pResultsV3Host)[0]);
            delete pResultsV3Host;
            delete syncQueryV3Host;

            startTransaction ();     
            updateWaveManagedObject (pV3HostObject);
            if ((traps <= nType) && (nType <= informs))
            {
                trace (TRACE_LEVEL_INFO, "SNMP V3Host worker: Setting elementName " + nType);
                pV3HostObject->setNotify (nType);
            }

            if (udpPort != -1)
                pV3HostObject->setUdpPort (udpPort);
	
			if ((sevLevel >= None) && (sevLevel <= Critical))
				pV3HostObject->setSevLevel(sevLevel);

            pV3HostObject->setEngineid (engineId);
            
            fstatus = commitTransaction ();

            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO,
                    "SNMP V3Host Global handler: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "SNMP V3Host Global handler : Commiting failed.Status:"
                            + FrameworkToolKit::localize (fstatus));
            }
            delete pV3HostObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigChangeV3HostIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigChangeV3HostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
	    delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigChangeV3HostMessage->setMessageString (errorMessage);
	    pSnmpServerSnmpConfigChangeV3HostMessage->setCompletionStatus (status);
		reply (pSnmpServerSnmpConfigChangeV3HostMessage);
    }

	/*
	*   This function is the Global handler to delete v1 or v2c V3host.
	*   Inputs:     IPAddress, Community string of existing V3host.
	*   Functions:  Sends message to Local delete V3Host handler, if success commit the DB and removes V3host.
	*   Error Case: Message sending to Local failed or commit to DB failed.
	* 	On success: Return WAVE_MESSAGE_SUCCESS.
	*/

    void  ConfigV3HostWorker::SnmpServerSnmpConfigDelV3HostMessageHandler( SnmpServerSnmpConfigDelV3HostMessage *pSnmpServerSnmpConfigDelV3HostMessage)
    {
		int i = 0, isExist = 0, resV3Size = 0, resUserSize = 0;	
		ObjectId userObjId;
		string ipStr;
		HostUC ipHostv3;
		
		if (pSnmpServerSnmpConfigDelV3HostMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Del V3Host Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
        HostUC		ipVal    = pSnmpServerSnmpConfigDelV3HostMessage->getHostUC();
		string      userName = pSnmpServerSnmpConfigDelV3HostMessage->getUsername();
			
		ipStr = ipVal.getHostValue();

		trace (TRACE_LEVEL_INFO, string("Del V3Host Global::Host ") +ipStr);
		trace (TRACE_LEVEL_INFO, string("Del V3Host Global::userName ") +userName);

		/* In case of last replay CLI configuration, check if the V3Host Entry
	     * is already deleted in the DB. If so, then return success without updating the DB.
    	 */

		if (pSnmpServerSnmpConfigDelV3HostMessage->getIsALastConfigReplay() == true) {
            trace (TRACE_LEVEL_INFO,
                "Del V3Host Global:: getIsALastConfigReplay");
			WaveManagedObjectSynchronousQueryContext *syncQueryUser = new
        	    WaveManagedObjectSynchronousQueryContext(
            	SnmpUserManagedObject::getClassName());
	        vector<WaveManagedObject *> *pResultsUser = querySynchronously (syncQueryUser);
			if ((NULL == pResultsUser) || (pResultsUser->size() == 0)) {
	            trace (TRACE_LEVEL_INFO,
    	            "Del V3Host Global:: No user MO");
			} else {
				resUserSize = pResultsUser->size();
		        trace (TRACE_LEVEL_INFO,
                	string ("Add V3Host Global::resUserSize : ") +resUserSize);
				for (i = 0; i < resUserSize; i ++) {
    			    SnmpUserManagedObject *pUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsUser)[i]);
                	if (pUserManagedObject == NULL) {
                    	trace (TRACE_LEVEL_ERROR,
                        "Del V3Host Global: pUserManagedObject dynamic_cast failed");
					}
					if (userName.compare(pUserManagedObject->getUsername()) == 0) {
						userObjId = pUserManagedObject->getObjectId();
					}
					delete pUserManagedObject;
				}
		
			  	WaveManagedObjectSynchronousQueryContext *syncQueryV3Host = new
    	        	WaveManagedObjectSynchronousQueryContext(
	        	    V3HostsManagedObject::getClassName());
				vector<WaveManagedObject *> *pResultsV3Host = querySynchronously (syncQueryV3Host);
    		    if ((NULL == pResultsV3Host) || (pResultsV3Host->size() == 0)) {
					trace (TRACE_LEVEL_INFO, "Del V3Host Global::No v3 host MO");
				} else {
					resV3Size = pResultsV3Host->size();
		        	trace (TRACE_LEVEL_INFO,
		                string ("Add V3Host Global::resV3Size : ") +resV3Size);
        		    for (i = 0; i < resV3Size; i++) {
		                V3HostsManagedObject *pV3HostManagedObject = dynamic_cast
    		                <V3HostsManagedObject *> ((*pResultsV3Host)[i]);

    	    	        if (pV3HostManagedObject == NULL) {
                		    trace (TRACE_LEVEL_ERROR,
                        	"Del V3Host Global:: pV3HostManagedObject dynamic_cast failed");
		                }
						ipHostv3 = pV3HostManagedObject->getHostUC();
						if ((ipStr.compare(ipHostv3.getHostValue()) == 0) &&
							(userObjId == pV3HostManagedObject->getUsername())) {
									isExist = 1;
						}
						delete pV3HostManagedObject;
					}	
				}
			delete pResultsV3Host;
			delete syncQueryV3Host;		
			}
		delete pResultsUser;
		delete syncQueryUser;

		if (isExist == 0) {
			trace (TRACE_LEVEL_INFO, string("Del V3Host Global::V3Host Already deleted:") +ipStr);
			trace (TRACE_LEVEL_INFO, string("Del V3Host Global::V3Host user Already deleted:") +userName);
			pSnmpServerSnmpConfigDelV3HostMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
			reply(pSnmpServerSnmpConfigDelV3HostMessage);
		}		 
	} // End of IsLast Replay
    	SnmpServerIntSnmpConfigDelV3HostIntMessage *pMessage = 
				new SnmpServerIntSnmpConfigDelV3HostIntMessage (ipVal, userName);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigV3HostWorker::delV3HostSTCCB),
             pSnmpServerSnmpConfigDelV3HostMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigV3HostWorker::delV3HostSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;

        SnmpServerSnmpConfigDelV3HostMessage *pSnmpServerSnmpConfigDelV3HostMessage = 
                                (SnmpServerSnmpConfigDelV3HostMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool allFailed       = false;
        string errorMessage;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS; 
        HostUC	ipVal    = pSnmpServerSnmpConfigDelV3HostMessage->getHostUC();
		string      userName = pSnmpServerSnmpConfigDelV3HostMessage->getUsername();

		FILE *fp = NULL;
		// Added for testing purpose to simulate lastconfig replay usecase
        if (pSnmpServerSnmpConfigDelV3HostMessage->getIsALastConfigReplay() == false 
					&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "triggered last replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigDelV3HostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigDelV3HostMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting snmp V3 host operation failed on one or more of the connected nodes.");
            else {
                trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting snmp V3 host operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigV3HostWorker::delV3HostSTCCB", errorMessage);
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
            V3HostsManagedObject    *pV3HostManagedObject 	= NULL;
            SnmpUserManagedObject   *pUserObject            = NULL;
            WaveManagedObjectSynchronousQueryContext *syncQueryContext =
                new WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (userName, "username"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +userName);
                pSnmpServerSnmpConfigDelV3HostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_USER_ERROR);
		        // clean up allocated messages
        		SnmpServerIntSnmpConfigDelV3HostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigDelV3HostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		        delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigDelV3HostMessage);
                delete syncQueryContext;
                delete pWaveSendToClusterContext;
                return;
            }

            pUserObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsRollback)[0]);
            delete pResultsRollback;
            delete syncQueryContext;

            ObjectId userOIRB = pUserObject->getObjectId();

            WaveManagedObjectSynchronousQueryContext *syncQueryV3Host = new 
                    WaveManagedObjectSynchronousQueryContext(V3HostsManagedObject::getClassName());
            syncQueryV3Host->addAndAttribute (new AttributeHostUC(&ipVal, true, "hostip", snmp_server_ip));
            syncQueryV3Host->addAndAttribute (new AttributeObjectIdAssociation(
                        &userOIRB,"username", SnmpUserManagedObject::getClassName ()));

            vector<WaveManagedObject *> *pResultsV3Host = querySynchronously (syncQueryV3Host);

            if ((NULL == pResultsV3Host) || (pResultsV3Host->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such V3host managed object");
                pSnmpServerSnmpConfigDelV3HostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_HOSTNAME_ERROR);
		        // clean up allocated messages
        		SnmpServerIntSnmpConfigDelV3HostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigDelV3HostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		        delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigDelV3HostMessage);
                delete syncQueryV3Host;
                delete pWaveSendToClusterContext;
                delete pUserObject;
                return;
            }
            pV3HostManagedObject = dynamic_cast<V3HostsManagedObject *> ((*pResultsV3Host)[0]);
            delete pResultsV3Host;
            delete syncQueryV3Host;	

            startTransaction ();
            delete pV3HostManagedObject;
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO,
                    "SNMP V3Host Delete MessageHandler:Successful Cluster-Wide committ");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                        "SNMP V3Host Delete MessageHandler:Commiting configuration failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
            delete pUserObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigDelV3HostIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigDelV3HostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigDelV3HostMessage->setMessageString (errorMessage);
		pSnmpServerSnmpConfigDelV3HostMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigDelV3HostMessage);
    }
}

