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

#include "Framework/Persistence/PersistenceObjectManagerExecuteTransactionAsynchronousContext.h"

namespace WaveNs
{
    
PersistenceObjectManagerExecuteTransactionAsynchronousContext::PersistenceObjectManagerExecuteTransactionAsynchronousContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext (pCaller, pCallback, pCallerContext),
      m_sql                         (""),
      m_isConfigurationChange       (false),
      m_isConfigurationTimeChange   (false),
      m_senderServiceCode           (0),
      m_senderLocationId            (0)
{   
}   

PersistenceObjectManagerExecuteTransactionAsynchronousContext::~PersistenceObjectManagerExecuteTransactionAsynchronousContext ()
{
}
    
string PersistenceObjectManagerExecuteTransactionAsynchronousContext::getSql () const
{
    return (m_sql);
}

void PersistenceObjectManagerExecuteTransactionAsynchronousContext::setSql (const string &sql)
{
    m_sql = sql;
}

bool PersistenceObjectManagerExecuteTransactionAsynchronousContext::getIsConfigurationChange () const
{
    return (m_isConfigurationChange);
}

void PersistenceObjectManagerExecuteTransactionAsynchronousContext::setIsConfigurationChange (const bool &isConfigurationChange)
{
    m_isConfigurationChange = isConfigurationChange;
}

bool PersistenceObjectManagerExecuteTransactionAsynchronousContext::getIsConfigurationTimeChange () const
{
    return (m_isConfigurationTimeChange);
}

void PersistenceObjectManagerExecuteTransactionAsynchronousContext::setIsConfigurationTimeChange (const bool &isConfigurationTimeChange)
{
    m_isConfigurationTimeChange = isConfigurationTimeChange;
}

PrismServiceId PersistenceObjectManagerExecuteTransactionAsynchronousContext::getSenderServiceCode () const
{
    return (m_senderServiceCode);
}

void PersistenceObjectManagerExecuteTransactionAsynchronousContext::setSenderServiceCode (const PrismServiceId &senderServiceCode)
{
    m_senderServiceCode = senderServiceCode;
}

LocationId PersistenceObjectManagerExecuteTransactionAsynchronousContext::getSenderLocationId () const
{
    return (m_senderLocationId);
}

void PersistenceObjectManagerExecuteTransactionAsynchronousContext::setSenderLocationId (const LocationId &senderLocationId)
{
    m_senderLocationId = senderLocationId;
}

}
