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
 *   Copyright (C) 2013	 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalShowClrWorker.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalShowMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalClearMessage.h"
#include "ClientInterface/Dhcp/DhcpRelClientShowMessage.h"
#include "ClientInterface/Dhcp/DhcpRelClientClearMessage.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "ClientInterface/Dhcp/DhcpUtils.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

    DhcpRelLocalShowClrWorker::DhcpRelLocalShowClrWorker ( DhcpRelLocalObjectManager *pDhcpRelLocalObjectManager)
        : WaveWorker  (pDhcpRelLocalObjectManager)
    {
        addOperationMap (IPHELPSHOW, reinterpret_cast<PrismMessageHandler> (&DhcpRelLocalShowClrWorker	::DhcpRelLocalShowMessageHandler));
        addOperationMap (IPHELPCLEAR, reinterpret_cast<PrismMessageHandler> (&DhcpRelLocalShowClrWorker::DhcpRelLocalClearMessageHandler));
    }

    DhcpRelLocalShowClrWorker::~DhcpRelLocalShowClrWorker ()
    {
    }

    PrismMessage  *DhcpRelLocalShowClrWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case IPHELPSHOW:
                pPrismMessage = new DhcpRelLocalShowMessage ();
                break;
            case IPHELPCLEAR:
                pPrismMessage = new DhcpRelLocalClearMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DhcpRelLocalShowClrWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId 
    DhcpRelLocalShowClrWorker::sendShowToIphelperDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        DhcpRelLocalShowMessage *msg_p = dynamic_cast<DhcpRelLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
        DhcpRelClientShowMessage *cl_msg_p = NULL;
        ResourceId      status = WAVE_MESSAGE_SUCCESS;

        if (! msg_p)
            return WAVE_MESSAGE_ERROR;

        trace(TRACE_LEVEL_DEBUG, "Entering  DhcpRelLocalShowClrWorker::sendShowToIphelperDaemon");


        cl_msg_p = new DhcpRelClientShowMessage(msg_p->getCmdCode(), msg_p->getIfName(), 
					msg_p->getIfType(), msg_p->getVlanValue() );

        status = sendSynchronouslyToWaveClient("iphelpd", cl_msg_p);

        if(WAVE_MESSAGE_SUCCESS == status)
        {
			status = cl_msg_p->getClientStatus();
        	trace (TRACE_LEVEL_DEBUG, string("DhcpRelLocalShowClrWorker::sendShowToIphelperDaemon - DHCP show message sent successfully to iphelpd backend, status = ") + status);
            if ((0 != status) && (WAVE_MESSAGE_SUCCESS != status))
              status = DhcpUtils::mapBackEndErrorsToDcmdErrors(status);
		}
		else
            trace (TRACE_LEVEL_ERROR, "DhcpRelLocalShowClrWorker::sendShowToIphelperDaemon - DHCP show message not sent to iphelpd backend");

        msg_p->setCompletionStatus(status);

        msg_p->copyAllBuffers(*cl_msg_p);

        delete cl_msg_p;
        return status;
    }

    void
    DhcpRelLocalShowClrWorker::DhcpRelLocalShowMessageHandler (
            DhcpRelLocalShowMessage *pDhcpRelLocalShowMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering DhcpRelLocalShowClrWorker::DhcpRelLocalShowMessageHandler\n");
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&DhcpRelLocalShowClrWorker::sendShowToIphelperDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&DhcpRelLocalShowClrWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&DhcpRelLocalShowClrWorker::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pDhcpRelLocalShowMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

    }

    ResourceId 
    DhcpRelLocalShowClrWorker::sendClearToIphelperDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        DhcpRelLocalClearMessage *msg_p = dynamic_cast<DhcpRelLocalClearMessage *>
                                      (ctx_p->getPPrismMessage ());
        DhcpRelClientClearMessage *cl_msg_p = NULL;
        ResourceId      status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, "Entering DhcpRelLocalShowClrWorker::sendClearToIphelperDaemon");

        if (! msg_p)
            return WAVE_MESSAGE_ERROR;

        cl_msg_p = new DhcpRelClientClearMessage();
		cl_msg_p->setCmdCode(msg_p->getCmdCode()); 
		cl_msg_p->setServerAddress(msg_p->getServerAddress());

        status = sendSynchronouslyToWaveClient("iphelpd", cl_msg_p);

        if(WAVE_MESSAGE_SUCCESS == status)
        {
			status = cl_msg_p->getClientStatus();
        	trace (TRACE_LEVEL_DEBUG, string("DhcpRelLocalShowClrWorker::sendClearToIphelperDaemon - DHCP clear message sent successfully to iphelpd backend, status = ") + status);
            if ((0 != status) && (WAVE_MESSAGE_SUCCESS != status))
              status = DhcpUtils::mapBackEndErrorsToDcmdErrors(status);
		}
		else
            trace (TRACE_LEVEL_ERROR, "DhcpRelLocalShowClrWorker::sendClearToIphelperDaemon - DHCP clear message not sent to iphelpd backend");

        msg_p->setCompletionStatus(status);

        delete cl_msg_p;
        return status;
    }


    void DhcpRelLocalShowClrWorker::DhcpRelClearCmdClusterCallback(WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext  *seq_ctx_p = NULL;
        DhcpRelLocalClearMessage     *resp_msg_p = NULL;
        DhcpRelLocalClearMessage     *sw_resp_p = NULL;
        ResourceId                   firstStatus = WAVE_MESSAGE_SUCCESS; 

        trace(TRACE_LEVEL_DEBUG, "Entering DhcpRelLocalShowClrWorker::DhcpRelClearCmdClusterCallback");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        if (!seq_ctx_p)
            return;

        resp_msg_p =  dynamic_cast<DhcpRelLocalClearMessage*> (ctx_p->getPPrismMessageForPhase1());
        if (!resp_msg_p)
            return;

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        trace(TRACE_LEVEL_DEBUG, string("DhcpRelClearCmdClusterCallback: Response processing - Num responses is ") + locations.size());

        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<DhcpRelLocalClearMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (! sw_resp_p)
               continue;

            if (WAVE_MESSAGE_SUCCESS != sw_resp_p->getCompletionStatus())
            {
                trace(TRACE_LEVEL_ERROR, (string("DhcpRelClearCmdClusterCallback: Response processing - The location: ") + locations[i]) + string(" returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
                if (WAVE_MESSAGE_SUCCESS == firstStatus)
                     firstStatus = sw_resp_p->getCompletionStatus();
            }
        }

        /* Return with status of first failure */
        seq_ctx_p->executeNextStep(firstStatus);

        delete resp_msg_p;
        delete ctx_p;
        return;
    }


    void DhcpRelLocalShowClrWorker::sendClearToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
        trace(TRACE_LEVEL_DEBUG, "Entering DhcpRelLocalShowClrWorker::sendClearToCluster\n");
        DhcpRelLocalClearMessage *msg_p = dynamic_cast<DhcpRelLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());
        if (!msg_p)
            return;

        DhcpRelLocalClearMessage *loc_msg_p = new DhcpRelLocalClearMessage();
        if (!loc_msg_p)
            return; 
        loc_msg_p->setCmdCode(msg_p->getCmdCode());       
        loc_msg_p->setServerAddress(msg_p->getServerAddress());       
        loc_msg_p->setIsDistributionReqd(false);

        WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&DhcpRelLocalShowClrWorker::DhcpRelClearCmdClusterCallback), seq_ctx_p);
        if (!send_ctx_p)
            return;

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        send_ctx_p->setLocationsToSendToForPhase1 (loc_msg_p->getLocationIds());
        sendToWaveCluster(send_ctx_p);
        return;
    }

    void
    DhcpRelLocalShowClrWorker::DhcpRelLocalClearMessageHandler (
            DhcpRelLocalClearMessage *pDhcpRelLocalClearMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering DhcpRelLocalShowClrWorker::DhcpRelLocalClearMessageHandler\n");
        if (pDhcpRelLocalClearMessage->getIsDistributionReqd())
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&DhcpRelLocalShowClrWorker::sendClearToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&DhcpRelLocalShowClrWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&DhcpRelLocalShowClrWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pLinearSeqContext = new  PrismLinearSequencerContext( pDhcpRelLocalClearMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pLinearSeqContext->start();
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep>
                      (&DhcpRelLocalShowClrWorker::sendClearToIphelperDaemon),

                reinterpret_cast<PrismSynchronousLinearSequencerStep>
                      (&DhcpRelLocalShowClrWorker::prismSynchronousLinearSequencerSucceededStep),

                reinterpret_cast<PrismSynchronousLinearSequencerStep>
                      (&DhcpRelLocalShowClrWorker::prismSynchronousLinearSequencerFailedStep)   
            };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(
                        pDhcpRelLocalClearMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();
         }
         return;
    }
}
