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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Attributes/AttributeBitMap.h"
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

AttributeBitMap::AttributeBitMap (const BitMap &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational, const bool force32bitPresentation)
    : Attribute (AttributeType::AttributeTypeBitMap, attributeName, attributeUserTag, isOperational)
{
    m_pData                     = new BitMap;
    *m_pData                    = data;
    m_isDefaultBitMapValid      = false;
    m_force32BitRepresentation  = force32bitPresentation;
    setIsMemoryOwnedByAttribute (true);
}

AttributeBitMap::AttributeBitMap (const BitMap &data, const BitMap &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational, const bool force32bitPresentation)
    : Attribute (AttributeType::AttributeTypeBitMap, attributeName, attributeUserTag, isOperational)
{
    m_pData                     = new BitMap;
    *m_pData                    = data;
    m_defaultBitMap             = defaultData;
    m_isDefaultBitMapValid      = true;
    m_force32BitRepresentation  = force32bitPresentation;
    setIsMemoryOwnedByAttribute (true);
}

AttributeBitMap::AttributeBitMap (BitMap *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational, const bool force32bitPresentation)
    : Attribute (AttributeType::AttributeTypeBitMap, attributeName, attributeUserTag, isOperational)
{
    m_pData                     = pData;
    m_isDefaultBitMapValid      = false;
    m_force32BitRepresentation  = force32bitPresentation;
    setIsMemoryOwnedByAttribute (false);
}

AttributeBitMap::AttributeBitMap (BitMap *pData, const BitMap &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational, const bool force32bitPresentation)
    : Attribute (AttributeType::AttributeTypeBitMap, attributeName, attributeUserTag, isOperational)
{
    m_pData                     = pData;
    m_defaultBitMap             = defaultData;
    m_isDefaultBitMapValid      = true;
    m_force32BitRepresentation  = force32bitPresentation; 
    setIsMemoryOwnedByAttribute (false);
}

AttributeBitMap::AttributeBitMap (const AttributeBitMap &attribute)
    : Attribute (attribute)
{
    m_pData                     = new BitMap;
    *m_pData                    = attribute.getValue ();
    m_defaultBitMap             = attribute.getDefaultBitMap();
    m_isDefaultBitMapValid      = attribute.getIsDefaultBitMapValidFlag();
    m_force32BitRepresentation  = attribute.getForce32BitRepresentationFlag (); 
    setIsMemoryOwnedByAttribute (true);
}

AttributeBitMap::~AttributeBitMap ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeBitMap &AttributeBitMap::operator = (const AttributeBitMap &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultBitMap        = attribute.getDefaultBitMap();
    m_isDefaultBitMapValid = attribute.getIsDefaultBitMapValidFlag();

    return (*this);
}

BitMap AttributeBitMap::getValue () const
{
    BitMap tempBitMap;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBitMap::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (tempBitMap);
    }
}

void AttributeBitMap::setValue (const BitMap &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeBitMap::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeBitMap::validate () const
{
    prismAssert (AttributeType::AttributeTypeBitMap == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeBitMap == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeBitMap::getSqlType ()
{
    return ("varchar");
}

void AttributeBitMap::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeBitMap::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeBitMap::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeBitMap::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeBitMap::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeBitMap::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeBitMap::toString (string &valueString)
{
    UI32   noOfBits   = m_pData->getNumberOfBits ();
    string tempString = m_pData->toString ();
    char   tempBuffer[1024] = {0};

    snprintf (tempBuffer, 1024, "%u:%s", noOfBits, tempString.c_str ());

    valueString = string (tempBuffer);
}

void AttributeBitMap::fromString (const string &valueString)
{
    string tempString;
    char   tempBuffer[1024] = {0};
    UI32   noOfBits;

    sscanf (valueString.c_str (), "%u:%s", &noOfBits, tempBuffer);

    tempString = string(tempBuffer);

    m_pData->fromString (tempString, noOfBits);
}

ResourceId AttributeBitMap::loadFromPlainString (const string &valueString)
{
    string tempString;
    char   tempBuffer[1024] = {0};
    UI32   noOfBits;

    if (sscanf (valueString.c_str (), "%u:%s", &noOfBits, tempBuffer) != 2)
    {
        return WAVE_MESSAGE_ERROR;
    }

    tempString = string(tempBuffer);

    return (m_pData->loadFromPlainString (tempString, noOfBits));
}

void *AttributeBitMap::getPData ()
{
    return (m_pData);
}

void AttributeBitMap::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const BitMap *> (pData));
}

Attribute *AttributeBitMap::clone ()
{
    AttributeBitMap *pAttributeBitMap = new AttributeBitMap (*this);

    return (pAttributeBitMap);
}

map<string, string> AttributeBitMap::getSupportedConversions ()
{

    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
   
    return (supportedConversions);
}

bool AttributeBitMap::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultBitMapValidFlag() )
    {   
        if ( getDefaultBitMap() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeBitMap::getIsDefaultBitMapValidFlag() const
{
    return m_isDefaultBitMapValid;
}

BitMap AttributeBitMap::getDefaultBitMap() const
{
    return (m_defaultBitMap);
}

void AttributeBitMap::setDefaultValue()
{
    if(true == m_isDefaultBitMapValid)
    {
        *m_pData = m_defaultBitMap;
    }
    else
    {
        m_pData->reset();
    }
}

bool AttributeBitMap::getForce32BitRepresentationFlag () const
{
    return (m_force32BitRepresentation);
}

void AttributeBitMap::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_bitmap (pCValue, getValue ());
}


void AttributeBitMap::populateNameValueBitMap (string name, UI32 bitPosition)
{

    m_bitNameToBitPositionMap[name]        = bitPosition;
    m_bitPositionToBitNameMap[bitPosition] = name;

}

void AttributeBitMap::getBitNameFromBitPosition (string &name, UI32 &bitPosition)
{
    map<UI32, string>::iterator it;

    it = m_bitPositionToBitNameMap.find (bitPosition);

    if (it != m_bitPositionToBitNameMap.end())
    {   
        name = it->second;
    }   
}

void AttributeBitMap::getBitPositionFromBitName (string &name, UI32 &bitPosition)
{
    map<string,UI32>::iterator it;

    it = m_bitNameToBitPositionMap.find (name);

    if (it != m_bitNameToBitPositionMap.end())
    {   
        bitPosition = it->second;
    }   

}


}
