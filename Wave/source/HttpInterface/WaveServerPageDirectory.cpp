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

#include "HttpInterface/WaveServerPageDirectory.h"
#include <Framework/Utils/AssertUtils.h>
#include "Framework/Utils/TraceUtils.h"
#include <Framework/Core/Wave.h>
#include "HttpInterface/WaveServerPage.h"
#include "HttpInterface/WaveServerPageDirectoryEntry.h"

namespace WaveNs
{

map<string, WaveServerPage *> WaveServerPageDirectory::m_serverPageMap;
WaveServerPageDirectoryEntry  WaveServerPageDirectory::m_waveServerPageDirectoryRoot ("/", NULL);

WaveServerPageDirectory::WaveServerPageDirectory ()
{
}

WaveServerPageDirectory::~WaveServerPageDirectory ()
{
}

bool WaveServerPageDirectory::isAKnownServerPage (const string& path)
{
    map<string, WaveServerPage *>::const_iterator element    = m_serverPageMap.find (path);
    map<string, WaveServerPage *>::const_iterator endElement = m_serverPageMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void WaveServerPageDirectory::registerServerPage (WaveServerPage *pWaveServerPage)
{
    prismAssert (NULL != pWaveServerPage, __FILE__, __LINE__);

    string path             = pWaveServerPage->getPath ();
    bool   isAnExistingPage = isAKnownServerPage (path);

    m_waveServerPageDirectoryRoot.addSibling (path, pWaveServerPage);

    if (true == isAnExistingPage)
    {
        trace (TRACE_LEVEL_FATAL, "WaveServerPageDirectory::registerServerPage : A Page with the path \"" + path + "\" has already been registered.");

        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        if (NULL != pWaveServerPage)
        {
            trace (TRACE_LEVEL_DEVEL, "WaveServerPageDirectory::registerServerPage : Registering a Wave Server Page : \"" + path + "\"");

            m_serverPageMap[path] = pWaveServerPage;
        }
    }
}

void WaveServerPageDirectory::registerServerPage (const string &path, WaveServerPage *pWaveServerPage)
{
    bool isAnExistingPage = isAKnownServerPage (path);

    m_waveServerPageDirectoryRoot.addSibling (path, pWaveServerPage);

    if (true == isAnExistingPage)
    {
        trace (TRACE_LEVEL_FATAL, "WaveServerPageDirectory::registerServerPage : A Page with the path \"" + path + "\" has already been registered.");

        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        if (NULL != pWaveServerPage)
        {
            trace (TRACE_LEVEL_DEVEL, "WaveServerPageDirectory::registerServerPage : Registering a Wave Server Page : \"" + path + "\"");

            m_serverPageMap[path] = pWaveServerPage;
        }
    }
}

WaveServerPage *WaveServerPageDirectory::getWaveServerPage (const string &path)
{
    return (m_waveServerPageDirectoryRoot.getWaveServerPageForRelativePath (path));
}

ResourceId WaveServerPageDirectory::print (UI32 argc, vector<string> argv)
{
    m_waveServerPageDirectoryRoot.print ();

    return (SHELL_OK);
}

}
