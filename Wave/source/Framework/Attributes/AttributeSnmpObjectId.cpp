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

#include "Framework/Attributes/AttributeSnmpObjectId.h"
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

AttributeSnmpObjectId::AttributeSnmpObjectId (const SnmpObjectId &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeSnmpObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new SnmpObjectId;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeSnmpObjectId::AttributeSnmpObjectId (const SnmpObjectId &data, const SnmpObjectId &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeSnmpObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new SnmpObjectId;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (true);
}

AttributeSnmpObjectId::AttributeSnmpObjectId (SnmpObjectId *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeSnmpObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSnmpObjectId::AttributeSnmpObjectId (SnmpObjectId *pData, const SnmpObjectId &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeSnmpObjectId, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (false);
}

AttributeSnmpObjectId::AttributeSnmpObjectId (const AttributeSnmpObjectId &attribute)
    : Attribute (attribute)
{
    m_pData              = new SnmpObjectId;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeSnmpObjectId::~AttributeSnmpObjectId ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeSnmpObjectId &AttributeSnmpObjectId::operator = (const AttributeSnmpObjectId &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

SnmpObjectId AttributeSnmpObjectId::getValue () const
{
    SnmpObjectId tempSnmpObjectId;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeSnmpObjectId::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempSnmpObjectId;
    }
}
void AttributeSnmpObjectId::getAllValues (UI32  *oid, SI32 *length)
{
    m_pData->getOid (oid);
    *length = m_pData->getLength ();
}

void AttributeSnmpObjectId::setValue (const SnmpObjectId &data)
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

bool AttributeSnmpObjectId::validate () const
{
    prismAssert (AttributeType::AttributeTypeSnmpObjectId == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeSnmpObjectId == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeSnmpObjectId::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
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

string AttributeSnmpObjectId::getSqlType ()
{
    return ("varchar");
}

void AttributeSnmpObjectId::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeSnmpObjectId::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeSnmpObjectId::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeSnmpObjectId::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeSnmpObjectId::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeSnmpObjectId::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeSnmpObjectId::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeSnmpObjectId::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

ResourceId AttributeSnmpObjectId::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString(valueString));
}

void *AttributeSnmpObjectId::getPData ()
{
    return (m_pData);
}

void AttributeSnmpObjectId::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const SnmpObjectId *> (pData));
}

Attribute *AttributeSnmpObjectId::clone ()
{
    AttributeSnmpObjectId *pAttributeSnmpObjectId = new AttributeSnmpObjectId (*this);

    return (pAttributeSnmpObjectId);
}

map<string, string> AttributeSnmpObjectId::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

bool AttributeSnmpObjectId::isCurrentValueSameAsDefault () const
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

bool AttributeSnmpObjectId::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

SnmpObjectId AttributeSnmpObjectId::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeSnmpObjectId::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        SnmpObjectId tempData;
        setValue(&tempData);
    }
}

void AttributeSnmpObjectId::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_snmp_objectid (pCValue, getValue ());
}
}
