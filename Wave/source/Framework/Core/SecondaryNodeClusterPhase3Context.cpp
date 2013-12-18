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
 *   Author : Anil ChannaveeraSetty                                        *
 ***************************************************************************/

#include "Framework/Core/SecondaryNodeClusterPhase3Context.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Trace/TraceMessages.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

SecondaryNodeClusterPhase3Context::SecondaryNodeClusterPhase3Context(PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, bool resetSecondaryNodeClusterCreationFlag, bool clusterHaSyncInProgressFlag)
    : SecondaryNodeClusterContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps, false, clusterHaSyncInProgressFlag)
{
    m_resetSecondaryNodeClusterCreationFlag = resetSecondaryNodeClusterCreationFlag;
}

SecondaryNodeClusterPhase3Context::SecondaryNodeClusterPhase3Context(PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, bool resetSecondaryNodeClusterCreationFlag, bool clusterHaSyncInProgressFlag)
    : SecondaryNodeClusterContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps, clusterHaSyncInProgressFlag)
{
    m_resetSecondaryNodeClusterCreationFlag = resetSecondaryNodeClusterCreationFlag;
}

SecondaryNodeClusterPhase3Context::~SecondaryNodeClusterPhase3Context()
{
    if (m_resetSecondaryNodeClusterCreationFlag)
    {   
        (PrismFrameworkObjectManager::getInstance ())->setSecondaryNodeClusterCreationFlag (false);
        if (true == (PrismFrameworkObjectManager::getInstance ())->getNeedNotifyClusterReadyState())
        {
            trace (TRACE_LEVEL_INFO, "SecondaryNodeClusterPhase3Context::Destructor: Notify on ClusterReady event for secondary join success event");
            bool readyState = true;
            (PrismFrameworkObjectManager::getInstance ())->notifyClusterReadyState (readyState);
            (PrismFrameworkObjectManager::getInstance ())->setNeedNotifyClusterReadyState (false);
        }

        (PrismFrameworkObjectManager::getInstance ())->resumePostponedMessages ();
    }
}

}
