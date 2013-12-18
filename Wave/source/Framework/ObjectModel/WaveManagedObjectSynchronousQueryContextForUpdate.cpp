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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mithun B S
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

namespace WaveNs
{

WaveManagedObjectSynchronousQueryContextForUpdate::WaveManagedObjectSynchronousQueryContextForUpdate (const string &classToQueryFor)
    : WaveManagedObjectSynchronousQueryContext (classToQueryFor)
{
    WaveObjectManager *pWaveObjectManagerForCurrentThread = PrismThread::getWaveObjectManagerForCurrentThread ();
    prismAssert (NULL != pWaveObjectManagerForCurrentThread, __FILE__, __LINE__);

    m_WaveManagedObjectToAddUpdateAttributes = pWaveObjectManagerForCurrentThread->createInMemoryManagedObject (classToQueryFor);
}

WaveManagedObjectSynchronousQueryContextForUpdate::~WaveManagedObjectSynchronousQueryContextForUpdate ()
{
    delete m_WaveManagedObjectToAddUpdateAttributes;
}

void WaveManagedObjectSynchronousQueryContextForUpdate::getSqlForUpdate (string &sql)
{
    m_WaveManagedObjectToAddUpdateAttributes->getSqlForUpdate (sql, OrmRepository::getWaveCurrentSchema ());
    updateAssociationTableAndWhereClause (sql);

    sql += ";";

    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForUpdate::getSqlForUpdate: Bulk update sql statement:" + sql);   
}

WaveManagedObject * WaveManagedObjectSynchronousQueryContextForUpdate::getWaveManagedObjectToAddUpdateAttributes ()
{
    return m_WaveManagedObjectToAddUpdateAttributes; 
}

void WaveManagedObjectSynchronousQueryContextForUpdate::setAttributeObjectIdVectorAssociation (AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::setAttributeObjectIdVectorAssociation: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::addOrderField (const string &orderField, const bool &ascending)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::addOrderField: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::addNTupleFormatForOrderField (const string &keyName, const char &tupleSeparator, const UI32 &numberOfTuples)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::addNTupleFormatForOrderField: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::addSelectField (const string &selectField)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::addSelectField: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::addSelectFieldForObjectId (const string &selectField)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::addSelectFieldForObjectId: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::addSelectFields (const vector<string> &selectFields)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::addSelectFields: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

vector<string> WaveManagedObjectSynchronousQueryContextForUpdate::getSelectFields () const
{
    vector<string> selectFields;
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getSelectFields: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return selectFields;
}

vector<string> WaveManagedObjectSynchronousQueryContextForUpdate::getSelectFieldsInManagedObject () const
{
    vector<string> selectFields;
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getSelectFieldsInManagedObject: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return selectFields;
}

bool WaveManagedObjectSynchronousQueryContextForUpdate::getLoadOneToManyRelationships () const
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getLoadOneToManyRelationships: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return false;
}

void WaveManagedObjectSynchronousQueryContextForUpdate::setLoadOneToManyRelationships (const bool &loadOneToManyRelationships)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::setLoadOneToManyRelationships: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

bool WaveManagedObjectSynchronousQueryContextForUpdate::getLoadCompositions () const
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getLoadCompositions: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return false;
}

void WaveManagedObjectSynchronousQueryContextForUpdate::setLoadCompositions (const bool loadCompositions)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::setLoadCompositions: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::addLimitForQueryResults (UI32 limitForQueryResults)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::addLimitForQueryResults: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

UI32 WaveManagedObjectSynchronousQueryContextForUpdate::getPageSizeForQueryResults ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getPageSizeForQueryResults : This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return (0);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::setPageSizeForQueryResults (UI32 limitForQueryResults)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::setPageSizeForQueryResults : This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousQueryContextForUpdate::setPageQueryContext (WaveObjectManager* callingOm)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::setPageQueryContext : This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);
}

bool WaveManagedObjectSynchronousQueryContextForUpdate::isPageQueryContextSet ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::isPageQueryContextSet : This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

string WaveManagedObjectSynchronousQueryContextForUpdate::getPageQuerySql ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getPageQuerySql : This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return ("");
}

void WaveManagedObjectSynchronousQueryContextForUpdate::moveOffsetOnSuccessfulQuery ()
{
    // Do nothing here. since this is called as part of the querySynchronously (QueryContext) 
}

string WaveManagedObjectSynchronousQueryContextForUpdate::getSqlWithPagingEnabled (WaveObjectManager* callingOm)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForUpdate::getSqlWithPagingEnabled: This method should never be invoked for this update context.");   
    prismAssert (false, __FILE__, __LINE__);

    return ("");
}

}

