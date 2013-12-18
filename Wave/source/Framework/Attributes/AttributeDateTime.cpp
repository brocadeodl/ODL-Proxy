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
#include "Framework/Attributes/AttributeDateTime.h"
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

AttributeDateTime::AttributeDateTime (const DateTime &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new DateTime;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeDateTime::AttributeDateTime (const DateTime &data, const DateTime &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new DateTime;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (true);
}

AttributeDateTime::AttributeDateTime (DateTime *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDateTime::AttributeDateTime (DateTime *pData, const DateTime &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDateTime::AttributeDateTime (const AttributeDateTime &attribute)
    : Attribute (attribute)
{
    m_pData              = new DateTime;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeDateTime::~AttributeDateTime ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }

}

AttributeDateTime &AttributeDateTime::operator = (const AttributeDateTime &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

DateTime AttributeDateTime::getValue () const
{
    DateTime tempDateTime;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateTime::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempDateTime);
    }
}

void AttributeDateTime::setValue (const DateTime &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateTime::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

void AttributeDateTime::getAllValues (SI16 *year, UI8 *month, UI8 *day, UI8 *hour, UI8 *min, UI8 *sec, UI32 *micro, SI8 *timezone, SI8 *timezone_minutes)
{
    if (true == (validate ()))
    {
        *month = m_pData->getMonth();
        *day = m_pData->getDay();
        *year = m_pData->getYear();
        *hour = m_pData->getHour();
        *min = m_pData->getMin();
        *sec = m_pData->getSec();
        *micro = m_pData->getMicro();
        //*timezone = m_pData->getTimezoneHours();
        *timezone = 0;
        //*timezone_minutes = m_pData->getTimezoneMinutes();
        *timezone_minutes = 0; // as the timestamp format from postgresql adjusts this in the time field
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateTimeVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeDateTime::validate () const
{
    prismAssert ( AttributeType::AttributeTypeDateTime == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeDateTime == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeDateTime::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeDateTime::getSqlType ()
{
    //return ("timestamp 6 with time zone");
    return ("timestamp without time zone");
}

void AttributeDateTime::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeDateTime::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeDateTime::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeDateTime::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    if (true == isFirst)
    {
        sqlForUpdate += getAttributeName () + " = " + string ("'") + tempString + string ("'");
    }
    else 
    {
        sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
    }
}

void AttributeDateTime::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeDateTime::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeDateTime::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeDateTime::fromString (const string &valueString)
{
    //trace (TRACE_LEVEL_INFO, string("AttributeDateTime::fromString cli arg:") + valueString);
    m_pData->fromString (valueString);
}

ResourceId AttributeDateTime::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeDateTime::getPData ()
{
    return (m_pData);
}

void AttributeDateTime::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const DateTime *> (pData));
}

Attribute *AttributeDateTime::clone ()
{
    AttributeDateTime *pAttributeDateTime = new AttributeDateTime (*this);

    return (pAttributeDateTime);
}

map<string, string> AttributeDateTime::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeDateTime::isCurrentValueSameAsDefault () const
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

bool AttributeDateTime::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

DateTime AttributeDateTime::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeDateTime::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        DateTime tempData;
        setValue(&tempData);
    }
}


void AttributeDateTime::getCValue (WaveCValue *pCValue)
{
  wave_cvalue_set_date_time ( pCValue, getValue ());
}

void AttributeDateTime::getPlainString (string &plainString)
{
    plainString = m_pData->getPlainString ();
}

AttributeDateTimeVector::AttributeDateTimeVector (const vector<DateTime> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTimeVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<DateTime>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeDateTimeVector::AttributeDateTimeVector (const vector<DateTime> &data, const vector<DateTime> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTimeVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<DateTime>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeDateTimeVector::AttributeDateTimeVector (vector<DateTime> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTimeVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDateTimeVector::AttributeDateTimeVector (vector<DateTime> *pData, const vector<DateTime> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeDateTimeVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeDateTimeVector::AttributeDateTimeVector (const AttributeDateTimeVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<DateTime>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeDateTimeVector::~AttributeDateTimeVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeDateTimeVector &AttributeDateTimeVector::operator = (const AttributeDateTimeVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<DateTime> AttributeDateTimeVector::getValue () const
{
    vector<DateTime> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateTimeVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeDateTimeVector::setValue (const vector<DateTime> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeDateTimeVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeDateTimeVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeDateTimeVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeDateTimeVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeDateTimeVector::getSqlType ()
{
    return ("varchar");
}

void AttributeDateTimeVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeDateTimeVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeDateTimeVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeDateTimeVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeDateTimeVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeDateTimeVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeDateTimeVector::toString (string &valueString)
{
    vector<DateTime>::iterator element = (*m_pData).begin ();
    vector<DateTime>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeDateTimeVector::fromString (const string &valueString)
{
    vector<DateTime>::iterator begin = (*m_pData).begin ();
    vector<DateTime>::iterator end   = (*m_pData).end ();

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

void *AttributeDateTimeVector::getPData ()
{
    return (m_pData);
}

void AttributeDateTimeVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<DateTime> *> (pData));
}

Attribute *AttributeDateTimeVector::clone ()
{
    AttributeDateTimeVector *pAttributeDateTimeVector = new AttributeDateTimeVector (*this);

    return (pAttributeDateTimeVector);
}

map<string, string> AttributeDateTimeVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeDateTimeVector::isCurrentValueSameAsDefault () const
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

bool AttributeDateTimeVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<DateTime> AttributeDateTimeVector::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeDateTimeVector::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        (*m_pData).clear();
    }
}

void AttributeDateTimeVector::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_date_time_vector ( pCValue, getValue ());
}


void AttributeDateTimeVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<DateTime>;        
    }

    vector<DateTime> vectorToAppend = (dynamic_cast<AttributeDateTimeVector *> (attribute))->getValue ();

    (*m_pData).insert((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}


void AttributeDateTimeVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<DateTime>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<DateTime> vectorToDelete = (dynamic_cast<AttributeDateTimeVector *> (attribute))->getValue ();
    vector<DateTime>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeDateTimeVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

}
