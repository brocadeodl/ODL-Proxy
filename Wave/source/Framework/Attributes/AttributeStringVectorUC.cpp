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

#include "Framework/Attributes/AttributeStringVectorUC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
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

AttributeStringVectorUC::AttributeStringVectorUC (const StringVectorUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringVectorUC, attributeName, attributeUserTag, isOperational),
      m_isDefaultStringValid (false)
{
    m_pData       = new StringVectorUC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVectorUC::AttributeStringVectorUC (const StringVectorUC &data, const bool &isNoElement, const bool &defaultFlag, const vector<string> &defaultStringVector, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringVectorUC, attributeName, attributeUserTag, isOperational),
      m_defaultStringVector  (defaultStringVector),
      m_isDefaultStringValid (defaultFlag)
{
    m_pData       = new StringVectorUC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVectorUC::AttributeStringVectorUC (StringVectorUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringVectorUC, attributeName, attributeUserTag, isOperational),
      m_isDefaultStringValid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeStringVectorUC::AttributeStringVectorUC (StringVectorUC *pData, const bool &isNoElement, const bool &defaultFlag, const vector<string> &defaultStringVector, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringVectorUC, attributeName, attributeUserTag, isOperational),
      m_defaultStringVector  (defaultStringVector),
      m_isDefaultStringValid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeStringVectorUC::AttributeStringVectorUC (const AttributeStringVectorUC &attribute)
    : Attribute (attribute)
{
    m_pData                = new StringVectorUC;

    *m_pData               = attribute.getValue ();
    m_isNoElement          = attribute.getIsNoElement ();
    m_defaultStringVector  = attribute.getDefaultStringVector ();
    m_isDefaultStringValid = attribute.getIsDefaultStringValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVectorUC::~AttributeStringVectorUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeStringVectorUC &AttributeStringVectorUC::operator = (const AttributeStringVectorUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement          = attribute.getIsNoElement ();
    m_defaultStringVector  = attribute.getDefaultStringVector ();
    m_isDefaultStringValid = attribute.getIsDefaultStringValidFlag ();

    return (*this);
}

StringVectorUC AttributeStringVectorUC::getValue () const
{
    StringVectorUC tempStringVector;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringVectorUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempStringVector;
    }
}

void AttributeStringVectorUC::setValue (const StringVectorUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringVectorUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeStringVectorUC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeStringVectorUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeStringVectorUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeStringVectorUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeStringVectorUC::getSqlType ()
{
    return ("varchar");
}

void AttributeStringVectorUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeStringVectorUC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeStringVectorUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeStringVectorUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeStringVectorUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeStringVectorUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeStringVectorUC::toString (string &valueString)
{
    m_pData->toString (valueString);
}

void AttributeStringVectorUC::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void *AttributeStringVectorUC::getPData ()
{
    return (m_pData);
}

void AttributeStringVectorUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const StringVectorUC *> (pData));
}

Attribute *AttributeStringVectorUC::clone ()
{
    AttributeStringVectorUC *pAttributeStringVectorUC = new AttributeStringVectorUC (*this);

    return (pAttributeStringVectorUC);
}

bool AttributeStringVectorUC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultStringValidFlag() )
    {   
        if ( getDefaultStringVector() == (getValue()).getStringVector() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

vector<string> AttributeStringVectorUC::getDefaultStringVector () const
{
    return (m_defaultStringVector);
}

bool AttributeStringVectorUC::getIsDefaultStringValidFlag () const
{
    return (m_isDefaultStringValid);
}

void AttributeStringVectorUC::setDefaultValue()
{
    if(true == m_isDefaultStringValid)
    {
        m_pData->setStringVector(m_defaultStringVector);
    }
    else
    {
        vector<string> tempData;
        m_pData->setStringVector(tempData);
    }
    m_pData->setIsUserConfigured(false);

}

map<string, string> AttributeStringVectorUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_UC"] = "WAVE_ATTRIBUTE_TYPE_STRING_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATE_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_DATE_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_TIME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_TIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR";
    
    return (supportedConversions);
}

void AttributeStringVectorUC::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_string_vector (pCValue, (getValue ()).getStringVector ());	
}

bool AttributeStringVectorUC::getIsUserConfigured ()
{
    return (m_pData->getIsUserConfigured ());
}

void AttributeStringVectorUC::getPlainString (string &valueString)
{
    valueString = m_pData->getPlainString ();
}

}
