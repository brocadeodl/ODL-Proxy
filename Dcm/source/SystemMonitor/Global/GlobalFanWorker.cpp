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

#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/GlobalFanWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeFanThresholdMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeFanAlertMessage.h"
#include "SystemMonitor/Global/FanThresholdManagedObject.h"
#include "SystemMonitor/Global/FanAlertManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalFanWorker::GlobalFanWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        FanThresholdManagedObject    FanThresholdManagedObject    (pSystemMonitorGlobalObjectManager);
        FanAlertManagedObject    FanAlertManagedObject    (pSystemMonitorGlobalObjectManager);
        FanThresholdManagedObject.setupOrm ();
        addManagedClass (FanThresholdManagedObject::getClassName (), this);
        FanAlertManagedObject.setupOrm ();
        addManagedClass (FanAlertManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGEFANTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&GlobalFanWorker::SystemMonitorGlobalChangeFanThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALCHANGEFANALERT, reinterpret_cast<PrismMessageHandler> (&GlobalFanWorker::SystemMonitorGlobalChangeFanAlertMessageHandler));
    }

    GlobalFanWorker::~GlobalFanWorker ()
    {
    }

    PrismMessage  *GlobalFanWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGEFANTHRESHOLD :
                pPrismMessage = new SystemMonitorGlobalChangeFanThresholdMessage ();
                break;
            case SYSTEMMONITORGLOBALCHANGEFANALERT :
                pPrismMessage = new SystemMonitorGlobalChangeFanAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalFanWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FanThresholdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FanThresholdManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FanAlertManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FanAlertManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalFanWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }
#if 0
    void GlobalFanWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        /*
		ResourceId                      status      = WAVE_MESSAGE_SUCCESS;
        FanThresholdManagedObject       *pFanThreshMO = NULL;
        startTransaction ();
        pFanThreshMO = new FanThresholdManagedObject
            (dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()),
             3, 3);
        status = commitTransaction();
        delete pFanThreshMO;
        */
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
#endif
    void  GlobalFanWorker::SystemMonitorGlobalChangeFanAlertMessageHandler( SystemMonitorGlobalChangeFanAlertMessage *pSystemMonitorGlobalChangeFanAlertMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeFanAlertMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  GlobalFanWorker::SystemMonitorGlobalChangeFanThresholdMessageHandler( SystemMonitorGlobalChangeFanThresholdMessage *pSystemMonitorGlobalChangeFanThresholdMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalFanWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeFanThresholdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
