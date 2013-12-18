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

#include "ManagementInterface/ManagementInterfaceReceiverObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ManagementInterface/ManagementInterfaceReceiverThread.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/ShellDebug.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include <map>

namespace WaveNs
{

static map<string, ManagementInterfaceReceiverThread *> s_managementInterfaceReceiverThreadCache;
static PrismMutex                                       s_managementInterfaceReceiverThreadCacheMutex;

bool ManagementInterfaceReceiverObjectManager::addManagementInterfaceReceiverThreadToCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, ManagementInterfaceReceiverThread *&pManagementInterfaceReceiverThread)
{
    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    if (element == end)
    {
        s_managementInterfaceReceiverThreadCache[remoteLocationUniqueId] = pManagementInterfaceReceiverThread;
        s_managementInterfaceReceiverThreadCacheMutex.unlock ();
        return (true);
    }
    else
    {
        s_managementInterfaceReceiverThreadCacheMutex.unlock ();;
        return (false);
    }
}

string ManagementInterfaceReceiverObjectManager::getMessageVersionForManagemntInterface (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort); 
    string messageVersion         = "";

    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    if (element != end)
    {
        ManagementInterfaceReceiverThread* pReceiverThread = element->second;
        messageVersion = pReceiverThread->getPeerServerMessageVersion ();
    }

    s_managementInterfaceReceiverThreadCacheMutex.unlock ();

    return (messageVersion);
}

UI8 ManagementInterfaceReceiverObjectManager::getSerializationTypeForManagemntInterface (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);
    UI8    serializationType      = SERIALIZE_WITH_UNKNOWN;

    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    if (element != end)
    {   
        ManagementInterfaceReceiverThread* pReceiverThread = element->second;
        serializationType = pReceiverThread->getPeerServerSerializationType ();
    }

    s_managementInterfaceReceiverThreadCacheMutex.unlock ();

    return (serializationType);
}

void ManagementInterfaceReceiverObjectManager::removeManagemntInterfaceReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    if (element != end)
    {
        s_managementInterfaceReceiverThreadCache.erase (element);
    }

    s_managementInterfaceReceiverThreadCacheMutex.unlock ();
}

void ManagementInterfaceReceiverObjectManager::terminateAndRemoveManagemntInterfaceReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    if (element != end)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "ManagementInterfaceReceiverObjectManager::terminateAndRemoveManagemntInterfaceReceiverThreadFromCache : Forcefully Terminating a thread that is monitoring messages from " + remoteLocationUniqueId);

#if 0
        int status = pthread_cancel ((element->second)->getId ());

        if (0 != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("ManagementInterfaceReceiverObjectManager::terminateAndRemoveManagemntInterfaceReceiverThreadFromCache : Could not terminate the thread : Status = ") + status);
        }
#endif

        // Reset the Peer Server details so that when the thread really exits, it will not try to remove the details from cache.

        (element->second)->setPeerServerDetails ("", 0);

        // Close and reset the Peer Server Streaming Socket.

#if 0
        (element->second)->closePeerServerStreamingSocket ();
#endif

        s_managementInterfaceReceiverThreadCache.erase (element);

    } else {
        WaveNs::trace (TRACE_LEVEL_INFO, "ManagementInterfaceReceiverObjectManager::terminateAndRemoveManagemntInterfaceReceiverThreadFromCache : Can not forcefully Terminating a thread that is monitoring messages from " + remoteLocationUniqueId);
    }

    s_managementInterfaceReceiverThreadCacheMutex.unlock ();
}

string ManagementInterfaceReceiverObjectManager::getRemoteLocationUniqueIdentifier (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    return (remoteLocationIpAddress + ":" + remoteLocationPort);
}

bool ManagementInterfaceReceiverObjectManager::isACurrentlyConnectedLocation (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    s_managementInterfaceReceiverThreadCacheMutex.unlock ();

    if (element != end)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void ManagementInterfaceReceiverObjectManager::debugPrint (UI32 argc, vector<string> argv)
{
    s_managementInterfaceReceiverThreadCacheMutex.lock ();

    map<string, ManagementInterfaceReceiverThread *>::iterator element = s_managementInterfaceReceiverThreadCache.begin ();
    map<string, ManagementInterfaceReceiverThread *>::iterator end     = s_managementInterfaceReceiverThreadCache.end ();

    while (end != element)
    {
	    WaveNs::trace (TRACE_LEVEL_INFO, element->first + " Name:" + (element->second)->getPeerClientName());
        element++;
    }

    s_managementInterfaceReceiverThreadCacheMutex.unlock ();
}

void ManagementInterfaceReceiverObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&ManagementInterfaceReceiverObjectManager::debugPrint), "printmircache");
}

ManagementInterfaceReceiverObjectManager::ManagementInterfaceReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks ("Management Interface Receiver"),
      m_pServerSocketForManagementInterfaceClients (NULL)
{
    setTraceLevel                          (TRACE_LEVEL_ERROR);
    setAllowAutomaticallyUnlistenForEvents (false);

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
}

ManagementInterfaceReceiverObjectManager::~ManagementInterfaceReceiverObjectManager ()
{
    if (NULL != m_pServerSocketForManagementInterfaceClients)
    {
        delete m_pServerSocketForManagementInterfaceClients;
        m_pServerSocketForManagementInterfaceClients = NULL;
    }
}

ManagementInterfaceReceiverObjectManager *ManagementInterfaceReceiverObjectManager::getInstance ()
{
    static ManagementInterfaceReceiverObjectManager *pManagementInterfaceReceiverObjectManager = NULL;

    if (NULL == pManagementInterfaceReceiverObjectManager)
    {
        pManagementInterfaceReceiverObjectManager = new ManagementInterfaceReceiverObjectManager ();
        WaveNs::prismAssert (NULL != pManagementInterfaceReceiverObjectManager, __FILE__, __LINE__);
    }

    return (pManagementInterfaceReceiverObjectManager);
}

PrismServiceId ManagementInterfaceReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void ManagementInterfaceReceiverObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    m_pServerSocketForManagementInterfaceClients = new ServerStreamingSocket (FrameworkToolKit::getManagementInterfaceReceiverPort (), 1);

    prismAssert (NULL != m_pServerSocketForManagementInterfaceClients, __FILE__, __LINE__);

    status = m_pServerSocketForManagementInterfaceClients->getStatus ();

    if (SERVER_STREAMING_SOCKET_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "ManagementInterfaceReceiverObjectManager::initialize : Could not bind to the Management Interface Receiver Port.  Please make sure that this port is not used by any other application.  Otherwise specify a different port for this purpose.");

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "ManagementInterfaceReceiverObjectManager::initialize : Successfully bound to the Management Interface Receiver Port");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void ManagementInterfaceReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void ManagementInterfaceReceiverObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    // Since we told framework not to unlistenEvents, we must explicitly unlisten for evnets since we asre going to go into an infinite while loop.

    unlistenEvents ();

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

    trace (TRACE_LEVEL_INFO, "ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now accepting connections from Wave Clients.");

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Awaiting NEW Management Interface Client Connections...");

        successfullyAcceptedNewConnection = m_pServerSocketForManagementInterfaceClients->accept (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Could not accept connection from a new Management Interface Client.  This could be because of an unwanted Client arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_DEBUG, string ("ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Accepted A NEW connection From : ") + pNewServerStreamingSocket->getPeerIpAddress () + string (" Port:") +  pNewServerStreamingSocket->getPeerPort());

        ManagementInterfaceReceiverThread *pManagementInterfaceReceiverThread = new ManagementInterfaceReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pManagementInterfaceReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "ManagementInterfaceReceiverObjectManager::boot : A new thread COULD NOT BE CREATED to monitor messages from this newly connected Client.  Status : " + FrameworkToolKit::localize (status));

            delete pManagementInterfaceReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "ManagementInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now a new thread will monitor messages from this newly connected Client.");
        }
    }
}

}
