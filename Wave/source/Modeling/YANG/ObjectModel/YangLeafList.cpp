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

#include "Modeling/YANG/ObjectModel/YangLeafList.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"

namespace WaveNs
{

YangLeafList::YangLeafList ()
    : YangDataElement (getYangName (), "")
{
}

YangLeafList::~YangLeafList()
{
}

string YangLeafList::getYangName ()
{
    return ("leaf-list");
}

YangElement *YangLeafList::createInstance ()
{
    return (new YangLeafList ());
}

void YangLeafList::displayConfigurtionForLeaf (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, WaveManagedObject *pWaveManagedObject)
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

    bool decrementTokenPositionFlag = false;

    bool checkForYangDefault = true;

    if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        decrementTokenPositionFlag = true;

        string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

        checkForYangDefault = false;
    }

    displayConfigurtionForSingleElement (pYangDisplayConfigurationContext, pWaveManagedObject, false, "", checkForYangDefault);

    if (true == decrementTokenPositionFlag)
    {
        pYangDisplayConfigurationContext->decrementTokenPosition ();
    }
}

// virtual
void YangLeafList::applyFormattingToConfigString (string &configString) const
{
    // TODO : handle flat-list-syntax

    if (false == getIsCliFlatListSyntax() )
    {
        configString = "[ " + configString + " ]";
    }
}

}
