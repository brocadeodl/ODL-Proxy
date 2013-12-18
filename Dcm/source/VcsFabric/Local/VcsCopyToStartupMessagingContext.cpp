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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "VcsFabric/Local/VcsCopyToStartupMessagingContext.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"

namespace DcmNs
{

VcsCopyToStartupMessagingContext::VcsCopyToStartupMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_contextInfo (UNKNOWN_CONTEXT_INFO),
    m_statusForCopyRunningToStartupOnActive (WAVE_MESSAGE_ERROR),
    m_statusForCopyRunningToStartupOnStandby (WAVE_MESSAGE_ERROR)
{
}

VcsCopyToStartupMessagingContext::~VcsCopyToStartupMessagingContext ()
{
}
           
UI32 VcsCopyToStartupMessagingContext::getContextInfo () const
{
    return (m_contextInfo);
}

void VcsCopyToStartupMessagingContext::setContextInfo (const UI32 contextInfo)
{
    m_contextInfo = contextInfo;
}

ResourceId VcsCopyToStartupMessagingContext::getStatusForCopyRunningToStartupOnActive () const
{
    return (m_statusForCopyRunningToStartupOnActive);
}

void VcsCopyToStartupMessagingContext::setStatusForCopyRunningToStartupOnActive (const ResourceId &status)
{
    m_statusForCopyRunningToStartupOnActive = status;
}

ResourceId VcsCopyToStartupMessagingContext::getStatusForCopyRunningToStartupOnStandby () const
{
    return (m_statusForCopyRunningToStartupOnStandby);
}

void VcsCopyToStartupMessagingContext::setStatusForCopyRunningToStartupOnStandby (const ResourceId &status)
{
    m_statusForCopyRunningToStartupOnStandby = status;
}

}
