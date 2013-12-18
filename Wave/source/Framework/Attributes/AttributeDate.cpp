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
 *   Author : Himanshu Varshney, Amit Agrawal                              *
 ***************************************************************************/

#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeDate.h"
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

AttributeDate::AttributeDate (const Date &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDate, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new Date;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeDate::AttributeDate (const Date &data, const Date &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDate, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new Date;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (true);
}

AttributeDate::AttributeDate (Date *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDate, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDate::AttributeDate (Date *pData, const Date &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDate, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDate::AttributeDate (const AttributeDate &attribute)
    : Attribute (attribute)
{
    m_pData              = new Date;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeDate::~AttributeDate ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeDate &AttributeDate::operator = (const AttributeDate &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

Date AttributeDate::getValue () const
{
    Date tempDate;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDate::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempDate);
    }
}

void AttributeDate::getAllValues (SI16 *year, UI8 *month, UI8 *day, SI8 *timezone, SI8 *timezone_minutes)
{
    if (true == (validate ()))
    {
        *year = m_pData->getYear();
        *month = m_pData->getMonth();
        *day = m_pData->getDay();
        *timezone = m_pData->getTimezoneHours();
        *timezone_minutes = m_pData->getTimezoneMinutes();
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDate::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

void AttributeDate::setValue (const Date &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDate::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeDate::validate () const
{
    prismAssert (AttributeType::AttributeTypeDate == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeDate == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeDate::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL                     == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL                 == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN              == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO  == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN                 == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN_OR_EQUAL_TO     == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE                      == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE                  == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE                     == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE                 == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeDate::getSqlType ()
{
    return ("date");
}

void AttributeDate::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeDate::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeDate::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeDate::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeDate::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeDate::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeDate::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeDate::fromString (const string &valueString)
{
    //trace (TRACE_LEVEL_INFO, string("AttributeDate::fromString string arg:") + valueString);
    m_pData->fromString (valueString);
}

ResourceId AttributeDate::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeDate::getPData ()
{
    return (m_pData);
}

void AttributeDate::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const Date *> (pData));
}

Attribute *AttributeDate::clone ()
{
    AttributeDate *pAttributeDate = new AttributeDate (*this);

    return (pAttributeDate);
}
      
map<string, string> AttributeDate::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATE_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_DATE_VECTOR";

    return (supportedConversions);
}

bool AttributeDate::isCurrentValueSameAsDefault () const
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

bool AttributeDate::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

Date AttributeDate::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeDate::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        Date tempDate;
        setValue(&tempDate);
    }
}

void AttributeDate::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_date(pCValue, getValue ());
}

void AttributeDate::getPlainString (string &plainString)
{
    plainString = m_pData->getPlainString ();
}

AttributeDateVector::AttributeDateVector (const vector<Date> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDateVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<Date>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeDateVector::AttributeDateVector (const vector<Date> &data, const vector<Date> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDateVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<Date>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeDateVector::AttributeDateVector (vector<Date> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDateVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDateVector::AttributeDateVector (vector<Date> *pData, const vector<Date> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeDateVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDateVector::AttributeDateVector (const AttributeDateVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<Date>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeDateVector::~AttributeDateVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeDateVector &AttributeDateVector::operator = (const AttributeDateVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

vector<Date> AttributeDateVector::getValue () const
{
    vector<Date> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeDateVector::setValue (const vector<Date> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeDateVector::validate () const
{
    prismAssert (AttributeType::AttributeTypeDateVector == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeDateVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeDateVector::getSqlType ()
{
    return ("varchar");
}

void AttributeDateVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeDateVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeDateVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeDateVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeDateVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeDateVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () +  FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeDateVector::toString (string &valueString)
{
    vector<Date>::iterator element = (*m_pData).begin ();
    vector<Date>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeDateVector::fromString (const string &valueString)
{
    vector<Date>::iterator begin = (*m_pData).begin ();
    vector<Date>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI32 stringSize     = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        stringSize = atoi (valueString.c_str () + startFromIndex);

        (*m_pData).push_back (valueString.substr (firstIndex + 1, stringSize));

        startFromIndex = firstIndex + stringSize + 1;
    }
}

void *AttributeDateVector::getPData ()
{
    return (m_pData);
}

void AttributeDateVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<Date> *> (pData));
}

Attribute *AttributeDateVector::clone ()
{
    AttributeDateVector *pAttributeDateVector = new AttributeDateVector (*this);

    return (pAttributeDateVector);
}

map<string, string> AttributeDateVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeDateVector::isCurrentValueSameAsDefault () const
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

bool AttributeDateVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<Date> AttributeDateVector::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeDateVector::setDefaultValue()
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

void AttributeDateVector::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_date_vector (pCValue, getValue ());
}

void AttributeDateVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<Date>;        
    }
 
    vector<Date> vectorToAppend =  (dynamic_cast<AttributeDateVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeDateVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<Date>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<Date> vectorToDelete = (dynamic_cast<AttributeDateVector *> (attribute))->getValue ();
    vector<Date>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeDateVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

                                                                                                                                  
}
