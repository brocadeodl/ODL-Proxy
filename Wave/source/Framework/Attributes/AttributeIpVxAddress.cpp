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
 *   Copyright (C) 2005 - 2010 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "Framework/Attributes/AttributeIpVxAddress.h"
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

#include <algorithm>

namespace WaveNs
{

AttributeIpVxAddress::AttributeIpVxAddress (const IpVxAddress &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeIpVxAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new IpVxAddress (data);
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpVxAddress::AttributeIpVxAddress (const IpVxAddress &data, const IpVxAddress &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeIpVxAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = new IpVxAddress (data);
    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpVxAddress::AttributeIpVxAddress (IpVxAddress *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeIpVxAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;
    m_isDefaultDataValid = false;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpVxAddress::AttributeIpVxAddress (IpVxAddress *pData, const IpVxAddress &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute ( AttributeType::AttributeTypeIpVxAddress, attributeName, attributeUserTag, isOperational)
{
    m_pData              = pData;

    m_defaultData        = defaultData;
    m_isDefaultDataValid = true;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpVxAddress::AttributeIpVxAddress (const AttributeIpVxAddress &attribute)
    : Attribute (attribute)
{
    m_pData = new IpVxAddress (attribute.getValue ());
    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpVxAddress::~AttributeIpVxAddress ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpVxAddress &AttributeIpVxAddress::operator = (const AttributeIpVxAddress &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag();

    return (*this);
}

IpVxAddress AttributeIpVxAddress::getValue () const
{
    IpVxAddress tempIpVxAddres;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpVxAddress::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempIpVxAddres);
    }
}

void AttributeIpVxAddress::setValue (const IpVxAddress &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpVxAddress::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpVxAddress::validate () const
{
    prismAssert ( AttributeType::AttributeTypeIpVxAddress == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeIpVxAddress == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeIpVxAddress::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeIpVxAddress::getSqlType ()
{
    return ("inet");
}

void AttributeIpVxAddress::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpVxAddress::getSqlForCreate ()
{
    return (getAttributeName () + string(" ") + getSqlType ());
}

void AttributeIpVxAddress::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
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

void AttributeIpVxAddress::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
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

void AttributeIpVxAddress::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpVxAddress::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpVxAddress::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeIpVxAddress::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

ResourceId AttributeIpVxAddress::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeIpVxAddress::getPData ()
{
    return (m_pData);
}

void AttributeIpVxAddress::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const IpVxAddress *> (pData));
}

Attribute *AttributeIpVxAddress::clone ()
{
    AttributeIpVxAddress *pAttributeIpVxAddress = new AttributeIpVxAddress (*this);

    return (pAttributeIpVxAddress);
}

map<string, string> AttributeIpVxAddress::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"; 
    
    return (supportedConversions);
}

bool AttributeIpVxAddress::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultValue() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeIpVxAddress::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

IpVxAddress AttributeIpVxAddress::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeIpVxAddress::setDefaultValue()
{
   if(true == m_isDefaultDataValid)
    {
        *m_pData = m_defaultData;
    }
    else
    {
        IpVxAddress tempData;
        setValue(&tempData);
    }
}

void AttributeIpVxAddress::getCValue(WaveCValue *pCValue)
{
   wave_cvalue_set_ipaddress (pCValue, (IpVxAddress *) getPData ());
}

AttributeIpVxAddressVector::AttributeIpVxAddressVector (const vector<IpVxAddress> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpVxAddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData  = new vector<IpVxAddress>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpVxAddressVector::AttributeIpVxAddressVector (const vector<IpVxAddress> &data, const vector<IpVxAddress> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpVxAddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new vector<IpVxAddress>;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeIpVxAddressVector::AttributeIpVxAddressVector (vector<IpVxAddress> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpVxAddressVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpVxAddressVector::AttributeIpVxAddressVector (vector<IpVxAddress> *pData, const vector<IpVxAddress> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeIpVxAddressVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeIpVxAddressVector::AttributeIpVxAddressVector (const AttributeIpVxAddressVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<IpVxAddress>;
    *m_pData             = attribute.getValue ();
    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = false;
    setIsMemoryOwnedByAttribute (true);
}

AttributeIpVxAddressVector::~AttributeIpVxAddressVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeIpVxAddressVector &AttributeIpVxAddressVector::operator = (const AttributeIpVxAddressVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultValue();
    m_isDefaultDataValid = false;

    return (*this);
}

vector<IpVxAddress> AttributeIpVxAddressVector::getValue () const
{
    vector<IpVxAddress> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpVxAddressVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeIpVxAddressVector::setValue (const vector<IpVxAddress> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeIpVxAddressVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeIpVxAddressVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeIpVxAddressVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeIpVxAddressVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeIpVxAddressVector::getSqlType ()
{
    return ("varchar");
}

void AttributeIpVxAddressVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeIpVxAddressVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeIpVxAddressVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");
    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeIpVxAddressVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeIpVxAddressVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeIpVxAddressVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeIpVxAddressVector::toString (string &valueString)
{
    vector<IpVxAddress>::iterator element = (*m_pData).begin ();
    vector<IpVxAddress>::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + ((*element).toString ()).size () + "#" + (*element).toString ();
        element++;
    }
}

void AttributeIpVxAddressVector::fromString (const string &valueString)
{
    vector<IpVxAddress>::iterator begin = (*m_pData).begin ();
    vector<IpVxAddress>::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI32 stringSize     = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        stringSize = atoi (valueString.c_str () + startFromIndex);

        (*m_pData).push_back (valueString.substr (firstIndex + 1, stringSize));

        startFromIndex = firstIndex + stringSize + 1;
    }
}

void *AttributeIpVxAddressVector::getPData ()
{
    return (m_pData);
}

void AttributeIpVxAddressVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<IpVxAddress> *> (pData));
}

Attribute *AttributeIpVxAddressVector::clone ()
{
    AttributeIpVxAddressVector *pAttributeIpVxAddressVector = new AttributeIpVxAddressVector (*this);

    return (pAttributeIpVxAddressVector);
}

map<string, string> AttributeIpVxAddressVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"; 
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"; 
    
    return (supportedConversions);
}

bool AttributeIpVxAddressVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultDataValidFlag() )
    {   
        if ( getDefaultValue() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeIpVxAddressVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

vector<IpVxAddress> AttributeIpVxAddressVector::getDefaultValue() const
{
    return (m_defaultData);
}

void AttributeIpVxAddressVector::setDefaultValue()
{
    if(true == m_isDefaultDataValid)
    {
         setValue(&m_defaultData);
    }
    else
    {
        (*m_pData).clear();
    }
}

void AttributeIpVxAddressVector::getCValue(WaveCValue *pCValue)
{
    wave_cvalue_set_ipvxaddress_vector (pCValue, getValue () );
}


void AttributeIpVxAddressVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<IpVxAddress>;        
    }

    vector<IpVxAddress> vectorToAppend = (dynamic_cast<AttributeIpVxAddressVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (),vectorToAppend.begin (),vectorToAppend.end ());

}

void AttributeIpVxAddressVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<IpVxAddress>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<IpVxAddress> vectorToDelete = (dynamic_cast<AttributeIpVxAddressVector *> (attribute))->getValue ();
    vector<IpVxAddress>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeIpVxAddressVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

                                                                                                                                  
}
