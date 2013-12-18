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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/DistributedLog/Test/DistributedLogTestContext.h"

namespace WaveNs
{

DistributedLogTestContext::DistributedLogTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_originalMaxAllowedLogEntries  (0),
    m_currentMaxAllowedLogEntries   (0),
    m_firstLogId                    (0),
    m_nextLogId                     (0),
    m_prismServiceId              (0),
    m_numberOfFailures              (0)
{
}

DistributedLogTestContext::~DistributedLogTestContext ()
{
}

UI64 DistributedLogTestContext::getOriginalMaxAllowedLogEntries () const
{
    return (m_originalMaxAllowedLogEntries);
}

void DistributedLogTestContext::setOriginalMaxAllowedLogEntries (const UI64 &originalMaxAllowedLogEntries)
{
    m_originalMaxAllowedLogEntries = originalMaxAllowedLogEntries;
}

UI64 DistributedLogTestContext::getCurrentMaxAllowedLogEntries () const
{
    return (m_currentMaxAllowedLogEntries);
}

void DistributedLogTestContext::setCurrentMaxAllowedLogEntries (const UI64 &currentMaxAllowedLogEntries)
{
    m_currentMaxAllowedLogEntries = currentMaxAllowedLogEntries;
}

UI64 DistributedLogTestContext::getFirstLogId () const
{
    return (m_firstLogId);
}

void DistributedLogTestContext::setFirstLogId (const UI64 &firstLogId)
{
    m_firstLogId = firstLogId;
}

UI64 DistributedLogTestContext::getNextLogId () const
{
    return (m_nextLogId);
}

void DistributedLogTestContext::setNextLogId (const UI64 &nextLogId)
{
    m_nextLogId = nextLogId;
}

PrismServiceId DistributedLogTestContext::getPrismServiceId () const
{
    return (m_prismServiceId);
}

void DistributedLogTestContext::setPrismServiceId (const PrismServiceId &prismServiceId)
{
    m_prismServiceId = prismServiceId;
}

ObjectId DistributedLogTestContext::getManagedObjectId () const
{
    return (m_managedObjectId);
}

void DistributedLogTestContext::setManagedObjectId (const ObjectId &managedObjectId)
{
    m_managedObjectId = managedObjectId;
}

UI32 DistributedLogTestContext::getNumberOfFailures () const
{
    return (m_numberOfFailures);
}

void DistributedLogTestContext::setNumberOfFailures (UI32 numberOfFailures)
{
    m_numberOfFailures = numberOfFailures;
}

void DistributedLogTestContext::incrementNumberOfFailures ()
{
    m_numberOfFailures += 1;
}

}
