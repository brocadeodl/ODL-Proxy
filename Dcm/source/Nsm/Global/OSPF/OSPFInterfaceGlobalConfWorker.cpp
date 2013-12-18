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
 *   Author : kverma                                                       *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/OSPF/OSPFInterfaceGlobalConfWorker.h"
#include "Nsm/Global/OSPF/OSPFInterfaceGlobalMessage.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalMessage.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalManagedObject.h"
#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    OSPFInterfaceGlobalConfWorker::OSPFInterfaceGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        addOperationMap (NSMGLOBALOSPFINTERFACECONFIG, reinterpret_cast<PrismMessageHandler> (&OSPFInterfaceGlobalConfWorker::NsmGlobalOSPFInterfaceConfigMessageHandler));
    }

    OSPFInterfaceGlobalConfWorker::~OSPFInterfaceGlobalConfWorker ()
    {
    }

    PrismMessage  *OSPFInterfaceGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMGLOBALOSPFINTERFACECONFIG :
                pPrismMessage = new OSPFInterfaceGlobalMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *OSPFInterfaceGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

#if 0
    void OSPFInterfaceGlobalConfWorker::ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid)
    {
        vector<LocationId>  locationsToSendToForPhase1 = ctx_p -> getLocationsToSendToForPhase1 ();

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        for(unsigned int i = 0; i < locationsToSendToForPhase1.size(); i++)
        {
            locationid = locationsToSendToForPhase1[i];
            status = ctx_p -> getCompletionStatusForPhase1(locationid);
            if (status != WAVE_MESSAGE_SUCCESS) {
                return;
            }

        }
    }

    void OSPFInterfaceGlobalConfWorker::OSPFInterfaceConfSendToClusterCallback (WaveSendToClusterContext  *ctx_p)
	{
		PrismSynchronousLinearSequencerContext             *seq_ctx_p;
		LocationId                              locationid;
		ClusterGetFirstFailureLocation(ctx_p, locationid);
		ResourceId sendToClusterCompletionStatus = ctx_p->getCompletionStatusForPhase1(locationid);

		trace (TRACE_LEVEL_ERROR, "OSPFInterfaceGlobalConfWorker::in OSPFInterfaceConfSendToClusterCallback");
		seq_ctx_p = reinterpret_cast<PrismSynchronousLinearSequencerContext *>(ctx_p->getPCallerContext());
		delete ctx_p  -> getPPrismMessageForPhase1();
		delete ctx_p;
		seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
		return;
	}
#endif

    ResourceId OSPFInterfaceGlobalConfWorker::ospfInterfaceConfigStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        int status = WAVE_MESSAGE_ERROR;
        OSPFInterfaceGlobalMessage *pOSPFInterfaceGlobalMessage = 
            dynamic_cast<OSPFInterfaceGlobalMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

		trace (TRACE_LEVEL_DEVEL, "OSPFInterfaceGlobalConfWorker::ospfInterfaceConfigStep message Global plugin entering");
        OSPFInterfaceLocalMessage *m = new OSPFInterfaceLocalMessage();

        m->setOpCode(pOSPFInterfaceGlobalMessage->getOpCode());
        m->setintfName(pOSPFInterfaceGlobalMessage->getintfName());
        m->setintfType(pOSPFInterfaceGlobalMessage->getintfType());

        m->setCmdCode(pOSPFInterfaceGlobalMessage->getCmdCode());
        m->setAreaId(pOSPFInterfaceGlobalMessage->getAreaId());
        m->setHelloInterval(pOSPFInterfaceGlobalMessage->getHelloInterval());
        m->setRouterDeadInterval(pOSPFInterfaceGlobalMessage->getRouterDeadInterval());
        m->setRetransmitInterval(pOSPFInterfaceGlobalMessage->getRetransmitInterval());
        m->setTransDelay(pOSPFInterfaceGlobalMessage->getTransDelay());
        m->setCost(pOSPFInterfaceGlobalMessage->getCost());
        m->setPriority(pOSPFInterfaceGlobalMessage->getPriority());
        m->setAuthChangeWaitTime(pOSPFInterfaceGlobalMessage->getAuthChangeWaitTime());
        m->setEncryptionType(pOSPFInterfaceGlobalMessage->getEncryptionType());
        m->setMD5EncryptionType(pOSPFInterfaceGlobalMessage->getMD5EncryptionType());
        m->setAuthType(pOSPFInterfaceGlobalMessage->getAuthType());
        m->setAuthKey(pOSPFInterfaceGlobalMessage->getAuthKey());
        m->setMD5KeyActWaitTime(pOSPFInterfaceGlobalMessage->getMD5KeyActWaitTime());
        m->setMD5KeyId(pOSPFInterfaceGlobalMessage->getMD5KeyId());
        m->setMD5Key(pOSPFInterfaceGlobalMessage->getMD5Key());
        m->setFilterAll(pOSPFInterfaceGlobalMessage->getFilterAll());
        m->setFilterOption(pOSPFInterfaceGlobalMessage->getFilterOption());
        m->setmtuIgnore(pOSPFInterfaceGlobalMessage->getmtuIgnore());
        m->setNetworkType(pOSPFInterfaceGlobalMessage->getNetworkType());
        m->setPassive(pOSPFInterfaceGlobalMessage->getPassive());
        trace (TRACE_LEVEL_INFO, "OSPFInterfaceGlobalConfWorker::ospfInterfaceConfigStep: Fill OSPFInterfaceLocalMessage ");

        status = sendSynchronously(m);

        pOSPFInterfaceGlobalMessage->setCompletionStatus(m->getCompletionStatus());

        if(status == WAVE_MESSAGE_SUCCESS) {
            if(pOSPFInterfaceGlobalMessage->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                status = pOSPFInterfaceGlobalMessage->getCompletionStatus();
            else
                trace(TRACE_LEVEL_INFO, string("OSPFInterfaceGlobalConfWorker::ospfInterfaceConfigStep: ")
                        + string("Successfully sent message to local worker."));
        }
        else {
            trace(TRACE_LEVEL_INFO, string("OSPFInterfaceGlobalConfWorker::ospfInterfaceConfigStep: ")
                    + string("Failed to send message to local worker. Status : ")
                    + FrameworkToolKit::localize (status));
        }

        delete m;

        return status;
	}

    void  OSPFInterfaceGlobalConfWorker::NsmGlobalOSPFInterfaceConfigMessageHandler( OSPFInterfaceGlobalMessage *pOSPFInterfaceGlobalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceGlobalConfWorker::ospfInterfaceConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceGlobalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceGlobalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceGlobalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&OSPFInterfaceGlobalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pOSPFInterfaceGlobalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

     	pPrismSynchronousLinearSequencerContext->execute();
    }
}
