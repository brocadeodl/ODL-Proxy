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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangType.h"
#include "Framework/Attributes/Attribute.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Modeling/YANG/ObjectModel/YangEnum.h"

namespace WaveNs
{
set<string>   YangType::m_knownBuiltinTypes;

YangType::YangType ()
    : YangElement     (getYangName (), ""),
      m_pAttribute    (NULL),
      m_nextEnumValue (0)
{
}

YangType::~YangType()
{
}

string YangType::getYangName ()
{
    return ("type");
}

YangElement *YangType::createInstance ()
{
    return (new YangType ());
}

void YangType::initializeBuiltinTypes()
{
    m_knownBuiltinTypes.insert("binary");
    m_knownBuiltinTypes.insert("bits");
    m_knownBuiltinTypes.insert("boolean");
    m_knownBuiltinTypes.insert("decimal64");
    m_knownBuiltinTypes.insert("empty");
    m_knownBuiltinTypes.insert("enumeration");
    m_knownBuiltinTypes.insert("identityref");
    m_knownBuiltinTypes.insert("instance-identifier");
    m_knownBuiltinTypes.insert("int8");
    m_knownBuiltinTypes.insert("int16");
    m_knownBuiltinTypes.insert("int32");
    m_knownBuiltinTypes.insert("int64");
    m_knownBuiltinTypes.insert("leafref");
    m_knownBuiltinTypes.insert("string");
    m_knownBuiltinTypes.insert("uint8");
    m_knownBuiltinTypes.insert("uint16");
    m_knownBuiltinTypes.insert("uint32");
    m_knownBuiltinTypes.insert("uint64");
    m_knownBuiltinTypes.insert("union");
}

bool YangType::isKnownBuiltinType(const string &typeName) const
{
    bool found = false;
    set<string>::iterator it = m_knownBuiltinTypes.begin();

    it=m_knownBuiltinTypes.find(typeName);

    if(it != m_knownBuiltinTypes.end())
    {
        found =  true;
    }

    return found;
}


void YangType::populateEnumAttribute (string name, UI32 value)
{
    if (NULL == m_pAttribute)
    {
        m_pAttribute = Attribute::getAttributeFromAttributeType (WAVE_ATTRIBUTE_TYPE_ENUM);
    }

    AttributeEnum *pAttributeEnum = dynamic_cast<AttributeEnum *> (m_pAttribute);

    pAttributeEnum->populateNameValueEnumMap (name,value);
}

ResourceId YangType::getValueFromEnumName (const string name, UI32 &value)
{
    AttributeEnum *pAttributeEnum = dynamic_cast<AttributeEnum *> (m_pAttribute);
    prismAssert (NULL != pAttributeEnum, __FILE__, __LINE__);

    return (pAttributeEnum->getValueFromEnumName (name, value));
}

ResourceId YangType::getEnumNameFromValue (string &name, const UI32 &value)
{
    AttributeEnum *pAttributeEnum = dynamic_cast<AttributeEnum *> (m_pAttribute);
    prismAssert (NULL != pAttributeEnum, __FILE__, __LINE__);

    return pAttributeEnum->getEnumNameFromValue (name, value);
}

void YangType::populateBitMapAttribute (string name, UI32 bitPosition)
{
    if (NULL == m_pAttribute)
    {
        m_pAttribute = Attribute::getAttributeFromAttributeType (WAVE_ATTRIBUTE_TYPE_BITMAP);
    }

    AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (m_pAttribute);

    pAttributeBitMap->populateNameValueBitMap (name, bitPosition);
}

void YangType::getBitPositionFromBitName (string name, UI32 &bitPosition)
{
    AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (m_pAttribute);
    prismAssert (NULL != pAttributeBitMap, __FILE__, __LINE__);

    pAttributeBitMap->getBitPositionFromBitName (name, bitPosition);
}

void YangType::getBitNameFromBitPosition (string &name, UI32 bitPosition)
{
    AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (m_pAttribute);
    prismAssert (NULL != pAttributeBitMap, __FILE__, __LINE__);

    pAttributeBitMap->getBitNameFromBitPosition (name, bitPosition);
}

void YangType::getStringValueFrom32BitBitMap (string &bitMapString, UI32 bitPattern)
{
    AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (m_pAttribute);
    prismAssert (NULL != pAttributeBitMap, __FILE__, __LINE__);

    bool isSeparatorRequired = false;

    for (UI32 i = 0 ; i < 32 ; i++)
    {
        if( bitPattern & (1 << i) )
        {
            string bitName;

            pAttributeBitMap->getBitNameFromBitPosition (bitName, i);

            if (false == isSeparatorRequired)
            {
                isSeparatorRequired = true;
            }
            else
            {
                bitMapString += ",";
            }

            bitMapString += bitName;
        }
    }

    // bitMapString = string ("[BITMAP_32:") + bitMapString  + string ("]");
}

void YangType::getStringValueFrom64BitBitMap (string &bitMapString, UI64 bitPattern)
{
    AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (m_pAttribute);
    prismAssert (NULL != pAttributeBitMap, __FILE__, __LINE__);

    bool isSeparatorRequired = false;

    for (UI32 i = 0 ; i < 64 ; i++)
    {
        if( bitPattern & (((UI64) 1) << i) )
        {
            string bitName;

            pAttributeBitMap->getBitNameFromBitPosition (bitName, i);

            if (false == isSeparatorRequired)
            {
                isSeparatorRequired = true;
            }
            else
            {
                bitMapString += ",";
            }

            bitMapString += bitName;
        }
    }

    // bitMapString = string ("[BITMAP_64:") + bitMapString  + string ("]");
}

void YangType::getDisplayStringForYangTypeInsideUnion (Attribute *pAttribute, string &displayString)
{
    vector<YangElement *> allTypeChildElements;
    UI32                  numberOfTypeChildElements = 0;
    UI32                  i                         = 0;

    getAllChildrenByYangName (YangType::getYangName (), allTypeChildElements);

    numberOfTypeChildElements = allTypeChildElements.size ();

    for (i = 0; i < (numberOfTypeChildElements) ; i++)
    {
        string usedTypeName;

        YangType  *pYangTypeChild = dynamic_cast<YangType *> (allTypeChildElements[i]);
        prismAssert (NULL != pYangTypeChild, __FILE__, __LINE__);

        usedTypeName = pYangTypeChild->getName ();

        if ("enumeration" == usedTypeName)
        {
            AttributeEnum *pAttributeEnum = dynamic_cast<AttributeEnum *> (pAttribute);

            if (NULL != pAttributeEnum)
            {
                UI32 enumValue = pAttributeEnum->getValue ();

                pYangTypeChild->getEnumNameFromValue(displayString,enumValue);
            }
            else
            {
                AttributeEnumUC *pAttributeEnumUC = dynamic_cast<AttributeEnumUC*> (pAttribute);

                if (NULL != pAttributeEnumUC)
                {
                    EnumUC enumValueUC = pAttributeEnumUC->getValue ();

                    pYangTypeChild->getEnumNameFromValue (displayString, enumValueUC.getUI32Value ());
                }
            }
        }
        else if ("bits" == usedTypeName)
        {
            AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (pAttribute);

            if(pAttributeBitMap)
            {
                string bitMapString;

                bool isOnlyThirtyTwoBit = pAttributeBitMap->getForce32BitRepresentationFlag ();

                BitMap bitMap = pAttributeBitMap->getValue ();

                if (true == isOnlyThirtyTwoBit)
                {
                    UI32 bitmapThirtyTwoBitValue = (UI32) bitMap.getElemValue ();

                    pYangTypeChild->getStringValueFrom32BitBitMap (displayString, bitmapThirtyTwoBitValue );
                }
                else
                {
                    UI64 bitmapSixtyFourBitValue = bitMap.getElemValue ();

                    pYangTypeChild->getStringValueFrom64BitBitMap (displayString, bitmapSixtyFourBitValue );
                }
            }
        }
        else if ("union" == usedTypeName)
        {
            pYangTypeChild->getDisplayStringForYangTypeInsideUnion (pAttribute, displayString);
        }

        if(!displayString.empty ())
        {
            break;
        }
    }
}

void YangType::processChildElement (YangElement *pYangElement)
{
    YangElement::processChildElement (pYangElement);

    const string yangName = pYangElement->getYangName ();

    if ((YangEnum::getYangName()) == yangName)
    {
        YangEnum *pYangEnum = dynamic_cast<YangEnum *> (pYangElement);

        prismAssert (NULL != pYangEnum, __FILE__, __LINE__);

        UI32 value                = pYangEnum->getValue ();
        bool valueIsExplicitlySet = pYangEnum->getValueIsExplicitlySet ();

        if (false == valueIsExplicitlySet)
        {
            pYangEnum->setValue (m_nextEnumValue);
            m_nextEnumValue++;
        }
        else
        {
            m_nextEnumValue = value + 1;
        }
    }
}
}
