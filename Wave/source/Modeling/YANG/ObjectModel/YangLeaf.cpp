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

#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "Modeling/YANG/ObjectModel/YangType.h"
#include "Modeling/YANG/ObjectModel/YangDefault.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

YangLeaf::YangLeaf ()
    : YangDataElement (getYangName (), ""),
      m_isEmpty       (false)
{
}

YangLeaf::~YangLeaf()
{
}

string YangLeaf::getYangName ()
{
    return ("leaf");
}

YangElement *YangLeaf::createInstance ()
{
    return (new YangLeaf ());
}

void YangLeaf::computeCliTargetNodeNameForSelfInternal ()
{
    string cliTargetNodeName  = getCliTargetNodeName  ();
    bool   dropNodeNameForCli = getDropNodeNameForCli ();
    string displayName        = getDisplayName        ();

    if ("" != cliTargetNodeName)
    {
        cliTargetNodeName += " ";
    }

    if (false == dropNodeNameForCli)
    {
        cliTargetNodeName += displayName;
    }
    else
    {
        cliTargetNodeName += "(" + displayName + ")";
    }

    setCliTargetNodeName (cliTargetNodeName);
}

bool YangLeaf::getIsEmpty () const
{
    return (m_isEmpty);
}

void YangLeaf::setIsEmpty (const bool &isEmpty)
{
    m_isEmpty = isEmpty;
}

void YangLeaf::processChildElement (YangElement *pYangElement)
{
    const string yangName = pYangElement->getYangName ();

    if ((YangType::getYangName()) == yangName)
    {
        YangType *pYangType = dynamic_cast<YangType *> (pYangElement);

        prismAssert (NULL != pYangType, __FILE__, __LINE__);

        string nameValue;

        pYangType->getAttributeValue ("name", nameValue);

        prismAssert ("" != nameValue, __FILE__, __LINE__);

        if ("empty" == nameValue)
        {
            setIsEmpty (true);
        }
    }

    YangDataElement::processChildElement (pYangElement);
}

void YangLeaf::displayConfigurtionForLeaf (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject)
{
    // Skip if not required to be shown.

    if (true == getIsHidden (pYangDisplayConfigurationContext))
    {
        return;
    }

    if (false == getIsConfiguration ())
    {
        return;
    }

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = getPConfigurationSegmentInformation ();

    if (NULL == pConfigurationSegmentInformation)
    {
        trace (TRACE_LEVEL_ERROR, string ("YangLeaf::displayConfigurtionForLeaf : does not know about [") + getConfigurationSegmentName () + string ("]"));

        return;
    }
    
    if (true == pYangDisplayConfigurationContext->isConfigFilterApplicable (this))
    {
        return;
    }

    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        return;
    }

    bool whenConditionSatisfied = evaluateWhenConditions (pYangDisplayConfigurationContext);

    if (false == whenConditionSatisfied)
    {
        return;
    }

    bool needToMatchLeafValue = false;

    string leafValueInput;

    bool decrementTokenPositionFlag = false;

    bool checkForYangDefault = true;

    if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        decrementTokenPositionFlag = true;

        string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

        checkForYangDefault = false;

        if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
        {
            needToMatchLeafValue = true;

            leafValueInput = pYangDisplayConfigurationContext->advanceToNextToken ();
        }
    }

    if (true == getIsCliRunTemplate ())
    {   
        WaveCustomCliDisplayConfigurationContext *pWaveCustomCliDisplayConfigurationContext = new WaveCustomCliDisplayConfigurationContext (this, pYangDisplayConfigurationContext);

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

        if (NULL != pWaveManagedObjectProper)
        {
            string displayString =  pWaveManagedObjectProper->customConfigurationDisplay (pWaveCustomCliDisplayConfigurationContext);

            if (false == displayString.empty ())
            {
                string templateOutputForDisplay = pYangDisplayConfigurationContext->getSpacePrefixString () + displayString;
                string outputWithSlashRRemoved  = StringUtils::removeAllCharactersBetweenNewLineCharacterAndReturnCharacter (templateOutputForDisplay);
                pYangDisplayConfigurationContext->printCustomTemplateConfigOutput (outputWithSlashRRemoved);
            }
        }

        delete pWaveCustomCliDisplayConfigurationContext;
    }
    else
    {
        displayConfigurtionForSingleElement (pYangDisplayConfigurationContext, pWaveManagedObject, needToMatchLeafValue, leafValueInput, checkForYangDefault);
    }

    if (true == decrementTokenPositionFlag)
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();

        if (true == needToMatchLeafValue)
        {
            pYangDisplayConfigurationContext->decrementTokenPosition ();
        }
    }
}

bool YangLeaf::isValueSameAsDefault (const string &valueString, ResourceId &valueType)
{
    bool sameAsDefault = false;

    vector<YangElement *> yangDefaultElements;
    getAllChildrenByYangName (YangDefault::getYangName (), yangDefaultElements);

    if (1 == yangDefaultElements.size ())
    {
        YangDefault *pYangDefault = dynamic_cast<YangDefault *> (yangDefaultElements[0]);
        prismAssert (NULL != pYangDefault, __FILE__, __LINE__);

        string defaultValueString;

        pYangDefault->getAttributeValue ("value", defaultValueString);

        if (valueString == defaultValueString)
        {
            sameAsDefault = true;
        }
    }

    // Managed Objects have AttributeString, YangModel has type enum, and in this case, even if string matches, do not treat as default.

    if ((true == sameAsDefault) && ((WAVE_ATTRIBUTE_TYPE_STRING == valueType) || (WAVE_ATTRIBUTE_TYPE_STRING_UC == valueType)))
    {
        vector<YangElement *> yangTypeChildElements;
        getAllChildrenByYangName (YangType::getYangName (), yangTypeChildElements);

        if (false == yangTypeChildElements.empty ())
        {
            YangType *pYangType = dynamic_cast<YangType *> (yangTypeChildElements[0]);
            prismAssert (NULL != pYangType, __FILE__, __LINE__);

            if ("enumeration" == pYangType->getName ())
            {
                sameAsDefault = false;
            }
        }
    }

    return sameAsDefault;
}

ResourceId YangLeaf::doesElementValueMatch (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, const string &matchValue)
{
    bool                disableFilters      = false;
    WaveManagedObject  *pWaveManagedObject  = NULL;

    pYangDisplayConfigurationContext->getPWaveManagedObject (this, pWaveManagedObject, disableFilters);

    if (NULL == pWaveManagedObject)
    {
        return WAVE_MESSAGE_ERROR;
    }

    bool    nothingToPrint      = false;
    bool    printNoConfig       = false;
    string  configString;       // Not used here.
    bool    incompleteCommand   = false;
    string  configValue;
    bool    checkForYangDefault = false;

    getConfigurtionForSingleElement (pYangDisplayConfigurationContext, pWaveManagedObject, nothingToPrint, printNoConfig, configString, incompleteCommand, configValue, checkForYangDefault);

    if (true == nothingToPrint)
    {
        return WAVE_MESSAGE_ERROR;
    }

    if (matchValue == configValue)
    {
        return WAVE_MESSAGE_SUCCESS;
    }

    return WAVE_MESSAGE_ERROR;
}

WaveYangMemberElementFilterInformation *YangLeaf::collectMemberElementFilterInformation (YangDisplayConfigurationContext *pYangDisplayConfigurationContext) const
{
    // TODO : Support leafs with drop-node-name - if parent is container.

    if (2 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        return NULL;
    }

    string firstToken = pYangDisplayConfigurationContext->advanceToNextToken ();

    if (firstToken != getDisplayName ())
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();

        return NULL;
    }

    string matchValue = pYangDisplayConfigurationContext->advanceToNextToken ();

    WaveYangMemberElementFilterInformation *pWaveYangMemberElementFilterInformation = new WaveYangMemberElementFilterInformation (this, matchValue, NULL);
    prismAssert (NULL != pWaveYangMemberElementFilterInformation, __FILE__, __LINE__);

    pWaveYangMemberElementFilterInformation->setNumberOfTokensAbsorbed (2);

    return pWaveYangMemberElementFilterInformation;
}

}
