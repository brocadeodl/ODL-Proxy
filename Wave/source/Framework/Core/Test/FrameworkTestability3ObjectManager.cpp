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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/FrameworkTestability3ObjectManager.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Messaging/Local/Test/FrameworkTestabilityEvents.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestTypes.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

FrameworkTestability3ObjectManager::FrameworkTestability3ObjectManager ()
    : WaveObjectManager ("Framework Testability3")
{
}

FrameworkTestability3ObjectManager *FrameworkTestability3ObjectManager::getInstance ()
{
    static FrameworkTestability3ObjectManager *pFrameworkTestability3ObjectManager = NULL;

    if (NULL == pFrameworkTestability3ObjectManager)
    {
        pFrameworkTestability3ObjectManager = new FrameworkTestability3ObjectManager ();
        WaveNs::prismAssert (NULL != pFrameworkTestability3ObjectManager, __FILE__, __LINE__);
    }

    return (pFrameworkTestability3ObjectManager);
}

FrameworkTestability3ObjectManager::~FrameworkTestability3ObjectManager ()
{
}

PrismServiceId FrameworkTestability3ObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void FrameworkTestability3ObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (FrameworkLocalMessagingTestObjectManager::getPrismServiceId (), FRAMEWORK_MESSAGING_LOCAL_TEST_EVENT1, reinterpret_cast<PrismEventHandler> (&FrameworkTestability3ObjectManager::frameworkTestabilityEvent1EventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

PrismMessage *FrameworkTestability3ObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    return (pPrismMessage);
}

void FrameworkTestability3ObjectManager::frameworkTestabilityEvent1EventHandler (const FrameworkTestabilityEvent1 *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    if (0 == (numberOfEventsRceivedSoFar % 10000))
    {
        trace (TRACE_LEVEL_INFO, string ("FrameworkTestability3ObjectManager::frameworkTestabilityEvent1EventHandler : Received Events ... ") + numberOfEventsRceivedSoFar);
        trace (TRACE_LEVEL_DEBUG, string ("m_eventUI32   = ") + pEvent->getEventUI32 ());
        trace (TRACE_LEVEL_DEBUG, string ("m_eventString = ") + pEvent->getEventString ());
    }

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

}
