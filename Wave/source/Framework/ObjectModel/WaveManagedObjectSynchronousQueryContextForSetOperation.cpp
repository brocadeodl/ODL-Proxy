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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Pritee Ghosh                                                 *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForSetOperation.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"

namespace WaveNs
{

WaveManagedObjectSynchronousQueryContextForSetOperation::WaveManagedObjectSynchronousQueryContextForSetOperation ()
{
}

WaveManagedObjectSynchronousQueryContextForSetOperation::~WaveManagedObjectSynchronousQueryContextForSetOperation ()
{
    UI32 i                     = 0;
    UI32 numberOfQueryContexts = m_waveManagedObjectInnerQueryContexts.size ();

    for (i = 0; i < numberOfQueryContexts; i++)
    {
        trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForSetOperation::~WaveManagedObjectSynchronousQueryContextForSetOperation (): deleting innerQueryContext.");
        delete m_waveManagedObjectInnerQueryContexts[i];
    }

    m_waveManagedObjectInnerQueryContexts.clear ();
    m_contextSetOperators.clear ();
    m_contextDuplicatesNeeded.clear ();
}

string WaveManagedObjectSynchronousQueryContextForSetOperation::getSql () 
{
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForSetOperation::getSql : Entering ...");

    string   sql                   = "";
    UI32     i                     = 0;
    UI32     numberOfQueryContexts = m_waveManagedObjectInnerQueryContexts.size ();

    for (i = 0; i < numberOfQueryContexts; i++)
    {
        string singleSql = (m_waveManagedObjectInnerQueryContexts[i])->getSqlForSelect (false);

        if (numberOfQueryContexts != 1 && (i < (numberOfQueryContexts - 1)))
        {
            if (OPERATOR_UNION == m_contextSetOperators[i+1])
            {
                singleSql += " UNION ";
            }
            else if (OPERATOR_INTERSECT == m_contextSetOperators[i+1])
            {
                singleSql += " INTERSECT ";
            }
            else if (OPERATOR_EXCEPT == m_contextSetOperators[i+1])
            {
                singleSql += " EXCEPT ";
            }
            else
            {
                tracePrintf (TRACE_LEVEL_FATAL, true, false, "WaveManagedObjectSynchronousQueryContextForSetOperation::getSql: set operator not set for context : %d", i+1);
                prismAssert (false, __FILE__, __LINE__);
            }

            // for duplicates
            if (true == m_contextDuplicatesNeeded [i+1])
            {
                singleSql += " ALL ";
            }
        }

        sql += singleSql;
    }

    if (false == sql.empty())
    {
        // adding semicolon at the end
        sql += ";";
    }


    return (sql);
}

WaveManagedObjectSynchronousInnerQueryContext* WaveManagedObjectSynchronousQueryContextForSetOperation::addInnerQueryContext (const string classToQueryFor, const vector<string> &selectFields, const UI32 setOperator, const bool keepDuplicate)
{
    trace (TRACE_LEVEL_INFO, "WaveManagedObjectSynchronousQueryContextForSetOperation::addInnerQueryContext : Entering...");

    if (true == classToQueryFor.empty ())
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForSetOperation::addInnerQueryContext : classToQueryFor for an InnerContext can't be empty.");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (0 != m_waveManagedObjectInnerQueryContexts.size ())
    {
        if ((OPERATOR_UNION != setOperator) && (OPERATOR_INTERSECT != setOperator) && (OPERATOR_EXCEPT != setOperator))
        {
            trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForSetOperation::addInnerQueryContext : setOperator must be one of OPERATOR_UNION/OPERATOR_INTERSECT/OPERATOR_EXCEPT");
            prismAssert (false, __FILE__, __LINE__);
        }

    }

    WaveManagedObjectSynchronousInnerQueryContext* pInnerQueryContext = new WaveManagedObjectSynchronousInnerQueryContext (classToQueryFor);

    prismAssert (NULL != pInnerQueryContext, __FILE__, __LINE__);

    pInnerQueryContext->addSelectFields (selectFields);

    addQueryContext (pInnerQueryContext, setOperator, keepDuplicate);

    return (pInnerQueryContext);
}

void WaveManagedObjectSynchronousQueryContextForSetOperation::addQueryContext (WaveManagedObjectSynchronousInnerQueryContext *pInnerQueryContext, const UI32 setOperator, const bool keepDuplicate)
{
    trace (TRACE_LEVEL_DEBUG, "WaveManagedObjectSynchronousQueryContextForSetOperation::addQueryContext : Entering ...");
    //
    // Check for the select fields - No. of select fields cannot be zero
    // Select field count should be same in all Query contexts
    // Order of datatype of select field should be same in all Query contexts
    //
    if (0 == m_waveManagedObjectInnerQueryContexts.size ())
    {
        if (0 == (pInnerQueryContext->getSelectFields ()).size())
        {
            trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForSetOperation::addQueryContext : Select Fields should be added in the Query context.");
            prismAssert (false, __FILE__, __LINE__);
        }
        m_waveManagedObjectInnerQueryContexts.push_back (pInnerQueryContext);
        m_contextSetOperators.push_back (OPERATOR_NONE);
        m_contextDuplicatesNeeded.push_back (false);
    }
    else
    {
        UI32 selectFieldCount = m_waveManagedObjectInnerQueryContexts[0]->getSelectFields ().size();

        if (selectFieldCount == (pInnerQueryContext->getSelectFields ()).size())
        {
            // check for the type of each select field.
            string          moNameForFirstContext        = m_waveManagedObjectInnerQueryContexts[0]->getClassToQueryFor ();
            vector<string>  selectFieldsInFirstContext   = m_waveManagedObjectInnerQueryContexts[0]->getSelectFields ();
            string          moNameForCurrentContext      = pInnerQueryContext->getClassToQueryFor ();
            vector<string>  selectFieldsInCurrentContext = pInnerQueryContext->getSelectFields ();


            for (UI32 i = 0; i < selectFieldCount; i++)
            {
                //if (FrameworkToolKit::getFieldTypeInATable (moNameForFirstContext, selectFieldsInFirstContext[i]) != FrameworkToolKit::getFieldTypeInATable (moNameForCurrentContext, selectFieldsInCurrentContext[i]))
                if (FrameworkToolKit::getSqlTypeForAFieldInATable (moNameForFirstContext, selectFieldsInFirstContext[i]) != FrameworkToolKit::getSqlTypeForAFieldInATable (moNameForCurrentContext, selectFieldsInCurrentContext[i]))
                {
                    trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForSetOperation::addQueryContext : select fields in all context must be of same type.");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }           

            m_waveManagedObjectInnerQueryContexts.push_back (pInnerQueryContext);
            m_contextSetOperators.push_back (setOperator);
            m_contextDuplicatesNeeded.push_back (keepDuplicate);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "WaveManagedObjectSynchronousQueryContextForSetOperation::addQueryContext : Number of Select Fields should be same in all the Query contexts.");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}




}
