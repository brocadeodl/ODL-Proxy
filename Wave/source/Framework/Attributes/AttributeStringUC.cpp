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

#include "Framework/Attributes/AttributeStringUC.h"
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

AttributeStringUC::AttributeStringUC (const StringUC &data, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringUC, attributeName, attributeUserTag, isOperational),
      m_defaultString        (""),
      m_isDefaultStringValid (false)
{
     m_pData       = new StringUC;
    *m_pData       = data;
     m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringUC::AttributeStringUC (const StringUC &data, const bool &isNoElement, const bool &defaultFlag, const string &defaultString, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringUC, attributeName, attributeUserTag, isOperational),
      m_defaultString        (defaultString),
      m_isDefaultStringValid (defaultFlag)
{
     m_pData       = new StringUC;
    *m_pData       = data;
     m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringUC::AttributeStringUC (StringUC *pData, const bool &isNoElement, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringUC, attributeName, attributeUserTag, isOperational),
      m_defaultString        (""),
      m_isDefaultStringValid (false)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeStringUC::AttributeStringUC (StringUC *pData, const bool &isNoElement, const bool &defaultFlag, const string &defaultString, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute              (AttributeType::AttributeTypeStringUC, attributeName, attributeUserTag, isOperational),
      m_defaultString        (defaultString),
      m_isDefaultStringValid (defaultFlag)
{
    m_pData       = pData;
    m_isNoElement = isNoElement;

    setIsMemoryOwnedByAttribute (false);
}

AttributeStringUC::AttributeStringUC (const AttributeStringUC &attribute)
    : Attribute (attribute)
{
    m_pData                = new StringUC;

    *m_pData               = attribute.getValue ();
    m_isNoElement          = attribute.getIsNoElement ();
    m_defaultString        = attribute.getDefaultString ();
    m_isDefaultStringValid = attribute.getIsDefaultStringValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeStringUC::~AttributeStringUC ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeStringUC &AttributeStringUC::operator = (const AttributeStringUC &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());
    m_isNoElement          = attribute.getIsNoElement ();
    m_defaultString        = attribute.getDefaultString ();
    m_isDefaultStringValid = attribute.getIsDefaultStringValidFlag ();

    return (*this);
}

StringUC AttributeStringUC::getValue () const
{
    StringUC tempStringUC;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringUC::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return tempStringUC;
    }
}

void AttributeStringUC::setValue (const StringUC &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeStringUC::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeStringUC::getIsNoElement () const
{
    return (m_isNoElement);
}

bool AttributeStringUC::validate () const
{
    prismAssert (AttributeType::AttributeTypeStringUC == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeStringUC == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeStringUC::getSqlType ()
{
    return ("varchar");
}

void AttributeStringUC::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeStringUC::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeStringUC::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString, tempString1;

    toString (tempString);

    toEscapedString (&tempString, tempString1);
    sqlForInsert += string (",'") + tempString1 + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeStringUC::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString, tempString1;

    toString (tempString);

    toEscapedString (&tempString, tempString1);
    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString1 + string ("'");
}

void AttributeStringUC::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeStringUC::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString, tempString1;

    toString (tempString);

    toEscapedString (&tempString, tempString1);
    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString1 + string ("'");
}

void AttributeStringUC::toString (string &valueString)
{
    valueString = m_pData->toString ();
}

void AttributeStringUC::fromString (const string &valueString)
{
    m_pData->fromString(valueString);
}

void AttributeStringUC::getPlainString (string &valueString)
{
     m_pData->getPlainString (valueString);
}

ResourceId AttributeStringUC::loadFromPlainString (const string &valueString)
{
    return (m_pData->loadFromPlainString (valueString));
}

void *AttributeStringUC::getPData ()
{
    return (m_pData);
}

void AttributeStringUC::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const StringUC *> (pData));
}

Attribute *AttributeStringUC::clone ()
{
    AttributeStringUC *pAttributeStringUC = new AttributeStringUC (*this);

    return (pAttributeStringUC);
}

bool AttributeStringUC::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultStringValidFlag() )
    {   
        if ( getDefaultString() == (getValue()).getStringValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

string AttributeStringUC::getDefaultString () const
{
    return (m_defaultString);
}

bool AttributeStringUC::getIsDefaultStringValidFlag () const
{
    return (m_isDefaultStringValid);
}

void AttributeStringUC::toEscapedString (string *inputString, string &valueString)
{
    UI32                size                  = inputString->size ();
    char               *pTemp                 = new char[2 * size + 1];
    UI32                returnSize            = 0;
    SI32                errorCode             = 0;
    DatabaseConnection *pDatabaseconnection   = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn             *pPGConnection         = NULL;
    string              databaseErrorMessage;

    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    returnSize = PQescapeStringConn (pPGConnection, pTemp, inputString->c_str (), size, &errorCode);

    if (0 != errorCode)
    {
        databaseErrorMessage = PQerrorMessage (pPGConnection);
        trace (TRACE_LEVEL_ERROR, "AttributeStringUC::toEscapedString : error connecting with Database :\n" + databaseErrorMessage + string (", Return Size : ") + returnSize);
    }

    prismAssert (0 == errorCode, __FILE__, __LINE__);

    valueString = pTemp;

    delete [] pTemp;
}

map<string, string> AttributeStringUC::getSupportedConversions ()
{
    map<string, string> supportedConversions;

    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UNION"] = "WAVE_ATTRIBUTE_TYPE_UNION";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK"] = "WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MAC_RANGE"] = "WAVE_ATTRIBUTE_TYPE_MAC_RANGE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR"] = "WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC"] = "WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC"] = "WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_HOST_UC"] = "WAVE_ATTRIBUTE_TYPE_HOST_UC";

    return (supportedConversions);
}


void AttributeStringUC::setDefaultValue()
{
    if(true == m_isDefaultStringValid)
    {
        setValue(&m_defaultString);
    }
    else
    {
        m_pData->setStringValue("");
    }
    m_pData->setIsUserConfigured(false);
}

void AttributeStringUC::getCValue (WaveCValue *pCValue)
{
	wave_cvalue_set_string (pCValue, (getValue ()).getStringValue ());	
}

bool AttributeStringUC::getIsUserConfigured ()
{
    return (m_pData->getIsUserConfigured ());
}

}
