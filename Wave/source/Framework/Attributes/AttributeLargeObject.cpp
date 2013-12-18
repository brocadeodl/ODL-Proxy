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
 *   Author : Raymond Lai                                                                          *
 ***************************************************************************/

#include "Framework/Attributes/AttributeLargeObject.h"
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

AttributeLargeObject::AttributeLargeObject (const LargeObject &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeLargeObject, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new LargeObject;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeLargeObject::AttributeLargeObject (const LargeObject &data, const LargeObject &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeLargeObject, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new LargeObject;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (true);
}

AttributeLargeObject::AttributeLargeObject (LargeObject *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeLargeObject, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeLargeObject::AttributeLargeObject (LargeObject *pData, const LargeObject &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeLargeObject, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (false);
}

AttributeLargeObject::AttributeLargeObject (const AttributeLargeObject &attribute)
    : Attribute (attribute)
{
    m_pData              = new LargeObject;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeLargeObject::~AttributeLargeObject ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeLargeObject &AttributeLargeObject::operator = (const AttributeLargeObject &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

LargeObject AttributeLargeObject::getValue () const
{
        LargeObject tempLargeObject;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeLargeObject::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempLargeObject);
    }
}

void AttributeLargeObject::setValue (const LargeObject &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeLargeObject::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeLargeObject::validate () const
{
    prismAssert (AttributeType::AttributeTypeLargeObject == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeLargeObject == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeLargeObject::getSqlType ()
{
    return ("varchar");
}

void AttributeLargeObject::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeLargeObject::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeLargeObject::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeLargeObject::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeLargeObject::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeLargeObject::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeLargeObject::toString (string &valueString)
{
        UI32    noOfBytes       = m_pData->getNumberOfBytes ();
        string  tempString      = m_pData->toString ();
        char    tempBuffer[1024] = {0};

    snprintf (tempBuffer, 1024, "%u:%s", noOfBytes, tempString.c_str ());

    valueString = string (tempBuffer);
}

void AttributeLargeObject::fromString (const string &valueString)
{
        string  tempString;
        char    tempBuffer[1024] = {0};
        UI32    noOfBytes;

        sscanf (valueString.c_str (), "%u:%s", &noOfBytes, tempBuffer);

        tempString = string (tempBuffer);

    m_pData->fromString (tempString, noOfBytes);
}

void *AttributeLargeObject::getPData ()
{
    return (m_pData);
}

void AttributeLargeObject::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const LargeObject *> (pData));
}

Attribute *AttributeLargeObject::clone ()
{
    AttributeLargeObject *pAttributeLargeObject = new AttributeLargeObject (*this);

    return (pAttributeLargeObject);
}

map<string, string> AttributeLargeObject::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

bool AttributeLargeObject::isCurrentValueSameAsDefault () const
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

bool AttributeLargeObject::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

LargeObject AttributeLargeObject::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeLargeObject::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        LargeObject tempData;
        setValue(&tempData);
    }
}


void AttributeLargeObject::getCValue(WaveCValue *pCValue)
{
    wave_cvalue_set_large_object (pCValue, getValue ());
}

}

