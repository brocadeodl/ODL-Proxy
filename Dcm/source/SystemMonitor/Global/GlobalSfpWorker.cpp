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
#include "SystemMonitor/Global/GlobalSfpWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalChangeSfpAlertMessage.h"
#include "SystemMonitor/Global/SfpAlertManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
namespace DcmNs
{

    GlobalSfpWorker::GlobalSfpWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        SfpAlertManagedObject    SfpAlertManagedObject    (pSystemMonitorGlobalObjectManager);
        SfpAlertManagedObject.setupOrm ();
        addManagedClass (SfpAlertManagedObject::getClassName (), this);
        addOperationMap (SYSTEMMONITORGLOBALCHANGESFPALERT, reinterpret_cast<PrismMessageHandler> (&GlobalSfpWorker::SystemMonitorGlobalChangeSfpAlertMessageHandler));
    }

    GlobalSfpWorker::~GlobalSfpWorker ()
    {
    }

    PrismMessage  *GlobalSfpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALCHANGESFPALERT :
                pPrismMessage = new SystemMonitorGlobalChangeSfpAlertMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalSfpWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((SfpAlertManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SfpAlertManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalSfpWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  GlobalSfpWorker::SystemMonitorGlobalChangeSfpAlertMessageHandler( SystemMonitorGlobalChangeSfpAlertMessage *pSystemMonitorGlobalChangeSfpAlertMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalSfpWorker::prismLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalSfpWorker::prismLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismLinearSequencerStep>(&GlobalSfpWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&GlobalSfpWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pSystemMonitorGlobalChangeSfpAlertMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

}
