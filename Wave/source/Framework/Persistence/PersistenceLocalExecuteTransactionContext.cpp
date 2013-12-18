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
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceLocalExecuteTransactionContext.h"

namespace WaveNs
{

PersistenceLocalExecuteTransactionContext::PersistenceLocalExecuteTransactionContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_currentTransactionId (0),
      m_isCommit (false),
      m_isRollback (false),
      m_isPrepare (false),
      m_isConfigurationChange (false),
      m_isConfigurationTimeChange (false),
      m_activeResult (WAVE_MESSAGE_SUCCESS),
      m_standbyResult (WAVE_MESSAGE_SUCCESS)
{
}

PersistenceLocalExecuteTransactionContext::PersistenceLocalExecuteTransactionContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_currentTransactionId (0),
      m_isCommit (false),
      m_isRollback (false),
      m_isPrepare (false),
      m_isConfigurationChange (false),
      m_isConfigurationTimeChange (false),
      m_activeResult (WAVE_MESSAGE_SUCCESS),
      m_standbyResult (WAVE_MESSAGE_SUCCESS)
{
}

PersistenceLocalExecuteTransactionContext::~PersistenceLocalExecuteTransactionContext ()
{
}

UI32 PersistenceLocalExecuteTransactionContext::getCurrentTransactionId () const
{
    return (m_currentTransactionId);
}

void PersistenceLocalExecuteTransactionContext::setCurrentTransactionId (const UI32 &currentTransactionId)
{
    m_currentTransactionId = currentTransactionId;
}

LocationId PersistenceLocalExecuteTransactionContext::getSenderLocationId () const
{
    return (m_senderLocationId);
}

void PersistenceLocalExecuteTransactionContext::setSenderLocationId (const LocationId &locationId)
{
    m_senderLocationId = locationId;
}

bool PersistenceLocalExecuteTransactionContext::getIsCommit () const
{
    return (m_isCommit);
}

void PersistenceLocalExecuteTransactionContext::setIsCommit (const bool &isCommit)
{
    m_isCommit = isCommit;
}

bool PersistenceLocalExecuteTransactionContext::getIsRollback () const
{
    return (m_isRollback);
}

void PersistenceLocalExecuteTransactionContext::setIsRollback (const bool &isRollback)
{
    m_isRollback = isRollback;
}

ResourceId PersistenceLocalExecuteTransactionContext::getActiveResult () const
{
    return (m_activeResult);
}

void PersistenceLocalExecuteTransactionContext::setActiveResult (const ResourceId &result)
{
    m_activeResult = result;
}

ResourceId PersistenceLocalExecuteTransactionContext::getStandbyResult () const
{
    return (m_standbyResult);
}

void PersistenceLocalExecuteTransactionContext::setStandbyResult (const ResourceId &result)
{
    m_standbyResult = result;
}

bool PersistenceLocalExecuteTransactionContext::getIsPrepare () const
{
    return (m_isPrepare);
}

void PersistenceLocalExecuteTransactionContext::setIsPrepare (const bool &isPrepare)
{
    m_isPrepare = isPrepare;
}

bool PersistenceLocalExecuteTransactionContext::getIsConfigurationChange () const
{
    return (m_isConfigurationChange);
}

void PersistenceLocalExecuteTransactionContext::setIsConfigurationChange (const bool &isConfigurationChange)
{
    m_isConfigurationChange = isConfigurationChange;
}

bool PersistenceLocalExecuteTransactionContext::getIsConfigurationTimeChange () const
{
    return (m_isConfigurationTimeChange);
}

void PersistenceLocalExecuteTransactionContext::setIsConfigurationTimeChange (const bool &isConfigurationTimeChange)
{
    m_isConfigurationTimeChange = isConfigurationTimeChange;
}

}
