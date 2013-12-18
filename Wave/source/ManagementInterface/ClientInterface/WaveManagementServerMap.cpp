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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/WaveManagementServerMap.h"
#include "ManagementInterface/ClientInterface/WaveManagementServer.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ServerStreamingSocket.h"

namespace WaveNs
{

WaveManagementServerMap::WaveManagementServerMap ()
{
}

WaveManagementServerMap::~WaveManagementServerMap ()
{
}

bool WaveManagementServerMap::isAKnownServerNoLock (const string &ipAddress, const UI32 &port)
{
    string                                        uniqueString = FrameworkToolKit::getUniqueString (ipAddress, port);
    map<string, WaveManagementServer *>::iterator element      = m_managementServers.find (uniqueString);
    map<string, WaveManagementServer *>::iterator end          = m_managementServers.end ();
    bool                                          known        = false;

    if (element != end)
    {
        known = true;
    }
    else
    {
        known = false;
    }

    return (known);
}

bool WaveManagementServerMap::isAKnownServer (const string &ipAddress, const UI32 &port)
{
    m_managementServersMutex.lock ();

    bool known = isAKnownServerNoLock (ipAddress, port);

    m_managementServersMutex.unlock ();

    return (known);
}


bool WaveManagementServerMap::isAKnownServerNoLock (const UI32 &id)
{
    map<UI32, WaveManagementServer *>::iterator element      = m_managementServersById.find (id);
    map<UI32, WaveManagementServer *>::iterator end          = m_managementServersById.end ();
    bool                                        known        = false;

    if (element != end)
    {
        known = true;
    }
    else
    {
        known = false;
    }

    return (known);
}

bool WaveManagementServerMap::isAKnownServer (const UI32 &id)
{
    m_managementServersMutex.lock ();

    bool known = isAKnownServerNoLock (id);

    m_managementServersMutex.unlock ();

    return (known);
}

void WaveManagementServerMap::addServer (WaveManagementServer *pWaveManagementServer)
{
   m_managementServersMutex.lock ();

   string ipAddress           = pWaveManagementServer->getIpAddress ();
   UI32   port                = pWaveManagementServer->getPort      ();
   bool   serverAlreadyExists = isAKnownServerNoLock (ipAddress, port);
   string uniqueString        = FrameworkToolKit::getUniqueString (ipAddress, port);

   prismAssert (false == serverAlreadyExists, __FILE__, __LINE__);

   m_managementServers[uniqueString] = pWaveManagementServer;
   m_managementServersById[pWaveManagementServer->getId ()] = pWaveManagementServer;

   m_managementServersMutex.unlock ();
}

WaveManagementServer *WaveManagementServerMap::removeServer (const string &ipAddress, const UI32 &port)
{
    m_managementServersMutex.lock ();

    string                                         uniqueString          = FrameworkToolKit::getUniqueString (ipAddress, port);
    map<string, WaveManagementServer *>::iterator  element               = m_managementServers.find (uniqueString);
    map<string, WaveManagementServer *>::iterator  end                   = m_managementServers.end ();
    WaveManagementServer                          *pWaveManagementServer = NULL;

    if (element != end)
    {
        pWaveManagementServer = element->second;

        m_managementServersById.erase (pWaveManagementServer->getId ());
        m_managementServers.erase (element);
    }
    else
    {
        //prismAssert (false , __FILE__, __LINE__);
    }

    m_managementServersMutex.unlock ();

    return (pWaveManagementServer);
}

WaveManagementServer *WaveManagementServerMap::removeServer (const UI32 &id)
{
    m_managementServersMutex.lock ();

    map<UI32, WaveManagementServer *>::iterator  element               = m_managementServersById.find (id);
    map<UI32, WaveManagementServer *>::iterator  end                   = m_managementServersById.end ();
    WaveManagementServer                        *pWaveManagementServer = NULL;

    if (element != end)
    {
        pWaveManagementServer = element->second;

        string uniqueString = FrameworkToolKit::getUniqueString (pWaveManagementServer->getIpAddress (), pWaveManagementServer->getPort ());

        m_managementServersById.erase (id);
        m_managementServers.erase (uniqueString);
    }
    else
    {
        //prismAssert (false , __FILE__, __LINE__);
    }

    m_managementServersMutex.unlock ();

    return (pWaveManagementServer);
}

WaveManagementServer *WaveManagementServerMap::getServer (const string &ipAddress, const UI32 &port)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (ipAddress, port);
    string                uniqueString              = FrameworkToolKit::getUniqueString (ipAddress, port);
    WaveManagementServer *pTempWaveManagementServer = NULL;

    prismAssert (true == serverAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementServer = m_managementServers[uniqueString];

    m_managementServersMutex.unlock ();

    return (pTempWaveManagementServer);
}

UI32 WaveManagementServerMap::getServerId (const string &ipAddress, const UI32 &port)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (ipAddress, port);
    string                uniqueString              = FrameworkToolKit::getUniqueString (ipAddress, port);
    WaveManagementServer *pTempWaveManagementServer = NULL;
    UI32                  serverId                  = 0;

    prismAssert (true == serverAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementServer = m_managementServers[uniqueString];

    prismAssert (NULL != pTempWaveManagementServer, __FILE__, __LINE__);

    serverId = pTempWaveManagementServer->getId ();

    m_managementServersMutex.unlock ();

    return (serverId);
}

WaveManagementServer *WaveManagementServerMap::getServer (const UI32 &id)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (id);
    WaveManagementServer *pTempWaveManagementServer = NULL;

    prismAssert (true == serverAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementServer = m_managementServersById[id];

    m_managementServersMutex.unlock ();

    return (pTempWaveManagementServer);
}

PrismServiceId WaveManagementServerMap::getPrismServiceIdForServiceName (const UI32 &serverId, const string &serviceName)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (serverId);
    WaveManagementServer *pTempWaveManagementServer = NULL;
    PrismServiceId        serviceId                 = 0;

    if (true == serverAlreadyExists)
    {
        pTempWaveManagementServer = m_managementServersById[serverId];

        prismAssert (NULL != pTempWaveManagementServer, __FILE__, __LINE__);

        serviceId = pTempWaveManagementServer->getPrismServiceIdForServiceName (serviceName);
    }

    m_managementServersMutex.unlock ();

    return (serviceId);
}

string WaveManagementServerMap::getPrismServiceNameForServiceId (const UI32 &serverId, const PrismServiceId &serviceId)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (serverId);
    WaveManagementServer *pTempWaveManagementServer = NULL;
    string                serviceName;

    if (true == serverAlreadyExists)
    {
        pTempWaveManagementServer = m_managementServersById[serverId];

        prismAssert (NULL != pTempWaveManagementServer, __FILE__, __LINE__);

        serviceName = pTempWaveManagementServer->getPrismServiceNameForServiceId (serviceId);
    }

    m_managementServersMutex.unlock ();

    return (serviceName);
}

void WaveManagementServerMap::addServiceCache (const UI32 &serverId, const vector<string> &serviceNames, const vector<UI32> &serviceIds)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (serverId);
    WaveManagementServer *pTempWaveManagementServer = NULL;

    prismAssert (true == serverAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementServer = m_managementServersById[serverId];

    prismAssert (NULL != pTempWaveManagementServer, __FILE__, __LINE__);

    pTempWaveManagementServer->addServiceCache (serviceNames, serviceIds);

    m_managementServersMutex.unlock ();

}

ResourceId WaveManagementServerMap::post (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    m_managementServersMutex.lock ();

    WaveManagementServer                        *pTempWaveManagementServer = NULL;
    ResourceId                                   status                    = WAVE_MESSAGE_SUCCESS;
    bool                                         postingStatus             = false;
    const UI32                                   id                        = pManagementInterfaceMessage->getServerId ();
    map<UI32, WaveManagementServer *>::iterator  element                   = m_managementServersById.find (id);
    map<UI32, WaveManagementServer *>::iterator  end                       = m_managementServersById.end ();
    bool                                         known                     = false;

    if (element != end)
    {
        known = true;
    }
    else
    {
        known = false;
    }

    prismAssert (true == known, __FILE__, __LINE__);

    pTempWaveManagementServer = m_managementServersById[id];

    if (NULL != pTempWaveManagementServer)
    {
        postingStatus = (*pTempWaveManagementServer) << pManagementInterfaceMessage;

        if (true == postingStatus)
        {
            status = WAVE_MGMT_CLIENT_INTF_POST_TO_SERVER_SUCCEEDED;
        }
        else
        {
            status = WAVE_MGMT_CLIENT_INTF_POST_TO_SERVER_FAILED;
        }
    }
    else
    {
        status = WAVE_MGMT_CLIENT_INTF_UNKNOWN_SERVER;
    }

    m_managementServersMutex.unlock ();

    return (status);
}

UI32 WaveManagementServerMap::getServerIdIfKnown (const string &ipAddress, const UI32 &port)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists       = isAKnownServerNoLock (ipAddress, port);
    string                uniqueString              = FrameworkToolKit::getUniqueString (ipAddress, port);
    WaveManagementServer *pTempWaveManagementServer = NULL;
    UI32                  serverId                  = 0;

    if (true == serverAlreadyExists)
    {
        pTempWaveManagementServer = m_managementServers[uniqueString];

        prismAssert (NULL != pTempWaveManagementServer, __FILE__, __LINE__);

        serverId = pTempWaveManagementServer->getId ();
    }

    m_managementServersMutex.unlock ();

    return (serverId);
}

UI32 WaveManagementServerMap::getServerIdIfKnownForUniqueServerIdentifier (const string &ipAddress, const SI32 &port, const UI32 &uniqueServerIdentifierFromServerPerspective)
{
    m_managementServersMutex.lock ();

    bool                  serverAlreadyExists                           = isAKnownServerNoLock (ipAddress, port);
    string                uniqueString                                  = FrameworkToolKit::getUniqueString (ipAddress, port);
    WaveManagementServer *pTempWaveManagementServer                     = NULL;
    UI32                  serverId                                      = 0;
    UI32                  uniqueServerIdentifierFromClientPerspective   = 0;

    if (true == serverAlreadyExists)
    {
        pTempWaveManagementServer = m_managementServers[uniqueString];

        prismAssert (NULL != pTempWaveManagementServer, __FILE__, __LINE__);

        uniqueServerIdentifierFromClientPerspective = pTempWaveManagementServer->getServerUniqueIdentifier ();

        if (uniqueServerIdentifierFromClientPerspective == uniqueServerIdentifierFromServerPerspective)
        {
            serverId = pTempWaveManagementServer->getId ();
        }
    }

    m_managementServersMutex.unlock ();

    return (serverId);
}

void WaveManagementServerMap::updateMessageVersionAndSerializationTypeForServer (const UI32 &serverId, const string &version)
{
    WaveManagementServer *pTempWaveManagementServer = getServer (serverId);

    if (NULL != pTempWaveManagementServer) 
    {
        pTempWaveManagementServer->updateMessageVersionAndSerializationTypeForSocket (version);
    }
}

}
