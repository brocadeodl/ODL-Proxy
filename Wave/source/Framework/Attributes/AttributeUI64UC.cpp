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
 *   Author : Mandar Datar                                                 *
 *   Note   : Copied from Framework/Attributes/AttributeUI32UC.cpp         *
 ***************************************************************************/

#include "Framework/Attributes/AttributeUI64UC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
// #include "Framework/Utils/Integer.h"
// #include "Framework/Utils/BigInteger.h"
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

AttributeUI64UC::AttributeUI64UC (const UI64UC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI64UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI64        (0),
      m_isDefaultUI64Valid (false)
{
    m_pData       = new UI64UC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64UC::AttributeUI64UC (const UI64UC &data, const bool &isNoElement, const bool &defaultFlag, const UI64 &defaultUI64UC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI64UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI64        (defaultUI64UC),
      m_isDefaultUI64Valid (defaultFlag)
{
    m_pData       = new UI64UC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64UC::AttributeUI64UC (UI64UC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI64UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI64        (0),
      m_isDefaultUI64Valid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI64UC::AttributeUI64UC (UI64UC *pData, const bool &isNoElement, const bool &defaultFlag, const UI64 &defaultUI64UC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI64UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI64        (defaultUI64UC),
      m_isDefaultUI64Valid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI64UC::AttributeUI64UC (const AttributeUI64UC &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI64UC;
    *m_pData             = attribute.getValue ();
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultUI64        = attribute.getDefaultUI64 ();
    m_isDefaultUI64Valid = attribute.getIsDefaultUI64ValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI64UC::~AttributeUI64UC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI64UC &AttributeUI64UC::operator = (const AttributeUI64UC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultUI64        = attribute.getDefaultUI64 ();
    m_isDefaultUI64Valid = attribute.getIsDefaultUI64ValidFlag ();

    return (*this);
}

UI64UC AttributeUI64UC::getValue () const
{
    UI64UC tempUI64UC;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI64UC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempUI64UC;
    }
}

void AttributeUI64UC::setValue (const UI64UC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI64UC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI64UC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeUI64UC::validate () const
{
    prismAssert (AttributeType::AttributeTypeUI64UC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeUI64UC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUI64UC::getSqlType ()
{
    return ("varchar");
}

void AttributeUI64UC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI64UC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUI64UC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI64UC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI64UC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI64UC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI64UC::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeUI64UC::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

void AttributeUI64UC::getPlainString (string &valueString)
{
     m_pData->getPlainString (valueString);
}
ResourceId AttributeUI64UC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString) );
}

void *AttributeUI64UC::getPData ()
{
    return (m_pData);
}

void AttributeUI64UC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI64UC *> (pData));
}

Attribute *AttributeUI64UC::clone ()
{
    AttributeUI64UC *pAttributeUI64UC = new AttributeUI64UC (*this);

    return (pAttributeUI64UC);
}

bool AttributeUI64UC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultUI64ValidFlag() )
    {   
        if ( getDefaultUI64() == (getValue()).getUI64Value() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

UI64 AttributeUI64UC::getDefaultUI64 () const
{
    return (m_defaultUI64);
}

bool AttributeUI64UC::getIsDefaultUI64ValidFlag () const
{
    return (m_isDefaultUI64Valid);
}

void AttributeUI64UC::setDefaultValue()
{
    if(true == m_isDefaultUI64Valid)
    {
        m_pData->setUI64Value(m_defaultUI64);
    }
    else
    {
        m_pData->setUI64Value(0);
    }
    m_pData->setIsUserConfigured(false);
}

map<string, string> AttributeUI64UC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = "WAVE_ATTRIBUTE_TYPE_UI32_UC";
    
    return (supportedConversions);
}

void AttributeUI64UC::getCValue (WaveCValue *pCValue) 
{
   wave_cvalue_set_ui64 (pCValue, (getValue ()).getUI64Value());
}

bool AttributeUI64UC::getIsUserConfigured ()
{   
    return (m_pData->getIsUserConfigured ());
}
}
