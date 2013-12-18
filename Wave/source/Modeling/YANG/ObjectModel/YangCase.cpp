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

#include "Modeling/YANG/ObjectModel/YangCase.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"

namespace WaveNs
{

YangCase::YangCase ()
    : YangElement (getYangName (), "")
{
}

YangCase::~YangCase()
{
}

string YangCase::getYangName ()
{
    return ("case");
}

YangElement *YangCase::createInstance ()
{
    return (new YangCase ());
}

void YangCase::computeCliTargetNodeNameForSelf ()
{
    setCliTargetNodeName ("");
}

void YangCase::computeIsConfigurationForSelf ()
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

bool YangCase::isYangElementForData () const
{
    return true;
}

void YangCase::displayConfiguration2 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
{
    UI32 numberOfChildElementsForData = selectedChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfChildElementsForData; i++)
    {
        YangElement *pYangElement = selectedChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        YangLeaf *pYangLeaf = dynamic_cast<YangLeaf *> (pYangElement);

        if (NULL != pYangLeaf)
        {
            pYangLeaf->displayConfigurtionForLeaf (pYangDisplayConfigurationContext, NULL);

            continue;
        }

        pYangElement->displayConfiguration1 (pYangDisplayConfigurationContext);
    }
}

void YangCase::displayConfiguration3 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData)
{
    trace (TRACE_LEVEL_ERROR, "YangCase::displayConfiguration3 : ConfigurationSegment is empty.");

    debugPrint ();
}

bool YangCase::considerForHFileForCLanguage () const
{
    return (true);
}

}
