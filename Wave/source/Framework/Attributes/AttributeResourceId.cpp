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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectRelationalMapping/OrmColumn.h"
#include "Framework/ObjectRelationalMapping/OrmRelation.h"
#include "Framework/ObjectRelationalMapping/OrmAssociation.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/Database/DatabaseConnection.h"
#include "Framework/Database/DatabaseObjectManager.h"

#include <algorithm>

namespace WaveNs
{

AttributeResourceId::AttributeResourceId (const UI32 &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeResourceId, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData  = new UI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeResourceId::AttributeResourceId (const UI32 &data, const UI32 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeResourceId, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData  = new UI32;
    *m_pData = data;

    setIsMemoryOwnedByAttribute (true);
}

AttributeResourceId::AttributeResourceId (UI32 *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeResourceId, attributeName, attributeUserTag, isOperational),
    m_defaultData        (0),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeResourceId::AttributeResourceId (UI32 *pData, const UI32 &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          ( AttributeType::AttributeTypeResourceId, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeResourceId::AttributeResourceId (const AttributeResourceId &attribute)
    : Attribute (attribute)
{
    m_pData              = new UI32;
    *m_pData             = attribute.getValue ();

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    setIsMemoryOwnedByAttribute (true);
}

AttributeResourceId::~AttributeResourceId ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeResourceId &AttributeResourceId::operator = (const AttributeResourceId &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();

    return (*this);
}

UI32 AttributeResourceId::getValue () const
{
    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeResourceId::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (0);
    }
}

void AttributeResourceId::setValue (const UI32 &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeResourceId::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeResourceId::validate () const
{
    prismAssert ( AttributeType::AttributeTypeResourceId == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeResourceId == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeResourceId::isConditionOperatorSupported ( AttributeConditionOperator attributeConditionOperator)
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

string AttributeResourceId::getSqlType ()
{
    return ("varchar");
}

void AttributeResourceId::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeResourceId::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeResourceId::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeResourceId::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeResourceId::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeResourceId::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeResourceId::toString (string &valueString)
{
    valueString = FrameworkToolKit::localizeToSourceCodeEnum (getValue ());
}

void AttributeResourceId::fromString (const string &valueString)
{
    setValue (FrameworkToolKit::localizeToSourceCodeResourceId (valueString));
}

void *AttributeResourceId::getPData ()
{
    return (m_pData);
}

void AttributeResourceId::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const UI32 *> (pData));
}

Attribute *AttributeResourceId::clone ()
{
    AttributeResourceId *pAttributeResourceId = new AttributeResourceId (*this);

    return (pAttributeResourceId);
}

bool AttributeResourceId::isCurrentValueSameAsDefault () const
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

UI32 AttributeResourceId::getDefaultData (void) const
{
    return (m_defaultData);
}

bool AttributeResourceId::getIsDefaultDataValidFlag (void) const
{
    return (m_isDefaultDataValid);
}

void AttributeResourceId::toEscapedString (string &valueString)
{
    string              tempString;
    UI32                returnSize            = 0;
    SI32                errorCode             = 0;
    DatabaseConnection *pDatabaseconnection   = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn             *pPGConnection         = NULL;
    string              databaseErrorMessage;

    toString (tempString);

    UI32                size                  = tempString.size ();
    char               *pTemp                 = new char[2 * size + 1];

    prismAssert (NULL != pTemp, __FILE__, __LINE__);

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    returnSize = PQescapeStringConn (pPGConnection, pTemp, tempString.c_str (), size, &errorCode);

    if (0 != errorCode)
    {
        databaseErrorMessage = PQerrorMessage (pPGConnection);
        trace (TRACE_LEVEL_ERROR, "AttributeResourceId::toEscapedString : error connecting with Database :\n" + databaseErrorMessage + string (", Return Size : ") + returnSize);
    }

    prismAssert (0 == errorCode, __FILE__, __LINE__);

    valueString = pTemp;

    delete [] pTemp;
}

map<string, string> AttributeResourceId::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

void AttributeResourceId::setDefaultValue()
{
   if(true == m_isDefaultDataValid)
   {
        setValue(&m_defaultData);
   }
   else
   {
        *m_pData = 0;
   }
}

void AttributeResourceId::getCValue(WaveCValue *pCValue)
{
   wave_cvalue_set_ui32 (pCValue, getValue ());
}

AttributeResourceIdVector::AttributeResourceIdVector (const vector<UI32> &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                      ( AttributeType::AttributeTypeResourceIdVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultResourceIdVectorValid (false)
{
    m_pData  = new vector<UI32>;
    *m_pData = data;
    setIsMemoryOwnedByAttribute (true);
}

AttributeResourceIdVector::AttributeResourceIdVector (const vector<UI32> &data, const vector<UI32> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                      ( AttributeType::AttributeTypeResourceIdVector, attributeName, attributeUserTag, isOperational),
    m_defaultResourceIdVector        (defaultData),
    m_isDefaultResourceIdVectorValid (true)
{
    m_pData  = new vector<UI32>;
    *m_pData = data;
    setIsMemoryOwnedByAttribute (true);
}

AttributeResourceIdVector::AttributeResourceIdVector (vector<UI32> *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                      ( AttributeType::AttributeTypeResourceIdVector, attributeName, attributeUserTag, isOperational),
    m_isDefaultResourceIdVectorValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeResourceIdVector::AttributeResourceIdVector (vector<UI32> *pData, const vector<UI32> &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute                      ( AttributeType::AttributeTypeResourceIdVector, attributeName, attributeUserTag, isOperational),
    m_defaultResourceIdVector        (defaultData),
    m_isDefaultResourceIdVectorValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeResourceIdVector::AttributeResourceIdVector (const AttributeResourceIdVector &attribute)
    : Attribute (attribute)
{
    m_pData                          = new vector<UI32>;
    *m_pData                         = attribute.getValue ();
    m_defaultResourceIdVector        = attribute.getDefaultResourceIdVector();
    m_isDefaultResourceIdVectorValid = attribute.getIsDefaultResourceIdVectorValidFlag();
    setIsMemoryOwnedByAttribute (true);
}

AttributeResourceIdVector::~AttributeResourceIdVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeResourceIdVector &AttributeResourceIdVector::operator = (const AttributeResourceIdVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultResourceIdVector        = attribute.getDefaultResourceIdVector();
    m_isDefaultResourceIdVectorValid = attribute.getIsDefaultResourceIdVectorValidFlag();

    return (*this);
}

vector<UI32> AttributeResourceIdVector::getValue () const
{
    vector<UI32> temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeResourceIdVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeResourceIdVector::setValue (const vector<UI32> &data)
{
    if (true == (validate ()))
    {
        *m_pData = data;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeResourceIdVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeResourceIdVector::validate () const
{
    prismAssert ( AttributeType::AttributeTypeResourceIdVector == (getAttributeType ()), __FILE__, __LINE__);

    if ( AttributeType::AttributeTypeResourceIdVector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

string AttributeResourceIdVector::getSqlType ()
{
    return ("varchar");
}

void AttributeResourceIdVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeResourceIdVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeResourceIdVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeResourceIdVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toEscapedString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeResourceIdVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeResourceIdVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toEscapedString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeResourceIdVector::toString (string &valueString)
{
    vector<UI32>::iterator element = (*m_pData).begin ();
    vector<UI32>::iterator end     = (*m_pData).end ();
    string tempString;

    valueString = "";

    while (end != element)
    {
        tempString = FrameworkToolKit::localizeToSourceCodeEnum (*element);
        valueString = valueString + tempString.size () + "#" + tempString;
        element++;
    }
}

void AttributeResourceIdVector::fromString (const string &valueString)
{
    vector<UI32>::iterator begin        = (*m_pData).begin ();
    vector<UI32>::iterator end          = (*m_pData).end ();
    UI32                   resourceId   = 0;
    string                 tempString;

    (*m_pData).erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI32 stringSize     = 0;

    while (string::npos != (firstIndex = valueString.find ("#", startFromIndex)))
    {
        stringSize = atoi (valueString.c_str () + startFromIndex);

        tempString = valueString.substr (firstIndex + 1, stringSize);

        resourceId = FrameworkToolKit::localizeToSourceCodeResourceId (tempString);

        (*m_pData).push_back (resourceId);

        startFromIndex = firstIndex + stringSize + 1;
    }
}


void *AttributeResourceIdVector::getPData ()
{
    return (m_pData);
}

void AttributeResourceIdVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<UI32> *> (pData));
}

Attribute *AttributeResourceIdVector::clone ()
{
    AttributeResourceIdVector *pAttributeResourceIdVector = new AttributeResourceIdVector (*this);

    return (pAttributeResourceIdVector);
}

void AttributeResourceIdVector::toEscapedString (string &valueString)
{
    UI32                    size                = 0;
    char                    *pTemp              = NULL;
    UI32                    returnSize          = 0;
    SI32                    errorCode           = 0;
    vector<UI32>::iterator  element             = (*m_pData).begin ();
    vector<UI32>::iterator  end                 = (*m_pData).end ();
    string                  tempString;

    DatabaseConnection       *pDatabaseconnection = DatabaseConnection::getInstance (DatabaseObjectManager::getDatabaseName (), DatabaseObjectManager::getDatabasePort ());
    PGconn                   *pPGConnection       = NULL;

    prismAssert (NULL != pDatabaseconnection, __FILE__, __LINE__);

    pPGConnection = pDatabaseconnection->getPConnection ();

    prismAssert (NULL != pPGConnection, __FILE__, __LINE__);

    valueString = "";

    while (end != element)
    {
        tempString = FrameworkToolKit::localizeToSourceCodeEnum (*element);
        size       = tempString.size ();
        pTemp      = new char[2 * size + 1];
        returnSize = 0;
        errorCode  = 0;

        prismAssert (NULL != pTemp, __FILE__, __LINE__);

        returnSize = PQescapeStringConn (pPGConnection, pTemp, tempString.c_str (), size, &errorCode);

        prismAssert (0 == errorCode, __FILE__, __LINE__);

        valueString = valueString + returnSize + "#" + pTemp;

        delete [] pTemp;

        element++;
    }
}

map<string, string> AttributeResourceIdVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    return (supportedConversions);
}

bool AttributeResourceIdVector::isCurrentValueSameAsDefault () const
{
    bool isDefault = false ;

    if ( getIsDefaultResourceIdVectorValidFlag() )
    {   
        if ( getDefaultResourceIdVector() == getValue() )
        {   
            isDefault = true;
        }
    }

    return (isDefault);
}

bool AttributeResourceIdVector::getIsDefaultResourceIdVectorValidFlag() const
{
   return (m_isDefaultResourceIdVectorValid);
}

vector<UI32> AttributeResourceIdVector::getDefaultResourceIdVector() const
{
   return (m_defaultResourceIdVector);
}

void AttributeResourceIdVector::setDefaultValue()
{
   if(true == m_isDefaultResourceIdVectorValid)
   {
        *m_pData = m_defaultResourceIdVector;
   }
   else
   {
        (*m_pData).clear();
   }
}


void AttributeResourceIdVector::getCValue (WaveCValue *pCValue)
{
    wave_cvalue_set_ui32_vector (pCValue, getValue ());
}

void AttributeResourceIdVector::addAttributeToVector (Attribute *attribute)
{
    if (m_pData == NULL)
    {
        m_pData = new vector<UI32>;        
    }

    vector<UI32> vectorToAppend = (dynamic_cast<AttributeResourceIdVector *> (attribute))->getValue ();

    (*m_pData).insert ((*m_pData).end (), vectorToAppend.begin (), vectorToAppend.end ());
}

void AttributeResourceIdVector::deleteAttributeFromVector (Attribute *attribute)
{
    vector<ResourceId>::iterator iterOriginal = (*m_pData).begin ();
    
    vector<ResourceId> vectorToDelete = (dynamic_cast<AttributeResourceIdVector *> (attribute))->getValue ();
    vector<ResourceId>::iterator iterDelete = vectorToDelete.begin ();

    for ( ; iterDelete != vectorToDelete.end (); iterDelete++)
    {   
        iterOriginal = std::find ((*m_pData).begin (), (*m_pData).end (), *iterDelete);

        if (iterOriginal != (*m_pData).end ())
        {   
            (*m_pData).erase (iterOriginal);
        }
        else
        {   
            trace (TRACE_LEVEL_FATAL, string ("AttributeResourceIdVector::deleteAttributeFromVector : Element to be deleted not found ."));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

                                                                                                                                  
}
