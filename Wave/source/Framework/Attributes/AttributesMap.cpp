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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <sstream>
#include "Framework/Attributes/AttributesMap.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectRelationalMapping/OrmTable.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Attributes/AttributeTypes.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeSerializableObject.cpp"
#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/BitMap.h"
#include "Version/PrismVersion.h"


namespace WaveNs
{

AttributesMap::AttributesMap ()
    : m_nextAvailableAttributeTag (0),
      m_nextAvailableAttributeTagForMapping (0)
{
}

AttributesMap::~AttributesMap ()
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;

    m_attributesByName.clear ();

    while (element != limitingElement)
    {
        pAttribute = element->second;

        if (NULL != pAttribute)
        {
            delete pAttribute;
        }

        element++;
    }

    m_attributes.clear ();
}

void AttributesMap::copyFrom (const AttributesMap &attributesMap)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;
    UI32                              attributeTag    = 0;

    while (element != limitingElement)
    {
        attributeTag = element->first;
        pAttribute   = element->second;

        if (NULL != pAttribute)
        {
            map<UI32, Attribute *>::const_iterator  element1         = attributesMap.m_attributes.find (attributeTag);
            map<UI32, Attribute *>::const_iterator  limitingElement1 = attributesMap.m_attributes.end  ();

            if (limitingElement1 != element1)
            {
                Attribute *pAttribute1 = element1->second;
                void *pData = pAttribute1->getPData ();
                pAttribute->setValue (pData);
            }
            else
            {
                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);
        }

        element++;
    }
}

void AttributesMap::addAttributeForOrderToNameMapping (const string& attributeName)
{
    Attribute *pAttributeInMap = getAttribute (attributeName);

    if (NULL == pAttributeInMap)
    {
        trace (TRACE_LEVEL_FATAL, string("AttributesMap::addAttributeForOrderToNameMapping: attribute with name [") + attributeName + string ("] does not exists in the attributes map. Please check!"));
        prismAssert (false, __FILE__, __LINE__);
    }

    m_nextAvailableAttributeTagForMapping++;
    m_attributesOrderToNameMapping[m_nextAvailableAttributeTagForMapping] = attributeName;

}

void AttributesMap::addAttribute (Attribute *pAttribute)
{
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    // Currently we do not see a need to lock this since the same object should not be modified on multiple threads at the same time.

    // If required LOCK here
    m_nextAvailableAttributeTag++;
    UI32 attributeTag = m_nextAvailableAttributeTag;
    // If required UNLOCK here
    pAttribute->setAttributeTag (attributeTag);

    map<UI32, Attribute *>::iterator element         = m_attributes.find (attributeTag);
    map<UI32, Attribute *>::iterator limitingElement = m_attributes.end ();

    if (element == limitingElement)
    {
        m_attributes[attributeTag] = pAttribute;

        const UI32 attributeUserTag = pAttribute->getAttributeUserTag ();

        if (0 != attributeUserTag)
        {
            if (false == (isAKnownAttributeByUserTag (attributeUserTag)))
            {
                m_attributesByUserTag[attributeUserTag] = pAttribute;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, string ("AttributesMap::addAttribute : Trying to add an Attribute with a User Tag that already exists (") + attributeUserTag + ").");
                prismAssert (false, __FILE__, __LINE__);
            }
        }

        string attributeName = pAttribute->getAttributeName ();

        if ("" != attributeName)
        {
            if (false == (isAKnownAttributeByName (attributeName)))
            {
                m_attributesByName[attributeName] = pAttribute;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, string ("----------------------------------------------------------IMPORTANT-----------------------------------------------"));
                trace (TRACE_LEVEL_FATAL, string ("AttributesMap::addAttribute : Trying to add an attribute with a name that already exists (") + attributeName + ").");
                trace (TRACE_LEVEL_FATAL, string ("---------------Please, take care of it by changing the attribute name to be unique..------------------------------"));
                // prismAssert (false, __FILE__, __LINE__);

                // Pritee: This must be removed once above prismAssert are uncommented.
                if (true == attributeName.empty())
                {
                    // use the attributeTag as a name as push it to this map
                    stringstream tStream;
                    tStream << attributeTag;

                    attributeName = tStream.str();

                    m_attributesByName[attributeName] = pAttribute;
                }
            }
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, string ("----------------------------------------------------------IMPORTANT-----------------------------------------------"));
            trace (TRACE_LEVEL_FATAL, string ("AttributesMap::addAttribute :  An attribute does not have a name. All attributes in a message being added must have unique name."));
            trace (TRACE_LEVEL_FATAL, string ("---------------Please, take care of it by giving name to an attribute which will be unique...---------------------"));
            // prismAssert (false, __FILE__, __LINE__);

            // Pritee: This must be removed once above prismAssert are uncommented.
            if (true == attributeName.empty())
            {
                // use the attributeTag as a name as push it to this map
                stringstream tStream;
                tStream << attributeTag;

                attributeName = tStream.str();

                m_attributesByName[attributeName] = pAttribute;
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::addAttribute : Trying to add an Attribute with a Tag that already exists (") + attributeTag + ").");
        prismAssert (false, __FILE__, __LINE__);
    }
}

UI32 AttributesMap::getSize ()
{
    return (m_attributes.size ());
}

Attribute *AttributesMap::getAttribute (UI32 attributeTag)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.find (attributeTag);
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();

    if (element != limitingElement)
    {
        return (element->second);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::getAttribute : Attribute with tag (") + attributeTag + ") does not exist.");
        prismAssert (false, __FILE__, __LINE__);
        return (NULL);
    }
}

Attribute *AttributesMap::getAttributeAt (const UI32 index)
{
    if (index >= (m_attributes.size ()))
    {
        return (NULL);
    }

    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    UI32                              tempIndex       = 0;

    while (element != limitingElement)
    {
        if (tempIndex == index)
        {
            break;
        }
        else
        {
            tempIndex++;
            element++;
        }
    }

	prismAssert (element != limitingElement, __FILE__, __LINE__);
    return (element->second);
}

// Temporarily disabled all xerces related code.
#if 0
void AttributesMap::serializeTo (DOMDocument *pDomDocument)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->serializeTo (pDomDocument);

        element++;
    }
}
#endif

void AttributesMap::loadFromSerializedData (const string &serializedData)
{
    //trace (TRACE_LEVEL_DEVEL, string("AttributesMap:: loadFromSerializedData: serializedData :") + serializedData);
    map<string, Attribute *>::iterator  element                   = m_attributesByName.begin ();
    map<string, Attribute *>::iterator  limitingElement           = m_attributesByName.end ();
    string                              attributeTag              = "";
    Attribute                        *pAttribute                = NULL;
    string                            attributeString           = "";
    string                            attributeTagString1       = "";
    //string                            attributeTagString2       = "";
    ULI                               index1                    = 0;
    ULI                               index2                    = 0;
    ULI                               attributeStringStartIndex = 0;

    while (element != limitingElement)
    {
        attributeTag = element->first;
        pAttribute   = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        attributeTagString1 = (string ("<A")) + attributeTag + ">";
        index1             = serializedData.find (attributeTagString1);
        //attributeTagString2 = (string ("</A")) + attributeTag + ">";
        attributeTagString1.insert (1, 1, '/');
        index2             = serializedData.find (attributeTagString1);

        if ((string::npos != index1) && (string::npos != index2))
        {
            attributeStringStartIndex = index1 + attributeTagString1.size () - 1;
            attributeString           = serializedData.substr (attributeStringStartIndex, index2 - attributeStringStartIndex);

            pAttribute->fromString (attributeString);

            //cout << attributeTagString1 << attributeString << attributeTagString2 << endl;
        }

        element++;
    }
}

void AttributesMap::loadFromSerializedDataInAttributeOrderFormat (const string &serializedData)
{
    trace (TRACE_LEVEL_DEVEL, string("AttributesMap:: loadFromSerializedDataInAttributeOrderFormat: serializedData :") + serializedData);
    map<UI32, string>::iterator  element                   = m_attributesOrderToNameMapping.begin ();
    map<UI32, string>::iterator  limitingElement           = m_attributesOrderToNameMapping.end ();
    UI32                              attributeOrder       = 0;
    UI32                              attributeTag         = 0;
    string                            attributeName        = "";
    Attribute                        *pAttribute           = NULL;
    string                            attributeString      = "";
    string                            attributeTagString1  = "";
    string                            attributeTagString2  = "";
    ULI                               index1               = 0;
    ULI                               index2               = 0;
    ULI                               attributeStringStartIndex = 0;

    while (element != limitingElement)
    {
        attributeOrder  = element->first;
        attributeName   = element->second;
        pAttribute      = getAttribute (attributeName);

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        attributeTag    = attributeOrder;

        attributeTagString1 = (string ("<A")) + attributeTag + ">";
        index1             = serializedData.find (attributeTagString1);
        attributeTagString2 = (string ("</A")) + attributeTag + ">";
        index2             = serializedData.find (attributeTagString2);

        if ((string::npos != index1) && (string::npos != index2))
        {
            attributeStringStartIndex = index1 + attributeTagString1.size ();
            attributeString           = serializedData.substr (attributeStringStartIndex, index2 - attributeStringStartIndex);

            pAttribute->fromString (attributeString);

            //cout << attributeTagString1 << attributeString << attributeTagString2 << endl;
        }

        element++;
    }
}

void AttributesMap::serializeTo (string &serializedData)
{
    map<string, Attribute *>::iterator  element             = m_attributesByName.begin ();
    map<string, Attribute *>::iterator  limitingElement     = m_attributesByName.end ();
    string                              attributeTag        = "";
    Attribute                        *pAttribute          = NULL;
    string                            attributeString     = "";

    while (element != limitingElement)
    {
        attributeTag = element->first;
        pAttribute   = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        serializedData += (string ("<A")) + attributeTag + ">";

        pAttribute->toString (attributeString);

        serializedData += attributeString;

        serializedData += (string ("</A")) + attributeTag + ">";

        element++;
    }

    //trace (TRACE_LEVEL_DEVEL, string("AttributesMap::serializeTo : serializedData :") + serializedData);
    //cout << serializedData << endl;
}

void AttributesMap::serializeToInAttributeOrderFormat (string &serializedData)
{
    map<UI32, string>::iterator  element         = m_attributesOrderToNameMapping.begin ();
    map<UI32, string>::iterator  limitingElement = m_attributesOrderToNameMapping.end ();

    UI32                         attributeTag         = 0;
    string                       attributeName        = "";
    Attribute                   *pAttribute           = NULL;
    string                       attributeString      = "";

    while (element != limitingElement)
    {
        attributeTag    = element->first;
        attributeName   = element->second;
        pAttribute      = getAttribute (attributeName);

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);
/*
        if (AttributeType::AttributeTypeSerializableObject == pAttribute->getAttributeType ())
        {
            pAttribute->setMessageVersionForData ("0.0.0");
        }
*/

        serializedData += (string ("<A")) + attributeTag + ">";

        pAttribute->toString (attributeString);

        serializedData += attributeString;

        serializedData += (string ("</A")) + attributeTag + ">";

        element++;
    }

    trace (TRACE_LEVEL_DEVEL, string("AttributesMap::serializeToInAttributeOrderFormat : serializedData :") + serializedData);
    //cout << serializedData << endl;
}

string AttributesMap::getSqlForCreate ()
{
    map<UI32, Attribute *>::iterator  element             = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement     = m_attributes.end ();
    UI32                              attributeTag        = 0;
    Attribute                        *pAttribute          = NULL;
    string                            sqlStringForCreate;

    while (element != limitingElement)
    {
        attributeTag = element->first;
        pAttribute   = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        // If it is not the first attribute add a comma and a new line.

        if (1 != attributeTag)
        {
            sqlStringForCreate += ",\n";
        }

        sqlStringForCreate += "    ";
        sqlStringForCreate += pAttribute->getSqlForCreate ();

        element++;
    }

    return (sqlStringForCreate);
}

void AttributesMap::getSqlForInsert (string &sqlForPreValues, string &sqlForInsert, string &sqlForInsert2)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;
    bool                              isFirst         = true;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->getSqlForInsert (sqlForPreValues, sqlForInsert, sqlForInsert2, isFirst);

        if (true == isFirst)
        {
            isFirst = false;
        }

        element++;
    }
}

void AttributesMap::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;
    string                            tempSql;
    bool                              isFirst         = true;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->getSqlForUpdate (sqlForUpdate, sqlForUpdate2, isFirst);

        if (true == isFirst)
        {
            isFirst = false;
        }

        element++;
    }
}

void AttributesMap::getSqlForUpdate (string &sqlForUpdate, string &sqlForUpdate2, const vector<string> &attributeNames)
{
          Attribute *pAttribute         = NULL;
          string     tempSql;
          bool       isFirst            = true;
          UI32       i                  = 0;
    const UI32       numberOfAttributes = attributeNames.size ();

    for (i = 0; i < numberOfAttributes; i++)
    {
        const string attributeName = attributeNames[i];

        pAttribute = m_attributesByName[attributeName];

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->getSqlForUpdate (sqlForUpdate, sqlForUpdate2, isFirst);

        if (true == isFirst)
        {
            isFirst = false;
        }
    }
}

void AttributesMap::setupOrm (OrmTable *pOrmTable)
{
    map<UI32, Attribute *>::iterator  element             = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement     = m_attributes.end ();
    Attribute                        *pAttribute          = NULL;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->setupOrm (pOrmTable);

        element++;
    }
}

void AttributesMap::loadFromPostgresQueryResult (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager, const bool loadOneToManyRelationships, const bool loadCompositions)
{
    UI32       numberOfSelectAttributes = selectFields.size ();
    Attribute *pAttribute               = NULL;

    if (0 != numberOfSelectAttributes)
    {
        UI32   i            = 0;
        string selectField;

        for (i = 0; i < numberOfSelectAttributes; i++)
        {
            selectField = selectFields[i];

            pAttribute = m_attributesByName[selectField];

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if ((AttributeType::AttributeTypeObjectIdVector != pAttribute->getAttributeType ()) && (AttributeType::AttributeTypeCompositionVector != pAttribute->getAttributeType ()))
            {
                pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
            }
        }
    }
    else
    {
        map<UI32, Attribute *>::iterator  element             = m_attributes.begin ();
        map<UI32, Attribute *>::iterator  limitingElement     = m_attributes.end ();

        while (element != limitingElement)
        {
            pAttribute   = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (AttributeType::AttributeTypeObjectIdVector == pAttribute->getAttributeType ())
            {
                //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresQueryResults : Detected 1->* Associations for " + pAttribute->getAttributeName ());

                if (true == loadOneToManyRelationships)
                {
                    //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresQueryResults : Loading 1->* Associations for " + pAttribute->getAttributeName ());

                    pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
                }
                else
                {
                    // Do not load 1->* associations/aggregations
                }
            }
            else if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
            {
                //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresQueryResults : Detected 1->1 Compositions for " + pAttribute->getAttributeName ());

                if (true == loadCompositions)
                {
                    //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresQueryResults : Loading 1->1 Compositions for " + pAttribute->getAttributeName ());

                    pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
                }
                else
                {
                    // Do not load 1->1 composition
                }
            }
            else if (AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType ())
            {
                //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresQueryResults : Detected 1->* Compositions for " + pAttribute->getAttributeName ());

                if (true == loadOneToManyRelationships)
                {
                    if (true == loadCompositions)
                    {
                        //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresQueryResults : Loading 1->* Compositions for " + pAttribute->getAttributeName ());

                        pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
                    }
                    else
                    {
                        // Do not load 1->* compositions
                    }
                }
                else
                {
                    // Do not load 1->* compositions
                }
            }
            else
            {
                pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
            }

            element++;
        }
    }
}

void AttributesMap::loadFromPostgresAuxilliaryQueryResult (map<string, PGresult *> &auxilliaryResultsMap, const ObjectId &parentObjectId, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager, const bool loadCompositions)
{
    const UI32 numberOfSelectFields = selectFields.size ();

    if (0 == numberOfSelectFields)
    {
        map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
        map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
        Attribute                        *pAttribute      = NULL;

        while (element != limitingElement)
        {
            pAttribute   = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType ())
            {
                //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresAuxilliaryQueryResults : Detected 1->* Compositions for " + pAttribute->getAttributeName ());

                if (true == loadCompositions)
                {
                    //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresAuxilliaryQueryResults : Loading 1->* Compositions for " + pAttribute->getAttributeName ());

                    pAttribute->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, parentObjectId, schema, pWaveObjectManager);
                }
                else
                {
                    // Do not load 1->* compositions for composition vector attributes
                }
            }
            else
            {
                pAttribute->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, parentObjectId, schema, pWaveObjectManager);
            }

            element++;
        }
    }
    else
    {
        Attribute *pAttribute = NULL;
        UI32       i          = 0;

        for (i = 0; i < numberOfSelectFields; i++)
        {
            pAttribute = m_attributesByName[selectFields[i]];

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType ())
            {
                //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresAuxilliaryQueryResults : Detected 1->* Compositions for " + pAttribute->getAttributeName ());

                if (true == loadCompositions)
                {
                    //trace (TRACE_LEVEL_DEVEL, "AttributesMap::loadFromPostgresAuxilliaryQueryResults : Loading 1->* Compositions for " + pAttribute->getAttributeName ());

                    pAttribute->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, parentObjectId, schema, pWaveObjectManager);
                }
                else
                {
                    // Do not load 1->* compositions for composition vector attributes
                }
            }
            else
            {
                pAttribute->loadFromPostgresAuxilliaryQueryResult (auxilliaryResultsMap, parentObjectId, schema, pWaveObjectManager);
            }
        }
    }
}

bool AttributesMap::isAKnownAttributeByName (const string &attributeName)
{
    map<string, Attribute *>::iterator element         = m_attributesByName.find (attributeName);
    map<string, Attribute *>::iterator limitingElement = m_attributesByName.end ();

    if (limitingElement == element)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

void AttributesMap::computeAttributesByName ()
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;
    string                            attributeName;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        attributeName = pAttribute->getAttributeName ();

        if ("" != attributeName)
        {
            if (false == (isAKnownAttributeByName (attributeName)))
            {
                m_attributesByName[attributeName] = pAttribute;
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "AttributesMap::computeAttributesByName : There CANNOT be multiple attributes with the same name in a inheritance hierarchy.  Duplicate Name encountered : \"" + attributeName + "\"");
                prismAssert (false, __FILE__, __LINE__);
            }
        }

        element++;
    }
}

Attribute *AttributesMap::getAttribute (const string &attributeName)
{
    Attribute *pAttribute = NULL;

    if (true == (isAKnownAttributeByName (attributeName)))
    {
        pAttribute = m_attributesByName[attributeName];
    }
    else
    {
        pAttribute = NULL;
    }

    return (pAttribute);
}

bool AttributesMap::isAKnownAttributeByUserTag (const UI32 &attributeUserTag)
{
    map<UI32, Attribute *>::iterator  element    = m_attributesByUserTag.find (attributeUserTag);
    map<UI32, Attribute *>::iterator  endElement = m_attributesByUserTag.end  ();

    if (endElement == element)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

Attribute *AttributesMap::getAttributeByUserTag (const UI32 &attributeUserTag)
{
    Attribute *pAttribute = NULL;

    if (true == (isAKnownAttributeByUserTag (attributeUserTag)))
    {
        pAttribute = m_attributesByUserTag[attributeUserTag];
    }
    else
    {
        pAttribute = NULL;
    }

    return (pAttribute);
}

void AttributesMap::setUserTagForAttribute (const string &attributeName, const UI32 &attributeUserTag)
{
    Attribute *pAttribute = getAttribute (attributeName);

    if (NULL == pAttribute)
    {
        trace (TRACE_LEVEL_FATAL, "AttributesMap::setUserTagForAttribute : There is no Attribute Registered with name : \"" + attributeName + "\"");
        prismAssert (NULL != pAttribute, __FILE__, __LINE__);
        return;
    }

    UI32 attributeUserTagFromAttribute = pAttribute->getAttributeUserTag ();

    if (0 != attributeUserTagFromAttribute)
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::setUserTagForAttribute : Resetting the User Tag for Attribute with name \"") + attributeName + "\"." );
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::setUserTagForAttribute : Old Value : ") + attributeUserTagFromAttribute);
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::setUserTagForAttribute : New Value : ") + attributeUserTag);

        prismAssert (0 == attributeUserTagFromAttribute, __FILE__, __LINE__);
        return;
    }

    if (true == (isAKnownAttributeByUserTag (attributeUserTag)))
    {
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::setUserTagForAttribute : Attribute with User \"") + attributeUserTag + "\" already exists in Attributes Map by User Tag.  Unexpected inconsistency detected." );
        trace (TRACE_LEVEL_FATAL, string ("AttributesMap::setUserTagForAttribute : Inconsistency detected for Attribute with name \"") + attributeName + "\"." );

        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    pAttribute->setAttributeUserTag (attributeUserTag);

    m_attributesByUserTag[attributeUserTag] = pAttribute;
}

void AttributesMap::updateOrmRelations (const PrismPersistableObject *pPrismPersistableObject)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->updateOrmRelations (pPrismPersistableObject);

        element++;
    }
}

WaveManagedObject *AttributesMap::getComposedManagedObject (const ObjectId &childObjectId)
{
	UI32 attributeSize = getSize();
	WaveManagedObject *pComposedManagedObject = NULL;

	for (UI8 i = 0; i < attributeSize; i++)
	{
        Attribute *pAttribute = getAttributeAt (i);

		if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
		{
			pComposedManagedObject = pAttribute->getComposedManagedObject(childObjectId);
            break;
		}
		else if (AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType ())
		{
			pComposedManagedObject = pAttribute->getComposedManagedObject(childObjectId);
            break;
		}
	}

	return pComposedManagedObject;
}

void AttributesMap::getShowDump (string &showDump, const WaveManagedObjectShowType &showType)
{
    map<UI32, Attribute *>::iterator  element                    = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement            = m_attributes.end ();
    Attribute                        *pAttribute                 = NULL;
    string                            tempDump;
    UI32                              maximumattributeNameLength = 0;
    UI32                              attributeNameLength        = 0;
    UI32                              differenceLength           = 0;
    UI32                              i                          = 0;

    if (WAVE_MANAGED_OBJECT_SHOW_ALL == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            showDump += "\n " + pAttribute->getAttributeName () + " : ";

            pAttribute->toString (tempDump);

            showDump += tempDump;

            element++;
        }
    }
    else if (WAVE_MANAGED_OBJECT_SHOW_CONFIGURATION == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (false == (pAttribute->getIsOperational ()))
            {
                if (0 != (pAttribute->getAttributeUserTag ()))
                {
                    attributeNameLength = (pAttribute->getAttributeName ()).length ();

                    if (maximumattributeNameLength < attributeNameLength)
                    {
                        maximumattributeNameLength = attributeNameLength;
                    }
                }
            }

            element++;
        }

        element = m_attributes.begin ();

        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (false == (pAttribute->getIsOperational ()))
            {
                if (0 != (pAttribute->getAttributeUserTag ()))
                {
                    differenceLength = maximumattributeNameLength - ((pAttribute->getAttributeName ()).length ());

                    showDump += "\n " + pAttribute->getAttributeName ();

                    for (i = 0; i < differenceLength; i++)
                    {
                        showDump += " ";
                    }

                    showDump += " ";

                    pAttribute->toString (tempDump);

                    showDump += tempDump;
                }
            }

            element++;
        }
    }
    else if (WAVE_MANAGED_OBJECT_SHOW_OPERATIONAL == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (true == (pAttribute->getIsOperational ()))
            {
                showDump += "\n " + pAttribute->getAttributeName () + " : ";

                pAttribute->toString (tempDump);

                showDump += tempDump;
            }

            element++;
        }
    }

    showDump += "\n!";
}

bool AttributesMap::isDeletableForOperation (const WaveManagedObjectOperation &operation)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        if (NULL != pAttribute)
        {
            if (false == pAttribute->isDeletableForOperation (operation))
            {
                return false;
            }
        }

        element++;
    }

    return true;
}


void AttributesMap::getRawShowDump (string &showDump, const WaveManagedObjectShowType &showType)
{
    map<UI32, Attribute *>::iterator  element                    = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement            = m_attributes.end ();
    Attribute                        *pAttribute                 = NULL;
    string                            tempDump;

    if (WAVE_MANAGED_OBJECT_SHOW_ALL == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            pAttribute->toString (tempDump);

            showDump += tempDump + ":";

            element++;
        }
    }
    else if (WAVE_MANAGED_OBJECT_SHOW_CONFIGURATION == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (false == (pAttribute->getIsOperational ()))
            {
                if (0 != (pAttribute->getAttributeUserTag ()))
                {
                    pAttribute->toString (tempDump);

                    showDump += tempDump + ":";
                }
            }

            element++;
        }
    }
    else if (WAVE_MANAGED_OBJECT_SHOW_OPERATIONAL == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (true == (pAttribute->getIsOperational ()))
            {
                pAttribute->toString (tempDump);

                showDump += tempDump + ":";
            }

            element++;
        }
    }
}

void AttributesMap::getAttributeNamesForHtmlTable (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType)
{
    map<UI32, Attribute *>::iterator  element                    = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement            = m_attributes.end ();
    Attribute                        *pAttribute                 = NULL;
    string                            tempDump;

    attributeNamesForHtmlTable.clear ();

    if (WAVE_MANAGED_OBJECT_SHOW_ALL == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            attributeNamesForHtmlTable.push_back (pAttribute->getAttributeName ());

            element++;
        }
    }
    else if (WAVE_MANAGED_OBJECT_SHOW_CONFIGURATION == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (false == (pAttribute->getIsOperational ()))
            {
                if (0 != (pAttribute->getAttributeUserTag ()))
                {
                    attributeNamesForHtmlTable.push_back (pAttribute->getAttributeName ());
                }
            }

            element++;
        }
    }
    else if (WAVE_MANAGED_OBJECT_SHOW_OPERATIONAL == showType)
    {
        while (element != limitingElement)
        {
            pAttribute = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (true == (pAttribute->getIsOperational ()))
            {
                attributeNamesForHtmlTable.push_back (pAttribute->getAttributeName ());
            }

            element++;
        }
    }
}

void AttributesMap::getHtmlTableHeaderNames (const vector<string> &attributeNamesForHtmlTable, vector<string> &htmlTableHeaderNames, const WaveManagedObjectShowType &showType)
{
    const UI32 numberOfAttributeNamesForHtmlTable = attributeNamesForHtmlTable.size ();
          UI32 i                                  = 0;

    htmlTableHeaderNames.clear ();

    for (i = 0; i < numberOfAttributeNamesForHtmlTable; i++)
    {
        string     attributeNameForHtmlTable = attributeNamesForHtmlTable[i];
        Attribute *pAttribute                = m_attributesByName[attributeNameForHtmlTable];

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        htmlTableHeaderNames.push_back (pAttribute->getAttributeName ());
    }
}

void AttributesMap::getHtmlTableHeaderNamesString (const vector<string> &attributeNamesForHtmlTable, string &htmlTableHeaderNamesString, const WaveManagedObjectShowType &showType)
{
    const UI32           numberOfAttributeNamesForHtmlTable = attributeNamesForHtmlTable.size ();
          UI32           i                                  = 0;

    htmlTableHeaderNamesString = "";

    for (i = 0; i < numberOfAttributeNamesForHtmlTable; i++)
    {
        htmlTableHeaderNamesString += "<TH>" + attributeNamesForHtmlTable[i] + "</TH>\r\n";
    }
}

void AttributesMap::getHtmlTableRowData (const vector<string> &attributeNamesForHtmlTable, vector<string> &htmlTableRowData, const WaveManagedObjectShowType &showType)
{
    UI32 numberOfAttributeNamesForHtmlTable = attributeNamesForHtmlTable.size ();
    UI32 i                                  = 0;

    htmlTableRowData.clear ();

    for (i = 0; i < numberOfAttributeNamesForHtmlTable; i++)
    {
        string     attributeNameForHtmlTable = attributeNamesForHtmlTable[i];
        Attribute *pAttribute                = m_attributesByName[attributeNameForHtmlTable];
        string     tempString;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->toString (tempString);

        htmlTableRowData.push_back (tempString);
    }
}

void AttributesMap::getHtmlTableRowDataString (const vector<string> &attributeNamesForHtmlTable, string &htmlTableRowDataString, const WaveManagedObjectShowType &showType)
{
    UI32           numberOfAttributeNamesForHtmlTable = 0;
    UI32           i                                  = 0;
    vector<string> htmlTableRowData;

    getHtmlTableRowData (attributeNamesForHtmlTable, htmlTableRowData, showType);

    numberOfAttributeNamesForHtmlTable = htmlTableRowData.size ();

    prismAssert ((attributeNamesForHtmlTable.size ()) == numberOfAttributeNamesForHtmlTable, __FILE__, __LINE__);

    htmlTableRowDataString = "";

    for (i = 0; i < numberOfAttributeNamesForHtmlTable; i++)
    {
        htmlTableRowDataString += "    <TD>" + htmlTableRowData[i] + "</TD>\r\n";
    }
}

void AttributesMap::getRestRowData (const vector<string> &attributeNamesForRest, const vector<string> &restHeaderNames, const string &managedObjectClassNameForRest, string &restRowData, const WaveManagedObjectShowType &showType)
{
    const UI32           numberOfAttributeNamesForRest  = attributeNamesForRest.size ();
    const UI32           numberOfRestHeaderNames        = restHeaderNames.size       ();
          UI32           i                              = 0;
          string         nestedRestRowData;


    prismAssert (numberOfAttributeNamesForRest == numberOfRestHeaderNames, __FILE__, __LINE__);

    restRowData = "";

    if ("" != managedObjectClassNameForRest)
    {
        restRowData += "<" + managedObjectClassNameForRest + " ";
    }

    for (i = 0; i < numberOfAttributeNamesForRest; i++)
    {
        string         attributeNameForRest = attributeNamesForRest[i];
        string         restHeaderName       = restHeaderNames[i];
        Attribute     *pAttribute           = m_attributesByName[attributeNameForRest];
        string         tempString;
        AttributeType  attributeType;
        string         tempRestRowData;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        attributeType = pAttribute->getAttributeType ();

        if (AttributeType::AttributeTypeCompositionVector != attributeType)
        {
            pAttribute->getRestRowData (tempString);

            restRowData += restHeaderName + "=\"";
            restRowData += tempString;

            restRowData += "\" ";
        }
        else
        {
            pAttribute->getRestRowData (tempRestRowData);

            if (0 != (tempRestRowData.size ()))
            {
                if ("" != restHeaderName)
                {
                    nestedRestRowData += "<" + restHeaderName + ">\r\n";
                }

                nestedRestRowData += tempRestRowData;

                if ("" != restHeaderName)
                {
                    nestedRestRowData += "</" + restHeaderName + ">\r\n";
                }
            }
        }
    }

    UI32 tempSize = restRowData.size ();

    if (0 != tempSize)
    {
        if (' ' == restRowData[tempSize - 1])
        {
            restRowData.erase ((restRowData.size ()) - 1, 1);
        }
    }

    if (0 == (nestedRestRowData.size ()))
    {
        if ("" != managedObjectClassNameForRest)
        {
            restRowData += "/>\r\n";
        }
    }
    else
    {
        if ("" != managedObjectClassNameForRest)
        {
            restRowData += ">\r\n";
        }

        restRowData += nestedRestRowData;

        if ("" != managedObjectClassNameForRest)
        {
            restRowData += "</" + managedObjectClassNameForRest + ">\r\n";
        }
    }
}

void AttributesMap::getAttributeNamesForJson (vector<string> &attributeNamesForJson, const WaveManagedObjectShowType &showType)
{
    getAttributeNamesForHtmlTable (attributeNamesForJson, showType);
}

void AttributesMap::getJsonNames (const vector<string> &attributeNamesForJson, vector<string> &jsonNames)
{
    const UI32 numberOfAttributeNamesForJson = attributeNamesForJson.size ();
          UI32 i                             = 0;

    jsonNames.clear ();

    for (i = 0; i < numberOfAttributeNamesForJson; i++)
    {
        string     attributeNameForJson = attributeNamesForJson[i];
        Attribute *pAttribute           = m_attributesByName[attributeNameForJson];

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        jsonNames.push_back (pAttribute->getAttributeName ());
    }
}

void AttributesMap::getJsonObjectData (const vector<string> &attributeNamesForJson, const vector<string> &jsonFieldNames, const string &managedObjectClassNameForJson, string &jsonObjectData)
{
    const UI32 numberOfAttributeNamesForJson          = attributeNamesForJson.size ();
    const UI32 numberOfJsonHeaderNames                = jsonFieldNames.size ();
          UI32 i                                      = 0;
          bool isManagedObjectClassNameForJsonPresent = false;

    prismAssert (numberOfAttributeNamesForJson == numberOfJsonHeaderNames, __FILE__, __LINE__);

    jsonObjectData += "{\r\n";

    if ("" != managedObjectClassNameForJson)
    {
        isManagedObjectClassNameForJsonPresent = true;

        jsonObjectData += "    \"" + managedObjectClassNameForJson + "\" :\r\n";
        jsonObjectData += "    {\r\n";
    }

    for (i = 0; i < numberOfAttributeNamesForJson; i++)
    {
        string         attributeNameForJson = attributeNamesForJson[i];
        string         jsonFieldName        = jsonFieldNames[i];
        Attribute     *pAttribute           = m_attributesByName[attributeNameForJson];
        string         tempString;
        AttributeType  attributeType;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->toJsonString (tempString);

        jsonObjectData += "    ";

        if (true == isManagedObjectClassNameForJsonPresent)
        {
            jsonObjectData += "    ";
        }

        jsonObjectData += "\"" + jsonFieldName + "\" : " + tempString + "\r\n";
    }

    if (true == isManagedObjectClassNameForJsonPresent)
    {
        jsonObjectData += "    }\r\n";
    }

    jsonObjectData += "}";
}

void AttributesMap::updateKeyString (const PrismPersistableObject *pPrismPersistableObject)
{
    map<UI32, Attribute *>::iterator  element         = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement = m_attributes.end ();
    Attribute                        *pAttribute      = NULL;

    while (element != limitingElement)
    {
        pAttribute = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->updateKeyString (pPrismPersistableObject);

        element++;
    }
}

void AttributesMap::getAllAttributeNames (vector<string> &allAttributeNames) const
{
    map<string, Attribute *>::const_iterator element    = m_attributesByName.begin ();
    map<string, Attribute *>::const_iterator endElement = m_attributesByName.end   ();

    while (endElement != element)
    {
        allAttributeNames.push_back (element->first);

        element++;
    }
}

void AttributesMap::loadFromPostgresQueryResult2 (PGresult *pResult, const UI32 &row, const string &schema, const vector<string> &selectFields, WaveObjectManager *pWaveObjectManager)
{
    UI32       numberOfSelectAttributes = selectFields.size ();
    Attribute *pAttribute               = NULL;

    if (0 == numberOfSelectAttributes)
    {
        map<UI32, Attribute *>::iterator  element             = m_attributes.begin ();
        map<UI32, Attribute *>::iterator  limitingElement     = m_attributes.end ();

        while (element != limitingElement)
        {
            pAttribute   = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            loadAnAttributeFromPostgresQueryResult2 (pAttribute, pResult, row, schema, pWaveObjectManager);

            element++;
        }
    }
    else
    {
        UI32   i            = 0;
        string selectField;

        for (i = 0; i < numberOfSelectAttributes; i++)
        {
            selectField = selectFields[i];

            pAttribute = m_attributesByName[selectField];

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            loadAnAttributeFromPostgresQueryResult2 (pAttribute, pResult, row, schema, pWaveObjectManager);
        }
    }
}

void AttributesMap::loadAnAttributeFromPostgresQueryResult2 (Attribute *pAttribute, PGresult *pResult, const UI32 &row, const string &schema, WaveObjectManager *pWaveObjectManager)
{
    if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
    {
        AttributeManagedObjectOneToOneRelationBase *pAttributeManagedObjectOneToOneRelationBase = dynamic_cast <AttributeManagedObjectOneToOneRelationBase *> (pAttribute);
        pAttributeManagedObjectOneToOneRelationBase->loadRelatedObjectIdFromPostgresQueryResult (pResult, row);
    }
    else if (AttributeType::AttributeTypeObjectIdVector ==  pAttribute->getAttributeType ())
    {
        AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation = dynamic_cast<AttributeObjectIdVectorAssociation *> (pAttribute);

        if (NULL == pAttributeObjectIdVectorAssociation)
        {
            pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
        }
    }
    else if (AttributeType::AttributeTypeCompositionVector != pAttribute->getAttributeType ())
    {
        pAttribute->loadFromPostgresQueryResult (pResult, row, schema, pWaveObjectManager);
    }
}

void AttributesMap::getOidsOfOneToOneCompositions (vector<ObjectId> &vectorOfCompositionOids)
{
    map<UI32, Attribute *>::iterator  element             = m_attributes.begin ();
    map<UI32, Attribute *>::iterator  limitingElement     = m_attributes.end ();

    while (element != limitingElement)
    {
        Attribute *pAttribute   = element->second;

        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
        {
            AttributeManagedObjectOneToOneRelationBase *pAttributeManagedObjectOneToOneRelationBase = dynamic_cast <AttributeManagedObjectOneToOneRelationBase *> (pAttribute);
            ObjectId relatedObjectId = pAttributeManagedObjectOneToOneRelationBase->getRelatedObjectId ();

            if (ObjectId::NullObjectId != relatedObjectId)
            {
                vectorOfCompositionOids.push_back (relatedObjectId);
            }
        }

        element++;
    }
}

void AttributesMap::popOneToOneCompositionsFromResults (map<ObjectId, WaveManagedObject*> &oidTopManagedObjectMap, const vector<string> &selectFieldsInManagedObject)
{
    UI32 numberOfSelectFields = selectFieldsInManagedObject.size ();

    if (0 == numberOfSelectFields)
    {
        map<UI32, Attribute *>::iterator  element             = m_attributes.begin ();
        map<UI32, Attribute *>::iterator  limitingElement     = m_attributes.end ();

        while (element != limitingElement)
        {
            Attribute *pAttribute   = element->second;

            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
            {
                AttributeManagedObjectOneToOneRelationBase *pAttributeManagedObjectOneToOneRelationBase = dynamic_cast <AttributeManagedObjectOneToOneRelationBase *> (pAttribute);

                pAttributeManagedObjectOneToOneRelationBase->popOneToOneCompositionsFromResult (oidTopManagedObjectMap);
            }

            element++;
        }
    }
    else
    {
        for (UI32 i = 0; i < numberOfSelectFields; i++)
        {
            Attribute* pAttribute = getAttribute (selectFieldsInManagedObject[i]);
            prismAssert (NULL != pAttribute, __FILE__, __LINE__);

            if (AttributeType::AttributeTypeComposition == pAttribute->getAttributeType ())
            {
                AttributeManagedObjectOneToOneRelationBase *pAttributeManagedObjectOneToOneRelationBase = dynamic_cast <AttributeManagedObjectOneToOneRelationBase *> (pAttribute);

                pAttributeManagedObjectOneToOneRelationBase->popOneToOneCompositionsFromResult (oidTopManagedObjectMap);
            }
        }
    }
}

void AttributesMap::storeRelatedObjectIdVectorForAOneToNAssociation (const string &relationName, const ObjectId &parentObjectId, const vector<ObjectId> &vectorOfRelatedObjectIds)
{
    Attribute *pAttribute = getAttribute (relationName);
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);
    prismAssert (AttributeType::AttributeTypeObjectIdVector == pAttribute->getAttributeType (), __FILE__, __LINE__);

    AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation = dynamic_cast<AttributeObjectIdVectorAssociation *> (pAttribute);
    prismAssert (NULL != pAttributeObjectIdVectorAssociation, __FILE__, __LINE__);

    pAttributeObjectIdVectorAssociation->storeRelatedObjectIdVector (parentObjectId, vectorOfRelatedObjectIds);
}

void AttributesMap::storeRelatedObjectVectorForAOneToNComposition (const string &relationName, const ObjectId &parentObjectId, const vector<WaveManagedObject *> &vectorOfRelatedObjects)
{
    Attribute *pAttribute = getAttribute (relationName);
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);
    prismAssert (AttributeType::AttributeTypeCompositionVector == pAttribute->getAttributeType (), __FILE__, __LINE__);

    AttributeManagedObjectVectorCompositionTemplateBase *pAttributeManagedObjectVectorCompositionTemplateBase = dynamic_cast<AttributeManagedObjectVectorCompositionTemplateBase *> (pAttribute);
    prismAssert (NULL != pAttributeManagedObjectVectorCompositionTemplateBase, __FILE__, __LINE__);

    pAttributeManagedObjectVectorCompositionTemplateBase->storeRelatedObjectVector (parentObjectId, vectorOfRelatedObjects);
}

}
