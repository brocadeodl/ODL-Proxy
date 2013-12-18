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
#include <sys/time.h>

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/ShowMonitorWorker.h"
#include "SystemMonitor/Local/MonitorLocalShowSystemMonitorMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include "PortCfg/Local/PortCfgPortCfgPortTranslateMessage.h"

/* error codes */
#include "DcmResourceIdEnums.h"

/* offsets for time translation functions */
#define YEAR_OFFSET 1900
#define MONTH_OFFSET 1

#include "thresh/fwdPub.h"

#define MIN_IPADDR_LEN 1
namespace DcmNs
{

    ShowMonitorWorker::ShowMonitorWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        addOperationMap (MONITORLOCALSHOWSYSTEMMONITOR, reinterpret_cast<PrismMessageHandler> (&ShowMonitorWorker::MonitorLocalShowSystemMonitorMessageHandler));
    }

    ShowMonitorWorker::~ShowMonitorWorker ()
    {
    }

    PrismMessage  *ShowMonitorWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case MONITORLOCALSHOWSYSTEMMONITOR :
                pPrismMessage = new MonitorLocalShowSystemMonitorMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ShowMonitorWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  ShowMonitorWorker::MonitorLocalShowSystemMonitorMessageHandler( MonitorLocalShowSystemMonitorMessage *pMonitorLocalShowSystemMonitorMessage)
    {
        if (NULL == pMonitorLocalShowSystemMonitorMessage)
        {
            trace (TRACE_LEVEL_INFO, "MonitorLocalShowSystemMonitorMessageHandler: empty message");
            prismAssert (false, __FILE__, __LINE__);
        }
		/*
        SystemMonitorLocalObjectManager *lOM = dynamic_cast<SystemMonitorLocalObjectManager *>(getPWaveObjectManager());
        if (lOM->getFlag() == false)
        {
            pMonitorLocalShowSystemMonitorMessage->setCompletionStatus
                (WRC_SM_FW_NOTREADY_ERROR);
            reply (pMonitorLocalShowSystemMonitorMessage);
            return;           
        }
		*/
        int status, size = sizeof(fwSwitchHealthReport_t) +
            sizeof(portInfo_t)*MAX_PORT;
        fwSwitchHealthReport_t *pReport = NULL;
        fwSwStatus_t *pSwStat = NULL;
        ResourceId ret_status;

        status = thNullPtr;
        pReport = (fwSwitchHealthReport_t *)calloc(1, size);
        pReport->portHealth = callocType(portInfo_t, MAX_PORT);

        pSwStat = (fwSwStatus_t *)calloc(1, sizeof (fwSwStatus_t));
        if (pReport && pSwStat)
        {
            status = fwSwitchHealthReportGet(pReport, size);
        }

        switch (status)
        {
            case thUninitializedFWD:
                 ret_status = SYSTEMMONITOR_FW_UNINIT_ERROR;
                 break;
            case thNullPtr:
                 ret_status = SYSTEMMONITOR_NO_MEMORY;
                 break;
            case thInvalidSize:
                 ret_status = SYSTEMMONITOR_INVALID_SIZE;
                 break;
            case 0:
                 ret_status = WAVE_MESSAGE_SUCCESS;
                 break;
            case -1:
            default:
                 ret_status = SYSTEMMONITOR_BACKEND_ERROR;
                 break;
        }

        if (WAVE_MESSAGE_SUCCESS == ret_status)
        { // loop through report and push data to message
            string switchName = pReport->switchName;
			string ip = string (pReport->ipAddress);
			int len = ip.length();
			if (len <= MIN_IPADDR_LEN) {
				ip = "0.0.0.0/0";
	        	trace (TRACE_LEVEL_INFO, "ShowMonitorWorker::Truncating IP Address"+ip);
			} 
            IpVxAddress switchIp = IpVxAddress(ip);
            UI32 switchStatus = pReport->healthState;
            struct tm *p_localTm = localtime(&pReport->date);
            DateTime switchTime;
            int numComp = pReport->numHWObjects;
            // int numPort = pReport->numPorts;

            // first clear the data
            pMonitorLocalShowSystemMonitorMessage->clearComponents();
            pMonitorLocalShowSystemMonitorMessage->clearPorts();

            pMonitorLocalShowSystemMonitorMessage->setSwitchName(switchName);
            pMonitorLocalShowSystemMonitorMessage->setSwitchIpAddr(switchIp);
            pMonitorLocalShowSystemMonitorMessage->setSwitchStatus(switchStatus);

            // get the switch status reason
            status = switchStatusGet(pSwStat);
            if (0 != status)
            {
                pMonitorLocalShowSystemMonitorMessage->setSwitchStatusReason("error getting reason");
            }
            else
            {
                pMonitorLocalShowSystemMonitorMessage->setSwitchStatusReason(string(pSwStat->statusLastLog));
            }
            switchTime.setAllValues(
                WaveNs::SI16(p_localTm->tm_year) + YEAR_OFFSET,
                WaveNs::UI8(p_localTm->tm_mon) + MONTH_OFFSET,
                WaveNs::UI8(p_localTm->tm_mday),WaveNs::UI8(p_localTm->tm_hour),                WaveNs::UI8(p_localTm->tm_min), WaveNs::UI8(p_localTm->tm_sec),
                WaveNs::UI32(0), WaveNs::SI8(0), WaveNs::SI8(0));
            pMonitorLocalShowSystemMonitorMessage->setDate(switchTime);

            /* fill in component status */
            for (int i = 0; i < numComp; i++)
            {
                string compName;
                UI32 compState, compBad, compAbsent;
                hwObj_t *p_hwObj = &pReport->hwObj[i];

                if ((SWSTATUS_MARGINAL_PORTS == p_hwObj->hwType) ||
                    (SWSTATUS_FAULTY_PORTS == p_hwObj->hwType) ||
                    (SWSTATUS_MISSING_GBICS == p_hwObj->hwType) ||
                    (SWSTATUS_ERROR_PORTS == p_hwObj->hwType))
                {   // these are currently not supported
                    continue;
                }
                compName = fwGetHWObjectName(p_hwObj->hwType);
                compState = p_hwObj->state;
                compBad = p_hwObj->numBad;
                compAbsent = p_hwObj->numAbsent;

                pMonitorLocalShowSystemMonitorMessage->setComponent(compName,
                    compState, compBad, compAbsent);
            }
#if 0
            /* fill in port status */
            for (int i = 0; i < numPort; i++)
            {
                string portName;
                UI32 portState, portArea;
                portInfo_t *p_pObj = &pReport->portHealth[i];

                portArea = p_pObj->area;
                PortCfgPortCfgPortTranslateMessage *pPortTranslate = new PortCfgPortCfgPortTranslateMessage;
                pPortTranslate->setSwitchPort(portArea);
                pPortTranslate->setRbridge(DcmToolKit::getLocalMappedId());
                if (WAVE_MESSAGE_SUCCESS == sendSynchronously (pPortTranslate))
                {
                    portName = pPortTranslate->getId();
                }
                else
                {
                    char convertToStr[20];
                    sprintf(convertToStr, "%d", portArea);
                    portName = convertToStr;
                }
                delete pPortTranslate;
                portState = p_pObj->healthState;

                pMonitorLocalShowSystemMonitorMessage->setPort(portArea,
                    portName, portState);
            }
#endif
        }
        if (pReport->portHealth)
            free (pReport->portHealth);
        if (pReport)
           free(pReport);
        if (pSwStat)
           free(pSwStat);

        trace (TRACE_LEVEL_INFO, "ShowMonitorWorker::MonitorLocalShowSystemMonitorMessageHandler Exiting.");
        pMonitorLocalShowSystemMonitorMessage->setCompletionStatus(ret_status);
        reply(pMonitorLocalShowSystemMonitorMessage);
    }

}
