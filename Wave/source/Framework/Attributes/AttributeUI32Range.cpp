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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Attributes/AttributeUI32Range.h"
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

AttributeUI32Range::AttributeUI32Range (const UI32Range &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeUI32Range, attributeName, attributeUserTag, isOperational),
    m_defaultData        ("0"),
    m_isDefaultDataValid (false)
{
     m_pData = new UI32Range;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32Range::AttributeUI32Range (const UI32Range &data, const UI32Range &DefaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           (AttributeType::AttributeTypeUI32Range, attributeName, attributeUserTag, isOperational),
     m_defaultData        (DefaultData),
     m_isDefaultDataValid ( true)
{
     m_pData = new UI32Range;
    *m_pData = data;
     setIsMemoryOwnedByAttribute (true);
}


AttributeUI32Range::AttributeUI32Range (UI32Range *pData,const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeUI32Range, attributeName, attributeUserTag, isOperational),
    m_defaultData        ("0"),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32Range::AttributeUI32Range (UI32Range *pData,const UI32Range &DefaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           (AttributeType::AttributeTypeUI32Range, attributeName, attributeUserTag, isOperational),
     m_defaultData        (DefaultData),
     m_isDefaultDataValid (true)
{
    m_pData = pData;
    setIsMemoryOwnedByAttribute (false);
}


AttributeUI32Range::AttributeUI32Range (const AttributeUI32Range &attribute)
    : Attribute (attribute)
{
     m_pData              = new UI32Range;
    *m_pData              = attribute.getValue ();

     m_defaultData        = attribute.getDefaultData ();
     m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

     setIsMemoryOwnedByAttribute (true);
}

AttributeUI32Range::~AttributeUI32Range ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI32Range &AttributeUI32Range::operator = (const AttributeUI32Range &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI32Range AttributeUI32Range::getValue () const
{
    UI32Range tempUI32Range;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32Range::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempUI32Range;
    }
}

void AttributeUI32Range::setValue (const UI32Range &data)
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

bool AttributeUI32Range::validate () const
{
    prismAssert (AttributeType::AttributeTypeUI32Range == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeUI32Range == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUI32Range::getSqlType ()
{
    return ("varchar");
}

void AttributeUI32Range::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI32Range::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeUI32Range::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI32Range::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI32Range::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI32Range::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI32Range::getSqlForSelectUsingRange (string &sqlForSelect)
{
    string tempString;

    toString (tempString);

    vector<string>  firstLevelToken;
    vector<string>  secondLevelToken;
    UI32            countFirstLevelToken   = 0;
    UI32            countSecondLevelToken = 0;

    tokenize (tempString, firstLevelToken, ',');
    countFirstLevelToken = firstLevelToken.size ();

    for (UI32 i = 0; i < countFirstLevelToken; i++)
    {
        tokenize (firstLevelToken [i], secondLevelToken, '-');

        sqlForSelect += "(";

        countSecondLevelToken = secondLevelToken.size ();

        for (UI32 j = 0; j < countSecondLevelToken; j++)
        {
            if (1 == countSecondLevelToken)
            {
                sqlForSelect += getAttributeName () + " = " + secondLevelToken [0];
            }
            else if (2 == countSecondLevelToken)
            {
                sqlForSelect += getAttributeName () + " >= " + secondLevelToken [0] + " AND " + getAttributeName () + " <= " +  secondLevelToken [1];
                break;
            }
            else
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }

        sqlForSelect += ")";

        if ((i + 1) < countFirstLevelToken)
        {
            sqlForSelect += " OR ";
        }
    }
}

void AttributeUI32Range::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeUI32Range::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

ResourceId AttributeUI32Range::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString(valueString));
}

void *AttributeUI32Range::getPData ()
{
    return (m_pData);
}

void AttributeUI32Range::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI32Range *> (pData));
}

Attribute *AttributeUI32Range::clone ()
{
    AttributeUI32Range *pAttributeUI32Range = new AttributeUI32Range (*this);

    return (pAttributeUI32Range);
}

bool AttributeUI32Range::isCurrentValueSameAsDefault () const
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

UI32Range AttributeUI32Range::getDefaultData (void) const
{
    return (m_defaultData);
}


bool AttributeUI32Range::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

void AttributeUI32Range::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        UI32Range tempData = UI32Range();
        setValue(&tempData);
    }

}

map<string, string> AttributeUI32Range::getSupportedConversions ()
{
    map<string, string> supportedConversions;
   
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";

    return (supportedConversions);
}

void AttributeUI32Range::getCValue (WaveCValue *pCValue)
{
    vector<UI32> rangeIntegers;
    (getValue ()).getUI32RangeVector( rangeIntegers);
    wave_cvalue_set_ui32_vector(pCValue, rangeIntegers);
}

}
