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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "DcmCore/DcmToolKit.h"
#include "FCAuth/Local/FCAuthLocalObjectManager.h"
#include "FCAuth/Local/LocalSecPolicyWorker.h"
#include "FCAuth/Local/FCAuthLocalAddPolicyMemberMessage.h"
#include "FCAuth/Local/FCAuthLocalDeletePolicyMemberMessage.h"
#include "FCAuth/Local/FCAuthLocalActivatePolicyMessage.h"
#include "FCAuth/Local/DefinedPolicyMemberLocalManagedObject.h"
#include "FCAuth/Local/DefinedPolicyLocalManagedObject.h"
#include "FCAuth/Local/ActivePolicyMemberLocalManagedObject.h"
#include "FCAuth/Local/ActivePolicyLocalManagedObject.h"
#include "FCAuth/Local/LocalSecPolicyFileReplayEndMessage.h"
#include "Utils/DceClusterUtils.h"
#include "FCAuth/Local/FCAuthTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "DcmResourceIdEnums.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Postboot/PrismPostPersistentBootMessages.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForFileReplayEnd.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "fabos/license.h"

//#define SECPOLICY_FSS_STAGE 15
extern "C"
{
	#include "security/public.h"
	#define SEC_DEFINE_SCC_POLICY "D_Scc.SCC_POLICY"
	#define SEC_ACTIVE_SCC_POLICY "A_Scc.SCC_POLICY"
	#define SEC_SCC_POLICY "SCC_POLICY"
	// #include "security/secipc.h"
	// #include "security/acl_public.h"
}
namespace DcmNs
{
    LocalSecPolicyWorker::LocalSecPolicyWorker ( FCAuthLocalObjectManager *pFCAuthLocalObjectManager)
        : WaveWorker  (pFCAuthLocalObjectManager)
    {
        DefinedPolicyMemberLocalManagedObject    DefinedPolicyMemberLocalManagedObject    (pFCAuthLocalObjectManager);
        DefinedPolicyLocalManagedObject    DefinedPolicyLocalManagedObject    (pFCAuthLocalObjectManager);
        ActivePolicyMemberLocalManagedObject    ActivePolicyMemberLocalManagedObject    (pFCAuthLocalObjectManager);
        ActivePolicyLocalManagedObject    ActivePolicyLocalManagedObject    (pFCAuthLocalObjectManager);
        DefinedPolicyMemberLocalManagedObject.setupOrm ();
        addManagedClass (DefinedPolicyMemberLocalManagedObject::getClassName (), this);
        DefinedPolicyLocalManagedObject.setupOrm ();
        addManagedClass (DefinedPolicyLocalManagedObject::getClassName (), this);
        ActivePolicyMemberLocalManagedObject.setupOrm ();
        addManagedClass (ActivePolicyMemberLocalManagedObject::getClassName (), this);
        ActivePolicyLocalManagedObject.setupOrm ();
        addManagedClass (ActivePolicyLocalManagedObject::getClassName (), this);
        addOperationMap (FCAUTHLOCALADDPOLICYMEMBER, reinterpret_cast<PrismMessageHandler> (&LocalSecPolicyWorker::FCAuthLocalAddPolicyMemberMessageHandler));
        addOperationMap (FCAUTHLOCALDELETEPOLICYMEMBER, reinterpret_cast<PrismMessageHandler> (&LocalSecPolicyWorker::FCAuthLocalDeletePolicyMemberMessageHandler));
        addOperationMap (FCAUTHLOCALACTIVATEPOLICY, reinterpret_cast<PrismMessageHandler> (&LocalSecPolicyWorker::FCAuthLocalActivatePolicyMessageHandler));
        addOperationMap (LOCALSECPOLICYFILEREPLAYEND, reinterpret_cast<PrismMessageHandler> (&LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageHandler));
    }

    LocalSecPolicyWorker::~LocalSecPolicyWorker ()
    {
    }

    PrismMessage  *LocalSecPolicyWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FCAUTHLOCALADDPOLICYMEMBER :
                pPrismMessage = new FCAuthLocalAddPolicyMemberMessage ();
                break;
            case FCAUTHLOCALDELETEPOLICYMEMBER :
                pPrismMessage = new FCAuthLocalDeletePolicyMemberMessage ();
                break;
            case FCAUTHLOCALACTIVATEPOLICY :
                pPrismMessage = new FCAuthLocalActivatePolicyMessage ();
                break;
            case LOCALSECPOLICYFILEREPLAYEND :
                pPrismMessage = new LocalSecPolicyFileReplayEndMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalSecPolicyWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((DefinedPolicyMemberLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new DefinedPolicyMemberLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((DefinedPolicyLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new DefinedPolicyLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ActivePolicyMemberLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ActivePolicyMemberLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ActivePolicyLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ActivePolicyLocalManagedObject(dynamic_cast<FCAuthLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LocalSecPolicyWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

#if 0
	void LocalSecPolicyWorker::externalStateSynchronization (
             WaveAsynchronousContextForExternalStateSynchronization
             *pWaveAsynchronousContextForExternalStateSynchronization)
    {
        if ( (SECPOLICY_FSS_STAGE == pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber()) &&
            (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW) )
        {
            trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::externalStateSynchronization Entering..");
            /*cal l the API*/
            if (0 != ret)
            {
                trace (TRACE_LEVEL_INFO, string("LocalSecPolicyWorker::externalStateSynchronization failed"));
            }
        }
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();
    }
#endif

	void LocalSecPolicyWorker::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
	    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_CONFIG_REPLAY_COMPLETED_EVENT, reinterpret_cast<PrismEventHandler> (&LocalSecPolicyWorker::configReplayCompletedEventHandler));
	    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	    pWaveAsynchronousContextForBootPhases->callback ();
	}

	void LocalSecPolicyWorker::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {

        SI8 passNum = pWaveAsynchronousContextForPostBootPhase->getPassNum();

        trace (TRACE_LEVEL_INFO, string("LocalSecPolicyWorker Postboot Entered with pass: ") + pWaveAsynchronousContextForPostBootPhase->getPassNum());
        switch (passNum) {
            case 0:
				configImport();
                break;
            default:
                break;
        }

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();
    }
	void LocalSecPolicyWorker::configReplayCompletedEventHandler (const ConfigReplayCompletedEvent* &pEvent)
	{
	    trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::configReplayCompletedEventHandler()");
		/*
		 * Once the DB is updated during config replay import the DB entries
		 * to the backend.
		 */
		configImport();
		reply (reinterpret_cast<const PrismEvent *&> (pEvent));
	}

	void LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageHandler (LocalSecPolicyFileReplayEndMessage *pLocalSecPolicyFileReplayEndMessage)
	{
		trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::FCAuthLocalSecPolicyFileReplayEndMessageHandler Enter");
		if (pLocalSecPolicyFileReplayEndMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		configImport();

        /* Fix for Defect 456366 */
		pLocalSecPolicyFileReplayEndMessage->setCompletionStatus (status);
		reply(pLocalSecPolicyFileReplayEndMessage);
	}

	void LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageCallback Enter");
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		PrismAsynchronousContext *pPrismAsynchronousContext = reinterpret_cast<PrismAsynchronousContext *>(pWaveSendToClusterContext->getPCallerContext());
        status = ClusterGetStatusFromContext(pWaveSendToClusterContext);
                        
		if (WAVE_MESSAGE_SUCCESS == status)
		{
			trace(TRACE_LEVEL_INFO, "LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageCallback : Send To Cluster Success");
		}
		else
		{
			trace(TRACE_LEVEL_ERROR,"LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageCallback : Send To Cluster failed");
		}
                                           
		delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
		delete (pWaveSendToClusterContext);
                                                                              
		pPrismAsynchronousContext->setCompletionStatus (status);
		pPrismAsynchronousContext->callback ();

	}

	void LocalSecPolicyWorker::fileReplayEnd (WaveAsynchronousContextForFileReplayEnd *pWaveAsynchronousContextForFileReplayEnd)
	{
		trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::fileReplayEnd");
		/*int retVal = 0;
		string policyName = "SCC_POLICY";
		retVal = secPolicyDeleteNos((char*)policyName.c_str());
		if (0 == retVal) {
			trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::Delete the policy in hte backend");
			retVal = secPolicyActivateNos();
		}*/
		ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType ();

		if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE == vcsMode) {
			trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::fileReplayEnd: Invoking configImport for standalone");
			configImport();
		pWaveAsynchronousContextForFileReplayEnd->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForFileReplayEnd->callback ();
		return;	
		} else if ((VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == vcsMode) ||
				(VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsMode)) {
			WaveSendToClusterContext    *pWaveSendToClusterContext;
			vector<UI32> locations;
			LocalSecPolicyFileReplayEndMessage *pMessage = new LocalSecPolicyFileReplayEndMessage();

			pWaveSendToClusterContext = new WaveSendToClusterContext(this,
				reinterpret_cast<PrismAsynchronousCallback>(&LocalSecPolicyWorker::LocalSecPolicyFileReplayEndMessageCallback),
					pWaveAsynchronousContextForFileReplayEnd);

			FrameworkToolKit::getFullyConnectedLocations(locations);
			locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
			pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
			pWaveSendToClusterContext->setPartialSuccessFlag(true);
			if(!locations.empty()) {
				pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
			}
			trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::fileReplayEnd: Invoking sendToWaveCluster for sending fileReplayEnd to other nodes MC");
			sendToWaveCluster(pWaveSendToClusterContext);
		}
	}

	void LocalSecPolicyWorker::configImport()
	{
		bool cfgReplayFlag = 1;

		FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage =
				new FCAuthLocalAddPolicyMemberMessage( string("SCC_POLICY"), string(""), 0, cfgReplayFlag);

		secPolicyDefinedImport(pFCAuthLocalAddPolicyMemberMessage);
		/*
		 * Import the Active policy alone to the back end in order to 
		 * handle the situation where there is no (or) different defined
		 * policy member.
		 */
        secPolicyActiveImport(pFCAuthLocalAddPolicyMemberMessage);
		/* Enable all disabled (SW violation) ports */
		(void) secPolicyEnableFCPorts();
		delete pFCAuthLocalAddPolicyMemberMessage;
	
	}

	ResourceId LocalSecPolicyWorker::secpolicyActivatePlugin()
	{
		trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secpolicyActivatePlugin entry\n");
		int retVal = -1;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		DefinedPolicyLocalManagedObject *pDefinedPolicyLocalManagedObject = NULL;
		ActivePolicyLocalManagedObject *pActivePolicyLocalManagedObject = NULL;

		if (!(pActivePolicyLocalManagedObject =
				queryActivePolicyLocalManagedObjectByName(string(SEC_SCC_POLICY)))) {
			if (!(pDefinedPolicyLocalManagedObject = 
					queryDefinedPolicyLocalManagedObjectByName(string(SEC_SCC_POLICY)))) {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::activating no policy\n");
				return (SECPOLICY_ACTIVATE_NOPOLICY);
			} 
		}
		retVal = secPolicyActivateNos();

		if (pDefinedPolicyLocalManagedObject) {
			delete pDefinedPolicyLocalManagedObject;
			pDefinedPolicyLocalManagedObject = NULL;
	 	}

		if (pActivePolicyLocalManagedObject) {
			delete pActivePolicyLocalManagedObject;
			pActivePolicyLocalManagedObject = NULL;
	 	}

		if (0 == retVal) {
			trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secpolicyActivatePlugin back end activate success\n");
			/* Remove all active policy list */
			startTransaction();
			/*
			 * Delete the Active poilicy list in order to update
			 * active list with the new defined list 
			 */
			//deleteAllManagedObjectInstances(ActivePolicyMemberLocalManagedObject::getClassName());
			//deleteAllManagedObjectInstances(ActivePolicyLocalManagedObject::getClassName());
			LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
			WaveManagedObjectSynchronousQueryContextForDeletion *pContextActiveMember =
				new WaveManagedObjectSynchronousQueryContextForDeletion(ActivePolicyMemberLocalManagedObject::getClassName());
			pContextActiveMember->setLocationIdFilter (thisLocationId);
			/*
			 * DEFECT000436572: deleting the MO using deleteWaveManagedObject
			 * causes crash, instead we should use deleteMultipleWaveManagedObjects.
			 */
			deleteMultipleWaveManagedObjects(pContextActiveMember);

			WaveManagedObjectSynchronousQueryContextForDeletion *pContextActive =
				new WaveManagedObjectSynchronousQueryContextForDeletion(ActivePolicyLocalManagedObject::getClassName());
			pContextActive->setLocationIdFilter (thisLocationId);
			deleteMultipleWaveManagedObjects(pContextActive);
			/*
			 * DEFECT000436572: since deleteMultipleWaveManagedObjects deletes 
			 * the MOs without loading into memory,
			 * seperate transcation should be used to delete and create MOs.
			 */
			commitTransaction();
			//WaveManagedObjectSynchronousQueryContext synchronousQueryContextDefine (DefinedPolicyLocalManagedObject::getClassName());
			//synchronousQueryContextDefine.addAndAttribute (new AttributeString ("SCC_POLICY", "policyName"));
			startTransaction();
        	vector<WaveManagedObject *> *pResultsDefine = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, DefinedPolicyLocalManagedObject::getClassName());
			//vector<WaveManagedObject *> *pResultsDefine =
			//	querySynchronously (&synchronousQueryContextDefine);
			if ((pResultsDefine != NULL) && (pResultsDefine->size () > 0)) {
				/* For now only SCC policy is supported */
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secpolicyActivatePlugin copy defined to active\n");
				/*TODO:To support DCC, FCS add a loop of 3 here*/
				pDefinedPolicyLocalManagedObject =
					dynamic_cast<DefinedPolicyLocalManagedObject*> ((*pResultsDefine)[0]);
				pActivePolicyLocalManagedObject =
					dynamic_cast<ActivePolicyLocalManagedObject *>(createManagedObjectInstance(ActivePolicyLocalManagedObject::getClassName()));

				vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > activeMember;
				vector<WaveManagedObjectPointer<DefinedPolicyMemberLocalManagedObject> > definedMember;

				pActivePolicyLocalManagedObject->setPolicyName(
							pDefinedPolicyLocalManagedObject->getPolicyName());
				/* collect defined policy members*/
				definedMember = pDefinedPolicyLocalManagedObject->getPolicyMembers();
				for(unsigned int i = 0; i<definedMember.size(); i++) {
					ActivePolicyMemberLocalManagedObject *pActivePolicyMemberLocalManagedObject =
                    dynamic_cast<ActivePolicyMemberLocalManagedObject*>(createManagedObjectInstance(ActivePolicyMemberLocalManagedObject::getClassName()));
					pActivePolicyMemberLocalManagedObject->setPolicyMemberEntry(
							definedMember.at(i)->getPolicyMemberEntry());
					pActivePolicyMemberLocalManagedObject->setListIndex(
							definedMember.at(i)->getListIndex());
					/* copy each defined members into active member list*/
					activeMember.push_back(pActivePolicyMemberLocalManagedObject);
				}
				pActivePolicyLocalManagedObject->setPolicyMembers(activeMember);
				
			}
			commitTransaction(); 
			if (pResultsDefine)
				delete pResultsDefine;
		} else {
			status = SecpolicyRespToStatus(retVal);
			if (SECPOLICY_GENERIC_FAILURE == status) {
				status = SECPOLICY_ACTIVATE_FAILED;
            }
		}
		if (pDefinedPolicyLocalManagedObject)
			delete pDefinedPolicyLocalManagedObject;
		if (pActivePolicyLocalManagedObject)
			delete pActivePolicyLocalManagedObject;

		return (status);
	}

    void  LocalSecPolicyWorker::FCAuthLocalActivatePolicyMessageHandler( FCAuthLocalActivatePolicyMessage *pFCAuthLocalActivatePolicyMessage)
    {
		trace (TRACE_LEVEL_DEBUG, "LocalSecPolicyWorker::FCAuthLocalActivatePolicyMessageHandler");

        if (NULL == pFCAuthLocalActivatePolicyMessage) {
            trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
            prismAssert (false, __FILE__, __LINE__);
        }
		if (!isSupportedPlatform()) {
            trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::FCAuthLocalActivatePolicyMessageHandler "
                    "Command not supported in this platform");
            pFCAuthLocalActivatePolicyMessage->setCompletionStatus(SECPOLICY_UNSUPPORTED_PLATFORM);
            reply(pFCAuthLocalActivatePolicyMessage);
            return;
        }
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        status = secpolicyActivatePlugin();

        pFCAuthLocalActivatePolicyMessage->setCompletionStatus(status);
        reply (pFCAuthLocalActivatePolicyMessage);

    }

	DefinedPolicyLocalManagedObject *LocalSecPolicyWorker::queryDefinedPolicyLocalManagedObjectByName(string policyName) 
	{
		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (DefinedPolicyLocalManagedObject::getClassName());
		synchronousQueryContext.addAndAttribute (new AttributeString (policyName, "policyName"));

		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults =
				querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, &synchronousQueryContext);
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			/* Empty defined list */
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::queryDefinedPolicyLocalManagedObjectByName: Cannot find entry");
				pResults->clear ();
				delete pResults;
				return (NULL);
			} else if (1 == numberOfResults) {
				/* Found the defined list */
				DefinedPolicyLocalManagedObject *pDefinedPolicyLocalManagedObject =
					dynamic_cast<DefinedPolicyLocalManagedObject*> ((*pResults)[0]);
				pResults->clear ();
				delete pResults;
				return (pDefinedPolicyLocalManagedObject);
			} else {
				/* Somthing wrong in the DB entries */
				pResults->clear ();
				delete pResults;
				trace (TRACE_LEVEL_FATAL, "LocalSecPolicyWorker::queryDefinedPolicyLocalManagedObjectByName More than one node");
				prismAssert (false, __FILE__, __LINE__);
				return (NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "LocalSecPolicyWorker::queryDefinedPolicyLocalManagedObjectByName: System failure");
			prismAssert (false, __FILE__, __LINE__);
            return (NULL);
		}
		return (NULL);
	}

	ActivePolicyLocalManagedObject*LocalSecPolicyWorker::queryActivePolicyLocalManagedObjectByName(string policyName) 
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (ActivePolicyLocalManagedObject::getClassName());
		synchronousQueryContext.addAndAttribute (new AttributeString (policyName, "policyName"));
		LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
		vector<WaveManagedObject *> *pResults =
                querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, &synchronousQueryContext);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::queryActivePolicyLocalManagedObjectByName: Cannot find entry");
                pResults->clear ();
                delete pResults;
                return (NULL);
            } else if (1 == numberOfResults) {
				ActivePolicyLocalManagedObject *pActivePolicyLocalManagedObject =
                    dynamic_cast<ActivePolicyLocalManagedObject*> ((*pResults)[0]);
                pResults->clear ();
                delete pResults;
                return (pActivePolicyLocalManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "LocalSecPolicyWorker::queryActivePolicyLocalManagedObjectByName More than one node");
                prismAssert (false, __FILE__, __LINE__);
                return (NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "LocalSecPolicyWorker::queryActivePolicyLocalManagedObjectByName: System failure");
            prismAssert (false, __FILE__, __LINE__);
            return (NULL);
        }
        return (NULL);
	}

	
	ResourceId LocalSecPolicyWorker::secPolicyDefinedAddPlugin(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage)
	{

		trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secpolicyAddPlugin entry\n");
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		int retVal = -1;

		DefinedPolicyLocalManagedObject *pDefinedPolicyLocalManagedObject = NULL;
		DefinedPolicyMemberLocalManagedObject *pDefinedPolicyMemberLocalManagedObject = NULL;
		/* If secpolicy exists Add the member into back end and DB*/
		if ((pDefinedPolicyLocalManagedObject = queryDefinedPolicyLocalManagedObjectByName(
                pFCAuthLocalAddPolicyMemberMessage->getPolicyName()))) {
			if (pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().empty()) {
				delete pDefinedPolicyLocalManagedObject;
				return (SECPOLICY_CREATE_POLICY_EXISTS);
			}
			int maxListIndex;
			pDefinedPolicyLocalManagedObject->numOfLeftPolicyMembers(
                    pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList(),
                    &maxListIndex);
			/* If config Reply: first need to update the DB then import the list to back end */
			if (pFCAuthLocalAddPolicyMemberMessage->getCfgReplayFlag()){
				retVal = 0;					
			} else { /* If CLI command */
				/* API to add the secpolicy */
				retVal = secPolicyAddNos(
					(char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(),
                    	    (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().c_str());
			}

			// if (!FrameworkToolKit::isConfigReplayInProgress()) {
				if (0 == retVal) {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyAddPlugin: backend success\n");
					startTransaction();
					pDefinedPolicyMemberLocalManagedObject =
						dynamic_cast<DefinedPolicyMemberLocalManagedObject *>(createManagedObjectInstance(DefinedPolicyMemberLocalManagedObject::getClassName()));
					/* Updated the DB */
					pDefinedPolicyMemberLocalManagedObject->setPolicyMemberEntry(
						pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList());
					pDefinedPolicyMemberLocalManagedObject->setListIndex(maxListIndex + 1);

					string className = DefinedPolicyLocalManagedObject::getClassName();
					string childClassName =
						DefinedPolicyMemberLocalManagedObject::getClassName();
					string compositionName = "policyMembers";

					pDefinedPolicyMemberLocalManagedObject->setOwnerManagedObjectId(
						pDefinedPolicyLocalManagedObject->getObjectId());
					addToComposition(className, childClassName,
						compositionName, pDefinedPolicyLocalManagedObject->getObjectId(),
						pDefinedPolicyMemberLocalManagedObject->getObjectId());
					commitTransaction();
					tracePrintf (TRACE_LEVEL_INFO, true, true,
            		    "secpolicy add plugin %s %s %d\n",
	        	        (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(),
    		            (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().c_str(),
	    	            retVal);
				} else {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyAddPlugin: secPolicyAdd failure \n");
					status = SecpolicyRespToStatus(retVal);
                	if (SECPOLICY_GENERIC_FAILURE == status) {
                    	status = SECPOLICY_MEMBER_ADD_FAILED;
                	}
				}
			//}
		} else {
			/* If there is no policy, create the poilcy and add the memeber(If exists) */
			if (pFCAuthLocalAddPolicyMemberMessage->getCfgReplayFlag()) {
				/* In case of Cfg  replay first update DB and import the list to Backend */
				retVal = 0;
			} else { /*Call NOs specifc API which does init and create*/
				if (pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().empty()) {
					retVal = secPolicyCreateNos(
						(char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(), NULL);
				} else {
					retVal = secPolicyCreateNos(
                        (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(),
                        (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().c_str());
				}
			}

			if (0 == retVal) {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyAddPlugin: backend secPolicyCreate success\n");
				startTransaction();
				/* Update the DB */
				pDefinedPolicyLocalManagedObject = 
					dynamic_cast<DefinedPolicyLocalManagedObject *>(createManagedObjectInstance(DefinedPolicyLocalManagedObject::getClassName()));
				pDefinedPolicyLocalManagedObject->setPolicyName(
					pFCAuthLocalAddPolicyMemberMessage->getPolicyName());

				if (!pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().empty()) {
					pDefinedPolicyMemberLocalManagedObject =
						dynamic_cast<DefinedPolicyMemberLocalManagedObject *>(createManagedObjectInstance(DefinedPolicyMemberLocalManagedObject::getClassName()));

	                pDefinedPolicyMemberLocalManagedObject->setPolicyMemberEntry(
	                    pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList());
	                pDefinedPolicyMemberLocalManagedObject->setListIndex(0);

					vector<WaveManagedObjectPointer<DefinedPolicyMemberLocalManagedObject> > policyMember;
					policyMember.push_back(pDefinedPolicyMemberLocalManagedObject);
				
					pDefinedPolicyLocalManagedObject->setPolicyMembers(policyMember);
				}
				commitTransaction();
			} else {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyAddPlugin: backend secPolicyCreate failure \n");
				status = SecpolicyRespToStatus(retVal);
                if (SECPOLICY_GENERIC_FAILURE == status) {
                    status = SECPOLICY_CREATE_FAILED;
                }
			}	
			tracePrintf (TRACE_LEVEL_INFO, true, true,
                "secpolicyDefineAddPlugin %s %s %d\n",
                (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(),
                (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().c_str(),
                retVal);		
		}
		if (pDefinedPolicyMemberLocalManagedObject)
			delete pDefinedPolicyMemberLocalManagedObject;
		if (pDefinedPolicyLocalManagedObject)
			delete pDefinedPolicyLocalManagedObject;
		return (status);
    }

	ResourceId LocalSecPolicyWorker::secPolicyDefinedImport(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage)
    {
        trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyDefinedImport entry\n");
        ResourceId status = WAVE_MESSAGE_SUCCESS;	
		int retVal = -1;
		/* Now import the defined list to backend(if cfg replay)*/
		if (pFCAuthLocalAddPolicyMemberMessage->getCfgReplayFlag()) {
			DefinedPolicyLocalManagedObject *pDefinedPolicyLocalMO = NULL;
			if ((pDefinedPolicyLocalMO = queryDefinedPolicyLocalManagedObjectByName(
                pFCAuthLocalAddPolicyMemberMessage->getPolicyName()))) {
                /* To import Defined policy to backend*/
                trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::addplugin cfg reply\n");
                vector<WaveManagedObjectPointer<DefinedPolicyMemberLocalManagedObject> > definedMember;
                string policyName;
                string policyMember;

                policyName.append(pDefinedPolicyLocalMO->getPolicyName());
                definedMember = pDefinedPolicyLocalMO->getPolicyMembers();

				/* policy members should be in WWN1;WWW2;WWN3;.... format */
                for(unsigned int i = 0; i<definedMember.size(); i++) {
                    policyMember.append(definedMember.at(i)->getPolicyMemberEntry());
                    policyMember.append(";");
                }
				if (policyMember.empty()) {
					retVal = secPolicyCfgDownloadNos((char *)SEC_DEFINE_SCC_POLICY,
								(char *)"EMPTY");
				} else {
					retVal = secPolicyCfgDownloadNos((char *)SEC_DEFINE_SCC_POLICY,
                            		(char *)policyMember.c_str());
				}
                if (retVal != 0) {
                    trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::Failed to import defined policy");
					status = SecpolicyRespToStatus(retVal);
                	if (SECPOLICY_GENERIC_FAILURE == status) {
                    	status = SECPOLICY_CFG_REPLAY_DEFINED_FAILED;
                	}
                }
				tracePrintf (TRACE_LEVEL_INFO, true, true,
                    "secpolicy cfg replay import defined policy ret: %d\n", retVal);
            } else {
                trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::No defined members to import");
				/* 366292: If there is no member in the DB remove
				 * the backend entries too(if exists)
				 */
				retVal = secPolicyDeleteNos(
							(char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str());
				if (0 == retVal) {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::Delete the policy in hte backend");
					retVal = secPolicyActivateNos();
					/*
					 * Removed swithcdisable/enable since it should not be done 
					 * without user's knowledge.
					 * Decided to document the behavior in FS
					 */
				}	
            }			
			if (pDefinedPolicyLocalMO)
				delete pDefinedPolicyLocalMO;
        }
		return (status);
	}

	ResourceId LocalSecPolicyWorker::secPolicyActiveAddPlugin(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage)
	{
        trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secpolicyActiveAddPlugin entry\n");
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int retVal = -1;
        
		ActivePolicyLocalManagedObject *pActivePolicyLocalManagedObject = NULL;
		ActivePolicyMemberLocalManagedObject *pActivePolicyMemberLocalManagedObject = NULL;
        /* If secpolicy exists Add the member into back end and DB*/
        if ((pActivePolicyLocalManagedObject = queryActivePolicyLocalManagedObjectByName(
                pFCAuthLocalAddPolicyMemberMessage->getPolicyName()))) {
            if (pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().empty()) {
				delete pActivePolicyLocalManagedObject;
                return (SECPOLICY_CREATE_POLICY_EXISTS);
            }   
			startTransaction();
            int maxListIndex;
            pActivePolicyLocalManagedObject->numOfLeftPolicyMembers(
                    pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList(),
                    &maxListIndex);
            trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyActiveAddPlugin: update the DB\n");
			pActivePolicyMemberLocalManagedObject =
                      dynamic_cast<ActivePolicyMemberLocalManagedObject *>(createManagedObjectInstance(ActivePolicyMemberLocalManagedObject::getClassName()));
            pActivePolicyMemberLocalManagedObject->setPolicyMemberEntry(
                       pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList());
            pActivePolicyMemberLocalManagedObject->setListIndex(maxListIndex + 1);

            string className = ActivePolicyLocalManagedObject::getClassName();
            string childClassName =
                        ActivePolicyMemberLocalManagedObject::getClassName();
            string compositionName = "PolicyMembers";

            pActivePolicyMemberLocalManagedObject->setOwnerManagedObjectId(
                        pActivePolicyLocalManagedObject->getObjectId());
            addToComposition(className, childClassName,
                        compositionName, pActivePolicyLocalManagedObject->getObjectId(),
                        pActivePolicyMemberLocalManagedObject->getObjectId());
			commitTransaction();
            tracePrintf (TRACE_LEVEL_INFO, true, true,
                        "secpolicy active add plugin %s %s %d\n",
                        (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(),
                        (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().c_str(),
                        retVal);
        } else {
            // if (0 == retVal) {
                trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyActiveAddPlugin: add to DB");
				startTransaction();
                pActivePolicyLocalManagedObject =
                    dynamic_cast<ActivePolicyLocalManagedObject *>(createManagedObjectInstance(ActivePolicyLocalManagedObject::getClassName()));
                pActivePolicyLocalManagedObject->setPolicyName(
                    pFCAuthLocalAddPolicyMemberMessage->getPolicyName());

                if (!pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().empty()) {
                    pActivePolicyMemberLocalManagedObject =
                        dynamic_cast<ActivePolicyMemberLocalManagedObject *>(createManagedObjectInstance(ActivePolicyMemberLocalManagedObject::getClassName()));

                    pActivePolicyMemberLocalManagedObject->setPolicyMemberEntry(
                        pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList());
                    pActivePolicyMemberLocalManagedObject->setListIndex(0);

                    vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > policyMember;
                    policyMember.push_back(pActivePolicyMemberLocalManagedObject);

                    pActivePolicyLocalManagedObject->setPolicyMembers(policyMember);
                }
				commitTransaction();	
	            tracePrintf (TRACE_LEVEL_INFO, true, true,
    	            "secpolicy active create plugin %s %s %d\n",
        	        (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str(),
            	    (char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyEntryList().c_str(),
                	retVal);
			//}
        }
		if (pActivePolicyLocalManagedObject)
			delete pActivePolicyLocalManagedObject;
		if (pActivePolicyMemberLocalManagedObject)
			delete pActivePolicyMemberLocalManagedObject;
		return (status);
    }

	ResourceId LocalSecPolicyWorker::secPolicyActiveImport(FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage)
	{
		trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyActiveImport entry\n");
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		int retVal = -1;
        /* Now import the active list to backend(if cfg replay)*/
        if (pFCAuthLocalAddPolicyMemberMessage->getCfgReplayFlag()) {
			ActivePolicyLocalManagedObject *pActivePolicyLocalMO = NULL;
            if ((pActivePolicyLocalMO = queryActivePolicyLocalManagedObjectByName(
                pFCAuthLocalAddPolicyMemberMessage->getPolicyName()))) {
                /* To import active policy to backend*/
                trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyActiveImport cfg reply\n");
                vector<WaveManagedObjectPointer<ActivePolicyMemberLocalManagedObject> > activeMember;
                string policyName;
                string policyMember;

                policyName.append(pActivePolicyLocalMO->getPolicyName());
                activeMember = pActivePolicyLocalMO->getPolicyMembers();

                for(unsigned int i = 0; i<activeMember.size(); i++) {
                    policyMember.append(activeMember.at(i)->getPolicyMemberEntry());
                    policyMember.append(";");
				}
				if (policyMember.empty()) {
					retVal = secPolicyCfgDownloadNos((char *)SEC_ACTIVE_SCC_POLICY,
										(char *)"EMPTY");
				} else {
					retVal = secPolicyCfgDownloadNos((char *)SEC_ACTIVE_SCC_POLICY,
                 	               (char *)policyMember.c_str());
				}
                if (retVal != 0) {
                    trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyActiveImport Failed to import active policy");
					status = SecpolicyRespToStatus(retVal);
                    if (SECPOLICY_GENERIC_FAILURE == status) {
                        status = SECPOLICY_CFG_REPLAY_ACTIVE_FAILED;
                    }
                }
				tracePrintf (TRACE_LEVEL_INFO, true, true,
                    "secpolicy cfg replay import active policy ret: %d\n", retVal);
            } else {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyActiveImport No Active mem to import");
			}
			DefinedPolicyLocalManagedObject *pDefinedPolicyLocalMO;
            if ((pDefinedPolicyLocalMO = queryDefinedPolicyLocalManagedObjectByName(
                pFCAuthLocalAddPolicyMemberMessage->getPolicyName()))) {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker:: no op");
			} else {
				trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::clear def list if DB is empty");
				/* 366292: While importing active list alone defined list also changed
				 * hence clearing defined list if ther is no defined DB entries
				 */
				retVal = secPolicyDeleteNos(
						(char *)pFCAuthLocalAddPolicyMemberMessage->getPolicyName().c_str());
			}
			if (pDefinedPolicyLocalMO)
				delete pDefinedPolicyLocalMO;
			if (pActivePolicyLocalMO)
				delete pActivePolicyLocalMO;
        }
		return (status);
	}

	ResourceId LocalSecPolicyWorker::secPolicyDeletePlugin(FCAuthLocalDeletePolicyMemberMessage *pFCAuthLocalDeletePolicyMemberMessage)
	{
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int retVal = -1;

        DefinedPolicyLocalManagedObject *pDefinedPolicyLocalManagedObject = NULL;
        /* If secpolicy exists */
        if ((pDefinedPolicyLocalManagedObject = queryDefinedPolicyLocalManagedObjectByName(
                pFCAuthLocalDeletePolicyMemberMessage->getPolicyName()))) {

			/* If no policy member, delete the policy itself */
			if (pFCAuthLocalDeletePolicyMemberMessage->getPolicyEntryList().empty()) {
				retVal = secPolicyDeleteNos(
					(char *)pFCAuthLocalDeletePolicyMemberMessage->getPolicyName().c_str());

				if (0 == retVal) {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyDeletePlugin: backend secPolicyDelte success\n");
					startTransaction();
					/* Delete from the DB */
					//deleteAllManagedObjectInstances(DefinedPolicyMemberLocalManagedObject::getClassName());i
					//deleteAllManagedObjectInstances(DefinedPolicyLocalManagedObject::getClassName());	
		            LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
       			    WaveManagedObjectSynchronousQueryContextForDeletion *pContextDefineMember =
						new WaveManagedObjectSynchronousQueryContextForDeletion (DefinedPolicyMemberLocalManagedObject::getClassName());
		            pContextDefineMember->setLocationIdFilter (thisLocationId);
		            deleteWaveManagedObject (pContextDefineMember);
		            WaveManagedObjectSynchronousQueryContextForDeletion *pContextDefine =
						new WaveManagedObjectSynchronousQueryContextForDeletion(DefinedPolicyLocalManagedObject::getClassName());
		            pContextDefine->setLocationIdFilter (thisLocationId);
		            deleteWaveManagedObject (pContextDefine);
					commitTransaction();
				} else {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyDeletePlugin: backend secPolicyDelte failure\n");
					status = SecpolicyRespToStatus(retVal);
                    if (SECPOLICY_GENERIC_FAILURE == status) {
                        status = SECPOLICY_DELETE_FAILED;
                    }
				}
				tracePrintf (TRACE_LEVEL_INFO, true, true,
                "secpolicy delete plugin %s %d\n",
                (char *)pFCAuthLocalDeletePolicyMemberMessage->getPolicyName().c_str(),
                retVal);
			} else {
				/* If policy member is given, remove the member from the policy list */
				retVal = secPolicyRemoveNos(
							(char *)pFCAuthLocalDeletePolicyMemberMessage->getPolicyName().c_str(),
                            (char *)pFCAuthLocalDeletePolicyMemberMessage->getPolicyEntryList().c_str());

				if (0 == retVal) {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyDeletePlugin: backend secPolicyRemove success\n");
					startTransaction();
					updateWaveManagedObject(pDefinedPolicyLocalManagedObject);
					/* Remove only the member */
					pDefinedPolicyLocalManagedObject->removePolicyMembers(
								pFCAuthLocalDeletePolicyMemberMessage->getPolicyEntryList());
					commitTransaction();
				} else {
					trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyDeletePlugin: backend secPolicyRemove failure\n");
					status = SecpolicyRespToStatus(retVal);
                    if (SECPOLICY_GENERIC_FAILURE == status) {
                        status = SECPOLICY_MEMBER_DELETE_FAILED;
                    }
				}
				tracePrintf (TRACE_LEVEL_INFO, true, true,
                	"secpolicy remove plugin %s %s %d\n",
					(char *)pFCAuthLocalDeletePolicyMemberMessage->getPolicyName().c_str(),
					(char *)pFCAuthLocalDeletePolicyMemberMessage->getPolicyEntryList().c_str(),
                	retVal);
			}
		} else {
			trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::secPolicyDelete: No policy/member to remove\n");
			if (pFCAuthLocalDeletePolicyMemberMessage->getPolicyEntryList().empty()) {
				status = SECPOLICY_DELETE_NO_POLICY;
			} else {
				status = SECPOLICY_DELETE_NO_MEMBER;
			}
		}
		if (pDefinedPolicyLocalManagedObject)
			delete pDefinedPolicyLocalManagedObject;
        return (status);
	}
    void  LocalSecPolicyWorker::FCAuthLocalAddPolicyMemberMessageHandler( FCAuthLocalAddPolicyMemberMessage *pFCAuthLocalAddPolicyMemberMessage)
    {
		trace (TRACE_LEVEL_DEBUG, "LocalSecPolicyWorker::FCAuthLocalAddPolicyMemberMessageHandler()");

		if (NULL == pFCAuthLocalAddPolicyMemberMessage) {
			trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
			prismAssert (false, __FILE__, __LINE__);
		}
		if (!isSupportedPlatform()) {
            trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::FCAuthLocalAddPolicyMemberMessageHandler "
                    "Command not supported in this platform");
            pFCAuthLocalAddPolicyMemberMessage->setCompletionStatus(SECPOLICY_UNSUPPORTED_PLATFORM);
            reply(pFCAuthLocalAddPolicyMemberMessage);
            return;
        }
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		if (pFCAuthLocalAddPolicyMemberMessage->getDefinedFlag()) {
			status = secPolicyDefinedAddPlugin(pFCAuthLocalAddPolicyMemberMessage);
		} else {
			status = secPolicyActiveAddPlugin(pFCAuthLocalAddPolicyMemberMessage);
		}

		pFCAuthLocalAddPolicyMemberMessage->setCompletionStatus(status);
		reply (pFCAuthLocalAddPolicyMemberMessage);
    }

    void  LocalSecPolicyWorker::FCAuthLocalDeletePolicyMemberMessageHandler( FCAuthLocalDeletePolicyMemberMessage *pFCAuthLocalDeletePolicyMemberMessage)
    {

		trace (TRACE_LEVEL_DEBUG, "LocalSecPolicyWorker::FCAuthLocalDeletePolicyMemberMessageHandler()");

        if (NULL == pFCAuthLocalDeletePolicyMemberMessage) {
            trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
            prismAssert (false, __FILE__, __LINE__);
        }
		if (!isSupportedPlatform()) {
            trace (TRACE_LEVEL_INFO, "LocalSecPolicyWorker::FCAuthLocalDeletePolicyMemberMessageHandler "
                    "Command not supported in this platform");
            pFCAuthLocalDeletePolicyMemberMessage->setCompletionStatus(SECPOLICY_UNSUPPORTED_PLATFORM);
            reply(pFCAuthLocalDeletePolicyMemberMessage);
            return;
        }
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		status = secPolicyDeletePlugin(pFCAuthLocalDeletePolicyMemberMessage);

        pFCAuthLocalDeletePolicyMemberMessage->setCompletionStatus(status);
        reply (pFCAuthLocalDeletePolicyMemberMessage);
    }

	ResourceId LocalSecPolicyWorker::SecpolicyRespToStatus(UI32 respCode)
	{
	    switch (respCode) {
	        case SEC_NO_ERROR:
	            return (WAVE_MESSAGE_SUCCESS);
			case SEC_NO_MEMORY:
				return (SECPOLICY_NO_MEMORY);		
	        case SEC_NO_ACCESS:
	            return (SECPOLICY_NO_ACCESS); 
			case SEC_INPUT_ERROR:
				return (SECPOLICY_INPUT_ERROR);
			case SEC_DATA_ERROR:
				return (SECPOLICY_DATA_ERROR);
			case SEC_PERM_DENY:
				return (SECPOLICY_PERM_DENY);
			case SEC_DB_TOO_LARGE:
				return (SECPOLICY_DB_TOO_LARGE);
	        default:
        	    return (SECPOLICY_GENERIC_FAILURE);
    	}
	}

	int LocalSecPolicyWorker::isSupportedPlatform()
	{
		int swModel = get_pod_SWBD();
		int isSupported = 0;
		trace (TRACE_LEVEL_INFO, string("LocalSecPolicyWorker::isSupportedPlatform: swModel:  ") + swModel);

		if ((SWBD_ELARA2F != swModel) && (SWBD_CALLISTOF != swModel)) {
			isSupported = 0;
        } else {
            isSupported = 1;
        }

        return (isSupported);
	}
}
