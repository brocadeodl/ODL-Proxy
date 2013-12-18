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
#include "SystemMonitor/Global/GlobalCompactFlashWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeCompactFlashThresholdMessage.h"
#include "SystemMonitor/Global/CompactFlashThresholdManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalCompactFlashWorker::GlobalCompactFlashWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        CompactFlashThresholdManagedObject    CompactFlashThresholdManagedObject    (pSystemMonitorGlobalObjectManager);
        CompactFlashThresholdManagedObject.setupOrm ();
        addManagedClass (CompactFlashThresholdManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGECOMPACTFLASHTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&GlobalCompactFlashWorker::SystemMonitorGlobalChangeCompactFlashThresholdMessageHandler));
    }

    GlobalCompactFlashWorker::~GlobalCompactFlashWorker ()
    {
    }

    PrismMessage  *GlobalCompactFlashWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGECOMPACTFLASHTHRESHOLD :
                pPrismMessage = new SystemMonitorGlobalChangeCompactFlashThresholdMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalCompactFlashWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((CompactFlashThresholdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CompactFlashThresholdManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalCompactFlashWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalCompactFlashWorker::SystemMonitorGlobalChangeCompactFlashThresholdMessageHandler( SystemMonitorGlobalChangeCompactFlashThresholdMessage *pSystemMonitorGlobalChangeCompactFlashThresholdMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalCompactFlashWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalCompactFlashWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalCompactFlashWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalCompactFlashWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeCompactFlashThresholdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
