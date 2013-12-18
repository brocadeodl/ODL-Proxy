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

#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverThread.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/ShellDebug.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include <map>


namespace WaveNs
{

map<string, WaveClientReceiverThread *> WaveClientReceiverObjectManager::s_waveClientReceiverThreadCache;
PrismMutex                              WaveClientReceiverObjectManager::s_waveClientReceiverThreadCacheMutex;
bool                                    WaveClientReceiverObjectManager::m_autoScanForAvailablePort            = false;
UI32                                    WaveClientReceiverObjectManager::m_autoScanLimit                       = 200;

bool WaveClientReceiverObjectManager::addWaveClientReceiverThreadToCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, WaveClientReceiverThread *&pWaveClientReceiverThread, const string& remoteLocationMessageVersion)
{
    s_waveClientReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    if (element == end)
    {
        s_waveClientReceiverThreadCache[remoteLocationUniqueId] = pWaveClientReceiverThread;
    
        (WaveClientTransportObjectManager::getInstance ())->updateMessageVersionAndSerializationTypeForServer (remoteLocationIpAddress, remoteLocationPort, remoteLocationMessageVersion);

        s_waveClientReceiverThreadCacheMutex.unlock ();
        return (true);
    }
    else
    {
        s_waveClientReceiverThreadCacheMutex.unlock ();;
        return (false);
    }
}

string WaveClientReceiverObjectManager::getMessageVersionForWaveClient (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort); 
    string messageVersion         = "";

    s_waveClientReceiverThreadCacheMutex.lock ();

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    if (element != end)
    {   
        WaveClientReceiverThread* pClientReceiverThread = element->second;
        messageVersion = pClientReceiverThread->getPeerServerMessageVersion ();
    }

    s_waveClientReceiverThreadCacheMutex.unlock ();

    return (messageVersion);
}

UI8 WaveClientReceiverObjectManager::getSerializationTypeForWaveClient (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);
    UI8    serializationType      = SERIALIZE_WITH_UNKNOWN;

    s_waveClientReceiverThreadCacheMutex.lock ();

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    if (element != end)
    {   
        WaveClientReceiverThread* pClientReceiverThread = element->second;
        serializationType = pClientReceiverThread->getPeerServerSerializationType ();
    }

    s_waveClientReceiverThreadCacheMutex.unlock ();

    return (serializationType);
}

void WaveClientReceiverObjectManager::removeWaveClientReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_waveClientReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    if (element != end)
    {
        s_waveClientReceiverThreadCache.erase (element);
    }

    s_waveClientReceiverThreadCacheMutex.unlock ();
}

void WaveClientReceiverObjectManager::terminateAndRemoveWaveClientReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_waveClientReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    if (element != end)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "WaveClientReceiverObjectManager::terminateAndRemoveWaveClientReceiverThreadFromCache : Forcefully Terminating a thread that is monitoring messages from " + remoteLocationUniqueId);

#if 0
        int status = pthread_cancel ((element->second)->getId ());

        if (0 != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("WaveClientReceiverObjectManager::terminateAndRemoveWaveClientReceiverThreadFromCache : Could not terminate the thread : Status = ") + status);
        }
#endif

        // Reset the Peer Server details so that when the thread really exits, it will not try to remove the details from cache.

        (element->second)->setPeerServerDetails ("", 0);

        // Close and reset the Peer Server Streaming Socket.

#if 0
        (element->second)->closePeerServerStreamingSocket ();
#endif

        s_waveClientReceiverThreadCache.erase (element);
    }

    s_waveClientReceiverThreadCacheMutex.unlock ();
}

string WaveClientReceiverObjectManager::getRemoteLocationUniqueIdentifier (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    return (remoteLocationIpAddress + ":" + remoteLocationPort);
}

bool WaveClientReceiverObjectManager::isACurrentlyConnectedLocation (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_waveClientReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    s_waveClientReceiverThreadCacheMutex.unlock ();

    if (element != end)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void WaveClientReceiverObjectManager::debugPrint (UI32 argc, vector<string> argv)
{
    s_waveClientReceiverThreadCacheMutex.lock ();

    map<string, WaveClientReceiverThread *>::iterator element = s_waveClientReceiverThreadCache.begin ();
    map<string, WaveClientReceiverThread *>::iterator end     = s_waveClientReceiverThreadCache.end ();

    while (end != element)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, element->first);

        element++;
    }

    s_waveClientReceiverThreadCacheMutex.unlock ();
}

void WaveClientReceiverObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&WaveClientReceiverObjectManager::debugPrint), "printwcrcache");
}

WaveClientReceiverObjectManager::WaveClientReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks      ("Wave Client Receiver"),
      m_pServerSocketForWaveManagementInterfaceServers (NULL)
{
    setTraceLevel (TRACE_LEVEL_ERROR);

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
}

WaveClientReceiverObjectManager::~WaveClientReceiverObjectManager ()
{
    if (NULL != m_pServerSocketForWaveManagementInterfaceServers)
    {
        delete m_pServerSocketForWaveManagementInterfaceServers;
        m_pServerSocketForWaveManagementInterfaceServers = NULL;
    }
}

WaveClientReceiverObjectManager *WaveClientReceiverObjectManager:: getInstance ()
{
    static WaveClientReceiverObjectManager *pWaveClientReceiverObjectManager = new WaveClientReceiverObjectManager ();

    WaveNs::prismAssert (NULL != pWaveClientReceiverObjectManager, __FILE__, __LINE__);

    return (pWaveClientReceiverObjectManager);
}

PrismServiceId WaveClientReceiverObjectManager:: getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void WaveClientReceiverObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status                                = WAVE_MESSAGE_SUCCESS;
    UI32       autoScanCount                         = 0;
    UI32       managementInterfaceClientReceiverPort = FrameworkToolKit::getManagementInterfaceClientReceiverPort ();

    m_pServerSocketForWaveManagementInterfaceServers = NULL;

    m_pServerSocketForWaveManagementInterfaceServers = new ServerStreamingSocket (managementInterfaceClientReceiverPort, 1);

    prismAssert (NULL != m_pServerSocketForWaveManagementInterfaceServers, __FILE__, __LINE__);

    status = m_pServerSocketForWaveManagementInterfaceServers->getStatus ();

    if (true == m_autoScanForAvailablePort)
    {
        if (SERVER_STREAMING_SOCKET_SUCCESS != status)
        {
            delete m_pServerSocketForWaveManagementInterfaceServers;

            while (m_autoScanLimit > autoScanCount)
            {
                managementInterfaceClientReceiverPort++;

                m_pServerSocketForWaveManagementInterfaceServers = new ServerStreamingSocket (managementInterfaceClientReceiverPort, 1);

                prismAssert (NULL != m_pServerSocketForWaveManagementInterfaceServers, __FILE__, __LINE__);

                status = m_pServerSocketForWaveManagementInterfaceServers->getStatus ();

                if (SERVER_STREAMING_SOCKET_SUCCESS == status)
                {
                    FrameworkToolKit::setManagementInterfaceClientReceiverPort (managementInterfaceClientReceiverPort);

                    break;
                }
                else
                {
                    delete m_pServerSocketForWaveManagementInterfaceServers;
                    m_pServerSocketForWaveManagementInterfaceServers = NULL;
                }

                autoScanCount++;
            }
        }
    }

    if (SERVER_STREAMING_SOCKET_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientReceiverObjectManager::initialize : Could not bind to the Wave Client Receiver Port.  Please make sure that this port is not used by any other application.  Otherwise specify a different port for this purpose.");

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveClientReceiverObjectManager::initialize : Successfully bound to the Wave Client Receiver Port");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void WaveClientReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "WaveClientReceiverObjectManager::boot : Awaiting NEW Management Interface Client Connections...");

        successfullyAcceptedNewConnection = m_pServerSocketForWaveManagementInterfaceServers->accept (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "WaveClientReceiverObjectManager::boot : Could not accept connection from a new Management Interface Client.  This could be because of an unwanted Client arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_INFO, "WaveClientReceiverObjectManager::boot : Accepted A NEW connection From : " + pNewServerStreamingSocket->getPeerIpAddress ());

        WaveClientReceiverThread *pWaveClientReceiverThread = new WaveClientReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pWaveClientReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientReceiverObjectManager::boot : A new thread COULD NOT BE CREATED to monitor messages from this newly connected Client.  status : " + FrameworkToolKit::localize (status));

            delete pWaveClientReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientReceiverObjectManager::boot : Now a new thread will monitor messages from this newly connected Client.");
        }
    }
}

void WaveClientReceiverObjectManager::setAutoScanForAvailablePort (const bool &autoScanForAvailablePort)
{
    m_autoScanForAvailablePort = autoScanForAvailablePort;
}

}
