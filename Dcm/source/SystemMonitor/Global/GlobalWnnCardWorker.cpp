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
#include "SystemMonitor/Global/GlobalWnnCardWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeWwnCardThresholdMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeWwnCardAlertMessage.h"
#include "SystemMonitor/Global/WwnCardThresholdManagedObject.h"
#include "SystemMonitor/Global/WwnCardAlertManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalWnnCardWorker::GlobalWnnCardWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        WwnCardThresholdManagedObject    WwnCardThresholdManagedObject    (pSystemMonitorGlobalObjectManager);
        WwnCardAlertManagedObject    WwnCardAlertManagedObject    (pSystemMonitorGlobalObjectManager);
        WwnCardThresholdManagedObject.setupOrm ();
        addManagedClass (WwnCardThresholdManagedObject::getClassName (), this);
        WwnCardAlertManagedObject.setupOrm ();
        addManagedClass (WwnCardAlertManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGEWWNCARDTHRESHOLD, reinterpret_cast<PrismMessageHandler> (&GlobalWnnCardWorker::SystemMonitorGlobalChangeWwnCardThresholdMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALCHANGEWWNCARDALERT, reinterpret_cast<PrismMessageHandler> (&GlobalWnnCardWorker::SystemMonitorGlobalChangeWwnCardAlertMessageHandler));
    }

    GlobalWnnCardWorker::~GlobalWnnCardWorker ()
    {
    }

    PrismMessage  *GlobalWnnCardWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGEWWNCARDTHRESHOLD :
                pPrismMessage = new SystemMonitorGlobalChangeWwnCardThresholdMessage ();
                break;
            case SYSTEMMONITORGLOBALCHANGEWWNCARDALERT :
                pPrismMessage = new SystemMonitorGlobalChangeWwnCardAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalWnnCardWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((WwnCardThresholdManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new WwnCardThresholdManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((WwnCardAlertManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new WwnCardAlertManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalWnnCardWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalWnnCardWorker::SystemMonitorGlobalChangeWwnCardAlertMessageHandler( SystemMonitorGlobalChangeWwnCardAlertMessage *pSystemMonitorGlobalChangeWwnCardAlertMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeWwnCardAlertMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  GlobalWnnCardWorker::SystemMonitorGlobalChangeWwnCardThresholdMessageHandler( SystemMonitorGlobalChangeWwnCardThresholdMessage *pSystemMonitorGlobalChangeWwnCardThresholdMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalWnnCardWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeWwnCardThresholdMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
