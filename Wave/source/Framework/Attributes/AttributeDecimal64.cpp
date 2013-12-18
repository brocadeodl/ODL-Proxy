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
 *   Copyright (C) 2005 - 2008 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeDecimal64.h"
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
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"

#include <algorithm>

namespace WaveNs
{

AttributeDecimal64::AttributeDecimal64 (const Decimal64 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeDecimal64, attributeName, attributeUserTag, isOperational)
{
    m_pData                   = new Decimal64;
    *m_pData                  = data;
    m_isDefaultDecimal64Valid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeDecimal64::AttributeDecimal64 (const Decimal64 &data, const Decimal64 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeDecimal64, attributeName, attributeUserTag, isOperational)
{
    m_pData                   = new Decimal64;
    *m_pData                  = data;
    m_defaultDecimal64        = defaultData;
    m_isDefaultDecimal64Valid = true;
    setIsMemoryOwnedByAttribute (true);
}

AttributeDecimal64::AttributeDecimal64 (Decimal64 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeDecimal64, attributeName, attributeUserTag, isOperational)
{
    m_pData                   = pData;
    m_isDefaultDecimal64Valid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDecimal64::AttributeDecimal64 (Decimal64 *pData, const Decimal64 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeDecimal64, attributeName, attributeUserTag, isOperational)
{
    m_pData                   = pData;
    m_defaultDecimal64        = defaultData;
    m_isDefaultDecimal64Valid = true;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDecimal64::AttributeDecimal64 (const AttributeDecimal64 &attribute)
    : Attribute (attribute)
{
    m_pData                   = new Decimal64;
    *m_pData                  = attribute.getValue ();
    m_defaultDecimal64        = attribute.getDefaultDecimal64();
    m_isDefaultDecimal64Valid = attribute.getIsDefaultDecimal64ValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeDecimal64::~AttributeDecimal64 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeDecimal64 &AttributeDecimal64::operator = (const AttributeDecimal64 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultDecimal64        = attribute.getDefaultDecimal64();
    m_isDefaultDecimal64Valid = attribute.getIsDefaultDecimal64ValidFlag();

    return (*this);
}

Decimal64 AttributeDecimal64::getValue () const
{
    Decimal64 tempDecimal64;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDecimal64::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempDecimal64);
    }
}

void AttributeDecimal64::getAllValues (SI64 *val, UI8 *fractiondigits)
{
    if (true == (validate ()))
    {
        *val = m_pData->getVal();
        *fractiondigits = m_pData->getFractionDigits();
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDecimal64::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

void AttributeDecimal64::setValue (const Decimal64 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDecimal64::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeDecimal64::validate () const
{
    prismAssert ( AttributeType::AttributeTypeDecimal64 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeDecimal64 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeDecimal64::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL                     == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL                 == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN              == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO  == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN                 == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN_OR_EQUAL_TO     == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeDecimal64::getSqlType ()
{
    return ("numeric");
}

void AttributeDecimal64::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeDecimal64::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeDecimal64::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    //sqlForInsert += string (",'") + tempString + string ("'");
    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeDecimal64::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    //sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeDecimal64::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeDecimal64::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    //sqlForSelect += getAttributeName () + " = " + string ("'") + tempString + string ("'");
    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeDecimal64::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeDecimal64::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeDecimal64::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeDecimal64::getPData ()
{
    return (m_pData);
}

void AttributeDecimal64::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const Decimal64 *> (pData));
}

Attribute *AttributeDecimal64::clone ()
{
    AttributeDecimal64 *pAttributeDecimal64 = new AttributeDecimal64 (*this);

    return (pAttributeDecimal64);
}

map<string, string> AttributeDecimal64::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeDecimal64::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDecimal64ValidFlag() )
    {   
        if ( getDefaultDecimal64() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeDecimal64::getIsDefaultDecimal64ValidFlag() const
{
    return (m_isDefaultDecimal64Valid);
}

Decimal64 AttributeDecimal64::getDefaultDecimal64() const
{
    return (m_defaultDecimal64);
}

void AttributeDecimal64::setDefaultValue()
{
    if(true == m_isDefaultDecimal64Valid)
    {
        setValue(&m_defaultDecimal64);
    }
    else
    {
        Decimal64 tempData;
        setValue(&tempData);
    }
}
void AttributeDecimal64::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_decimal_64 (pCValue, getValue ());
}

AttributeDecimal64Vector::AttributeDecimal64Vector (const vector<Decimal64> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                     ( AttributeType::AttributeTypeDecimal64Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDecimal64VectorValid (false)
{
    m_pData  = new vector<Decimal64>;
    *m_pData = data;
}

AttributeDecimal64Vector::AttributeDecimal64Vector (const vector<Decimal64> &data, const vector<Decimal64> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                     ( AttributeType::AttributeTypeDecimal64Vector, attributeName, attributeUserTag, isOperational),
    m_defaultDecimal64Vector        (defaultData),
    m_isDefaultDecimal64VectorValid (true)
{
    m_pData  = new vector<Decimal64>;
    *m_pData = data;
}

AttributeDecimal64Vector::AttributeDecimal64Vector (vector<Decimal64> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                     ( AttributeType::AttributeTypeDecimal64Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDecimal64VectorValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDecimal64Vector::AttributeDecimal64Vector (vector<Decimal64> *pData, const vector<Decimal64> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                     ( AttributeType::AttributeTypeDecimal64Vector, attributeName, attributeUserTag, isOperational),
    m_defaultDecimal64Vector        (defaultData),
    m_isDefaultDecimal64VectorValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDecimal64Vector::AttributeDecimal64Vector (const AttributeDecimal64Vector &attribute)
    : Attribute (attribute)
{
    m_pData                         = new vector<Decimal64>;
    *m_pData                        = attribute.getValue ();
    m_defaultDecimal64Vector        = attribute.getDefaultDecimal64Vector();
    m_isDefaultDecimal64VectorValid = attribute.getIsDefaultDecimal64VectorValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeDecimal64Vector::~AttributeDecimal64Vector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeDecimal64Vector &AttributeDecimal64Vector::operator = (const AttributeDecimal64Vector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultDecimal64Vector        = attribute.getDefaultDecimal64Vector();
    m_isDefaultDecimal64VectorValid = attribute.getIsDefaultDecimal64VectorValidFlag();

    return (*this);
}

vector<Decimal64> AttributeDecimal64Vector::getValue () const
{
    vector<Decimal64> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDecimal64Vector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeDecimal64Vector::setValue (const vector<Decimal64> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDecimal64Vector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeDecimal64Vector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeDecimal64Vector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeDecimal64Vector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeDecimal64Vector::getSqlType ()
{
    return ("varchar");
}

void AttributeDecimal64Vector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeDecimal64Vector::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeDecimal64Vector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeDecimal64Vector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeDecimal64Vector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeDecimal64Vector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeDecimal64Vector::toString (string &valueString)
{
    vector<Decimal64>::iterator element = (*m_pData).begin ();
    vector<Decimal64>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeDecimal64Vector::fromString (const string &valueString)
{
    vector<Decimal64>::iterator begin = (*m_pData).begin ();
    vector<Decimal64>::iterator end   = (*m_pData).end ();
    Decimal64                   tmpDecimal64;

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI32 stringSize     = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        stringSize = atoi (valueString.c_str () + startFromIndex);

        tmpDecimal64 = valueString.substr (firstIndex + 1, stringSize);
        (*m_pData).push_back (tmpDecimal64);

        startFromIndex = firstIndex + stringSize + 1;
    }
}

void *AttributeDecimal64Vector::getPData ()
{
    return (m_pData);
}

void AttributeDecimal64Vector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<Decimal64> *> (pData));
}

Attribute *AttributeDecimal64Vector::clone ()
{
    AttributeDecimal64Vector *pAttributeDecimal64Vector = new AttributeDecimal64Vector (*this);

    return (pAttributeDecimal64Vector);
}

map<string, string> AttributeDecimal64Vector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeDecimal64Vector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDecimal64VectorValidFlag() )
    {   
        if ( getDefaultDecimal64Vector() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeDecimal64Vector::getIsDefaultDecimal64VectorValidFlag() const
{
    return (m_isDefaultDecimal64VectorValid);
}

vector<Decimal64> AttributeDecimal64Vector::getDefaultDecimal64Vector() const
{
    return (m_defaultDecimal64Vector);
}

void AttributeDecimal64Vector::setDefaultValue()
{
   if (true == m_isDefaultDecimal64VectorValid)
   {
       setValue(&m_defaultDecimal64Vector);
   }
   else
   {
       (*m_pData).clear();
   }
}

void AttributeDecimal64Vector::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_decimal_64_vector (pCValue, getValue ());
}

void AttributeDecimal64Vector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<Decimal64>;        
    }
 
    vector<Decimal64> vectorToAppend = (dynamic_cast<AttributeDecimal64Vector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeDecimal64Vector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<Decimal64>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<Decimal64> vectorToDelete = (dynamic_cast<AttributeDecimal64Vector *> (attribute))->getValue ();
    vector<Decimal64>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeDecimal64Vector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}
                                                                                                                                  
}
