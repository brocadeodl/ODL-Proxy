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

#include "Modeling/YANG/ObjectModel/YangContainer.h"
#include "Modeling/YANG/ObjectModel/YangPresence.h"
#include "Modeling/YANG/ObjectModel/YangChoice.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"
#include "Modeling/YANG/ObjectModel/YangLeafList.h"

namespace WaveNs
{

YangContainer::YangContainer ()
    : YangDataElement       (getYangName (), ""),
      m_isPresenceContainer (false)
{
    setIsConfiguration (false);

    m_pYangLeafWithHideInSubMode = NULL;
}

YangContainer::YangContainer (const string &name)
    : YangDataElement       (getYangName (), name),
      m_isPresenceContainer (false)
{
    setIsConfiguration (false);

    m_pYangLeafWithHideInSubMode = NULL;
}

YangContainer::~YangContainer()
{
}

string YangContainer::getYangName ()
{
    return ("container");
}

YangElement *YangContainer::createInstance ()
{
    return (new YangContainer ());
}

void YangContainer::processChildElement (YangElement *pYangElement)
{
    YangDataElement *pYangDataElement = dynamic_cast<YangDataElement *> (pYangElement);

    if ((NULL != pYangDataElement) || ((YangChoice::getYangName ()) == (pYangElement->getYangName ())))
    {
        if (false == (getIsConfigurationExplicitlySet ()))
        {
            setIsConfiguration (true);
        }
    }

    YangDataElement::processChildElement (pYangElement);

    const string yangName = pYangElement->getYangName ();

    if ((YangPresence::getYangName()) == yangName)
    {
         setIsConfiguration (true);

         m_isPresenceContainer = true;
    }
}

bool YangContainer::meetsCriteriaToBeShownInConfiguration () const
{
    return (m_isPresenceContainer);
}

void YangContainer::collectHideInSubModeLeafForSelf ()
{
    vector<YangElement *> pChildLeafYangElements;
    getAllChildrenByYangName (YangLeaf::getYangName (), pChildLeafYangElements);

    UI32 numberOfPChildLeafYangElements = pChildLeafYangElements.size ();

    for (UI32 i = 0; i < numberOfPChildLeafYangElements; i++)
    {
        YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pChildLeafYangElements[i]);
        prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

        if (true == pYangLeaf->getCliHideInSubMode ())
        {
            m_pYangLeafWithHideInSubMode = pYangLeaf;

            break;
        }
    }
}

void YangContainer::displayConfiguration3 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
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

    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        return;
    }

    bool whenConditionSatisfied = evaluateWhenConditions (pYangDisplayConfigurationContext);

    if (false == whenConditionSatisfied)
    {
        return;
    }

    // Get cli-modifiers for state.

    bool compactSyntax = false;

    if (true == (getCliCompactSyntax ()).empty ())  // else, it has newline characters.
    {
        compactSyntax = true;
    }

    bool addMode = false;

    if (true == getIsCliAddMode ())
    {
        addMode = true;
    }

    if (true == m_isPresenceContainer)
    {
        bool isNotPresent = false;

        WaveManagedObject *pWaveManagedObject = NULL;

        bool disableFilters = false;

        pYangDisplayConfigurationContext->getPWaveManagedObject (this, pWaveManagedObject, disableFilters);

        if (NULL != pWaveManagedObject)
        {
            string      valueString;
            ResourceId  valueType;

            if (WAVE_MESSAGE_SUCCESS == getValueString (pYangDisplayConfigurationContext, pWaveManagedObject, valueString, valueType))
            {
                if ("false" == valueString)
                {
                    isNotPresent = true;
                }
            }
            else
            {
                isNotPresent = true;
            }
        }
        else
        {
            isNotPresent = true;
        }

        if (true == isNotPresent)
        {
            if (true == getIsCliShowNo ())
            {
                pYangDisplayConfigurationContext->printNoConfigString (getDisplayName ());
            }

            return;
        }

        // pYangDisplayConfigurationContext->printConfigString (getDisplayName ());
    }

    // Create state.

    pYangDisplayConfigurationContext->addYangElementDisplayState (addMode ? false : true, compactSyntax);       // [

    string nodeName = (false == getDropNodeNameForCli ()) ? getDisplayName () : "";

    bool hideInSubModeLeafFound = false;

    if (true == addMode)
    {
        string configurationString;
        hideInSubModeLeafFound = getConfigurationStringForLeafWithHideInSubMode (pYangDisplayConfigurationContext, configurationString);
        pYangDisplayConfigurationContext->setPendingOutputString (StringUtils::joinStrings (nodeName, configurationString));
        pYangDisplayConfigurationContext->incrementDepth ();                // [
    }
    else
    {
        pYangDisplayConfigurationContext->setPrefixString (nodeName);
    }

    // Display configuration for selected child elements.

    UI32 numberOfSelectedChildElementsForData = selectedChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfSelectedChildElementsForData; i++)
    {
        YangElement *pYangElement = selectedChildElementsForData[i];

        if (YangLeaf::getYangName () == pYangElement->getYangName ())
        {
            if (true == pYangElement->getCliHideInSubMode ())
            {
                continue;
            }

            YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pYangElement);
            prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

            pYangLeaf->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, NULL);

            continue;
        }

        if (YangLeafList::getYangName () == pYangElement->getYangName ())
        {
            YangLeafList *pYangLeafList = dynamic_cast<YangLeafList *> (pYangElement);
            prismAssert (NULL != pYangLeafList, __FILE__, __LINE__);

            pYangLeafList->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, NULL);

            continue;
        }

        pYangElement->displayConfiguration1 (pYangDisplayConfigurationContext);
    }

    if (false == addMode)
    {
        pYangDisplayConfigurationContext->setPrefixString ("");

        if (true == m_isPresenceContainer)
        {
            if (false == pYangDisplayConfigurationContext->getOutputWasPrinted ())
            {
                pYangDisplayConfigurationContext->printConfigString (nodeName);
            }
        }

        // container is over. Flush pending newline.

        pYangDisplayConfigurationContext->printNewLineIfRequired ();
    }
    else
    {
        bool needToPrintModeSeparator = true;

        if (true == hideInSubModeLeafFound)
        {
            bool tokenAbsorbed = false;

            if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
            {
                string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

                if (tokenString == m_pYangLeafWithHideInSubMode->getDisplayName ())
                {
                    tokenAbsorbed = true;
                }
                else
                {
                    pYangDisplayConfigurationContext->decrementTokenPosition ();
                }
            }

            pYangDisplayConfigurationContext->printPendingOutput ();

            if (true == tokenAbsorbed)
            {
                pYangDisplayConfigurationContext->decrementTokenPosition ();
            }
        }

        if (false == pYangDisplayConfigurationContext->getOutputWasPrinted ())
        {
            if (true == m_isPresenceContainer)
            {
                pYangDisplayConfigurationContext->printPendingOutput ();
            }
            else
            {
                pYangDisplayConfigurationContext->resetPendingOutputString ();

                needToPrintModeSeparator = false;
            }
        }

        pYangDisplayConfigurationContext->decrementDepth ();                // ]

        if (true == needToPrintModeSeparator)
        {
            pYangDisplayConfigurationContext->printSeparator ();
        }
    }

    // Delete state.

    pYangDisplayConfigurationContext->removeLastYangElementDisplayState ();                                     // ]
}

void YangContainer::displayConfiguration2 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
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

    if (true == pYangDisplayConfigurationContext->isCommandAbortedByUser ())
    {
        return;
    }

    bool whenConditionSatisfied = evaluateWhenConditions (pYangDisplayConfigurationContext);

    if (false == whenConditionSatisfied)
    {
        return;
    }

    // Get cli-modifiers for state.

    bool compactSyntax = false;

    if (true == (getCliCompactSyntax ()).empty ())  // else, it has newline characters.
    {
        compactSyntax = true;
    }

    bool addMode = false;

    if (true == getIsCliAddMode ())
    {
        addMode = true;
    }

    if (true == m_isPresenceContainer)
    {
        bool isNotPresent = false;

        WaveManagedObject *pWaveManagedObject = NULL;

        bool disableFilters = false;

        pYangDisplayConfigurationContext->getPWaveManagedObject (this, pWaveManagedObject, disableFilters);

        if (NULL != pWaveManagedObject)
        {
            string      valueString;
            ResourceId  valueType;

            if (WAVE_MESSAGE_SUCCESS == getValueString (pYangDisplayConfigurationContext, pWaveManagedObject, valueString, valueType))
            {
                if ("false" == valueString)
                {
                    isNotPresent = true;
                }
            }
            else
            {
                isNotPresent = true;
            }
        }
        else
        {
            isNotPresent = true;
        }

        if (true == isNotPresent)
        {
            if (true == getIsCliShowNo ())
            {
                pYangDisplayConfigurationContext->printNoConfigString (getDisplayName ());
            }

            return;
        }

        // pYangDisplayConfigurationContext->printConfigString (getDisplayName ());
    }

    // Create state.

    pYangDisplayConfigurationContext->addYangElementDisplayState (addMode ? false : true, compactSyntax);       // [

    string nodeName = (false == getDropNodeNameForCli ()) ? getDisplayName () : "";

    bool hideInSubModeLeafFound = false;

    if (false == addMode)
    {
        pYangDisplayConfigurationContext->setPrefixString (nodeName);
    }
    else
    {
        string configurationString;
        hideInSubModeLeafFound = getConfigurationStringForLeafWithHideInSubMode (pYangDisplayConfigurationContext, configurationString);
        pYangDisplayConfigurationContext->setPendingOutputString (StringUtils::joinStrings (nodeName, configurationString));
        pYangDisplayConfigurationContext->incrementDepth ();                            // [
    }

    // Display configuration for selected child elements.

    UI32 numberOfSelectedChildElementsForData = selectedChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfSelectedChildElementsForData; i++)
    {
        YangElement *pYangElement = selectedChildElementsForData[i];

        if (YangLeaf::getYangName () == pYangElement->getYangName ())
        {
            if (true == pYangElement->getCliHideInSubMode ())
            {
                continue;
            }

            YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pYangElement);
            prismAssert (NULL != pYangLeaf, __FILE__, __LINE__);

            pYangLeaf->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, NULL);

            continue;
        }

        if (YangLeafList::getYangName () == pYangElement->getYangName ())
        {
            YangLeafList *pYangLeafList = dynamic_cast<YangLeafList *> (pYangElement);
            prismAssert (NULL != pYangLeafList, __FILE__, __LINE__);

            pYangLeafList->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, NULL);

            continue;
        }

        pYangElement->displayConfiguration1 (pYangDisplayConfigurationContext);
    }

    if (false == addMode)
    {
        pYangDisplayConfigurationContext->setPrefixString ("");

        bool outputPrinted = pYangDisplayConfigurationContext->getOutputWasPrinted ();

        if (true == m_isPresenceContainer)
        {
            if (false == outputPrinted)
            {
                pYangDisplayConfigurationContext->printConfigString (nodeName);

                // outputPrinted = true;
            }
        }

        // container is over. Flush pending newline.

        pYangDisplayConfigurationContext->printNewLineIfRequired ();
    }
    else
    {
        bool needToPrintModeSeparator = true;

        if (true == hideInSubModeLeafFound)
        {
            bool tokenAbsorbed = false;

            if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
            {
                string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

                if (tokenString == m_pYangLeafWithHideInSubMode->getDisplayName ())
                {
                    tokenAbsorbed = true;
                }
                else
                {
                    pYangDisplayConfigurationContext->decrementTokenPosition ();
                }
            }

            pYangDisplayConfigurationContext->printPendingOutput ();

            if (true == tokenAbsorbed)
            {
                pYangDisplayConfigurationContext->decrementTokenPosition ();
            }
        }

        if (false == pYangDisplayConfigurationContext->getOutputWasPrinted ())
        {
            if (true == m_isPresenceContainer)
            {
                pYangDisplayConfigurationContext->printPendingOutput ();
            }
            else
            {
                pYangDisplayConfigurationContext->resetPendingOutputString ();

                needToPrintModeSeparator = false;
            }
        }

        pYangDisplayConfigurationContext->decrementDepth ();                            // ]

        if (true == needToPrintModeSeparator)
        {
            pYangDisplayConfigurationContext->printSeparator ();
        }
    }

    // Delete state.

    pYangDisplayConfigurationContext->removeLastYangElementDisplayState ();                                     // ]
}

bool YangContainer::getConfigurationStringForLeafWithHideInSubMode (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, string &configurationString)
{
    if (NULL == m_pYangLeafWithHideInSubMode)
    {
        return false;
    }

    bool    nothingToPrint      = false;
    bool    printNoConfig       = false;
    string  configString;
    bool    incompleteCommand   = false;
    string  configValue;

    m_pYangLeafWithHideInSubMode->getConfigurtionForSingleElement (pYangDisplayConfigurationContext, NULL, nothingToPrint, printNoConfig, configString, incompleteCommand, configValue, false);

    if (true == nothingToPrint)
    {
        return false;
    }

    if (true == printNoConfig)
    {
        return false;
    }

    configurationString = configString;

    return true;
}

void YangContainer::propagateCompactSyntaxOneLevelForSelf ()
{
    YangElement *pParentYangElement = getPParentElement ();
    prismAssert (NULL != pParentYangElement, __FILE__, __LINE__);

    if (true == (pParentYangElement->getCliCompactSyntax ()).empty ())
    {
        setCliCompactSyntax ();
    }
}

}
