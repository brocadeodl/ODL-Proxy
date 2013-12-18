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
 *   Copyright (C) 2005 - 2011 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Attributes/AttributeUI32VectorUC.h"
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

AttributeUI32VectorUC::AttributeUI32VectorUC (const UI32VectorUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32VectorUC, attributeName, attributeUserTag, isOperational),
      m_isDefaultUI32Valid (false)
{
    m_pData       = new UI32VectorUC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32VectorUC::AttributeUI32VectorUC (const UI32VectorUC &data, const bool &isNoElement, const bool &defaultFlag, const vector<UI32> &defaultUI32Vector, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32VectorUC, attributeName, attributeUserTag, isOperational),
      m_defaultUI32Vector  (defaultUI32Vector),
      m_isDefaultUI32Valid (defaultFlag)
{
    m_pData       = new UI32VectorUC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32VectorUC::AttributeUI32VectorUC (UI32VectorUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32VectorUC, attributeName, attributeUserTag, isOperational),
      m_isDefaultUI32Valid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32VectorUC::AttributeUI32VectorUC (UI32VectorUC *pData, const bool &isNoElement, const bool &defaultFlag, const vector<UI32> &defaultUI32Vector, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeUI32VectorUC, attributeName, attributeUserTag, isOperational),
      m_defaultUI32Vector  (defaultUI32Vector),
      m_isDefaultUI32Valid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32VectorUC::AttributeUI32VectorUC (const AttributeUI32VectorUC &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI32VectorUC;
    *m_pData             = attribute.getValue ();
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultUI32Vector  = attribute.getDefaultUI32Vector ();
    m_isDefaultUI32Valid = attribute.getIsDefaultUI32ValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32VectorUC::~AttributeUI32VectorUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI32VectorUC &AttributeUI32VectorUC::operator = (const AttributeUI32VectorUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultUI32Vector  = attribute.getDefaultUI32Vector ();
    m_isDefaultUI32Valid = attribute.getIsDefaultUI32ValidFlag ();

    return (*this);
}

UI32VectorUC AttributeUI32VectorUC::getValue () const
{
    UI32VectorUC tempUI32Vector;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32VectorUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempUI32Vector;
    }
}

void AttributeUI32VectorUC::setValue (const UI32VectorUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32VectorUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI32VectorUC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeUI32VectorUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeUI32VectorUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeUI32VectorUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeUI32VectorUC::getSqlType ()
{
    return ("varchar");
}

void AttributeUI32VectorUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI32VectorUC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUI32VectorUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI32VectorUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI32VectorUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI32VectorUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI32VectorUC::toString (string &valueString)
{
    m_pData->toString (valueString);
}

void AttributeUI32VectorUC::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void *AttributeUI32VectorUC::getPData ()
{
    return (m_pData);
}

void AttributeUI32VectorUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI32VectorUC *> (pData));
}

Attribute *AttributeUI32VectorUC::clone ()
{
    AttributeUI32VectorUC *pAttributeUI32VectorUC = new AttributeUI32VectorUC (*this);

    return (pAttributeUI32VectorUC);
}

bool AttributeUI32VectorUC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultUI32ValidFlag() )
    {   
        if ( getDefaultUI32Vector() == (getValue()).getUI32Vector() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<UI32> AttributeUI32VectorUC::getDefaultUI32Vector () const
{
    return (m_defaultUI32Vector);
}

bool AttributeUI32VectorUC::getIsDefaultUI32ValidFlag () const
{
    return (m_isDefaultUI32Valid);
}

void AttributeUI32VectorUC::setDefaultValue()
{
    if(true == m_isDefaultUI32Valid)
    {
        setValue(&m_defaultUI32Vector);
    }
    else
    {
        (m_pData->getUI32Vector()).clear();
    }
    m_pData->setIsUserConfigured(false);
}

map<string, string> AttributeUI32VectorUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = "WAVE_ATTRIBUTE_TYPE_UI32_UC";
    
    return (supportedConversions);
}

void AttributeUI32VectorUC::getCValue (WaveCValue *pCValue) 
{
    wave_cvalue_set_ui32_vector (pCValue, (getValue ()).getUI32Vector());
}

bool AttributeUI32VectorUC::getIsUserConfigured ()
{   
    return (m_pData->getIsUserConfigured ());
}
}
