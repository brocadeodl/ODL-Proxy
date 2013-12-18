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
#include "Framework/Attributes/AttributeTime.h"
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

AttributeTime::AttributeTime (const Time &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new Time;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeTime::AttributeTime (const Time &data, const Time &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new Time;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

AttributeTime::AttributeTime (Time *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeTime::AttributeTime (Time *pData, const Time &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeTime, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (false);
}

AttributeTime::AttributeTime (const AttributeTime &attribute)
    : Attribute (attribute)
{
    m_pData              = new Time;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultValueValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeTime::~AttributeTime ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeTime &AttributeTime::operator = (const AttributeTime &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultValueValidFlag();

    return (*this);
}

Time AttributeTime::getValue () const
{
    Time tempTime;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeTime::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempTime);
    }
}

void AttributeTime::getAllValues (UI8 *hour, UI8 *min, UI8 *sec, UI32 *micro, SI8 *timezonehours, SI8 *timezoneminutes)
{
    if (true == (validate ()))
    {
        *hour = m_pData->getHour();
        *min = m_pData->getMin();
        *sec = m_pData->getSec();
        *micro = m_pData->getMicro();
        //*timezonehours = m_pData->getTimezoneHours();
        //*timezoneminutes = m_pData->getTimezoneMinutes();
        *timezonehours = 0;
        *timezoneminutes = 0;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeTime::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

void AttributeTime::setValue (const Time &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeTime::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeTime::validate () const
{
    prismAssert ( AttributeType::AttributeTypeTime == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeTime == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeTime::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeTime::getSqlType ()
{
    //return ("time 6 with time zone");
    return ("time without time zone");
}

void AttributeTime::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeTime::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeTime::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeTime::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeTime::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeTime::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeTime::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeTime::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void *AttributeTime::getPData ()
{
    return (m_pData);
}

void AttributeTime::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const Time *> (pData));
}

Attribute *AttributeTime::clone ()
{
    AttributeTime *pAttributeTime = new AttributeTime (*this);

    return (pAttributeTime);
}

map<string, string> AttributeTime::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_TIME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_TIME_VECTOR";
    
    return (supportedConversions);
}

bool AttributeTime::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultValueValidFlag() )
    {   
        if ( getDefaultValue() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeTime::getIsDefaultValueValidFlag() const
{
    return (m_isDefaultDataValid);
}

Time AttributeTime::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeTime::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        Time tempTime;
        setValue(&tempTime);
    }
}

void AttributeTime::getCValue(WaveCValue *pCValue)
{
	wave_cvalue_set_time(pCValue,*m_pData);
}

void AttributeTime::getPlainString (string &plainString)
{
    plainString = m_pData->getPlainString ();
}

AttributeTimeVector::AttributeTimeVector (const vector<Time> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeTimeVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<Time>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeTimeVector::AttributeTimeVector (const vector<Time> &data, const vector<Time> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeTimeVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<Time>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeTimeVector::AttributeTimeVector (vector<Time> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeTimeVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeTimeVector::AttributeTimeVector (vector<Time> *pData, const vector<Time> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeTimeVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeTimeVector::AttributeTimeVector (const AttributeTimeVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<Time>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeTimeVector::~AttributeTimeVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeTimeVector &AttributeTimeVector::operator = (const AttributeTimeVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<Time> AttributeTimeVector::getValue () const
{
    vector<Time> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeTimeVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeTimeVector::setValue (const vector<Time> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeTimeVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeTimeVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeTimeVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeTimeVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeTimeVector::getSqlType ()
{
    return ("varchar");
}

void AttributeTimeVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeTimeVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeTimeVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeTimeVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeTimeVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeTimeVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeTimeVector::toString (string &valueString)
{
    vector<Time>::iterator element = (*m_pData).begin ();
    vector<Time>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeTimeVector::fromString (const string &valueString)
{
    vector<Time>::iterator begin = (*m_pData).begin ();
    vector<Time>::iterator end   = (*m_pData).end ();

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

void *AttributeTimeVector::getPData ()
{
    return (m_pData);
}

void AttributeTimeVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<Time> *> (pData));
}

Attribute *AttributeTimeVector::clone ()
{
    AttributeTimeVector *pAttributeTimeVector = new AttributeTimeVector (*this);

    return (pAttributeTimeVector);
}

map<string, string> AttributeTimeVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

bool AttributeTimeVector::isCurrentValueSameAsDefault () const
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

bool AttributeTimeVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<Time> AttributeTimeVector::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeTimeVector::setDefaultValue()
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

void AttributeTimeVector::getCValue(WaveCValue *pCValue)
{
    wave_cvalue_set_time_vector (pCValue, getValue ());
}

void AttributeTimeVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<Time>;        
    }
    
    vector<Time> vectorToAppend = (dynamic_cast<AttributeTimeVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
  
}

void AttributeTimeVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<Time>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<Time> vectorToDelete = (dynamic_cast<AttributeTimeVector *> (attribute))->getValue ();
    vector<Time>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeTimeVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

}
