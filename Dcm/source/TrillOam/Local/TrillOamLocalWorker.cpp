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
 *   Author : mmahajan                                                     *
 **************************************************************************/
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "TrillOam/Local/TrillOamLocalObjectManager.h"
#include "TrillOam/Local/TrillOamLocalWorker.h"
#include "TrillOam/Local/TrillOamTrillOamPingRequestMessage.h"
#include "TrillOam/Local/TrillOamTrillOamTracerouteRequestMessage.h"
#include "TrillOam/Local/TrillOamTypes.h"
#include "ClientInterface/TrillOam/TrillOamPingMessage.h"
#include "ClientInterface/TrillOam/TrillOamTraceRouteMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
namespace DcmNs
{

    TrillOamLocalWorker::TrillOamLocalWorker ( TrillOamLocalObjectManager *pTrillOamLocalObjectManager)
        : WaveWorker  (pTrillOamLocalObjectManager)
    {
        addOperationMap (TRILLOAMTRILLOAMPINGREQUEST, reinterpret_cast<PrismMessageHandler> (&TrillOamLocalWorker::TrillOamTrillOamPingRequestMessageHandler));
        addOperationMap (TRILLOAMTRILLOAMTRACEROUTEREQUEST, reinterpret_cast<PrismMessageHandler> (&TrillOamLocalWorker::TrillOamTrillOamTracerouteRequestMessageHandler));
    }

    TrillOamLocalWorker::~TrillOamLocalWorker ()
    {
    }

    PrismMessage  *TrillOamLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case TRILLOAMTRILLOAMPINGREQUEST :
                pPrismMessage = new TrillOamTrillOamPingRequestMessage ();
                break;
            case TRILLOAMTRILLOAMTRACEROUTEREQUEST :
                pPrismMessage = new TrillOamTrillOamTracerouteRequestMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *TrillOamLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  TrillOamLocalWorker::TrillOamTrillOamPingRequestMessageHandler( TrillOamTrillOamPingRequestMessage *pTrillOamTrillOamPingRequestMessage)
    {
// Validations and Queries should go here
		trace (TRACE_LEVEL_INFO, "TrillOamLocalWorker::TrillOamTrillOamPingRequestMessageHandler: Enter \n");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&TrillOamLocalWorker::trillOamPing),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&TrillOamLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&TrillOamLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pTrillOamTrillOamPingRequestMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  TrillOamLocalWorker::TrillOamTrillOamTracerouteRequestMessageHandler( TrillOamTrillOamTracerouteRequestMessage *pTrillOamTrillOamTracerouteRequestMessage)
    {
// Validations and Queries should go here
		trace (TRACE_LEVEL_INFO, "TrillOamLocalWorker::TrillOamTrillOamTracerouteRequestMessageHandler: Enter \n");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&TrillOamLocalWorker::trillOamTraceroute),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&TrillOamLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&TrillOamLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pTrillOamTrillOamTracerouteRequestMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
	/*     * Send message to backend daemon     */         
ResourceId TrillOamLocalWorker::trillOamPing (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS; //temp hack
		UI32 cmd_code = 0;
        TrillOamTrillOamPingRequestMessage *pLocalMessage = dynamic_cast<TrillOamTrillOamPingRequestMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_INFO, "TrillOamLocalWorker::trillOamPing: Enter \n");
        /* Does Sanity check need here ??? */
        TrillOamPingMessage  *message =  new TrillOamPingMessage(pLocalMessage->getSrcmac(),
																 pLocalMessage->getDestmac(),
																 pLocalMessage->getVlanid(),
																 cmd_code);
        status = sendSynchronouslyToWaveClient("toam", message);

        pLocalMessage->setCompletionStatus(message->getCompletionStatus());

        if( status == WAVE_MESSAGE_SUCCESS)
            if(  pLocalMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                status =  pLocalMessage->getCompletionStatus();

        trace (TRACE_LEVEL_INFO, "TrillOamLocalWorker::trillOamPing: status  "+FrameworkToolKit::localize(status))
;
        return status;
    }

ResourceId TrillOamLocalWorker::trillOamTraceroute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
	
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 cmd_code = 0;

		TrillOamTrillOamTracerouteRequestMessage *pLocalMessage = dynamic_cast<TrillOamTrillOamTracerouteRequestMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());


        trace (TRACE_LEVEL_INFO, "TrillOamLocalWorker::trillOamTraceroute: Enter \n");
        /* Does Sanity check need here ??? */
        TrillOamTraceRouteMessage  *message = new TrillOamTraceRouteMessage(
														pLocalMessage->getSrcmac(),
														pLocalMessage->getDestmac(),
														pLocalMessage->getVlanid(),
														pLocalMessage->getRbridgeid(),
														pLocalMessage->getProtocol(),
														pLocalMessage->getSrcip(),
														pLocalMessage->getDestip(),
														pLocalMessage->getSrcport(),
														pLocalMessage->getDestport(),
														cmd_code, 
                                                        pLocalMessage->getSessionId(),
                                                        pLocalMessage->getBegin(), 
                                                        pLocalMessage->getMsgStatus());
        status = sendSynchronouslyToWaveClient("toam", message);
      	trace (TRACE_LEVEL_INFO,string("TrillOamLocalWorker::trillOamTraceroute: status ==> \n") + status);

        pLocalMessage->setCompletionStatus(message->getCompletionStatus());

        if( status == WAVE_MESSAGE_SUCCESS) {
            if(  pLocalMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
        	   trace (TRACE_LEVEL_ERROR, "TrillOamLocalWorker::trillOamTraceroute: WAVE_!__SUCCESS \n");
		    } else {
                pLocalMessage->copyAllBuffers(*message);
            }
		}

	    trace (TRACE_LEVEL_INFO, "TrillOamLocalWorker::trillOamTraceroute: status  "+FrameworkToolKit::localize(status));

        delete message;

        return pLocalMessage->getCompletionStatus();
    }

}
