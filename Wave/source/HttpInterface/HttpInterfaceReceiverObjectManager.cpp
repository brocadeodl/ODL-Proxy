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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "HttpInterface/HttpInterfaceReceiverObjectManager.h"
#include "HttpInterface/HttpInterfaceReceiverThread.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/ServerStreamingSocket.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "HttpInterface/HttpInterfaceMethodWorker.h"
#include "HttpInterface/HttpInterfaceGetWorker.h"
#include "HttpInterface/HttpInterfacePostWorker.h"
#include "HttpInterface/HttpInterfacePutWorker.h"
#include "HttpInterface/HttpInterfaceDeleteWorker.h"
#include "Debug/WaveDebugServerPage.h"
#include "Shell/ShellDebug.h"
#include "HttpInterface/WaveServerPageDirectory.h"

namespace WaveNs
{

map<WaveHttpInterfaceMethod, HttpInterfaceMethodWorker *> HttpInterfaceReceiverObjectManager::m_httpInterfaceMethodWorkers;

HttpInterfaceReceiverObjectManager::HttpInterfaceReceiverObjectManager ()
 : WaveLocalObjectManagerForUserSpecificTasks (getServiceName ())
{
    setAllowAutomaticallyUnlistenForEvents (false);

    restrictMessageHistoryLogging (false, false, false);

    HttpInterfaceGetWorker *pHttpInterfaceGetWorker = new HttpInterfaceGetWorker (this);

    prismAssert (NULL != pHttpInterfaceGetWorker, __FILE__, __LINE__);

    HttpInterfacePostWorker *pHttpInterfacePostWorker = new HttpInterfacePostWorker (this);

    prismAssert (NULL != pHttpInterfacePostWorker, __FILE__, __LINE__);

    HttpInterfacePutWorker *pHttpInterfacePutWorker = new HttpInterfacePutWorker (this);

    prismAssert (NULL != pHttpInterfacePutWorker, __FILE__, __LINE__);

    HttpInterfaceDeleteWorker *pHttpInterfaceDeleteWorker = new HttpInterfaceDeleteWorker (this);

    prismAssert (NULL != pHttpInterfaceDeleteWorker, __FILE__, __LINE__);

    WaveDebugServerPage *pWaveDebugServerPage = new WaveDebugServerPage (this, "/debug");

    prismAssert (NULL != pWaveDebugServerPage, __FILE__, __LINE__);

    addDebugFunction ((ShellCmdFunction) (&WaveServerPageDirectory::print), "printwaveserverpagetree");
}

HttpInterfaceReceiverObjectManager::~HttpInterfaceReceiverObjectManager ()
{
}

string HttpInterfaceReceiverObjectManager::getServiceName ()
{
    return ("HTTP Interface");
}

HttpInterfaceReceiverObjectManager *HttpInterfaceReceiverObjectManager::getInstance ()
{
    static HttpInterfaceReceiverObjectManager *pHttpInterfaceReceiverObjectManager = new HttpInterfaceReceiverObjectManager ();

    WaveNs::prismAssert (NULL != pHttpInterfaceReceiverObjectManager, __FILE__, __LINE__);

    return (pHttpInterfaceReceiverObjectManager);
}

PrismServiceId HttpInterfaceReceiverObjectManager::getPrismServiceId ()
{
    return ((getInstance())->getServiceId ());
}

void HttpInterfaceReceiverObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    m_pServerSocketForHttpInterfaceClients = new ServerStreamingSocket (FrameworkToolKit::getHttpInterfaceReceiverPort (), 1);

    prismAssert (NULL != m_pServerSocketForHttpInterfaceClients, __FILE__, __LINE__);

    status = m_pServerSocketForHttpInterfaceClients->getStatus ();

    if (SERVER_STREAMING_SOCKET_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "HttpInterfaceReceiverObjectManager::initialize : Could not bind to the HTTP Interface Receiver Port.  Please make sure that this port is not used by any other application.  Otherwise specify a different port for this purpose.");

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverObjectManager::initialize : Successfully bound to the Management Interface Receiver Port");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void HttpInterfaceReceiverObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void HttpInterfaceReceiverObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler (const BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    // Since we told framework not to unlistenEvents, we must explicitly unlisten for evnets since we asre going to go into an infinite while loop.

    unlistenEvents ();

    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));

    trace (TRACE_LEVEL_INFO, "HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now accepting connections from Wave Clients.");

    for (;;)
    {
        ServerStreamingSocket *pNewServerStreamingSocket         = new ServerStreamingSocket;
        bool                   successfullyAcceptedNewConnection = false;

        prismAssert (NULL != pNewServerStreamingSocket, __FILE__, __LINE__);

        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Awaiting NEW HTTP Interface Client Connections...");

        successfullyAcceptedNewConnection = m_pServerSocketForHttpInterfaceClients->accept (*pNewServerStreamingSocket);

        if (true != successfullyAcceptedNewConnection)
        {
            trace (TRACE_LEVEL_INFO, "HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Could not accept connection from a new HTTP Interface Client.  This could be because of an unwanted Client arbitrarily trying to connect to us.");

            delete pNewServerStreamingSocket;
            continue;
        }

        trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Accepted A NEW connection From : " + pNewServerStreamingSocket->getPeerIpAddress ());

        HttpInterfaceReceiverThread *pHttpInterfaceReceiverThread = new HttpInterfaceReceiverThread (pNewServerStreamingSocket);

        WaveThreadStatus status = pHttpInterfaceReceiverThread->run ();

        if (WAVE_THREAD_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "HttpInterfaceReceiverObjectManager::boot : A new thread COULD NOT BE CREATED to monitor messages from this newly connected Client.  Status : " + FrameworkToolKit::localize (status));

            delete pHttpInterfaceReceiverThread;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "HttpInterfaceReceiverObjectManager::bootCompleteForThisLocationEventHandler : Now a new thread will monitor messages from this newly connected Client.");
        }
    }
}

bool HttpInterfaceReceiverObjectManager::isAKnownHttpInterfaceMethod (const WaveHttpInterfaceMethod &waveHttpInterfaceMethod)
{
    const map<WaveHttpInterfaceMethod, HttpInterfaceMethodWorker *>::iterator element    = m_httpInterfaceMethodWorkers.find (waveHttpInterfaceMethod);
    const map<WaveHttpInterfaceMethod, HttpInterfaceMethodWorker *>::iterator endElement = m_httpInterfaceMethodWorkers.end ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

HttpInterfaceMethodWorker *HttpInterfaceReceiverObjectManager::getHttpInterfaceMethodWorker (const WaveHttpInterfaceMethod &waveHttpInterfaceMethod)
{
    HttpInterfaceMethodWorker *pHttpInterfaceMethodWorker = NULL;

    if (true == (isAKnownHttpInterfaceMethod (waveHttpInterfaceMethod)))
    {
        pHttpInterfaceMethodWorker = m_httpInterfaceMethodWorkers[waveHttpInterfaceMethod];
    }

    return (pHttpInterfaceMethodWorker);
}

void HttpInterfaceReceiverObjectManager::addHttpInterfaceMethodWorker(const WaveHttpInterfaceMethod& waveHttpInterfaceMethod, HttpInterfaceMethodWorker* pHttpInterfaceMethodWorker)
{
    if (false == (isAKnownHttpInterfaceMethod (waveHttpInterfaceMethod)))
    {
        m_httpInterfaceMethodWorkers[waveHttpInterfaceMethod] = pHttpInterfaceMethodWorker;
    }
    else
    {
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
}

}
