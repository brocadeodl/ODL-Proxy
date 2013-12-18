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
 *   Author : Vidyasagara Reddy Guntaka / Mandar Datar                     *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangDataElement.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectRelationalMapping/OrmRepository.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Attributes/AttributeUC.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeUI32VectorUC.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeEnum.h"

#include "Modeling/YANG/ObjectModel/YangType.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "Modeling/YANG/ObjectModel/YangLeafList.h"
#include "Modeling/YANG/ObjectModel/YangContainer.h"
#include "Modeling/YANG/ObjectModel/YangWhen.h"

#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeEnumUC.h"

#include <stdlib.h>
#include <list>
#include <algorithm>

namespace WaveNs
{

YangDataElement::YangDataElement (const string &yangName)
    : YangElement           (yangName, ""),
      m_dataIndex           (0)
{
    setIsConfiguration (true);
}

YangDataElement::YangDataElement (const string &yangName, const string &name)
    : YangElement           (yangName, name),
      m_dataIndex           (0)
{
    setIsConfiguration (true);
}

YangDataElement::~YangDataElement()
{
}

string YangDataElement::getCliTargetNodeNameForData () const
{
    return (getCliTargetNodeName ());
}

void YangDataElement::computeIsConfigurationForSelf ()
{
    if (false == (getIsFirstDataElementInHierarchy ()))
    {
        if (false == (getIsConfigurationExplicitlySet ()))
        {
            YangElement *pParentElement = getPParentElement ();

            if (NULL != pParentElement)
            {
                setIsConfiguration (pParentElement->getIsConfiguration ());
            }
        }
    }
}

void YangDataElement::computeFirstDataElementInHierarchy ()
{
    setIsFirstDataElementInHierarchy (true);
}

UI32 YangDataElement::getDataIndex () const
{
    return (m_dataIndex);
}

void YangDataElement::setDataIndex (const UI32 &dataIndex)
{
    m_dataIndex = dataIndex;
}

void YangDataElement::processAttribute (const string &attributeName, const string &attributeValue)
{
    YangElement::processAttribute (attributeName, attributeValue);
}

void YangDataElement::processChildElement (YangElement *pYangElement)
{
    YangElement::processChildElement (pYangElement);
}

void YangDataElement::unprocessChildElement (YangElement *pYangElement)
{
    YangElement::unprocessChildElement (pYangElement);
}

bool YangDataElement::isYangElementForData () const
{
    return true;
}

ResourceId YangDataElement::getValueString (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, string &valueString, ResourceId &valueType)
{
    UI32        userTagValue    = getUserTagValue (); // Can 0 be a valid hash ?
    Attribute  *pAttribute      = pWaveManagedObject->getAttributeByUserTag (userTagValue);

    bool isAttributeUserDefined = false;

    if (NULL == pAttribute)
    {
        pAttribute = pWaveManagedObject->getAttributeByUserTagUserDefined (userTagValue);

        if (NULL == pAttribute)
        {
            trace (TRACE_LEVEL_FATAL, string ("YangDataElement::getValueString : [") + getYangName () + string (":") + getName () + string (" : ") + getCliTargetNodeName () + string ("] : Could not get attribute for user tag [") + userTagValue + string ("] in Managed Object [") + pWaveManagedObject->getObjectClassName () + string (" : ") + pWaveManagedObject->getKeyString () + string ("]"));

            prismAssert (false, __FILE__, __LINE__);
        }

        // Verify that 'getAttributeByUserTagUserDefined' is being used only for AttributeBool !

        AttributeBool *pAttributeBool = dynamic_cast<AttributeBool *> (pAttribute);
        prismAssert (NULL != pAttributeBool, __FILE__, __LINE__);

        isAttributeUserDefined = true;
    }

    const bool checkForAttributeDefaultValue = (false == pYangDisplayConfigurationContext->getPrintDefaultValuesFlag ()) ? true : false;

    ResourceId returnStatus = getValueStringForAttribute (pYangDisplayConfigurationContext, pWaveManagedObject, pAttribute, valueString, valueType, checkForAttributeDefaultValue);

    if (true == isAttributeUserDefined)
    {
        delete pAttribute;
    }

    return returnStatus;
}

ResourceId YangDataElement::formatValueString (string &valueString, const ResourceId &valueType) const
{
    switch (valueType)
    {
        case WAVE_ATTRIBUTE_TYPE_STRING:
        case WAVE_ATTRIBUTE_TYPE_STRING_UC:
            {
                StringUtils::applyConfigStringFormatting (valueString, getIsCliMultiValue ());
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_BITMAP:
            {
                if (true == valueString.empty ())
                {
                    valueString = "\"\"";
                }
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS:
            {
                string valueStringCopy;
                StringUtils::getLowerCaseString (valueString, valueStringCopy);
                valueString = valueStringCopy;
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_SI32_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC:
        case WAVE_ATTRIBUTE_TYPE_STRING_VECTOR:
            {
                if (true == valueString.empty ())
                {
                    return WAVE_MESSAGE_ERROR; // WAVE_EMPTY_VECTOR
                }
            }
            break;
    }

    return WAVE_MESSAGE_SUCCESS;
}

ResourceId YangDataElement::getValueStringForAttribute (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, Attribute *pAttribute, string &valueString, ResourceId &valueType, const bool &checkForAttributeDefaultValue)
{
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    ResourceId  returnStatus    = WAVE_MESSAGE_SUCCESS;

    // If Association, query associated Managed Object to get its key value (only one key - ?).

    if (AttributeType::AttributeTypeObjectId == pAttribute->getAttributeType ())
    {
        AttributeObjectId *pAttributeObjectId = dynamic_cast<AttributeObjectId*> (pAttribute); // AttributeObjectIdAssociation ?
        prismAssert (NULL != pAttributeObjectId, __FILE__, __LINE__);

        ObjectId objectId = pAttributeObjectId->getValue ();

        getKeyStringForAssociatedManagedObject (pYangDisplayConfigurationContext, objectId, valueString);

        if (true == valueString.empty ())
        {
            returnStatus = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        Attribute *pFinalAttribute =NULL;
        ResourceId attributeValueStatus = getAttributeValueStringForDisplayConfiguration (pYangDisplayConfigurationContext, pAttribute, valueString, pWaveManagedObject, valueType , pFinalAttribute, checkForAttributeDefaultValue);

        if (WAVE_MESSAGE_SUCCESS == attributeValueStatus)
        {
            // Extra processing. e.g. for bitmap,  etc.
            vector<YangElement *> allTypeChildElements;
            UI32                  typeChildElementsVectorSize = 0;

            getAllChildrenByYangName (YangType::getYangName (), allTypeChildElements);

            typeChildElementsVectorSize = allTypeChildElements.size();
            prismAssert ( 2 > typeChildElementsVectorSize , __FILE__, __LINE__);

            if(typeChildElementsVectorSize > 0)
            {
                YangType  *pYangType  = dynamic_cast<YangType *> (allTypeChildElements[0]);

                if (WAVE_ATTRIBUTE_TYPE_BITMAP == valueType)
                {
                    valueString.clear ();

                    AttributeUnion *pAttributeUnion = dynamic_cast<AttributeUnion *> (pAttribute);

                    if (NULL != pAttributeUnion)
                    {
                        //If the attribute bitmap was contained in an union
                        pYangType->getDisplayStringForYangTypeInsideUnion ( pFinalAttribute, valueString);
                    }
                    else
                    {
                        AttributeBitMap *pAttributeBitMap = dynamic_cast<AttributeBitMap *> (pFinalAttribute);
                        prismAssert (NULL != pAttributeBitMap, __FILE__, __LINE__);

                        bool isOnlyThirtyTwoBit = pAttributeBitMap->getForce32BitRepresentationFlag ();

                        BitMap bitMap = pAttributeBitMap->getValue ();

                        if (true == isOnlyThirtyTwoBit)
                        {
                            UI32 bitmapThirtyTwoBitValue = (UI32) bitMap.getElemValue ();
                            pYangType->getStringValueFrom32BitBitMap (valueString, bitmapThirtyTwoBitValue );
                        }
                        else
                        {
                            UI64 bitmapSixtyFourBitValue = bitMap.getElemValue ();
                            pYangType->getStringValueFrom64BitBitMap (valueString, bitmapSixtyFourBitValue );
                        }
                    }
                }
                else if (WAVE_ATTRIBUTE_TYPE_ENUM == valueType)
                {
                    valueString.clear ();

                    AttributeUnion *pAttributeUnion = dynamic_cast<AttributeUnion *> (pAttribute);

                    if (NULL != pAttributeUnion)
                    {
                        //IF the attribute enum was contained in an union
                        pYangType->getDisplayStringForYangTypeInsideUnion (pFinalAttribute, valueString);
                    }
                    else
                    {
                        AttributeEnum *pAttributeEnum = dynamic_cast<AttributeEnum *> (pFinalAttribute);
                        prismAssert (NULL != pAttributeEnum, __FILE__, __LINE__);

                        UI32 enumValue = pAttributeEnum->getValue ();

                        ResourceId enumToValueStringStatus = pYangType->getEnumNameFromValue (valueString, enumValue);

                        if (WAVE_MESSAGE_SUCCESS != enumToValueStringStatus)
                        {
                            valueString = string ("enum=") + enumValue;
                        }
                    }
                }
                else if (WAVE_ATTRIBUTE_TYPE_ENUM_UC == valueType)
                {
                    valueString.clear ();

                    AttributeUnion *pAttributeUnion = dynamic_cast<AttributeUnion *> (pAttribute);

                    if (NULL != pAttributeUnion)
                    {
                        //IF the attribute enum was contained in an union
                        pYangType->getDisplayStringForYangTypeInsideUnion (pFinalAttribute, valueString);
                    }
                    else
                    {
                        AttributeEnumUC *pAttributeEnumUC = dynamic_cast<AttributeEnumUC *> (pFinalAttribute);
                        prismAssert (NULL != pAttributeEnumUC, __FILE__, __LINE__);

                        UI32 enumValue = (pAttributeEnumUC->getValue ()).getUI32Value ();

                        ResourceId enumToValueStringStatus = pYangType->getEnumNameFromValue (valueString, enumValue);

                        if (WAVE_MESSAGE_SUCCESS != enumToValueStringStatus)
                        {
                            valueString = string ("enum=") + enumValue;
                        }
                    }
                }
            }
        }
        returnStatus = attributeValueStatus;
    }

    return returnStatus;
}

void YangDataElement::getKeyStringForAssociatedManagedObject (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const ObjectId &objectId, string &valueString)
{
    if (ObjectId::NullObjectId == objectId)
    {
        valueString = "";

        return;
    }

    WaveManagedObject *pWaveManagedObjectForObjectId = WaveObjectManagerToolKit::queryManagedObject (objectId, pYangDisplayConfigurationContext->getConfigurationSchema ());

    if (NULL == pWaveManagedObjectForObjectId)
    {
        valueString = "";

        return;
    }

    vector<string> userDefinedKeyCombination;

    string managedObjectClassName = pWaveManagedObjectForObjectId->getObjectClassName ();

    OrmRepository::getUserDefinedKeyCombinationForTable (managedObjectClassName, userDefinedKeyCombination);

    UI32 numberOfKeysInUserDefinedKeyCombination = userDefinedKeyCombination.size ();

    if (1 == numberOfKeysInUserDefinedKeyCombination)
    {
        Attribute *pAttribute = pWaveManagedObjectForObjectId->getAttributeByName (userDefinedKeyCombination[0]);
        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        pAttribute->getPlainString (valueString); // XXX Is recursion required ?
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("YangDataElement::getKeyStringForAssociatedManagedObject : [") + managedObjectClassName + string ("] has [") + numberOfKeysInUserDefinedKeyCombination + string ("] keys."));
    }

    delete pWaveManagedObjectForObjectId;
}

ResourceId YangDataElement::getAttributeValueStringForDisplayConfiguration (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, Attribute *pAttribute, string &displayString, WaveManagedObject *pWaveManagedObject, ResourceId &valueType, Attribute *&pFinalAttribute, const bool &checkForAttributeDefaultValue)
{
    ResourceId attributeValueStatus = WAVE_MESSAGE_SUCCESS;

    AttributeType   attributeTypeLocal              = pAttribute->getAttributeType ();
    ResourceId      attributeTypeResourceIdLocal    = attributeTypeLocal.getAttributeTypeResourceId ();

    switch (attributeTypeResourceIdLocal)
    {
        case WAVE_ATTRIBUTE_TYPE_UI8:
        case WAVE_ATTRIBUTE_TYPE_UI16:
        case WAVE_ATTRIBUTE_TYPE_UI32:
        case WAVE_ATTRIBUTE_TYPE_UI64:
        case WAVE_ATTRIBUTE_TYPE_SI8:
        case WAVE_ATTRIBUTE_TYPE_SI16:
        case WAVE_ATTRIBUTE_TYPE_SI32:
        case WAVE_ATTRIBUTE_TYPE_SI64:
        case WAVE_ATTRIBUTE_TYPE_BOOL:
        case WAVE_ATTRIBUTE_TYPE_STRING:
        case WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME:
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS:
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS:
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS:
        case WAVE_ATTRIBUTE_TYPE_ENUM:
        case WAVE_ATTRIBUTE_TYPE_DATE:
        case WAVE_ATTRIBUTE_TYPE_TIME:
        case WAVE_ATTRIBUTE_TYPE_DATETIME:
        case WAVE_ATTRIBUTE_TYPE_BITMAP:
        case WAVE_ATTRIBUTE_TYPE_DECIMAL64:
        case WAVE_ATTRIBUTE_TYPE_IPVXADDRESS:
        case WAVE_ATTRIBUTE_TYPE_MACADDRESS2:
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK:
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK:
        case WAVE_ATTRIBUTE_TYPE_UI32_RANGE:
        case WAVE_ATTRIBUTE_TYPE_MAC_RANGE:
        case WAVE_ATTRIBUTE_TYPE_SNMPOBJECTID:
        case WAVE_ATTRIBUTE_TYPE_UUID:
        case WAVE_ATTRIBUTE_TYPE_SI32_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_STRING_VECTOR:
        case WAVE_ATTRIBUTE_TYPE_URI:
            {
                if ((false == checkForAttributeDefaultValue) || (false == pAttribute->isCurrentValueSameAsDefault ()))
                {
                    pAttribute->getPlainString (displayString);
                }
                else
                {
                    attributeValueStatus = WAVE_MESSAGE_ERROR; // WAVE_ATTRIBUTE_DEFAULT_VALUE;
                }
                valueType = attributeTypeResourceIdLocal;
                pFinalAttribute = pAttribute;
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_UC:
        case WAVE_ATTRIBUTE_TYPE_BOOL_UC:
        case WAVE_ATTRIBUTE_TYPE_ENUM_UC:
        case WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC:
        case WAVE_ATTRIBUTE_TYPE_HOST_UC:
        case WAVE_ATTRIBUTE_TYPE_UI64_UC:
        case WAVE_ATTRIBUTE_TYPE_STRING_UC:
            {
                AttributeUC *pAttributeUC = dynamic_cast<AttributeUC *> (pAttribute);
                prismAssert (NULL != pAttributeUC, __FILE__, __LINE__);

                if (true == pAttributeUC->getIsUserConfigured ())
                {
                    if ((false == checkForAttributeDefaultValue) || (false == pAttribute->isCurrentValueSameAsDefault ()))
                    {
                        pAttribute->getPlainString (displayString);
                        pFinalAttribute = pAttribute;
                    }
                    else
                    {
                        attributeValueStatus = WAVE_MESSAGE_ERROR; // WAVE_ATTRIBUTE_DEFAULT_VALUE;
                    }
                }
                else
                {
                    attributeValueStatus = WAVE_MESSAGE_ERROR; // WAVE_ATTRIBUTE_UC_NOT_CONFIGURED;
                }
                valueType = attributeTypeResourceIdLocal;
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UNION:
            {
                AttributeUnion *pAttributeUnion =  dynamic_cast<AttributeUnion *> (pAttribute);
                prismAssert (NULL != pAttributeUnion, __FILE__, __LINE__);
                WaveUnion targetAttributeName = pAttributeUnion->getValue ();
                Attribute *pTargetAttribute = pWaveManagedObject->getAttributeByName (targetAttributeName);
                // Recursive function call.
                attributeValueStatus = getAttributeValueStringForDisplayConfiguration (pYangDisplayConfigurationContext, pTargetAttribute, displayString, pWaveManagedObject, valueType, pFinalAttribute, checkForAttributeDefaultValue);
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_VECTOR:
            {
                AttributeUI32Vector *pAttributeUI32Vector = dynamic_cast<AttributeUI32Vector *> (pAttribute);
                prismAssert (NULL != pAttributeUI32Vector, __FILE__, __LINE__);

                vector<UI32> ui32VectorValue = pAttributeUI32Vector->getValue ();

                for (UI32 i = 0; i < ui32VectorValue.size (); i++)
                {
                    if (0 != i)
                    {
                        displayString += " ";
                    }

                    displayString = displayString + ui32VectorValue[i];
                }
                valueType = attributeTypeResourceIdLocal;
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC:
            {
                AttributeUI32VectorUC *pAttributeUI32VectorUC = dynamic_cast<AttributeUI32VectorUC *> (pAttribute);
                prismAssert (NULL != pAttributeUI32VectorUC, __FILE__, __LINE__);

                vector<UI32> ui32VectorValue = (pAttributeUI32VectorUC->getValue ()).getUI32Vector ();

                for (UI32 i = 0; i < ui32VectorValue.size (); i++)
                {
                    if (0 != i)
                    {
                        displayString += " ";
                    }

                    displayString = displayString + ui32VectorValue[i];
                }
                valueType = attributeTypeResourceIdLocal;
            }
            break;
        case WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR:
            {
                AttributeObjectIdVectorAssociation *pAttributeObjectIdVectorAssociation = dynamic_cast<AttributeObjectIdVectorAssociation *> (pAttribute);
                prismAssert (NULL != pAttributeObjectIdVectorAssociation, __FILE__, __LINE__);

                vector<ObjectId>    objectIdVector              = pAttributeObjectIdVectorAssociation->getValue ();
                UI32                numberOfObjectIds           = objectIdVector.size ();
                vector<string>      stringValueVector;

                for (UI32 i = 0; i < numberOfObjectIds; i++)
                {
                    ObjectId anObjectId = objectIdVector[i];

                    if (ObjectId::NullObjectId != anObjectId)
                    {
                        string keyValueForManagedObject;
                        getKeyStringForAssociatedManagedObject (pYangDisplayConfigurationContext, anObjectId, keyValueForManagedObject);
                        stringValueVector.push_back (keyValueForManagedObject);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_ERROR, string ("YangDataElement::getAttributeValueStringForDisplayConfiguration : NullObjectId in [") + pAttributeObjectIdVectorAssociation->getAttributeName () + string ("]"));
                    }
                }

                UI32 numberOfStringValues = stringValueVector.size ();

                if (false == pAttributeObjectIdVectorAssociation->getOutputAsString ())
                {
                    string  vectorValueString;
                    bool    isSeparatorRequired = false;

                    for (UI32 i = 0; i < numberOfStringValues; i++)
                    {
                        if (false == isSeparatorRequired)
                        {
                            isSeparatorRequired = true;
                        }
                        else
                        {
                            vectorValueString += " ";
                        }

                        vectorValueString += stringValueVector[i];
                    }
                    displayString   = vectorValueString;
                    valueType       = attributeTypeResourceIdLocal;
                }
                else
                {
                    // Special case. Only UI32 is supported. (Same as existing behaviour.)

                    if (0 == numberOfStringValues)
                    {
                        attributeValueStatus = WAVE_MESSAGE_ERROR;
                    }
                    else
                    {
                        set<UI32> setOfUI32Values;

                        for (UI32 i = 0; i < numberOfStringValues; i++)
                        {
                            setOfUI32Values.insert ((UI32) strtoll ((stringValueVector[i]).c_str (), NULL, 10));
                        }

                        vector<UI32> rangeIntegers (setOfUI32Values.begin (), setOfUI32Values.end ());

                        displayString   = UI32Range::getUI32RangeStringFromVector (rangeIntegers);
                    }
                    valueType       = WAVE_ATTRIBUTE_TYPE_STRING;
                }
            }
            break;
        default:
            trace (TRACE_LEVEL_ERROR, string ("YangDataElement::getAttributeValueStringForDisplayConfiguration : unknown type [") + FrameworkToolKit::localizeToSourceCodeEnum (attributeTypeResourceIdLocal) + string ("]"));
            attributeValueStatus = WAVE_MESSAGE_ERROR;
    }

    return attributeValueStatus;
}

void YangDataElement::displayConfigurtionForSingleElement (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, const bool &displayOnMatch, const string &stringToMatch, const bool &checkForYangDefault)
{
    bool    nothingToPrint      = false;
    bool    printNoConfig       = false;
    string  configString;
    bool    incompleteCommand   = false;
    string  configValue;

    getConfigurtionForSingleElement (pYangDisplayConfigurationContext, pWaveManagedObject, nothingToPrint, printNoConfig, configString, incompleteCommand, configValue, checkForYangDefault);

    if (true == nothingToPrint)
    {
        return;
    }

    if (true == printNoConfig)
    {
        pYangDisplayConfigurationContext->printNoConfigString (configString);
    }
    else
    {
        if (true == displayOnMatch)
        {
            if (stringToMatch != configValue)
            {
                return;
            }
        }

        pYangDisplayConfigurationContext->printConfigString (configString, incompleteCommand);
    }
}

// virtual
void YangDataElement::applyFormattingToConfigString (string &configString) const
{
}

void YangDataElement::getConfigurtionForSingleElement (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, bool &nothingToPrint, bool &printNoConfig, string &configString, bool &incompleteCommand, string &configValue, const bool &checkForYangDefault)
{
    nothingToPrint = true;

    if (NULL == getPConfigurationSegmentInformation ())
    {
        trace (TRACE_LEVEL_ERROR, string ("YangDataElement::displayConfigurtionForSingleElement : does not know about [") + getConfigurationSegmentName () + string ("]"));
        return;
    }

    prismAssert (((YangLeaf::getYangName () == getYangName ()) || (YangContainer::getYangName () == getYangName ()) || (YangLeafList::getYangName () == getYangName ())), __FILE__, __LINE__);

    WaveManagedObject *pWaveManagedObjectProper = NULL;

    if (NULL == pWaveManagedObject)
    {
        bool disableFilters = false;
        pYangDisplayConfigurationContext->getPWaveManagedObject (this, pWaveManagedObjectProper, disableFilters);
    }
    else
    {
        pWaveManagedObjectProper = pWaveManagedObject;
    }

    if (NULL == pWaveManagedObjectProper)
    {
        if (true == getIsCliShowNo ())
        {
            // pYangDisplayConfigurationContext->printNoConfigString (getDisplayName ());

            nothingToPrint = false;
            printNoConfig = true;
            configString = getDisplayName ();
            configValue  = "false";
        }
        else
        {
            // pass.
        }

        return;
    }

    string      valueString;
    ResourceId  valueType;

    if (WAVE_MESSAGE_SUCCESS == getValueString (pYangDisplayConfigurationContext, pWaveManagedObjectProper, valueString, valueType))
    {
        if ((true == checkForYangDefault) && (false == pYangDisplayConfigurationContext->getPrintDefaultValuesFlag ()))
        {
            if (true == isValueSameAsDefault (valueString, valueType))
            {
                return;
            }
        }

        if (WAVE_MESSAGE_SUCCESS != formatValueString (valueString, valueType))
        {
            return;
        }

        if ((WAVE_ATTRIBUTE_TYPE_BOOL == valueType) || (WAVE_ATTRIBUTE_TYPE_BOOL_UC == valueType))
        {
            configValue     = valueString;

            if (YangLeaf::getYangName () == getYangName ())
            {
                YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (this);
                prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

                if (false == pYangLeaf->getIsEmpty ())
                {
                    nothingToPrint = false;
                    printNoConfig = false;
                    configString = getDisplayName () + string (" ") + valueString;
                    return;
                }
            }

            if (false == getDropNodeNameForCli ())
            {
                if ("false" == valueString)
                {
                    if (true == getIsCliShowNo ())
                    {
                        // pYangDisplayConfigurationContext->printNoConfigString (getDisplayName ());

                        nothingToPrint = false;
                        printNoConfig = true;
                        configString = getDisplayName ();
                    }
                    else
                    {
                        // pass.
                    }
                }
                else // if ("true" == valueString)
                {
                    // pYangDisplayConfigurationContext->printConfigString (getDisplayName (), getCliIncompleteCommand ());

                    nothingToPrint = false;
                    printNoConfig = false;
                    configString = getDisplayName ();
                    incompleteCommand = getCliIncompleteCommand ();
                }
            }
            else
            {
                // empty leafs with drop-node name !

                if ("false" == valueString)
                {
                    if (true == getIsCliShowNo ())
                    {
                        // pYangDisplayConfigurationContext->printNoConfigString (" ");
                        // No for for empty leafs with drop-node-name (!)
                        nothingToPrint = false;
                        printNoConfig = true;
                        configString = "";
                    }
                    else
                    {
                        // pass.
                    }
                }
                else // if ("true" == valueString)
                {
                    // pYangDisplayConfigurationContext->printConfigString (" ");

                    nothingToPrint = false;
                    printNoConfig = false;
                    ////// configString = " ";
                    configString = "";
                }
            }
        }
        else
        {
            applyFormattingToConfigString (valueString);

            configValue       = valueString;

            if (false == getDropNodeNameForCli ())
            {
                // pYangDisplayConfigurationContext->printConfigString (getDisplayName () + " " + valueString, getCliIncompleteCommand ());

                nothingToPrint = false;
                printNoConfig = false;
                configString = getDisplayName () + " " + valueString;
                incompleteCommand = getCliIncompleteCommand ();
            }
            else
            {
                // pYangDisplayConfigurationContext->printConfigString (valueString, getCliIncompleteCommand ());

                nothingToPrint = false;
                printNoConfig = false;
                configString = valueString;
                incompleteCommand = getCliIncompleteCommand ();
            }
        }
    }
    else
    {
        if (true == getIsCliShowNo ())
        {
            // pYangDisplayConfigurationContext->printNoConfigString (getDisplayName ());

            nothingToPrint = false;
            printNoConfig = true;
            configString = getDisplayName ();
        }
        else
        {
            // pass.
        }
    }
}

bool YangDataElement::isValueSameAsDefault (const string &valueString, ResourceId &valueType)
{
    return false;
}

bool YangDataElement::evaluateWhenConditions (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    bool returnValue = true;

    vector<YangElement *> pWhenYangElements;

    getAllChildrenByYangName (YangWhen::getYangName (), pWhenYangElements);

    UI32 numberOfWhenYangElements = pWhenYangElements.size ();

    for (UI32 i = 0; i < numberOfWhenYangElements; i++)
    {
        YangWhen *pYangWhen = dynamic_cast<YangWhen *> (pWhenYangElements[i]);
        prismAssert (NULL != pYangWhen, __FILE__, __LINE__);

        bool resultValue = pYangWhen->evaluateCondition (pYangDisplayConfigurationContext);

        if (false == resultValue)
        {
            returnValue = false;

            break;
        }
    }

    return returnValue;
}

bool YangDataElement::considerForHFileForCLanguage () const
{
    return (true);
}

}
