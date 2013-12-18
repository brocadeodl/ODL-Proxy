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
#include "SystemMonitor/Global/GlobalFaultyPortsWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeFaultyPortThresholdMessage.h"
#include "SystemMonitor/Global/FaultyPortsThresholdManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalFaultyPortsWorker::GlobalFaultyPortsWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        FaultyPortsThresholdManagedObject    FaultyPortsThresholdManagedObject    (pSystemMonitorGlobalObjectManager);
        FaultyPortsThresholdManagedObject.setupOrm ();
        addManagedClass (FaultyPortsThresholdManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGEFAULTYPORTTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&GlobalFaultyPortsWorker::SystemMonitorGlobalChangeFaultyPortThresholdMessageHandler));
    }

    GlobalFaultyPortsWorker::~GlobalFaultyPortsWorker ()
    {
    }

    PrismMessage  *GlobalFaultyPortsWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGEFAULTYPORTTHRESHOLD :
                pPrismMessage = new SystemMonitorGlobalChangeFaultyPortThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalFaultyPortsWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FaultyPortsThresholdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FaultyPortsThresholdManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalFaultyPortsWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalFaultyPortsWorker::SystemMonitorGlobalChangeFaultyPortThresholdMessageHandler( SystemMonitorGlobalChangeFaultyPortThresholdMessage *pSystemMonitorGlobalChangeFaultyPortThresholdMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFaultyPortsWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFaultyPortsWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFaultyPortsWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFaultyPortsWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeFaultyPortThresholdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
