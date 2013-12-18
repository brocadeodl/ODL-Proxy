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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/Attributes/AttributeSqlIn.h"
#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousInnerQueryContext.h"

namespace WaveNs
{

AttributeSqlIn::AttributeSqlIn (const vector<Attribute *> &attributes)
    : Attribute (AttributeType::AttributeTypeSqlIn)
{
    m_attributes    = attributes;
    m_isInnerQuery  = false;
}

AttributeSqlIn::AttributeSqlIn (WaveManagedObjectSynchronousInnerQueryContext *pWaveManagedObjectSynchronousInnerQueryContext, const string &attributeName)
    : Attribute (AttributeType::AttributeTypeSqlIn)
{
    m_isInnerQuery  = true;
    m_pWaveManagedObjectSynchronousInnerQueryContext = pWaveManagedObjectSynchronousInnerQueryContext;
    m_attributeName = attributeName;
}

AttributeSqlIn::~AttributeSqlIn ()
{
    UI32 numberOfAttributes = m_attributes.size ();

    for (UI32 i = 0; i < numberOfAttributes; i++)
    {
        delete (m_attributes[i]);
    }
}

bool AttributeSqlIn::validate () const
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn::validate : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

bool AttributeSqlIn::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN == attributeConditionOperator || WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_IN == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

AttributeSqlIn &AttributeSqlIn::operator = (const AttributeSqlIn &attribute)
{
    Attribute::operator = (attribute);

    return (*this);
}

string AttributeSqlIn::getSqlType ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return "";
}

void AttributeSqlIn::setupOrm (OrmTable *pOrmTable)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

string AttributeSqlIn::getSqlForCreate ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return "";
}

void AttributeSqlIn::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSqlIn::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    if (false == m_isInnerQuery)
    {
        UI32    numberOfAttributesForSqlIn  = m_attributes.size ();
        string  attributeColumnName         = "";
        string  currentAttributeValue       = "";
        string  allAttributeValues          = "";

        for (UI32 i = 0; i < numberOfAttributesForSqlIn; i++)
        {
            if (NULL == m_attributes[i])
            {
                trace (TRACE_LEVEL_FATAL, "AttributeSqlIn::getSqlForSelect : Attribute is not initialized!");
                prismAssert (false, __FILE__, __LINE__);
            }

            if (0 == i)
            {
                // store the attribute column name to be compared with other attributes
                attributeColumnName = m_attributes[i]->getAttributeName ();
            }
            else
            {
                // compare all other attribute's column names against the initial attribute's column name

                if (m_attributes[i]->getAttributeName () != attributeColumnName)
                {
                    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn::getSqlForSelect : Attribute index: ") + i + " is for column \"" + m_attributes[i]->getAttributeName () + "\" and is not consistent with the initial attribute's column \"" + attributeColumnName + "\"");
                    prismAssert (false, __FILE__, __LINE__);
                }
            }

            // build up the value string

            m_attributes[i]->toEscapedString (currentAttributeValue);

            allAttributeValues += string ("'") + currentAttributeValue + string ("'");

            if (i != (numberOfAttributesForSqlIn - 1))
            {
                allAttributeValues += string (",");
            }
        }

        sqlForSelect += attributeColumnName + FrameworkToolKit::localize (attributeConditionOperator) + string ("(") + allAttributeValues + string (")");
    }
    else
    {
        UI32 numberOfSelectFields = (m_pWaveManagedObjectSynchronousInnerQueryContext->getSelectFields ()).size ();
        if (1 != numberOfSelectFields)
        {
            trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn::getSqlForSelect : The inner query context cannot have more than 1 select fields but it has ") + numberOfSelectFields );
            prismAssert (false, __FILE__, __LINE__);
        }
        string  innerSqlQuery   = m_pWaveManagedObjectSynchronousInnerQueryContext->getSqlForSelect (false);

        sqlForSelect += m_attributeName + FrameworkToolKit::localize (attributeConditionOperator) + string ("(") + innerSqlQuery + string (")");
        trace (TRACE_LEVEL_DEBUG, string ("AttributeSqlIn:: getSqlForSelect : Sql Generated is :") + sqlForSelect);
    }
}

void AttributeSqlIn::toString (string &valueString)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::fromString (const string &valueString)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void *AttributeSqlIn::getPData ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

void AttributeSqlIn::setValue (const void *pData)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

Attribute *AttributeSqlIn::clone ()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

void AttributeSqlIn::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::updateOrmRelations (const PrismPersistableObject *pPrismPersistableObject)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

WaveManagedObject *AttributeSqlIn::getComposedManagedObject (const ObjectId &childObjectId)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return NULL;
}

bool AttributeSqlIn::isDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);

    return (false);
}

void AttributeSqlIn::updateKeyString (const PrismPersistableObject *pPrismPersistableObject)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::getRestRowData (string &restRowData)
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

map<string, string> AttributeSqlIn::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

void AttributeSqlIn::setDefaultValue()
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn:: : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeSqlIn::getCValue(WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__);
}

bool AttributeSqlIn::isCurrentValueSameAsDefault () const
{
    trace (TRACE_LEVEL_FATAL, string ("AttributeSqlIn::isCurrentValueSameAsDefault : This attribute is only for the SQL IN condition and does not make sense to invoke this method."));
    prismAssert (false, __FILE__, __LINE__);
    return (false);
}
}
