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

#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
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

AttributeIpV6AddressNetworkMask::AttributeIpV6AddressNetworkMask (const IpV6AddressNetworkMask &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                            (AttributeType::AttributeTypeIpV6AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_isDefaultIpV6AddressNetworkMaskValid (false)
{
    m_pData  = new IpV6AddressNetworkMask;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV6AddressNetworkMask::AttributeIpV6AddressNetworkMask (const IpV6AddressNetworkMask &data, const IpV6AddressNetworkMask &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                            (AttributeType::AttributeTypeIpV6AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_defaultIpV6AddressNetworkMask        (defaultData),
    m_isDefaultIpV6AddressNetworkMaskValid (true)
{
    m_pData  = new IpV6AddressNetworkMask;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV6AddressNetworkMask::AttributeIpV6AddressNetworkMask (IpV6AddressNetworkMask *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                            (AttributeType::AttributeTypeIpV6AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_isDefaultIpV6AddressNetworkMaskValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpV6AddressNetworkMask::AttributeIpV6AddressNetworkMask (IpV6AddressNetworkMask *pData, const IpV6AddressNetworkMask &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                            (AttributeType::AttributeTypeIpV6AddressNetworkMask, attributeName, attributeUserTag, isOperational),
    m_defaultIpV6AddressNetworkMask        (defaultData),
    m_isDefaultIpV6AddressNetworkMaskValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpV6AddressNetworkMask::AttributeIpV6AddressNetworkMask (const AttributeIpV6AddressNetworkMask &attribute)
    : Attribute (attribute)
{
    m_pData                                = new IpV6AddressNetworkMask;
    *m_pData                               = attribute.getValue ();
    m_defaultIpV6AddressNetworkMask        = attribute.getDefaultIpV6AddressNetworkMask();
    m_isDefaultIpV6AddressNetworkMaskValid = attribute.getIsDefaultIpV6AddressNetworkMaskValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV6AddressNetworkMask::~AttributeIpV6AddressNetworkMask ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV6AddressNetworkMask &AttributeIpV6AddressNetworkMask::operator = (const AttributeIpV6AddressNetworkMask &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultIpV6AddressNetworkMask        = attribute.getDefaultIpV6AddressNetworkMask();
    m_isDefaultIpV6AddressNetworkMaskValid = attribute.getIsDefaultIpV6AddressNetworkMaskValidFlag();

    return (*this);
}

IpV6AddressNetworkMask AttributeIpV6AddressNetworkMask::getValue () const
{
    IpV6AddressNetworkMask tempIpV6AddressNetworkMask;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV6AddressNetworkMask::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempIpV6AddressNetworkMask;
    }
}

void AttributeIpV6AddressNetworkMask:: getAllValues (string &NetworkMask, UI8 *length)
{
    NetworkMask = m_pData->getIpAddress ();
    *length     = m_pData->getLength ();
}

void AttributeIpV6AddressNetworkMask::setValue (const IpV6AddressNetworkMask &data)
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

bool AttributeIpV6AddressNetworkMask::validate () const
{
    prismAssert (AttributeType::AttributeTypeIpV6AddressNetworkMask == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeIpV6AddressNetworkMask == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeIpV6AddressNetworkMask::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
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

string AttributeIpV6AddressNetworkMask::getSqlType ()
{
    return ("inet");
}

void AttributeIpV6AddressNetworkMask::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV6AddressNetworkMask::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeIpV6AddressNetworkMask::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
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

void AttributeIpV6AddressNetworkMask::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
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

void AttributeIpV6AddressNetworkMask::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV6AddressNetworkMask::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV6AddressNetworkMask::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeIpV6AddressNetworkMask::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeIpV6AddressNetworkMask::loadFromPlainString (const string &valueString)
{
   return ( m_pData->loadFromPlainString (valueString));
}

void *AttributeIpV6AddressNetworkMask::getPData ()
{
    return (m_pData);
}

void AttributeIpV6AddressNetworkMask::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const IpV6AddressNetworkMask *> (pData));
}

Attribute *AttributeIpV6AddressNetworkMask::clone ()
{
    AttributeIpV6AddressNetworkMask *pAttributeIpV6AddressNetworkMask = new AttributeIpV6AddressNetworkMask (*this);

    return (pAttributeIpV6AddressNetworkMask);
}

map<string, string> AttributeIpV6AddressNetworkMask::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"; 
    
    return (supportedConversions);
}

bool AttributeIpV6AddressNetworkMask::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultIpV6AddressNetworkMaskValidFlag() )
    {   
        if ( getDefaultIpV6AddressNetworkMask() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

IpV6AddressNetworkMask AttributeIpV6AddressNetworkMask::getDefaultIpV6AddressNetworkMask() const
{
    return (m_defaultIpV6AddressNetworkMask);
}

bool AttributeIpV6AddressNetworkMask::getIsDefaultIpV6AddressNetworkMaskValidFlag() const
{
    return m_isDefaultIpV6AddressNetworkMaskValid;
}

void AttributeIpV6AddressNetworkMask::setDefaultValue()
{
   if (true == m_isDefaultIpV6AddressNetworkMaskValid)
   {
        setValue(&m_defaultIpV6AddressNetworkMask);
   }
   else
   {
        IpV6AddressNetworkMask tempData;
        setValue(&tempData);
   }
}

void AttributeIpV6AddressNetworkMask::getCValue (WaveCValue *pCValue)
{
   wave_cvalue_set_ipaddress_networkmask (pCValue, (WaveNs::IpV6AddressNetworkMask*) getPData ());
}

}
