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

#include "Framework/ObjectModel/WaveConditionSetBuilder.h"
#include "Framework/Attributes/Attribute.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

WaveConditionSetBuilder::WaveConditionSet::WaveConditionSet (const string &conditionSetName)
    : m_conditionSetName    (conditionSetName)
{
}

WaveConditionSetBuilder::WaveConditionSet::~WaveConditionSet ()
{
    UI32 numberOfAttributes = m_attributes.size ();

    for (UI32 i = 0; i < numberOfAttributes; i++)
    {
        delete (m_attributes[i]);
        m_attributes[i] =  NULL;
    }
}

const string & WaveConditionSetBuilder::WaveConditionSet::getConditionSetName () const
{
    return (m_conditionSetName);
}

void WaveConditionSetBuilder::WaveConditionSet::addAttributeToConditionSet (Attribute *pAttribute, AttributeConcatenationOperator concatenationOperation, AttributeConditionOperator conditionOperation)
{
    if (NULL == pAttribute)
    {
        trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::WaveConditionSet::addAttributeToConditionSet : Null attribute detected.  Please instantiate a new Attribute before adding it to the condition set.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_attributes.push_back (pAttribute);
    m_attributeConcatenationOperatorsMap[pAttribute] = concatenationOperation;
    m_attributeConditionOperatorsMap[pAttribute] = conditionOperation;
}

string WaveConditionSetBuilder::WaveConditionSet::getSql ()
{
    UI32    numberOfAttributes  = m_attributes.size ();
    string  sqlString           = "";

    if (0 == numberOfAttributes)
    {
        trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::WaveConditionSet::getSql : No attributes have been added to condition set: " + m_conditionSetName);
        prismAssert (false, __FILE__, __LINE__);
    }

    sqlString += "(";

    for (UI32 i = 0; i < numberOfAttributes; i++)
    {
        (m_attributes[i])->getSqlForSelect (sqlString, m_attributeConditionOperatorsMap[(m_attributes[i])]);

        if (i != (numberOfAttributes - 1))
        {
            if (WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_AND == m_attributeConcatenationOperatorsMap[(m_attributes[i])])
            {
                sqlString += " AND ";
            }
            else if (WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR == m_attributeConcatenationOperatorsMap[(m_attributes[i])])
            {
                sqlString += " OR ";
            }
            else
            {
                // No other concatenation operators supported
                trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::WaveConditionSet::getSql : Unsupported concatenation operator defined.");
                prismAssert (false, __FILE__, __LINE__);
            }   
        }
    }

    sqlString += ")";

    return (sqlString);
}


WaveConditionSetBuilder::WaveConditionSetBuilder ()
    : m_conditionSetCombinationSql ("")
{
}

WaveConditionSetBuilder::~WaveConditionSetBuilder ()
{
    map<string, WaveConditionSet *>::iterator   element;

    for (element = m_conditionSets.begin (); element != m_conditionSets.end (); element ++)
    {
        delete (element->second);
        element->second = NULL;
    }

    m_conditionSets.clear ();
}

bool WaveConditionSetBuilder::isConditionSetFound (const string &conditionSetName)
{
    map<string, WaveConditionSet *>::iterator   element = m_conditionSets.find (conditionSetName);
    map<string, WaveConditionSet *>::iterator   end     = m_conditionSets.end ();
    bool                                        result  = false;

    if (element == end)
    {
        result = false;
    }    
    else
    {
        result = true;
    }

    return (result);
}

bool WaveConditionSetBuilder::isConditionSetCombinationSyntaxCorrect (const string &conditionSetCombination)
{
    string::const_iterator  element;
    bool                    result                  = true;
    SI32                    unmatchedParenthesis    = 0;

    for (element = conditionSetCombination.begin (); element != conditionSetCombination.end (); element++) 
    {
        if ('(' == *element)
        {
            ++unmatchedParenthesis;
        } 
        else if (')' == *element)
        {
            --unmatchedParenthesis;
        }
    }

    if (0 != unmatchedParenthesis)
    {
        result = false;
    }
    else
    {
        result = true;
    }

    return (result);
}

string WaveConditionSetBuilder::getSql () const
{
    if ("" == m_conditionSetCombinationSql)
    {
        trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::getSql : A condition set combination has not been defined.  Please define a condition set combination.");       
        prismAssert (false, __FILE__, __LINE__);
    }

    trace (TRACE_LEVEL_DEBUG, string ("WaveConditionSetBuilder::getSql : Combined SQL WHERE Clause: \"") + m_conditionSetCombinationSql + "\"");

    return (m_conditionSetCombinationSql);
}

void WaveConditionSetBuilder::createConditionSet (const string &conditionSetName)
{
    bool    conditionSetFound     = isConditionSetFound (conditionSetName);
    
    if (true == conditionSetFound)
    {
        trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::createConditionSet : Condition set \"" + conditionSetName + "\" is already created!  Condition set names need to be unique.");       
        prismAssert (false, __FILE__, __LINE__);
    }

    WaveConditionSet *pWaveConditionSet = new WaveConditionSet (conditionSetName);

    m_conditionSets[conditionSetName] = pWaveConditionSet;
}

void WaveConditionSetBuilder::addAttributeToConditionSet (const string &conditionSetName, Attribute *pAttribute, AttributeConcatenationOperator concatenationOperation, AttributeConditionOperator conditionOperation)
{
    bool    conditionSetFound     = isConditionSetFound (conditionSetName);

    if (false == conditionSetFound)
    {
        trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::addAttributeToConditionSet : Condition set \"" + conditionSetName + "\" is not found!  Please create the condition set before adding any attributes to the condition set.");     
        prismAssert (false, __FILE__, __LINE__);
    }

    m_conditionSets[conditionSetName]->addAttributeToConditionSet (pAttribute, concatenationOperation, conditionOperation);
}

void WaveConditionSetBuilder::combineConditionSets (const string &conditionSetCombination)
{
    map<string, WaveConditionSet *>::iterator   element;
    string                                      conditionSetCombinationSql = conditionSetCombination;

    // verify combination set syntax

    if (false == isConditionSetCombinationSyntaxCorrect (conditionSetCombination))
    {
        trace (TRACE_LEVEL_FATAL, "WaveConditionSetBuilder::combineConditionSets : Condition set combination \"" + conditionSetCombination + "\" has syntax errors!  Please verify the condition set combination syntax.");
        prismAssert (false, __FILE__, __LINE__);
    }
    
    // Substitute combination set names for SQL equivalent

    for (element = m_conditionSets.begin (); element != m_conditionSets.end (); element++)
    {
        trace (TRACE_LEVEL_DEVEL, "WaveConditionSetBuilder::combineConditionSets : Replacing \"" + element->first + "\" with \"" + element->second->getSql () + "\"");
        StringUtils::replaceAllInstancesOfInputStringWith (conditionSetCombinationSql, element->first, element->second->getSql ());
    }

    // Substitute combination set concatenation operators for SQL equivalent

    StringUtils::replaceAllInstancesOfInputStringWith (conditionSetCombinationSql, "*", " AND ");
    StringUtils::replaceAllInstancesOfInputStringWith (conditionSetCombinationSql, "+", " OR ");

    m_conditionSetCombinationSql = string (" (") + conditionSetCombinationSql + string (") ");
}

}
