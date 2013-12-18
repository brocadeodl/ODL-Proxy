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

#include "ManagementInterface/WaveManagementClient.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/WaveManagementServerInformation.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Version/PrismVersion.h"

namespace WaveNs
{

static UI32       s_waveManagementInterfaceClientId       = 1;
static PrismMutex s_waveManagementInterfaceClientIdMutex;

WaveManagementClient::WaveManagementClient (const string &name, const string &ipAddress, const SI32 &port, const PrismServiceId &userInterfaceServiceId)
    : m_name                   (name),
      m_ipAddress              (ipAddress),
      m_port                   (port),
      m_userInterfaceServiceId (userInterfaceServiceId),
      m_pClientStreamingSocket (NULL)
{
    s_waveManagementInterfaceClientIdMutex.lock ();

    m_id = s_waveManagementInterfaceClientId;
    s_waveManagementInterfaceClientId++;

    s_waveManagementInterfaceClientIdMutex.unlock ();
}

WaveManagementClient::~WaveManagementClient ()
{
    indicateClosing ();
    if(NULL!=m_pClientStreamingSocket)
    {
       delete m_pClientStreamingSocket;
    }
}

ResourceId WaveManagementClient::connect (const string &serverIpAddressFromClientPerspective, const UI32 &numberOfRetries, const UI32 &maximumNumberOfSecondsToTryFor, const UI32 &serverUniqueIdentifierFromServerPerspective)
{
    UI32       numberOfTimesWeTriedToConnect = 0;
    UI32       numberOfSecondsElapsed        = 0;
    timeval    startTime;
    timeval    currentTime;
    ResourceId status                        = FRAMEWORK_ERROR;

    if (0 != (gettimeofday (&startTime, NULL)))
    {
        trace (TRACE_LEVEL_FATAL, string ("WaveManagementClient::connect : Could not obtain start time."));
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
                trace (TRACE_LEVEL_FATAL, string ("WaveManagementClient::connect : Could not allocate a ClientSocket for Management Interface Client : ") + m_ipAddress + m_port);
                prismAssert (false, __FILE__, __LINE__);
                break;
            }

            if (false == (m_pClientStreamingSocket->getIsConnected ()))
            {
                delete m_pClientStreamingSocket;
                m_pClientStreamingSocket = NULL;
                prismSleep (1);
            }
            else
            {
                // If we are able to connect post them the header.
                // the header currently contains The pass code for authorization, ipaddress and
                // the server port for this location so that the remote locations knows how to contact us back.

                bool isSuccessful = false;

                isSuccessful = (*m_pClientStreamingSocket) << FrameworkToolKit::getPrismConnectionPassPhrase ();

                if (true != isSuccessful)
                {
                    //prismAssert (false, __FILE__, __LINE__);
                    status = FRAMEWORK_ERROR;
                    break;
                }

                // now create the Wave Management Server Information object and post it to the Management Interface Client.

                WaveManagementServerInformation waveManagementServerInformation (serverIpAddressFromClientPerspective, FrameworkToolKit::getManagementInterfaceReceiverPort ());

                waveManagementServerInformation.setMessageVersion (PrismVersion::getVersionString());
                waveManagementServerInformation.setServerUniqueIdentifierFromServerPerspective (serverUniqueIdentifierFromServerPerspective);

                //isSuccessful = (*m_pClientStreamingSocket) << (&waveManagementServerInformation);
                string serializedData;
                waveManagementServerInformation.serialize2 (serializedData, SERIALIZE_WITH_ATTRIBUTE_NAME);
                UI32 size = serializedData.size();

                isSuccessful = (*m_pClientStreamingSocket) << (size);
                
                if (true == isSuccessful)
                {
                    isSuccessful = (*m_pClientStreamingSocket) << (serializedData); 
                }

                if (true != isSuccessful)
                {
                    //prismAssert (false, __FILE__, __LINE__);
                    status = FRAMEWORK_ERROR;
                    break;
                }

                status = FRAMEWORK_SUCCESS;
                break;
            }

            numberOfTimesWeTriedToConnect++;

            if (0 != (gettimeofday (&currentTime, NULL)))
            {
                trace (TRACE_LEVEL_FATAL, string ("WaveManagementClient::connect : Could not obtain current time."));
                prismAssert (false, __FILE__, __LINE__);
                status = FRAMEWORK_ERROR;
                break;
            }

            numberOfSecondsElapsed = (UI32) (currentTime.tv_sec - startTime.tv_sec);
        }
    }

    return (status);
}

void WaveManagementClient::indicateClosing ()
{
    if (NULL != m_pClientStreamingSocket)
    {
        (*m_pClientStreamingSocket) << 0xFFFFFFFF;
    }
}

UI32 WaveManagementClient::getId () const
{
    return (m_id);
}

string WaveManagementClient::getName () const
{
    return (m_name);
}

string WaveManagementClient::getIpAddress () const
{
    return (m_ipAddress);
}

SI32 WaveManagementClient::getPort () const
{
    return (m_port);
}

PrismServiceId WaveManagementClient::getUserInterfaceServiceId () const
{
    return (m_userInterfaceServiceId);
}

bool WaveManagementClient::operator << (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    prismAssert (NULL != m_pClientStreamingSocket,    __FILE__, __LINE__);
    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    pManagementInterfaceMessage->setServiceCode (getUserInterfaceServiceId ());

    return ((*m_pClientStreamingSocket) << pManagementInterfaceMessage);
}

void WaveManagementClient::updateMessageVersionAndSerializationTypeForSocket (const string& msgVersion)
{
    UI8 serializationType = ((0 == msgVersion.compare("0.0.0"))?SERIALIZE_WITH_ATTRIBUTE_ORDER : SERIALIZE_WITH_ATTRIBUTE_NAME); 

    if (NULL != m_pClientStreamingSocket)
    {
        m_pClientStreamingSocket->setToMessageVersionAndSerializationType (msgVersion, serializationType);
    }
}

}
