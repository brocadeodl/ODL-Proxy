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

#include "ManagementInterface/ClientInterface/WaveManagementServer.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ClientInterface/WaveManagementClientInformation.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Version/PrismVersion.h" 
#include <stdio.h>

namespace WaveNs
{

static UI32       s_waveManagementInterfaceServerId       = 1;
static PrismMutex s_waveManagementInterfaceServerIdMutex;

WaveManagementServer::WaveManagementServer (const string &waveClientName, const string &waveClientNameWithoutExtension, const string &ipAddress, const SI32 &port)
    : m_waveClientName                 (waveClientName),
      m_waveClientNameWithoutExtension (waveClientNameWithoutExtension),
      m_ipAddress                      (ipAddress),
      m_port                           (port),
      m_pClientStreamingSocket         (NULL)
{
    s_waveManagementInterfaceServerIdMutex.lock ();

    m_id = s_waveManagementInterfaceServerId;
    s_waveManagementInterfaceServerId++;

    m_serverUniqueIdentifier = rand ();

    s_waveManagementInterfaceServerIdMutex.unlock ();
}

WaveManagementServer::~WaveManagementServer ()
{
    indicateClosing ();

    if (NULL != m_pClientStreamingSocket)
    {
        delete m_pClientStreamingSocket;
    
        m_pClientStreamingSocket = NULL;
    }
}

ResourceId WaveManagementServer::connect (const UI32 &numberOfRetries, const UI32 &maximumNumberOfSecondsToTryFor)
{
    UI32       numberOfTimesWeTriedToConnect = 0;
    UI32       numberOfSecondsElapsed        = 0;
    timeval    startTime;
    timeval    currentTime;
    ResourceId status                        = FRAMEWORK_ERROR;

    if (0 != (gettimeofday (&startTime, NULL)))
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveManagementServer::connect : Could not obtain start time."));
        prismAssert (false, __FILE__, __LINE__);
        status = FRAMEWORK_ERROR;
    }
    else
    {
        while (true)
        {
            if (!(((0 == numberOfRetries) || (numberOfTimesWeTriedToConnect < numberOfRetries)) && ((0 == maximumNumberOfSecondsToTryFor) || (numberOfSecondsElapsed <= maximumNumberOfSecondsToTryFor))))
            {
                break;
            }

            m_pClientStreamingSocket = new ClientStreamingSocket (m_ipAddress, m_port, 1);

            if (NULL == m_pClientStreamingSocket)
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveManagementServer::connect : Could not allocate a ClientSocket for Management Interface Client : ") + m_ipAddress + m_port);
                prismAssert (false, __FILE__, __LINE__);
                break;
            }

            if (false == (m_pClientStreamingSocket->getIsConnected ()))
            {
                delete m_pClientStreamingSocket;

                m_pClientStreamingSocket = NULL;

                tracePrintf (TRACE_LEVEL_WARN, false, true, ".");
                fflush (stdout);
                prismSleep (1);
            }
            else
            {
                bool isSuccessful = false;

                isSuccessful = m_pClientStreamingSocket->setDefaultKeepAliveTimeouts ();

                // Need a trace message to indicate if 'Set custom Keep Alive timeout settings' failed.

                if (false == isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, string ("WaveManagementServer::connect : \"setDefaultKeepAliveTimeouts ()\" failed. peerIpAddress [") + m_ipAddress + string ("]"));
                }

                // If we are able to connect post them the header.
                // the header currently contains The pass code for authorization, ipaddress and
                // the server port for this location so that the remote locations knows how to contact us back.

                isSuccessful = (*m_pClientStreamingSocket) << FrameworkToolKit::getPrismConnectionPassPhrase ();

                if (true != isSuccessful)
                {
                    prismAssert (false, __FILE__, __LINE__);
                }

                string messageVersion = PrismVersion::getVersionString ();

                // now create the Wave Management Server Information object and post it to the Management Interface Client.

                if ("127.0.0.1" == m_ipAddress)
                {
                    WaveManagementClientInformation waveManagementClientInformation (m_waveClientName, m_waveClientNameWithoutExtension, m_ipAddress, FrameworkToolKit::getManagementInterfaceClientReceiverPort (), WaveUserInterfaceObjectManager::getPrismServiceId (), messageVersion);
                    waveManagementClientInformation.setServerIpAddressFromClientPerspective (m_ipAddress);
                    waveManagementClientInformation.setServerUniqueIdentifier               (getServerUniqueIdentifier ());

                    //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementClientInformation);
                    string serializedData;
                    waveManagementClientInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                    UI32 size = serializedData.size(); 
                    isSuccessful = (*m_pClientStreamingSocket) << (size);

                    if (true == isSuccessful)
                    {
                        isSuccessful = (*m_pClientStreamingSocket) << (serializedData);
                    }
                }
                else
                {
                    WaveManagementClientInformation waveManagementClientInformation (m_waveClientName, m_waveClientNameWithoutExtension, FrameworkToolKit::getThisLocationIpAddress (), FrameworkToolKit::getManagementInterfaceClientReceiverPort (), WaveUserInterfaceObjectManager::getPrismServiceId (), messageVersion);

                    waveManagementClientInformation.setServerIpAddressFromClientPerspective (m_ipAddress);
                    waveManagementClientInformation.setServerUniqueIdentifier               (getServerUniqueIdentifier ());

                    //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementClientInformation);
                    string serializedData;
                    waveManagementClientInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                    UI32 size = serializedData.size();
                    isSuccessful = (*m_pClientStreamingSocket) << (size); 

                    if (true == isSuccessful)
                    {
                        isSuccessful = (*m_pClientStreamingSocket) << (serializedData);
                    }
                }

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, string ("WaveManagementServer::connect : Error while sending WaveManagementClientInformation."));

                    status = FRAMEWORK_ERROR;
                }
                else
                {
                status = FRAMEWORK_SUCCESS;
                }

                break;
            }

            numberOfTimesWeTriedToConnect++;

            if (0 != (gettimeofday (&currentTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveManagementServer::connect : Could not obtain current time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
                break;
            }

            numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
        }
    }

    //tracePrintf (TRACE_LEVEL_SUCCESS, false, true, "\n");

    return (status);
}

void WaveManagementServer::indicateClosing ()
{
    if (NULL != m_pClientStreamingSocket)
    {
        (*m_pClientStreamingSocket) << 0xFFFFFFFF;
    }
}

UI32 WaveManagementServer::getId () const
{
    return (m_id);
}

string WaveManagementServer::getIpAddress () const
{
    return (m_ipAddress);
}

SI32 WaveManagementServer::getPort () const
{
    return (m_port);
}

bool WaveManagementServer::operator << (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    prismAssert (NULL != m_pClientStreamingSocket, __FILE__, __LINE__);

    return ((*m_pClientStreamingSocket) << pManagementInterfaceMessage);
}

void WaveManagementServer::addServiceCache (const vector<string> &serviceNames, const vector<UI32> &serviceIds)
{
    UI32                        numberOfServiceNames = serviceNames.size ();
    UI32                        numberOfServiceIds   = serviceIds.size ();

    map<string, UI32>::iterator endForMapByname      = m_serviceMapByName.end ();
    map<UI32, string>::iterator endForMapById        = m_serviceMapById.end ();

    UI32                        i                    = 0;

    prismAssert (numberOfServiceNames == numberOfServiceIds, __FILE__, __LINE__);

    m_serviceNames = serviceNames;
    m_serviceIds   = serviceIds;

    trace (TRACE_LEVEL_DEBUG, string ("WaveManagementServer::addServiceCache : Number Of Service Names : ") + numberOfServiceNames);

    for (i = 0; i < numberOfServiceNames; i++)
    {
        prismAssert (endForMapByname == (m_serviceMapByName.find (serviceNames[i])), __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, string ("WaveManagementServer::addServiceCache : ") + serviceNames[i] + string (":") + serviceIds[i]);

        m_serviceMapByName[serviceNames[i]] = serviceIds[i];
    }

    trace (TRACE_LEVEL_DEBUG, string ("WaveManagementServer::addServiceCache : Number Of Service Ids : ") + numberOfServiceIds);

    for (i = 0; i < numberOfServiceIds; i++)
    {
        prismAssert (endForMapById == (m_serviceMapById.find (serviceIds[i])), __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, string ("WaveManagementServer::addServiceCache : ") + serviceIds[i] + string (":") + serviceNames[i]);

        m_serviceMapById[serviceIds[i]] = serviceNames[i];
    }
}

PrismServiceId WaveManagementServer::getPrismServiceIdForServiceName (const string &serviceName)
{
    map<string, UI32>::iterator element   = m_serviceMapByName.find (serviceName);
    map<string, UI32>::iterator end       = m_serviceMapByName.end ();
    PrismServiceId              serviceId = 0;

    if (element != end)
    {
        serviceId = element->second;
    }
    else
    {
        serviceId = 0;
    }

    return (serviceId);
}

string WaveManagementServer::getPrismServiceNameForServiceId (const PrismServiceId &serviceId)
{
    map<UI32, string>::iterator element     = m_serviceMapById.find (serviceId);
    map<UI32, string>::iterator end         = m_serviceMapById.end ();
    string                      serviceName = "";

    if (element != end)
    {
        serviceName = element->second;
    }
    else
    {
        serviceName = "";
    }

    return (serviceName);
}

UI32 WaveManagementServer::getServerUniqueIdentifier () const
{
    return (m_serverUniqueIdentifier);
}

void WaveManagementServer::updateMessageVersionAndSerializationTypeForSocket (const string& msgVersion) 
{
    UI8 serializationType = ((0 == msgVersion.compare("0.0.0"))?SERIALIZE_WITH_ATTRIBUTE_ORDER : SERIALIZE_WITH_ATTRIBUTE_NAME);
    
    if (NULL != m_pClientStreamingSocket)
    {
        m_pClientStreamingSocket->setToMessageVersionAndSerializationType (msgVersion, serializationType); 
    }
}

}
