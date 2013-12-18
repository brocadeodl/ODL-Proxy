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
 *   Author : Anil Channaveerasetty                                        *
 ***************************************************************************/

#include "Framework/Attributes/AttributeMACRange.h"
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

AttributeMACRange::AttributeMACRange (const MACRange &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeMACRange, attributeName, attributeUserTag, isOperational),
    m_defaultData        ("0000.0000.0000"),
    m_isDefaultDataValid (false)
{
     m_pData = new MACRange;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeMACRange::AttributeMACRange (const MACRange &data, const MACRange &DefaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           (AttributeType::AttributeTypeMACRange, attributeName, attributeUserTag, isOperational),
     m_defaultData        (DefaultData),
     m_isDefaultDataValid (true)
{
     m_pData = new MACRange;
    *m_pData = data;
     setIsMemoryOwnedByAttribute (true);
}


AttributeMACRange::AttributeMACRange (MACRange *pData,const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeMACRange, attributeName, attributeUserTag, isOperational),
    m_defaultData        ("0000.0000.0000"),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeMACRange::AttributeMACRange (MACRange *pData,const MACRange &DefaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           (AttributeType::AttributeTypeMACRange, attributeName, attributeUserTag, isOperational),
     m_defaultData        (DefaultData),
     m_isDefaultDataValid (true)
{
    m_pData = pData;
    setIsMemoryOwnedByAttribute (false);
}


AttributeMACRange::AttributeMACRange (const AttributeMACRange &attribute)
    : Attribute (attribute)
{
     m_pData              = new MACRange;
    *m_pData              = attribute.getValue ();

     m_defaultData        = attribute.getDefaultData ();
     m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

     setIsMemoryOwnedByAttribute (true);
}

AttributeMACRange::~AttributeMACRange ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeMACRange &AttributeMACRange::operator = (const AttributeMACRange &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

MACRange AttributeMACRange::getValue () const
{
    MACRange tempMACRange;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string ("AttributeMACRange::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempMACRange;
    }
}

void AttributeMACRange::setValue (const MACRange &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string ("AttributeIpV4Address::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeMACRange::validate () const
{
    prismAssert (AttributeType::AttributeTypeMACRange == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeMACRange == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeMACRange::getSqlType ()
{
    return ("varchar");
}

void AttributeMACRange::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeMACRange::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeMACRange::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeMACRange::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeMACRange::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeMACRange::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeMACRange::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeMACRange::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

ResourceId AttributeMACRange::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString(valueString));
}

void *AttributeMACRange::getPData ()
{
    return (m_pData);
}

void AttributeMACRange::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const MACRange *> (pData));
}

Attribute *AttributeMACRange::clone ()
{
    AttributeMACRange *pAttributeMACRange = new AttributeMACRange (*this);

    return (pAttributeMACRange);
}

MACRange AttributeMACRange::getDefaultData (void) const
{
    return (m_defaultData);
}

bool AttributeMACRange::isCurrentValueSameAsDefault () const
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

bool AttributeMACRange::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

void AttributeMACRange::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        MACRange tempData;
        setValue(&tempData);
    }
}

map<string, string> AttributeMACRange::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"; 
    
    return (supportedConversions);
}


void AttributeMACRange::getCValue (WaveCValue *pCValue)
{
    vector <string> rangeMacVector;
    (getValue ()).getMacRangeVector (rangeMacVector);
    wave_cvalue_set_mac_range(pCValue, rangeMacVector);
}

}

