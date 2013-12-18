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
 *   Author : Yugant Chandrakar                                            *
 **************************************************************************/

#include "DcmResourceIdEnums.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Utils/StringUtils.h"
#include "WaveResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"

#include "ThresholdMonitor/Global/GlobalInterfaceShowWorker.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "ThresholdMonitor/Local/InterfaceLocalShowMessage.h"
#include "ThresholdMonitor/Global/InterfaceGlobalShowMessage.h"
//#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"

namespace DcmNs
{
    GlobalInterfaceShowWorker::GlobalInterfaceShowWorker (ThresholdMonitorGlobalObjectManager *pThresholdMonitorGlobalObjectManager)
        : WaveWorker (pThresholdMonitorGlobalObjectManager)
    {
        addOperationMap (INTERFACEGLOBALSHOWMESSAGE, reinterpret_cast<PrismMessageHandler> (&GlobalInterfaceShowWorker::InterfaceGlobalShowMessageHandler));
    }

    GlobalInterfaceShowWorker::~GlobalInterfaceShowWorker ()
    {
    }

    PrismMessage *GlobalInterfaceShowWorker::createMessageInstance (const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;
        switch (operationCode) {
            case INTERFACEGLOBALSHOWMESSAGE:
                pPrismMessage = new InterfaceGlobalShowMessage();
                break;
            default :
                pPrismMessage = NULL;
        }
        return (pPrismMessage);
    }

    void GlobalInterfaceShowWorker::InterfaceGlobalShowMessageHandler(InterfaceGlobalShowMessage *pInterfaceGlobalShowMessage)
    {
        trace (TRACE_LEVEL_INFO, "Entering InterfaceGlobalShowMessageHandler..");

        UI32 areaName = pInterfaceGlobalShowMessage->getAreaName();
        string ifName = pInterfaceGlobalShowMessage->getIfName();
        trace (TRACE_LEVEL_INFO, string("InterfaceGlobalShowMessageHandler areaName-")+areaName+" ifName-"+ifName); //+" NumOfLocations-"+location_Id.size());

            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&GlobalInterfaceShowWorker::sendShowInterfaceCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&GlobalInterfaceShowWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&GlobalInterfaceShowWorker::prismLinearSequencerFailedStep)
            };
            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext
            (pInterfaceGlobalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
        return;
    }

    void GlobalInterfaceShowWorker::sendShowInterfaceCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        trace (TRACE_LEVEL_INFO, string("Entering sendShowInterfaceCommandToCluster.."));

        InterfaceGlobalShowMessage *msg_p = NULL;
        InterfaceLocalShowMessage *loc_msg_p = NULL;
        WaveSendToClusterContext *send_ctx_p = NULL;
        vector<UI32> locations;


        msg_p = dynamic_cast<InterfaceGlobalShowMessage *>(seq_ctx_p->getPPrismMessage ());
        loc_msg_p = new InterfaceLocalShowMessage();
        loc_msg_p->setAreaName(msg_p->getAreaName());
        loc_msg_p->setIfName(msg_p->getIfName());
        if (msg_p->getIsGwRequest()) {
            FrameworkToolKit::getFullyConnectedLocations(locations);
            locations.push_back(DcmToolKit::getLocationIdFromMappedId(DcmToolKit::getLocalMappedId()));
        } else {
            locations.push_back(msg_p->getLocationId());
        }

        send_ctx_p = new WaveSendToClusterContext(this,reinterpret_cast <PrismAsynchronousCallback>(&GlobalInterfaceShowWorker::sendShowInterfaceCommandToClusterCallback), seq_ctx_p);
        if (!send_ctx_p)
            return;
        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        if(!locations.empty()) {
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }
        send_ctx_p->setPartialSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);
        trace(TRACE_LEVEL_INFO, string("GlobalInterfaceShowWorker::sendShowInterfaceCommandToCluster returned from sendToWaveCluster() "));

        return;
    }

    void GlobalInterfaceShowWorker::sendShowInterfaceCommandToClusterCallback (WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext     *seq_ctx_p  = NULL;
        InterfaceGlobalShowMessage            *req_msg_p  = NULL;
        InterfaceLocalShowMessage             *resp_msg_p = NULL,
                                        *sw_resp_p  = NULL;
        UI32 i = 0;

        seq_ctx_p   = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p   = dynamic_cast<InterfaceGlobalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p  = dynamic_cast<InterfaceLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        trace(TRACE_LEVEL_INFO, string("Response processing: Num resp:") + locations.size());
        for (i = 0; i < locations.size(); i++) {
            if (i ==0)
                trace(TRACE_LEVEL_INFO, string("locations:") + locations[i]);
            else
                trace(TRACE_LEVEL_INFO, string(":") + locations[i]);
        }

        for (i = 0; i < locations.size(); i++) {
            sw_resp_p = dynamic_cast<InterfaceLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
            prismAssert (sw_resp_p != NULL, __FILE__, __LINE__);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                trace(TRACE_LEVEL_INFO,string("GlobalInterfaceShowWorker::sendShowInterfaceCommandToClusterCallback WAVE_MESSAGE_SUCCESS "));
                prismAssert (req_msg_p != NULL, __FILE__, __LINE__);
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                ResourceId clusterCompletionStatus = sw_resp_p->getCompletionStatus();
                trace(TRACE_LEVEL_INFO, string("Response processing: The location: ")
                    + locations[i] + string("returned error: ")
                    + FrameworkToolKit::localize(clusterCompletionStatus));
                //req_msg_p->setCompletionStatus(clusterCompletionStatus);
                break;
            }
        }

        req_msg_p->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
        delete resp_msg_p;
        delete ctx_p;
        return;

    }

}

