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

#include "Framework/Attributes/AttributeSqlExists.h"
#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousInnerQueryContext.h"

namespace WaveNs
{

AttributeSqlExists::AttributeSqlExists (WaveManagedObjectSynchronousInnerQueryContext *pWaveManagedObjectSynchronousInnerQueryContext)
    : Attribute (AttributeType::AttributeTypeSqlExists)
{
    m_pWaveManagedObjectSynchronousInnerQueryContext = pWaveManagedObjectSynchronousInnerQueryContext;
}

AttributeSqlExists::~AttributeSqlExists ()
{
}

bool AttributeSqlExists::validate () const
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists::validate : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

bool AttributeSqlExists::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EXISTS == attributeConditionOperator || WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EXISTS == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

AttributeSqlExists &AttributeSqlExists::operator = (const AttributeSqlExists &attribute)
{
    Attribute::operator = (attribute);

    return (*this);
}

string AttributeSqlExists::getSqlType ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return "";
}

void AttributeSqlExists::setupOrm (OrmTable *pOrmTable)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

string AttributeSqlExists::getSqlForCreate ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return "";
}

void AttributeSqlExists::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSqlExists::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string  innerSqlQuery   = m_pWaveManagedObjectSynchronousInnerQueryContext->getSqlForSelect (false);

    sqlForSelect += FrameworkToolKit::localize (attributeConditionOperator) + string ("(") + innerSqlQuery + string (")");
    trace (TRACE_LEVEL_DEBUG, string ("AttributeSqlExists:: getSqlForSelect : Sql Generated is :") + sqlForSelect);
}

void AttributeSqlExists::toString (string &valueString)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::fromString (const string &valueString)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void *AttributeSqlExists::getPData ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

void AttributeSqlExists::setValue (const void *pData)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

Attribute *AttributeSqlExists::clone ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

void AttributeSqlExists::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::updateOrmRelations (const PrismPersistableObject *pPrismPersistableObject)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

WaveManagedObject *AttributeSqlExists::getComposedManagedObject (const ObjectId &childObjectId)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

bool AttributeSqlExists::isDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

void AttributeSqlExists::updateKeyString (const PrismPersistableObject *pPrismPersistableObject)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::getRestRowData (string &restRowData)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

map<string, string> AttributeSqlExists::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

void AttributeSqlExists::setDefaultValue()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists:: : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlExists::getCValue(WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__);
}

bool AttributeSqlExists::isCurrentValueSameAsDefault () const
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlExists::isCurrentValueSameAsDefault : This attribute is only for the SQL EXISTS condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
    return (false);
}
}
