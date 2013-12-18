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

#include "Framework/Attributes/AttributeUI32UC.h"
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

AttributeUI32UC::AttributeUI32UC (const UI32UC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI32        (0),
      m_isDefaultUI32Valid (false)
{
    m_pData       = new UI32UC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32UC::AttributeUI32UC (const UI32UC &data, const bool &isNoElement, const bool &defaultFlag, const UI32 &defaultUI32UC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI32        (defaultUI32UC),
      m_isDefaultUI32Valid (defaultFlag)
{
    m_pData       = new UI32UC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32UC::AttributeUI32UC (UI32UC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI32        (0),
      m_isDefaultUI32Valid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32UC::AttributeUI32UC (UI32UC *pData, const bool &isNoElement, const bool &defaultFlag, const UI32 &defaultUI32UC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32UC, attributeName, attributeUserTag, isOperational),
      m_defaultUI32        (defaultUI32UC),
      m_isDefaultUI32Valid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32UC::AttributeUI32UC (const AttributeUI32UC &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI32UC;
    *m_pData             = attribute.getValue ();
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultUI32        = attribute.getDefaultUI32 ();
    m_isDefaultUI32Valid = attribute.getIsDefaultUI32ValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32UC::~AttributeUI32UC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI32UC &AttributeUI32UC::operator = (const AttributeUI32UC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultUI32        = attribute.getDefaultUI32 ();
    m_isDefaultUI32Valid = attribute.getIsDefaultUI32ValidFlag ();

    return (*this);
}

UI32UC AttributeUI32UC::getValue () const
{
    UI32UC tempUI32UC;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32UC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempUI32UC;
    }
}

void AttributeUI32UC::setValue (const UI32UC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32UC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI32UC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeUI32UC::validate () const
{
    prismAssert (AttributeType::AttributeTypeUI32UC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeUI32UC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUI32UC::getSqlType ()
{
    return ("varchar");
}

void AttributeUI32UC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI32UC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUI32UC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI32UC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI32UC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI32UC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI32UC::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeUI32UC::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

void AttributeUI32UC::getPlainString (string &valueString)
{
     m_pData->getPlainString (valueString);
}

ResourceId AttributeUI32UC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString) );
}
void *AttributeUI32UC::getPData ()
{
    return (m_pData);
}

void AttributeUI32UC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI32UC *> (pData));
}

Attribute *AttributeUI32UC::clone ()
{
    AttributeUI32UC *pAttributeUI32UC = new AttributeUI32UC (*this);

    return (pAttributeUI32UC);
}

bool AttributeUI32UC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;
  
    if ( getIsDefaultUI32ValidFlag() )
    {   
        if ( getDefaultUI32() == (getValue()).getUI32Value() )
        {   
            isDefault = true;
        }
    }
  
    return (isDefault);
}

UI32 AttributeUI32UC::getDefaultUI32 () const
{
    return (m_defaultUI32);
}

bool AttributeUI32UC::getIsDefaultUI32ValidFlag () const
{
    return (m_isDefaultUI32Valid);
}

void AttributeUI32UC::setDefaultValue ()
{
    if (true == m_isDefaultUI32Valid)
    {
        m_pData->setUI32Value (m_defaultUI32);
    }
    else
    {
        m_pData->setUI32Value (0);
    }

    m_pData->setIsUserConfigured (false);
}

map<string, string> AttributeUI32UC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64_UC"] = "WAVE_ATTRIBUTE_TYPE_UI64_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32VECTOR_UC"] = "WAVE_ATTRIBUTE_TYPE_UI32VECTOR_UC";
    
    return (supportedConversions);
}

void AttributeUI32UC::getCValue(WaveCValue *pCValue) 
{
    wave_cvalue_set_ui32 (pCValue, (getValue ()).getUI32Value());
}

bool AttributeUI32UC::getIsUserConfigured ()
{
    return (m_pData->getIsUserConfigured ());
}

}
