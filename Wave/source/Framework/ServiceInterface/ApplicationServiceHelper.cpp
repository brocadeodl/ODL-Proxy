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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ServiceInterface/ApplicationServiceHelper.h"

namespace WaveNs
{

PrismMutex                                               ApplicationServiceHelper::m_applicationServiceHelperMutex;
UI32                                                     ApplicationServiceHelper::m_nextApplicationServiceId                 = 1;
map<UI32, map<UI32, ApplicationServiceMessageHandler> *> ApplicationServiceHelper::m_allApplicationServiceMessageHandlersMap;

void ApplicationServiceHelper::lock ()
{
    m_applicationServiceHelperMutex.lock ();
}

void ApplicationServiceHelper::unlock ()
{
    m_applicationServiceHelperMutex.unlock ();
}

UI32 ApplicationServiceHelper::getNextApplicationServiceId ()
{
    UI32 nextApplicationServiceId = m_nextApplicationServiceId;

    m_nextApplicationServiceId++;

    return (nextApplicationServiceId);
}

void ApplicationServiceHelper::addMessageHandler (const UI32 &applicationServiceId, const UI32 &operationCode, ApplicationServiceMessageHandler handler)
{
    map<UI32, map<UI32, ApplicationServiceMessageHandler> *>::iterator element    = m_allApplicationServiceMessageHandlersMap.find (applicationServiceId);
    map<UI32, map<UI32, ApplicationServiceMessageHandler> *>::iterator endElement = m_allApplicationServiceMessageHandlersMap.end ();

    if (element == endElement)
    {
        m_allApplicationServiceMessageHandlersMap[applicationServiceId] = new map<UI32, ApplicationServiceMessageHandler>;
    }

    (*(m_allApplicationServiceMessageHandlersMap[applicationServiceId]))[operationCode] = handler;
}

map<UI32, ApplicationServiceMessageHandler> *ApplicationServiceHelper::getHandlersMapForApplicationService (const UI32 &applicationServiceId)
{
    map<UI32, map<UI32, ApplicationServiceMessageHandler> *>::iterator element    = m_allApplicationServiceMessageHandlersMap.find (applicationServiceId);
    map<UI32, map<UI32, ApplicationServiceMessageHandler> *>::iterator endElement = m_allApplicationServiceMessageHandlersMap.end ();

    if (element == endElement)
    {
        return (NULL);
    }
    else
    {
        return (m_allApplicationServiceMessageHandlersMap[applicationServiceId]);
    }
}

}
