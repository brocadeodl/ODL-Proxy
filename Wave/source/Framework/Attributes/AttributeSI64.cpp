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

#include "Framework/Attributes/AttributeSI64.h"
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

namespace WaveNs
{

AttributeSI64::AttributeSI64 (const SI64 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData  = new SI64;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI64::AttributeSI64 (const SI64 &data, const SI64 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
     m_pData = new SI64;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSI64::AttributeSI64 (SI64 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI64::AttributeSI64 (SI64 *pData, const SI64 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeSI64, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSI64::AttributeSI64 (const AttributeSI64 &attribute)
    : Attribute (attribute)
{
    m_pData              = new SI64;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeSI64::~AttributeSI64 ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSI64 &AttributeSI64::operator = (const AttributeSI64 &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

SI64 AttributeSI64::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI64::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeSI64::setValue (const SI64 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSI64::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeSI64::validate () const
{
    prismAssert (AttributeType::AttributeTypeSI64 == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeSI64 == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeSI64::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
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

string AttributeSI64::getSqlType ()
{
    return ("bigint");
}

void AttributeSI64::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSI64::getSqlForCreate ()
{
    return (getAttributeName () + " bigint");
}

void AttributeSI64::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeSI64::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void AttributeSI64::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSI64::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeSI64::toString (string &valueString)
{
    char buffer[128] = {0};

    snprintf (buffer, 128, "%lld", (long long int) (getValue ()));

    valueString = string (buffer);
}

void AttributeSI64::fromString (const string &valueString)
{
    setValue (strtoll (valueString.c_str (), NULL, 10));
}

ResourceId AttributeSI64::loadFromPlainString (const string &valueString)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    char*   firstInvalidCharacterPosition = NULL;

    SI64 value = strtoll (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("AttributeSI64::loadFromPlainString : Improper format of AttributeSI64 in string"));
    }

    return status ;
}

void *AttributeSI64::getPData ()
{
    return (m_pData);
}

void AttributeSI64::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const SI64 *> (pData));
}

Attribute *AttributeSI64::clone ()
{
    AttributeSI64 *pAttributeSI64 = new AttributeSI64 (*this);

    return (pAttributeSI64);
}

map<string, string> AttributeSI64::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64"] = "WAVE_ATTRIBUTE_TYPE_UI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";

    return (supportedConversions);
}

bool AttributeSI64::isCurrentValueSameAsDefault () const
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

bool AttributeSI64::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

SI64 AttributeSI64::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeSI64::setDefaultValue()
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

void AttributeSI64::getCValue(WaveCValue *pCValue)
{
    wave_cvalue_set_si64 (pCValue, getValue ());
}


}

