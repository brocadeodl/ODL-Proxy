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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Attributes/AttributeUri.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"

namespace WaveNs
{

AttributeUri::AttributeUri (const Uri &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           ( AttributeType::AttributeTypeUri, attributeName, attributeUserTag, isOperational),
      m_isDefaultUriValid (false)
{
     m_pData = new Uri ();
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUri::AttributeUri (const Uri &data, const bool &defaultFlag, const Uri &defaultUri, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           ( AttributeType::AttributeTypeUri, attributeName, attributeUserTag, isOperational),
      m_defaultUri        (defaultUri),
      m_isDefaultUriValid (defaultFlag)
{
     m_pData = new Uri ();
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeUri::AttributeUri (Uri *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           ( AttributeType::AttributeTypeUri, attributeName, attributeUserTag, isOperational),
      m_isDefaultUriValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUri::AttributeUri (Uri *pData, const bool &defaultFlag, const Uri &defaultUri, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute           ( AttributeType::AttributeTypeUri, attributeName, attributeUserTag, isOperational),
      m_defaultUri        (defaultUri),
      m_isDefaultUriValid (defaultFlag)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUri::AttributeUri (const AttributeUri &attribute)
    : Attribute (attribute)
{
     m_pData             = new Uri                            ();
    *m_pData             = attribute.getValue                 ();
     m_defaultUri        = attribute.getDefaultUri            ();
     m_isDefaultUriValid = attribute.getIsDefaultUriValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeUri::~AttributeUri ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUri &AttributeUri::operator = (const AttributeUri &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultUri        = attribute.getDefaultUri            ();
    m_isDefaultUriValid = attribute.getIsDefaultUriValidFlag ();

    return (*this);
}

Uri AttributeUri::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUri::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (Uri ());
    }
}

void AttributeUri::setValue (const Uri &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUri::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUri::validate () const
{
    prismAssert ( AttributeType::AttributeTypeUri == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeUri == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUri::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeUri::getSqlType ()
{
    return ("varchar");
}

void AttributeUri::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUri::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUri::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUri::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUri::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUri::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUri::toString (string &valueString)
{
    m_pData->toString (valueString);
}

void AttributeUri::fromString (const string &valueString)
{
    m_pData->fromString (valueString);
}

void *AttributeUri::getPData ()
{
    return (m_pData);
}

void AttributeUri::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const Uri *> (pData));
}

Attribute *AttributeUri::clone ()
{
    AttributeUri *pAttributeString = new AttributeUri (*this);

    return (pAttributeString);
}

void AttributeUri::toEscapedString (string &valueString)
{
    string uriString;

    toString (uriString);

    UI32                size                  = uriString.size ();
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

    returnSize = PQescapeStringConn (pPGConnection, pTemp, uriString.c_str (), size, &errorCode);

    if (0 != errorCode)
    {
        databaseErrorMessage = PQerrorMessage (pPGConnection);
        trace (TRACE_LEVEL_ERROR, "AttributeUri::toEscapedString : error connecting with Database :\n" + databaseErrorMessage + string (", Return Size : ") + returnSize);
    }

    prismAssert (0 == errorCode, __FILE__, __LINE__);

    valueString = pTemp;

    delete [] pTemp;
}

bool AttributeUri::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if (true == (getIsDefaultUriValidFlag ()))
    {
        if ((getDefaultUri ()) == (getValue()))
        {
            isDefault = true;
        }
    }

    return (isDefault);
}

Uri AttributeUri::getDefaultUri () const
{
    return (m_defaultUri);
}

bool AttributeUri::getIsDefaultUriValidFlag () const
{
    return (m_isDefaultUriValid);
}

void AttributeUri::setDefaultValue()
{
    if(true == m_isDefaultUriValid)
    {
        setValue(&m_defaultUri);
    }
    else
    {
        *m_pData = Uri ();
    }
}

map<string, string> AttributeUri::getSupportedConversions ()
{
    map<string, string> supportedConversions;

    supportedConversions["WAVE_ATTRIBUTE_TYPE_STRING"] = "WAVE_ATTRIBUTE_TYPE_STRING";

    return (supportedConversions);
}

void AttributeUri::getCValue (WaveCValue *pCValue)
{
    trace (TRACE_LEVEL_FATAL, "AttributeUri::getCValue : Currently not supported");

    prismAssert (false, __FILE__, __LINE__);
}

}

