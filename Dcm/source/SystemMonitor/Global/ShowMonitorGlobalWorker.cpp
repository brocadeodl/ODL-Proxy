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

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmResourceIdEnums.h"

#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/ShowMonitorGlobalWorker.h"
#include "SystemMonitor/Global/MonitorGlobalShowSystemMonitorMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/MonitorLocalShowSystemMonitorMessage.h"

namespace DcmNs
{

    ShowMonitorGlobalWorker::ShowMonitorGlobalWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        addOperationMap (MONITORGLOBALSHOWSYSTEMMONITOR, reinterpret_cast<PrismMessageHandler> (&ShowMonitorGlobalWorker::MonitorGlobalShowSystemMonitorMessageHandler));
    }

    ShowMonitorGlobalWorker::~ShowMonitorGlobalWorker ()
    {
    }

    PrismMessage  *ShowMonitorGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case MONITORGLOBALSHOWSYSTEMMONITOR :
                pPrismMessage = new MonitorGlobalShowSystemMonitorMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ShowMonitorGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  ShowMonitorGlobalWorker::MonitorGlobalShowSystemMonitorMessageHandler( MonitorGlobalShowSystemMonitorMessage *pMonitorGlobalShowSystemMonitorMessage)
    {
        bool destAllFlag = false;
        UI32 mappedId;

        trace (TRACE_LEVEL_INFO, string("ShowMonitorGlobalWorker::MonitorGlobalShowSystemMonitorMessageHandler Entering"));

        // Validations and Queries should go here
        if (pMonitorGlobalShowSystemMonitorMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        // clear the response data
        pMonitorGlobalShowSystemMonitorMessage->clearSwitchStatus();
        pMonitorGlobalShowSystemMonitorMessage->clearComponents();
        pMonitorGlobalShowSystemMonitorMessage->clearPorts();

        // send get message to single location or to cluster
        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&ShowMonitorGlobalWorker::getSTCCallback), pMonitorGlobalShowSystemMonitorMessage);

        destAllFlag = pMonitorGlobalShowSystemMonitorMessage->getDestinationAllFlag();
        mappedId = pMonitorGlobalShowSystemMonitorMessage->getDestinationMapId();

        if (false == destAllFlag)
        { // mappedId is specified, sending only to one switch
            vector<LocationId> locationsVector;

            if (LOCAL_SWITCH_ID == mappedId)
            {
                locationsVector.push_back(FrameworkToolKit::getThisLocationId());
            }
            else
            {
                locationsVector.push_back(DcmToolKit::getLocationIdFromMappedId(mappedId));
                if (0 == locationsVector[0]) // invalid mappedId
                {
                    char swidCstr[16];
                    sprintf(swidCstr, "%d", mappedId);
                    trace (TRACE_LEVEL_INFO, string("MonitorGlobalShowSystemMonitorMessage invalid switchid") + string(swidCstr));
                    pMonitorGlobalShowSystemMonitorMessage->setCompletionStatus(SYSTEMMONITOR_INVALID_MAPPED_ID);
                    reply (pMonitorGlobalShowSystemMonitorMessage);
					delete pWaveSendToClusterContext;
                    return;
                }
            }
            pWaveSendToClusterContext->setLocationsToSendToForPhase1(locationsVector);
        }

        MonitorLocalShowSystemMonitorMessage *pMessage = new MonitorLocalShowSystemMonitorMessage ();
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void ShowMonitorGlobalWorker::getSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        ResourceId status;

        trace(TRACE_LEVEL_INFO, string("ShowMonitorGlobalWorker::getSTCCallback"));

        MonitorGlobalShowSystemMonitorMessage *pOrigMsg = (MonitorGlobalShowSystemMonitorMessage *)pWaveSendToClusterContext->getPCallerContext();

        status = pWaveSendToClusterContext->getCompletionStatus();
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, string("ShowMonitorGlobalWorker::getSTCCallback : SUCCESS"));
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("ShowMonitorGlobalWorker::getSTCCallback: failed on sendToCluster with Framework error - ") + FrameworkToolKit::localize (status));
        }

        ResourceId lStatus;
        UI32 mappedId;
        // get location ids for return status or data
        vector<LocationId> locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
        for (unsigned int i = 0; i < locationsVector.size(); i++)
        { // loop through each location and get msg rsp pointer for location
            MonitorLocalShowSystemMonitorMessage *pLocMsg = (MonitorLocalShowSystemMonitorMessage *)pWaveSendToClusterContext->getResultingMessageForPhase1(locationsVector[i]);
            mappedId = DcmToolKit::getMappedIdFromLocationId(locationsVector[i]);

            lStatus = pLocMsg->getCompletionStatus();
            if (WAVE_MESSAGE_SUCCESS == lStatus)
            {
                string swName = pLocMsg->getSwitchName();
                IpVxAddress swIpAddr = pLocMsg->getSwitchIpAddr();
                UI32 swStatus = pLocMsg->getSwitchStatus();
                string swStatusReason = pLocMsg->getSwitchStatusReason();
                DateTime dtime = pLocMsg->getDate();
                UI32 numComp, numPort;
                numComp = pLocMsg->getNumComponent();
                numPort = pLocMsg->getNumPorts();

                pOrigMsg->setSwitchStatus(mappedId, swName, swIpAddr,
                    swStatus, swStatusReason, dtime, numComp, numPort);

                for (UI32 j = 0; j < numComp; j++)
                {
                    string component;
                    UI32 state, numbad, numabsent;
                    pLocMsg->getComponent(j, component, state, numbad, numabsent);
                    pOrigMsg->setComponent(mappedId, component, state, numbad, numabsent);
                }

                for (UI32 j = 0; j < numPort; j++)
                {
                    UI32 area, state;
                    string name;
                    pLocMsg->getPort(j, area, name, state);
                    pOrigMsg->setPort(mappedId, area, name, state);
                }
            }
            else
            {
                if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == status)
                { // if failed on all nodes, lStatus might be relevent
                    status = lStatus;
                }
            }

            // clear pLocMsg contents
            pLocMsg->clearComponents();
            pLocMsg->clearPorts();
            // don't need to free pLocMsg
        }

        MonitorLocalShowSystemMonitorMessage *pGetMsg = (MonitorLocalShowSystemMonitorMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete(pGetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("ShowMonitorGlobalWorker::getSTCCallback Exiting"));
        pOrigMsg->setCompletionStatus(status);
        reply (pOrigMsg);
    }

}
