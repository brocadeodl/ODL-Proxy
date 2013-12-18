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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "HttpInterface/WaveServerPageDirectoryEntry.h"
#include "HttpInterface/WaveServerPage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

WaveServerPageDirectoryEntry::WaveServerPageDirectoryEntry (const string &nodeName, WaveServerPage *pWaveServerPage)
    : m_nodeName        (nodeName),
      m_pWaveServerPage (pWaveServerPage)
{
}

WaveServerPageDirectoryEntry::~WaveServerPageDirectoryEntry ()
{
}

bool WaveServerPageDirectoryEntry::isAKnownImmediateSibling (const string &immediateSiblingName)
{
    map<string, WaveServerPageDirectoryEntry *>::const_iterator element    = m_siblingsMap.find (immediateSiblingName);
    map<string, WaveServerPageDirectoryEntry *>::const_iterator endElement = m_siblingsMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

WaveServerPageDirectoryEntry *WaveServerPageDirectoryEntry::getImmediateSibling (const string &immediateSiblingName)
{
    WaveServerPageDirectoryEntry *pWaveServerPageDirectoryEntry = NULL;

    if (true == (isAKnownImmediateSibling (immediateSiblingName)))
    {
        pWaveServerPageDirectoryEntry = m_siblingsMap[immediateSiblingName];
    }

    if (NULL == pWaveServerPageDirectoryEntry)
    {
        if (true == (isAKnownImmediateSibling ("*")))
        {
            pWaveServerPageDirectoryEntry = m_siblingsMap["*"];
        }
    }

    return (pWaveServerPageDirectoryEntry);
}

bool WaveServerPageDirectoryEntry::isAKnownSibling (const string &siblingName)
{
    vector<string>                siblingNameTokens;
    UI32                          numberOFTokensInSiblingName       = 0;
    UI32                          i                                 = 0;
    WaveServerPageDirectoryEntry *pTempWaveServerPageDirectoryEntry = this;

    StringUtils::tokenize (siblingName, siblingNameTokens, '/');

    numberOFTokensInSiblingName = siblingNameTokens.size ();

    for ( i = 0; i < numberOFTokensInSiblingName; i++)
    {
        pTempWaveServerPageDirectoryEntry = pTempWaveServerPageDirectoryEntry->getImmediateSibling (siblingNameTokens[i]);

        // If not found the exact match, check for wild card handler (*)

        if (NULL == pTempWaveServerPageDirectoryEntry)
        {
            pTempWaveServerPageDirectoryEntry = pTempWaveServerPageDirectoryEntry->getImmediateSibling ("*");
        }

        if (NULL == pTempWaveServerPageDirectoryEntry)
        {
            return (false);
        }
    }

    return (true);
}

WaveServerPageDirectoryEntry *WaveServerPageDirectoryEntry::getSibling (const string &siblingName)
{
    vector<string>                siblingNameTokens;
    UI32                          numberOFTokensInSiblingName       = 0;
    UI32                          i                                 = 0;
    WaveServerPageDirectoryEntry *pTempWaveServerPageDirectoryEntry = this;

    StringUtils::tokenize (siblingName, siblingNameTokens, '/');

    numberOFTokensInSiblingName = siblingNameTokens.size ();

    for ( i = 0; i < numberOFTokensInSiblingName; i++)
    {
        pTempWaveServerPageDirectoryEntry = pTempWaveServerPageDirectoryEntry->getImmediateSibling (siblingNameTokens[i]);

        if (NULL == pTempWaveServerPageDirectoryEntry)
        {
            return (NULL);
        }
    }

    return (pTempWaveServerPageDirectoryEntry);
}

void WaveServerPageDirectoryEntry::addImmediateSibling (const string &immediateSiblingName, WaveServerPage *pWaveServerPage)
{
    bool isImmeidateSiblingKnown = isAKnownImmediateSibling (immediateSiblingName);

    if (false == isImmeidateSiblingKnown)
    {
        WaveServerPageDirectoryEntry *pNewWaveServerPageDirectoryEntry = new WaveServerPageDirectoryEntry (immediateSiblingName, pWaveServerPage);

        prismAssert (NULL != pNewWaveServerPageDirectoryEntry, __FILE__, __LINE__);

        m_siblingsMap[immediateSiblingName] = pNewWaveServerPageDirectoryEntry;
    }
    else
    {
        if (NULL != pWaveServerPage)
        {
            WaveServerPageDirectoryEntry *pExistingWaveServerPageDirectoryEntry = m_siblingsMap[immediateSiblingName];
            WaveServerPage               *pExistingWaveServerPage               = pExistingWaveServerPageDirectoryEntry->getPWaveServerPage ();

            if (NULL == pExistingWaveServerPage)
            {
                pExistingWaveServerPageDirectoryEntry->setPWaveServerPage (pWaveServerPage);
            }
            else
            {
                trace (TRACE_LEVEL_FATAL, "WaveServerPageDirectoryEntry::addImmediateSibling : Adding a duplicate immediate Sibling : \'" + immediateSiblingName + "\'");
                prismAssert (false, __FILE__, __LINE__);
            }
        }
    }
}

void WaveServerPageDirectoryEntry::addSibling (const string &siblingName, WaveServerPage *pWaveServerPage)
{
    vector<string>                siblingNameTokens;
    UI32                          numberOFTokensInSiblingName       = 0;
    UI32                          i                                 = 0;
    WaveServerPageDirectoryEntry *pTempWaveServerPageDirectoryEntry = this;

    StringUtils::tokenize (siblingName, siblingNameTokens, '/');

    numberOFTokensInSiblingName = siblingNameTokens.size ();

    for ( i = 0; i < numberOFTokensInSiblingName; i++)
    {
        if ((numberOFTokensInSiblingName - 1) == i)
        {
            pTempWaveServerPageDirectoryEntry->addImmediateSibling (siblingNameTokens[i], pWaveServerPage);
        }
        else
        {
            pTempWaveServerPageDirectoryEntry->addImmediateSibling (siblingNameTokens[i], NULL);
        }

        pTempWaveServerPageDirectoryEntry = pTempWaveServerPageDirectoryEntry->getImmediateSibling (siblingNameTokens[i]);

        prismAssert (NULL != pTempWaveServerPageDirectoryEntry, __FILE__, __LINE__);
    }
}

void WaveServerPageDirectoryEntry::print (const string &prefix) const
{
    string                        siblingName;
    WaveServerPageDirectoryEntry *pWaveServerPageDirectoryEntry = NULL;
    string                        postfix;

    if (NULL != m_pWaveServerPage)
    {
        postfix = "(.)";
    }

    trace (TRACE_LEVEL_INFO, prefix + m_nodeName + postfix);

    map<string, WaveServerPageDirectoryEntry *>::const_iterator element    = m_siblingsMap.begin ();
    map<string, WaveServerPageDirectoryEntry *>::const_iterator endElement = m_siblingsMap.end   ();

    while (element != endElement)
    {
        pWaveServerPageDirectoryEntry = element->second;

        prismAssert (NULL != pWaveServerPageDirectoryEntry, __FILE__, __LINE__);

        pWaveServerPageDirectoryEntry->print (prefix + "|-");

        element++;
    }
}

WaveServerPage *WaveServerPageDirectoryEntry::getPWaveServerPage ()
{
    return (m_pWaveServerPage);
}

void WaveServerPageDirectoryEntry::setPWaveServerPage (WaveServerPage *pWaveServerPage)
{
    m_pWaveServerPage = pWaveServerPage;
}

WaveServerPage *WaveServerPageDirectoryEntry::getWaveServerPageForRelativePath (const string &path)
{
    WaveServerPageDirectoryEntry *pWaveServerPageDirectoryEntry = getSibling (path);

    if (NULL != pWaveServerPageDirectoryEntry)
    {
        return (pWaveServerPageDirectoryEntry->getPWaveServerPage ());
    }
    else
    {
        return (NULL);
    }
}

}
