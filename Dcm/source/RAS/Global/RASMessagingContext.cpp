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
 *   Author : James Chen                                             *
 ***************************************************************************/

#include "RAS/Global/RASMessagingContext.h"

namespace DcmNs
{

RASMessagingContext::RASMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_remoteLocationId          (0)
{
}


RASMessagingContext::~RASMessagingContext ()
{
}
LocationId RASMessagingContext::getRemoteLocationId ()
{
    return (m_remoteLocationId);
}

void RASMessagingContext::setRemoteLocationId (const LocationId &remoteLocationId)
{
    m_remoteLocationId = remoteLocationId;
}

UI32 RASMessagingContext::getMappedId () const
{
    return (m_mappedId);
}

PrismMessage * RASMessagingContext::getPMessage () const
{
	return m_pMessage;
}
void RASMessagingContext::setPMessage (PrismMessage *pMessage)
{
	m_pMessage = pMessage;
}

void RASMessagingContext::setMappedId (const UI32 &mapped_id)
{
    m_mappedId = mapped_id;
}

}
