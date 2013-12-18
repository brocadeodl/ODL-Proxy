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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/GlobalPowerWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangePowerThresholdMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangePowerAlertMessage.h"
#include "SystemMonitor/Global/PowerThresholdManagedObject.h"
#include "SystemMonitor/Global/PowerAlertManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalPowerWorker::GlobalPowerWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        PowerThresholdManagedObject    PowerThresholdManagedObject    (pSystemMonitorGlobalObjectManager);
        PowerAlertManagedObject    PowerAlertManagedObject    (pSystemMonitorGlobalObjectManager);
        PowerThresholdManagedObject.setupOrm ();
        addManagedClass (PowerThresholdManagedObject::getClassName (), this);
        PowerAlertManagedObject.setupOrm ();
        addManagedClass (PowerAlertManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGEPOWERTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&GlobalPowerWorker::SystemMonitorGlobalChangePowerThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALCHANGEPOWERALERT, reinterpret_cast<PrismMessageHandler> (&GlobalPowerWorker::SystemMonitorGlobalChangePowerAlertMessageHandler));
    }

    GlobalPowerWorker::~GlobalPowerWorker ()
    {
    }

    PrismMessage  *GlobalPowerWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGEPOWERTHRESHOLD :
                pPrismMessage = new SystemMonitorGlobalChangePowerThresholdMessage ();
                break;
            case SYSTEMMONITORGLOBALCHANGEPOWERALERT :
                pPrismMessage = new SystemMonitorGlobalChangePowerAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalPowerWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PowerThresholdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PowerThresholdManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PowerAlertManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PowerAlertManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalPowerWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalPowerWorker::SystemMonitorGlobalChangePowerAlertMessageHandler( SystemMonitorGlobalChangePowerAlertMessage *pSystemMonitorGlobalChangePowerAlertMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangePowerAlertMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  GlobalPowerWorker::SystemMonitorGlobalChangePowerThresholdMessageHandler( SystemMonitorGlobalChangePowerThresholdMessage *pSystemMonitorGlobalChangePowerThresholdMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalPowerWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangePowerThresholdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
