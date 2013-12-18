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
 *   Copyright (C) 2005 - 2010 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Attributes/AttributeSI64Vector.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/Integer.h"
#include "Framework/Utils/BigInteger.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/OrmColumn.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmAssociation.h"
#include "Framework/ObjectRelationalMapping/OrmAggregation.h"
#include "Framework/ObjectRelationalMapping/OrmComposition.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"

#include <algorithm>

namespace WaveNs
{

AttributeSI64Vector::AttributeSI64Vector (const vector<SI64> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<SI64>;
    *m_pData = data;
    setIsMemoryOwnedByAttribute (true);
}

AttributeSI64Vector::AttributeSI64Vector (const vector<SI64> &data, const vector<SI64> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<SI64>;
    *m_pData = data;
    setIsMemoryOwnedByAttribute (true);
}

AttributeSI64Vector::AttributeSI64Vector (vector<SI64> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI64Vector::AttributeSI64Vector (vector<SI64> *pData, const vector<SI64> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI64Vector::AttributeSI64Vector (const AttributeSI64Vector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<SI64>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI64Vector::~AttributeSI64Vector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSI64Vector &AttributeSI64Vector::operator = (const AttributeSI64Vector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

vector<SI64> AttributeSI64Vector::getValue () const
{
    vector<SI64> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI64Vector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeSI64Vector::setValue (const vector<SI64> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI64Vector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeSI64Vector::validate () const
{
    prismAssert (AttributeType::AttributeTypeSI64Vector == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeSI64Vector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeSI64Vector::getSqlType ()
{
    return ("varchar");
}

void AttributeSI64Vector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSI64Vector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeSI64Vector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeSI64Vector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeSI64Vector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSI64Vector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeSI64Vector::toString (string &valueString)
{
    vector<SI64>::iterator element = (*m_pData).begin ();
    vector<SI64>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString += string ("#") + (BigInteger (0xFFFFFFFFFFFFFFFFLLU & (*element)).toString ());
        element++;
    }
}

void AttributeSI64Vector::fromString (const string &valueString)
{
    vector<SI64>::iterator begin = (*m_pData).begin ();
    vector<SI64>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    SI64 value          = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        value = strtoull (valueString.c_str () + firstIndex + 1, NULL, 10);
        (*m_pData).push_back (value);

        startFromIndex = firstIndex + 1;
    }
}

void *AttributeSI64Vector::getPData ()
{
    return (m_pData);
}

void AttributeSI64Vector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<SI64> *> (pData));
}

Attribute *AttributeSI64Vector::clone ()
{
    AttributeSI64Vector *pAttributeSI64Vector = new AttributeSI64Vector (*this);

    return (pAttributeSI64Vector);
}

map<string, string> AttributeSI64Vector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_SI32_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_UI64_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_UI32_VECTOR";
 
    return (supportedConversions);
}

bool AttributeSI64Vector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultValue() == getValue() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeSI64Vector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<SI64> AttributeSI64Vector::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeSI64Vector::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        (*m_pData).clear();
    }
}

void AttributeSI64Vector::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_si64_vector (pCValue, getValue ());
}

void AttributeSI64Vector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<SI64>;        
    }
 
    vector<SI64> vectorToAppend = ( dynamic_cast<AttributeSI64Vector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ()) ;
}

void AttributeSI64Vector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<SI64>::iterator iterOriginal = (*m_pData).begin () ;
    
    vector<SI64> vectorToDelete = (dynamic_cast<AttributeSI64Vector *> (attribute))->getValue ();
    vector<SI64>::iterator iterDelete = vectorToDelete.begin () ;

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeSI64Vector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

}

