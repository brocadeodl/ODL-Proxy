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

#include "Framework/Attributes/AttributeEnumUC.h"
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

AttributeEnumUC::AttributeEnumUC (const EnumUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeEnumUC, attributeName, attributeUserTag, isOperational),
      m_pDefaultEnum (0),
      m_IsDefaultEnumValid (false)
{
     m_pData       = new EnumUC;
    *m_pData       = data;
     m_isNoElement = isNoElement; 

    setIsMemoryOwnedByAttribute (true);
}

AttributeEnumUC::AttributeEnumUC (const EnumUC &data, const bool &isNoElement, const bool &defaultFlag, const UI32 &defaultEnumUC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeEnumUC, attributeName, attributeUserTag, isOperational),
      m_pDefaultEnum (defaultEnumUC),
      m_IsDefaultEnumValid (defaultFlag)
{
     m_pData       = new EnumUC;
    *m_pData       = data;
     m_isNoElement = isNoElement; 

    setIsMemoryOwnedByAttribute (true);
}

AttributeEnumUC::AttributeEnumUC (EnumUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeEnumUC, attributeName, attributeUserTag, isOperational),
      m_pDefaultEnum (0),
      m_IsDefaultEnumValid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;
    
    setIsMemoryOwnedByAttribute (false);
}

AttributeEnumUC::AttributeEnumUC (EnumUC *pData, const bool &isNoElement, const bool &defaultFlag, const UI32 &defaultEnumUC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeEnumUC, attributeName, attributeUserTag, isOperational),
      m_pDefaultEnum (defaultEnumUC),
      m_IsDefaultEnumValid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;
    
    setIsMemoryOwnedByAttribute (false);
}

AttributeEnumUC::AttributeEnumUC (const AttributeEnumUC &attribute)
    : Attribute (attribute)
{
     m_pData       = new EnumUC;

    *m_pData       = attribute.getValue ();
     m_isNoElement = attribute.getIsNoElement ();
     m_pDefaultEnum = attribute.getDefaultEnum ();
     m_IsDefaultEnumValid = attribute.getIsDefaultEnumValidFlag ();
 
    setIsMemoryOwnedByAttribute (true);
}

AttributeEnumUC::~AttributeEnumUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeEnumUC &AttributeEnumUC::operator = (const AttributeEnumUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement = attribute.getIsNoElement ();
    m_pDefaultEnum = attribute.getDefaultEnum ();
    m_IsDefaultEnumValid = attribute.getIsDefaultEnumValidFlag ();

    return (*this);
}

EnumUC AttributeEnumUC::getValue () const
{
    EnumUC tempEnumUC;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeEnumUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempEnumUC;
    }
}

void AttributeEnumUC::setValue (const EnumUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeEnumUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeEnumUC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeEnumUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeEnumUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeEnumUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeEnumUC::getSqlType ()
{
    return ("varchar");
}

void AttributeEnumUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeEnumUC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeEnumUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeEnumUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeEnumUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeEnumUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeEnumUC::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeEnumUC::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

ResourceId AttributeEnumUC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString(valueString));
}

void *AttributeEnumUC::getPData ()
{
    return (m_pData);
}

void AttributeEnumUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const EnumUC *> (pData));
}

Attribute *AttributeEnumUC::clone ()
{
    AttributeEnumUC *pAttributeEnumUC = new AttributeEnumUC (*this);

    return (pAttributeEnumUC);
}

bool AttributeEnumUC::isCurrentValueSameAsDefault () const
{   
    bool isDefault = false ;

    if ( getIsDefaultEnumValidFlag() )
    {   
        if ( getDefaultEnum() == (getValue()).getUI32Value() )
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

UI32 AttributeEnumUC::getDefaultEnum () const
{
    return (m_pDefaultEnum);
}

bool AttributeEnumUC::getIsDefaultEnumValidFlag () const
{
    return (m_IsDefaultEnumValid);
}

void AttributeEnumUC::setDefaultValue ()
{
    if (true == m_IsDefaultEnumValid)
    {
        m_pData->setUI32Value (m_pDefaultEnum);
    }
    else
    {
        m_pData->setUI32Value (0);
    }

    m_pData->setIsUserConfigured (false);
}

map<string, string> AttributeEnumUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = "WAVE_ATTRIBUTE_TYPE_UI32_UC";
    
    return (supportedConversions);
}

void AttributeEnumUC::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_ui32 (pCValue, (getValue ()).getUI32Value());
}

bool AttributeEnumUC::getIsUserConfigured ()
{   
    return (m_pData->getIsUserConfigured ());
}
}
