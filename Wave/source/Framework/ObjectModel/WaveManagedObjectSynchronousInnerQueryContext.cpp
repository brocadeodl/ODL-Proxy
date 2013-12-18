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
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousInnerQueryContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

#include <algorithm>

namespace WaveNs
{

WaveManagedObjectSynchronousInnerQueryContext::WaveManagedObjectSynchronousInnerQueryContext (const string &classToQueryFor)
    : WaveManagedObjectSynchronousQueryContext (classToQueryFor)
{
}

WaveManagedObjectSynchronousInnerQueryContext::~WaveManagedObjectSynchronousInnerQueryContext ()
{
}

UI32 WaveManagedObjectSynchronousInnerQueryContext::getPageSizeForQueryResults ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::getPageSizeForQueryResults : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);

    return (0);
}

void WaveManagedObjectSynchronousInnerQueryContext::setPageSizeForQueryResults (UI32 limitForQueryResults)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::setPageSizeForQueryResults : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);
}

void WaveManagedObjectSynchronousInnerQueryContext::setPageQueryContext (WaveObjectManager* callingOm)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::setPageQueryContext : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);
}

bool WaveManagedObjectSynchronousInnerQueryContext::isPageQueryContextSet ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::isPageQueryContextSet : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

string WaveManagedObjectSynchronousInnerQueryContext::getPageQuerySql ()
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::getPageQuerySql : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);

    return ("");
}

void WaveManagedObjectSynchronousInnerQueryContext::moveOffsetOnSuccessfulQuery ()
{
    // Do nothing here. since this is called as part of the querySynchronously (QueryContext) 
}

string WaveManagedObjectSynchronousInnerQueryContext::getSqlWithPagingEnabled (WaveObjectManager* callingOm)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::getSqlWithPagingEnabled : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);

    return ("");
}

string WaveManagedObjectSynchronousInnerQueryContext::getSqlForSelect (const bool &forCount)
{
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousInnerQueryContext::getSqlForSelect : Entering...");   

    string sql = getSql (forCount);
    //Removing the last semicolon
    sql.erase (sql.size () - 1);
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousInnerQueryContext::getSqlForSelect: After Erase" + sql);   

    return (sql);
}

void WaveManagedObjectSynchronousInnerQueryContext::addSelectField (const string &selectField)
{
    if (true == (isSelectFieldAlreadyAdded (selectField)))
    {
        return;
    }

    bool            isAColumn       = false;
    bool            isARelation     = false;
    OrmRelationType ormRelationType = ORM_RELATION_TYPE_ONE_TO_ONE;

    OrmRepository::getEntityDetails (m_classToQueryFor, selectField, isAColumn, isARelation, ormRelationType);

    if (true == isAColumn)
    {
        m_selectFields.push_back (selectField);
    }
    else if (true == isARelation)
    {
        if (ORM_RELATION_TYPE_ONE_TO_ONE == ormRelationType)
        {
            if (false == (isSelectFieldAlreadyAdded (selectField  + "ClassId")))
            {
                m_selectFields.push_back (selectField + "ClassId");
            }
            if (false == (isSelectFieldAlreadyAdded (selectField  + "InstanceId")))
            {
                m_selectFields.push_back (selectField + "InstanceId");
            }
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::addSelectField : Not Supported Yet : You cannot have a One to many relations as select fields in inner query context.");   
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        const bool isASimpleObjectIdColumn = OrmRepository::isASimpleObjectIdColumn (m_classToQueryFor, selectField);

        if (true == isASimpleObjectIdColumn)
        {
            if (false == (isSelectFieldAlreadyAdded (selectField  + "ClassId")))
            {
                m_selectFields.push_back (selectField + "ClassId");
            }
            if (false == (isSelectFieldAlreadyAdded (selectField  + "InstanceId")))
            {
                m_selectFields.push_back (selectField + "InstanceId");
            }
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void WaveManagedObjectSynchronousInnerQueryContext::addSelectFields (const vector<string> &selectFields)
{
    const UI32 numberOfSelectFields = selectFields.size ();
          UI32 i                    = 0;

    for (i = 0; i < numberOfSelectFields; i++)
    {
        addSelectField (selectFields[i]);
    }
}

void WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectIdClassId (const string &selectField)
{
    addSelectFieldForObjectIdWithSuffix (selectField, "ClassId");
}

void WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectIdInstanceId (const string &selectField)
{
    addSelectFieldForObjectIdWithSuffix (selectField, "InstanceId");
}

void WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectIdWithSuffix (const string &selectField, string objectIdSuffix)
{
    bool            isAColumn       = false;
    bool            isARelation     = false;
    OrmRelationType ormRelationType = ORM_RELATION_TYPE_ONE_TO_ONE;

    OrmRepository::getEntityDetails (m_classToQueryFor, selectField, isAColumn, isARelation, ormRelationType);

    if (true == isAColumn)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectIdWithSuffix : You cannot have a simple column while selecting ObjectId.");   
        prismAssert (false, __FILE__, __LINE__);
    }
    else if (true == isARelation)
    {
        if (ORM_RELATION_TYPE_ONE_TO_ONE == ormRelationType)
        {
            if (true == (isSelectFieldAlreadyAdded (selectField  + objectIdSuffix)))
            {
                return;
            }

            m_selectFields.push_back (selectField + objectIdSuffix);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectIdWithSuffix : Not Supported Yet : You cannot have a One to many relations as select fields in inner query context.");   
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        const bool isASimpleObjectIdColumn = OrmRepository::isASimpleObjectIdColumn (m_classToQueryFor, selectField);

        if (true == isASimpleObjectIdColumn)
        {
            if (true == (isSelectFieldAlreadyAdded (selectField  + objectIdSuffix)))
            {
                return;
            }

            m_selectFields.push_back (selectField + objectIdSuffix);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectId (const string &selectField)
{
    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousInnerQueryContext::addSelectFieldForObjectId : This method should never be invoked for this inner query context.");   
    prismAssert (false, __FILE__, __LINE__);
}

bool WaveManagedObjectSynchronousInnerQueryContext::isSelectFieldAlreadyAdded (const string &selectField) const
{
    bool isAlreadyPresent = false;

    vector<string>::const_iterator element = find (m_selectFields.begin (), m_selectFields.end (), selectField);

    if (m_selectFields.end () != element)
    {   
        isAlreadyPresent = true;
    }
    else
    {   
        isAlreadyPresent = false;
    }

    return (isAlreadyPresent);
}

}
