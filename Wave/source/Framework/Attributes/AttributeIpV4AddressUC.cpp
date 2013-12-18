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

#include "Framework/Attributes/AttributeIpV4AddressUC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
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

AttributeIpV4AddressUC::AttributeIpV4AddressUC (const IpV4AddressUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressUC, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData        = new IpV4AddressUC;
    *m_pData       = data;
    m_isNoElement  = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressUC::AttributeIpV4AddressUC (const IpV4AddressUC &data, const bool &isNoElement, const bool &defaultFlag, const IpV4Address &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressUC, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (defaultFlag)
{
    m_pData       = new IpV4AddressUC;
    *m_pData      = data;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressUC::AttributeIpV4AddressUC (IpV4AddressUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressUC, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpV4AddressUC::AttributeIpV4AddressUC (IpV4AddressUC *pData, const bool &isNoElement, const bool &defaultFlag, const IpV4Address &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeIpV4AddressUC, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpV4AddressUC::AttributeIpV4AddressUC (const AttributeIpV4AddressUC &attribute)
    : Attribute (attribute)
{
    m_pData              = new IpV4AddressUC;
    *m_pData             = attribute.getValue ();
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

     setIsMemoryOwnedByAttribute (true);
}

AttributeIpV4AddressUC::~AttributeIpV4AddressUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpV4AddressUC &AttributeIpV4AddressUC::operator = (const AttributeIpV4AddressUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement        = attribute.getIsNoElement ();
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

IpV4AddressUC AttributeIpV4AddressUC::getValue () const
{
    IpV4AddressUC tempIpV4Addres;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4AddressUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempIpV4Addres);
    }
}

void AttributeIpV4AddressUC::setValue (const IpV4AddressUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpV4AddressUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpV4AddressUC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeIpV4AddressUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeIpV4AddressUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeIpV4AddressUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeIpV4AddressUC::getSqlType ()
{
    return ("varchar");
}

void AttributeIpV4AddressUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpV4AddressUC::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeIpV4AddressUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeIpV4AddressUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeIpV4AddressUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpV4AddressUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpV4AddressUC::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeIpV4AddressUC::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void AttributeIpV4AddressUC::getPlainString (string &valueString)
{
     m_pData->getPlainString (valueString);
}

ResourceId AttributeIpV4AddressUC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeIpV4AddressUC::getPData ()
{
    return (m_pData);
}

void AttributeIpV4AddressUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const IpV4AddressUC *> (pData));
}

Attribute *AttributeIpV4AddressUC::clone ()
{
    AttributeIpV4AddressUC *pAttributeIpV4AddressUC = new AttributeIpV4AddressUC (*this);

    return (pAttributeIpV4AddressUC);
}

bool AttributeIpV4AddressUC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultData() == (getValue()).getIpV4AddressValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

IpV4Address AttributeIpV4AddressUC::getDefaultData (void) const
{
    return (m_defaultData);
}

bool AttributeIpV4AddressUC::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

void AttributeIpV4AddressUC::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
        setValue(&m_defaultData);
    }
    else
    {
        IpV4AddressUC tempData= IpV4AddressUC();
        setValue(&tempData);
    }
    m_pData -> setIsUserConfigured(false);
}

map<string, string> AttributeIpV4AddressUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"; 
    
    return (supportedConversions);
}


void AttributeIpV4AddressUC::getCValue(WaveCValue *pCValue)
{
    IpV4Address ipadress = IpV4Address ((getValue ()).getIpV4AddressValue ());
    wave_cvalue_set_ipaddress (pCValue, &ipadress );
}

bool AttributeIpV4AddressUC::getIsUserConfigured ()
{
    return (m_pData->getIsUserConfigured ());
}

}
