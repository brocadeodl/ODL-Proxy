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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Framework/Utils/AssertUtils.h"
#include "VcsFabric/Local/Test/VcsNodeLocalEventTestObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"


namespace DcmNs
{

VcsNodeLocalEventTestObjectManager::VcsNodeLocalEventTestObjectManager ()
    : WaveObjectManager ("Base services Events test object manager")
{
}

VcsNodeLocalEventTestObjectManager *VcsNodeLocalEventTestObjectManager::getInstance ()
{
    static VcsNodeLocalEventTestObjectManager *pVcsNodeLocalEventTestObjectManager = NULL;

    if (NULL == pVcsNodeLocalEventTestObjectManager)
    {
        pVcsNodeLocalEventTestObjectManager = new VcsNodeLocalEventTestObjectManager ();
        WaveNs::prismAssert (NULL != pVcsNodeLocalEventTestObjectManager, __FILE__, __LINE__);
    }

    return (pVcsNodeLocalEventTestObjectManager);
}

VcsNodeLocalEventTestObjectManager::~VcsNodeLocalEventTestObjectManager ()
{
}

PrismServiceId VcsNodeLocalEventTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void VcsNodeLocalEventTestObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_VALID, reinterpret_cast<PrismEventHandler> (&VcsNodeLocalEventTestObjectManager::domainValidEventHandler));
    listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_REACHABLE, reinterpret_cast<PrismEventHandler> (&VcsNodeLocalEventTestObjectManager::domainReachableEventHandler));
    listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_DOMAIN_UNREACHABLE, reinterpret_cast<PrismEventHandler> (&VcsNodeLocalEventTestObjectManager::domainUnreachableEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VcsNodeLocalEventTestObjectManager::domainValidEventHandler (const VcsNodeDomainValidEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    trace (TRACE_LEVEL_DEBUG, string ("Domain valid for m_domainId   = ") + pEvent->getDomainId ());

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsNodeLocalEventTestObjectManager::domainReachableEventHandler (const VcsNodeDomainReachableEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    trace (TRACE_LEVEL_DEBUG, string ("Domain reachable for m_domainId   = ") + pEvent->getDomainId ());

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

void VcsNodeLocalEventTestObjectManager::domainUnreachableEventHandler (const VcsNodeDomainUnreachableEvent *&pEvent)
{
    static UI32 numberOfEventsRceivedSoFar = 0;

    numberOfEventsRceivedSoFar++;

    trace (TRACE_LEVEL_DEBUG, string ("Domain unreachable for m_domainId   = ") + pEvent->getDomainId ());

    reply (reinterpret_cast<const PrismEvent *&> (pEvent));
}

}
