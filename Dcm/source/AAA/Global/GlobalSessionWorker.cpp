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
 *   Author : noku                                                     *
 **************************************************************************/

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmResourceIdEnums.h"

#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/GlobalSessionWorker.h"
#include "AAA/Global/AAAGlobalGetActiveUsersMessage.h"
#include "AAA/Global/AAAGlobalGetLockedUsersMessage.h"
#include "AAA/Global/AAAGlobalTypes.h"
#include "AAA/Local/AAALocalGetActiveUsersMessage.h"
#include "AAA/Local/AAALocalGetLockedUsersMessage.h"

namespace DcmNs
{

	GlobalSessionWorker::GlobalSessionWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager)
		: WaveWorker  (pAAAGlobalObjectManager)
	{
		addOperationMap (AAAGLOBALGETACTIVEUSERS, reinterpret_cast<PrismMessageHandler> (&GlobalSessionWorker::AAAGlobalGetActiveUsersMessageHandler));
		addOperationMap (AAAGLOBALGETLOCKEDUSERS, reinterpret_cast<PrismMessageHandler> (&GlobalSessionWorker::AAAGlobalGetLockedUsersMessageHandler));
	}

	GlobalSessionWorker::~GlobalSessionWorker ()
	{
	}

	PrismMessage  *GlobalSessionWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{

			case AAAGLOBALGETACTIVEUSERS :
				pPrismMessage = new AAAGlobalGetActiveUsersMessage ();
				break;
			case AAAGLOBALGETLOCKEDUSERS :
				pPrismMessage = new AAAGlobalGetLockedUsersMessage ();
				break;
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	WaveManagedObject  *GlobalSessionWorker::createManagedObjectInstance(const string &managedClassName)
	{
		return NULL;
	}

	void  GlobalSessionWorker::AAAGlobalGetLockedUsersMessageHandler( AAAGlobalGetLockedUsersMessage *pAAAGlobalGetLockedUsersMessage)
	{
		trace (TRACE_LEVEL_INFO, string("GlobalSessionWorker::AAAGlobalGetLockedUsersMessageHandler Enter."));

		// Validations and Queries should go here
		if (pAAAGlobalGetLockedUsersMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
			prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		// send getlock message to cluster asynchronously with callback
		AAALocalGetLockedUsersMessage *pMessage = new AAALocalGetLockedUsersMessage ();

		WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalSessionWorker::LockedUsersSTCCallback), pAAAGlobalGetLockedUsersMessage);
		pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}

		sendToWaveCluster (pWaveSendToClusterContext);
	}

	void GlobalSessionWorker::LockedUsersSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		string username;
		UI32 numrecords, switchid;
		vector<UI32> switchIds;
		string formattedSwitchIds;
		char swidCstr[16];

		trace(TRACE_LEVEL_INFO, string("GlobalSessionWorker::LockedUsersSTCCallback"));

		// pOrigMsg is the CLI wysereaoperation worker message to send the response
		AAAGlobalGetLockedUsersMessage *pOrigMsg = (AAAGlobalGetLockedUsersMessage *)pWaveSendToClusterContext->getPCallerContext();
		// get location per switch in cluster to get each local msg response
		vector<LocationId> locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

		// if failed on all nodes, probably backend error
		// so try to respond with a meaningful error msg from backend
		if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
		{
			AAALocalGetLockedUsersMessage *pTempMsg = (AAALocalGetLockedUsersMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[0]);
			status = pTempMsg->getCompletionStatus();
			trace (TRACE_LEVEL_WARN, string("GlobalSessionWorker::LockedUsersSTCCallback error on all nodes - ") + FrameworkToolKit::localize (status));
			// don't need to free pTempMsg
	        delete pWaveSendToClusterContext;
	        pOrigMsg->setCompletionStatus(status);
			reply (pOrigMsg);
			return;
		}

		// get each switch in cluster responses
		// loop through data, add switchid and push to pOrigMsg
		switchIds.clear();
		pOrigMsg->clearNumVectors();
			
		// get location ids for return status or data
		for (unsigned int i = 0; i < locationsVector.size(); i++)
		{
			// loop through each location and get msg rsp pointer for loc
			AAALocalGetLockedUsersMessage *pWaveResultMsg = (AAALocalGetLockedUsersMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[i]);
			switchid = DcmToolKit::getMappedIdFromLocationId(locationsVector[i]);
			snprintf(swidCstr, sizeof (swidCstr), "%d", switchid);

			ResourceId lStatus = pWaveResultMsg->getCompletionStatus();
			if (WAVE_MESSAGE_SUCCESS == lStatus)
			{
				// loop through each record in response and set OrigMsg resp
				numrecords = pWaveResultMsg->getNumVectors();
				for (unsigned int j = 0; j < numrecords; j++)
				{
					if (pWaveResultMsg->getLockedUser(j, username))
					{
						pOrigMsg->setLockedUser(username, switchid);
					}
				}
			}
			else
			{
				// trace error
				trace (TRACE_LEVEL_WARN, string("GlobalSessionWorker::LockedUsersSTCCallback : FAILURE on switchid ") + string(swidCstr) + string(" with error - ") + FrameworkToolKit::localize (lStatus));
				switchIds.push_back(switchid);
				status = CLUSTER_ERROR_1001;
			}

			// if switchdIds is non-zero, partial cluster error so format ids
			if (0 != switchIds.size())
			{
				formattedSwitchIds = DcmToolKit::getFormattedSwitchIDs(switchIds);
				pOrigMsg->setMessageString(formattedSwitchIds);
			}
		}
		// don't need to free pWaveResultMsg

		// clean up allocated msgs
		AAALocalGetLockedUsersMessage *pLocalMsg = (AAALocalGetLockedUsersMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		delete(pLocalMsg);
		delete pWaveSendToClusterContext;

		// respond to original msg
		trace (TRACE_LEVEL_INFO, string("GlobalSessionWorker::LockedUsersSTCCallback Exiting"));
		pOrigMsg->setCompletionStatus(status);
		reply (pOrigMsg);
	}

	void  GlobalSessionWorker::AAAGlobalGetActiveUsersMessageHandler( AAAGlobalGetActiveUsersMessage *pAAAGlobalGetActiveUsersMessage)
	{
		trace (TRACE_LEVEL_INFO, string("GlobalSessionWorker::AAAGlobalGetActiveUsersMessageHandler Enter."));

		// Validations and Queries should go here
		if (pAAAGlobalGetActiveUsersMessage == NULL)
		{
			trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
			prismAssert (false, __FILE__, __LINE__);
		}
		vector<UI32> locations;

		FrameworkToolKit::getFullyConnectedLocations(locations);
		locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));

		// send getlog message to cluster asynchronously with callback
		AAALocalGetActiveUsersMessage *pMessage = new AAALocalGetActiveUsersMessage ();

		WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalSessionWorker::ActiveUsersSTCCallback), pAAAGlobalGetActiveUsersMessage);
		pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);
		if(!locations.empty()) {
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}

		sendToWaveCluster (pWaveSendToClusterContext);
	}

	void GlobalSessionWorker::ActiveUsersSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
	{
		vector<UI32> switchIds;
		string username, hostname, ipaddr, devicename, role;
		UI32 numrecords, switchid, sessionid, pid;
		DateTime timeoflog;
		char swidCstr[16];

		trace(TRACE_LEVEL_INFO, string("GlobalSessionWorker::ActiveUsersSTCCallback"));

		AAAGlobalGetActiveUsersMessage *pOrigMsg = (AAAGlobalGetActiveUsersMessage *)pWaveSendToClusterContext->getPCallerContext();
		// get location ids for return status or data
		vector<LocationId> locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
		ResourceId status = pWaveSendToClusterContext->getCompletionStatus();

		// if failed on all nodes, likely backend error, so
		// try propagate a meaningful backend error status instead of generic
		if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
		{
			// if failed on all nodes, report the reason for the first failure
			AAALocalGetActiveUsersMessage *pTempMsg = (AAALocalGetActiveUsersMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[0]);
			status = pTempMsg->getCompletionStatus();
			trace (TRACE_LEVEL_WARN, string("GlobalSessionWorker::ActiveUsersSTCCallback : error - ") + FrameworkToolKit::localize (status));
			// don't need to free pTempMsg
	        delete pWaveSendToClusterContext;
	        pOrigMsg->setCompletionStatus(status);
			reply (pOrigMsg);
			return;
		}

		// loop through each switch location in cluster to accumulate responses
		switchIds.clear();
		pOrigMsg->clearNumVectors();

		// loop through data, lookup role, add switchid and push to pOrigMsg
		for (unsigned int i = 0; i < locationsVector.size(); i++)
		{
			// loop through each location and get msg rsp pointer for location
			AAALocalGetActiveUsersMessage *pWaveResultMsg = (AAALocalGetActiveUsersMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[i]);
			switchid = DcmToolKit::getMappedIdFromLocationId(locationsVector[i]);
			snprintf(swidCstr, sizeof (swidCstr), "%d", switchid);

			ResourceId lStatus = pWaveResultMsg->getCompletionStatus();
			if (WAVE_MESSAGE_SUCCESS == lStatus)
			{
				// loop through all records in local response and add to
				// original message response
				numrecords = pWaveResultMsg->getNumVectors();
				for (unsigned int j = 0; j < numrecords; j++)
				{
					if (pWaveResultMsg->getActiveUser(j, username, hostname, sessionid, ipaddr, devicename, pid, timeoflog, role))
					{
						trace (TRACE_LEVEL_INFO, string("Role ") + role);
						// role = "not impl";
						// TODO: lkup role from username
						// since role not displayed, not implemented
						pOrigMsg->setActiveUser(username, hostname, sessionid, ipaddr, devicename, pid, timeoflog, switchid, role);
					}
				}
			}
			else
			{
				// cluster error, since already checked for error on all nodes
				trace (TRACE_LEVEL_WARN, string("GlobalSessionWorker::ActiveUsersSTCCallback : FAILURE on switchid ") + string(swidCstr) + string(" with error - ") + FrameworkToolKit::localize (lStatus));
				switchIds.push_back(switchid);
				status = CLUSTER_ERROR_1001;
			}

			// don't need to free pWaveResultMsg
		}

		// if switchids not zero, partial cluster error, so format ids
		if (0 != switchIds.size())
		{
			string formattedSwitchIds = DcmToolKit::getFormattedSwitchIDs(switchIds);
			pOrigMsg->setMessageString(formattedSwitchIds);
		}

		// clean up allocated messages
		AAALocalGetActiveUsersMessage *pLocalMsg = (AAALocalGetActiveUsersMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
		delete(pLocalMsg);
		delete pWaveSendToClusterContext;

		// respond back to original message
		trace (TRACE_LEVEL_INFO, string("GlobalSessionWorker::ActiveUsersSTCCallback Exiting"));
		pOrigMsg->setCompletionStatus(status);
		reply (pOrigMsg);
	}
}
