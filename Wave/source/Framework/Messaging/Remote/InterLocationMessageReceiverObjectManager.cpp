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

#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverThread.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/ShellDebug.h"
#include <map>
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

static map<string, InterLocationMessageReceiverThread *> s_interLocationMessageReceiverThreadCache;
static PrismMutex                                        s_interLocationMessageReceiverThreadCacheMutex;

bool InterLocationMessageReceiverObjectManager::addInterLocationMessageReceiverThreadToCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort, InterLocationMessageReceiverThread *&pInterLocationMessageReceiverThread)
{
#if 0
    while (true)
    {
        s_interLocationMessageReceiverThreadCacheMutex.lock ();

        map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationIpAddress);
        map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

        if (element == end)
        {
            s_interLocationMessageReceiverThreadCache[remoteLocationIpAddress] = pInterLocationMessageReceiverThread;

            s_interLocationMessageReceiverThreadCacheMutex.unlock ();

            break;
        }
        else
        {
            s_interLocationMessageReceiverThreadCacheMutex.unlock ();
        }

        prismSleep (1);
    }
#else

    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element == end)
    {
        s_interLocationMessageReceiverThreadCache[remoteLocationUniqueId] = pInterLocationMessageReceiverThread;
        s_interLocationMessageReceiverThreadCacheMutex.unlock ();
        return (true);
    }
    else
    {
        s_interLocationMessageReceiverThreadCacheMutex.unlock ();;
        return (false);
    }
#endif
}

void InterLocationMessageReceiverObjectManager::removeInterLocationMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {
        s_interLocationMessageReceiverThreadCache.erase (element);
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
}

void InterLocationMessageReceiverObjectManager::terminateAndRemoveInterLocationMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (element != end)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "InterLocationMessageReceiverObjectManager::terminateAndRemoveInterLocationMessageReceiverThreadFromCache : Forcefully Terminating a thread that is monitoring messages from " + remoteLocationUniqueId);

#if 0
        int status = pthread_cancel ((element->second)->getId ());

        if (0 != status)
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("InterLocationMessageReceiverObjectManager::terminateAndRemoveInterLocationMessageReceiverThreadFromCache : Could not terminate the thread : Status = ") + status);
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

void InterLocationMessageReceiverObjectManager::closePeerServerStreamingSocketAndRemoveInterLocationMessageReceiverThreadFromCache (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

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

bool InterLocationMessageReceiverObjectManager::isACurrentlyConnectedLocation (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    string remoteLocationUniqueId = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

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

void InterLocationMessageReceiverObjectManager::debugPrint (UI32 argc, vector<string> argv)
{
    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.begin ();
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    while (end != element)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, element->first);

        element++;
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();
}

void InterLocationMessageReceiverObjectManager::registerDebugShellEntries ()
{
    addDebugFunction ((ShellCmdFunction) (&InterLocationMessageReceiverObjectManager::debugPrint), "printilmrcache");
}

string InterLocationMessageReceiverObjectManager::getRemoteLocationUniqueIdentifier (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    return (remoteLocationIpAddress + ":" + remoteLocationPort);
}

InterLocationMessageReceiverObjectManager::InterLocationMessageReceiverObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks ("Remote Message Receiver")
{

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
}

InterLocationMessageReceiverObjectManager::~InterLocationMessageReceiverObjectManager ()
{
}

InterLocationMessageReceiverObjectManager *InterLocationMessageReceiverObjectManager::getInstance ()
{
    static InterLocationMessageReceiverObjectManager *pInterLocationMessageReceiverObjectManager = NULL;

    if (NULL == pInterLocationMessageReceiverObjectManager)
    {
        pInterLocationMessageReceiverObjectManager = new InterLocationMessageReceiverObjectManager ();
        WaveNs::prismAssert (NULL != pInterLocationMessageReceiverObjectManager, __FILE__, __LINE__);
    }

    return (pInterLocationMessageReceiverObjectManager);
}

PrismServiceId InterLocationMessageReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void InterLocationMessageReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    InterLocationMessageReceiverThread *pNewInterLocationMessageReceiverThread = NULL;

    (PrismFrameworkObjectManager::getInstance ())->initializeServerCommunications ();

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();

    trace (TRACE_LEVEL_DEBUG, "InterLocationMessageReceiverObjectManager::initialize : Now Getting Ready To Accept Connections ...");

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "InterLocationMessageReceiverObjectManager::initialize : Awaiting NEW Connections...");

        successfullyAcceptedNewConnection = (PrismFrameworkObjectManager::getInstance ())->acceptNewConnection (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "InterLocationMessageReceiverObjectManager::initialize : Could not accept connection from a new location.  This could be because of an unwanted location arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_INFO, "InterLocationMessageReceiverObjectManager::initialize : Accepted A NEW connection From : " + pNewServerStreamingSocket->getPeerIpAddress ());

        pNewInterLocationMessageReceiverThread = new InterLocationMessageReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pNewInterLocationMessageReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "InterLocationMessageReceiverObjectManager::initialize : A new thread COULD NOT BE CREATED to monitor messages from this newly connected location.  Some things is seriously wrong.");

            delete pNewInterLocationMessageReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "InterLocationMessageReceiverObjectManager::initialize : Now a new thread will monitor messages from this newly connected location.");
        }
    }
}

string InterLocationMessageReceiverObjectManager::getMessageVersionForRemoteLocation (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort)
{
    string  remoteLocationUniqueId  = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);
    string  messageVersion          = "";

    s_interLocationMessageReceiverThreadCacheMutex.lock ();

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end ();

    if (end != element)
    {
        InterLocationMessageReceiverThread* pThread = element->second;
        messageVersion = pThread->getPeerServerMessageVersion ();        
    }

    s_interLocationMessageReceiverThreadCacheMutex.unlock ();

    return (messageVersion);
}

UI8 InterLocationMessageReceiverObjectManager::getSerializationTypeForRemoteLocation (const string &remoteLocationIpAddress, const SI32 &remoteLocationPort) 
{
    string  remoteLocationUniqueId  = getRemoteLocationUniqueIdentifier (remoteLocationIpAddress, remoteLocationPort);
    UI8     serializationType       = SERIALIZE_WITH_UNKNOWN;

    s_interLocationMessageReceiverThreadCacheMutex.lock (); 

    map<string, InterLocationMessageReceiverThread *>::iterator element = s_interLocationMessageReceiverThreadCache.find (remoteLocationUniqueId);
    map<string, InterLocationMessageReceiverThread *>::iterator end     = s_interLocationMessageReceiverThreadCache.end (); 

    if (end != element)
    {   
        InterLocationMessageReceiverThread* pThread = element->second;
        serializationType = pThread->getPeerServerSerializationType ();    
    }   

    s_interLocationMessageReceiverThreadCacheMutex.unlock (); 

    return (serializationType);
}

}

