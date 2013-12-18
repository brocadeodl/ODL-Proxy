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
 *   Copyright (C) 2005 - 2012 Brocade Communications Systems, Inc.        *
 *   All rights reserved.                                                  *
 *   Author : Chaitanya Gangwar                                            *
 ***************************************************************************/

#include "Framework/Attributes/AttributeUI32VectorVector.h"
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

AttributeUI32VectorVector::AttributeUI32VectorVector (const vector<vector<UI32> > &data, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = new vector<vector<UI32> > (data);

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32VectorVector::AttributeUI32VectorVector (const vector<vector<UI32> > &data, const vector<vector<UI32> > &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = new vector<vector<UI32> > (data);

    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32VectorVector::AttributeUI32VectorVector (vector<vector<UI32> > *pData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_isDefaultDataValid (false)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32VectorVector::AttributeUI32VectorVector (vector<vector<UI32> > *pData, const vector<vector<UI32> > &defaultData, const string &attributeName, const UI32 &attributeUserTag, const bool &isOperational)
    : Attribute          (AttributeType::AttributeTypeUI32Vector, attributeName, attributeUserTag, isOperational),
    m_defaultData        (defaultData),
    m_isDefaultDataValid (true)
{
    m_pData = pData;

    setIsMemoryOwnedByAttribute (false);
}

AttributeUI32VectorVector::AttributeUI32VectorVector (const AttributeUI32VectorVector &attribute)
    : Attribute (attribute)
{
    m_pData              = new vector<vector<UI32> > (attribute.getValue ());
    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();
    setIsMemoryOwnedByAttribute (true);
}

AttributeUI32VectorVector::~AttributeUI32VectorVector ()
{
    if (true == (getIsMemoryOwnedByAttribute ()))
    {
        delete m_pData;
    }
}

AttributeUI32VectorVector &AttributeUI32VectorVector::operator = (const AttributeUI32VectorVector &attribute)
{
    Attribute::operator = (attribute);

    setValue (attribute.getValue ());

    m_defaultData        = attribute.getDefaultData ();
    m_isDefaultDataValid = attribute.getIsDefaultDataValidFlag ();
    return (*this);
}

vector<vector<UI32> > AttributeUI32VectorVector::getValue () const
{
    vector<vector<UI32> > temp;

    if (true == (validate ()))
    {
        return (*m_pData);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32VectorVector::getValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
        return (temp);
    }
}

void AttributeUI32VectorVector::setValue (const vector<vector<UI32> > &data)
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
        trace (TRACE_LEVEL_FATAL, string ("AttributeUI32VectorVector::setValue : Invalid Cast of the underlying Attribute."));
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool AttributeUI32VectorVector::validate () const
{
    prismAssert (AttributeType::AttributeTypeUI32Vector == (getAttributeType ()), __FILE__, __LINE__);

    if (AttributeType::AttributeTypeUI32Vector == (getAttributeType ()))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool AttributeUI32VectorVector::isConditionOperatorSupported (AttributeConditionOperator attributeConditionOperator)
{
    bool isSupported = false;
    
    if (WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL                     == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL                 == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN              == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO  == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN                 == attributeConditionOperator ||
        WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN_OR_EQUAL_TO     == attributeConditionOperator)
    {
        isSupported = true;
    }
    else
    {
        isSupported = false;
    }

    return (isSupported);
}

string AttributeUI32VectorVector::getSqlType ()
{
    return ("varchar");
}

void AttributeUI32VectorVector::setupOrm (OrmTable *pOrmTable)
{
    pOrmTable->addColumn (new OrmColumn (getAttributeName (), getSqlType (), getAttributeTypeString (), getIsPrimary ()));
}

string AttributeUI32VectorVector::getSqlForCreate ()
{
    return (getAttributeName () + " varchar");
}

void AttributeUI32VectorVector::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForInsert += string (",'") + tempString + string ("'");

    sqlForPreValues += string (",") + getAttributeName ();
}

void AttributeUI32VectorVector::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, bool isFirst)
{
    string tempString;

    toString (tempString);

    sqlForUpdate += "," + getAttributeName () + " = " + string ("'") + tempString + string ("'");
}

void AttributeUI32VectorVector::getSqlForSelect (string &sqlForSelect, AttributeConditionOperator attributeConditionOperator)
{
    if (false == isConditionOperatorSupported (attributeConditionOperator))
    {
        trace (TRACE_LEVEL_FATAL, "AttributeUI32VectorVector::getSqlForSelect : This attribute does not support the condition operator:" + FrameworkToolKit::localize (attributeConditionOperator));
        prismAssert (false, __FILE__, __LINE__);
    }

    string tempString;

    toString (tempString);

    sqlForSelect += getAttributeName () + FrameworkToolKit::localize (attributeConditionOperator) + string ("'") + tempString + string ("'");
}

void AttributeUI32VectorVector::toString (string &valueString)
{
    vector<vector<UI32> >::iterator element = (*m_pData).begin ();
    vector<vector<UI32> >::iterator end     = (*m_pData).end ();

    valueString = "";

    while (end != element)
    {
        AttributeUI32Vector   tempAttribute (&(*element));
        string                tempString;

        tempAttribute.toString (tempString);

        valueString = valueString + tempString.size () + "#" + tempString;

        element++;
    }
}

void AttributeUI32VectorVector::fromString (const string &valueString)
{
    vector<vector<UI32> >::iterator begin = (*m_pData).begin ();
    vector<vector<UI32> >::iterator end   = (*m_pData).end ();

    (*m_pData).erase (begin, end);

    ULI                   startFromIndex    = 0;
    ULI                   firstIndex        = 0;
    UI32                  stringSize        = 0;
    vector<UI32>          tempUI32Vector;
    AttributeUI32Vector   tempAttribute     (tempUI32Vector);
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

void *AttributeUI32VectorVector::getPData ()
{
    return (m_pData);
}

void AttributeUI32VectorVector::setValue (const void *pData)
{
    *m_pData = *(reinterpret_cast<const vector<vector<UI32> > *> (pData));
}

Attribute *AttributeUI32VectorVector::clone ()
{
    AttributeUI32VectorVector *pAttributeUI32Vector = new AttributeUI32VectorVector (*this);

    return (pAttributeUI32Vector);
}

bool AttributeUI32VectorVector::isCurrentValueSameAsDefault () const
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

vector<vector<UI32> > AttributeUI32VectorVector::getDefaultData() const
{
    return (m_defaultData);
}

bool AttributeUI32VectorVector::getIsDefaultDataValidFlag() const
{
    return (m_isDefaultDataValid);
}

void AttributeUI32VectorVector::setDefaultValue()
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

map<string, string> AttributeUI32VectorVector::getSupportedConversions ()
{
    map<string, string> supportedConversions;
    
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY"] = "WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID"] = "WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE"] = "WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_LOCATIONID"] = "WAVE_ATTRIBUTE_TYPE_LOCATIONID";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI64"] = "WAVE_ATTRIBUTE_TYPE_SI64";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI32"] = "WAVE_ATTRIBUTE_TYPE_SI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI16"] = "WAVE_ATTRIBUTE_TYPE_SI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_SI8"] = "WAVE_ATTRIBUTE_TYPE_SI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32"] = "WAVE_ATTRIBUTE_TYPE_UI32";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI16"] = "WAVE_ATTRIBUTE_TYPE_UI16";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI8"] = "WAVE_ATTRIBUTE_TYPE_UI8";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI64_UC"] = "WAVE_ATTRIBUTE_TYPE_UI64_UC";
    supportedConversions["WAVE_ATTRIBUTE_TYPE_UI32_UC"] = "WAVE_ATTRIBUTE_TYPE_UI32_UC";

    return (supportedConversions);
}

void AttributeUI32VectorVector::getCValue (WaveCValue *pCValue)
{  
    prismAssert (false, __FILE__, __LINE__); 
}

}
