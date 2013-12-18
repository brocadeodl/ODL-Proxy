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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Attributes/AttributeStringVectorVector.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/OrmColumn.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmAssociation.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

AttributeStringVectorVector::AttributeStringVectorVector (const vector<vector<string> > &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = new vector<vector<string> > (data);

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVectorVector::AttributeStringVectorVector (const vector<vector<string> > &data, const vector<vector<string> > &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = new vector<vector<string> > (data);

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVectorVector::AttributeStringVectorVector (vector<vector<string> > *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeStringVectorVector::AttributeStringVectorVector (vector<vector<string> > *pData, const vector<vector<string> > &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeStringVector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeStringVectorVector::AttributeStringVectorVector (const AttributeStringVectorVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<vector<string> > (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();
    setIsMemoryOwnedByAttribute (true);
}

AttributeStringVectorVector::~AttributeStringVectorVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeStringVectorVector &AttributeStringVectorVector::operator = (const AttributeStringVectorVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();
    return (*this);
}

vector<vector<string> > AttributeStringVectorVector::getValue () const
{
    vector<vector<string> > temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringVectorVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeStringVectorVector::setValue (const vector<vector<string> > &data)
{
    if (true == (validate ()))
    {
        UI32 numberOfEntries = data.size ();
        UI32 i;

        for (i = 0; i < numberOfEntries; i++)
        {
            (*m_pData)[i] = data[i];
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringVectorVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeStringVectorVector::validate () const
{
    prismAssert (AttributeType::AttributeTypeStringVector == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeStringVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeStringVectorVector::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
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

string AttributeStringVectorVector::getSqlType ()
{
    return ("varchar");
}

void AttributeStringVectorVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeStringVectorVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeStringVectorVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeStringVectorVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeStringVectorVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeStringVectorVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeStringVectorVector::toString (string &valueString)
{
    vector<vector<string> >::iterator element = (*m_pData).begin ();
    vector<vector<string> >::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        AttributeStringVector tempAttribute (&(*element));
        string                tempString;

        tempAttribute.toString (tempString);

        valueString = valueString + tempString.size () + "#" + tempString;

        element++;
    }
}

void AttributeStringVectorVector::fromString (const string &valueString)
{
    vector<vector<string> >::iterator begin = (*m_pData).begin ();
    vector<vector<string> >::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI                   startFromIndex    = 0;
    ULI                   firstIndex        = 0;
    UI32                  stringSize        = 0;
    vector<string>        tempStringVector;
    AttributeStringVector tempAttribute     (tempStringVector);
    string                tempString;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        stringSize = atoi (valueString.c_str () + startFromIndex);

        tempString = valueString.substr (firstIndex + 1, stringSize);

        tempAttribute.fromString (tempString);

        (*m_pData).push_back (tempAttribute.getValue ());

        startFromIndex = firstIndex + stringSize + 1;
    }
}

void *AttributeStringVectorVector::getPData ()
{
    return (m_pData);
}

void AttributeStringVectorVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<vector<string> > *> (pData));
}

Attribute *AttributeStringVectorVector::clone ()
{
    AttributeStringVectorVector *pAttributeStringVector = new AttributeStringVectorVector (*this);

    return (pAttributeStringVector);
}

void AttributeStringVectorVector::toEscapedString (string &valueString)
{
    vector<vector<string> >::iterator  element             = (*m_pData).begin ();
    vector<vector<string> >::iterator  end                 = (*m_pData).end ();

    DatabaseConnection       *pDatabaseconnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn                   *pPGConnection       = NULL;

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    valueString = "";

    while (end != element)
    {
        AttributeStringVector tempAttribute (&(*element));
        string                tempString;

        tempAttribute.toString (tempString);

        UI32  size       = tempString.size ();
        char *pTemp      = new char[2 * size + 1];
        UI32  returnSize = 0;
        SI32  errorCode  = 0;

        prismAssert (NULL != pTemp, __FILE__, __LINE__);

        returnSize = PQescapeStringConn (pPGConnection, pTemp, tempString.c_str (), size, &errorCode);

        prismAssert (0 == errorCode, __FILE__, __LINE__);

        valueString = valueString + returnSize + "#" + pTemp;

        delete [] pTemp;

        element++;
    }
}

bool AttributeStringVectorVector::isCurrentValueSameAsDefault () const
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

vector<vector<string> > AttributeStringVectorVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeStringVectorVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeStringVectorVector::setDefaultValue()
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

map<string, string> AttributeStringVectorVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"]                 = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_UC"]              = "WAVE_ATTRIBUTE_TYPE_STRING_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"]          = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC"]       = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"]                 = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_UC"]              = "WAVE_ATTRIBUTE_TYPE_STRING_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"]      = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"]     = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"]     = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATE_VECTOR"]            = "WAVE_ATTRIBUTE_TYPE_DATE_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_TIME_VECTOR"]            = "WAVE_ATTRIBUTE_TYPE_TIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR"]        = "WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"]     = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"]     = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"]      = "WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"]    = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_RANGE"]             = "WAVE_ATTRIBUTE_TYPE_UI32_RANGE";

    return (supportedConversions);
}

void AttributeStringVectorVector::getCValue (WaveCValue *pCValue)
{  
    prismAssert (false, __FILE__, __LINE__); 
}

}
