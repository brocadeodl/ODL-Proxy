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

#include "Framework/Attributes/AttributeHostUC.h"
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

namespace WaveNs
{

AttributeHostUC::AttributeHostUC (const HostUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeHostUC, attributeName, attributeUserTag, isOperational),
      m_defaultData        (""),
      m_isDefaultDataValid (false)
{
    m_pData       = new HostUC (data);
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeHostUC::AttributeHostUC (const HostUC &data, const bool &isNoElement, const bool &defaultFlag, const HostUC &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeHostUC, attributeName, attributeUserTag, isOperational),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (defaultFlag)
{
    m_pData       = new HostUC (data);
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeHostUC::AttributeHostUC (HostUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeHostUC, attributeName, attributeUserTag, isOperational),
      m_defaultData        (""),
      m_isDefaultDataValid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeHostUC::AttributeHostUC (HostUC *pData, const bool &isNoElement, const bool &defaultFlag, const HostUC &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute            (AttributeType::AttributeTypeHostUC, attributeName, attributeUserTag, isOperational),
      m_defaultData        (defaultData),
      m_isDefaultDataValid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeHostUC::AttributeHostUC (const AttributeHostUC &attribute)
    : Attribute (attribute)
{
    m_pData              = new HostUC (attribute.getValue ());

    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeHostUC::~AttributeHostUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeHostUC &AttributeHostUC::operator = (const AttributeHostUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

HostUC AttributeHostUC::getValue () const
{
    HostUC tempHost;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeHostUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempHost);
    }
}

void AttributeHostUC::setValue (const HostUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeHostUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeHostUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeHostUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeHostUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeHostUC::getSqlType ()
{
    return ("varchar");
}

void AttributeHostUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeHostUC::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeHostUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeHostUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeHostUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeHostUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeHostUC::toString (string &valueString)
{
    m_pData->toString (valueString);
}

void AttributeHostUC::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void AttributeHostUC::getPlainString (string &valueString)
{
     m_pData->getPlainString (valueString);
}

ResourceId AttributeHostUC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeHostUC::getPData ()
{
    return (m_pData);
}

void AttributeHostUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const HostUC *> (pData));
}

Attribute *AttributeHostUC::clone ()
{
    AttributeHostUC *pAttributeHostUC = new AttributeHostUC (*this);

    return (pAttributeHostUC);
}

bool AttributeHostUC::isCurrentValueSameAsDefault () const
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

HostUC AttributeHostUC::getDefaultData (void) const
{
    return (m_defaultData);
}


bool AttributeHostUC::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

bool AttributeHostUC::getIsNoElement () const
{
    return (m_isNoElement);
}

map<string, string> AttributeHostUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

void AttributeHostUC::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
         setValue(&m_defaultData);
    }
    else
    {
         HostUC tempData;
         setValue(&tempData);
    }
    m_pData->setIsUserConfigured(false);
}
 
void AttributeHostUC::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_host (pCValue, getValue ());
}

bool AttributeHostUC::getIsUserConfigured ()
{   
    return (m_pData->getIsUserConfigured ());
}
}
