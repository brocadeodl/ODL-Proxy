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
 *   Author : Ayush Jaiswal												   *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Utils/StringUtils.h"
#include "WaveResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIds.h"
#include "DcmResourceIdEnums.h"

#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "ThresholdMonitor/Global/GlobalSecShowWorker.h"
#include "ThresholdMonitor/Global/SecGlobalShowMessage.h"
#include "ThresholdMonitor/Local/SecLocalShowMessage.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include <hasm/ls.h>
#include <em/em_if.h>
namespace DcmNs
{
	GlobalSecShowWorker::GlobalSecShowWorker ( ThresholdMonitorGlobalObjectManager *pThresholdMonitorGlobalObjectManager)
		: WaveWorker  (pThresholdMonitorGlobalObjectManager)
		{

			addOperationMap (SECURITYGLOBALSHOW, reinterpret_cast<PrismMessageHandler> (&GlobalSecShowWorker::SecGlobalShowMessageHandler));
		}
	GlobalSecShowWorker::~GlobalSecShowWorker ()
	{
	}
	
	PrismMessage  *GlobalSecShowWorker::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;
		switch (operationCode)
		{
			case SECURITYGLOBALSHOW :
				pPrismMessage = new SecGlobalShowMessage();
				break;
			default :
				pPrismMessage = NULL;
		}
		return (pPrismMessage);
			
	}

	void GlobalSecShowWorker::SecGlobalShowMessageHandler(SecGlobalShowMessage *pSecGlobalShowMessage)
	{
		trace (TRACE_LEVEL_INFO, "Entering SecGlobalShowMessageHandler..");
		UI32 areaName = pSecGlobalShowMessage->getAreaName();
		trace (TRACE_LEVEL_INFO, string("SfpGlobalShowMessageHandler areaName-")+areaName);
		
		/* Define sequencer steps to send global message to wave cluster*/
		PrismLinearSequencerStep sequencerSteps[] = 
		{
			reinterpret_cast<PrismLinearSequencerStep> (&GlobalSecShowWorker::sendShowSecCommandToCluster),
			reinterpret_cast<PrismLinearSequencerStep> (&GlobalSecShowWorker::prismLinearSequencerSucceededStep),
			reinterpret_cast<PrismLinearSequencerStep> (&GlobalSecShowWorker::prismLinearSequencerFailedStep)
		};
		
		PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSecGlobalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		pPrismLinearSequencerContext->start ();
		return;
	}

	void GlobalSecShowWorker::sendShowSecCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
	{
		trace (TRACE_LEVEL_INFO, string("Entering sendShowSecCommandToCluster.."));

		SecGlobalShowMessage	*msg_p      = NULL;
		SecLocalShowMessage	*loc_msg_p  = NULL;
		WaveSendToClusterContext	*send_ctx_p = NULL;
		vector<UI32> locations;

		msg_p = dynamic_cast<SecGlobalShowMessage *>(seq_ctx_p->getPPrismMessage ());
		
		loc_msg_p = new SecLocalShowMessage();

		loc_msg_p->setAreaName(msg_p->getAreaName());
		/* Get locations to send the local message*/
		if (msg_p->getSendToAllNodes()) {
			FrameworkToolKit::getFullyConnectedLocations(locations);
			locations.push_back(FrameworkToolKit::getThisLocationId());	
		} else {
			locations.push_back(msg_p->getLocationId());
		}
		
		/*Define context for sending message to cluster node locations through Wave infra*/
		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&GlobalSecShowWorker::sendShowSecCommandToClusterCallback), 
													seq_ctx_p);
		if (!send_ctx_p)
			return;
		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
		trace(TRACE_LEVEL_INFO, "GlobalSecShowWorker::sendShowSecCommandToCluster setPPrismMessageForPhase1 done");

		if(!locations.empty()) {
			send_ctx_p->setLocationsToSendToForPhase1(locations);
		}
		send_ctx_p->setPartialSuccessFlag(true);
		sendToWaveCluster(send_ctx_p);
		trace(TRACE_LEVEL_INFO, "GlobalSecShowWorker::sendShowSecCommandToCluster sendToWaveCluster done");

		return;
	}

	/* Callback function handling through return status of messages at cluster node locations*/
	void GlobalSecShowWorker::sendShowSecCommandToClusterCallback (WaveSendToClusterContext *ctx_p)
	{
		trace(TRACE_LEVEL_INFO, "Entering GlobalSecShowWorker::sendShowSecCommandToClusterCallback");
		PrismLinearSequencerContext *seq_ctx_p = NULL;
		SecGlobalShowMessage *gl_msg_p = NULL;
		SecLocalShowMessage *loc_msg_p = NULL, *loc_msg_p_phase1 = NULL;
		UI32 i = 0;
				
		seq_ctx_p   = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
		gl_msg_p   = dynamic_cast<SecGlobalShowMessage *> (seq_ctx_p->getPPrismMessage ());
		loc_msg_p_phase1  = dynamic_cast<SecLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

		vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
		/* Get the completion status of messages at node locations and aggregate buffers from local */
		/* messages into global message buffer*/

		for (i = 0; i < locations.size(); i++) {
 			loc_msg_p = dynamic_cast<SecLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
			prismAssert (loc_msg_p != NULL, __FILE__, __LINE__);
			if (WAVE_MESSAGE_SUCCESS == loc_msg_p->getCompletionStatus()) {
				prismAssert (gl_msg_p != NULL, __FILE__, __LINE__);
				gl_msg_p->copyAllBuffers(*loc_msg_p);  // Copy buffers from local messages into global message's buffer
				trace(TRACE_LEVEL_INFO, "GlobalSecShowWorker::sendShowSecCommandToClusterCallback: Aggregated buffers");
			} else {
				ResourceId clusterCompletionStatus = loc_msg_p->getCompletionStatus();
				trace(TRACE_LEVEL_INFO, string("Response processing: The location: ")
					+ locations[i] + string("returned error: ")
					+ FrameworkToolKit::localize(clusterCompletionStatus));
				break;
			}
		}
		gl_msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
		trace(TRACE_LEVEL_INFO, "executeNextStep in sequential context");
		
		delete loc_msg_p_phase1;
		delete ctx_p;
		return;
	}
}
