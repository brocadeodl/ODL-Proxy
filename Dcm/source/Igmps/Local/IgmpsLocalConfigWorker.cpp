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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Igmps/Local/IgmpsLocalConfigWorker.h"
#include "Igmps/Local/IgmpsLocalShowMessage.h"
#include "Igmps/Local/IgmpsLocalDebugMessage.h"
#include "Igmps/Local/IgmpsLocalClearMessage.h"
#include "Igmps/Local/IgmpsUpdateIgmpsConfigMessage.h"
#include "Igmps/Local/IgmpsTypes.h"
#include "ClientInterface/Igmps/IgmpsClientShowMessage.h"
#include "ClientInterface/Igmps/IgmpsClientDebugMessage.h"
#include "ClientInterface/Igmps/IgmpsClientClearMessage.h"
#include "ClientInterface/Igmps/IgmpsClientUpdateIgmpsConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Igmps/IgmpsUtils.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{

    IgmpsLocalConfigWorker::IgmpsLocalConfigWorker ( IgmpsLocalObjectManager *pIgmpsLocalObjectManager)
        : WaveWorker  (pIgmpsLocalObjectManager)
    {
        addOperationMap (IGMPSUPDATEIGMPSCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpsLocalConfigWorker::IgmpsUpdateIgmpsConfigMessageHandler));
        addOperationMap (IGMPSLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&IgmpsLocalConfigWorker::IgmpsLocalShowMessageHandler));
        addOperationMap (IGMPSLOCALDEBUG, reinterpret_cast<PrismMessageHandler> (&IgmpsLocalConfigWorker::IgmpsLocalDebugMessageHandler));
        addOperationMap (IGMPSLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&IgmpsLocalConfigWorker::IgmpsLocalClearMessageHandler));
    }

    IgmpsLocalConfigWorker::~IgmpsLocalConfigWorker ()
    {
    }

    PrismMessage  *IgmpsLocalConfigWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case IGMPSUPDATEIGMPSCONFIG :
                pPrismMessage = new IgmpsUpdateIgmpsConfigMessage ();
                break;
            case IGMPSLOCALSHOW:
                pPrismMessage = new IgmpsLocalShowMessage ();
                break;
            case IGMPSLOCALDEBUG:
                pPrismMessage = new IgmpsLocalDebugMessage ();
                break;
            case IGMPSLOCALCLEAR:
                pPrismMessage = new IgmpsLocalClearMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IgmpsLocalConfigWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId IgmpsLocalConfigWorker::updateIgmpsLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        IgmpsUpdateIgmpsConfigMessage *pIgmpsUpdateIgmpsConfigMessage = dynamic_cast<IgmpsUpdateIgmpsConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        if (! pIgmpsUpdateIgmpsConfigMessage)
            return WAVE_MESSAGE_ERROR;

        int status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEBUG, "IgmpsLocalConfigWorker::updateIgmpsLocalConfigStep: Entered....");
        IgmpsClientUpdateIgmpsConfigMessage *m = new IgmpsClientUpdateIgmpsConfigMessage ();
        m->setOpCode(pIgmpsUpdateIgmpsConfigMessage->getOpCode());
        switch(pIgmpsUpdateIgmpsConfigMessage->getOpCode())
        {
            case IGMPS_GLOBAL_SNOOP_ENABLE:
                 m->setEnable(pIgmpsUpdateIgmpsConfigMessage->getEnable());
            break;
            case IGMPS_GLOBAL_RESTRICT_UNKNOWN_MCAST_ENABLE:
                 m->setRestrictUnknownMcast(pIgmpsUpdateIgmpsConfigMessage->getRestrictUnknownMcast());
                 break;
            default:
                if(m)
                {
                    delete m;
                }
                return WAVE_MESSAGE_ERROR;
        }
        status = sendSynchronouslyToWaveClient ("IGMP", m);

        ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pIgmpsUpdateIgmpsConfigMessage, m, status);
        delete m;
        return retStatus;
    }

    void  IgmpsLocalConfigWorker::IgmpsUpdateIgmpsConfigMessageHandler( IgmpsUpdateIgmpsConfigMessage *pIgmpsUpdateIgmpsConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpsLocalConfigWorker::updateIgmpsLocalConfigStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIgmpsUpdateIgmpsConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void
    IgmpsLocalConfigWorker::IgmpsLocalShowSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        IgmpsLocalShowMessage           *req_msg_p = NULL,
                                        *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;
        ResourceId                      returnStatus = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::IgmpsLocalShowSendToClusterCallback\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<IgmpsLocalShowMessage *>
                    (seq_ctx_p->getPPrismMessage ());
        
        resp_msg_p =  dynamic_cast<IgmpsLocalShowMessage *>
                     (ctx_p->getPPrismMessageForPhase1());

        if (!req_msg_p || !resp_msg_p)
            return;

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<IgmpsLocalShowMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (! sw_resp_p)
                continue;

            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                 trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ") 
                            + locations[i] + string("returned error: ")  + 
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
                 if (WAVE_MESSAGE_SUCCESS == returnStatus)
                     returnStatus = sw_resp_p->getCompletionStatus();
            }
        }

        delete resp_msg_p;
        delete ctx_p;

        /* Return with status of first failure */
        seq_ctx_p->executeNextStep(returnStatus);
        return;
    }

    void
    IgmpsLocalConfigWorker::sendShowCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        IgmpsLocalShowMessage        *msg_p = NULL;
        IgmpsLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;
        bool    				     b_isMgmtClusterCreated = false; 

		ResourceId thisNodeRole = DcmToolKit::getThisVcsNodeClusterType ();
		if ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == thisNodeRole )
		{
			b_isMgmtClusterCreated = true;
		}

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::sendShowCommandToCluster\n");
        trace(TRACE_LEVEL_DEBUG, string("DcmToolKit::getThisVcsNodeClusterType() returns value =  ") + thisNodeRole);
        trace(TRACE_LEVEL_DEBUG, string("b_isMgmtClusterCreated = ") + b_isMgmtClusterCreated);
        msg_p = dynamic_cast<IgmpsLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        if (! msg_p)
            return;

        loc_msg_p = new IgmpsLocalShowMessage(msg_p->getGrpName(),
                                             msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false, b_isMgmtClusterCreated);

        trace (TRACE_LEVEL_DEBUG, string("Sending Show Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() + 
                        string (" grpName =")  + msg_p->getGrpName() +
                        string (" ifName =")  + msg_p->getIfName());

        send_ctx_p = new WaveSendToClusterContext(this, 
                reinterpret_cast<PrismAsynchronousCallback>
                (&IgmpsLocalConfigWorker::IgmpsLocalShowSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId 
    IgmpsLocalConfigWorker::sendShowToIgmpsDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        IgmpsLocalShowMessage *msg_p = dynamic_cast<IgmpsLocalShowMessage *>
                                      (ctx_p->getPPrismMessage ());
        IgmpsClientShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;

        if (! msg_p)
            return WAVE_MESSAGE_ERROR;

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::sendShowToIgmpsDaemon");
        mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == (-1)) { 
            mappedId = 0;
        }

        trace(TRACE_LEVEL_DEBUG, "IgmpsLocalConfigWorker::sendShowToIgmpsDaemon::GrpName="
            + msg_p->getGrpName() + " IfName=" + msg_p->getIfName() + " CmdCode=" + msg_p->getCmdCode() + " mappedId=" + mappedId);

        cl_msg_p = new IgmpsClientShowMessage(msg_p->getGrpName(), msg_p->getIfName(), msg_p->getCmdCode(), mappedId, 
										       msg_p->getIsMgmtCluster () );

        status = sendSynchronouslyToWaveClient("IGMP", cl_msg_p);

        if (WAVE_MESSAGE_SUCCESS == IgmpsUtils::IgmpsStatusReturn (msg_p, cl_msg_p, status))
            msg_p->copyAllBuffers(*cl_msg_p);

        delete cl_msg_p;
        return msg_p->getCompletionStatus();
    }

    void
    IgmpsLocalConfigWorker::IgmpsLocalShowMessageHandler (
            IgmpsLocalShowMessage *pIgmpsLocalShowMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::IgmpsLocalShowMessageHandler\n");
        if (pIgmpsLocalShowMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IgmpsLocalConfigWorker::sendShowCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IgmpsLocalConfigWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IgmpsLocalConfigWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pIgmpsLocalShowMessage, 
                    this, sequencerSteps, 
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IgmpsLocalConfigWorker::sendShowToIgmpsDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pIgmpsLocalShowMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

    ResourceId 
    IgmpsLocalConfigWorker::sendDebugToIgmpsDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        IgmpsLocalDebugMessage *msg_p = dynamic_cast<IgmpsLocalDebugMessage *>
                                      (ctx_p->getPPrismMessage ());
        IgmpsClientDebugMessage *cl_msg_p = NULL;
        ResourceId      status;

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::sendDebugToIgmpsDaemon");

        if (! msg_p)
            return WAVE_MESSAGE_ERROR;

        trace(TRACE_LEVEL_DEBUG, "IgmpsLocalConfigWorker::sendDebugToIgmpsDaemon::GrpName="
            + msg_p->getGrpName() + " IfName=" + msg_p->getIfName() + " CmdCode=" + msg_p->getCmdCode() + " Negation=" + msg_p->getNegation());

        cl_msg_p = new IgmpsClientDebugMessage(msg_p->getGrpName(), msg_p->getIfName(), msg_p->getCmdCode(), msg_p->getNegation());

        status = sendSynchronouslyToWaveClient("IGMP", cl_msg_p);

        if (WAVE_MESSAGE_SUCCESS == IgmpsUtils::IgmpsStatusReturn (msg_p, cl_msg_p, status))
            msg_p->copyAllBuffers(*cl_msg_p);

        delete cl_msg_p;
        return msg_p->getCompletionStatus();
    }

    void  
    IgmpsLocalConfigWorker::IgmpsLocalDebugMessageHandler (
            IgmpsLocalDebugMessage *pIgmpsLocalDebugMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::IgmpsLocalDebugMessageHandler\n");
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>
                (&IgmpsLocalConfigWorker::sendDebugToIgmpsDaemon),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>
                (&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>
                (&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)   
        };

        PrismSynchronousLinearSequencerContext 
            *pPrismSynchronousLinearSequencerContext= 
            new PrismSynchronousLinearSequencerContext(
                pIgmpsLocalDebugMessage, this, sequencerSteps, 
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void
    IgmpsLocalConfigWorker::IgmpsLocalClearSendToClusterCallback (
            WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p = NULL;
        IgmpsLocalClearMessage          *resp_msg_p = NULL,
                                        *sw_resp_p = NULL;
        //IgmpsLocalClearMessage          *req_msg_p = NULL;
        ResourceId                      returnStatus = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::IgmpsLocalClearSendToClusterCallback\n");

        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>
            (ctx_p->getPCallerContext());

        //req_msg_p = dynamic_cast<IgmpsLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());
        
        resp_msg_p =  dynamic_cast<IgmpsLocalClearMessage *> (ctx_p->getPPrismMessageForPhase1());

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<IgmpsLocalClearMessage *>
                (ctx_p->getResultingMessageForPhase1(locations[i]));
            if (! sw_resp_p)
                continue;

            if (WAVE_MESSAGE_SUCCESS != sw_resp_p->getCompletionStatus()) {
                 trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ") 
                            + locations[i] + string("returned error: ")  + 
                 FrameworkToolKit::localize(sw_resp_p->getCompletionStatus())); 
                 if (WAVE_MESSAGE_SUCCESS == returnStatus)
                     returnStatus = sw_resp_p->getCompletionStatus();
            }
        }

        delete resp_msg_p;
        delete ctx_p;

        /* Return with status of first failure */
        seq_ctx_p->executeNextStep(returnStatus);
        return;
    }

    void
    IgmpsLocalConfigWorker::sendClearCommandToCluster (
            PrismLinearSequencerContext *seq_ctx_p)
    {
        IgmpsLocalClearMessage        *msg_p = NULL;
        IgmpsLocalClearMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext     *send_ctx_p = NULL;

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::sendClearCommandToCluster\n");
        msg_p = dynamic_cast<IgmpsLocalClearMessage *> (seq_ctx_p->getPPrismMessage ());
        if (! msg_p)
            return;

        loc_msg_p = new IgmpsLocalClearMessage(msg_p->getGrpName(),
                                             msg_p->getIfName(),
                                             msg_p->getCmdCode(),
                                             false);

        trace (TRACE_LEVEL_DEBUG, string("Sending Clear Command to Cluster") +
                        string(" CmdCode =") + msg_p->getCmdCode() + 
                        string (" grpName =")  + msg_p->getGrpName() +
                        string (" ifName =")  + msg_p->getIfName());

        send_ctx_p = new WaveSendToClusterContext(this, 
                reinterpret_cast<PrismAsynchronousCallback>
                (&IgmpsLocalConfigWorker::IgmpsLocalClearSendToClusterCallback),
                seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        sendToWaveCluster(send_ctx_p);

        return;
    }

    ResourceId 
    IgmpsLocalConfigWorker::sendClearToIgmpsDaemon (
            PrismSynchronousLinearSequencerContext *ctx_p)
    {
        IgmpsLocalClearMessage *msg_p = dynamic_cast<IgmpsLocalClearMessage *>
                                      (ctx_p->getPPrismMessage ());
        IgmpsClientClearMessage *cl_msg_p = NULL;
        ResourceId      status;

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::sendClearToIgmpsDaemon");

        if (! msg_p)
            return WAVE_MESSAGE_ERROR;

        trace(TRACE_LEVEL_DEBUG, "IgmpsLocalConfigWorker::sendClearToIgmpsDaemon::GrpName="
            + msg_p->getGrpName() + " IfName=" + msg_p->getIfName() + " CmdCode=" + msg_p->getCmdCode());

        cl_msg_p = new IgmpsClientClearMessage(msg_p->getGrpName(), msg_p->getIfName(), msg_p->getCmdCode());

        status = sendSynchronouslyToWaveClient("IGMP", cl_msg_p);

        ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (msg_p, cl_msg_p, status);
        delete cl_msg_p;
        return retStatus;
    }

    void
    IgmpsLocalConfigWorker::IgmpsLocalClearMessageHandler (
            IgmpsLocalClearMessage *pIgmpsLocalClearMessage)
    {

        trace(TRACE_LEVEL_DEBUG, "Entering IgmpsLocalConfigWorker::IgmpsLocalClearMessageHandler\n");
        if (pIgmpsLocalClearMessage->getIsGwRequest()) {
            PrismLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IgmpsLocalConfigWorker::sendClearCommandToCluster),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IgmpsLocalConfigWorker::prismLinearSequencerSucceededStep),

                    reinterpret_cast<PrismLinearSequencerStep>
                    (&IgmpsLocalConfigWorker::prismLinearSequencerFailedStep)
                };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = 
                new PrismLinearSequencerContext(pIgmpsLocalClearMessage, 
                    this, sequencerSteps, 
                    sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

            pPrismLinearSequencerContext->start ();
            return;
        } else { 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
                {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
                        (&IgmpsLocalConfigWorker::sendClearToIgmpsDaemon),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerSucceededStep),

                    reinterpret_cast<PrismSynchronousLinearSequencerStep>
           (&IgmpsLocalConfigWorker::prismSynchronousLinearSequencerFailedStep)   
                };

            PrismSynchronousLinearSequencerContext 
                *pPrismSynchronousLinearSequencerContext= 
                new PrismSynchronousLinearSequencerContext(
                        pIgmpsLocalClearMessage, this, sequencerSteps, 
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }
}
