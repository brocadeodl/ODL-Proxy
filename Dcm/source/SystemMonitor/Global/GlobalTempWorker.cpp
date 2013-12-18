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
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/GlobalTempWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeTempThresholdMessage.h"
#include "SystemMonitor/Global/TempThresholdManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalTempWorker::GlobalTempWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        TempThresholdManagedObject    TempThresholdManagedObject    (pSystemMonitorGlobalObjectManager);
        TempThresholdManagedObject.setupOrm ();
        addManagedClass (TempThresholdManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGETEMPTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&GlobalTempWorker::SystemMonitorGlobalChangeTempThresholdMessageHandler));
    }

    GlobalTempWorker::~GlobalTempWorker ()
    {
    }

    PrismMessage  *GlobalTempWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGETEMPTHRESHOLD :
                pPrismMessage = new SystemMonitorGlobalChangeTempThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalTempWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((TempThresholdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new TempThresholdManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalTempWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalTempWorker::SystemMonitorGlobalChangeTempThresholdMessageHandler( SystemMonitorGlobalChangeTempThresholdMessage *pSystemMonitorGlobalChangeTempThresholdMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalTempWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalTempWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalTempWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalTempWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeTempThresholdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
