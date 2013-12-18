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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Messaging/HaPeer/HaPeerMessageReceiverObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageReceiverThread.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/ShellDebug.h"
#include <map>
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

static map<string, HaPeerMessageReceiverThread *> s_interLocationMessageReceiverThreadCache;
static PrismMutex                                 s_interLocationMessageReceiverThreadCacheMutex;


bool HaPeerMessageReceiverObjectManager::addHaPeerMessageReceiverThreadToCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, HaPeerMessageReceiverThread *&pHaPeerMessageReceiverThread)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element == end)
    {
        s_interLocationMessageReceiverThreadCache[remoteLocationUniqueId] = pHaPeerMessageReceiverThread;
        s_interLocationMessageReceiverThreadCacheMutex.unlock ();
        return (true);
    }
    else
    {
        s_interLocationMessageReceiverThreadCacheMutex.unlock ();;
        return (false);
    }
}

void HaPeerMessageReceiverObjectManager::removeHaPeerMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {
        s_interLocationMessageReceiverThreadCache.erase (element);
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
}

void HaPeerMessageReceiverObjectManager::terminateAndRemoveHaPeerMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "HaPeerMessageReceiverObjectManager::terminateAndRemoveHaPeerMessageReceiverThreadFromCache : Forcefully Terminating a thread that is monitoring messages from " + remoteLocationUniqueId);

#if 0
        int status = pthread_cancel ((element->second)->getId ());

        if (0 != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("HaPeerMessageReceiverObjectManager::terminateAndRemoveHaPeerMessageReceiverThreadFromCache : Could not terminate the thread : Status = ") + status);
        }
#endif

        // Reset the Peer Server details so that when the thread really exits, it will not try to remove the details from cache.

        (element->second)->setPeerServerDetails ("", 0);

        // Close and reset the Peer Server Streaming Socket.

#if 0
        (element->second)->closePeerServerStreamingSocket ();
#endif

        s_interLocationMessageReceiverThreadCache.erase (element);
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
}

void HaPeerMessageReceiverObjectManager::closePeerServerStreamingSocketAndRemoveHaPeerMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {
        // Reset the Peer Server details so that when the thread really exits, it will not try to remove the details from cache.

        (element->second)->setPeerServerDetails ("", 0);

        // Close and reset the Peer Server Streaming Socket.

        (element->second)->closePeerServerStreamingSocket ();

        s_interLocationMessageReceiverThreadCache.erase (element);
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
}

bool HaPeerMessageReceiverObjectManager::isACurrentlyConnectedLocation (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();

    if (element != end)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void HaPeerMessageReceiverObjectManager::debugPrint (UI32 argc, vector<string> argv)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.begin ();
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    while (end != element)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, element->first);

        element++;
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
}

void HaPeerMessageReceiverObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&HaPeerMessageReceiverObjectManager::debugPrint), "printilmrcache");
}

string HaPeerMessageReceiverObjectManager::getRemoteLocationUniqueIdentifier (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    return (remoteLocationIpAddress + ":" + remoteLocationPort);
}

HaPeerMessageReceiverObjectManager::HaPeerMessageReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks ("Ha Peer Message Receiver")
{
}

HaPeerMessageReceiverObjectManager::~HaPeerMessageReceiverObjectManager ()
{
}

HaPeerMessageReceiverObjectManager *HaPeerMessageReceiverObjectManager::getInstance ()
{
    static HaPeerMessageReceiverObjectManager *pHaPeerMessageReceiverObjectManager = NULL;

    if (NULL == pHaPeerMessageReceiverObjectManager)
    {
        pHaPeerMessageReceiverObjectManager = new HaPeerMessageReceiverObjectManager ();
        WaveNs::prismAssert (NULL != pHaPeerMessageReceiverObjectManager, __FILE__, __LINE__);
    }

    return (pHaPeerMessageReceiverObjectManager);
}

PrismServiceId HaPeerMessageReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void HaPeerMessageReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    HaPeerMessageReceiverThread *pNewHaPeerMessageReceiverThread = NULL;

    (PrismFrameworkObjectManagerHaSyncWorker::getInstance ())->initializeHaPeerServerCommunications ();

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();

    trace (TRACE_LEVEL_DEBUG, "HaPeerMessageReceiverObjectManager::initialize : Now Getting Ready To Accept Connections ...");

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "HaPeerMessageReceiverObjectManager::initialize : Awaiting NEW Connections...");

        successfullyAcceptedNewConnection = (PrismFrameworkObjectManagerHaSyncWorker::getInstance ())->acceptHaPeerConnection (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "HaPeerMessageReceiverObjectManager::initialize : Could not accept connection from a new location.  This could be because of an unwanted location arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_INFO, "HaPeerMessageReceiverObjectManager::initialize : Accepted A NEW connection From : " + pNewServerStreamingSocket->getPeerIpAddress ());

        pNewHaPeerMessageReceiverThread = new HaPeerMessageReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pNewHaPeerMessageReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "HaPeerMessageReceiverObjectManager::initialize : A new thread COULD NOT BE CREATED to monitor messages from this newly connected location.  Some things is seriously wrong.");

            delete pNewHaPeerMessageReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "HaPeerMessageReceiverObjectManager::initialize : Now a new thread will monitor messages from this newly connected location.");
        }
    }
}

string HaPeerMessageReceiverObjectManager::getMessageVersionForHaPeer (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string remoteLocationUniqueId   = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);
    string messageVersion           = "";

    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {
        HaPeerMessageReceiverThread* pThread = element->second;
        messageVersion = pThread->getPeerServerMessageVersion ();
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
    
    return (messageVersion);
}

UI8 HaPeerMessageReceiverObjectManager::getSerializationTypeForHaPeer (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string remoteLocationUniqueId   = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);
    UI8    serializationType        = SERIALIZE_WITH_UNKNOWN;

    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    map<string, HaPeerMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, HaPeerMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {   
        HaPeerMessageReceiverThread* pThread = element->second;
        serializationType = pThread->getPeerServerSerializationType ();
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();

    return (serializationType);
}

}
