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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIds.h"

#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/ConfigUserWorker.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigSetUserMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigRemoveUserMessage.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigAddUserMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetUserIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddUserIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveUserIntMessage.h"
#include "SnmpServer/Global/SnmpUserManagedObject.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/V3HostsManagedObject.h"
#include <snmp/snmplib.h>
#include "AAA/Global/AAASequencerContexts.h"
#include "SnmpAuthCrypto.h"

namespace DcmNs
{

    ConfigUserWorker::ConfigUserWorker ( SnmpServerObjectManager *pSnmpServerObjectManager)
        : WaveWorker  (pSnmpServerObjectManager)
    {
        SnmpUserManagedObject    SnmpUserManagedObject    (pSnmpServerObjectManager);
        SnmpUserManagedObject.setupOrm ();
        addManagedClass (SnmpUserManagedObject::getClassName (), this);
        addOperationMap (SNMPSERVERSNMPCONFIGSETUSER, reinterpret_cast<PrismMessageHandler> (&ConfigUserWorker::SnmpServerSnmpConfigSetUserMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGREMOVEUSER, reinterpret_cast<PrismMessageHandler> (&ConfigUserWorker::SnmpServerSnmpConfigRemoveUserMessageHandler));
        addOperationMap (SNMPSERVERSNMPCONFIGADDUSER, reinterpret_cast<PrismMessageHandler> (&ConfigUserWorker::SnmpServerSnmpConfigAddUserMessageHandler));
    }

    ConfigUserWorker::~ConfigUserWorker ()
    {
    }

    PrismMessage  *ConfigUserWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SNMPSERVERSNMPCONFIGSETUSER :
                pPrismMessage = new SnmpServerSnmpConfigSetUserMessage ();
                break;
            case SNMPSERVERSNMPCONFIGREMOVEUSER :
                pPrismMessage = new SnmpServerSnmpConfigRemoveUserMessage ();
                break;
            case SNMPSERVERSNMPCONFIGADDUSER :
                pPrismMessage = new SnmpServerSnmpConfigAddUserMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ConfigUserWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((SnmpUserManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SnmpUserManagedObject(dynamic_cast<SnmpServerObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "ConfigUserWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  ConfigUserWorker::SnmpServerSnmpConfigAddUserMessageHandler( SnmpServerSnmpConfigAddUserMessage *pSnmpServerSnmpConfigAddUserMessage)
    {
		int resUserSize = 0, i = 0;
		if (pSnmpServerSnmpConfigAddUserMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Add User Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		
		vector<UI32> locations;
        string userName   	= pSnmpServerSnmpConfigAddUserMessage->getUsername();
        string groupName   	= pSnmpServerSnmpConfigAddUserMessage->getGroupname();
        AuthType authType   = pSnmpServerSnmpConfigAddUserMessage->getAuth();
        string authPswd   	= pSnmpServerSnmpConfigAddUserMessage->getAuthpswd();
        PrivType privType   = pSnmpServerSnmpConfigAddUserMessage->getPriv();
        string privPswd   	= pSnmpServerSnmpConfigAddUserMessage->getPrivpswd();
        bool encpswdflag = pSnmpServerSnmpConfigAddUserMessage->getEncpswdflag();
		unsigned char *base64AuthPaswd = NULL;
		unsigned char *base64PrivPaswd = NULL;
		char authPaswd[256];
		char privPaswd[256];
		int encLen = 0, isLastCmd = 0;
        
		if (encpswdflag)
		{
			base64AuthPaswd = SnmpAuthCrypto::base64_decode((unsigned char *)authPswd.c_str(),&encLen);
			if (!base64AuthPaswd)
			{
            	pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_ERROR);
            	reply (pSnmpServerSnmpConfigAddUserMessage);
				return;	
			}
			memset(authPaswd, 0, 256);
			strncpy(authPaswd,(char *)base64AuthPaswd,encLen);
			authPaswd[encLen+1] = '\0';
			authPswd.assign(authPaswd); 

			encLen = 0;
			base64PrivPaswd = SnmpAuthCrypto::base64_decode((unsigned char *)privPswd.c_str(),&encLen);
			if (!base64PrivPaswd)
			{
            	pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_ERROR);
            	reply (pSnmpServerSnmpConfigAddUserMessage);
				return;	
			}
			memset(privPaswd, 0, 256);
			strncpy(privPaswd,(char *)base64PrivPaswd,encLen);
			privPaswd[encLen+1] = '\0';
			privPswd.assign(privPaswd);
			
			if (base64AuthPaswd != NULL)
			{
				free(base64AuthPaswd);
			}
			if (base64PrivPaswd != NULL)
			{
				free(base64PrivPaswd);
			}
		}
        if (((authType == AuthNone) && (authPswd.length() > 0)) || 
				((privType == PrivNone) && (privPswd.length() > 0)))
        {
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_SECURITYLEVEL);
            reply (pSnmpServerSnmpConfigAddUserMessage);
            return;
        }
        if ((authType != AuthNone) && ((authPswd.length() < 1) || (authPswd.length() > AGT_MAX_PSWD_SIZE) ))
        {
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_LEN_ERROR);
            reply (pSnmpServerSnmpConfigAddUserMessage);
            return;
        }

        if ((privType != PrivNone) && ((privPswd.length() < 1) || (privPswd.length() > AGT_MAX_PSWD_SIZE) ))
        {
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_LEN_ERROR);
            reply (pSnmpServerSnmpConfigAddUserMessage);
            return;
        }

        if ((authType == AuthNone) && (privType != PrivNone))
        {
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_SECURITYLEVEL);
            reply (pSnmpServerSnmpConfigAddUserMessage);
            return;
        }
        
        if ((groupName.compare("snmpadmin") != 0) && (groupName.compare("snmpuser") != 0) )
        {
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_GROUP_ERROR);
            reply (pSnmpServerSnmpConfigAddUserMessage);
			return;
		}

		if ((authType == AuthNone) && (encpswdflag))
		{
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_ERROR);
            reply (pSnmpServerSnmpConfigAddUserMessage);
            return;
		}

	/* In case of last replay CLI configuration, check if the userName 
     * is already present in the DB. If so, then return success without updating the DB.
     */

		if (pSnmpServerSnmpConfigAddUserMessage->getIsALastConfigReplay() == true) {	
            trace (TRACE_LEVEL_INFO,
                "Add User Global::getIsALastConfigReplay");
			WaveManagedObjectSynchronousQueryContext *syncQueryUser = new
            	WaveManagedObjectSynchronousQueryContext(
	            SnmpUserManagedObject::getClassName());
    	    vector<WaveManagedObject *> *pResultsUser = querySynchronously (syncQueryUser);
		
			if ((NULL == pResultsUser) || (pResultsUser->size() == 0)) {
    	        trace (TRACE_LEVEL_INFO,
        	        "Add User Global::No user MO");
			} else {
				resUserSize = pResultsUser->size();
				trace (TRACE_LEVEL_INFO,
            	    string (" Add User Global::resUserSize : ") +resUserSize);
				 for (i = 0; i < resUserSize; i ++) {
    			 	SnmpUserManagedObject *pUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsUser)[i]);
		            if (pUserManagedObject == NULL) {
        	            trace (TRACE_LEVEL_ERROR,
                    	    "Add User Global:: pUserManagedObject dynamic_cast failed");
            		 }
					if (userName.compare(pUserManagedObject->getUsername()) == 0) {
						isLastCmd = 1;
		       		}
				delete pUserManagedObject;
				}
			}
	        delete pResultsUser;
			delete syncQueryUser;

			if (isLastCmd == 1) {
				trace (TRACE_LEVEL_INFO,
                    	string ("Add User Global:: UserName already exists: ") +userName);
				pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
                reply(pSnmpServerSnmpConfigAddUserMessage);
				return;
			}	
		} // End of isLast Replay 

	    SnmpServerIntSnmpConfigAddUserIntMessage *pMessage =
					new SnmpServerIntSnmpConfigAddUserIntMessage (userName, groupName, authType, authPswd, privType, privPswd, encpswdflag);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigUserWorker::addUserSTCCB), pSnmpServerSnmpConfigAddUserMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext); 
    }

    void ConfigUserWorker::addUserSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigAddUserMessage *pSnmpServerSnmpConfigAddUserMessage = 
                                (SnmpServerSnmpConfigAddUserMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS;
        string errorMessage;
		char authPaswd[256];	
		char privPaswd[256];	
		unsigned char *base64AuthPaswd = NULL;
		unsigned char *base64PrivPaswd = NULL;
		int encLen=0;
		unsigned int authType1 = NO_AUTH_PROTOCOL;
		octetString* encAuthPswd = NULL;
		octetString* encPrivPswd = NULL;

        SnmpUserManagedObject *pUserManagedObject = NULL;
        string userName   	= pSnmpServerSnmpConfigAddUserMessage->getUsername();
        string groupName   	= pSnmpServerSnmpConfigAddUserMessage->getGroupname();
        AuthType authType   = pSnmpServerSnmpConfigAddUserMessage->getAuth();
        string authPswd   	= pSnmpServerSnmpConfigAddUserMessage->getAuthpswd();
        PrivType privType   = pSnmpServerSnmpConfigAddUserMessage->getPriv();
        string privPswd   	= pSnmpServerSnmpConfigAddUserMessage->getPrivpswd();
        bool encpswdflag = pSnmpServerSnmpConfigAddUserMessage->getEncpswdflag();

		FILE *fp = NULL;
		// Added for testing purpose to simulate last replay usecase
        if (pSnmpServerSnmpConfigAddUserMessage->getIsALastConfigReplay() == false 
						&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered Last replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigAddUserMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Adding snmp user operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Adding snmp user operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigUserWorker::addUserSTCCB", errorMessage);
                trace (TRACE_LEVEL_INFO, "Coming here" +errorMessage);
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
		* Use the input username and group (default snmpuser) to commit to db.
		*/
        if (!allFailed)
        {
            startTransaction ();
            pUserManagedObject = dynamic_cast<SnmpUserManagedObject *>(createManagedObjectInstance(SnmpUserManagedObject::getClassName()));

			authType1 = authType + 1;
			if (!encpswdflag) {
				memset(authPaswd, 0, 256);
				strncpy(authPaswd,authPswd.c_str(),authPswd.length());
				authPaswd[authPswd.length()+1] = '\0';
				encAuthPswd = encryptPasswd(authType1, authPaswd);

				if (encAuthPswd != NULL) {
					base64AuthPaswd = SnmpAuthCrypto::base64_encode(encAuthPswd->octet_ptr,encAuthPswd->length,&encLen);

					memset(authPaswd, 0, 256);
					strncpy(authPaswd,(char *)base64AuthPaswd,encLen);
					authPaswd[encLen+1] = '\0';
					authPswd.assign(authPaswd);

					if (encAuthPswd != NULL) {
						free(encAuthPswd);
					}

					if (privPswd.length() > 1 ) {
						memset(privPaswd, 0, 256);
						strncpy(privPaswd,privPswd.c_str(),privPswd.length());
						privPaswd[privPswd.length()+1] = '\0';
						encPrivPswd = encryptPasswd(authType1, privPaswd);

						encLen = 0;
						if (encPrivPswd != NULL) {
					 		base64PrivPaswd = SnmpAuthCrypto::base64_encode(encPrivPswd->octet_ptr,encPrivPswd->length,&encLen);
						}

						memset(privPaswd, 0, 256);
						strncpy(privPaswd,(char *)base64PrivPaswd,encLen);
						privPaswd[encLen+1] = '\0';
						privPswd.assign(privPaswd);
						if (encPrivPswd != NULL) {
							free(encPrivPswd);
						}
					}
				}
			}
			if (authType == AuthNone) {
				encpswdflag = false;
			} else {
				encpswdflag = true;
			}
            pUserManagedObject->setUsername(userName);
            pUserManagedObject->setGroupname(groupName);
            pUserManagedObject->setAuth(authType);
            pUserManagedObject->setAuthpswd(authPswd);
            pUserManagedObject->setPriv(privType);
            pUserManagedObject->setPrivpswd(privPswd);
            pUserManagedObject->setEncpswdflag(encpswdflag);

            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == fstatus)
            {
                trace (TRACE_LEVEL_INFO, 
                    "SNMP GlobalAdd User MessageHandler: Successful Cluster-Wide commit");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR,
                        "SNMP GlobalAdd User MessageHandler:Commiting failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
            delete pUserManagedObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigAddUserIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigAddUserIntMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete pWaveSendToClusterContext;
        pSnmpServerSnmpConfigAddUserMessage->setCompletionStatus (status);
        pSnmpServerSnmpConfigAddUserMessage->setMessageString (errorMessage);
        reply (pSnmpServerSnmpConfigAddUserMessage);
    }

    void  ConfigUserWorker::SnmpServerSnmpConfigRemoveUserMessageHandler( SnmpServerSnmpConfigRemoveUserMessage *pSnmpServerSnmpConfigRemoveUserMessage)
    {
		int i = 0, isExist = 0, retUserSize = 0, resV3Size = 0, chkV3Host = 0;
		ObjectId Objv3HostId;
		trace (TRACE_LEVEL_INFO, "Entering SNMP User remove handler");

		if (pSnmpServerSnmpConfigRemoveUserMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Remove User Global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}

		vector<UI32> locations;
		string userName   = pSnmpServerSnmpConfigRemoveUserMessage->getUsername();

		/* Before Deleting SNMP user check if any v3Host entry is associated with it */
		WaveManagedObjectSynchronousQueryContext *syncQueryContextUser = new
            WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryContextUser);

        if ((NULL == pResults) || (pResults->size() == 0)) {
			trace (TRACE_LEVEL_INFO, "Del User Global:: No User MO");
		} else {
			retUserSize = pResults->size();
			trace (TRACE_LEVEL_INFO,
            	    string (" Del User Global::retUserSize : ") +retUserSize);
			for (i = 0; i < retUserSize; i ++) {
            	SnmpUserManagedObject *pUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResults)[i]);
        	    if (pUserManagedObject == NULL) {
                    trace (TRACE_LEVEL_ERROR,
                           "Del User Global:: pUserManagedObject dynamic_cast failed");
                }
				if (userName.compare(pUserManagedObject->getUsername()) == 0) {
					Objv3HostId = pUserManagedObject->getObjectId();
					isExist = 1;
				}
				delete pUserManagedObject;
			}
		
		 	WaveManagedObjectSynchronousQueryContext *syncQueryV3Host = new
               WaveManagedObjectSynchronousQueryContext(V3HostsManagedObject::getClassName());

            vector<WaveManagedObject *> *pResultsV3Host = querySynchronously (syncQueryV3Host);

            if ((NULL == pResultsV3Host) || (pResultsV3Host->size() == 0)) {
				trace (TRACE_LEVEL_INFO,
	                  "Del User Global:: No v3 Host MO");
			} else {
				resV3Size = pResultsV3Host->size();
				trace (TRACE_LEVEL_INFO,
            	    string (" Del User Global::resV3Size : ") +resV3Size);
                for (i = 0; i < resV3Size; i ++) {
                   	V3HostsManagedObject *pV3HostManagedObject = dynamic_cast <V3HostsManagedObject *> ((*pResultsV3Host)[i]);
                    if (pV3HostManagedObject == NULL) {
                       	trace (TRACE_LEVEL_ERROR,
                               "Del User Host:: pV3HostManagedObject dynamic_cast failed");
                    }
					if (Objv3HostId == pV3HostManagedObject->getUsername()) {
						chkV3Host = 1;
					}
					delete pV3HostManagedObject;
				}
			}
			delete  pResultsV3Host;
			delete syncQueryV3Host;
		}
		delete pResults;
		delete syncQueryContextUser;

	if (chkV3Host) {
            pSnmpServerSnmpConfigRemoveUserMessage->setCompletionStatus (WRC_SNMP_USER_ASSOCIATED_ERROR);
			trace (TRACE_LEVEL_INFO, "Del User Global:: Delete User Failed");
            reply (pSnmpServerSnmpConfigRemoveUserMessage);
            return;
	}

	/* In case of last replay CLI configuration, check if the userName
     * is already deleted in the DB. If so, then return success without updating the DB.
     */

		if (pSnmpServerSnmpConfigRemoveUserMessage->getIsALastConfigReplay() == true) {
            trace (TRACE_LEVEL_INFO,
                "Remove User Global:: getIsALastConfigReplay");
	
		if (isExist == 0) {	
            trace (TRACE_LEVEL_INFO,
                    string ("Remove User Global:: UserName already exists: ") +userName);
            pSnmpServerSnmpConfigRemoveUserMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigRemoveUserMessage);
            return;
			}
		} // End of Last replay

        SnmpServerIntSnmpConfigRemoveUserIntMessage *pMessage =
					new SnmpServerIntSnmpConfigRemoveUserIntMessage (userName);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigUserWorker::delUserSTCCB), pSnmpServerSnmpConfigRemoveUserMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void  ConfigUserWorker::delUserSTCCB (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigRemoveUserMessage *pSnmpServerSnmpConfigRemoveUserMessage = 
                                (SnmpServerSnmpConfigRemoveUserMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS;
		string     userName  = pSnmpServerSnmpConfigRemoveUserMessage->getUsername();
        string     errorMessage;

        SnmpUserManagedObject *pUserManagedObject = NULL;

		FILE *fp = NULL;
		// Added for testing purpose to simulate last replay usecase
        if (pSnmpServerSnmpConfigRemoveUserMessage->getIsALastConfigReplay() == false 
				&& (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered Last replay event");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigRemoveUserMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigRemoveUserMessage);
            return;
        }

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Deleting snmp user operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Deleting snmp user operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigUserWorker::delUserSTCCB", errorMessage);
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
     
            WaveManagedObjectSynchronousQueryContext *syncQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (userName, "username"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) 
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +userName);
                pSnmpServerSnmpConfigRemoveUserMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_USER_ERROR);
				// clean up allocated messages
				SnmpServerIntSnmpConfigRemoveUserIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigRemoveUserIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
				delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigRemoveUserMessage);
                delete syncQueryContext;
                delete pWaveSendToClusterContext;
                return;
            }

            pUserManagedObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsRollback)[0]);
            delete pResultsRollback;
            delete syncQueryContext;

            startTransaction ();
            delete pUserManagedObject;
            fstatus = commitTransaction ();
            if (FRAMEWORK_SUCCESS == status)
            {
                trace (TRACE_LEVEL_INFO,
                        "SNMP UserDelete MessageHandler: Successful Cluster-Wide commit UserConfig");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, 
                        "SNMP User Delete MessageHandler:Commiting Community config failed.Status:"
                        + FrameworkToolKit::localize (fstatus));
            }
        } 
		// clean up allocated messages
        SnmpServerIntSnmpConfigRemoveUserIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigRemoveUserIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1); 
        delete pWaveSendToClusterContext;
        pSnmpServerSnmpConfigRemoveUserMessage->setMessageString (errorMessage);
        pSnmpServerSnmpConfigRemoveUserMessage->setCompletionStatus (status);
        reply (pSnmpServerSnmpConfigRemoveUserMessage);
    }

    void  ConfigUserWorker::SnmpServerSnmpConfigSetUserMessageHandler( SnmpServerSnmpConfigSetUserMessage *pSnmpServerSnmpConfigSetUserMessage)
    {
		trace (TRACE_LEVEL_INFO, "Entering SNMP User Set message handler");

		if (pSnmpServerSnmpConfigSetUserMessage == NULL) {
			trace (TRACE_LEVEL_FATAL, "Set User global:NULL message arrived");
			prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;
		unsigned char *base64AuthPaswd = NULL;
		unsigned char *base64PrivPaswd = NULL;
		char authPaswd[256];
		char privPaswd[256];
		int encLen = 0;
        string userName   	= pSnmpServerSnmpConfigSetUserMessage->getUsername();
        string groupName   	= pSnmpServerSnmpConfigSetUserMessage->getGroupname();
        AuthType authType   = pSnmpServerSnmpConfigSetUserMessage->getAuth();
        string authPswd   	= pSnmpServerSnmpConfigSetUserMessage->getAuthpswd();
        PrivType privType   = pSnmpServerSnmpConfigSetUserMessage->getPriv();
        string privPswd   	= pSnmpServerSnmpConfigSetUserMessage->getPrivpswd();
        bool encPswdFlag 	= pSnmpServerSnmpConfigSetUserMessage->getEncpswdflag();

		if (encPswdFlag)
		{
			base64AuthPaswd = SnmpAuthCrypto::base64_decode((unsigned char *)authPswd.c_str(),&encLen);
			if (!base64AuthPaswd)
			{
            	pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_ERROR);
            	reply (pSnmpServerSnmpConfigSetUserMessage);
				return;	
			}
			memset(authPaswd, 0, 256);
			strncpy(authPaswd,(char *)base64AuthPaswd,encLen);	
			authPaswd[encLen+1] = '\0';
			authPswd.assign(authPaswd);
			if (base64AuthPaswd != NULL)
			{
				free(base64AuthPaswd);
			}

			encLen = 0;
			base64PrivPaswd = SnmpAuthCrypto::base64_decode((unsigned char *)privPswd.c_str(),&encLen);
			if (!base64PrivPaswd)
			{
            	pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_ERROR);
            	reply (pSnmpServerSnmpConfigSetUserMessage);
				return;	
			}
			memset(privPaswd, 0, 256);
			strncpy(privPaswd,(char *)base64PrivPaswd,encLen);
			privPaswd[encLen+1] = '\0';
			privPswd.assign(privPaswd);	
			if (base64PrivPaswd != NULL)
			{
				free(base64PrivPaswd);
			}
		}

        if (((authType == AuthNone) && (authPswd.length() > 0)) || 
				((privType == PrivNone) && (privPswd.length() > 0)))
        {
            pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_SECURITYLEVEL);
            reply (pSnmpServerSnmpConfigSetUserMessage);
            return;
        }
        if (((authType != AuthNone) && ((authPswd.length() < 1) || (authPswd.length() > AGT_MAX_PSWD_SIZE) )) ||
                ((privType != PrivNone) && ((privPswd.length() < 1) || (privPswd.length() > AGT_MAX_PSWD_SIZE) )) )

        {
            pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_LEN_ERROR);
            reply (pSnmpServerSnmpConfigSetUserMessage);
            return;
        }

        if ((authType == AuthNone) && (privType != PrivNone) )
        {
           pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_SECURITYLEVEL);
           reply (pSnmpServerSnmpConfigSetUserMessage);
           return;
        }
        
        if ((groupName.compare("snmpadmin") != 0) && (groupName.compare("snmpuser") != 0) )
        {
            pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_GROUP_ERROR);
            reply (pSnmpServerSnmpConfigSetUserMessage);
            return;
        }
		
		if ((authType == AuthNone) && (encPswdFlag))
		{
            pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (WRC_SNMP_INVALID_PSWD_ERROR);
            reply (pSnmpServerSnmpConfigSetUserMessage);
            return;
		}
		
        SnmpServerIntSnmpConfigSetUserIntMessage *pMessage =
             new SnmpServerIntSnmpConfigSetUserIntMessage (userName, groupName,
                     authType, authPswd, privType, privPswd, encPswdFlag);

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext 
            (this, reinterpret_cast<PrismAsynchronousCallback> (&ConfigUserWorker::setUserSTCCB), pSnmpServerSnmpConfigSetUserMessage);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}
        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ConfigUserWorker::setUserSTCCB(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        AAAChangeRadiusHostContext *pRadiusHostCtx = NULL;
        SnmpServerSnmpConfigSetUserMessage *pSnmpServerSnmpConfigSetUserMessage = 
                                (SnmpServerSnmpConfigSetUserMessage *)pWaveSendToClusterContext->getPCallerContext();

        bool       allFailed = false;
        ResourceId status    = pWaveSendToClusterContext->getCompletionStatus();
        ResourceId fstatus   = WAVE_MESSAGE_SUCCESS;
        string     errorMessage;
		char authPaswd[256];	
		char privPaswd[256];	
		unsigned char *base64AuthPaswd = NULL;
		unsigned char *base64PrivPaswd = NULL;
		int encLen = 0;
		unsigned int authType1 = NO_AUTH_PROTOCOL;
		octetString *encAuthPswd = NULL;
		octetString *encPrivPswd = NULL;

        string userName   	= pSnmpServerSnmpConfigSetUserMessage->getUsername();
        string groupName   	= pSnmpServerSnmpConfigSetUserMessage->getGroupname();
        AuthType authType   = pSnmpServerSnmpConfigSetUserMessage->getAuth();
        string authPswd   	= pSnmpServerSnmpConfigSetUserMessage->getAuthpswd();
        PrivType privType   = pSnmpServerSnmpConfigSetUserMessage->getPriv();
        string privPswd   	= pSnmpServerSnmpConfigSetUserMessage->getPrivpswd();
        bool encPswdFlag    = pSnmpServerSnmpConfigSetUserMessage->getEncpswdflag();


		SnmpUserManagedObject   *pSetObject     = NULL;
		FILE *fp = NULL;
        // Added for testing purpose to simulate last replay usecase
        if (pSnmpServerSnmpConfigSetUserMessage->getIsALastConfigReplay() == false
                        && (fp = fopen("/tmp/unsync.txt", "r"))) {
            trace (TRACE_LEVEL_INFO, "Triggered Last replay usecase");
            fclose(fp);
            system ("/sbin/reboot -f");
            pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pSnmpServerSnmpConfigSetUserMessage);
            return;
        }


        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "PARTIAL SUCCESS - Setting snmp user operation failed on one or more of the connected nodes.");
            else
            {
                allFailed = true;
                trace (TRACE_LEVEL_ERROR, "FAILURE - Setting snmp user operation failed on all nodes");
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "ConfigUserWorker::setUserSTCCB", errorMessage);
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
     
            WaveManagedObjectSynchronousQueryContext *syncQueryContext = 
                new WaveManagedObjectSynchronousQueryContext(SnmpUserManagedObject::getClassName());
            syncQueryContext->addAndAttribute (new AttributeString (userName, "username"));
            vector<WaveManagedObject *> *pResultsRollback = querySynchronously (syncQueryContext);
            if ((NULL == pResultsRollback) || (pResultsRollback->size() == 0)) 
            {
                trace (TRACE_LEVEL_ERROR, "No such managed object" +userName);
                pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus
                                    (WRC_SNMP_INVALID_USER_ERROR);
       			 // clean up allocated messages
		        SnmpServerIntSnmpConfigSetUserIntMessage *pLocalMsgForPhase1 =
				(SnmpServerIntSnmpConfigSetUserIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		        delete(pLocalMsgForPhase1);
                reply (pSnmpServerSnmpConfigSetUserMessage);
                delete syncQueryContext;
                delete pWaveSendToClusterContext;
                return;
            }

            pSetObject = dynamic_cast<SnmpUserManagedObject *> ((*pResultsRollback)[0]);
            delete syncQueryContext;
            delete pResultsRollback;

            startTransaction ();
            updateWaveManagedObject (pSetObject);

			authType1 = authType + 1;
			if (!encPswdFlag) 
			{
				memset(authPaswd, 0, 256);
				strncpy(authPaswd,authPswd.c_str(),authPswd.length());
				authPaswd[authPswd.length()+1] = '\0';
				encAuthPswd = encryptPasswd(authType1, authPaswd);

				if (encAuthPswd != NULL) 
				{
					base64AuthPaswd = SnmpAuthCrypto::base64_encode(encAuthPswd->octet_ptr, encAuthPswd->length, &encLen);

					memset(authPaswd, 0, 256);
					strncpy(authPaswd,(char *)base64AuthPaswd,encLen);
					authPaswd[encLen] = '\0';
					authPswd.assign(authPaswd);
					if (encAuthPswd != NULL) 
					{
						free(encAuthPswd);
					}
					if (privPswd.length() > 1 ) 
					{
						memset(privPaswd, 0, 256);
						strncpy(privPaswd,privPswd.c_str(),privPswd.length());
						privPaswd[privPswd.length()+1] = '\0';
						encPrivPswd = encryptPasswd(authType1, privPaswd);

						encLen = 0;
						if (encPrivPswd != NULL) 
						{
							base64PrivPaswd = SnmpAuthCrypto::base64_encode(encPrivPswd->octet_ptr, encPrivPswd->length, &encLen);
						}

						memset(privPaswd, 0, 256);
						strncpy(privPaswd, (char *)base64PrivPaswd, encLen);
						privPaswd[encLen+1] = '\0';
						privPswd.assign(privPaswd);
						if (encPrivPswd != NULL) 
						{
							free(encPrivPswd);
						}
					}
				}
			}
			if (authType == AuthNone) {
				encPswdFlag = false;
			} else {
				encPswdFlag = true;
			}
            pSetObject->setGroupname(groupName);
            pSetObject->setAuth(authType);
            pSetObject->setAuthpswd(authPswd);
            pSetObject->setPriv(privType);
            pSetObject->setPrivpswd(privPswd);
            pSetObject->setEncpswdflag(encPswdFlag);

            fstatus = commitTransaction ();

            if (FRAMEWORK_SUCCESS == status)
            {
                trace (TRACE_LEVEL_INFO, " SNMP User SET: Successfully Cluster-Wide committed the SNMPV3 User Configuration");
                fstatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL,
                    "SNMP User SET : Commiting SNMPV3 User config failed.Status:"
                    + FrameworkToolKit::localize (fstatus));
            }
		    delete pSetObject;
        }
        // clean up allocated messages
        SnmpServerIntSnmpConfigSetUserIntMessage *pLocalMsgForPhase1 =
		(SnmpServerIntSnmpConfigSetUserIntMessage*)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pLocalMsgForPhase1);
        delete pWaveSendToClusterContext;
        pSnmpServerSnmpConfigSetUserMessage->setMessageString (errorMessage);
		pSnmpServerSnmpConfigSetUserMessage->setCompletionStatus (status);
		reply (pSnmpServerSnmpConfigSetUserMessage);
    }
}
