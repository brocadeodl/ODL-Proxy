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
 *   Author : bgangise					                                   *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmCore/DcmToolKit.h"

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalShowWorker.h"
#include "APPM/Local/APPMLocalShowMessage.h"
#include "APPM/Local/APPMLocalTypes.h"

#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/APPM/APPMShowMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    APPMLocalShowWorker::APPMLocalShowWorker ( APPMLocalObjectManager *pAPPMLocalObjectManager)
        : WaveWorker  (pAPPMLocalObjectManager)
    {
        addOperationMap (APPMLOCALSHOWPROFILE, reinterpret_cast<PrismMessageHandler> (&APPMLocalShowWorker::APPMLocalShowMessageHandler));
    }

    APPMLocalShowWorker::~APPMLocalShowWorker ()
    {
    }

    PrismMessage  *APPMLocalShowWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case APPMLOCALSHOWPROFILE:
                pPrismMessage = new APPMLocalShowMessage();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *APPMLocalShowWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void
    APPMLocalShowWorker::showMessageClusterStepCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        PrismLinearSequencerContext *pPrismLinearSequencerContext = NULL;
        APPMLocalShowMessage *pReqMsg = NULL, *pRespMsg = NULL, *pResultMsg = NULL;

        pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>
            (pWaveSendToClusterContext->getPCallerContext());

        pReqMsg = dynamic_cast<APPMLocalShowMessage *>
                    (pPrismLinearSequencerContext->getPPrismMessage ());
        pRespMsg = dynamic_cast<APPMLocalShowMessage *>
                    (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
                    
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
        
        if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {
            
            vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
            UI32    i = 0;
            
            for (i = 0; i < locations.size(); i++) {
                
                ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(pWaveSendToClusterContext, locations[i]);
                
                if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {
                    pResultMsg = dynamic_cast<APPMLocalShowMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));
                    pReqMsg->copyAllBuffers(*pResultMsg);
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, string("Response processing: The location: ")
                            + locations[i] + string("returned error: ")  + FrameworkToolKit::localize(statusPerLocationId));
                }
            }
        } else {
            APPM_PLUG_TRACE(TRACE_LEVEL_ERROR, string("showMessageClusterStepCallback returned error: ") + FrameworkToolKit::localize(sendToClusterCompletionStatus));
        }
        
        pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);

        delete pRespMsg;
        delete pWaveSendToClusterContext;
        return;
    }
                                                                     
	ResourceId APPMLocalShowWorker::showMessageClusterStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int status = WAVE_MESSAGE_SUCCESS; 
		APPMLocalShowMessage *pMsg = NULL;
		APPMLocalShowMessage *pMsgLocal = NULL;
		WaveSendToClusterContext *pWaveSendToClusterContext = NULL;
		UI32 locationId = 0;
		vector<UI32> locations;

		pMsg = dynamic_cast<APPMLocalShowMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);
		pMsgLocal = new APPMLocalShowMessage(pMsg->getCmdCode(), pMsg->getIfName(),
			pMsg->getProfileName(), false, pMsg->getLastProfileMac(), pMsg->getNumberOfRecords(), pMsg->getFetchModelType());
		pMsgLocal->setInterface(pMsg->getInterface());
		pMsgLocal->setPortProfileDomainName(pMsg->getPortProfileDomainName());

		pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&APPMLocalShowWorker::showMessageClusterStepCallback), pPrismLinearSequencerContext);

		if (!pWaveSendToClusterContext) {
			delete pMsgLocal;
			return (status);
		}

		pWaveSendToClusterContext->setPPrismMessageForPhase1(pMsgLocal);

		if (pMsg->getLocationId() > 0) {
			locationId = pMsg->getLocationId();
		} else {
			locationId = NsmUtils::getLocalLocationId();
		}
		//To handle the command: " show port-profile interface all"

		if (locationId) {
			locations.push_back(locationId);
			pWaveSendToClusterContext->setLocationsToSendToForPhase1(locations);
		}

		sendToWaveCluster(pWaveSendToClusterContext);
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
		return (status);
	}

	ResourceId APPMLocalShowWorker::showMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		int status = WAVE_MESSAGE_SUCCESS;
		SI32 mappedId = 0;
		string ifName;
		LocationId locationId;

		APPMLocalShowMessage *pAPPMLocalShowMessage =
			dynamic_cast<APPMLocalShowMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		mappedId = DcmToolKit::getLocalMappedId();
		if (mappedId == (-1)) {
			mappedId = 0;
		}
		if(pAPPMLocalShowMessage->getInterface().compare("all"))
		{	
			if(0 != getIfShortName(pAPPMLocalShowMessage->getInterface().c_str(), pAPPMLocalShowMessage->getIfName().c_str(), ifName, locationId) )
			{
				APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Error if Id");
				//interface all commands hitting this so commenting return
				//TODO need to check cmdcode and handle properly
				//		return status;
			}
		}

        APPMShowMessage *pMsg = new APPMShowMessage
			(pAPPMLocalShowMessage->getCmdCode(), ifName,
			 pAPPMLocalShowMessage->getProfileName(), mappedId, pAPPMLocalShowMessage->getLastProfileMac(), pAPPMLocalShowMessage->getNumberOfRecords(), pAPPMLocalShowMessage->getFetchModelType());
		pMsg->setPortProfileDomainName(pAPPMLocalShowMessage->getPortProfileDomainName());
        prismAssert(NULL != pMsg, __FILE__, __LINE__);

		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " Local message received");

		trace(
			TRACE_LEVEL_DEBUG,
			string(
					"APPMLocalShowWorker  Sending message to nsm daemon : CmdCode <")
					+ pMsg->getCmdCode() + "> IfType <" + pMsg->getIfName()
					+ "> Fetch Model <" + pMsg->getFetchModelType() + ">");

        status = sendSynchronouslyToWaveClient("nsm",pMsg);

        if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			status = pMsg->getCompletionStatus();
			delete pMsg;
			return status;
        }

		APPM_PLUG_TRACE(TRACE_LEVEL_DEVEL, " message sent to NSM backend");
		//pAPPMLocalShowMessage->transferOutputBuffer(pMsg);
		pAPPMLocalShowMessage->copyAllBuffers(*pMsg);
		
		delete pMsg;
		return (status);
	}

    void  APPMLocalShowWorker::APPMLocalShowMessageHandler(APPMLocalShowMessage *pAPPMLocalShowMessage)
    {
		// Validations and Queries should go here
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_DEBUG);
		if (pAPPMLocalShowMessage->getIsGwRequest()) {
			PrismLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismLinearSequencerStep>(&APPMLocalShowWorker::showMessageClusterStep),
				reinterpret_cast<PrismLinearSequencerStep>(&APPMLocalShowWorker::prismLinearSequencerSucceededStep),
				reinterpret_cast<PrismLinearSequencerStep>(&APPMLocalShowWorker::prismLinearSequencerFailedStep)
			};

			PrismLinearSequencerContext *pPrismLinearSequencerContext =
				new PrismLinearSequencerContext(pAPPMLocalShowMessage,
					this, sequencerSteps,
					sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

			pPrismLinearSequencerContext->start ();
			return;
		} else {
			PrismSynchronousLinearSequencerStep sequencerSteps[] =
			{
				reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalShowWorker::showMessageStep),
				reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalShowWorker::prismSynchronousLinearSequencerSucceededStep),
				reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMLocalShowWorker::prismSynchronousLinearSequencerFailedStep)
			};

			PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMLocalShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

			pPrismSynchronousLinearSequencerContext->execute ();
		}
		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_DEBUG);
	}
}
