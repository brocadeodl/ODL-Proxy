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

#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUUID.h"
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

AttributeUUID::AttributeUUID (const Uuid &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeUUID, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new Uuid;
    *m_pData             = data;
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeUUID::AttributeUUID (const Uuid &data, const Uuid &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeUUID, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new Uuid;
    *m_pData             = data;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

AttributeUUID::AttributeUUID (Uuid *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeUUID, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUUID::AttributeUUID (Uuid *pData, const Uuid &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeUUID, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (false);
}

AttributeUUID::AttributeUUID (const AttributeUUID &attribute)
    : Attribute (attribute)
{
    m_pData              = new Uuid;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultValueValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUUID::~AttributeUUID ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUUID &AttributeUUID::operator = (const AttributeUUID &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultValueValidFlag();

    return (*this);
}

Uuid AttributeUUID::getValue () const
{
    Uuid tempUuid;
    tempUuid.clear();

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUUID::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempUuid);
    }
}

void AttributeUUID::setValue (const Uuid &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUUID::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUUID::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUUID == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUUID == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUUID::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;

    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL                     == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL                 == attributeConditionOperator )
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeUUID::getSqlType ()
{
    return ("varchar");
}

void AttributeUUID::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUUID::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeUUID::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUUID::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUUID::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUUID::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUUID::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeUUID::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void *AttributeUUID::getPData ()
{
    return (m_pData);
}

void AttributeUUID::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const Uuid *> (pData));
}

Attribute *AttributeUUID::clone ()
{
    AttributeUUID *pAttributeUUID = new AttributeUUID (*this);

    return (pAttributeUUID);
}

bool AttributeUUID::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultValueValidFlag () )
    {   
        if ( getDefaultValue ().getUuidString () == (getValue ()).getUuidString () )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

map<string, string> AttributeUUID::getSupportedConversions ()
{
    map<string, string> supportedConversions;

    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeUUID::getIsDefaultValueValidFlag() const
{
    return (m_isDefaultDataValid);
}

Uuid AttributeUUID::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeUUID::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        Uuid tempUUID;
        tempUUID.clear();
        setValue(&tempUUID);
    }
}

void AttributeUUID::getCValue(WaveCValue *pCValue)
{
	wave_cvalue_set_string (pCValue, (getValue ()).toString());
}

}
