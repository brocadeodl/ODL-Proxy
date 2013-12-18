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

#include "Modeling/YANG/ObjectModel/YangAugment.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Modeling/YANG/ObjectModel/YangModule.h"
#include "Modeling/YANG/ObjectModel/YangWhen.h"

namespace WaveNs
{

YangAugment::YangAugment ()
    : YangElement  (getYangName (), "")
{
}

YangAugment::YangAugment (const string &targetNode)
    : YangElement  (getYangName (), ""),
      m_targetNode (targetNode)
{
}

YangAugment::~YangAugment ()
{
}

string YangAugment::getYangName ()
{
    return ("augment");
}

YangElement *YangAugment::createInstance ()
{
    return (new YangAugment ());
}

string YangAugment::getTargetNode () const
{
    return (m_targetNode);
}

void YangAugment::setTargetNode (const string &targetNode)
{
    m_targetNode = targetNode;
}

void YangAugment::processAttribute (const string &attributeName, const string &attributeValue)
{
    YangElement::processAttribute (attributeName, attributeValue);

    if ("target-node" == attributeName)
    {
        setTargetNode (attributeValue);
    }
}

void YangAugment::getTargetNodeDetails (string &targetNodeModuleName, string &targetNodePathInModule)
{
    vector<string> targetNodeLevel1Tokens;
    UI32           numberOfTargetNodeLevel1Tokens   = 0;
    UI32           i                                = 0;
    vector<string> targetNodeTokensForPathInModule;

    targetNodeModuleName = "";

    StringUtils::tokenize (m_targetNode, targetNodeLevel1Tokens, '/');

    numberOfTargetNodeLevel1Tokens = targetNodeLevel1Tokens.size ();

    prismAssert (1 <= numberOfTargetNodeLevel1Tokens, __FILE__, __LINE__);

    for (i = 0; i < numberOfTargetNodeLevel1Tokens; i++)
    {
        vector<string> targetNodeLevel2Tokens;
        UI32           numberOfTargetNodeLevel2Tokens   = 0;

        StringUtils::tokenize (targetNodeLevel1Tokens[i], targetNodeLevel2Tokens, ':');

        numberOfTargetNodeLevel2Tokens = targetNodeLevel2Tokens.size ();

        prismAssert (1 <= numberOfTargetNodeLevel2Tokens, __FILE__, __LINE__);
        prismAssert (2 >= numberOfTargetNodeLevel2Tokens, __FILE__, __LINE__);

        numberOfTargetNodeLevel2Tokens = targetNodeLevel2Tokens.size ();

        if (2 == numberOfTargetNodeLevel2Tokens)
        {
            targetNodeTokensForPathInModule.push_back (targetNodeLevel2Tokens[1]);

            if (0 == i)
            {
                targetNodeModuleName = targetNodeLevel2Tokens[0];

                YangModule *pOwnerYangModule = getPOwnerYangModule ();

                prismAssert (NULL != pOwnerYangModule, __FILE__, __LINE__);

                targetNodeModuleName = pOwnerYangModule->getImportedModuleName (targetNodeModuleName);
            }
        }
        else
        {
            targetNodeTokensForPathInModule.push_back (targetNodeLevel2Tokens[0]);

            if (0 == i)
            {
                targetNodeModuleName = (getPOwnerYangModule ())->getName ();
            }
        }
    }

    targetNodePathInModule = "";

    for (i = 0; i < numberOfTargetNodeLevel1Tokens; i++)
    {
        targetNodePathInModule += "/" + targetNodeTokensForPathInModule[i];
    }
}

void YangAugment::transferWhenConditionsToChildren ()
{
    // Find out if there are any child elements of the YangAugment which are either "when" or "display-when"
    vector<YangElement *> allWhenChildElements;


    getAllChildrenByYangName (YangWhen::getYangName (), allWhenChildElements);
    
    UI32 allWhenChildElementsSize = allWhenChildElements.size ();

    if (allWhenChildElementsSize > 1)
    {
        trace (TRACE_LEVEL_FATAL, "YangAugment::transferWhenConditionsToChildren : Cannot have more than 1 'When' or 'Display-When' condition in a Augment");
        prismAssert (false, __FILE__, __LINE__);
    }
    else if (allWhenChildElementsSize == 0)
    {
        return;
    }

    YangElement *pYangElement = NULL;
    
    if (allWhenChildElementsSize != 0)
    {
        pYangElement = allWhenChildElements[0];    
    }

    prismAssert (NULL != pYangElement, __FILE__, __LINE__);

    vector<YangElement *> &childElements = getChildElements ();

    UI32 numberOfChildElements = childElements.size (); 

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildElement = childElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        string childYangName = pChildElement->getYangName ();

        if (childYangName != YangWhen::getYangName ())
        {
            YangElement *pClonedChildElement = pYangElement->clone ();

            prismAssert (pClonedChildElement, __FILE__, __LINE__);

            pChildElement->addChildElement (pClonedChildElement);
        }
    }

    removeChildElement (pYangElement);

    delete (pYangElement);
}

}
