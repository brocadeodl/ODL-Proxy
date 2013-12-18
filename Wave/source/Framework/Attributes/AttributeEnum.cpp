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
 *   Copyright (C) 2010        Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
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

namespace WaveNs
{
AttributeEnum::AttributeEnum (const UI32 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeEnum, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData  = new UI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeEnum::AttributeEnum (const UI32 &data, const UI32 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeEnum, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new UI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeEnum::AttributeEnum (UI32 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeEnum, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeEnum::AttributeEnum (UI32 *pData, const UI32 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeEnum, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeEnum::AttributeEnum (const AttributeEnum &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI32;
    *m_pData             = attribute.getValue ();

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeEnum::~AttributeEnum ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeEnum &AttributeEnum::operator = (const AttributeEnum &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI32 AttributeEnum::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeEnum::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeEnum::setValue (const UI32 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeEnum::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeEnum::validate () const
{
    prismAssert (AttributeType::AttributeTypeEnum == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeEnum == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeEnum::getSqlType ()
{
    return ("integer");
}

void AttributeEnum::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeEnum::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeEnum::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString = (Integer (0xFFFFFFFF & (getValue ()))).toString ();

    sqlForInsert += string (",") + tempString;

    sqlForPreValues += string (",") + getAttributeName ();
}

void  AttributeEnum::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString = (Integer (0xFFFFFFFF & (getValue ()))).toString ();

    sqlForUpdate += "," + getAttributeName () + " = " + tempString;
}

void  AttributeEnum::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeEnum::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString = (Integer (0xFFFFFFFF & (getValue ()))).toString ();

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + tempString;
}

void AttributeEnum::toString (string &valueString)
{
    char buffer[64] = {0};

    snprintf (buffer, 64, "%u", (unsigned int) (getValue ()));

    valueString = string (buffer);
}

void AttributeEnum::fromString (const string &valueString)
{
    setValue (strtol (valueString.c_str (), NULL, 10));
}

ResourceId AttributeEnum::loadFromPlainString (const string &valueString)
{
    char *firstInvalidCharacterPosition = NULL;
    ResourceId status = WAVE_MESSAGE_ERROR;

    UI32 value = strtol (valueString.c_str (), &(firstInvalidCharacterPosition), 10);

    if( *firstInvalidCharacterPosition == '\0')
    {
        setValue (value);
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "AttributeEnum::loadFromPlainString invalid value string");
    }

    return status;
}

void *AttributeEnum::getPData ()
{
    return (m_pData);
}

void AttributeEnum::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI32 *> (pData));
}

Attribute *AttributeEnum::clone ()
{
    AttributeEnum *pAttributeEnum = new AttributeEnum (*this);

    return (pAttributeEnum);
}

UI32 AttributeEnum::getDefaultData (void) const
{
    return (m_defaultData);
}

bool AttributeEnum::isCurrentValueSameAsDefault () const
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

bool AttributeEnum::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

void AttributeEnum::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(& m_defaultData);
    }
    else
    {
        *m_pData = 0;
    }
}

map<string, string> AttributeEnum::getSupportedConversions ()
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
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] ="WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";
 
    return (supportedConversions);
}

void AttributeEnum::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_ui32 (pCValue, getValue ());
}



void AttributeEnum::populateNameValueEnumMap (string name, UI32 value)
{
    // Assertion checks begin   [
    map<string, UI32>::iterator itr1    = m_NameToValueMap.find (name);
    map<string, UI32>::iterator end1    = m_NameToValueMap.end ();
    prismAssert (end1 == itr1, __FILE__, __LINE__);

    map <UI32, string>::iterator itr2   = m_ValueToNameMap.find (value);
    map <UI32, string>::iterator end2   = m_ValueToNameMap.end ();
    prismAssert (end2 == itr2, __FILE__, __LINE__);
    // Assertion checks end     ]

    m_NameToValueMap[name]  = value;
    m_ValueToNameMap[value] = name;
}

ResourceId AttributeEnum::getValueFromEnumName (const string &name, UI32 &value)
{
    map<string,UI32>::iterator it;
    
    it = m_NameToValueMap.find (name);

    if (it != m_NameToValueMap.end())
    {
        value = it->second;

        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        return WAVE_MESSAGE_ERROR;
    }
}

ResourceId AttributeEnum::getEnumNameFromValue (string &name, const UI32 &value)
{
    map <UI32, string>::iterator it;
    
    it = m_ValueToNameMap.find (value);

    if (it != m_ValueToNameMap.end())
    {
        name = it->second;
        // name = "ENUM:" + name;

        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        return WAVE_MESSAGE_ERROR;
    }
}


}

