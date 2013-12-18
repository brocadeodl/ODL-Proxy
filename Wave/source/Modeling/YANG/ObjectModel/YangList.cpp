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

#include "Modeling/YANG/ObjectModel/YangList.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "Modeling/YANG/ObjectModel/YangLeafList.h"
#include "Modeling/YANG/ObjectModel/YangKey.h"
#include "Modeling/YANG/ObjectModel/YangType.h"
#include "Modeling/YANG/ObjectModel/YangContainer.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveLocalManagedObjectBase.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"

#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

YangList::YangList ()
    : YangDataElement (getYangName (), "")
{
    m_hasChildLeafWithCustomDisplay = false;
}

YangList::~YangList()
{
}

string YangList::getYangName ()
{
    return ("list");
}

YangElement *YangList::createInstance ()
{
    return (new YangList ());
}

vector<string> YangList::getKeyCombination () const
{
    return (m_keyCombination);
}

void YangList::setKeyCombination (const vector<string> &keyCombination)
{
    m_keyCombination.clear ();
    m_keyCombinationSet.clear ();

    UI32 numberOfKeyCombinationEntries = keyCombination.size ();

    for (UI32 i = 0; i < numberOfKeyCombinationEntries; i++)
    {
        string keyEntry = keyCombination[i];
        StringUtils::removeAllInstancesOfInputChar (keyEntry, '\n');
        m_keyCombination.push_back (keyEntry);
        m_keyCombinationSet.insert (keyEntry);
    }
}

bool YangList::isLeafNamePartOfKeyCombination (const string &leafName) const
{
    set<string>::const_iterator element    = m_keyCombinationSet.find (leafName);
    set<string>::const_iterator endElement = m_keyCombinationSet.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void YangList::processChildElement (YangElement *pYangElement)
{
    YangDataElement::processChildElement (pYangElement);

    const string yangName = pYangElement->getYangName ();

    if ((YangKey::getYangName()) == yangName)
    {
        YangKey *pYangKey = dynamic_cast<YangKey *> (pYangElement);

        prismAssert (NULL != pYangKey, __FILE__, __LINE__);

        string keyValue;

        pYangKey->getAttributeValue ("value", keyValue);

        prismAssert ("" != keyValue, __FILE__, __LINE__);

        vector<string> keyCombination;

        StringUtils::tokenize (keyValue, keyCombination, ' ');

        setKeyCombination (keyCombination);
    }
}

void YangList::prepareTypeInformationForSelf ()
{
    UI32 numberOfEntriesInKeyCombination = m_keyCombination.size ();

    for (UI32 i = 0; i < numberOfEntriesInKeyCombination; i++)
    {
        string keyEntry = m_keyCombination[i];

        YangElement *pYangElement = getUniqueYangElementByName (keyEntry);
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        vector<YangElement *> allYangTypeChildElements;
        pYangElement->getAllChildrenByYangName (YangType::getYangName (), allYangTypeChildElements);

        UI32 numberOfAllYangTypeChildElements = allYangTypeChildElements.size ();
        prismAssert (1 == numberOfAllYangTypeChildElements, __FILE__, __LINE__);

        YangType *pYangType = dynamic_cast<YangType *> (allYangTypeChildElements[0]);
        prismAssert (NULL != pYangType, __FILE__, __LINE__);

        string typeName = pYangType->getName ();
        m_keyCombinationTypes.push_back (typeName);
    }
}

void YangList::addKeyDefaultValue (const string &keyEntry, const string &defaultValueString)
{
    // Assertion checks BEGIN   [

    map<string, string>::const_iterator itr = m_keyNameToDefaultValueMap.find (keyEntry);
    map<string, string>::const_iterator end = m_keyNameToDefaultValueMap.end ();

    prismAssert (end == itr, __FILE__, __LINE__);

    // Assertion checks END     ]

    m_keyNameToDefaultValueMap[keyEntry] = defaultValueString;
}

void YangList::getDefaultValueStringForKeyIfPresent (const string &keyEntry, bool &isDefaultValuePresent, string &defaultValueString) const
{
    map<string, string>::const_iterator itr = m_keyNameToDefaultValueMap.find (keyEntry);
    map<string, string>::const_iterator end = m_keyNameToDefaultValueMap.end ();

    if (end != itr)
    {
        isDefaultValuePresent   = true;

        defaultValueString      = itr->second;
    }
}

void YangList::collectInformationAboutChildLeafWithCustomDisplayForSelf ()
{
    vector<YangElement *> pChildLeafYangElements;
    getAllChildrenByYangName (YangLeaf::getYangName (), pChildLeafYangElements);

    UI32 numberOfPChildLeafYangElements = pChildLeafYangElements.size ();

    for (UI32 i = 0; i < numberOfPChildLeafYangElements; i++)
    {
        YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pChildLeafYangElements[i]);
        prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

        if (true == pYangLeaf->getIsCliRunTemplate ())
        {
            m_hasChildLeafWithCustomDisplay = true;

            break;
        }
    }
}

void YangList::displayConfiguration (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    FILE *pFile = pYangDisplayConfigurationContext->getPFile ();
    if (NULL == pFile)
    {
        pFile= stdout;
    }

    vector<WaveManagedObject *> *pResults = pYangDisplayConfigurationContext->getPManagedObjectsToDisplay ();
    prismAssert (NULL != pResults, __FILE__, __LINE__);

    UI32 numberOfResults = pResults->size ();
    vector<YangElement *> leafElements;
    getAllChildrenByYangName (YangLeaf::getYangName (), leafElements);
    UI32 numberOfLeafElements = leafElements.size ();

    string cliTargetNodeName         = getCliTargetNodeName ();
    bool   isCliSuppressMode         = getIsCliSuppressMode (); // yang anotation flag
    bool   isCliShowNo               = getIsCliShowNo();        // yang anotation flag      
    string compactSyntax             = getCliCompactSyntax ();  // Yang annotation at list level


    for (UI32 i = 0; i < numberOfResults; i++)
    {   
        WaveManagedObject *pWaveManagedObject = (*pResults)[i];
        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        string printStringForKey         = cliTargetNodeName;
        string printStringForNonKeyLeafs = "";
        string suppressModePrefix        = "";

        for (UI32 j = 0; j < numberOfLeafElements; j++)
        {   
            YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (leafElements[j]);
            prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);
            
            if ((true == (pYangLeaf->getIsConfiguration ())) && (true == (isLeafNamePartOfKeyCombination (pYangLeaf->getName ()))))
            {   
                // if block is for all key leaf nodes 
                string exposeKeyString;
                UI32 userTag = pYangLeaf->getUserTagValue ();
                // this yang annotation is only applicable to key leaf
                exposeKeyString = pYangLeaf->getExposeKeyName ();

                Attribute *pAttribute = pWaveManagedObject->getAttributeByUserTag (userTag);
                prismAssert (NULL != pAttribute, __FILE__, __LINE__);

                string fieldValue;
                pAttribute->getPlainString (fieldValue);
                
                if (isCliSuppressMode && (compactSyntax != ""))
                {
                    suppressModePrefix = cliTargetNodeName +" " + exposeKeyString +" " +fieldValue + " ";
                }
                else
                {
                    printStringForKey = printStringForKey + " " + exposeKeyString + " " + fieldValue + " ";
                }
            } 
            else if ((true == (pYangLeaf->getIsConfiguration ())) && (false == (isLeafNamePartOfKeyCombination (pYangLeaf->getName ()))))
            {  
                // this else block is for all non key leafs 
                string  displayName      = pYangLeaf->getDisplayName  ();
                UI32    userTag          = pYangLeaf->getUserTagValue ();
                bool    isCliShowNoleaf  = pYangLeaf->getIsCliShowNo  ();  // yang annotation at leaf level
                string  showNo           = "";
                string  fieldValue;

                Attribute *pAttribute = pWaveManagedObject->getAttributeByUserTag (userTag);
                prismAssert (NULL != pAttribute, __FILE__, __LINE__);

                pAttribute->getPlainString (fieldValue);

                if ("" != fieldValue)
                {   
                    // extra processing required for leaf type empty 
                    if (true == pYangLeaf->getIsEmpty ())
                    {  
                        // If show no annotaiton is present either at leaf or list level  
                        if ( (isCliShowNoleaf || isCliShowNo) && ("true" != fieldValue) )
                        {
                            showNo = "no ";
                        }
                       
                        fieldValue ="";
                    }
                    // order of print should not be changed as it is very important        
                    printStringForNonKeyLeafs = printStringForNonKeyLeafs + compactSyntax + suppressModePrefix + showNo + displayName + " " + fieldValue + " ";
                }
            }
        }

        fprintf (pFile, "%s %s \r\n!\r\n",printStringForKey.c_str(), printStringForNonKeyLeafs.c_str());
    }
}

void YangList::displayConfiguration2 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
{
    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        return;
    }

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = getPConfigurationSegmentInformation ();

    if (NULL == pConfigurationSegmentInformation)
    {
        trace (TRACE_LEVEL_ERROR, string ("YangElement::displayConfiguration3 : [") + getName () + string ("], configurationSegment [") + getConfigurationSegmentName () + string ("] : does not know about managed object."));
        return;
    }


    UI32 pageNumber = 0;

    pYangDisplayConfigurationContext->incrementCommandKeyIndex ();

    vector<WaveManagedObject *> pWaveManagedObjects;

    pYangDisplayConfigurationContext->getPWaveManagedObjects (this, pWaveManagedObjects, pageNumber);

    UI32 numberOfManagedObjects = pWaveManagedObjects.size ();

    if (0 == numberOfManagedObjects)
    {
        pYangDisplayConfigurationContext->decrementCommandKeyIndex ();

        return;
    }

    pYangDisplayConfigurationContext->addConfigurationLevel (pWaveManagedObjects, this);

    WaveYangMemberElementFilterInformation *pWaveYangMemberElementFilterInformation = collectMemberElementFilterInformation (pYangDisplayConfigurationContext);

    vector<YangElement *> selectedChildElementsWithFilter;

    if (NULL == pWaveYangMemberElementFilterInformation)
    {
        selectedChildElementsWithFilter = selectedChildElementsForData;
    }
    else
    {
        selectedChildElementsWithFilter = getPChildElementsForData ();
    }

  do
  {
    for (UI32 i = 0; i < numberOfManagedObjects; i++)
    {
        if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
        {
            break;
        }

        /* */ pYangDisplayConfigurationContext->addSlotForPartialOutput ();

        WaveManagedObject *pWaveManagedObject = pWaveManagedObjects[i];
        prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

        if (true == getIsNodeSpecificBaseList ())
        {
            WaveLocalManagedObjectBase *pWaveLocalManagedObjectBase = dynamic_cast<WaveLocalManagedObjectBase *> (pWaveManagedObject);
            prismAssert (NULL != pWaveLocalManagedObjectBase, __FILE__, __LINE__);

            ObjectId ownerWaveNodeObjectId = pWaveLocalManagedObjectBase->getOwnerWaveNodeObjectId ();
            prismAssert (ObjectId::NullObjectId != ownerWaveNodeObjectId, __FILE__, __LINE__);

            pYangDisplayConfigurationContext->setOwnerWaveNodeObjectId (ownerWaveNodeObjectId);
        }

        if (true == getIsMultiPartitionBaseList ())
        {
            ObjectId ownerPartitionManagedObjectId = pWaveManagedObject->getOwnerPartitionManagedObjectId ();
            prismAssert (ObjectId::NullObjectId != ownerPartitionManagedObjectId, __FILE__, __LINE__);

            pYangDisplayConfigurationContext->setOwnerPartitionObjectId (ownerPartitionManagedObjectId);
        }

        ResourceId filterMatchStatus = WAVE_MESSAGE_SUCCESS;

        if (NULL != pWaveYangMemberElementFilterInformation)
        {
            filterMatchStatus = pWaveYangMemberElementFilterInformation->doesFilterInformationMatch (pYangDisplayConfigurationContext);
        }

        if (WAVE_MESSAGE_SUCCESS == filterMatchStatus)
        {
            displayConfigurationForOneInstanceOfList (pYangDisplayConfigurationContext, selectedChildElementsWithFilter, pWaveManagedObject);
        }

        if (true == getIsNodeSpecificBaseList ())
        {
            pYangDisplayConfigurationContext->resetOwnerWaveNodeObjectId ();
        }

        if (true == getIsMultiPartitionBaseList ())
        {
            pYangDisplayConfigurationContext->resetOwnerPartitionObjectId ();
        }

        pYangDisplayConfigurationContext->incrementIndexForLastConfigLevel ();

        /* */ pYangDisplayConfigurationContext->discardContentsOfPartialOutputBuffer ();
    }

    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        break;
    }

    if ((numberOfManagedObjects < pYangDisplayConfigurationContext->getBatchSizeForQueryingListInstances ()) || (0 == pYangDisplayConfigurationContext->getBatchSizeForQueryingListInstances ()))
    {
        break;
    }
    else
    {
        pageNumber++;

        pYangDisplayConfigurationContext->getPWaveManagedObjects (this, pWaveManagedObjects, pageNumber);

        numberOfManagedObjects = pWaveManagedObjects.size ();

        if (0 == numberOfManagedObjects)
        {
            break;
        }

        pYangDisplayConfigurationContext->addNextBatchOfWaveManagedObjects (pWaveManagedObjects, this);
    }

  } while (0 != numberOfManagedObjects);

    if (NULL != pWaveYangMemberElementFilterInformation)
    {
        pWaveYangMemberElementFilterInformation->restoreAbsorbedTokens (pYangDisplayConfigurationContext);

        delete pWaveYangMemberElementFilterInformation;
    }

    pYangDisplayConfigurationContext->decrementCommandKeyIndex ();

    pYangDisplayConfigurationContext->deleteLastConfigLevel ();
}

void YangList::displayConfigurationForOneInstanceOfList (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData, WaveManagedObject *pWaveManagedObject)
{
    bool whenConditionSatisfied = evaluateWhenConditions (pYangDisplayConfigurationContext);

    if (false == whenConditionSatisfied)
    {
        return;
    }

    string keyCombinationValueString;
	prismAssert (NULL != pWaveManagedObject, __FILE__, __LINE__);

    ResourceId listInstanceKeyStatus = getKeyCombinationValueString (pYangDisplayConfigurationContext, pWaveManagedObject, keyCombinationValueString);

    if (WAVE_MESSAGE_SUCCESS != listInstanceKeyStatus)
    {
        return;
    }

    bool compactSyntax = false;

    if (true == (getCliCompactSyntax ()).empty ())  // else, it has newline characters.
    {
        compactSyntax = true;
    }

    pYangDisplayConfigurationContext->addYangElementDisplayState (getIsCliSuppressMode (), compactSyntax);  // [

    string listInstanceNameString;

    if(true != getIsCliRunTemplateEnter ())
    {

        string nodeName = (false == getDropNodeNameForCli ()) ? getDisplayName () : "";

        listInstanceNameString = StringUtils::joinStrings (nodeName, keyCombinationValueString);
    }
    else
    {
        WaveCustomCliDisplayConfigurationContext *pWaveCustomCliDisplayConfigurationContext = new WaveCustomCliDisplayConfigurationContext (this, pYangDisplayConfigurationContext);

        WaveManagedObject * pWaveManagedObjectProper = NULL;

        if (NULL == pWaveManagedObject) 
        {
            bool disableFilters = false;

            pYangDisplayConfigurationContext->getPWaveManagedObject (this, pWaveManagedObjectProper, disableFilters);
        }
        else
        {   
            pWaveManagedObjectProper = pWaveManagedObject;
        }

        listInstanceNameString =  pWaveManagedObjectProper->customConfigurationDisplay (pWaveCustomCliDisplayConfigurationContext);
    }

    if (false == getIsCliSuppressMode ())
    {
        if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
        {
            pYangDisplayConfigurationContext->setPendingOutputString (listInstanceNameString);
        }
        else
        {
            pYangDisplayConfigurationContext->printHeaderString (listInstanceNameString);
        }

        pYangDisplayConfigurationContext->incrementDepth ();    // [
    }
    else
    {
        pYangDisplayConfigurationContext->setPrefixString (listInstanceNameString);
    }

    UI32 numberOfSelectedChildElements = selectedChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfSelectedChildElements; i++)
    {
        YangElement *pYangElement = selectedChildElementsForData[i];

        if (YangLeaf::getYangName () == pYangElement->getYangName ())
        {
            YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pYangElement);
            prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

            if (true == isLeafNamePartOfKeyCombination (pYangLeaf->getName ()))
            {
                // pass
            }
            else
            {
                pYangLeaf->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, pWaveManagedObject);
            }

            continue;
        }

        if (YangLeafList::getYangName () == pYangElement->getYangName ())
        {
            YangLeafList *pYangLeafList = dynamic_cast<YangLeafList *> (pYangElement);
            prismAssert (NULL != pYangLeafList, __FILE__, __LINE__);

            pYangLeafList->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, pWaveManagedObject);

            continue;
        }

        if (YangContainer::getYangName () == pYangElement->getYangName ())
        {
            YangContainer *pYangContainer = dynamic_cast<YangContainer *> (pYangElement);
            prismAssert (NULL != pYangContainer, __FILE__, __LINE__);

            pYangDisplayConfigurationContext->printNewLineIfRequired ();

            pYangContainer->displayConfiguration1 (pYangDisplayConfigurationContext);

            continue;
        }

        pYangElement->displayConfiguration1 (pYangDisplayConfigurationContext);
    }

    if (false == getIsCliSuppressMode ())
    {
#if 0
        if ((0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ()) && (false == pYangDisplayConfigurationContext->getOutputWasPrinted ()))
        {
            pYangDisplayConfigurationContext->printPendingOutput ();
        }

        pYangDisplayConfigurationContext->decrementDepth ();    // ]
        pYangDisplayConfigurationContext->printSeparator ();
#endif

        pYangDisplayConfigurationContext->decrementDepth ();    // ]

        if (((0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ()) && (true == pYangDisplayConfigurationContext->getOutputWasPrinted ()))
            || (0 == pYangDisplayConfigurationContext->getNumberOfRemainingTokens ()))
        {
            pYangDisplayConfigurationContext->printSeparator ();
        }
    }
    else
    {
        if (false == pYangDisplayConfigurationContext->getOutputWasPrinted ())
        {
            if (false == m_hasChildLeafWithCustomDisplay)
            {
                // TODO : handle the case where leaf with 'custom display' was not selected for display.

                pYangDisplayConfigurationContext->printHeaderString (listInstanceNameString);
            }
        }

        pYangDisplayConfigurationContext->setPrefixString ("");
        pYangDisplayConfigurationContext->printNewLineIfRequired ();
    }

    pYangDisplayConfigurationContext->removeLastYangElementDisplayState ();                                 // ]
}

ResourceId YangList::getKeyCombinationValueString (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject, string &keyCombinationValueString)
{
    ResourceId listInstanceKeyStatus = WAVE_MESSAGE_SUCCESS;

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = getPConfigurationSegmentInformation ();
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    const vector<string> &userDefinedKeyCombination = pConfigurationSegmentInformation->getUserDefinedKeyCombination ();

    UI32 numberOfAttributeNamesInUserDefinedKeyCombination  = userDefinedKeyCombination.size ();
    UI32 numberOfKeyCombinationEntries                      = m_keyCombination.size ();

    // For 'config' data list, at least one key should be present.

    prismAssert (0 != numberOfKeyCombinationEntries, __FILE__, __LINE__);
    prismAssert (numberOfKeyCombinationEntries == numberOfAttributeNamesInUserDefinedKeyCombination, __FILE__, __LINE__);

    for (UI32 i = 0; i < numberOfKeyCombinationEntries; i++)
    {
        string keyEntry = m_keyCombination[i];

        // 1. Name (if exposed)

        string keyName;

        YangElement *pYangElement = getUniqueYangElementByName (keyEntry);
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);
        prismAssert (true == pYangElement->getIsConfiguration (), __FILE__, __LINE__);

        if (false == (pYangElement->getExposeKeyName ()).empty ())
        {
            keyName = pYangElement->getDisplayName ();
        }

        // 2. Value

        // Note : For leafs in key-combination in yang model, 'user-tag to Attribute mapping' may not be present.
        //        So, match userDefinedKeyCombination (from ORM) and m_keyCombination (from Yang) in sequence.

        // Assert that it is Data element.
        YangDataElement *pYangDataElement = dynamic_cast<YangDataElement *> (pYangElement);
        prismAssert (NULL != pYangDataElement, __FILE__, __LINE__);

        Attribute *pAttribute = pWaveManagedObject->getAttributeByName (userDefinedKeyCombination[i]);
        prismAssert (NULL != pAttribute, __FILE__, __LINE__);

        string      valueString;
        ResourceId  valueType                       = WAVE_MESSAGE_ERROR;
        const bool  checkForAttributeDefaultValue   = false;

        ResourceId  getStringStatus = pYangDataElement->getValueStringForAttribute (pYangDisplayConfigurationContext, pWaveManagedObject, pAttribute, valueString, valueType, checkForAttributeDefaultValue);

        if (WAVE_MESSAGE_SUCCESS != getStringStatus)
        {
            listInstanceKeyStatus = getStringStatus;

            break;
        }

        ResourceId formatStringStatus = formatValueString (valueString, valueType);

        if (WAVE_MESSAGE_SUCCESS != formatStringStatus)
        {
            listInstanceKeyStatus = formatStringStatus;

            break;
        }

#if 0
        bool    isDefaultValuePresent = false;
        string  defaultValueString;

        getDefaultValueStringForKeyIfPresent (keyEntry, isDefaultValuePresent, defaultValueString);

        bool    printKeyValue         = true;

        if (true == isDefaultValuePresent)
        {
            if (defaultValueString == valueString)
            {
                printKeyValue = false;
            }
        }

        if (true == printKeyValue)
        {
            if (i != 0)
            {
                keyCombinationValueString += " ";
            }

            keyCombinationValueString += StringUtils::joinStrings (keyName, valueString);
        }
#endif

        if (i != 0)
        {
            keyCombinationValueString += " ";
        }

        keyCombinationValueString += StringUtils::joinStrings (keyName, valueString);
    }

    return listInstanceKeyStatus;
}

void YangList::restoreAbsorbedKeys (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const UI32 &numberOfKeysAbsorbed) const
{
    for (UI32 i = 0; i < numberOfKeysAbsorbed; i++)
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();
    }

    pYangDisplayConfigurationContext->removeLastCommandKey ();
}

UI32 YangList::absorbeKey (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData) const
{
    vector<string>  keyCombination                      = getKeyCombination ();
    UI32            numberOfElementsInKeyCombination    = keyCombination.size ();
    vector<string>  keyVector;

    UI32            numberOfTokensAbsorbed              = 0;

    for (UI32 i = 0; i < numberOfElementsInKeyCombination; i++)
    {
        if (0 == pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
        {
            break;
        }

        YangElement *pYangElement = getUniqueYangElementByName (keyCombination[i]);
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        if (false == (pYangElement->getExposeKeyName ()).empty ())
        {
            if (2 <= pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
            {
                string exposedKeyName = pYangDisplayConfigurationContext->advanceToNextToken ();

                if (exposedKeyName == pYangElement->getDisplayName ())
                {
                    string keyValue = pYangDisplayConfigurationContext->advanceToNextToken ();

                    keyVector.push_back (keyValue);

                    numberOfTokensAbsorbed++;
                    numberOfTokensAbsorbed++;

                    continue;
                }
                else
                {
                    pYangDisplayConfigurationContext->decrementTokenPosition ();
                }
            }
            else
            {
                string exposedKeyName = pYangDisplayConfigurationContext->advanceToNextToken ();

                numberOfTokensAbsorbed++;

                if (exposedKeyName == pYangElement->getDisplayName ())
                {
                    break;
                }
                else
                {
                    pYangDisplayConfigurationContext->decrementTokenPosition ();

                    numberOfTokensAbsorbed--;
                }
            }
        }

        string nextToken = pYangDisplayConfigurationContext->advanceToNextToken ();

        numberOfTokensAbsorbed++;

        vector<YangElement *> childElements;

        // skip key leafs.

        getSelectedChildElementsForDataWithExactMatch (nextToken, childElements);

        UI32 numberOfSelectedChildElements = childElements.size ();

        if (0 == numberOfSelectedChildElements)
        {
            keyVector.push_back (nextToken);
        }
        else if (1 == numberOfSelectedChildElements)
        {
            YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (childElements[0]);

            if (NULL != pYangLeaf)
            {
                if (true == isLeafNamePartOfKeyCombination (pYangLeaf->getName ()))
                {
                    keyVector.push_back (nextToken);
                }
                else
                {
                    pYangDisplayConfigurationContext->decrementTokenPosition ();

                    numberOfTokensAbsorbed--;

                    break;
                }
            }
            else
            {
                pYangDisplayConfigurationContext->decrementTokenPosition ();

                numberOfTokensAbsorbed--;

                break;
            }
        }
        else
        {
            pYangDisplayConfigurationContext->decrementTokenPosition ();

            numberOfTokensAbsorbed--;

            break;
        }
    }

    pYangDisplayConfigurationContext->insertCommandKey (keyVector);

    return numberOfTokensAbsorbed;

#if 0
     trace (TRACE_LEVEL_DEBUG, string ("YangList::absorbeKey : Entering...."));

     UI32 keyCount  = 0;
     bool keyProcessingDone = false;
     vector<string> keyVector;
     vector<string> keyCombinations = getKeyCombination();

     string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();
     getSelectedChildElementsForData (tokenString, selectedChildElementsForData);


     while ( (true == selectedChildElementsForData.empty ()) && (false == keyProcessingDone))
     {
         trace (TRACE_LEVEL_ERROR, "YangList:: absorbeKey" + getYangName () + ":" + getName () + " could not find child elements for [" + tokenString + "]");

         keyVector.push_back(tokenString);

         keyCount ++;

         if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
         {
            tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();
            getSelectedChildElementsForData (tokenString, selectedChildElementsForData);
         }
         else
         {
            keyProcessingDone = true ;
         }

     }

     pYangDisplayConfigurationContext->insertCommandKey(keyVector);

     if (false == selectedChildElementsForData.empty ())
     {
        pYangDisplayConfigurationContext->decrementTokenPosition ();
     }

     // User entered More keys then expected
     if(keyCount > keyCombinations.size())
     {
        trace (TRACE_LEVEL_ERROR, "YangList::absorbeKey : user entered invalid keys ----------");
        return 0;
     }
     else
     {
        return keyCount;
     }
#endif
}

ResourceId YangList::getStringRepresentationForKeyValueAtIndex (const UI32 &index, const string &userInputKeyValue, string &stringRepresentation) const
{
    UI32 numberOfKeyCombinationEntries = m_keyCombination.size ();

    prismAssert (index < numberOfKeyCombinationEntries, __FILE__, __LINE__);

    if ("enumeration" == m_keyCombinationTypes[index])
    {
        string keyLeafName = m_keyCombination[index];

        YangElement *pYangElement = getUniqueYangElementByName (keyLeafName);
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        vector<YangElement *> allYangTypeChildElements;
        pYangElement->getAllChildrenByYangName (YangType::getYangName (), allYangTypeChildElements);

        YangType *pYangType = dynamic_cast<YangType *> (allYangTypeChildElements[0]);
        prismAssert (NULL != pYangType, __FILE__, __LINE__);

        UI32 enumValue = 0;
        ResourceId status = pYangType->getValueFromEnumName (userInputKeyValue, enumValue);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            stringRepresentation = string ("") + enumValue;
        }

        return status;
    }
    else
    {
        stringRepresentation = userInputKeyValue;

        return WAVE_MESSAGE_SUCCESS;
    }
}

void YangList::propagateCompactSyntaxOneLevelForSelf ()
{
    YangElement *pParentYangElement = getPParentElement ();
    prismAssert (NULL != pParentYangElement, __FILE__, __LINE__);

    if (true == (pParentYangElement->getCliCompactSyntax ()).empty ())
    {
        setCliCompactSyntax ();
    }
}

WaveYangMemberElementFilterInformation *YangList::collectMemberElementFilterInformation (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const
{
    if (0 == pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        return NULL;
    }

    WaveYangMemberElementFilterInformation *pWaveYangMemberElementFilterInformation = NULL;

    vector<YangElement *> leafChildElements;
    
    getAllChildrenByYangName (YangLeaf::getYangName (), leafChildElements);

    UI32 numberOfLeafChildElements = leafChildElements.size ();

    for (UI32 i = 0; i < numberOfLeafChildElements; i++)
    {
        YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (leafChildElements[i]);
        prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

        if (true == isLeafNamePartOfKeyCombination (pYangLeaf->getName ()))
        {
            continue;
        }

        pWaveYangMemberElementFilterInformation = pYangLeaf->collectMemberElementFilterInformation (pYangDisplayConfigurationContext);

        if (NULL != pWaveYangMemberElementFilterInformation)
        {
            break;
        }
    }

    return pWaveYangMemberElementFilterInformation;
}

}
