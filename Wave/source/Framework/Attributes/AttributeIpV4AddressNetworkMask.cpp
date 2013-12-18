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

#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
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

AttributeIpV4AddressNetworkMask::AttributeIpV4AddressNetworkMask (const IpV4AddressNetworkMask &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new IpV4AddressNetworkMask;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressNetworkMask::AttributeIpV4AddressNetworkMask (const IpV4AddressNetworkMask &data, const IpV4AddressNetworkMask &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new IpV4AddressNetworkMask;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressNetworkMask::AttributeIpV4AddressNetworkMask (IpV4AddressNetworkMask *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpV4AddressNetworkMask::AttributeIpV4AddressNetworkMask (IpV4AddressNetworkMask *pData, const IpV4AddressNetworkMask &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;
    setIsMemoryOwnedByAttribute (false);
}
AttributeIpV4AddressNetworkMask::AttributeIpV4AddressNetworkMask (const AttributeIpV4AddressNetworkMask &attribute)
    : Attribute (attribute)
{
    m_pData              = new IpV4AddressNetworkMask;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressNetworkMask::~AttributeIpV4AddressNetworkMask ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV4AddressNetworkMask &AttributeIpV4AddressNetworkMask::operator = (const AttributeIpV4AddressNetworkMask &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    return (*this);
}

IpV4AddressNetworkMask AttributeIpV4AddressNetworkMask::getValue () const
{
    IpV4AddressNetworkMask tempIpV4AddressNetworkMask;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4AddressNetworkMask::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempIpV4AddressNetworkMask;
    }
}

void AttributeIpV4AddressNetworkMask:: getAllValues (string &NetworkMask, UI8 *length)
{
    NetworkMask = m_pData->getIpAddress ();
    *length     = m_pData->getLength ();
}

void AttributeIpV4AddressNetworkMask::setValue (const IpV4AddressNetworkMask &data)
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

bool AttributeIpV4AddressNetworkMask::validate () const
{
    prismAssert (AttributeType::AttributeTypeIpV4AddressNetworkMask == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeIpV4AddressNetworkMask == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeIpV4AddressNetworkMask::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
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

string AttributeIpV4AddressNetworkMask::getSqlType ()
{
    return ("inet");
}

void AttributeIpV4AddressNetworkMask::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV4AddressNetworkMask::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeIpV4AddressNetworkMask::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
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

void AttributeIpV4AddressNetworkMask::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
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

void AttributeIpV4AddressNetworkMask::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV4AddressNetworkMask::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV4AddressNetworkMask::toString (string &valueString)
{
    valueString = m_pData->toString ();;
}

void AttributeIpV4AddressNetworkMask::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeIpV4AddressNetworkMask::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void* AttributeIpV4AddressNetworkMask::getPData ()
{
    return (m_pData);
}

void AttributeIpV4AddressNetworkMask::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const IpV4AddressNetworkMask *> (pData));
}

Attribute *AttributeIpV4AddressNetworkMask::clone ()
{
    AttributeIpV4AddressNetworkMask *pAttributeIpV4AddressNetworkMask = new AttributeIpV4AddressNetworkMask (*this);

    return (pAttributeIpV4AddressNetworkMask);
}

map<string, string> AttributeIpV4AddressNetworkMask::getSupportedConversions ()
{
    map<string, string> supportedConversions;

    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"; 
    
    return (supportedConversions);
}

bool AttributeIpV4AddressNetworkMask::isCurrentValueSameAsDefault () const
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

bool AttributeIpV4AddressNetworkMask::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

IpV4AddressNetworkMask AttributeIpV4AddressNetworkMask::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeIpV4AddressNetworkMask::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        IpV4AddressNetworkMask tempData;
        setValue(&tempData);
    }
}


void AttributeIpV4AddressNetworkMask::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ipaddress_networkmask (pCValue, (WaveNs::IpV4AddressNetworkMask*) getPData ());
}

}
