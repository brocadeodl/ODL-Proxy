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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/WaveSystemManagementGetYangUserInterfaceWorker.h"
#include "SystemManagement/WaveSystemManagementGetYangUserInterfaceMessage.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "SystemManagement/WaveSystemManagementTypes.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

namespace WaveNs
{

WaveSystemManagementGetYangUserInterfaceWorker::WaveSystemManagementGetYangUserInterfaceWorker (WaveSystemManagementObjectManager *pWaveSystemManagementObjectManager)
    : WaveWorker                           (pWaveSystemManagementObjectManager),
      m_pWaveSystemManagementObjectManager (pWaveSystemManagementObjectManager)
{
    addOperationMap (WAVE_SYSTEM_MANAGEMENT_GET_YANG_USER_INTERFACE, reinterpret_cast<PrismMessageHandler> (&WaveSystemManagementGetYangUserInterfaceWorker::getYangUserInterfaceMessageHandler));
}

WaveSystemManagementGetYangUserInterfaceWorker::~WaveSystemManagementGetYangUserInterfaceWorker ()
{
}

void WaveSystemManagementGetYangUserInterfaceWorker::getYangUserInterfaceMessageHandler (WaveSystemManagementGetYangUserInterfaceMessage *pWaveSystemManagementGetYangUserInterfaceMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementGetYangUserInterfaceWorker::getYangUserInterfaceCreateYangUserInterfaceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementGetYangUserInterfaceWorker::getYangUserInterfaceApplyAAAOnYangUserInterfaceStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementGetYangUserInterfaceWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveSystemManagementGetYangUserInterfaceWorker::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveSystemManagementGetYangUserInterfaceMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void WaveSystemManagementGetYangUserInterfaceWorker::getYangUserInterfaceCreateYangUserInterfaceStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    WaveSystemManagementGetYangUserInterfaceMessage *pWaveSystemManagementGetYangUserInterfaceMessage = dynamic_cast<WaveSystemManagementGetYangUserInterfaceMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

    prismAssert (NULL != pWaveSystemManagementGetYangUserInterfaceMessage, __FILE__, __LINE__);

#if 0
    YangUserInterface *pClonedYangUserInterface = WaveSystemManagementObjectManager::getClonedYangUserInterface ();

    prismAssert (NULL != pClonedYangUserInterface, __FILE__, __LINE__);

    pClonedYangUserInterface->printYinToString (pWaveSystemManagementGetYangUserInterfaceMessage->getYangUserInterface ());
#else
    WaveSystemManagementObjectManager::getYinForYangUserInterface (pWaveSystemManagementGetYangUserInterfaceMessage->getYangUserInterface ());
#endif

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void WaveSystemManagementGetYangUserInterfaceWorker::getYangUserInterfaceApplyAAAOnYangUserInterfaceStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
