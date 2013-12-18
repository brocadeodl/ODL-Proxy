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
 *   Author : Jitendra Singh                                                 *
 ***************************************************************************/

#include "Framework/Postboot/PrismPostbootAgentThreadContext.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

PrismPostbootAgentThreadContext::PrismPostbootAgentThreadContext ()
{
    m_pPrismPostPersistenceBootMessage = NULL;
    m_pPostbootMutex = NULL;
    m_pPostbootSynchronizingCondition = NULL;
}

PrismPostbootAgentThreadContext::~PrismPostbootAgentThreadContext ()
{
}

void   PrismPostbootAgentThreadContext::setPostbootMessagePointer (PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage)
{
    m_pPrismPostPersistenceBootMessage = pPrismPostPersistenceBootMessage;

}

PrismPostPersistenceBootMessage *PrismPostbootAgentThreadContext::getPostbootMessagePointer ()
{
    return m_pPrismPostPersistenceBootMessage;
}

void PrismPostbootAgentThreadContext::setPostbootMutex (PrismMutex *pPostbootMutex)
{
    m_pPostbootMutex = pPostbootMutex;
}

PrismMutex *PrismPostbootAgentThreadContext::getPostbootMutex () const
{   
    return (m_pPostbootMutex);
}   
    
void PrismPostbootAgentThreadContext::setPostbootSynchronizingCondition (PrismCondition *pPostbootSynchronizingCondition)
{   
    m_pPostbootSynchronizingCondition = pPostbootSynchronizingCondition;
}

PrismCondition *PrismPostbootAgentThreadContext::getPostbootSynchronizingCondition () const
{   
    return (m_pPostbootSynchronizingCondition);
}

}
