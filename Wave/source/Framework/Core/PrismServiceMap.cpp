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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "Framework/Core/PrismServiceMap.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

PrismServiceMap::PrismServiceMap ()
{
}

PrismServiceMap::~PrismServiceMap ()
{
}

void PrismServiceMap::addServiceMap  (PrismServiceId id, PrismThread *pPrismThread, const string &serviceName)
{
    m_mutex.lock ();

    PrismThread    *pTempPrismThread   = m_servicesMap[id];
    PrismServiceId  tempPrismServiceId = m_servicesIdMap[serviceName];

    if (NULL != pTempPrismThread)
    {
        prismAssert (false, __FILE__, __LINE__);
        cerr << "Cannot proceed.  Trying to add a duplicate service id." << endl;
        exit (0);
    }

    if (0 != tempPrismServiceId)
    {
        prismAssert (false, __FILE__, __LINE__);
        cerr << "Cannot proceed.  Trying to add a duplicate service name." << endl;
        exit (0);
    }

    m_servicesMap[id]            = pPrismThread;
    m_servicesNameMap[id]        = serviceName;
    m_servicesIdMap[serviceName] = id;

    m_mutex.unlock ();
}

void PrismServiceMap::removeServiceMap (PrismServiceId id)
{
    map<PrismServiceId, PrismThread *>::iterator  element      = m_servicesMap.find (id);
    map<PrismServiceId, PrismThread *>::iterator  endElement   = m_servicesMap.end  ();

    PrismThread                                  *pPrismThread = NULL;

    if (endElement != element)
    {
        pPrismThread = element->second;

        prismAssert (NULL != pPrismThread, __FILE__, __LINE__);

        m_servicesMap.erase (element);
    }

    map<PrismServiceId, string>::iterator element1     = m_servicesNameMap.find (id);
    map<PrismServiceId, string>::iterator endElement1  = m_servicesNameMap.end  ();

    string                                serviceName;

    if (endElement1 != element1)
    {
        serviceName = element1->second;

        prismAssert (NULL != pPrismThread, __FILE__, __LINE__);

        m_servicesNameMap.erase (element1);
    }

    map<string, PrismServiceId>::iterator element2    = m_servicesIdMap.find (serviceName);
    map<string, PrismServiceId>::iterator endElement2 = m_servicesIdMap.end  ();

    if (endElement2 != element2)
    {
        prismAssert (id == element2->second, __FILE__, __LINE__);

        m_servicesIdMap.erase (element2);
    }

    m_mutex.unlock ();
}

PrismThread *PrismServiceMap::getPrismThreadForServiceId (PrismServiceId id)
{
    m_mutex.lock ();

    map<PrismServiceId, PrismThread *>::iterator  element    = m_servicesMap.find (id);
    map<PrismServiceId, PrismThread *>::iterator  end        = m_servicesMap.end ();
    PrismThread                                *pPrismThread = NULL;

    if (end != element)
    {
        pPrismThread = element->second;
    }

    m_mutex.unlock ();

    return (pPrismThread);
}

void PrismServiceMap::getListOfServiceIds (vector<PrismServiceId> &serviceIds)
{
    m_mutex.lock ();

    map<PrismServiceId, string>::iterator element      = m_servicesNameMap.begin ();
    map<PrismServiceId, string>::iterator end          = m_servicesNameMap.end ();

    serviceIds.clear ();

    while (element != end)
    {
        serviceIds.push_back (element->first);
        element++;
    }

    m_mutex.unlock ();

    return;
}

string PrismServiceMap::getPrismServiceNameForServiceId (const PrismServiceId &id)
{
    m_mutex.lock ();

    map<PrismServiceId, string>::iterator element      = m_servicesNameMap.find (id);
    map<PrismServiceId, string>::iterator end          = m_servicesNameMap.end ();
    string                                serviceName;

    if (end != element)
    {
        serviceName = element->second;
    }

    m_mutex.unlock ();

    return (serviceName);
}

PrismServiceId PrismServiceMap::getPrismServiceIdForServiceName (const string &serviceName)
{
    m_mutex.lock ();

    map<string, PrismServiceId>::iterator element   = m_servicesIdMap.find (serviceName);
    map<string, PrismServiceId>::iterator end       = m_servicesIdMap.end ();
    PrismServiceId                        serviceId = 0;

    if (end != element)
    {
        serviceId = element->second;
    }

    m_mutex.unlock ();

    return (serviceId);
}

WaveThreadStatus PrismServiceMap::joinAllThreads ()
{
    m_mutex.lock ();

    map<PrismServiceId, PrismThread *>::iterator  element         = m_servicesMap.begin ();
    map<PrismServiceId, PrismThread *>::iterator  limitingElement = m_servicesMap.end ();
    PrismThread                                *pPrismThread      = NULL;

    while (element != limitingElement)
    {
        pPrismThread = element->second;
        pPrismThread->joinCallingThread ();
        element++;
    }

    m_mutex.unlock ();

    return (WAVE_THREAD_SUCCESS);
}

}
