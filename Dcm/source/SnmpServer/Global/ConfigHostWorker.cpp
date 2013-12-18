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
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"      
#include "DcmResourceIds.h"

#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/ConfigHostWorker.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigAddHostMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigDelHostMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigChangeHostMessage.h"
#include "SnmpServer/Global/CommunityManagedObject.h"
#include "SnmpServer/Global/SnmpUserManagedObject.h"
#include "SnmpServer/Global/SystemGroupManagedObject.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigChangeHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigDelHostIntMessage.h"
#include "SnmpServer/Local/SNMPLocalBootMessage.h"
#include "SnmpServer/Global/HostsManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeHostUC.h"

#include "snmp/snmplib.h"

#include "AAA/Global/AAASequencerContexts.h"

namespace DcmNs
{

    ConfigHostWorker::ConfigHostWorker ( SnmpServerObjectManager *pSnmpServerObjectManager)
        : WaveWorker  (pSnmpServerObjectManager)
    {
        HostsManagedObject    HostsManagedObject    (pSnmpServerObjectManager);
        HostsManagedObject.setupOrm ();
        addManagedClass (HostsManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERSNMPCONFIGADDHOST, reinterpret_cast<PrismMessageHandler> (&ConfigHostWorker::SnmpServerSnmpConfigAddHostMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGDELHOST, reinterpret_cast<PrismMessageHandler> (&ConfigHostWorker::SnmpServerSnmpConfigDelHostMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGCHANGEHOST, reinterpret_cast<PrismMessageHandler> (&ConfigHostWorker::SnmpServerSnmpConfigChangeHostMessageHandler));
    }

    ConfigHostWorker::~ConfigHostWorker ()
    {
    }

    PrismMessage  *ConfigHostWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERSNMPCONFIGADDHOST :
                pPrismMessage = new SnmpServerSnmpConfigAddHostMessage ();
                break;
            case SNMPSERVERSNMPCONFIGDELHOST :
                pPrismMessage = new SnmpServerSnmpConfigDelHostMessage ();
                break;
            case SNMPSERVERSNMPCONFIGCHANGEHOST :
                pPrismMessage = new SnmpServerSnmpConfigChangeHostMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigHostWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((HostsManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new HostsManagedObject(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "ConfigHostWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }


	void ConfigHostWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{

		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        	SystemGroupManagedObject *pSysGroupMO       = NULL;
        	WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
        	if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || 
			(WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason))
        	{
            		trace (TRACE_LEVEL_INFO, "SNMPHostworker::install: First time boot");
            		startTransaction ();
		    	pSysGroupMO = new SystemGroupManagedObject
			(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()),
			string ("Field Support."), string ("End User Premise."), string("Brocade VDX Switch."));
            		status = commitTransaction ();
			if (status == FRAMEWORK_SUCCESS)
				status = WAVE_MESSAGE_SUCCESS;
			    else {
				trace (TRACE_LEVEL_ERROR, "SNMPHostworker::install:Commit failed");
				status = WAVE_MESSAGE_ERROR;
			}
            		delete pSysGroupMO;
            		trace (TRACE_LEVEL_INFO, string("SNMPHostworker::install..Done") +status);

		}
/*
        UI32                            i           = 0;
        CommunityManagedObject *pCommObject[MAX_COMMUNITY_ENTRIES];
        SnmpUserManagedObject *pUserObject[MAX_USM_ENTRIES];
        SystemGroupManagedObject *pSysGroupMO       = NULL;
        string CommString[]= {"Secret C0de", "OrigEquipMfr", "private",
                            "public", "common", "ConvergedNetwork"};
        int    CommAccess[]= {1, 1, 1, 0, 0, 0};
        string    userAccess[]= {"snmpadmin", "snmpadmin", "snmpadmin",
                            "snmpuser", "snmpuser", "snmpuser"};
        string userString[]= {"snmpadmin1", "snmpadmin2", "snmpadmin3",
                            "snmpuser1", "snmpuser2", "snmpuser3"};
 
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

         trace (TRACE_LEVEL_INFO, "SNMPHostworker::install: BootReasonCode : " + bootReason);
        if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) || (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason))
        {
            trace (TRACE_LEVEL_INFO, "SNMPHostworker::install: First time boot");
            startTransaction ();
            for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
                pCommObject[i] = new CommunityManagedObject
                    (dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()),
                    CommString[i], (Access) CommAccess[i]);
                pUserObject[i] = new SnmpUserManagedObject
                    (dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()),
                    userString[i], userAccess[i], (AuthType) 2, "", (PrivType) 1, "");
 
            }

            pSysGroupMO = new SystemGroupManagedObject
                (dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()),
                string ("Field Support."), string ("End User Premise."));
            status = commitTransaction ();
            if (status == FRAMEWORK_SUCCESS)
                status = WAVE_MESSAGE_SUCCESS;
            else {
                trace (TRACE_LEVEL_ERROR, "SNMPHostworker::install:Commit failed");
                status = WAVE_MESSAGE_ERROR;
            }

			for (i = 0; i < MAX_COMMUNITY_ENTRIES; i++) {
                delete pCommObject[i];
                delete pUserObject[i];
            }
            delete pSysGroupMO;

            trace (TRACE_LEVEL_INFO, string("SNMPHostworker::install..Done") +status);
        }
*/
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
	}


	void ConfigHostWorker::installDefaults ()
	{
		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        UI32                            i           = 0;
        CommunityManagedObject *pCommObject[DEFAULT_COMMUNITY_ENTRIES];
        SnmpUserManagedObject *pUserObject[MAX_USM_ENTRIES];
        string CommString[]= {"Secret C0de", "OrigEquipMfr", "private",
                            "public", "common", "ConvergedNetwork" };
        int    CommAccess[]= {1, 1, 1, 0, 0, 0};

        string    userAccess[]= {"snmpadmin", "snmpadmin", "snmpadmin",
                            "snmpuser", "snmpuser", "snmpuser"};
        string userString[]= {"snmpadmin1", "snmpadmin2", "snmpadmin3",
                            "snmpuser1", "snmpuser2", "snmpuser3"};
 		int tSize = 0;
		/*
		 * Read community MO from DB
		 * If no MO's found (size is 0), allow this to update the DB
		 * If size is non-zero, return it as its already installed defaults
		 */

		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new 
			WaveManagedObjectSynchronousQueryContext(
			CommunityManagedObject::getClassName());
		vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

		tSize = pResults->size();
		if (tSize > 0) {
			trace (TRACE_LEVEL_INFO, 
					string ("SNMPHostworker::installDefaults: Its not the first time, return it :") +tSize);
			return;
		}
         trace (TRACE_LEVEL_INFO, "SNMPHostworker::installDefaults:  ");
         trace (TRACE_LEVEL_INFO, "SNMPHostworker::installDefaults: First time boot");
         startTransaction ();
         for (i = 0; i < DEFAULT_COMMUNITY_ENTRIES; i++) {
             pCommObject[i] = new CommunityManagedObject
                 (dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()),
                 CommString[i], (Access) CommAccess[i]);
         }
         for (i = 0; i < MAX_USM_ENTRIES; i++) {
             pUserObject[i] = new SnmpUserManagedObject
                 (dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()),
                 userString[i], userAccess[i], AuthNone, "", PrivNone, "", false);
		}
           status = commitTransaction ();
            if (status == FRAMEWORK_SUCCESS)
                status = WAVE_MESSAGE_SUCCESS;
            else {
                trace (TRACE_LEVEL_ERROR, "SNMPHostworker::installDefaults:Commit failed");
                status = WAVE_MESSAGE_ERROR;
            }

			for (i = 0; i < DEFAULT_COMMUNITY_ENTRIES; i++) {
                delete pCommObject[i];
            }
         for (i = 0; i < MAX_USM_ENTRIES; i++) {
                delete pUserObject[i];
			}

            trace (TRACE_LEVEL_INFO, string("SNMPHostworker::installDefaults..Done") +status);
	}

/*
*	This function is the Global handler to add new v1 or v2c host.
*	Inputs: 	IPAddress, Community string added by user, optional version, Port number.
*	Functions: 	Sends message to Local Add Host handler, if success commit the DB with new host.
*	Error Case:	Message sending to Local failed or commit to DB failed.	
*/

    void  ConfigHostWorker::SnmpServerSnmpConfigAddHostMessageHandler( SnmpServerSnmpConfigAddHostMessage *pSnmpServerSnmpConfigAddHostMessage)
    {
		int v1hostSize = 0, i = 0, version1 = 0, version2c = 1;
		int ComExist = 0, retSize = 0, isLastCmd = 0;
		string ipVal;
		HostUC ipHost;
		ObjectId ObjId;

		trace (TRACE_LEVEL_INFO, "Entering SNMP Host Add handler");
			if (pSnmpServerSnmpConfigAddHostMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Add Host Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;

		string commStr   = pSnmpServerSnmpConfigAddHostMessage->getCommunity();
		HostUC ipAddr = pSnmpServerSnmpConfigAddHostMessage->getHostUC();
		UI32 udpPort = pSnmpServerSnmpConfigAddHostMessage->getUdpPort();
		Version versionVal = pSnmpServerSnmpConfigAddHostMessage->getVersion();
		SevLevel sevLevel  = pSnmpServerSnmpConfigAddHostMessage->getSevLevel();

		trace (TRACE_LEVEL_INFO,
            	    string (" Add Host Global::Community: ") +commStr);

		/* Before adding any host entry check if the associated community is exist or not */	

		WaveManagedObjectSynchronousQueryContext *synchronousQueryCommunity = new 
			WaveManagedObjectSynchronousQueryContext(CommunityManagedObject::getClassName());

        vector<WaveManagedObject *> *pResultsCom = querySynchronously (synchronousQueryCommunity);

        if ((NULL == pResultsCom) || (pResultsCom->size() == 0)) {
            trace (TRACE_LEVEL_INFO, "Add Host Global:: No community MO");
        } else {
			retSize = pResultsCom->size();
			trace (TRACE_LEVEL_INFO,
            	    string (" Add Host Global::Community retSize : ") +retSize);
            for (i = 0; i < retSize; i ++) {
                CommunityManagedObject *pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResultsCom)[i]);
                if (pCommManagedObject == NULL) {
                        trace (TRACE_LEVEL_ERROR,
                        "Add Host Global:: pCommManagedObject dynamic_cast failed");
                }
                if (commStr.compare(pCommManagedObject->getCommunity()) == 0 ) {
					 ObjId = pCommManagedObject->getObjectId();
                     ComExist = 1;
                }
                delete pCommManagedObject;
            }
		}  
        delete pResultsCom;
        delete synchronousQueryCommunity;

	    if (!ComExist) {
            pSnmpServerSnmpConfigAddHostMessage->setCompletionStatus(WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
    	   	trace (TRACE_LEVEL_INFO, "Add Host Global:: Add Host Failed");
         	reply (pSnmpServerSnmpConfigAddHostMessage);
            return;
         }

		/* In case of last replay CLI configuration, check if the host
	     * is already present in the DB. If so, then return success without updating the DB.
	     */

		if (pSnmpServerSnmpConfigAddHostMessage->getIsALastConfigReplay() == true) {
	            trace (TRACE_LEVEL_INFO, "Add Host Global:: getIsALastConfigReplay");

				ipVal = ipAddr.getHostValue();
		
				WaveManagedObjectSynchronousQueryContext *syncQueryHost = new
    	        WaveManagedObjectSynchronousQueryContext(
        	    HostsManagedObject::getClassName());

				if (versionVal == v1) {
					syncQueryHost->addAndAttribute (new AttributeEnum((UI32 *)&version1, "version"));
				} else if (versionVal == v2c) {
					syncQueryHost->addAndAttribute (new AttributeEnum((UI32 *)&version2c, "version"));
				}
				vector<WaveManagedObject *> *pResultsHost = querySynchronously (syncQueryHost);
		
				if ((NULL == pResultsHost) || (pResultsHost->size() == 0)) {
	            	trace (TRACE_LEVEL_INFO, "Add Host Global::No v1/v2c host MO");
				} else {
					v1hostSize = pResultsHost->size();
					trace (TRACE_LEVEL_INFO, string ("Add Host Global::v1hostSize : ") +v1hostSize);
					for (i = 0; i < v1hostSize; i ++) {
        		        HostsManagedObject *pHostManagedObject = dynamic_cast
                		   <HostsManagedObject *> ((*pResultsHost)[i]);
		                if (pHostManagedObject == NULL) {
	    	                trace (TRACE_LEVEL_ERROR,
            	            "Add Host Global:: pHostManagedObject dynamic_cast failed");
                		}
						ipHost = pHostManagedObject->getHostUC();
	
						if ((ipVal.compare(ipHost.getHostValue()) == 0) &&
								(ObjId == pHostManagedObject->getCommunity())) { 
									isLastCmd = 1;
						}
						delete pHostManagedObject;
					}	
				}
	    	delete pResultsHost;
			delete syncQueryHost;

			if (isLastCmd == 1) {
				trace (TRACE_LEVEL_INFO, string("Add Host Global::host") +ipVal);
				 trace (TRACE_LEVEL_INFO, string ("Add Host Global:: v1/v2c Host community already exists: ") +commStr);	
				 pSnmpServerSnmpConfigAddHostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
	    	     reply(pSnmpServerSnmpConfigAddHostMessage);
            	 return;
			}
		} // End of getIsALastConfigReplay

        SnmpServerIntSnmpConfigAddHostIntMessage *pAddMessage = 
					new SnmpServerIntSnmpConfigAddHostIntMessage (ipAddr,versionVal, udpPort, commStr, sevLevel);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigHostWorker::addHostSTCCB), pSnmpServerSnmpConfigAddHostMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigHostWorker::addHostSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigAddHostMessage *pSnmpServerSnmpConfigAddHostMessage = 
                                (SnmpServerSnmpConfigAddHostMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS;
        string errorMessage;

		CommunityManagedObject *pCommManagedObject = NULL;
		HostsManagedObject     *pHostManagedObject = NULL;

		string      commStr    = pSnmpServerSnmpConfigAddHostMessage->getCommunity();
		HostUC		ipAddr     = pSnmpServerSnmpConfigAddHostMessage->getHostUC();
		UI32        udpPort    = pSnmpServerSnmpConfigAddHostMessage->getUdpPort();
		Version     versionVal = pSnmpServerSnmpConfigAddHostMessage->getVersion();
		SevLevel 	sevLevel   = pSnmpServerSnmpConfigAddHostMessage->getSevLevel();

		FILE *fp = NULL;
		// Added for testing purpose to trigger LastReplay CLI usecase
		if (pSnmpServerSnmpConfigAddHostMessage->getIsALastConfigReplay() == false 
						&& (fp = fopen("/tmp/unsync.txt", "r"))) {
 				trace (TRACE_LEVEL_INFO, "isLast replay event Triggered");
			fclose(fp);
  			system ("/sbin/reboot -f");
  			pSnmpServerSnmpConfigAddHostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
  			reply (pSnmpServerSnmpConfigAddHostMessage);
  			return;
 		}

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding snmp host operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Adding snmp host operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::addHostSTCCB", errorMessage);
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
		* Query community MO, get Object Id for passed community string.
		* Use this Id to query host MO and add the Id as community field in Host MO.
		*/

        if (!allFailed)
        {
            WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new 
                WaveManagedObjectSynchronousQueryContext(CommunityManagedObject::getClassName());
            synchronousQueryContext->addAndAttribute (new AttributeString (commStr, "community"));
            vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
            if ((NULL == pResults) || (pResults->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +commStr);
                pSnmpServerSnmpConfigAddHostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
				
				// clean up allocated messages
				SnmpServerIntSnmpConfigAddHostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigAddHostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigAddHostMessage);
                delete synchronousQueryContext;
                return;
            }

            pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResults)[0]);
           
            startTransaction ();
            pHostManagedObject = dynamic_cast<HostsManagedObject *>
                (createManagedObjectInstance(HostsManagedObject::getClassName()));

            pHostManagedObject->setVersion(versionVal);         
            pHostManagedObject->setCommunity(pCommManagedObject->getObjectId());
            pHostManagedObject->setHostUC(ipAddr);
            pHostManagedObject->setUdpPort (udpPort);
            pHostManagedObject->setSevLevel (sevLevel);

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

            delete synchronousQueryContext;
            delete pResults;
            delete pCommManagedObject;
            delete pHostManagedObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigAddHostIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigAddHostIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete pWaveSendToClusterContext;
        pSnmpServerSnmpConfigAddHostMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigAddHostMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigAddHostMessage);
    }

/*
*   This function is the Global handler to change attributes (version, port) of v1 or v2c host.
*   Inputs:     IPAddress, Community string of existing host, version and/or Port number.
*   Functions:  Sends message to Local Add Host handler, if success commit the DB with change.
*   Error Case: Message sending to Local failed or commit to DB failed.
*   On success: Return WAVE_MESSAGE_SUCCESS.
*/

    void  ConfigHostWorker::SnmpServerSnmpConfigChangeHostMessageHandler( SnmpServerSnmpConfigChangeHostMessage 
            *pSnmpServerSnmpConfigChangeHostMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SNMP Change host message handler");
		if (pSnmpServerSnmpConfigChangeHostMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Change Host Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
		HostUC		ipAddr   = pSnmpServerSnmpConfigChangeHostMessage->getHostUC();
		int         udpPort  = -1;
		udpPort              = pSnmpServerSnmpConfigChangeHostMessage->getUdpPort();
		string commStr       = pSnmpServerSnmpConfigChangeHostMessage->getCommunity();
		Version versionVal   = pSnmpServerSnmpConfigChangeHostMessage->getVersion();
		SevLevel sevLevel  	 = pSnmpServerSnmpConfigChangeHostMessage->getSevLevel();

        /* Validate the version and port number */
		if ((versionVal != v1) && (versionVal != v2c))
		{
			trace (TRACE_LEVEL_INFO, "SNMP Hostworker: Invalid version " + versionVal);
			pSnmpServerSnmpConfigChangeHostMessage->setCompletionStatus
                                (WRC_SNMP_INVALID_VERSION_ERROR);
            reply (pSnmpServerSnmpConfigChangeHostMessage);
            return;
		}

		if (udpPort == -1) {
			trace (TRACE_LEVEL_INFO, string ("SNMP Hostworker: Invalid port") +udpPort);
			pSnmpServerSnmpConfigChangeHostMessage->setCompletionStatus
                                (WRC_SNMP_INVALID_PORT_NUMBER);
            reply (pSnmpServerSnmpConfigChangeHostMessage);
            return;
		}

    	SnmpServerIntSnmpConfigChangeHostIntMessage *pAddMessage =
            new SnmpServerIntSnmpConfigChangeHostIntMessage (ipAddr,versionVal,udpPort, commStr, sevLevel);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigHostWorker::changeHostSTCCB),
                        pSnmpServerSnmpConfigChangeHostMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigHostWorker::changeHostSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;

        SnmpServerSnmpConfigChangeHostMessage *pSnmpServerSnmpConfigChangeHostMessage = 
                        (SnmpServerSnmpConfigChangeHostMessage *)pWaveSendToClusterContext->getPCallerContext();

        string errorMessage;
        bool        allFailed    = false;
        ResourceId  status       = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId  fstatus      = WAVE_MESSAGE_SUCCESS;
 		HostUC		ipAddr       = pSnmpServerSnmpConfigChangeHostMessage->getHostUC();
		int         udpPort      = -1;
		udpPort                  = pSnmpServerSnmpConfigChangeHostMessage->getUdpPort();
		string      commStr      = pSnmpServerSnmpConfigChangeHostMessage->getCommunity();
		Version     versionVal   = pSnmpServerSnmpConfigChangeHostMessage->getVersion();
		SevLevel 	sevLevel  	 = pSnmpServerSnmpConfigChangeHostMessage->getSevLevel();

		HostsManagedObject       *pHostObject         = NULL;
		CommunityManagedObject   *pCommObject         = NULL;

		 // Added for testing purpose to trigger LastReplay CLI usecase
        FILE *fp = NULL;
        if (pSnmpServerSnmpConfigChangeHostMessage->getIsALastConfigReplay() == false
                            && (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered isLast Replay ");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigChangeHostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigChangeHostMessage);
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
                new WaveManagedObjectSynchronousQueryContext(CommunityManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (commStr, "community"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +commStr);
                pSnmpServerSnmpConfigChangeHostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
        		// clean up allocated messages
				SnmpServerIntSnmpConfigChangeHostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigChangeHostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigChangeHostMessage);
                delete syncQueryContext;
                return;
            }

            pCommObject = dynamic_cast<CommunityManagedObject *> ((*pResultsRollback)[0]);
            delete pResultsRollback; 
            ObjectId commOIRB = pCommObject->getObjectId();
            delete pCommObject;

            WaveManagedObjectSynchronousQueryContext *syncQueryHost = new 
                    WaveManagedObjectSynchronousQueryContext(HostsManagedObject::getClassName());
            syncQueryHost->addAndAttribute (new AttributeHostUC(&ipAddr, true, "ip", snmp_server_ip));
            syncQueryHost->addAndAttribute (new AttributeObjectIdAssociation( 
                    &commOIRB,"community", CommunityManagedObject::getClassName ()));

            vector<WaveManagedObject *> *pResultsHost = querySynchronously (syncQueryHost);

            if ((NULL == pResultsHost) || (pResultsHost->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such host managed object");
                pSnmpServerSnmpConfigChangeHostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_HOSTNAME_ERROR);
				// clean up allocated messages		
				SnmpServerIntSnmpConfigChangeHostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigChangeHostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigChangeHostMessage);
                delete syncQueryHost;
                delete syncQueryContext;
                return;
            }

            pHostObject = dynamic_cast<HostsManagedObject *> ((*pResultsHost)[0]);
            delete pResultsHost;
            delete syncQueryHost;
            delete syncQueryContext;
            /*
            * By now, messages to local handlers and backend sets have returned success.
            * Prepare to commit to DB.
            * Query Community table (MO), get ObjectId for community string passed to this handler.
            * Use this Id to query Host MO and change the attribute of a distinct row.
            */

            startTransaction ();     
            updateWaveManagedObject (pHostObject);
            if ((v1 <= versionVal) && (versionVal <= v2c))
            {
                trace (TRACE_LEVEL_INFO, "SNMP Host worker: Setting elementName " + versionVal);
                pHostObject->setVersion (versionVal);
            }

            if (udpPort != -1)
                pHostObject->setUdpPort (udpPort);
			
			if ((sevLevel >= None) && (sevLevel <= Critical)) 
				pHostObject->setSevLevel(sevLevel);

            fstatus = commitTransaction ();

            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO,
                    "SNMP Host Global handler: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "SNMP Host Global handler : Commiting failed.Status:"
                            + FrameworkToolKit::localize (fstatus));
            }
            delete pHostObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigChangeHostIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigChangeHostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
        pSnmpServerSnmpConfigChangeHostMessage->setMessageString (errorMessage);
		pSnmpServerSnmpConfigChangeHostMessage->setCompletionStatus (status);
		reply (pSnmpServerSnmpConfigChangeHostMessage);

    }

	/*
	*   This function is the Global handler to delete v1 or v2c host.
	*   Inputs:     IPAddress, Community string of existing host.
	*   Functions:  Sends message to Local delete Host handler, if success commit the DB and removes host.
	*   Error Case: Message sending to Local failed or commit to DB failed.
	* 	On success: Return WAVE_MESSAGE_SUCCESS.
	*/

    void  ConfigHostWorker::SnmpServerSnmpConfigDelHostMessageHandler( SnmpServerSnmpConfigDelHostMessage *pSnmpServerSnmpConfigDelHostMessage)
    {
		int i = 0, v1hostSize = 0, v2hostSize = 0, resSize = 0;
		int isExist = 0, version1 = 0, version2c = 1;
		string ipStr;
		HostUC ipHostv1, ipHostv2;
		ObjectId ObjId;

		if (pSnmpServerSnmpConfigDelHostMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Del Host Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
        HostUC		ipVal   = pSnmpServerSnmpConfigDelHostMessage->getHostUC();
		string      commStr = pSnmpServerSnmpConfigDelHostMessage->getCommunity();
		
		trace (TRACE_LEVEL_INFO,
            	    string (" Del Host Global::Community: ") +commStr);

	/* In case of last replay CLI configuration, check if the host entry string
     * is already deleted in the DB. If so, then return success without updating the DB.
     */

	if (pSnmpServerSnmpConfigDelHostMessage->getIsALastConfigReplay() == true) {
        trace (TRACE_LEVEL_INFO, "Del Host Global:: getIsALastConfigReplay");
		ipStr = ipVal.getHostValue();
		WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new
        		    WaveManagedObjectSynchronousQueryContext(
            CommunityManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

        if ((NULL == pResults) || (pResults->size() == 0)) {
            trace (TRACE_LEVEL_INFO,
                "Del Host Global:: No community MO");
        } else {
            resSize = pResults->size();
			trace (TRACE_LEVEL_INFO, string("Del Host Global::  Community resSize") +resSize);
            for (i = 0; i < resSize; i ++) {
		        CommunityManagedObject *pCommManagedObject = dynamic_cast<CommunityManagedObject *> ((*pResults)[i]);              
        	    if (pCommManagedObject == NULL) {
                    trace (TRACE_LEVEL_ERROR,
                        "Del Host Global:: pCommManagedObject dynamic_cast failed");
                }
				if (commStr.compare(pCommManagedObject->getCommunity()) == 0) {
	                ObjId = pCommManagedObject->getObjectId();
				}
                delete pCommManagedObject;
         	}           

			WaveManagedObjectSynchronousQueryContext *syncQueryHost = new
            WaveManagedObjectSynchronousQueryContext(
            HostsManagedObject::getClassName());
	
    	    syncQueryHost->addAndAttribute (new AttributeEnum((UI32 *)&version1, "version"));
	        vector<WaveManagedObject *> *pResultsHost = querySynchronously (syncQueryHost);

    	    if ((NULL == pResultsHost) || (pResultsHost->size() == 0))
        	{
            	trace (TRACE_LEVEL_INFO, "Del Host Global::No host MO");
	        } else {
    		    v1hostSize = pResultsHost->size();
				trace (TRACE_LEVEL_INFO, string("Del Host Global::v1hostSize:") +v1hostSize);
        		for (i = 0; i < v1hostSize; i ++) {
                	HostsManagedObject *pHostManagedObject = dynamic_cast
                    	<HostsManagedObject *> ((*pResultsHost)[i]);
	                if (pHostManagedObject == NULL) {
    	                trace (TRACE_LEVEL_ERROR,
                        "Del Host Global:: pHostv1ManagedObject dynamic_cast failed");
        	        }
					ipHostv1 = pHostManagedObject->getHostUC();
		        	if ((ipStr.compare(ipHostv1.getHostValue()) == 0) &&
							ObjId == pHostManagedObject->getCommunity()) {
								isExist = 1;
                    	}
					delete pHostManagedObject;
				}
			}
        	delete pResultsHost;
	        delete syncQueryHost;
		
			if (!isExist) {
			WaveManagedObjectSynchronousQueryContext *syncQueryHostv2 = new
    	        WaveManagedObjectSynchronousQueryContext(
        	    HostsManagedObject::getClassName());

	        syncQueryHostv2->addAndAttribute (new AttributeEnum((UI32 *)&version2c, "version"));
    	    vector<WaveManagedObject *> *pResultsHostv2 = querySynchronously (syncQueryHostv2);

    	    if ((NULL == pResultsHostv2) || (pResultsHostv2->size() == 0))
	        {
        	    trace (TRACE_LEVEL_INFO, "Del Host Global::No host v2c MO");		
			} else {
    	    	v2hostSize = pResultsHostv2->size();
				trace (TRACE_LEVEL_INFO, string("Del Host Global::v2hostSize: ") +v2hostSize);
	    	    for (i = 0; i < v2hostSize; i ++) {
            	    HostsManagedObject *pHostv2ManagedObject = dynamic_cast
                    	<HostsManagedObject *> ((*pResultsHostv2)[i]);
                	if (pHostv2ManagedObject == NULL) {
                    trace (TRACE_LEVEL_ERROR,
                        "Del Host Global:: pHostManagedObject dynamic_cast failed");
                	}
					ipHostv2 = pHostv2ManagedObject->getHostUC();

			        if ((ipStr.compare(ipHostv2.getHostValue()) == 0) &&
						(ObjId == pHostv2ManagedObject->getCommunity())) {
                        	    isExist = 1;
        		    }
					delete pHostv2ManagedObject;
				}
			}
	        delete pResultsHostv2;
			delete syncQueryHostv2;
		}
		}
        delete pResults;              
		delete synchronousQueryContext;
		
		if (isExist == 0) {
			trace (TRACE_LEVEL_INFO, string("Del Host Global::host v1/v2c already deleted:") 
				+ipStr);
			trace (TRACE_LEVEL_INFO, string("Del Host Global::host community v1/v2c already deleted:")
				+commStr);
                pSnmpServerSnmpConfigDelHostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                reply (pSnmpServerSnmpConfigDelHostMessage);
                return;
		}
	} // End of isLastReplay

    	SnmpServerIntSnmpConfigDelHostIntMessage *pAddMessage =
				new SnmpServerIntSnmpConfigDelHostIntMessage (ipVal,commStr);
		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigHostWorker::delHostSTCCB), pSnmpServerSnmpConfigDelHostMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pAddMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigHostWorker::delHostSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;

        SnmpServerSnmpConfigDelHostMessage *pSnmpServerSnmpConfigDelHostMessage = 
                                (SnmpServerSnmpConfigDelHostMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool allFailed       = false;
        string errorMessage;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS;
        HostUC		ipVal    = pSnmpServerSnmpConfigDelHostMessage->getHostUC();
		string commStr       = pSnmpServerSnmpConfigDelHostMessage->getCommunity();

		HostsManagedObject       *pHostManagedObject 	= NULL;
		CommunityManagedObject   *pCommObject           = NULL;

		// Added for testing purpose to trigger LastReplay CLI usecase
		FILE *fp = NULL;
		if (pSnmpServerSnmpConfigDelHostMessage->getIsALastConfigReplay() == false 
							&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered isLast Replay ");
			fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigDelHostMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigDelHostMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting snmp host operation failed on one or more of the connected nodes.");
            else {
                trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting snmp host operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigHostWorker::delHostSTCCB", errorMessage);
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
                pSnmpServerSnmpConfigDelHostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_COMMUNITY_STR_ERROR);
				
                // clean up allocated messages
                SnmpServerIntSnmpConfigDelHostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigDelHostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
                delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigDelHostMessage);
                delete syncQueryContext;
                return;
            }

            pCommObject = dynamic_cast<CommunityManagedObject *> ((*pResultsRollback)[0]);
            delete pResultsRollback;
            delete syncQueryContext;
            ObjectId commOIRB = pCommObject->getObjectId();
            delete pCommObject;

            WaveManagedObjectSynchronousQueryContext *syncQueryHost = new 
                    WaveManagedObjectSynchronousQueryContext(HostsManagedObject::getClassName());
            syncQueryHost->addAndAttribute (new AttributeHostUC(&ipVal, true, "ip", snmp_server_ip));
            syncQueryHost->addAndAttribute (new AttributeObjectIdAssociation(
                        &commOIRB,"community", CommunityManagedObject::getClassName ()));

            vector<WaveManagedObject *> *pResultsHost = querySynchronously (syncQueryHost);

            if ((NULL == pResultsHost) || (pResultsHost->size() == 0))
            {
                trace (TRACE_LEVEL_ERROR, "No such host managed object");
                pSnmpServerSnmpConfigDelHostMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_HOSTNAME_ERROR);
				
				// clean up allocated messages
				SnmpServerIntSnmpConfigDelHostIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigDelHostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigDelHostMessage);
                delete syncQueryHost;
                return;
            }
            pHostManagedObject = dynamic_cast<HostsManagedObject *> ((*pResultsHost)[0]);
            delete pResultsHost;
            delete syncQueryHost;	

            /* By this time local message handler messages have returned success
            ** Prepare to commit to DB.
            ** Query Community table (MO) and get corresponding ObjectId. Use this ObjectId
            ** to query for distinct host.
            ** On success, commit to DB, remove the host.
            */

            startTransaction ();
            delete pHostManagedObject;
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO,
                    "SNMP Host Delete MessageHandler:Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                        "SNMP Host Delete MessageHandler:Commiting configuration failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigDelHostIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigDelHostIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete (pWaveSendToClusterContext);
		pSnmpServerSnmpConfigDelHostMessage->setCompletionStatus (status);
		pSnmpServerSnmpConfigDelHostMessage->setMessageString (errorMessage);
        reply (pSnmpServerSnmpConfigDelHostMessage);
    }

void ConfigHostWorker::configReplayEnd (WaveAsynchronousContextForConfigReplayEnd *pWaveAsynchronousContextForConfigReplayEnd)
{
    trace (TRACE_LEVEL_INFO, "ConfigHostWorker::configReplayEnd : Start ");

    if ((FrameworkToolKit::isConfigReplayInProgress() == true) &&
		(DcmToolKit::isStartupConfigFileIsLinkedToDefaultConfig() == true)) {
        
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

    	trace (TRACE_LEVEL_INFO, "ConfigHostWorker::configReplayEnd : Default <> configuration");
    	installDefaults();

   	    trace (TRACE_LEVEL_INFO, "ConfigHostWorker::configReplayEnd : Send to Local OM ");

    	SnmpServerIntConfigureDefaultsMessage *pDefMessage = 
	     		new SnmpServerIntConfigureDefaultsMessage ();
    	status = sendSynchronously (pDefMessage, FrameworkToolKit::getThisLocationId());
    	trace (TRACE_LEVEL_INFO, "ConfigHostWorker::configReplayEnd : Message Send ");
	    if (WAVE_MESSAGE_SUCCESS == status) {
		    status = pDefMessage->getCompletionStatus(); 
    	    if (WAVE_MESSAGE_SUCCESS == status)
               	trace (TRACE_LEVEL_INFO,
				"SNMP Configure Defaults Worker::backend data : Successful Commit.");
		    else 
               	trace (TRACE_LEVEL_ERROR,
				"SNMP Configure Defaults Worker::backend data : Commit failed : "
				  +FrameworkToolKit::localize (status));
	    }
	    else 		
            	trace (TRACE_LEVEL_ERROR,
				"SNMP Configure Defaults Worker::backend data : Commit failed : "
				  +FrameworkToolKit::localize (status));
        
	    delete pDefMessage;
     }

    trace (TRACE_LEVEL_INFO, "ConfigHostWorker::configReplayEnd : End ");

    pWaveAsynchronousContextForConfigReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForConfigReplayEnd->callback ();
}

}

