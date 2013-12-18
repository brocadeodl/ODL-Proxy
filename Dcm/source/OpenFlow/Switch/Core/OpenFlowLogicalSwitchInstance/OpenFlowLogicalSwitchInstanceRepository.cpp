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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchInstance/OpenFlowLogicalSwitchInstanceRepository.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchInstance/OpenFlowLogicalSwitchInstanceLocalObjectManager.h"

namespace OpenFlowNs
{

map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *> OpenFlowLogicalSwitchInstanceRepository::m_openFlowLogicalSwitchInstanceLocalObjectManagersMap;
PrismMutex                                                     OpenFlowLogicalSwitchInstanceRepository::m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex;

bool OpenFlowLogicalSwitchInstanceRepository::isAKnownOpenFlowLogicalSwitch (const string &openFlowLogicalSwitchName)
{
    m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex.lock ();

    map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *>::const_iterator element    = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap.find (openFlowLogicalSwitchName);
    map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *>::const_iterator endElement = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap.end  ();
    bool                                                                           isKnown    = false;

    if (element != endElement)
    {
        isKnown = true;
    }
    else
    {
        isKnown = false;
    }

    m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex.unlock ();

    return (isKnown);
}

void OpenFlowLogicalSwitchInstanceRepository::addOpenFlowLogicalSwitch (const string &openFlowLogicalSwitchName, OpenFlowLogicalSwitchInstanceLocalObjectManager *pOpenFlowLogicalSwitchInstanceLocalObjectManager)
{
    m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex.lock ();

    map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *>::const_iterator element    = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap.find (openFlowLogicalSwitchName);
    map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *>::const_iterator endElement = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap.end  ();
    bool                                                                           isKnown    = false;

    if (element != endElement)
    {
        prismAssert (false == isKnown, __FILE__, __LINE__);
    }
    else
    {
        m_openFlowLogicalSwitchInstanceLocalObjectManagersMap[openFlowLogicalSwitchName] = pOpenFlowLogicalSwitchInstanceLocalObjectManager;
    }

    m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex.unlock ();
}

OpenFlowLogicalSwitchInstanceLocalObjectManager *OpenFlowLogicalSwitchInstanceRepository::getOpenFlowLogicalSwitchInstanceLocalObjectManager (const string &openFlowLogicalSwitchName)
{
    m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex.lock ();

    map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *>::const_iterator  element                                          = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap.find (openFlowLogicalSwitchName);
    map<string, OpenFlowLogicalSwitchInstanceLocalObjectManager *>::const_iterator  endElement                                       = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap.end  ();
    OpenFlowLogicalSwitchInstanceLocalObjectManager                                *pOpenFlowLogicalSwitchInstanceLocalObjectManager = NULL;

    if (element != endElement)
    {
        pOpenFlowLogicalSwitchInstanceLocalObjectManager = m_openFlowLogicalSwitchInstanceLocalObjectManagersMap[openFlowLogicalSwitchName];
    }

    m_openFlowLogicalSwitchInstanceLocalObjectManagersMapMutex.unlock ();

    return (pOpenFlowLogicalSwitchInstanceLocalObjectManager);
}

}
