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

#include "Framework/Attributes/AttributeBoolUC.h"
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

AttributeBoolUC::AttributeBoolUC (const BoolUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeBoolUC, attributeName, attributeUserTag, isOperational),
    m_defaultData        (false),
    m_isDefaultDataValid (false)
{
     m_pData       = new BoolUC;
    *m_pData       = data;
     m_isNoElement = isNoElement; 

    setIsMemoryOwnedByAttribute (true);
}

AttributeBoolUC::AttributeBoolUC (const BoolUC &data, const bool &isNoElement, const string &defaultFlag, const string &defaultBoolUC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeBoolUC, attributeName, attributeUserTag, isOperational)
{
     m_pData       = new BoolUC;
    *m_pData       = data;
     m_isNoElement = isNoElement;

    setDefaultDataForBoolUC( defaultFlag, defaultBoolUC );
    setIsMemoryOwnedByAttribute (true);
}

AttributeBoolUC::AttributeBoolUC (BoolUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeBoolUC, attributeName, attributeUserTag, isOperational),
    m_defaultData        (false),
    m_isDefaultDataValid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;
    
    setIsMemoryOwnedByAttribute (false);
}

AttributeBoolUC::AttributeBoolUC (BoolUC *pData, const bool &isNoElement, const string &defaultFlag, const string &defaultBoolUC, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute (AttributeType::AttributeTypeBoolUC, attributeName, attributeUserTag, isOperational)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setDefaultDataForBoolUC( defaultFlag, defaultBoolUC );
    setIsMemoryOwnedByAttribute (false);
}

AttributeBoolUC::AttributeBoolUC (const AttributeBoolUC &attribute)
    : Attribute (attribute)
{
     m_pData       = new BoolUC;

    *m_pData       = attribute.getValue ();
     m_isNoElement = attribute.getIsNoElement ();
    
    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeBoolUC::~AttributeBoolUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

void AttributeBoolUC::setDefaultDataForBoolUC( const string &defaultFlag, const string &defaultBoolUC )
{
    m_defaultData =       false ;
    m_isDefaultDataValid = false ;
    
    if ( "true" == defaultFlag )
    {
        m_isDefaultDataValid = true ;
    }
    
    if ( "true" == defaultBoolUC )
    {  
        m_defaultData = true ;
    }
}

AttributeBoolUC &AttributeBoolUC::operator = (const AttributeBoolUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement = attribute.getIsNoElement ();

    m_defaultData        = attribute.getDefaultData();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

BoolUC AttributeBoolUC::getValue () const
{
    BoolUC tempBoolUC;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBoolUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempBoolUC;
    }
}

void AttributeBoolUC::setValue (const BoolUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBoolUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeBoolUC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeBoolUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeBoolUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeBoolUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeBoolUC::getSqlType ()
{
    return ("varchar");
}

void AttributeBoolUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeBoolUC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeBoolUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeBoolUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeBoolUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeBoolUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator)); 
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeBoolUC::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeBoolUC::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

void AttributeBoolUC::getPlainString (string &valueString)
{
     m_pData->getPlainString (valueString);
}

ResourceId AttributeBoolUC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeBoolUC::getPData ()
{
    return (m_pData);
}

void AttributeBoolUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const BoolUC *> (pData));
}

Attribute *AttributeBoolUC::clone ()
{
    AttributeBoolUC *pAttributeBoolUC = new AttributeBoolUC (*this);

    return (pAttributeBoolUC);
}

map<string, string> AttributeBoolUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_BOOL"] = "WAVE_ATTRIBUTE_TYPE_BOOL";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

bool AttributeBoolUC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == (getValue()).getBoolValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeBoolUC::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeBoolUC::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}   

void AttributeBoolUC::setDefaultValue ()
{
    m_pData->setBoolValue (false);
    m_pData->setIsUserConfigured (false);
}

void AttributeBoolUC::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_bool (pCValue, (getValue ()).getBoolValue() );    
}

bool AttributeBoolUC::getIsUserConfigured ()
{
    return (m_pData->getIsUserConfigured ());    
}

}
