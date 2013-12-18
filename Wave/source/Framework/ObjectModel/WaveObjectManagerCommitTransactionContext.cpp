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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveObjectManagerCommitTransactionContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"

namespace WaveNs
{

WaveObjectManagerCommitTransactionContext::WaveObjectManagerCommitTransactionContext () :
    m_isADelayedCommitTransaction (false)
{
}

WaveObjectManagerCommitTransactionContext::WaveObjectManagerCommitTransactionContext (PrismLinearSequencerContext *pPrismLinearSequencerContext) :
    m_isADelayedCommitTransaction (pPrismLinearSequencerContext->getIsADelayedCommitTransaction ())
{
}

WaveObjectManagerCommitTransactionContext::WaveObjectManagerCommitTransactionContext (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) :
    m_isADelayedCommitTransaction (pPrismSynchronousLinearSequencerContext->getIsADelayedCommitTransaction ())
{
}

WaveObjectManagerCommitTransactionContext::~WaveObjectManagerCommitTransactionContext ()
{
}

bool WaveObjectManagerCommitTransactionContext::getIsADelayedCommitTransaction () const
{
    return (m_isADelayedCommitTransaction);
}

void WaveObjectManagerCommitTransactionContext::setIsADelayedCommitTransaction (const bool &isADelayedCommitTransaction)
{
    m_isADelayedCommitTransaction = isADelayedCommitTransaction;
}

}
