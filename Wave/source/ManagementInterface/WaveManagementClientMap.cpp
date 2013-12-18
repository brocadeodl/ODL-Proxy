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

#include "ManagementInterface/WaveManagementClientMap.h"
#include "ManagementInterface/WaveManagementClient.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/ClientStreamingSocket.h"

namespace WaveNs
{

WaveManagementClientMap::WaveManagementClientMap ()
{
}

WaveManagementClientMap::~WaveManagementClientMap ()
{
}

bool WaveManagementClientMap::isAKnownClient (const string &name)
{
    m_managementClientsMutex.lock ();

    map<string, WaveManagementClient *>::iterator element = m_managementClientsByName.find (name);
    map<string, WaveManagementClient *>::iterator end     = m_managementClientsByName.end  ();
    bool                                          known   = false;

    if (element != end)
    {
        known = true;
    }
    else
    {
        known = false;
    }

    m_managementClientsMutex.unlock ();

    return (known);
}

bool WaveManagementClientMap::isAKnownClient (const string &ipAddress, const UI32 &port)
{
    m_managementClientsMutex.lock ();

    string                                        uniqueString = FrameworkToolKit::getUniqueString (ipAddress, port);
    map<string, WaveManagementClient *>::iterator element      = m_managementClients.find (uniqueString);
    map<string, WaveManagementClient *>::iterator end          = m_managementClients.end ();
    bool                                          known        = false;

    if (element != end)
    {
        known = true;
    }
    else
    {
        known = false;
    }

    m_managementClientsMutex.unlock ();

    return (known);
}

bool WaveManagementClientMap::isAKnownClient (const UI32 &id)
{
    m_managementClientsMutex.lock ();

    map<UI32, WaveManagementClient *>::iterator element      = m_managementClientsById.find (id);
    map<UI32, WaveManagementClient *>::iterator end          = m_managementClientsById.end ();
    bool                                        known        = false;

    if (element != end)
    {
        known = true;
    }
    else
    {
        known = false;
    }

    m_managementClientsMutex.unlock ();

    return (known);
}

void WaveManagementClientMap::addClient (WaveManagementClient *pWaveManagementClient)
{
    prismAssert (NULL != pWaveManagementClient, __FILE__, __LINE__);

    string name                      = pWaveManagementClient->getName      ();
    string ipAddress                 = pWaveManagementClient->getIpAddress ();
    UI32   port                      = pWaveManagementClient->getPort      ();
    bool   clientAlreadyExists       = isAKnownClient (ipAddress, port);
    bool   clientAlreadyExistsByName = isAKnownClient (name);
    string uniqueString              = FrameworkToolKit::getUniqueString   (ipAddress, port);

    m_managementClientsMutex.lock ();

    prismAssert (false == clientAlreadyExists,       __FILE__, __LINE__);
    prismAssert (false == clientAlreadyExistsByName, __FILE__, __LINE__);

    m_managementClientsByName[name]                          = pWaveManagementClient;
    m_managementClients[uniqueString]                        = pWaveManagementClient;
    m_managementClientsById[pWaveManagementClient->getId ()] = pWaveManagementClient;

    //trace (TRACE_LEVEL_DEBUG, "WaveManagementClientMap::addClient : Adding a Client with name : \'" + name + "\'");

    m_managementClientsMutex.unlock ();
}

WaveManagementClient *WaveManagementClientMap::removeClient (const string &ipAddress, const UI32 &port)
{
    m_managementClientsMutex.lock ();

    string                                         uniqueString          = FrameworkToolKit::getUniqueString (ipAddress, port);
    map<string, WaveManagementClient *>::iterator  element               = m_managementClients.find (uniqueString);
    map<string, WaveManagementClient *>::iterator  end                   = m_managementClients.end ();
    WaveManagementClient                          *pWaveManagementClient = NULL;

    if (element != end)
    {
        pWaveManagementClient = element->second;

        m_managementClientsByName.erase     (pWaveManagementClient->getName ());
        m_managementClientsById.erase       (pWaveManagementClient->getId ());
        m_clientMessageVersionById.erase    (pWaveManagementClient->getId ());
        m_managementClients.erase (element);
    }
    else
    {
        //prismAssert (false , __FILE__, __LINE__);
    }

    m_managementClientsMutex.unlock ();

    return (pWaveManagementClient);
}

vector<WaveManagementClient *> *WaveManagementClientMap::removeAllClient (const string &ipAddress)
{
    m_managementClientsMutex.lock ();

    map<string, WaveManagementClient *>::iterator  element               = m_managementClients.begin ();
    map<string, WaveManagementClient *>::iterator  end                   = m_managementClients.end ();
    WaveManagementClient  *pWaveManagementClient = NULL;
    vector<WaveManagementClient *> *pResults = new vector<WaveManagementClient *>;

    while (element != end)
    {
        pWaveManagementClient = element->second;
        if (pWaveManagementClient == NULL) {
	    trace (TRACE_LEVEL_ERROR, string("WaveManagementClientMap::removeAllClient:empty element entry "));

	} else if ((pWaveManagementClient->getIpAddress ()) == ipAddress) {

	    pResults->push_back (pWaveManagementClient);
	}
	element++;
    }

    m_managementClientsMutex.unlock ();

    return (pResults);
}

WaveManagementClient *WaveManagementClientMap::removeClient (const UI32 &id)
{
    m_managementClientsMutex.lock ();

    map<UI32, WaveManagementClient *>::iterator  element               = m_managementClientsById.find (id);
    map<UI32, WaveManagementClient *>::iterator  end                   = m_managementClientsById.end ();
    WaveManagementClient                        *pWaveManagementClient = NULL;

    if (element != end)
    {
        pWaveManagementClient = element->second;

        string uniqueString = FrameworkToolKit::getUniqueString (pWaveManagementClient->getIpAddress (), pWaveManagementClient->getPort ());

        m_managementClientsById.erase (id);
        m_managementClientsByName.erase (pWaveManagementClient->getName ());
        m_managementClients.erase (uniqueString);

        m_clientMessageVersionById.erase (id); 
    }
    else
    {
        //prismAssert (false , __FILE__, __LINE__);
    }

    m_managementClientsMutex.unlock ();

    return (pWaveManagementClient);
}

WaveManagementClient *WaveManagementClientMap::getClient (const string &ipAddress, const UI32 &port)
{
    bool                  clientAlreadyExists       = isAKnownClient (ipAddress, port);
    string                uniqueString              = FrameworkToolKit::getUniqueString (ipAddress, port);
    WaveManagementClient *pTempWaveManagementClient = NULL;

    m_managementClientsMutex.lock ();

    prismAssert (true == clientAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementClient = m_managementClients[uniqueString];

    m_managementClientsMutex.unlock ();

    return (pTempWaveManagementClient);
}

UI32 WaveManagementClientMap::getClientId (const string &ipAddress, const UI32 &port)
{
    bool                  clientAlreadyExists       = isAKnownClient (ipAddress, port);
    string                uniqueString              = FrameworkToolKit::getUniqueString (ipAddress, port);
    WaveManagementClient *pTempWaveManagementClient = NULL;
    UI32                  clientId                  = 0;

    m_managementClientsMutex.lock ();

    prismAssert (true == clientAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementClient = m_managementClients[uniqueString];

    prismAssert (NULL != pTempWaveManagementClient, __FILE__, __LINE__);

    clientId = pTempWaveManagementClient->getId ();

    m_managementClientsMutex.unlock ();

    return (clientId);
}

UI32 WaveManagementClientMap::getClientId (const string &name)
{
    bool                  clientAlreadyExists       = isAKnownClient (name);
    WaveManagementClient *pTempWaveManagementClient = NULL;
    UI32                  clientId                  = 0;

    m_managementClientsMutex.lock ();

    prismAssert (true == clientAlreadyExists, __FILE__, __LINE__);

    prismAssert (NULL != pTempWaveManagementClient, __FILE__, __LINE__);

    clientId = pTempWaveManagementClient->getId ();

    m_managementClientsMutex.unlock ();

    return (clientId);
}

WaveManagementClient *WaveManagementClientMap::getClient (const UI32 &id)
{
    bool                  clientAlreadyExists       = isAKnownClient (id);
    WaveManagementClient *pTempWaveManagementClient = NULL;

    m_managementClientsMutex.lock ();

    prismAssert (true == clientAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementClient = m_managementClientsById[id];

    m_managementClientsMutex.unlock ();

    return (pTempWaveManagementClient);
}

WaveManagementClient *WaveManagementClientMap::getClient (const string &name)
{
    bool                  clientAlreadyExists       = isAKnownClient (name);
    WaveManagementClient *pTempWaveManagementClient = NULL;

    m_managementClientsMutex.lock ();

    prismAssert (true == clientAlreadyExists, __FILE__, __LINE__);

    pTempWaveManagementClient = m_managementClientsByName[name];

    m_managementClientsMutex.unlock ();

    return (pTempWaveManagementClient);
}

ResourceId WaveManagementClientMap::post (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    m_managementClientsMutex.lock ();

    WaveManagementClient *pTempWaveManagementClient = NULL;
    ResourceId            status                    = WAVE_MESSAGE_SUCCESS;
    bool                  postingStatus             = false;
    const UI32            id                        = pManagementInterfaceMessage->getClientId ();
    const string          name                      = pManagementInterfaceMessage->getClientName ();
    bool                  allClients                = pManagementInterfaceMessage->getAllWaveClients();  

    if (0 != id)
    {
        //trace (TRACE_LEVEL_DEBUG, string ("WaveManagementClientMap::post : Sending a message to Client with id : ") + id);

        map<UI32, WaveManagementClient *>::iterator  element = m_managementClientsById.find (id);
        map<UI32, WaveManagementClient *>::iterator  end     = m_managementClientsById.end ();

        if (end != element)
        {
            pTempWaveManagementClient = m_managementClientsById[id];
        }
    }
    else if ("" != name)
    {
        //trace (TRACE_LEVEL_DEBUG, string ("WaveManagementClientMap::post : Sending a message to Client with name : \'") + name + "\'");

        map<string, WaveManagementClient *>::iterator  element = m_managementClientsByName.find (name);
        map<string, WaveManagementClient *>::iterator  end     = m_managementClientsByName.end  ();

        if (end != element)
        {
            pTempWaveManagementClient = m_managementClientsByName[name];
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "WaveManagementClientMap::post : Could not find the client with name \"" + name + "\"");
        }

    }
    else if ( true == allClients )
    {
        //Now the message will be sent to all clients
        trace (TRACE_LEVEL_DEBUG, string ("WaveManagementClientMap::post : Sending message to all clients"));

    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagementClientMap::post : Client must be identified when posting a message to a client.  Either the client id or client name must be set.");
        prismAssert (false, __FILE__, __LINE__);
    }

    if (NULL != pTempWaveManagementClient)
    {
        pManagementInterfaceMessage->setClientId (pTempWaveManagementClient->getId ());

        postingStatus = (*pTempWaveManagementClient) << pManagementInterfaceMessage;

        if (true == postingStatus)
        {
            status = WAVE_MGMT_INTF_POST_TO_CLIENT_SUCCEEDED;
        }
        else
        {
            status = WAVE_MGMT_INTF_POST_TO_CLIENT_FAILED;
        }
    }
    else if (true == allClients)
    {
        // Send this to all clients by looping through the Map
        map<string, WaveManagementClient *>::iterator  element = m_managementClientsByName.begin ();
        map<string, WaveManagementClient *>::iterator  end     = m_managementClientsByName.end  ();
        status = WAVE_MGMT_INTF_POST_TO_CLIENT_SUCCEEDED;

        while (element != end)
        {
            pTempWaveManagementClient = element->second;
            pManagementInterfaceMessage->setClientId (pTempWaveManagementClient->getId ());
            trace (TRACE_LEVEL_DEBUG, string ("WaveManagementClientMap::post : Sending message to ") + element->first);

            postingStatus = (*pTempWaveManagementClient) << pManagementInterfaceMessage;
            if ( false == postingStatus )
            {
                status = WAVE_MGMT_INTF_POST_TO_CLIENT_FAILED;
                trace (TRACE_LEVEL_ERROR, string ("WaveManagementClientMap::post : allClients is true, Failed to send message to client ") + element->first);
            }
            element++;    
        }
    }
    else
    {
        status = WAVE_MGMT_INTF_UNKNOWN_CLIENT;
    }

    m_managementClientsMutex.unlock ();

    return (status);
}


vector<string> WaveManagementClientMap::getClientsConnected () const
{
    vector<string> clientsConnected;
    map<string, WaveManagementClient *>::const_iterator it = m_managementClientsByName.begin();

    while (it != m_managementClientsByName.end())
    {
        clientsConnected.push_back(it->first);
        
        it++;
    }

    return (clientsConnected);
}

}
