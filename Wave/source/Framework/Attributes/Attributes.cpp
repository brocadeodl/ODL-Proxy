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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Attributes/Attributes.h"
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
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"

#include <algorithm>

namespace WaveNs
{

AttributeUI32::AttributeUI32 (const UI32 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
     m_pData = new UI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32::AttributeUI32 (const UI32 &data, const UI32 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
     m_pData = new UI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32::AttributeUI32 (UI32 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32::AttributeUI32 (UI32 *pData, const UI32 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32::AttributeUI32 (const AttributeUI32 &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI32;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32::~AttributeUI32 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI32 &AttributeUI32::operator = (const AttributeUI32 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI32 AttributeUI32::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeUI32::setValue (const UI32 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI32::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUI32 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUI32 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUI32::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeUI32::getSqlType ()
{
    return ("integer");
}

void AttributeUI32::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI32::getSqlForCreate ()
{
    return (getAttributeName () + " integer");
}

void AttributeUI32::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString = (Integer (0xFFFFFFFF & (getValue ()))).toString ();

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI32::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString = (Integer (0xFFFFFFFF & (getValue ()))).toString ();

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeUI32::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI32::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString = (Integer (0xFFFFFFFF & (getValue ()))).toString ();

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeUI32::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%lu", (long unsigned int) (getValue ()));

    valueString = string (buffer);
}

void AttributeUI32::fromString (const string &valueString)
{
    setValue (strtoul (valueString.c_str (), NULL, 10));
}

ResourceId AttributeUI32::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    UI32 value = strtoul (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeUI32::loadFromPlainString : Improper format of AttributeUI32  string"));
    }

    return status ;
}

void *AttributeUI32::getPData ()
{
    return (m_pData);
}

void AttributeUI32::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI32 *> (pData));
}

Attribute *AttributeUI32::clone ()
{
    AttributeUI32 *pAttributeUI32 = new AttributeUI32 (*this);

    return (pAttributeUI32);
}

bool AttributeUI32::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

UI32 AttributeUI32::getDefaultData (void) const
{
    return (m_defaultData);
}

bool AttributeUI32::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

map<string, string> AttributeUI32::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"]= "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] ="WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] ="WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";

    return (supportedConversions);
}

void AttributeUI32::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

void AttributeUI32::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ui32 (pCValue, getValue ());  
}

AttributeUI32Vector::AttributeUI32Vector (const vector<UI32> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<UI32>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32Vector::AttributeUI32Vector (const vector<UI32> &data, const vector<UI32> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<UI32>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32Vector::AttributeUI32Vector (vector<UI32> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32Vector::AttributeUI32Vector (vector<UI32> *pData, const vector<UI32> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32Vector::AttributeUI32Vector (const AttributeUI32Vector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<UI32>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32Vector::~AttributeUI32Vector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI32Vector &AttributeUI32Vector::operator = (const AttributeUI32Vector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<UI32> AttributeUI32Vector::getValue () const
{
    vector<UI32> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32Vector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeUI32Vector::setValue (const vector<UI32> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32Vector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI32Vector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUI32Vector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUI32Vector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUI32Vector::getSqlType ()
{
    return ("varchar");
}

void AttributeUI32Vector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI32Vector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUI32Vector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI32Vector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI32Vector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI32Vector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI32Vector::toString (string &valueString)
{
    vector<UI32>::iterator element = (*m_pData).begin ();
    vector<UI32>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString += string ("#") + (*element);
        element++;
    }
}

void AttributeUI32Vector::fromString (const string &valueString)
{
    vector<UI32>::iterator begin = (*m_pData).begin ();
    vector<UI32>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI32 value          = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        value = strtoul ((valueString.c_str () + firstIndex + 1), NULL, 10);
        (*m_pData).push_back (value);

        startFromIndex = firstIndex + 1;
    }
}


void *AttributeUI32Vector::getPData ()
{
    return (m_pData);
}

void AttributeUI32Vector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<UI32> *> (pData));
}

Attribute *AttributeUI32Vector::clone ()
{
    AttributeUI32Vector *pAttributeUI32Vector = new AttributeUI32Vector (*this);

    return (pAttributeUI32Vector);
}

map<string, string> AttributeUI32Vector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_UI64_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_SI64_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_SI32_VECTOR";

    return (supportedConversions);
}

bool AttributeUI32Vector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<UI32> AttributeUI32Vector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeUI32Vector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeUI32Vector::setDefaultValue()
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
void AttributeUI32Vector::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ui32_vector (pCValue, getValue ());  
}

void AttributeUI32Vector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<UI32>;        
    }
    
    vector<UI32> vectorToAppend = (dynamic_cast<AttributeUI32Vector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeUI32Vector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<UI32>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<UI32> vectorToDelete = (dynamic_cast<AttributeUI32Vector *> (attribute))->getValue ();
    vector<UI32>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeUI32Vector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

AttributeUI64::AttributeUI64 (const UI64 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute             ( AttributeType::AttributeTypeUI64, attributeName, attributeUserTag, isOperational),
      m_defaultData         (0),
      m_isDefaultDataValid  (false)
{
    m_pData  = new UI64;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64::AttributeUI64 (const UI64 &data, const UI64 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute             ( AttributeType::AttributeTypeUI64, attributeName, attributeUserTag, isOperational),
      m_defaultData         (defaultData),
      m_isDefaultDataValid  (true)
{
    m_pData  = new UI64;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64::AttributeUI64 (UI64 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute             ( AttributeType::AttributeTypeUI64, attributeName, attributeUserTag, isOperational),
      m_defaultData         (0),
      m_isDefaultDataValid  (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI64::AttributeUI64 (UI64 *pData, const UI64 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute             ( AttributeType::AttributeTypeUI64, attributeName, attributeUserTag, isOperational),
      m_defaultData         (defaultData),
      m_isDefaultDataValid  (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI64::AttributeUI64 (const AttributeUI64 &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI64;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64::~AttributeUI64 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI64 &AttributeUI64::operator = (const AttributeUI64 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI64 AttributeUI64::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI64::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeUI64::setValue (const UI64 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI64::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI64::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUI64 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUI64 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUI64::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeUI64::getSqlType ()
{
    return ("bigint");
}

void AttributeUI64::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI64::getSqlForCreate ()
{
    return (getAttributeName () + " bigint");
}

void AttributeUI64::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString = (BigInteger (0xFFFFFFFFFFFFFFFFLLU & (getValue ()))).toString ();

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI64::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString = (BigInteger (0xFFFFFFFFFFFFFFFFLLU & (getValue ()))).toString ();

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeUI64::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI64::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString = (BigInteger (0xFFFFFFFFFFFFFFFFLLU & (getValue ()))).toString ();

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeUI64::toString (string &valueString)
{
    char buffer[128] = {0};

    snprintf (buffer, 128, "%llu", (long long unsigned int) (getValue ()));

    valueString = string (buffer);
}

void AttributeUI64::fromString (const string &valueString)
{
    setValue (strtoull (valueString.c_str (), NULL, 10));
}


ResourceId AttributeUI64::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    UI64 value = strtoull (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeUI64::loadFromPlainString : Improper format of AttributeUI64 string"));
    }

    return status ;
}

void *AttributeUI64::getPData ()
{
    return (m_pData);
}

void AttributeUI64::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI64 *> (pData));
}

Attribute *AttributeUI64::clone ()
{
    AttributeUI64 *pAttributeUI64 = new AttributeUI64 (*this);

    return (pAttributeUI64);
}

bool AttributeUI64::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;
    
    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

UI64 AttributeUI64::getDefaultData (void) const
{
  return (m_defaultData);
}

bool AttributeUI64::getIsDefaultDataValidFlag (void) const
{
  return (m_isDefaultDataValid);
}

void AttributeUI64::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

map<string, string> AttributeUI64::getSupportedConversions ()
{
    map<string, string> supportedConversions;

    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64_UC"] = "WAVE_ATTRIBUTE_TYPE_UI64_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = "WAVE_ATTRIBUTE_TYPE_UI32_UC";

    return (supportedConversions);
}

void AttributeUI64::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ui64 (pCValue, getValue ());  
}

AttributeUI64Vector::AttributeUI64Vector (const vector<UI64> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI64Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<UI64>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64Vector::AttributeUI64Vector (const vector<UI64> &data, const vector<UI64> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI64Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<UI64>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64Vector::AttributeUI64Vector (vector<UI64> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI64Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI64Vector::AttributeUI64Vector (vector<UI64> *pData, const vector<UI64> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUI64Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI64Vector::AttributeUI64Vector (const AttributeUI64Vector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<UI64>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64Vector::~AttributeUI64Vector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI64Vector &AttributeUI64Vector::operator = (const AttributeUI64Vector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<UI64> AttributeUI64Vector::getValue () const
{
    vector<UI64> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI64Vector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeUI64Vector::setValue (const vector<UI64> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI64Vector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI64Vector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUI64Vector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUI64Vector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUI64Vector::getSqlType ()
{
    return ("varchar");
}

void AttributeUI64Vector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI64Vector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUI64Vector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI64Vector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI64Vector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI64Vector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI64Vector::toString (string &valueString)
{
    vector<UI64>::iterator element = (*m_pData).begin ();
    vector<UI64>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString += string ("#") + (BigInteger (0xFFFFFFFFFFFFFFFFLLU & (*element)).toString ());
        element++;
    }
}

void AttributeUI64Vector::fromString (const string &valueString)
{
    vector<UI64>::iterator begin = (*m_pData).begin ();
    vector<UI64>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI64 value          = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        value = strtoull (valueString.c_str () + firstIndex + 1, NULL, 10);
        (*m_pData).push_back (value);

        startFromIndex = firstIndex + 1;
    }
}

void *AttributeUI64Vector::getPData ()
{
    return (m_pData);
}

void AttributeUI64Vector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<UI64> *> (pData));
}

Attribute *AttributeUI64Vector::clone ()
{
    AttributeUI64Vector *pAttributeUI64Vector = new AttributeUI64Vector (*this);

    return (pAttributeUI64Vector);
}

map<string, string> AttributeUI64Vector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_SI64_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_SI32_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_UI32_VECTOR";

    return (supportedConversions);
}

bool AttributeUI64Vector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<UI64> AttributeUI64Vector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeUI64Vector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeUI64Vector::setDefaultValue()
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

void AttributeUI64Vector::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ui64_vector (pCValue, getValue ());  
}

void AttributeUI64Vector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<UI64>;        
    }
  
    vector<UI64> vectorToAppend = ( dynamic_cast<AttributeUI64Vector *> (attribute))->getValue ();

    (*m_pData).insert( (*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end () );
}

void AttributeUI64Vector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<UI64>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<UI64> vectorToDelete = (dynamic_cast<AttributeUI64Vector *> (attribute))->getValue ();
    vector<UI64>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeUI64Vector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}
                                                                                                                                  
AttributeSI32::AttributeSI32 (const SI32 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData  = new SI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI32::AttributeSI32 (const SI32 &data, const SI32 &defData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defData),
    m_isDefaultDataValid (true)
{
    m_pData  = new SI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI32::AttributeSI32 (SI32 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI32::AttributeSI32 (SI32 *pData, const SI32 &defData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI32::AttributeSI32 (const AttributeSI32 &attribute)
    : Attribute (attribute)
{
    m_pData              = new SI32;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI32::~AttributeSI32 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSI32 &AttributeSI32::operator = (const AttributeSI32 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

SI32 AttributeSI32::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI32::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeSI32::setValue (const SI32 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI32::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeSI32::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;
    
    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

SI32 AttributeSI32::getDefaultData  () const
{
    return (m_defaultData);
}

bool AttributeSI32::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

bool AttributeSI32::validate () const
{
    prismAssert ( AttributeType::AttributeTypeSI32 == (getAttributeType ()), __FILE__, __LINE__);

    return ( AttributeType::AttributeTypeSI32 == (getAttributeType ()));
}

bool AttributeSI32::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeSI32::getSqlType ()
{
    return ("integer");
}

void AttributeSI32::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSI32::getSqlForCreate ()
{
    return (getAttributeName () + " integer");
}

void AttributeSI32::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void  AttributeSI32::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void  AttributeSI32::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSI32::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeSI32::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%ld", (long int) (getValue ()));

    valueString = string (buffer);
}

void AttributeSI32::fromString (const string &valueString)
{
    setValue (strtol (valueString.c_str (), NULL, 10));
}

ResourceId AttributeSI32::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    SI32 value = strtol (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeSI32::loadFromPlainString : Improper format of AttributeSI32 string"));
    }

    return status ;
}

void *AttributeSI32::getPData ()
{
    return (m_pData);
}

void AttributeSI32::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const SI32 *> (pData));
}

Attribute *AttributeSI32::clone ()
{
    AttributeSI32 *pAttributeSI32 = new AttributeSI32 (*this);

    return (pAttributeSI32);
}

map<string, string> AttributeSI32::getSupportedConversions ()
{
    map<string, string> supportedConversions;

    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_ENUM"] = "WAVE_ATTRIBUTE_TYPE_ENUM";

    return (supportedConversions);
}


void AttributeSI32::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

void AttributeSI32::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_si32 (pCValue, getValue ());  
}


AttributeSI32Vector::AttributeSI32Vector (const vector<SI32> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<SI32>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI32Vector::AttributeSI32Vector (const vector<SI32> &data, const vector<SI32> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<SI32>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI32Vector::AttributeSI32Vector (vector<SI32> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI32Vector::AttributeSI32Vector (vector<SI32> *pData, const vector<SI32> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI32Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}
AttributeSI32Vector::AttributeSI32Vector (const AttributeSI32Vector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<SI32>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeSI32Vector::~AttributeSI32Vector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSI32Vector &AttributeSI32Vector::operator = (const AttributeSI32Vector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<SI32> AttributeSI32Vector::getValue () const
{
    vector<SI32> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI32Vector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeSI32Vector::setValue (const vector<SI32> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI32Vector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeSI32Vector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeSI32Vector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeSI32Vector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeSI32Vector::getSqlType ()
{
    return ("varchar");
}

void AttributeSI32Vector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSI32Vector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeSI32Vector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void  AttributeSI32Vector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void  AttributeSI32Vector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSI32Vector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeSI32Vector::toString (string &valueString)
{
    vector<SI32>::iterator element = (*m_pData).begin ();
    vector<SI32>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString += string ("#") + (*element);
        element++;
    }
}

void AttributeSI32Vector::fromString (const string &valueString)
{
    vector<SI32>::iterator begin = (*m_pData).begin ();
    vector<SI32>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    SI32 value          = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        value = atoi (valueString.c_str () + firstIndex + 1);
        (*m_pData).push_back (value);

        startFromIndex = firstIndex + 1;
    }
}

void *AttributeSI32Vector::getPData ()
{
    return (m_pData);
}

void AttributeSI32Vector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<SI32> *> (pData));
}

map<string, string> AttributeSI32Vector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_SI64_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_UI64_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_UI32_VECTOR";

    return (supportedConversions);
}

Attribute *AttributeSI32Vector::clone ()
{
    AttributeSI32Vector *pAttributeSI32Vector = new AttributeSI32Vector (*this);

    return (pAttributeSI32Vector);
}

bool AttributeSI32Vector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<SI32> AttributeSI32Vector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeSI32Vector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeSI32Vector::setDefaultValue()
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

void AttributeSI32Vector::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_si32_vector (pCValue, getValue ());  
}

void AttributeSI32Vector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<SI32>;        
    }
  
    vector<SI32> vectorToAppend = ( dynamic_cast<AttributeSI32Vector *> (attribute))->getValue ();

    (*m_pData).insert( (*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end () );
}

void AttributeSI32Vector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<SI32>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<SI32> vectorToDelete = (dynamic_cast<AttributeSI32Vector *> (attribute))->getValue ();
    vector<SI32>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeSI32Vector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void AttributeSI32Vector::getPlainString (string &valueString)
{
    vector<SI32>::const_iterator itrElement = (*m_pData).begin ();
    vector<SI32>::const_iterator endElement = (*m_pData).end ();

    bool separatorRequired = false;

    for (; endElement != itrElement; itrElement++)
    {
        if (false == separatorRequired)
        {
            separatorRequired = true;
        }
        else
        {
            valueString += " ";
        }

        SI32 si32Value = *itrElement;

        valueString = valueString + si32Value;
    }
}

AttributeUI16::AttributeUI16 (const UI16 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute             ( AttributeType::AttributeTypeUI16, attributeName, attributeUserTag, isOperational),
      m_defaultData         (0),
      m_isDefaultDataValid  (false)
{
    m_pData  = new UI16;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI16::AttributeUI16 (const UI16 &data, const UI16 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI16, attributeName, attributeUserTag, isOperational),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (true)
{
    m_pData  = new UI16;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI16::AttributeUI16 (UI16 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI16, attributeName, attributeUserTag, isOperational),
      m_defaultData        (0),
      m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI16::AttributeUI16 (UI16 *pData, const UI16 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI16, attributeName, attributeUserTag, isOperational),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI16::AttributeUI16 (const AttributeUI16 &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI16;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI16::~AttributeUI16 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI16 &AttributeUI16::operator = (const AttributeUI16 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI16 AttributeUI16::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI16::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeUI16::setValue (const UI16 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI16::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI16::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUI16 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUI16 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUI16::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeUI16::getSqlType ()
{
    return ("integer");
}

void AttributeUI16::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI16::getSqlForCreate ()
{
    return (getAttributeName () + " integer");
}

void AttributeUI16::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI16::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeUI16::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI16::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeUI16::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%u", (unsigned int) (getValue ()));

    valueString = string (buffer);
}

void AttributeUI16::fromString (const string &valueString)
{
    setValue (strtoul (valueString.c_str (), NULL, 10));
}

ResourceId AttributeUI16::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    UI32 value = strtoul (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeUI16::loadFromPlainString : Improper format of AttributeUI16 string"));
    }

    return status ;
}

void *AttributeUI16::getPData ()
{
    return (m_pData);
}

void AttributeUI16::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI16 *> (pData));
}

Attribute *AttributeUI16::clone ()
{
    AttributeUI16 *pAttributeUI16 = new AttributeUI16 (*this);

    return (pAttributeUI16);
}

bool AttributeUI16::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;
    
    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

UI16 AttributeUI16::getDefaultData (void) const
{
  return (m_defaultData);
}

bool AttributeUI16::getIsDefaultDataValidFlag (void) const
{
  return (m_isDefaultDataValid);
}

void AttributeUI16::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

map<string, string> AttributeUI16::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";

    return (supportedConversions);
}

void AttributeUI16::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ui16 (pCValue, getValue ());  
}

AttributeSI16::AttributeSI16 (const SI16 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI16, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData  = new SI16;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI16::AttributeSI16 (const SI16 &data, const SI16 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI16, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new SI16;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI16::AttributeSI16 (SI16 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI16, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI16::AttributeSI16 (SI16 *pData, const SI16 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI16, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI16::AttributeSI16 (const AttributeSI16 &attribute)
    : Attribute (attribute)
{
    m_pData              = new SI16;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI16::~AttributeSI16 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSI16 &AttributeSI16::operator = (const AttributeSI16 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

SI16 AttributeSI16::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI16::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeSI16::setValue (const SI16 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI16::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeSI16::validate () const
{
    prismAssert ( AttributeType::AttributeTypeSI16 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeSI16 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeSI16::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeSI16::getSqlType ()
{
    return ("integer");
}

void AttributeSI16::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSI16::getSqlForCreate ()
{
    return (getAttributeName () + " integer");
}

void AttributeSI16::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeSI16::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeSI16::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSI16::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeSI16::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%d", (int) (getValue ()));

    valueString = string (buffer);
}

void AttributeSI16::fromString (const string &valueString)
{
    setValue (strtol (valueString.c_str (), NULL, 10));
}

ResourceId AttributeSI16::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    UI32 value = strtol (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeSI16::loadFromPlainString : Improper format of AttributeSI16 string"));
    }

    return status ;
}

void *AttributeSI16::getPData ()
{
    return (m_pData);
}

void AttributeSI16::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast< const SI16 *> (pData));
}

Attribute *AttributeSI16::clone ()
{
    AttributeSI16 *pAttributeSI16 = new AttributeSI16 (*this);

    return (pAttributeSI16);
}

map<string, string> AttributeSI16::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";

    return (supportedConversions);
}

bool AttributeSI16::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

SI16 AttributeSI16::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeSI16::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeSI16::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

void AttributeSI16::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_si16 (pCValue, getValue ());  
}

AttributeUI8::AttributeUI8 (const UI8 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI8, attributeName, attributeUserTag, isOperational),
      m_defaultData        (0),
      m_isDefaultDataValid (false)
{
    m_pData  = new UI8;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI8::AttributeUI8 (const UI8 &data, const UI8 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI8, attributeName, attributeUserTag, isOperational),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (true)
{
    m_pData  = new UI8;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI8::AttributeUI8 (UI8 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI8, attributeName, attributeUserTag, isOperational),
      m_defaultData        (0),
      m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI8::AttributeUI8 (UI8 *pData, const UI8 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            ( AttributeType::AttributeTypeUI8, attributeName, attributeUserTag, isOperational),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI8::AttributeUI8 (const AttributeUI8 &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI8;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI8::~AttributeUI8 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI8 &AttributeUI8::operator = (const AttributeUI8 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI8 AttributeUI8::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI8::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeUI8::setValue (const UI8 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI8::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI8::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUI8 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUI8 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUI8::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeUI8::getSqlType ()
{
    return ("integer");
}

void AttributeUI8::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI8::getSqlForCreate ()
{
    return (getAttributeName () + " integer");
}

void AttributeUI8::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI8::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeUI8::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI8::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeUI8::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%u", (unsigned int) getValue ());

    valueString = string (buffer);
}

void AttributeUI8::fromString (const string &valueString)
{
    setValue (strtoul (valueString.c_str (), NULL, 10));
}

ResourceId AttributeUI8::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    UI32 value = strtoul (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeUI8::loadFromPlainString : Improper format of AttributeUI8 string"));
    }

    return status ;
}

void *AttributeUI8::getPData ()
{
    return (m_pData);
}

void AttributeUI8::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI8 *> (pData));
}

Attribute *AttributeUI8::clone ()
{
    AttributeUI8 *pAttributeUI8 = new AttributeUI8 (*this);

    return (pAttributeUI8);
}

bool AttributeUI8::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;
    
    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

UI8 AttributeUI8::getDefaultData (void) const
{
  return (m_defaultData);
}

bool AttributeUI8::getIsDefaultDataValidFlag (void) const
{
  return (m_isDefaultDataValid);
}

void AttributeUI8::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

map<string, string> AttributeUI8::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";

    return (supportedConversions);
}

void AttributeUI8::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ui8 (pCValue, getValue ());  
}

AttributeSI8::AttributeSI8 (const SI8 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI8, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData  = new SI8;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI8::AttributeSI8 (const SI8 &data, const SI8 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI8, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new SI8;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}
AttributeSI8::AttributeSI8 (SI8 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI8, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI8::AttributeSI8 (SI8 *pData, const SI8 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeSI8, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}
AttributeSI8::AttributeSI8 (const AttributeSI8 &attribute)
    : Attribute (attribute)
{
    m_pData              = new SI8;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeSI8::~AttributeSI8 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSI8 &AttributeSI8::operator = (const AttributeSI8 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

SI8 AttributeSI8::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI8::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeSI8::setValue (const SI8 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI8::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeSI8::validate () const
{
    prismAssert ( AttributeType::AttributeTypeSI8 == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeSI8 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeSI8::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeSI8::getSqlType ()
{
    return ("integer");
}

void AttributeSI8::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSI8::getSqlForCreate ()
{
    return (getAttributeName () + " integer");
}

void  AttributeSI8::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeSI8::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeSI8::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSI8::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeSI8::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%d", (int) (getValue ()));

    valueString = string (buffer);
}

void AttributeSI8::fromString (const string &valueString)
{
    setValue (strtol (valueString.c_str (), NULL, 10));
}

ResourceId AttributeSI8::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    UI32 value = strtol (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeSI8::loadFromPlainString : Improper format of AttributeSI8 string"));
    }

    return status ;
}

void *AttributeSI8::getPData ()
{
    return (m_pData);
}

void AttributeSI8::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const SI8 *> (pData));
}

Attribute *AttributeSI8::clone ()
{
    AttributeSI8 *pAttributeSI8 = new AttributeSI8 (*this);

    return (pAttributeSI8);
}

map<string, string> AttributeSI8::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";

    return (supportedConversions);
}

bool AttributeSI8::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

SI8 AttributeSI8::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeSI8::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeSI8::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

void AttributeSI8::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_si8 (pCValue, getValue ());  
}

AttributeBool::AttributeBool (const bool &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeBool, attributeName, attributeUserTag, isOperational),
    m_defaultData        (false),
    m_isDefaultDataValid (false)
{
    m_pData  = new bool;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}


AttributeBool::AttributeBool (const bool &data, const bool &defaultFlag, const string &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeBool, attributeName, attributeUserTag, isOperational)
{
    m_pData  = new bool;
    *m_pData = data;

    setDefaultDataForBool( defaultFlag, defaultData);
    setIsMemoryOwnedByAttribute (true);
}


AttributeBool::AttributeBool (bool *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeBool, attributeName, attributeUserTag, isOperational),
    m_defaultData        (false),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}


AttributeBool::AttributeBool (bool *pData, const bool &defaultFlag, const string &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeBool, attributeName, attributeUserTag, isOperational)
{
    m_pData = pData;
    setDefaultDataForBool( defaultFlag, defaultData );
    setIsMemoryOwnedByAttribute (false);
}


AttributeBool::AttributeBool (const AttributeBool &attribute)
    : Attribute (attribute)
{
    m_pData  = new bool;
    *m_pData = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeBool::~AttributeBool ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

void AttributeBool::setDefaultDataForBool ( const bool &defaultFlag, const string &defaultData )
{
    m_isDefaultDataValid = defaultFlag;
    m_defaultData = false ;

    if ( "true" == defaultData )
    {
        m_defaultData = true;
    }
}

AttributeBool &AttributeBool::operator = (const AttributeBool &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

bool AttributeBool::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBool::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (false);
    }
}

void AttributeBool::setValue (const bool &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBool::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeBool::validate () const
{
    prismAssert ( AttributeType::AttributeTypeBool == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeBool == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeBool::getSqlType ()
{
    return ("boolean");
}

void AttributeBool::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeBool::getSqlForCreate ()
{
    return (getAttributeName () + " boolean");
}

void AttributeBool::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString = (getValue ()) ? "true" : "false";

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeBool::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeBool::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeBool::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeBool::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%s", true == (getValue ()) ? "true" : "false");

    valueString = string (buffer);
}

void AttributeBool::fromString (const string &valueString)
{
    if (("true" == valueString) || ("t" == valueString))
    {
        setValue (true);
    }
    else if (("false" == valueString) || ("f" == valueString))
    {
        setValue (false);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

ResourceId AttributeBool::loadFromPlainString (const string &valueString)
{
    if (("true" == valueString) || ("t" == valueString))
    {
        setValue (true);
    }
    else if (("false" == valueString) || ("f" == valueString))
    {
        setValue (false);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AttributeBool::loadFromPlainString invalid input string");
        return WAVE_MESSAGE_ERROR;
    }

    return WAVE_MESSAGE_SUCCESS;
}

void *AttributeBool::getPData ()
{
    return (m_pData);
}

void AttributeBool::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const bool *> (pData));
}

Attribute *AttributeBool::clone ()
{
    AttributeBool *pAttributeBool = new AttributeBool (*this);

    return (pAttributeBool);
}

map<string, string> AttributeBool::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR";

    return (supportedConversions);

}


bool AttributeBool::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeBool::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeBool::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}


void AttributeBool::setDefaultValue()
{
    *m_pData = false;
}

void AttributeBool::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_bool (pCValue, getValue ());
}

AttributeBoolVector::AttributeBoolVector (const vector<bool> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeBoolVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<bool>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeBoolVector::AttributeBoolVector (const vector<bool> &data, const vector<bool> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeBoolVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<bool>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeBoolVector::AttributeBoolVector (vector<bool> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeBoolVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeBoolVector::AttributeBoolVector (vector<bool> *pData, const vector<bool> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeBoolVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeBoolVector::AttributeBoolVector (const AttributeBoolVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<bool>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeBoolVector::~AttributeBoolVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeBoolVector &AttributeBoolVector::operator = (const AttributeBoolVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<bool> AttributeBoolVector::getValue () const
{
    vector<bool> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBoolVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeBoolVector::setValue (const vector<bool> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBoolVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeBoolVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeBoolVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeBoolVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeBoolVector::getSqlType ()
{
    return ("varchar");
}

void AttributeBoolVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeBoolVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeBoolVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeBoolVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeBoolVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeBoolVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeBoolVector::toString (string &valueString)
{
    vector<bool>::iterator element = (*m_pData).begin ();
    vector<bool>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        char buffer[64] = {0};

        snprintf (buffer, 64, "%s", true == (*element) ? "true" : "false");
        valueString += string ("#") + buffer;
        element++;
    }
}

void AttributeBoolVector::fromString (const string &valueString)
{
    vector<bool>::iterator begin = (*m_pData).begin ();
    vector<bool>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI    startFromIndex = 0;
    ULI    firstIndex     = 0;
    ULI    secondIndex    = 0;
    string value          = "";

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        secondIndex = valueString.find ("#", firstIndex + 1);

        if (string::npos == secondIndex)
        {
            secondIndex = valueString.size ();
        }

        value = valueString.substr (firstIndex + 1, secondIndex - firstIndex - 1);

        if ("true" == value)
        {
            (*m_pData).push_back (true);
        }
        else if ("false" == value)
        {
            (*m_pData).push_back (false);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        startFromIndex = firstIndex + 1;
    }
}

void *AttributeBoolVector::getPData ()
{
    return (m_pData);
}

void AttributeBoolVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<bool> *> (pData));
}

Attribute *AttributeBoolVector::clone ()
{
    AttributeBoolVector *pAttributeBoolVector = new AttributeBoolVector (*this);

    return (pAttributeBoolVector);
}

map<string, string> AttributeBoolVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";

    return (supportedConversions);
}

bool AttributeBoolVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<bool> AttributeBoolVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeBoolVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeBoolVector::setDefaultValue()
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

void AttributeBoolVector::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_bool_vector (pCValue, getValue ());
}

void AttributeBoolVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<bool>;        
    }
  
    vector<bool> vectorToAppend = ( dynamic_cast<AttributeBoolVector *> (attribute))->getValue ();

    (*m_pData).insert( (*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end () );
}

void AttributeBoolVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<bool>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<bool> vectorToDelete = (dynamic_cast<AttributeBoolVector *> (attribute))->getValue ();
    vector<bool>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeBoolVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

AttributeString::AttributeString (const string &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              ( AttributeType::AttributeTypeString, attributeName, attributeUserTag, isOperational),
    m_defaultString        (""),
    m_isDefaultStringValid (false)
{
    m_pData  = new string;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeString::AttributeString (const string &data, const bool &defaultFlag, const string &defaultString, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute              ( AttributeType::AttributeTypeString, attributeName, attributeUserTag, isOperational),
    m_defaultString        (defaultString),
    m_isDefaultStringValid (defaultFlag)
{
    m_pData  = new string;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeString::AttributeString (string *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              ( AttributeType::AttributeTypeString, attributeName, attributeUserTag, isOperational),
    m_defaultString        (""),
    m_isDefaultStringValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeString::AttributeString (string *pData, const bool &defaultFlag, const string &defaultString, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute              ( AttributeType::AttributeTypeString, attributeName, attributeUserTag, isOperational),
    m_defaultString        (defaultString),
    m_isDefaultStringValid (defaultFlag)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeString::AttributeString (const AttributeString &attribute)
: Attribute (attribute)
{
    m_pData                = new string;
    *m_pData               = attribute.getValue ();
    m_defaultString        = attribute.getDefaultString ();
    m_isDefaultStringValid = attribute.getIsDefaultStringValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeString::~AttributeString ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeString &AttributeString::operator = (const AttributeString &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultString        = attribute.getDefaultString ();
    m_isDefaultStringValid = attribute.getIsDefaultStringValidFlag ();

    return (*this);
}

string AttributeString::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeString::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return ("");
    }
}

void AttributeString::setValue (const string &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeString::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeString::validate () const
{
    prismAssert ( AttributeType::AttributeTypeString == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeString == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeString::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL              == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE           == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE       == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE          == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_SIMILAR_TO     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_SIMILAR_TO == attributeConditionOperator) 
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeString::getSqlType ()
{
    return ("varchar");
}

void AttributeString::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeString::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeString::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeString::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeString::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeString::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_SIMILAR_TO != attributeConditionOperator && WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_SIMILAR_TO != attributeConditionOperator)
    {
        sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
    }
    else
    {
        sqlForSelect += "(" + getAttributeName () + " || \'\')" + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
    }
}

void AttributeString::toString (string &valueString)
{
    valueString = (*m_pData);
}

void AttributeString::fromString (const string &valueString)
{
    *m_pData = valueString;
}

void *AttributeString::getPData ()
{
    return (m_pData);
}

void AttributeString::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const string *> (pData));
}

Attribute *AttributeString::clone ()
{
    AttributeString *pAttributeString = new AttributeString (*this);

    return (pAttributeString);
}

void AttributeString::toEscapedString (string &valueString)
{
    UI32                size                  = m_pData->size ();
    char               *pTemp                 = new char[2 * size + 1];
    UI32                returnSize            = 0;
    SI32                errorCode             = 0;
    DatabaseConnection *pDatabaseconnection   = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn             *pPGConnection         = NULL;
    string              databaseErrorMessage;

    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    returnSize = PQescapeStringConn (pPGConnection, pTemp, m_pData->c_str (), size, &errorCode);

    if (0 != errorCode)
    {
        databaseErrorMessage = PQerrorMessage (pPGConnection);
        trace (TRACE_LEVEL_ERROR, "AttributeString::toEscapedString : error connecting with Database :\n" + databaseErrorMessage + string (", Return Size : ") + returnSize);
    }

    prismAssert (0 == errorCode, __FILE__, __LINE__);

    valueString = pTemp;

    delete [] pTemp;
}

bool AttributeString::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultStringValidFlag() )
    {   
        if ( getDefaultString() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

string AttributeString::getDefaultString () const
{
    return (m_defaultString);
}

bool AttributeString::getIsDefaultStringValidFlag () const
{
    return (m_isDefaultStringValid);
}

void AttributeString::setDefaultValue()
{
    if(true == m_isDefaultStringValid)
    {
        setValue(&m_defaultString);
    }
    else
    {
        *m_pData = "";
    }
}

map<string, string> AttributeString::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UNION"] = "WAVE_ATTRIBUTE_TYPE_UNION";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MAC_RANGE"] = "WAVE_ATTRIBUTE_TYPE_MAC_RANGE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_RANGE"] = "WAVE_ATTRIBUTE_TYPE_UI32_RANGE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_UC"] = "WAVE_ATTRIBUTE_TYPE_STRING_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_HOST_UC"] = "WAVE_ATTRIBUTE_TYPE_HOST_UC";

    return (supportedConversions);
}

void AttributeString::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_string (pCValue, getValue ()); 
}

AttributeUnion::AttributeUnion (const string &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUnion, attributeName, attributeUserTag, isOperational),
    m_defaultData        (""),
    m_isDefaultDataValid (false)
{
    m_pData  = new string;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeUnion::AttributeUnion (const string &data, const bool &defaultFlag, const string &defaultUnion, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeUnion, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultUnion),
    m_isDefaultDataValid (defaultFlag)
{
    m_pData  = new string;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUnion::AttributeUnion (string *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeUnion, attributeName, attributeUserTag, isOperational),
    m_defaultData        (""),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeUnion::AttributeUnion (string *pData, const bool &defaultFlag, const string &defaultUnion, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeUnion, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultUnion),
    m_isDefaultDataValid (defaultFlag)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}


    AttributeUnion::AttributeUnion (const AttributeUnion &attribute)
: Attribute (attribute)
{
    m_pData              = new string;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeUnion::~AttributeUnion ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUnion &AttributeUnion::operator = (const AttributeUnion &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

string AttributeUnion::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUnion::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return ("");
    }
}

void AttributeUnion::setValue (const string &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUnion::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUnion::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUnion == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUnion == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUnion::getSqlType ()
{
    return ("varchar");
}

void AttributeUnion::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUnion::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUnion::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUnion::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUnion::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUnion::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUnion::toString (string &valueString)
{
    valueString = (*m_pData);
}

void AttributeUnion::fromString (const string &valueString)
{
    *m_pData = valueString;
}

void *AttributeUnion::getPData ()
{
    return (m_pData);
}

void AttributeUnion::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const string *> (pData));
}

Attribute *AttributeUnion::clone ()
{
    AttributeUnion *pAttributeUnion = new AttributeUnion (*this);

    return (pAttributeUnion);
}

void AttributeUnion::toEscapedString (string &valueString)
{
    UI32                size                = m_pData->size ();
    char               *pTemp               = new char[2 * size + 1];
    UI32                returnSize          = 0;
    SI32                errorCode           = 0;
    DatabaseConnection *pDatabaseconnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn             *pPGConnection       = NULL;

    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    returnSize = PQescapeStringConn (pPGConnection, pTemp, m_pData->c_str (), size, &errorCode);

    if (0 != errorCode)
    {
    	trace (TRACE_LEVEL_ERROR, string ("AttributeUnion::toEscapedString : Return Size ") + returnSize);
    }

    prismAssert (0 == errorCode, __FILE__, __LINE__);

    valueString = pTemp;

    delete [] pTemp;
}

map<string, string> AttributeUnion::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";

    return (supportedConversions);
}

bool AttributeUnion::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

string AttributeUnion::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeUnion::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeUnion::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = "";
    }
}

void AttributeUnion::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_string (pCValue, getValue ()); 
}

AttributeChar::AttributeChar (const char &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeChar, attributeName, attributeUserTag, isOperational),
    m_defaultData        ('\0'),
    m_isDefaultDataValid (false)
{
    m_pData  = new char;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeChar::AttributeChar (const char &data, const char &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeChar, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new char;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeChar::AttributeChar (char *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeChar, attributeName, attributeUserTag, isOperational),
    m_defaultData        ('\0'),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeChar::AttributeChar (char *pData, const char &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeChar, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeChar::AttributeChar (const AttributeChar &attribute)
: Attribute (attribute)
{
    m_pData              = new char;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeChar::~AttributeChar ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeChar &AttributeChar::operator = (const AttributeChar &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

char AttributeChar::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeChar::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return ('\0');
    }
}

void AttributeChar::setValue (const char &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeChar::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeChar::validate () const
{
    prismAssert ( AttributeType::AttributeTypeChar == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeChar == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeChar::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeChar::getSqlType ()
{
    return ("char (1)");
}

void AttributeChar::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeChar::getSqlForCreate ()
{
    return (getAttributeName () + " char (1)");
}

void AttributeChar::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    sqlForInsert += string (",'") + getValue () + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeChar::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeChar::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeChar::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeChar::toString (string &valueString)
{
    if (0 == (*m_pData))
    {
        valueString = "(null)";
    }
    else
    {
        valueString = (*m_pData);
    }
}

void AttributeChar::fromString (const string &valueString)
{
    if (0 == (strcmp (valueString.c_str (), "(null)")))
    {
        *m_pData = '\0';
    }
    else
    {
        *m_pData = (valueString.c_str ())[0];
    }
}

ResourceId AttributeChar::loadFromPlainString (const string &valueString)
{
    if (0 == (strcmp (valueString.c_str (), "(null)")))
    {
        *m_pData = '\0';
    }
    else if(valueString.length() > 1)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeChar::loadFromPlainString : Improper format for  AttributeChar"));
        return WAVE_MESSAGE_ERROR;
    }
    else
    {
        *m_pData = (valueString.c_str ())[0];
    }
    
    return WAVE_MESSAGE_SUCCESS;
}

void *AttributeChar::getPData ()
{
    return (m_pData);
}

void AttributeChar::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const char *> (pData));
}

Attribute *AttributeChar::clone ()
{
    AttributeChar *pAttributeChar = new AttributeChar (*this);

    return (pAttributeChar);
}

map<string, string> AttributeChar::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeChar::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

char AttributeChar::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeChar::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeChar::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        *m_pData = '\0';
    }
}

void AttributeChar::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_char (pCValue, getValue ()); 
}

    AttributeStringVector::AttributeStringVector (const vector<string> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = new vector<string> (data);

    setIsMemoryOwnedByAttribute (true);
}

    AttributeStringVector::AttributeStringVector (const vector<string> &data, const vector<string> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = new vector<string> (data);

    setIsMemoryOwnedByAttribute (true);
}

    AttributeStringVector::AttributeStringVector (vector<string> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeStringVector::AttributeStringVector (vector<string> *pData, const vector<string> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeStringVector::AttributeStringVector (const AttributeStringVector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<string> (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVector::~AttributeStringVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeStringVector &AttributeStringVector::operator = (const AttributeStringVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<string> AttributeStringVector::getValue () const
{
    vector<string> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeStringVector::setValue (const vector<string> &data)
{
    if (true == (validate ()))
    {
        UI32 numberOfEntries = data.size ();
        UI32 i;

        for (i = 0; i < numberOfEntries; i++)
        {
            (*m_pData)[i] = data[i];
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeStringVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeStringVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeStringVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeStringVector::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE  == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeStringVector::getSqlType ()
{
    return ("varchar");
}

void AttributeStringVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeStringVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeStringVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeStringVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeStringVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeStringVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeStringVector::toString (string &valueString)
{
    vector<string>::iterator element = (*m_pData).begin ();
    vector<string>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + (*element).size () + "#" + (*element);
        element++;
    }
}


void AttributeStringVector::getPlainString (string &valueString)
{
    vector<string>::const_iterator itrElement = (*m_pData).begin ();
    vector<string>::const_iterator endElement = (*m_pData).end ();

    bool separatorRequired = false;

    for (; endElement != itrElement; itrElement++)
    {
        if (false == separatorRequired)
        {
            separatorRequired = true;
        }
        else
        {
            valueString += " "; 
        }
        
        string value = *itrElement;

        StringUtils::applyConfigStringFormatting (value, false);
        
        valueString += value;  
            
    }
}

void AttributeStringVector::fromString (const string &valueString)
{
    vector<string>::iterator begin = (*m_pData).begin ();
    vector<string>::iterator end   = (*m_pData).end ();

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

void *AttributeStringVector::getPData ()
{
    return (m_pData);
}

void AttributeStringVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<string> *> (pData));
}

Attribute *AttributeStringVector::clone ()
{
    AttributeStringVector *pAttributeStringVector = new AttributeStringVector (*this);

    return (pAttributeStringVector);
}

void AttributeStringVector::toEscapedString (string &valueString)
{
    vector<string>::iterator  element             = (*m_pData).begin ();
    vector<string>::iterator  end                 = (*m_pData).end ();

    DatabaseConnection       *pDatabaseconnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn                   *pPGConnection       = NULL;

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    valueString = "";

    while (end != element)
    {
        UI32  size       = element->size ();
        char *pTemp      = new char[2 * size + 1];
        UI32  returnSize = 0;
        SI32  errorCode  = 0;

        prismAssert (NULL != pTemp, __FILE__, __LINE__);

        returnSize = PQescapeStringConn (pPGConnection, pTemp, element->c_str (), size, &errorCode);

        prismAssert (0 == errorCode, __FILE__, __LINE__);

        valueString = valueString + returnSize + "#" + pTemp;

        delete [] pTemp;

        element++;
    }
}

bool AttributeStringVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<string> AttributeStringVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeStringVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeStringVector::setDefaultValue()
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

map<string, string> AttributeStringVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_UC"] = "WAVE_ATTRIBUTE_TYPE_STRING_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATE_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_DATE_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_TIME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_TIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_RANGE"] = "WAVE_ATTRIBUTE_TYPE_UI32_RANGE";

    return (supportedConversions);
}

void AttributeStringVector::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_string_vector (pCValue, getValue ());
}

void AttributeStringVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<string>;        
    }

    vector<string> vectorToAppend = ( dynamic_cast<AttributeStringVector *> (attribute))->getValue ();

    (*m_pData).insert( (*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end () );
}

void AttributeStringVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<string>::iterator iterOriginal = (*m_pData).begin ();

    vector<string> vectorToDelete = (dynamic_cast<AttributeStringVector *> (attribute))->getValue ();
    vector<string>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeStringVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

    AttributeObjectId::AttributeObjectId (const ObjectId &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new ObjectId;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeObjectId::AttributeObjectId (const ObjectId &data, const ObjectId &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new ObjectId;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeObjectId::AttributeObjectId (ObjectId *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeObjectId::AttributeObjectId (ObjectId *pData, const ObjectId &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;

    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeObjectId::AttributeObjectId (const AttributeObjectId &attribute)
: Attribute (attribute)
{
    m_pData              = new ObjectId;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeObjectId::~AttributeObjectId ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeObjectId &AttributeObjectId::operator = (const AttributeObjectId &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

ObjectId AttributeObjectId::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeObjectId::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (ObjectId (0, 0));
    }
}

void AttributeObjectId::setValue (const ObjectId &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeObjectId::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeObjectId::validate () const
{
    prismAssert ( AttributeType::AttributeTypeObjectId == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeObjectId == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeObjectId::getSqlType ()
{
    return ("integer");
}

void AttributeObjectId::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName () + "ClassId", getSqlType (), getAttributeTypeString ()));
    pOrmTable->addColumn (new OrmColumn (getAttributeName () + "InstanceId", "bigint", getAttributeTypeString (),getIsPrimary ()));
}

string AttributeObjectId::getSqlForCreate ()
{
    string attributeName = getAttributeName ();

    return (attributeName + "ClassId integer, " + attributeName + "InstanceId bigint");
}

void AttributeObjectId::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    if (true == isFirst)
    {
        sqlForInsert += string ("") + m_pData->getClassId () + string (",") + m_pData->getInstanceId ();

        sqlForPreValues += string ("") + getAttributeName () + "ClassId," + getAttributeName () + "InstanceId";
    }
    else
    {
        sqlForInsert += string (",") + m_pData->getClassId () + string (",") + m_pData->getInstanceId ();

        sqlForPreValues += string (",") + getAttributeName () + "ClassId," + getAttributeName () + "InstanceId";
    }
}

void AttributeObjectId::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    if (true == isFirst)
    {
        sqlForUpdate += getAttributeName () + string ("ClassId = ") + (m_pData->getClassId ()) + ", " + (getAttributeName ()) + string ("InstanceId = ") + (m_pData->getInstanceId ());
    }
    else
    {
        sqlForUpdate += "," + getAttributeName () + string ("ClassId = ") + (m_pData->getClassId ()) + ", " + (getAttributeName ()) + string ("InstanceId = ") + (m_pData->getInstanceId ());
    }
}

void AttributeObjectId::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeObjectId::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string logicOperator = "";

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL == attributeConditionOperator)
    {
        logicOperator = " AND ";
    }
    else if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator)
    {
        logicOperator = " OR ";
    }
    else
    {
        // The equal and not equal condition operators are the only two that should be supported.  The SQL query will fail in this case if the check above is not set properly.
    }

    sqlForSelect += getAttributeName () + string ("ClassId") + FrameworkToolKit::localize (attributeConditionOperator) + (m_pData->getClassId ()) + logicOperator + (getAttributeName ()) + string ("InstanceId") + FrameworkToolKit::localize (attributeConditionOperator) + (m_pData->getInstanceId ());
}

void AttributeObjectId::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%u*%llu", m_pData->getClassId (), m_pData->getInstanceId ());

    valueString = string (buffer);
}

void AttributeObjectId::fromString (const string &valueString)
{
    ObjectId tempObjectId;
    ULI      tempIndex;

    tempObjectId.setClassId (strtoul (valueString.c_str (), NULL, 10));

    tempIndex = valueString.find ("*", 0);

    prismAssert (string::npos != tempIndex, __FILE__, __LINE__);

    if (string::npos != tempIndex)
    {
        tempObjectId.setInstanceId (strtoull (valueString.c_str () + tempIndex + 1, NULL, 10));
    }

    setValue (tempObjectId);
}


ResourceId AttributeObjectId::loadFromPlainString (const string &valueString)
{
    ResourceId  status                        = WAVE_MESSAGE_ERROR;
    char*       firstInvalidCharacterPosition = NULL;
    ObjectId    tempObjectId;
    ULI         tempIndex;

    tempIndex = valueString.find ("*", 0);

    if (string::npos != tempIndex)
    {
        tempObjectId.setClassId (strtoul ((valueString.substr(0,tempIndex)).c_str (), &(firstInvalidCharacterPosition), 10));

        // Check if the string has invalid character
        if ( *firstInvalidCharacterPosition == '\0' )
        {   
            tempObjectId.setInstanceId (strtoull ( (valueString.substr (tempIndex+1)).c_str (), &(firstInvalidCharacterPosition), 10));
            
            if ( *firstInvalidCharacterPosition == '\0' )
            {
                status = WAVE_MESSAGE_SUCCESS;
                setValue (tempObjectId);
            }
        }
    }

    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeObjectId::loadFromPlainString : Improper format of AttributeObjectId string"));
    }

    return status ;
}

void *AttributeObjectId::getPData ()
{
    return (m_pData);
}

void AttributeObjectId::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const ObjectId *> (pData));
}

Attribute *AttributeObjectId::clone ()
{
    AttributeObjectId *pAttributeObjectId = new AttributeObjectId (*this);

    return (pAttributeObjectId);
}

void AttributeObjectId::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    string  attributeName    = getAttributeName ();
    SI32    classIdColumn    = PQfnumber (pResult, ((getAttributeName ()) + "ClassId").c_str ());
    SI32    instanceIdColumn = PQfnumber (pResult, ((getAttributeName ()) + "InstanceId").c_str ());

    if ((-1 != classIdColumn) && (-1 != instanceIdColumn))
    {
        char   *pclassIdValue    = PQgetvalue (pResult, row, classIdColumn);
        char   *pInstanceIdValue = PQgetvalue (pResult, row, instanceIdColumn);
        UI32    classId          = strtoul  (pclassIdValue, NULL, 10);
        UI64    instanceId       = strtoull (pInstanceIdValue, NULL, 10);

        *m_pData = ObjectId (classId, instanceId);
    }
}

map<string, string> AttributeObjectId::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR";

    return (supportedConversions);
}

bool AttributeObjectId::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

ObjectId AttributeObjectId::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeObjectId::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeObjectId::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        ObjectId tempData;
        setValue(&tempData);
    }
}

void AttributeObjectId::getCValue (WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__);
}

    AttributeObjectIdAssociation::AttributeObjectIdAssociation (const ObjectId &data, const string &attributeName, const string &associatedTo, const bool &canBeEmpty, const UI32 &attributeUserTag, const bool &isOperational)
: AttributeObjectId (data, attributeName, attributeUserTag, isOperational),
    m_associatedTo    (associatedTo),
    m_canBeEmpty      (canBeEmpty)
{
}

    AttributeObjectIdAssociation::AttributeObjectIdAssociation (ObjectId *pData, const string &attributeName, const string &associatedTo, const bool &canBeEmpty, const UI32 &attributeUserTag, const bool &isOperational)
: AttributeObjectId (pData, attributeName, attributeUserTag, isOperational),
    m_associatedTo    (associatedTo),
    m_canBeEmpty      (canBeEmpty)
{
}

    AttributeObjectIdAssociation::AttributeObjectIdAssociation (const AttributeObjectIdAssociation &attribute)
: AttributeObjectId (attribute)
{
    m_associatedTo = attribute.m_associatedTo;
    m_canBeEmpty   = attribute.m_canBeEmpty;
}

AttributeObjectIdAssociation::~AttributeObjectIdAssociation ()
{
}

bool AttributeObjectIdAssociation::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NULL == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NOT_NULL == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

AttributeObjectIdAssociation &AttributeObjectIdAssociation::operator = (const AttributeObjectIdAssociation &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_associatedTo = attribute.m_associatedTo;
    m_canBeEmpty   = attribute.m_canBeEmpty;

    return (*this);
}

string AttributeObjectIdAssociation::getSqlType ()
{
    return ("integer");
}

void AttributeObjectIdAssociation::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addRelation (new OrmAssociation (getAttributeName (), m_associatedTo, ORM_RELATION_TYPE_ONE_TO_ONE, m_canBeEmpty));
}

string AttributeObjectIdAssociation::getSqlForCreate ()
{
    string attributeName = getAttributeName ();

    if (true == m_canBeEmpty)
    {
        return (attributeName + "ClassId integer, " + attributeName + "InstanceId bigint");
    }
    else
    {
        return (attributeName + "ClassId integer NOT NULL, " + attributeName + "InstanceId bigint NOT NULL");
    }
}

void AttributeObjectIdAssociation::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    if (true == m_canBeEmpty)
    {
        if (ObjectId::NullObjectId != (*m_pData))
        {
            sqlForInsert += string (",") + m_pData->getClassId () + string (",") + m_pData->getInstanceId ();

            sqlForPreValues += string (",") + getAttributeName () + "ClassId," + getAttributeName () + "InstanceId";
        }
    }
    else
    {
        sqlForInsert += string (",") + m_pData->getClassId () + string (",") + m_pData->getInstanceId ();

        sqlForPreValues += string (",") + getAttributeName () + "ClassId," + getAttributeName () + "InstanceId";
    }
}

void AttributeObjectIdAssociation::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    if (true == m_canBeEmpty)
    {
        if (ObjectId::NullObjectId != (*m_pData))
        {
            sqlForUpdate += "," + getAttributeName () + string ("ClassId = ") + (m_pData->getClassId ()) + ", " + (getAttributeName ()) + string ("InstanceId = ") + (m_pData->getInstanceId ());
        }
        else
        {
            sqlForUpdate += "," + getAttributeName () + string ("ClassId = NULL, ") + (getAttributeName ()) + string ("InstanceId = NULL");
        }
    }
    else
    {
        sqlForUpdate += "," + getAttributeName () + string ("ClassId = ") + (m_pData->getClassId ()) + ", " + (getAttributeName ()) + string ("InstanceId = ") + (m_pData->getInstanceId ());
    }
}

void AttributeObjectIdAssociation::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeObjectIdAssociation::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NULL == attributeConditionOperator)
    {
        sqlForSelect += getAttributeName () + string ("ClassId IS NULL AND ") + getAttributeName () + string ("InstanceId IS NULL ");
    }
    else if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_IS_NOT_NULL == attributeConditionOperator)
    {
        sqlForSelect += getAttributeName () + string ("ClassId IS NOT NULL AND ") + getAttributeName () + string ("InstanceId IS NOT NULL ");
    }
    else
    {
        sqlForSelect += getAttributeName () + string ("ClassId = ") + (m_pData->getClassId ()) + " AND " + (getAttributeName ()) + string ("InstanceId = ") + (m_pData->getInstanceId ());
    }
}

Attribute *AttributeObjectIdAssociation::clone ()
{
    AttributeObjectIdAssociation *pAttributeObjectIdAssociation = new AttributeObjectIdAssociation (*this);

    return (pAttributeObjectIdAssociation);
}

string AttributeObjectIdAssociation::getAssociatedTo () const
{
    return (m_associatedTo);
}
        
bool AttributeObjectIdAssociation::getCanBeEmpty () const
{
    return (m_canBeEmpty);
}

void AttributeObjectIdAssociation::getCValue (WaveCValue *pCValue)
{
    // getCValue is not supported to relationnal data types
    prismAssert (false, __FILE__, __LINE__);
}

    AttributeObjectIdVector::AttributeObjectIdVector (const vector<ObjectId> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeObjectIdVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<ObjectId>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeObjectIdVector::AttributeObjectIdVector (const vector<ObjectId> &data, const vector<ObjectId> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeObjectIdVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<ObjectId>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeObjectIdVector::AttributeObjectIdVector (vector<ObjectId> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeObjectIdVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeObjectIdVector::AttributeObjectIdVector (vector<ObjectId> *pData, const vector<ObjectId> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeObjectIdVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeObjectIdVector::AttributeObjectIdVector (const AttributeObjectIdVector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<ObjectId>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeObjectIdVector::~AttributeObjectIdVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeObjectIdVector &AttributeObjectIdVector::operator = (const AttributeObjectIdVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<ObjectId> AttributeObjectIdVector::getValue () const
{
    vector<ObjectId> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeObjectIdVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeObjectIdVector::setValue (const vector<ObjectId> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeObjectIdVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeObjectIdVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeObjectIdVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeObjectIdVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeObjectIdVector::getSqlType ()
{
    return ("varchar");
}

void AttributeObjectIdVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeObjectIdVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeObjectIdVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeObjectIdVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeObjectIdVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeObjectIdVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeObjectIdVector::toString (string &valueString)
{
    UI32 numberOfObjectIds = m_pData->size ();
    UI32 i                 = 0;
    char tempBuffer[64] = {0};

    valueString = "";

    for (i = 0; i < numberOfObjectIds; i++)
    {
        snprintf (tempBuffer, 64, "#%u*%llu", (*m_pData)[i].getClassId (), (*m_pData)[i].getInstanceId ());

        valueString += tempBuffer;
    }
}

void AttributeObjectIdVector::fromString (const string &valueString)
{
    vector<ObjectId>::iterator begin = (*m_pData).begin ();
    vector<ObjectId>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI startFromIndex = 0;
    ULI firstIndex     = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        UI32 classId    = 0;
        UI64 instanceId = 0;

        sscanf (valueString.c_str () + firstIndex + 1, "%u*%llu", &classId, &instanceId);
        (*m_pData).push_back (ObjectId (classId, instanceId));

        startFromIndex = firstIndex + 1;
    }
}

void *AttributeObjectIdVector::getPData ()
{
    return (m_pData);
}

void AttributeObjectIdVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<ObjectId> *> (pData));
}

Attribute *AttributeObjectIdVector::clone ()
{
    AttributeObjectIdVector *pAttributeObjectIdVector = new AttributeObjectIdVector (*this);

    return (pAttributeObjectIdVector);
}

map<string, string> AttributeObjectIdVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_OBJECTID"] = "WAVE_ATTRIBUTE_TYPE_OBJECTID";

    return (supportedConversions);
}

bool AttributeObjectIdVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeObjectIdVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<ObjectId> AttributeObjectIdVector::getDefaultData() const
{
    return (m_defaultData);
}

void AttributeObjectIdVector::setDefaultValue()
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

void AttributeObjectIdVector::getCValue (WaveCValue *pCValue)
{
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeObjectIdVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {   
        m_pData = new vector<ObjectId>;
    }

    vector<ObjectId> vectorToAppend = (dynamic_cast<AttributeObjectIdVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeObjectIdVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<ObjectId>::iterator iterOriginal = (*m_pData).begin ();
    vector<ObjectId> vectorToDelete = (dynamic_cast<AttributeObjectIdVector *> (attribute))->getValue ();

    vector<ObjectId>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {
            (*m_pData).erase (iterOriginal);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("AttributeObjectIdVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

    AttributeObjectIdVectorAssociation::AttributeObjectIdVectorAssociation (const vector<ObjectId> &data, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &associatedTo, const UI32 &attributeUserTag, const bool &isOperational, const bool &outputAsString)
: AttributeObjectIdVector (data, attributeName, attributeUserTag, isOperational),
    m_parent                (parent),
    m_parentObjectId        (parentObjectId),
    m_associatedTo          (associatedTo),
    m_outputAsString        (outputAsString)
{
}

    AttributeObjectIdVectorAssociation::AttributeObjectIdVectorAssociation (vector<ObjectId> *pData, const string &attributeName, const string &parent, const ObjectId &parentObjectId, const string &associatedTo, const UI32 &attributeUserTag, const bool &isOperational, const bool &outputAsString)
: AttributeObjectIdVector (pData, attributeName, attributeUserTag, isOperational),
    m_parent                (parent),
    m_parentObjectId        (parentObjectId),
    m_associatedTo          (associatedTo),
    m_outputAsString        (outputAsString)
{
}

    AttributeObjectIdVectorAssociation::AttributeObjectIdVectorAssociation (const AttributeObjectIdVectorAssociation &attribute)
: AttributeObjectIdVector (attribute)
{
    m_parent               = attribute.m_parent;
    m_parentObjectId       = attribute.m_parentObjectId;
    m_associatedTo         = attribute.m_associatedTo;
    m_associationTableName = attribute.m_associationTableName;
    m_outputAsString       = attribute.m_outputAsString;
}

AttributeObjectIdVectorAssociation::~AttributeObjectIdVectorAssociation ()
{
}

AttributeObjectIdVectorAssociation &AttributeObjectIdVectorAssociation::operator = (const AttributeObjectIdVectorAssociation &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_parent               = attribute.m_parent;
    m_parentObjectId       = attribute.m_parentObjectId;
    m_associatedTo         = attribute.m_associatedTo;
    m_associationTableName = attribute.m_associationTableName;
    m_outputAsString       = attribute.m_outputAsString;

    return (*this);
}

string AttributeObjectIdVectorAssociation::getSqlType ()
{
    return ("varchar");
}

void AttributeObjectIdVectorAssociation::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addRelation (new OrmAssociation (getAttributeName (), m_associatedTo, ORM_RELATION_TYPE_ONE_TO_MANY));
}

string AttributeObjectIdVectorAssociation::getSqlForCreate ()
{
    string sqlForCreate2;
    UI32   parentTableId    = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId = OrmRepository::getTableId (m_associatedTo);
    string schema           = OrmRepository::getWaveCurrentSchema ();

    m_associationTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    sqlForCreate2 += string ("CREATE TABLE ") + m_associationTableName + "\n";
    sqlForCreate2 += "(\n";
    sqlForCreate2 += "    ownerClassId integer REFERENCES " + schema + "." + m_parent + " (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
    sqlForCreate2 += "    ownerInstanceId bigint REFERENCES " + schema + "." + m_parent + " (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED,\n";
    sqlForCreate2 += "    relatedToClassId integer REFERENCES " + schema + "." + m_associatedTo + " (objectIdClassId) DEFERRABLE INITIALLY DEFERRED,\n";
    sqlForCreate2 += "    relatedToInstanceId bigint REFERENCES " + schema + "." + m_associatedTo + " (objectIdInstanceId) DEFERRABLE INITIALLY DEFERRED\n";
    sqlForCreate2 += ");";

    return (sqlForCreate2);
}

void AttributeObjectIdVectorAssociation::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    UI32   numberOfObjectIdsToInsert = m_pData->size ();
    UI32   i                         = 0;
    UI32   parentTableId             = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId          = OrmRepository::getTableId (m_associatedTo);

    m_associationTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    string   insertPrefix              = string ("INSERT INTO ") + OrmRepository::getWaveCurrentSchema () + "." + m_associationTableName + string (" VALUES (") + m_parentObjectId.getClassId () + string (", ") + m_parentObjectId.getInstanceId () + string (", ");
    string   insertPostfix             = ");\n";
    ObjectId tempObjectId;

    for (i = 0; i < numberOfObjectIdsToInsert; i++)
    {
        tempObjectId = (*m_pData)[i];

        sqlForInsert2 += insertPrefix + tempObjectId.getClassId () + string (", ") + tempObjectId.getInstanceId () + insertPostfix;
    }
}

void AttributeObjectIdVectorAssociation::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string sqlForPreValues;
    string sqlForInsert;
    UI32   parentTableId    = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId = OrmRepository::getTableId (m_associatedTo);

    m_associationTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    sqlForUpdate2 += "DELETE FROM " + OrmRepository::getWaveCurrentSchema () + "." + m_associationTableName + string (" WHERE ownerClassId = ") + m_parentObjectId.getClassId () + string (" AND ownerInstanceId = ") + m_parentObjectId.getInstanceId () + string (";");
    getSqlForInsert (sqlForPreValues, sqlForInsert, sqlForUpdate2, isFirst);
}

void AttributeObjectIdVectorAssociation::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    trace (TRACE_LEVEL_FATAL, "AttributeObjectIdVectorAssociation::getSqlForSelect : AttributeObjectIdVectorAssociation must not be added via addAndAttribute/addOrAttribute member functions.  It msut be added via addAttributeObjectIdVectorAssociation.");
    prismAssert (false, __FILE__, __LINE__);
}

void AttributeObjectIdVectorAssociation::getSqlForSelect2 (string &associationTableName, string &whereClause)
{
    UI32     parentTableId             = OrmRepository::getTableId (m_parent);
    UI32     relatedToTableId          = OrmRepository::getTableId (m_associatedTo);
    ObjectId tempObjectId;
    UI32     i                         = 0;
    UI32     numberOfObjectIdsToSelect = m_pData->size ();

    m_associationTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    associationTableName = m_associationTableName;

    for (i = 0; i < numberOfObjectIdsToSelect; i++)
    {
        tempObjectId = (*m_pData)[i];

        whereClause += string ("( relatedToClassId = ") + tempObjectId.getClassId () + string (" AND relatedToInstanceId = ") + tempObjectId.getInstanceId () + string (")");

        if (i != (numberOfObjectIdsToSelect - 1))
        {
            whereClause += " OR ";
        }
    }
}

bool AttributeObjectIdVectorAssociation::getOutputAsString (void) const
{
    return (m_outputAsString);
}

void AttributeObjectIdVectorAssociation::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
}

void AttributeObjectIdVectorAssociation::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    UI32   parentTableId    = OrmRepository::getTableId (m_parent);
    UI32   relatedToTableId = OrmRepository::getTableId (m_associatedTo);

    // This is a special case.  Set the parent ObjectId explicitly.  In normal cases, this would have been set during the call to prepareForPersistence.  However,
    // in case of queried objects the object id for parent at that time will be null object id.

    m_parentObjectId = parentObjectId;

    m_associationTableName = string ("t") + parentTableId + getAttributeName () + string ("t") + relatedToTableId;

    PGresult *pResult = auxilliaryResultsMap[m_associationTableName];

    prismAssert (NULL != pResult, __FILE__, __LINE__);

    UI32  numberOfResults     = PQntuples (pResult);
    UI32  i                   = 0;
    UI32  columnForClassId    = PQfnumber (pResult, "relatedToClassId");
    UI32  columnForInstanceId = PQfnumber (pResult, "relatedToInstanceId");
    UI32  classId             = 0;
    UI64  instanceId          = 0;
    char *pValueString        = NULL;

    for (i = 0; i < numberOfResults; i++)
    {
        pValueString = PQgetvalue (pResult, i, columnForClassId);
        classId      = strtoul (pValueString, NULL, 10);
        pValueString = PQgetvalue (pResult, i, columnForInstanceId);
        instanceId   = strtoull (pValueString, NULL, 10);

        (*m_pData).push_back (ObjectId (classId, instanceId));
    }
}

Attribute *AttributeObjectIdVectorAssociation::clone ()
{
    AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation = new AttributeObjectIdVectorAssociation (*this);

    return (pAttributeObjectIdVectorAssociation);
}

void AttributeObjectIdVectorAssociation::getCValue (WaveCValue *pCValue)
{
    // getCValue is not supported to relationnal data types
    prismAssert (false, __FILE__, __LINE__);
}

string AttributeObjectIdVectorAssociation::getAssociatedTo () const
{
    return (m_associatedTo);
}

void AttributeObjectIdVectorAssociation::storeRelatedObjectIdVector (const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds)
{
    m_parentObjectId = parentObjectId;
   *m_pData          = vectorOfRelatedObjectIds;
}

    AttributeWorldWideName::AttributeWorldWideName (const WorldWideName &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeWorldWideName, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new WorldWideName;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeWorldWideName::AttributeWorldWideName (const WorldWideName &data, const WorldWideName &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeWorldWideName, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new WorldWideName;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeWorldWideName::AttributeWorldWideName (WorldWideName *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeWorldWideName, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeWorldWideName::AttributeWorldWideName (WorldWideName *pData, const WorldWideName &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeWorldWideName, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeWorldWideName::AttributeWorldWideName (const AttributeWorldWideName &attribute)
: Attribute (attribute)
{
    m_pData              = new WorldWideName;
    *m_pData             = attribute.getValue ();

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeWorldWideName::~AttributeWorldWideName ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeWorldWideName &AttributeWorldWideName::operator = (const AttributeWorldWideName &attribute)
{
    Attribute::operator = (attribute);

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setValue (attribute.getValue ());

    return (*this);
}

WorldWideName AttributeWorldWideName::getValue () const
{
    WorldWideName tempWorlWideName;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeWorldWideName::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempWorlWideName);
    }
}

void AttributeWorldWideName::setValue (const WorldWideName &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeWorldWideName::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeWorldWideName::validate () const
{
    prismAssert ( AttributeType::AttributeTypeWorldWideName == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeWorldWideName == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeWorldWideName::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE  == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeWorldWideName::getSqlType ()
{
    return ("varchar");
}

void AttributeWorldWideName::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeWorldWideName::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeWorldWideName::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeWorldWideName::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeWorldWideName::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeWorldWideName::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeWorldWideName::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeWorldWideName::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeWorldWideName::loadFromPlainString (const string &valueString)
{
    return  (m_pData->loadFromPlainString (valueString));
}

void *AttributeWorldWideName::getPData ()
{
    return (m_pData);
}

void AttributeWorldWideName::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const WorldWideName *> (pData));
}

Attribute *AttributeWorldWideName::clone ()
{
    AttributeWorldWideName *pAttributeWorldWideName = new AttributeWorldWideName (*this);

    return (pAttributeWorldWideName);
}

map<string, string> AttributeWorldWideName::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    return (supportedConversions);
}

bool AttributeWorldWideName::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeWorldWideName::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

WorldWideName AttributeWorldWideName::getDefaultData() const
{
    return (m_defaultData);
}

void AttributeWorldWideName::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        WorldWideName tempData;
        setValue(&tempData);
    }
}

void AttributeWorldWideName::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_world_wide_name (pCValue, (getValue ()).toString ());
}

    AttributeWorldWideNameVector::AttributeWorldWideNameVector (const vector<WorldWideName> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeWorldWideNameVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<WorldWideName>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeWorldWideNameVector::AttributeWorldWideNameVector (const vector<WorldWideName> &data, const vector<WorldWideName> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeWorldWideNameVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<WorldWideName>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeWorldWideNameVector::AttributeWorldWideNameVector (vector<WorldWideName> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeWorldWideNameVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeWorldWideNameVector::AttributeWorldWideNameVector (vector<WorldWideName> *pData, const vector<WorldWideName> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeWorldWideNameVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeWorldWideNameVector::AttributeWorldWideNameVector (const AttributeWorldWideNameVector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<WorldWideName>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeWorldWideNameVector::~AttributeWorldWideNameVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeWorldWideNameVector &AttributeWorldWideNameVector::operator = (const AttributeWorldWideNameVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<WorldWideName> AttributeWorldWideNameVector::getValue () const
{
    vector<WorldWideName> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeWorldWideNameVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeWorldWideNameVector::setValue (const vector<WorldWideName> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeWorldWideNameVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeWorldWideNameVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeWorldWideNameVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeWorldWideNameVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeWorldWideNameVector::getSqlType ()
{
    return ("varchar");
}

void AttributeWorldWideNameVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeWorldWideNameVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeWorldWideNameVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeWorldWideNameVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeWorldWideNameVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeWorldWideNameVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeWorldWideNameVector::toString (string &valueString)
{
    vector<WorldWideName>::iterator element = (*m_pData).begin ();
    vector<WorldWideName>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeWorldWideNameVector::fromString (const string &valueString)
{
    vector<WorldWideName>::iterator begin = (*m_pData).begin ();
    vector<WorldWideName>::iterator end   = (*m_pData).end ();

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

void *AttributeWorldWideNameVector::getPData ()
{
    return (m_pData);
}

void AttributeWorldWideNameVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<WorldWideName> *> (pData));
}

Attribute *AttributeWorldWideNameVector::clone ()
{
    AttributeWorldWideNameVector *pAttributeWorldWideNameVector = new AttributeWorldWideNameVector (*this);

    return (pAttributeWorldWideNameVector);
}

map<string, string> AttributeWorldWideNameVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";

    return (supportedConversions);

}

bool AttributeWorldWideNameVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<WorldWideName> AttributeWorldWideNameVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeWorldWideNameVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeWorldWideNameVector::setDefaultValue()
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

void AttributeWorldWideNameVector::getCValue (WaveCValue *pCValue)
{
    vector<string> value;
    vector<WorldWideName>::iterator iter ;
    vector<WorldWideName> data = getValue ();

    for(iter = data.begin (); iter != data.end (); iter++)
    {
        value.push_back(iter->toString());
    }

    wave_cvalue_set_world_wide_name_vector (pCValue, value);
}

void AttributeWorldWideNameVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {   
        m_pData = new vector<WorldWideName>;
    }

    vector<WorldWideName> vectorToAppend = (dynamic_cast<AttributeWorldWideNameVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeWorldWideNameVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<WorldWideName>::iterator iterOriginal = (*m_pData).begin ();
    vector<WorldWideName> vectorToDelete = (dynamic_cast<AttributeWorldWideNameVector *> (attribute))->getValue ();

    vector<WorldWideName>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {
            (*m_pData).erase (iterOriginal);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("AttributeWorldWideNameVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

    AttributeMacAddress::AttributeMacAddress (const MacAddress &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new MacAddress;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddress::AttributeMacAddress (const MacAddress &data, const MacAddress &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new MacAddress;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddress::AttributeMacAddress (MacAddress *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddress::AttributeMacAddress (MacAddress *pData, const MacAddress &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddress::AttributeMacAddress (const AttributeMacAddress &attribute)
: Attribute (attribute)
{
    m_pData              = new MacAddress;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeMacAddress::~AttributeMacAddress ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeMacAddress &AttributeMacAddress::operator = (const AttributeMacAddress &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

MacAddress AttributeMacAddress::getValue () const
{
    MacAddress tempMacAddress;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempMacAddress);
    }
}

void AttributeMacAddress::setValue (const MacAddress &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeMacAddress::validate () const
{
    prismAssert ( AttributeType::AttributeTypeMacAddress == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeMacAddress == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeMacAddress::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE  == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeMacAddress::getSqlType ()
{
    return ("varchar");
}

void AttributeMacAddress::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeMacAddress::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeMacAddress::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeMacAddress::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeMacAddress::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeMacAddress::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeMacAddress::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeMacAddress::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeMacAddress::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeMacAddress::getPData ()
{
    return (m_pData);
}

void AttributeMacAddress::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const MacAddress *> (pData));
}

Attribute *AttributeMacAddress::clone ()
{
    AttributeMacAddress *pAttributeMacAddress = new AttributeMacAddress (*this);

    return (pAttributeMacAddress);
}

map<string, string> AttributeMacAddress::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";

    return (supportedConversions);
}

bool AttributeMacAddress::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

MacAddress AttributeMacAddress::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeMacAddress::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeMacAddress::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        MacAddress tempData;
        setValue(&tempData);
    }
}

void AttributeMacAddress::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_macaddress( pCValue, (getValue ()).toString());
}

    AttributeMacAddressVector::AttributeMacAddressVector (const vector<MacAddress> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<MacAddress>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddressVector::AttributeMacAddressVector (const vector<MacAddress> &data, const vector<MacAddress> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<MacAddress>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddressVector::AttributeMacAddressVector (vector<MacAddress> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddressVector::AttributeMacAddressVector (vector<MacAddress> *pData, const vector<MacAddress> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddressVector::AttributeMacAddressVector (const AttributeMacAddressVector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<MacAddress>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeMacAddressVector::~AttributeMacAddressVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeMacAddressVector &AttributeMacAddressVector::operator = (const AttributeMacAddressVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

vector<MacAddress> AttributeMacAddressVector::getValue () const
{
    vector<MacAddress> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddressVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeMacAddressVector::setValue (const vector<MacAddress> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddressVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeMacAddressVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeMacAddressVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeMacAddressVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeMacAddressVector::getSqlType ()
{
    return ("varchar");
}

void AttributeMacAddressVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeMacAddressVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeMacAddressVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeMacAddressVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeMacAddressVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeMacAddressVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeMacAddressVector::toString (string &valueString)
{
    vector<MacAddress>::iterator element = (*m_pData).begin ();
    vector<MacAddress>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeMacAddressVector::fromString (const string &valueString)
{
    vector<MacAddress>::iterator begin = (*m_pData).begin ();
    vector<MacAddress>::iterator end   = (*m_pData).end ();

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

void *AttributeMacAddressVector::getPData ()
{
    return (m_pData);
}

void AttributeMacAddressVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<MacAddress> *> (pData));
}

Attribute *AttributeMacAddressVector::clone ()
{
    AttributeMacAddressVector *pAttributeMacAddressVector = new AttributeMacAddressVector (*this);

    return (pAttributeMacAddressVector);
}

map<string, string> AttributeMacAddressVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2";

    return (supportedConversions);

}

bool AttributeMacAddressVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<MacAddress> AttributeMacAddressVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeMacAddressVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeMacAddressVector::setDefaultValue()
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

void AttributeMacAddressVector::getCValue (WaveCValue *pCValue)
{
    vector<string> value;
    vector<MacAddress>::iterator iter ;
    vector<MacAddress> data = getValue ();

    for(iter = data.begin (); iter != data.end (); iter++)
    {
        value.push_back(iter->toString ());
    }

    wave_cvalue_set_macaddress_vector (pCValue, value);
}

void AttributeMacAddressVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {   
        m_pData = new vector<MacAddress>;
    }

    vector<MacAddress> vectorToAppend = (dynamic_cast<AttributeMacAddressVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeMacAddressVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<MacAddress>::iterator iterOriginal = (*m_pData).begin ();

    vector<MacAddress> vectorToDelete = (dynamic_cast<AttributeMacAddressVector *> (attribute))->getValue ();
    vector<MacAddress>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {
            (*m_pData).erase (iterOriginal);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddressVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}


    AttributeMacAddress2::AttributeMacAddress2 (const MacAddress2 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress2, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new MacAddress2;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddress2::AttributeMacAddress2 (const MacAddress2 &data, const MacAddress2 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress2, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new MacAddress2;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddress2::AttributeMacAddress2 (MacAddress2 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress2, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddress2::AttributeMacAddress2 (MacAddress2 *pData, const MacAddress2 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute ( AttributeType::AttributeTypeMacAddress2, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddress2::AttributeMacAddress2 (const AttributeMacAddress2 &attribute)
: Attribute (attribute)
{
    m_pData              = new MacAddress2;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeMacAddress2::~AttributeMacAddress2 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeMacAddress2 &AttributeMacAddress2::operator = (const AttributeMacAddress2 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

MacAddress2 AttributeMacAddress2::getValue () const
{
    MacAddress2 tempMacAddress2;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress2::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempMacAddress2);
    }
}

void AttributeMacAddress2::setValue (const MacAddress2 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress2::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeMacAddress2::validate () const
{
    prismAssert ( AttributeType::AttributeTypeMacAddress2 == (getAttributeType ()), __FILE__, __LINE__);

    return ( AttributeType::AttributeTypeMacAddress2 == (getAttributeType ()));
}

bool AttributeMacAddress2::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE  == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeMacAddress2::getSqlType ()
{
    return ("varchar");
}

void AttributeMacAddress2::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeMacAddress2::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeMacAddress2::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeMacAddress2::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeMacAddress2::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeMacAddress2::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeMacAddress2::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeMacAddress2::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeMacAddress2::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeMacAddress2::getPData ()
{
    return (m_pData);
}

void AttributeMacAddress2::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const MacAddress2 *> (pData));
}

Attribute *AttributeMacAddress2::clone ()
{
    AttributeMacAddress2 *pAttributeMacAddress2 = new AttributeMacAddress2 (*this);

    return (pAttributeMacAddress2);
}

map<string, string> AttributeMacAddress2::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";

    return (supportedConversions);
}

bool AttributeMacAddress2::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

MacAddress2 AttributeMacAddress2::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeMacAddress2::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeMacAddress2::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        MacAddress2 tempData;
        setValue(&tempData);
    }
}

void AttributeMacAddress2::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_macaddress2 (pCValue, (getValue ()).toString());
}

    AttributeMacAddress2Vector::AttributeMacAddress2Vector (const vector<MacAddress2> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddress2Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<MacAddress2>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddress2Vector::AttributeMacAddress2Vector (const vector<MacAddress2> &data, const vector<MacAddress2> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddress2Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<MacAddress2>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeMacAddress2Vector::AttributeMacAddress2Vector (vector<MacAddress2> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddress2Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddress2Vector::AttributeMacAddress2Vector (vector<MacAddress2> *pData, const vector<MacAddress2> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeMacAddress2Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeMacAddress2Vector::AttributeMacAddress2Vector (const AttributeMacAddress2Vector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<MacAddress2>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeMacAddress2Vector::~AttributeMacAddress2Vector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}
AttributeMacAddress2Vector &AttributeMacAddress2Vector::operator = (const AttributeMacAddress2Vector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

vector<MacAddress2> AttributeMacAddress2Vector::getValue () const
{
    vector<MacAddress2> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress2Vector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeMacAddress2Vector::setValue (const vector<MacAddress2> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress2Vector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeMacAddress2Vector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeMacAddress2Vector == (getAttributeType ()), __FILE__, __LINE__);

    return ( AttributeType::AttributeTypeMacAddress2Vector == (getAttributeType ()));
}

string AttributeMacAddress2Vector::getSqlType ()
{
    return ("varchar");
}

void AttributeMacAddress2Vector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeMacAddress2Vector::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeMacAddress2Vector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeMacAddress2Vector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeMacAddress2Vector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeMacAddress2Vector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeMacAddress2Vector::toString (string &valueString)
{
    vector<MacAddress2>::iterator element = (*m_pData).begin ();
    vector<MacAddress2>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeMacAddress2Vector::fromString (const string &valueString)
{
    vector<MacAddress2>::iterator begin = (*m_pData).begin ();
    vector<MacAddress2>::iterator end   = (*m_pData).end ();

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

void *AttributeMacAddress2Vector::getPData ()
{
    return (m_pData);
}

void AttributeMacAddress2Vector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<MacAddress2> *> (pData));
}

Attribute *AttributeMacAddress2Vector::clone ()
{
    AttributeMacAddress2Vector *pAttributeMacAddress2Vector = new AttributeMacAddress2Vector (*this);

    return (pAttributeMacAddress2Vector);
}

map<string, string> AttributeMacAddress2Vector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";

    return (supportedConversions);
}

bool AttributeMacAddress2Vector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<MacAddress2> AttributeMacAddress2Vector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeMacAddress2Vector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeMacAddress2Vector::setDefaultValue()
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

void AttributeMacAddress2Vector::getCValue (WaveCValue *pCValue)
{
    vector<string> value;
    vector<MacAddress2>::iterator iter ;
    vector<MacAddress2> data;

    for(iter = data.begin (); iter != data.end (); iter++)
    {
        value.push_back(iter->toString());
    }

    wave_cvalue_set_macaddress2_vector (pCValue, value);
}

void AttributeMacAddress2Vector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {   
        m_pData = new vector<MacAddress2>;
    }

    vector<MacAddress2> vectorToAppend = (dynamic_cast<AttributeMacAddress2Vector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeMacAddress2Vector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<MacAddress2>::iterator iterOriginal = (*m_pData).begin ();

    vector<MacAddress2> vectorToDelete = (dynamic_cast<AttributeMacAddress2Vector *> (attribute))->getValue ();
    vector<MacAddress2>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {
            (*m_pData).erase (iterOriginal);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("AttributeMacAddress2Vector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

AttributeIpV4Address::AttributeIpV4Address (const IpV4Address &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpV4Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        (""),
    m_isDefaultDataValid (false)
{
    m_pData  = new IpV4Address;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeIpV4Address::AttributeIpV4Address (const IpV4Address &data, const IpV4Address &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV4Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new IpV4Address;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4Address::AttributeIpV4Address (IpV4Address *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpV4Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        (""),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV4Address::AttributeIpV4Address (IpV4Address *pData, const IpV4Address &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV4Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV4Address::AttributeIpV4Address (const AttributeIpV4Address &attribute)
: Attribute (attribute)
{
    m_pData              = new IpV4Address;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4Address::~AttributeIpV4Address ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV4Address &AttributeIpV4Address::operator = (const AttributeIpV4Address &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

IpV4Address AttributeIpV4Address::getValue () const
{
    IpV4Address tempIpV4Addres;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4Address::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempIpV4Addres);
    }
}

void AttributeIpV4Address::setValue (const IpV4Address &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4Address::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpV4Address::validate () const
{
    prismAssert ( AttributeType::AttributeTypeIpV4Address == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeIpV4Address == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeIpV4Address::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE  == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeIpV4Address::getSqlType ()
{
    return ("inet");
}

void AttributeIpV4Address::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV4Address::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeIpV4Address::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    if (false == tempString.empty ())
    {
        sqlForInsert += string (",'") + tempString + string ("'");
    }
    else
    {
        sqlForInsert += string (",NULL");
    }

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeIpV4Address::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    if (false == tempString.empty ())
    {
        sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
    }
    else
    {
        sqlForUpdate += "," + getAttributeName () + " = " + string ("NULL");
    }
}

void AttributeIpV4Address::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV4Address::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV4Address::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeIpV4Address::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeIpV4Address::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeIpV4Address::getPData ()
{
    return (m_pData);
}

void AttributeIpV4Address::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const IpV4Address *> (pData));
}

Attribute *AttributeIpV4Address::clone ()
{
    AttributeIpV4Address *pAttributeIpV4Address = new AttributeIpV4Address (*this);

    return (pAttributeIpV4Address);
}

bool AttributeIpV4Address::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;
    
    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

IpV4Address AttributeIpV4Address::getDefaultData (void) const
{
    return (m_defaultData);
}

bool AttributeIpV4Address::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

void AttributeIpV4Address::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        IpV4Address tempData;
        setValue(&tempData);
    }
}

map<string, string> AttributeIpV4Address::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC";

    return (supportedConversions);
}

void AttributeIpV4Address::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_ipaddress (pCValue, (IpV4Address *) getPData () );
}

    AttributeIpV4AddressVector::AttributeIpV4AddressVector (const vector<IpV4Address> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV4AddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<IpV4Address>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeIpV4AddressVector::AttributeIpV4AddressVector (const vector<IpV4Address> &data, const vector<IpV4Address> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV4AddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<IpV4Address>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeIpV4AddressVector::AttributeIpV4AddressVector (vector<IpV4Address> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV4AddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV4AddressVector::AttributeIpV4AddressVector (vector<IpV4Address> *pData, const vector<IpV4Address> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV4AddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV4AddressVector::AttributeIpV4AddressVector (const AttributeIpV4AddressVector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<IpV4Address>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressVector::~AttributeIpV4AddressVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV4AddressVector &AttributeIpV4AddressVector::operator = (const AttributeIpV4AddressVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

vector<IpV4Address> AttributeIpV4AddressVector::getValue () const
{
    vector<IpV4Address> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4AddressVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeIpV4AddressVector::setValue (const vector<IpV4Address> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4AddressVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpV4AddressVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeIpV4AddressVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeIpV4AddressVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeIpV4AddressVector::getSqlType ()
{
    return ("varchar");
}

void AttributeIpV4AddressVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV4AddressVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeIpV4AddressVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeIpV4AddressVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeIpV4AddressVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV4AddressVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV4AddressVector::toString (string &valueString)
{
    vector<IpV4Address>::iterator element = (*m_pData).begin ();
    vector<IpV4Address>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeIpV4AddressVector::fromString (const string &valueString)
{
    vector<IpV4Address>::iterator begin = (*m_pData).begin ();
    vector<IpV4Address>::iterator end   = (*m_pData).end ();

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

void *AttributeIpV4AddressVector::getPData ()
{
    return (m_pData);
}

void AttributeIpV4AddressVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<IpV4Address> *> (pData));
}

Attribute *AttributeIpV4AddressVector::clone ()
{
    AttributeIpV4AddressVector *pAttributeIpV4AddressVector = new AttributeIpV4AddressVector (*this);

    return (pAttributeIpV4AddressVector);
}

map<string, string> AttributeIpV4AddressVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";

    return (supportedConversions);
}

bool AttributeIpV4AddressVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeIpV4AddressVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<IpV4Address> AttributeIpV4AddressVector::getDefaultData() const
{
    return (m_defaultData);
}

void AttributeIpV4AddressVector::setDefaultValue()
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

void AttributeIpV4AddressVector::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_ipv4address_vector (pCValue, getValue() );
}

void AttributeIpV4AddressVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {   
        m_pData = new vector<IpV4Address>;
    }

    vector<IpV4Address> vectorToAppend = (dynamic_cast<AttributeIpV4AddressVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeIpV4AddressVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<IpV4Address>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<IpV4Address> vectorToDelete = (dynamic_cast<AttributeIpV4AddressVector *> (attribute))->getValue ();
    vector<IpV4Address>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {    
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4AddressVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void AttributeIpV4AddressVector::getPlainString (string &valueString)
{
    vector<IpV4Address>::const_iterator itrElement = (*m_pData).begin ();
    vector<IpV4Address>::const_iterator endElement = (*m_pData).end ();

    bool separatorRequired = false;

    for (; endElement != itrElement; itrElement++)
    {
        if (false == separatorRequired)
        {
            separatorRequired = true;
        }
        else
        {
            valueString += " ";
        }

        valueString += (*itrElement).toString ();
    }
}

AttributeIpV6Address::AttributeIpV6Address (const IpV6Address &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpV6Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        ("0:0:0:0:0:0:0:1"),
    m_isDefaultDataValid (false)
{
    m_pData  = new IpV6Address;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeIpV6Address::AttributeIpV6Address (const IpV6Address &data, const IpV6Address &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV6Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new IpV6Address;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV6Address::AttributeIpV6Address (IpV6Address *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpV6Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        ("0:0:0:0:0:0:0:1"),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV6Address::AttributeIpV6Address (IpV6Address *pData, const IpV6Address &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV6Address, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV6Address::AttributeIpV6Address (const AttributeIpV6Address &attribute)
: Attribute (attribute)
{
    m_pData              = new IpV6Address;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV6Address::~AttributeIpV6Address ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV6Address &AttributeIpV6Address::operator = (const AttributeIpV6Address &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

IpV6Address AttributeIpV6Address::getValue () const
{
    IpV6Address tempIpV4Addres;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV6Address::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempIpV4Addres);
    }
}

void AttributeIpV6Address::setValue (const IpV6Address &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV6Address::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpV6Address::validate () const
{
    prismAssert ( AttributeType::AttributeTypeIpV6Address == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeIpV6Address == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeIpV6Address::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE      == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_LIKE  == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_ILIKE     == attributeConditionOperator ||
            WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_ILIKE == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeIpV6Address::getSqlType ()
{
    return ("inet");
}

void AttributeIpV6Address::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV6Address::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeIpV6Address::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    if (false == tempString.empty ())
    {
        sqlForInsert += string (",'") + tempString + string ("'");
    }
    else
    {
        sqlForInsert += string (",NULL");
    }

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeIpV6Address::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    if (false == tempString.empty ())
    {
        sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
    }
    else
    {
        sqlForUpdate += "," + getAttributeName () + " = " + string ("NULL");
    }
}

void AttributeIpV6Address::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV6Address::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV6Address::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeIpV6Address::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeIpV6Address::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeIpV6Address::getPData ()
{
    return (m_pData);
}

void AttributeIpV6Address::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const IpV6Address *> (pData));
}

Attribute *AttributeIpV6Address::clone ()
{
    AttributeIpV6Address *pAttributeIpV6Address = new AttributeIpV6Address (*this);

    return (pAttributeIpV6Address);
}

map<string, string> AttributeIpV6Address::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";

    return (supportedConversions);
}

bool AttributeIpV6Address::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeIpV6Address::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

IpV6Address AttributeIpV6Address::getDefaultData() const
{
    return (m_defaultData);
}

void AttributeIpV6Address::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        IpV6Address tempData;
        setValue(&tempData);
    }
}

void AttributeIpV6Address::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_ipaddress (pCValue, (IpV6Address *) getPData ()) ;
}

    AttributeIpV6AddressVector::AttributeIpV6AddressVector (const vector<IpV6Address> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV6AddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<IpV6Address>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeIpV6AddressVector::AttributeIpV6AddressVector (const vector<IpV6Address> &data, const vector<IpV6Address> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV6AddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<IpV6Address>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

    AttributeIpV6AddressVector::AttributeIpV6AddressVector (vector<IpV6Address> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV6AddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV6AddressVector::AttributeIpV6AddressVector (vector<IpV6Address> *pData, const vector<IpV6Address> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
: Attribute          ( AttributeType::AttributeTypeIpV6AddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

    AttributeIpV6AddressVector::AttributeIpV6AddressVector (const AttributeIpV6AddressVector &attribute)
: Attribute (attribute)
{
    m_pData              = new vector<IpV6Address>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV6AddressVector::~AttributeIpV6AddressVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV6AddressVector &AttributeIpV6AddressVector::operator = (const AttributeIpV6AddressVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

vector<IpV6Address> AttributeIpV6AddressVector::getValue () const
{
    vector<IpV6Address> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV6AddressVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeIpV6AddressVector::setValue (const vector<IpV6Address> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV6AddressVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpV6AddressVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeIpV6AddressVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeIpV6AddressVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeIpV6AddressVector::getSqlType ()
{
    return ("varchar");
}

void AttributeIpV6AddressVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV6AddressVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeIpV6AddressVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeIpV6AddressVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeIpV6AddressVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV6AddressVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV6AddressVector::toString (string &valueString)
{
    vector<IpV6Address>::iterator element = (*m_pData).begin ();
    vector<IpV6Address>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeIpV6AddressVector::fromString (const string &valueString)
{
    vector<IpV6Address>::iterator begin = (*m_pData).begin ();
    vector<IpV6Address>::iterator end   = (*m_pData).end ();

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

void *AttributeIpV6AddressVector::getPData ()
{
    return (m_pData);
}

void AttributeIpV6AddressVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<IpV6Address> *> (pData));
}

Attribute *AttributeIpV6AddressVector::clone ()
{
    AttributeIpV6AddressVector *pAttributeIpV6AddressVector = new AttributeIpV6AddressVector (*this);

    return (pAttributeIpV6AddressVector);
}

map<string, string> AttributeIpV6AddressVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK";

    return (supportedConversions);
}

bool AttributeIpV6AddressVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}


vector<IpV6Address> AttributeIpV6AddressVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeIpV6AddressVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeIpV6AddressVector::setDefaultValue()
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

void AttributeIpV6AddressVector::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_ipv6address_vector (pCValue, getValue ());
}

void AttributeIpV6AddressVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<IpV6Address>;        
    }

    vector<IpV6Address> vectorToAppend = (dynamic_cast<AttributeIpV6AddressVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeIpV6AddressVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<IpV6Address>::iterator iterOriginal = (*m_pData).begin ();

    vector<IpV6Address> vectorToDelete = (dynamic_cast<AttributeIpV6AddressVector *> (attribute))->getValue ();
    vector<IpV6Address>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("AttributeIpV6AddressVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

void AttributeIpV6AddressVector::getPlainString (string &valueString)
{
    vector<IpV6Address>::const_iterator itrElement = (*m_pData).begin ();
    vector<IpV6Address>::const_iterator endElement = (*m_pData).end ();

    bool separatorRequired = false;

    for (; endElement != itrElement; itrElement++)
    {
        if (false == separatorRequired)
        {
            separatorRequired = true;
        }
        else
        {
            valueString += " ";
        }

        valueString += (*itrElement).toString ();
    }
}

}
