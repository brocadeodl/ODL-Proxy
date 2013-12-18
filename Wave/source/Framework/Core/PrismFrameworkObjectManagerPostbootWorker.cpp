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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkObjectManagerPostbootWorker.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Postboot/PrismPostbootAgent.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

PrismFrameworkObjectManagerPostbootWorker::PrismFrameworkObjectManagerPostbootWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    addOperationMap (FRAMEWORK_OBJECT_MANAGER_EXECUTE_POSTBOOT_PASSES, reinterpret_cast<PrismMessageHandler> (&PrismFrameworkObjectManagerPostbootWorker::postbootMessageHandler));
}

PrismFrameworkObjectManagerPostbootWorker::~PrismFrameworkObjectManagerPostbootWorker ()
{
}

void PrismFrameworkObjectManagerPostbootWorker::postbootMessageHandler (PrismPostbootObjectManagerMessage *pPrismPostbootObjectManagerMessage)
{
    ResourceId  status;

    PrismPostbootAgent  *pPrismPostbootAgent = new PrismPostbootAgent(m_pWaveObjectManager);

    updateHardwareSynchronizationState (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_SYNCHRONIZING, FrameworkToolKit::getThisLocationId ());

    status = pPrismPostbootAgent->execute();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        updateHardwareSynchronizationState (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_IN_SYNC, FrameworkToolKit::getThisLocationId ());
    }
    else
    {
        updateHardwareSynchronizationState (WAVE_NODE_HARDWARE_SYNCHRONIZATION_STATE_OUT_OF_SYNC, FrameworkToolKit::getThisLocationId ());
    }

    pPrismPostbootObjectManagerMessage->setCompletionStatus (status);
    reply (pPrismPostbootObjectManagerMessage);

    delete(pPrismPostbootAgent);
}

}
