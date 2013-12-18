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
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Eld/EldLocalWorker.h"
#include "Nsm/Local/Eld/NsmSetRxlimMessage.h" 
#include "Nsm/Local/NsmTypes.h"
//#include "Nsm/Local/Eld/EldLocalTypes.h"
#include "ClientInterface/Eld/EldClientUpdateRxlimConfigMessage.h" 
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Eld/EldShowMessage.h"
#include "ClientInterface/Eld/EldClearMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Arp/ArpShowMessage.h"
#include "Utils/DceClusterUtils.h"

namespace DcmNs
{

    EldLocalWorker::EldLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        addOperationMap (ELDGLOBALSETRXLIM, reinterpret_cast<PrismMessageHandler> (&EldLocalWorker::NsmUpdateEldRxlimLocalConfigMessageHandler));
        addOperationMap (ELDLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&EldLocalWorker::EldLocalShowMessageHandler));
        addOperationMap (ELDLOCALCLEAR, reinterpret_cast<PrismMessageHandler> (&EldLocalWorker::EldLocalClearMessageHandler));
    }

    EldLocalWorker::~EldLocalWorker ()
    {
    }

    PrismMessage  *EldLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;
        PLUG_DBG("Entering");

        switch (operationCode)
        {

            case ELDGLOBALSETRXLIM :
                pPrismMessage = new NsmSetRxlimMessage ();
                break;
           case ELDLOCALSHOW:
               PLUG_DBG("Entering Show ");
               pPrismMessage = new EldLocalShowMessage ();
               break;
           case ELDLOCALCLEAR:
               PLUG_DBG("Entering Show ");
               pPrismMessage = new EldLocalClearMessage ();
               break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *EldLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId EldLocalWorker::updateEldRxlimLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmSetRxlimMessage *pNsmSetRxlimMessage = dynamic_cast<NsmSetRxlimMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

		if (NULL == pNsmSetRxlimMessage) {
			trace (TRACE_LEVEL_ERROR, "EldLocalWorker::updateEldRxlimLocalConfigStep: pNsmSetRxlimMessage is NULL.");
			return WAVE_MESSAGE_SUCCESS;
		}

        int status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_DEBUG, "EldLocalWorker::updateEldRxlimLocalConfigStep: Entered....");
        EldClientUpdateRxlimConfigMessage *m = new EldClientUpdateRxlimConfigMessage (); 

		m->setOpCode(pNsmSetRxlimMessage->getOpCode());
		if (pNsmSetRxlimMessage->getOpCode() == ELD_GLOBAL_CONF_VCS_RX_LIMIT) {
			m->setRxlim(pNsmSetRxlimMessage->getRxlim());
		}
		if (pNsmSetRxlimMessage->getOpCode() == ELD_GLOBAL_CONF_HELLO_TIME) {
			m->setHelloInt(pNsmSetRxlimMessage->getHelloInt());
		}
		if (pNsmSetRxlimMessage->getOpCode() == ELD_GLOBAL_CONF_SHUTDOWN_TIME) {
			m->setShutdnTime(pNsmSetRxlimMessage->getShutdnTime());
		}

        status = sendSynchronouslyToWaveClient ("eld", m);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : Sagar : We need to handle this case.  Otherwise, please remove the status variable compeletely.
        }

        delete m;
		return WAVE_MESSAGE_SUCCESS;
    }

    void  EldLocalWorker::NsmUpdateEldRxlimLocalConfigMessageHandler(NsmSetRxlimMessage *pNsmSetRxlimMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&EldLocalWorker::updateEldRxlimLocalConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&EldLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&EldLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&EldLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&EldLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmSetRxlimMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

 void  EldLocalWorker::EldLocalShowMessageHandler( EldLocalShowMessage *pEldLocalShowMessage)
    {
        if (pEldLocalShowMessage->getisDistributionRequired())
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&EldLocalWorker::sendEldShowCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&EldLocalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&EldLocalWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pEldLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EldLocalWorker::sendEldShowCommandToEldDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EldLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EldLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pEldLocalShowMessage , this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }
 void EldLocalWorker::sendEldShowCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
        PLUG_DBG("Entering ...");

        EldLocalShowMessage *msg_p = dynamic_cast<EldLocalShowMessage*> (seq_ctx_p->getPPrismMessage ());
        PLUG_ASSERT_PTR(msg_p);

        EldLocalShowMessage *loc_msg_p = new EldLocalShowMessage(msg_p);
        PLUG_ASSERT_PTR(loc_msg_p);
        loc_msg_p->setisDistributionRequired(false);

        WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&EldLocalWorker::EldShowCmdClusterCallback), seq_ctx_p);
        PLUG_ASSERT_PTR(send_ctx_p);

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

            PLUG_DBG_S(string("No of locationids")+ msg_p->m_locs.size());
        if(msg_p->m_locs.size() > 0) {
            PLUG_DBG_S(string("setting locationids"));
            send_ctx_p->setLocationsToSendToForPhase1(msg_p->m_locs);
        }

        sendToWaveCluster(send_ctx_p);
        return;
    }

    ResourceId EldLocalWorker::sendEldShowCommandToEldDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
        PLUG_DBG("Entering ...");
        EldLocalShowMessage *msg_p = dynamic_cast<EldLocalShowMessage*> (seq_ctx_p->getPPrismMessage ());
        EldShowMessage  *m = new EldShowMessage(msg_p);
        if(!m)
        {
            NSM_PLUG_DBG_ERR_S(string(" Memory allocation failed"));
            return(WAVE_MESSAGE_ERROR);
        }
        ResourceId status = sendSynchronouslyToWaveClient ("eld",m);

        if(WAVE_MESSAGE_SUCCESS != status)
        {
            PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }
        msg_p->copyAllBuffers(*m);
        if(m){
            delete m;
        }
        return status;
    }

void EldLocalWorker::EldShowCmdClusterCallback(WaveSendToClusterContext *ctx_p)
    {
        PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        EldLocalShowMessage *req_msg_p = NULL;
        EldLocalShowMessage *resp_msg_p = NULL;
        EldLocalShowMessage *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<EldLocalShowMessage*> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<EldLocalShowMessage*> (ctx_p->getPPrismMessageForPhase1());

        PLUG_ASSERT_PTR(req_msg_p);
        PLUG_ASSERT_PTR(resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());


        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<EldLocalShowMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                req_msg_p->copyAllBuffers(*sw_resp_p);
                PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
            }
            else
            {
                PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
                PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

   }
 void  EldLocalWorker::EldLocalClearMessageHandler( EldLocalClearMessage *pEldLocalClearMessage)
    {
        if (pEldLocalClearMessage->getisDistributionRequired())
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&EldLocalWorker::sendEldClearCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&EldLocalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&EldLocalWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pEldLocalClearMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EldLocalWorker::sendEldClearCommandToEldDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EldLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&EldLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pEldLocalClearMessage , this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }
 void EldLocalWorker::sendEldClearCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
        PLUG_DBG("Entering ...");

        EldLocalClearMessage *msg_p = dynamic_cast<EldLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());
        PLUG_ASSERT_PTR(msg_p);

        EldLocalClearMessage *loc_msg_p = new EldLocalClearMessage(msg_p);
        PLUG_ASSERT_PTR(loc_msg_p);
        loc_msg_p->setisDistributionRequired(false);

        WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&EldLocalWorker::EldClearCmdClusterCallback), seq_ctx_p);
        PLUG_ASSERT_PTR(send_ctx_p);

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
            PLUG_DBG_S(string("No of locationids")+ msg_p->m_locs.size());
        if(msg_p->m_locs.size() > 0) {
            PLUG_DBG_S(string("setting locationids"));
            send_ctx_p->setLocationsToSendToForPhase1(msg_p->m_locs);
        }
        sendToWaveCluster(send_ctx_p);
        return;
    }

    ResourceId EldLocalWorker::sendEldClearCommandToEldDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
        PLUG_DBG("Entering ...");
        EldLocalClearMessage *msg_p = dynamic_cast<EldLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());
        EldClearMessage  *m = new EldClearMessage(msg_p);
        ResourceId status = sendSynchronouslyToWaveClient ("eld",m);

        if(WAVE_MESSAGE_SUCCESS != status)
        {
            PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }
        if(m){
            delete m;
        }
        return status;
    }

void EldLocalWorker::EldClearCmdClusterCallback(WaveSendToClusterContext *ctx_p)
    {
        PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        EldLocalClearMessage *req_msg_p = NULL;
        EldLocalClearMessage *resp_msg_p = NULL;
        EldLocalClearMessage *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<EldLocalClearMessage*> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<EldLocalClearMessage*> (ctx_p->getPPrismMessageForPhase1());

        PLUG_ASSERT_PTR(req_msg_p);
        PLUG_ASSERT_PTR(resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());


        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<EldLocalClearMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
            }
            else
            {
                PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
                PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

   }



}
