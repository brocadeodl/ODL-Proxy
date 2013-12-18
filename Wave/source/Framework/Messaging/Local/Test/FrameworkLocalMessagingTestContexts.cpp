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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestContexts.h"

namespace WaveNs
{

FrameworkLocalMessagingTestContext::FrameworkLocalMessagingTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_remoteLocationId          (0),
      m_numberOfMessagesToSend    (0),
      m_numberOfMessagesSend      (0)
{
}

FrameworkLocalMessagingTestContext::~FrameworkLocalMessagingTestContext ()
{
}

LocationId FrameworkLocalMessagingTestContext::getRemoteLocationId ()
{
    return (m_remoteLocationId);
}

void FrameworkLocalMessagingTestContext::setRemoteLocationId (const LocationId &remoteLocationId)
{
    m_remoteLocationId = remoteLocationId;
}

void FrameworkLocalMessagingTestContext::setNumberOfMessagesToSend (const UI32 &numberOfMessagesToSend)
{
    m_numberOfMessagesToSend = numberOfMessagesToSend;
}

UI32 FrameworkLocalMessagingTestContext::getNumberOfMessagesToSend ()
{
    return (m_numberOfMessagesToSend);
}

void FrameworkLocalMessagingTestContext::incrementNumberOfMessagesSend ()
{
    ++m_numberOfMessagesSend;
}

void FrameworkLocalMessagingTestContext::resetNumberOfMessagesSend ()
{
    m_numberOfMessagesSend = 0;
}

UI32 FrameworkLocalMessagingTestContext::getNumberOfMessagesSend ()
{
    return (m_numberOfMessagesSend);
}

bool FrameworkLocalMessagingTestContext::areAllMessagesSent ()
{
    if (m_numberOfMessagesSend == m_numberOfMessagesToSend)
    {
        return true;
    }

    return false;
}

}
