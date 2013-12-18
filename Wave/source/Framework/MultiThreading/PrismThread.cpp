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

#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Messaging/Local/PrismEvent.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Messaging/Remote/InterLocationMessageTransportObjectManager.h"
#include "Framework/Messaging/Remote/InterLocationMessageReceiverObjectManager.h"
#include "Framework/Messaging/HaPeer/HaPeerMessageTransportObjectManager.h"
#include "WaveResourceIdEnums.h"
#include "Framework/ObjectModel/ReservedWaveLocalObjectManager.h"

#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>

using namespace std;

namespace WaveNs
{

static PrismServiceMap                         *s_pPrismServiceMap                                       = NULL;

       map<PrismThreadId, WaveObjectManager *>  PrismThread::m_prismThreadIdToWaveObjectManagerMap;
       PrismMutex                               PrismThread::m_prismThreadIdToWaveObjectManagerMapMutex;

PrismThread::PrismThread (PrismServiceId id, const string &serviceName, const UI32 &stackSize, const vector<UI32> *pCpuAffinityVector)
    : PrismPosixThread    (stackSize),
      m_prismServiceId    (id),
      m_wakeupCaller      (),
      m_wakeupCondition   (&m_wakeupCaller),
      m_cpuAffinityVector (*pCpuAffinityVector),
      m_terminateThread   (false)
{
    if (NULL == s_pPrismServiceMap)
    {
        s_pPrismServiceMap = new PrismServiceMap ();
    }

    s_pPrismServiceMap->addServiceMap (m_prismServiceId, this, serviceName);
    initializeHoldCounts ();
}

PrismThread::PrismThread (PrismServiceId id, WaveObjectManager *pWaveObjectManager, const string &serviceName, const UI32 &stackSize, const vector<UI32> *pCpuAffinityVector)
    : PrismPosixThread    (stackSize),
      m_prismServiceId    (id),
      m_wakeupCaller      (),
      m_wakeupCondition   (&m_wakeupCaller),
      m_cpuAffinityVector (*pCpuAffinityVector),
      m_terminateThread   (false)
{
    if (NULL == s_pPrismServiceMap)
    {
        s_pPrismServiceMap = new PrismServiceMap ();
    }

    s_pPrismServiceMap->addServiceMap (m_prismServiceId, this, serviceName);
    addWaveObjectManager (pWaveObjectManager);
    initializeHoldCounts ();
}

PrismThread::~PrismThread ()
{
    s_pPrismServiceMap->removeServiceMap (m_prismServiceId);

    UI32 numberOfWaveObjectManagers = m_pWaveObjectManagers.size ();
    UI32 i                          = 0;

    for (i = 0 ;i < numberOfWaveObjectManagers; i++)
    {
        delete m_pWaveObjectManagers[i];
    }

    PrismThreadId thisThreadId = pthread_self ();

    prismAssert (1 == numberOfWaveObjectManagers, __FILE__, __LINE__); // For now, enforce that there is exactly on Wave OM per thread.

    m_prismThreadIdToWaveObjectManagerMapMutex.lock ();

    m_prismThreadIdToWaveObjectManagerMap.erase (m_prismThreadIdToWaveObjectManagerMap.find (thisThreadId));

    m_prismThreadIdToWaveObjectManagerMapMutex.unlock ();
}

WaveThreadStatus PrismThread::start ()
{
    UI32          numberOfWaveObjectManagers = m_pWaveObjectManagers.size ();
    PrismThreadId thisThreadId               = pthread_self ();

    prismAssert (1 == numberOfWaveObjectManagers, __FILE__, __LINE__); // For now, enfore that there is exactly on Wave OM per thread.

    while (true)
    {
        if (true == (FrameworkToolKit::isFrameworkReadyToBoot ()))
        {
            break;
        }

        prismUSleep (10000);
    }

    m_prismThreadIdToWaveObjectManagerMapMutex.lock ();
    m_prismThreadIdToWaveObjectManagerMap[thisThreadId] = m_pWaveObjectManagers[0];
    //trace (TRACE_LEVEL_INFO, string ("PrismThread::start : Prism Thread ID ") + thisThreadId + string (" corresponds to Wave OM : ") + (m_pWaveObjectManagers[0])->getName ());
    m_prismThreadIdToWaveObjectManagerMapMutex.unlock ();

    setCpuAffinity (m_cpuAffinityVector);

    PrismMessage *pPrismMessage = NULL;
    PrismEvent   *pPrismEvent   = NULL;

    while (true)
    {
        if (true == m_terminateThread)
        {
            break;
        }

        m_gateKeeper.lock ();

        pPrismMessage = getNextMessageToProcess ();

        if (NULL == pPrismMessage)
        {
            m_wakeupCaller.lock ();
            m_gateKeeper.unlock ();
            m_wakeupCondition.wait ();
            m_wakeupCaller.unlock ();
        }
        else
        {
            m_gateKeeper.unlock ();

            WaveObjectManager *pWaveObjectManager = NULL;

            switch (pPrismMessage->getType ())
            {
                case WAVE_MESSAGE_TYPE_REQUEST :
                    pWaveObjectManager = getWaveObjectManagerForOperationCode (pPrismMessage->getOperationCode ());

                    if (NULL != pWaveObjectManager)
                    {
                        pWaveObjectManager->handlePrismMessage (pPrismMessage);
                    }
                    else
                    {
                        pPrismMessage->setCompletionStatus (WAVE_THREAD_ERROR_COULD_NOT_FIND_OM);

                        if (0 < m_pWaveObjectManagers.size ())
                        {
                            m_pWaveObjectManagers[0]->reply (pPrismMessage);
                        }
                        else
                        {
                            cerr << "This Service does not have any object managers to handle any requests.";
                            prismAssert (false, __FILE__, __LINE__);
                        }
                    }

                    break;

                case WAVE_MESSAGE_TYPE_RESPONSE :
                    pWaveObjectManager = getWaveObjectManagerForPrismMessageId (pPrismMessage->getMessageId ());

                    if (NULL != pWaveObjectManager)
                    {
                        pWaveObjectManager->handlePrismMessageResponse (FRAMEWORK_SUCCESS, pPrismMessage);
                    }
                    else
                    {
                        trace (TRACE_LEVEL_FATAL, "Failed to deliver response : Could not find corresponding Prism Object Manager.");
                        trace (TRACE_LEVEL_FATAL, string ("ServiceCode = ") + pPrismMessage->getServiceCode () + string (", OperationCode = ") + pPrismMessage->getOperationCode () +  string(", MessageId = ") + pPrismMessage->getMessageId ());
                        prismAssert (false, __FILE__, __LINE__);
                    }

                    break;

                case WAVE_MESSAGE_TYPE_EVENT :
                    pPrismEvent = dynamic_cast<PrismEvent *> (pPrismMessage);
                    prismAssert( NULL != pPrismEvent , __FILE__ , __LINE__);

                    pWaveObjectManager = getWaveObjectManagerForEventOperationCodeForListening (pPrismEvent->getSenderLocationId (), pPrismEvent->getServiceCode (), pPrismEvent->getOperationCode ());

                    if (NULL != pWaveObjectManager)
                    {
                        const PrismEvent *pTempEvent = reinterpret_cast <const PrismEvent *> (pPrismEvent);

                        pWaveObjectManager->handlePrismEvent (pTempEvent);
                    }
                    else
                    {
                        cerr << "This Service does not have any object managers to handle any events.";
                        prismAssert (false, __FILE__, __LINE__);

                        delete pPrismEvent;
                    }

                    break;

                default :
                    cerr << "PrismThread::start : Unknown message type : " << pPrismMessage->getType () << ", Service : " << s_pPrismServiceMap->getPrismServiceNameForServiceId (m_prismServiceId) << endl;
                    prismAssert (false, __FILE__, __LINE__);

                    break;
            }
        }
    }

    delete this;

    stopCallingThread ();

    // We should never reach here.  The above statement ends the execution of the thread.

    return (WAVE_THREAD_SUCCESS);
}

WaveThreadStatus PrismThread::consumePendingMessages ()
{
    while (true)
    {
        if (true == (FrameworkToolKit::isFrameworkReadyToBoot ()))
        {
            break;
        }

        prismSleep (1);
    }

    PrismMessage *pPrismMessage = NULL;
    PrismEvent   *pPrismEvent   = NULL;

    while (true)
    {
        if (true == m_terminateThread)
        {
            break;
        }

        m_gateKeeper.lock ();

        pPrismMessage = getNextMessageToProcess ();

        if (NULL == pPrismMessage)
        {
            m_gateKeeper.unlock ();

            return (WAVE_THREAD_SUCCESS);
        }
        else
        {
            m_gateKeeper.unlock ();

            WaveObjectManager *pWaveObjectManager = NULL;

            switch (pPrismMessage->getType ())
            {
                case WAVE_MESSAGE_TYPE_REQUEST :
                    pWaveObjectManager = getWaveObjectManagerForOperationCode (pPrismMessage->getOperationCode ());

                    if (NULL != pWaveObjectManager)
                    {
                        pWaveObjectManager->handlePrismMessage (pPrismMessage);
                    }
                    else
                    {
                        pPrismMessage->setCompletionStatus (WAVE_THREAD_ERROR_COULD_NOT_FIND_OM);

                        if (0 < m_pWaveObjectManagers.size ())
                        {
                            m_pWaveObjectManagers[0]->reply (pPrismMessage);
                        }
                        else
                        {
                            cerr << "This Service does not have any object managers to handle any requests.";
                            prismAssert (false, __FILE__, __LINE__);
                        }
                    }

                    break;

                case WAVE_MESSAGE_TYPE_RESPONSE :
                    pWaveObjectManager = getWaveObjectManagerForPrismMessageId (pPrismMessage->getMessageId ());

                    if (NULL != pWaveObjectManager)
                    {
                        pWaveObjectManager->handlePrismMessageResponse (FRAMEWORK_SUCCESS, pPrismMessage);
                    }
                    else
                    {
                        cout << ": Failed to deliver response : Could not find corresponding Prism Object Manager." << endl;
                        cout << "ServiceCode = " << pPrismMessage->getServiceCode () << ", OperationCode = " << pPrismMessage->getOperationCode () << ", MessageId = " << pPrismMessage->getMessageId () << endl;
                        prismAssert (false, __FILE__, __LINE__);
                    }

                    break;

                case WAVE_MESSAGE_TYPE_EVENT :

                    pPrismEvent = dynamic_cast<PrismEvent *> (pPrismMessage);
                    prismAssert( NULL != pPrismEvent , __FILE__ , __LINE__);

                    pWaveObjectManager = getWaveObjectManagerForEventOperationCodeForListening (pPrismEvent->getSenderLocationId (), pPrismEvent->getServiceCode (), pPrismEvent->getOperationCode ());

                    if (NULL != pWaveObjectManager)
                    {
                        const PrismEvent *pTempEvent = reinterpret_cast <const PrismEvent *> (pPrismEvent);

                        pWaveObjectManager->handlePrismEvent (pTempEvent);
                    }
                    else
                    {
                        cerr << "This Service does not have any object managers to handle any events.";
                        prismAssert (false, __FILE__, __LINE__);

                        delete pPrismEvent;
                    }

                    break;

                default :
                    cerr << "PrismThread::consumePendingMessages : Unknown message type : " << pPrismMessage->getType () << ", Service : " << s_pPrismServiceMap->getPrismServiceNameForServiceId (m_prismServiceId) << endl;
                    prismAssert (false, __FILE__, __LINE__);

                    break;
            }
        }
    }

    stopCallingThread ();

    // We should never reach here.  The above statement ends the execution of the thread.

    return (WAVE_THREAD_SUCCESS);
}

WaveMessageStatus PrismThread::submitMessage (PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    WaveMessagePriority messagePriority = pPrismMessage->getPriority ();

    // Check if the message has been submitted to a wrong thread.  This done by comparing this thread serviceid
    // to the message serviceid.
    // there is a n exception to this.  If the this thread serviceid is WAVE_SERVICE_INTER_LOCATION_MESSAGE_TRANSPORT, then
    // the thread will receive messages destined for other services since the thread has to transport to remote locations.

    if (((InterLocationMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) &&
         ((HaPeerMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) &&
         (m_prismServiceId != (pPrismMessage->getServiceCode ())))
    {
        cerr << "PrismThread::submitMessage : Internal Error : Submitted message to a wrong Prism Thread." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_SUBMIT_TO_INVALID_THREAD);
    }

    // Before proceeding check if the Service has been enabled to accept messages other services.
    // We accept a few messages even before the service is enabled.  We accept messages like initialize
    // and enable messages.

    UI32                operationCode       = pPrismMessage->getOperationCode ();
    WaveObjectManager *pWaveObjectManager = getWaveObjectManagerForOperationCode (operationCode);

    // by the time we reach here we must not encounter a NULL WaveObjectManager.

    if (NULL == pWaveObjectManager)
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismThread::submitMessage : There is no ObjectManager registered to rceive this operation code (") + operationCode + ").");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_NO_OM_TO_ACCEPT_MESSAGE);
    }
    else
    {
        if (false == (pWaveObjectManager->getIsEnabled ()))
        {
            if (true == (pWaveObjectManager->isOperationAllowedBeforeEnabling (operationCode)))
            {
                // We cannot use trace method here.  It will lead to an infinite recursion because the trace service
                // might not have been enabled.  Also the trace statement causes invoking subMitMessage.

                // trace (TRACE_LEVEL_DEBUG, string ("PrismThread::submitMessage : Allowing the operation (") + operationCode + ") before enabling the service.");
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, "PrismThread::submitMessage : The Service (" + FrameworkToolKit::getServiceNameById (pWaveObjectManager->getServiceId ()) + ") is not enabled yet.  Try later.");
                return (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED);
            }
        }
    }

    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    m_gateKeeper.lock ();

    m_wakeupCaller.lock ();

    // We treat timer expirations differently.  Timer expirations will be handled when they expire.
    // Even if the messages, high priority messages and events are held for the service, still we deliver the
    // timer expiration notifications.  To achieve this we have a separate queue dedicated for for Timer Expiration notifications
    // messages.  Whenever the Timer service sends such a notification message that messages goes into a separate queue.  Otherwise
    // all the messages go into repective queues according to their priority and type.

    UI32 opertionCode = pPrismMessage->getOperationCode ();

    if ((WAVE_OBJECT_MANAGER_RESUME == operationCode) || (true == pPrismMessage->getIsMessageSupportedWhenServiceIsPaused ()))
    {
        m_frameworkResumeMessages.insertAtTheBack (pPrismMessage);
    }
    else if (WAVE_OBJECT_MANAGER_PAUSE == operationCode)
    {
        m_frameworkMessages.insertAtTheFront (pPrismMessage);
    }
    else if (WAVE_OBJECT_MANAGER_TIMER_EXPIRED == opertionCode) // For Timer Expiration Messages
    {
        m_timerExpirations.insertAtTheBack (pPrismMessage); // for all the other Framework messages like initialize, enable etc.,
    }
    else if (opertionCode >= (UINT_MAX - 1001))
    {
        m_frameworkMessages.insertAtTheBack (pPrismMessage);
    }
    else
    {
        if (WAVE_MESSAGE_PRIORITY_HIGH == messagePriority)
        {
            m_highPriorityMessages.insertAtTheBack (pPrismMessage);
        }
        else if (WAVE_MESSAGE_PRIORITY_NORMAL == messagePriority)
        {
            m_messages.insertAtTheBack (pPrismMessage);
        }
        else
        {
            cout << "PrismThread::submitMessage : Submitting message with unknown priority (" << messagePriority << ")." << endl;
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR_UNKNOWN_PRIORITY;
        }
    }

    m_wakeupCondition.resume ();

    m_wakeupCaller.unlock ();

    m_gateKeeper.unlock ();

    return (status);
}

WaveMessageStatus PrismThread::submitMessageAtFront (PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    WaveMessagePriority messagePriority = pPrismMessage->getPriority ();

    // Check if the message has been submitted to a wrong thread.  This done by comparing this thread serviceid
    // to the message serviceid.
    // there is a n exception to this.  If the this thread serviceid is WAVE_SERVICE_INTER_LOCATION_MESSAGE_TRANSPORT, then
    // the thread will receive messages destined for other services since the thread has to transport to remote locations.

    if (((InterLocationMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) &&
         ((HaPeerMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) &&
         (m_prismServiceId != (pPrismMessage->getServiceCode ())))
    {
        cerr << "PrismThread::submitMessageAtFront : Internal Error : Submitted message to a wrong Prism Thread." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_SUBMIT_TO_INVALID_THREAD);
    }

    // Before proceeding check if the Service has been enabled to accept messages other services.
    // We accept a few messages even before the service is enabled.  We accept messages like initialize
    // and enable messages.

    UI32                operationCode       = pPrismMessage->getOperationCode ();
    WaveObjectManager *pWaveObjectManager = getWaveObjectManagerForOperationCode (operationCode);

    // by the time we reach here we must not encounter a NULL WaveObjectManager.

    if (NULL == pWaveObjectManager)
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismThread::submitMessageAtFront : There is no ObjectManager registered to rceive this operation code (") + operationCode + ").");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_NO_OM_TO_ACCEPT_MESSAGE);
    }
    else
    {
        if (false == (pWaveObjectManager->getIsEnabled ()))
        {
            if (true == (pWaveObjectManager->isOperationAllowedBeforeEnabling (operationCode)))
            {
                // We cannot use trace method here.  It will lead to an infinite recursion because the trace service
                // might not have been enabled.  Also the trace statement causes invoking subMitMessage.

                // trace (TRACE_LEVEL_DEBUG, string ("PrismThread::submitMessage : Allowing the operation (") + operationCode + ") before enabling the service.");
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, "PrismThread::submitMessageAtFront : The Service (" + FrameworkToolKit::getServiceNameById (pWaveObjectManager->getServiceId ()) + ") is not enabled yet.  Try later.");
                return (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED);
            }
        }
    }

    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    m_gateKeeper.lock ();

    m_wakeupCaller.lock ();

    // We treat timer expirations differently.  Timer expirations will be handled when they expire.
    // Even if the messages, high priority messages and events are held for the service, still we deliver the
    // timer expiration notifications.  To achieve this we have a separate queue dedicated for for Timer Expiration notifications
    // messages.  Whenever the Timer service sends such a notification message that messages goes into a separate queue.  Otherwise
    // all the messages go into repective queues according to their priority and type.

    UI32 opertionCode = pPrismMessage->getOperationCode ();

    if (WAVE_OBJECT_MANAGER_RESUME == operationCode)
    {
        m_frameworkResumeMessages.insertAtTheFront (pPrismMessage);
    }
    else if (WAVE_OBJECT_MANAGER_PAUSE == operationCode)
    {
        m_frameworkMessages.insertAtTheFront (pPrismMessage);
    }
    else if (WAVE_OBJECT_MANAGER_TIMER_EXPIRED == opertionCode) // For Timer Expiration Messages
    {
        m_timerExpirations.insertAtTheFront (pPrismMessage); // for all the other Framework messages like initialize, enable etc.,
    }
    else if (opertionCode >= (UINT_MAX - 1001))
    {
        m_frameworkMessages.insertAtTheFront (pPrismMessage);
    }
    else
    {
        if (WAVE_MESSAGE_PRIORITY_HIGH == messagePriority)
        {
            m_highPriorityMessages.insertAtTheFront (pPrismMessage);
        }
        else if (WAVE_MESSAGE_PRIORITY_NORMAL == messagePriority)
        {
            m_messages.insertAtTheFront (pPrismMessage);
        }
        else
        {
            cout << "PrismThread::submitMessageAtFront : Submitting message with unknown priority (" << messagePriority << ")." << endl;
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR_UNKNOWN_PRIORITY;
        }
    }

    m_wakeupCondition.resume ();

    m_wakeupCaller.unlock ();

    m_gateKeeper.unlock ();

    return (status);
}

WaveMessageStatus PrismThread::submitMessageAtBack (PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    WaveMessagePriority messagePriority = pPrismMessage->getPriority ();

    // Check if the message has been submitted to a wrong thread.  This done by comparing this thread serviceid
    // to the message serviceid.
    // there is a n exception to this.  If the this thread serviceid is WAVE_SERVICE_INTER_LOCATION_MESSAGE_TRANSPORT, then
    // the thread will receive messages destined for other services since the thread has to transport to remote locations.

    if (((InterLocationMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) &&
         ((HaPeerMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) &&
         (m_prismServiceId != (pPrismMessage->getServiceCode ())))
    {
        cerr << "PrismThread::submitMessageAtBack : Internal Error : Submitted message to a wrong Prism Thread." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_SUBMIT_TO_INVALID_THREAD);
    }

    // Before proceeding check if the Service has been enabled to accept messages other services.
    // We accept a few messages even before the service is enabled.  We accept messages like initialize
    // and enable messages.

    UI32                operationCode       = pPrismMessage->getOperationCode ();
    WaveObjectManager *pWaveObjectManager = getWaveObjectManagerForOperationCode (operationCode);

    // by the time we reach here we must not encounter a NULL WaveObjectManager.

    if (NULL == pWaveObjectManager)
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismThread::submitMessageAtBack : There is no ObjectManager registered to rceive this operation code (") + operationCode + ").");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_NO_OM_TO_ACCEPT_MESSAGE);
    }
    else
    {
        if (false == (pWaveObjectManager->getIsEnabled ()))
        {
            if (true == (pWaveObjectManager->isOperationAllowedBeforeEnabling (operationCode)))
            {
                // We cannot use trace method here.  It will lead to an infinite recursion because the trace service
                // might not have been enabled.  Also the trace statement causes invoking subMitMessage.

                // trace (TRACE_LEVEL_DEBUG, string ("PrismThread::submitMessage : Allowing the operation (") + operationCode + ") before enabling the service.");
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, "PrismThread::submitMessageAtBack : The Service (" + FrameworkToolKit::getServiceNameById (pWaveObjectManager->getServiceId ()) + ") is not enabled yet.  Try later.");
                return (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED);
            }
        }
    }

    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    m_gateKeeper.lock ();

    m_wakeupCaller.lock ();

    // We treat timer expirations differently.  Timer expirations will be handled when they expire.
    // Even if the messages, high priority messages and events are held for the service, still we deliver the
    // timer expiration notifications.  To achieve this we have a separate queue dedicated for for Timer Expiration notifications
    // messages.  Whenever the Timer service sends such a notification message that messages goes into a separate queue.  Otherwise
    // all the messages go into repective queues according to their priority and type.

    UI32 opertionCode = pPrismMessage->getOperationCode ();

    if (WAVE_OBJECT_MANAGER_RESUME == operationCode)
    {
        m_frameworkResumeMessages.insertAtTheBack (pPrismMessage);
    }
    else if (WAVE_OBJECT_MANAGER_PAUSE == operationCode)
    {
        m_frameworkMessages.insertAtTheBack (pPrismMessage);
    }
    else if (WAVE_OBJECT_MANAGER_TIMER_EXPIRED == opertionCode) // For Timer Expiration Messages
    {
        m_timerExpirations.insertAtTheBack (pPrismMessage); // for all the other Framework messages like initialize, enable etc.,
    }
    else if (opertionCode >= (UINT_MAX - 1001))
    {
        m_frameworkMessages.insertAtTheBack (pPrismMessage);
    }
    else
    {
        if (WAVE_MESSAGE_PRIORITY_HIGH == messagePriority)
        {
            m_highPriorityMessages.insertAtTheBack (pPrismMessage);
        }
        else if (WAVE_MESSAGE_PRIORITY_NORMAL == messagePriority)
        {
            m_messages.insertAtTheBack (pPrismMessage);
        }
        else
        {
            cout << "PrismThread::submitMessageAtBack : Submitting message with unknown priority (" << messagePriority << ")." << endl;
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR_UNKNOWN_PRIORITY;
        }
    }

    m_wakeupCondition.resume ();

    m_wakeupCaller.unlock ();

    m_gateKeeper.unlock ();

    return (status);
}

WaveMessageStatus PrismThread::recallMessage (PrismMessage *pPrismMessage)
{
    // We need not lock in this method since the message queues have built in locking mechanism.

    WaveMessageStatus   status          = WAVE_MESSAGE_SUCCESS;
    WaveMessagePriority messagePriority = pPrismMessage->getPriority ();

    if (WAVE_MESSAGE_PRIORITY_HIGH == messagePriority)
    {
        status = m_highPriorityMessageResponses.remove (pPrismMessage);
    }
    else if (WAVE_MESSAGE_PRIORITY_NORMAL == messagePriority)
    {
        status = m_messages.remove (pPrismMessage);
    }
    else
    {
        cout << "PrismThread::recallMessage : Submitting a reply message with unknown priority (" << messagePriority << ")." << endl;
        prismAssert (false, __FILE__, __LINE__);
        status = WAVE_MESSAGE_ERROR_RESPONSE_UNKNOWN_PRIORITY;
    }

    return (status);
}

UI32 PrismThread::recallTimerExpirationMessagesForTimer (const TimerHandle &timerHandle)
{
    // We need not lock in this method since the message queues have built in locking mechanism.

    return (m_timerExpirations.removeTimerExpirationsForTimer (timerHandle));
}

WaveMessageStatus PrismThread::submitReplyMessage (PrismMessage *pPrismMessage)
{
    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    WaveMessagePriority messagePriority = pPrismMessage->getPriority ();

    // FIXME : Sagar make sure that the thread is up and ready for receiving messages

    PrismServiceId senderServiceId = pPrismMessage->getSenderServiceCode ();

    // In general a thread accepts responses only for the messages that it sent out.

    if (m_prismServiceId != senderServiceId)
    {
        cerr << "PrismThread::submitReplyMessage : Submitting reply message to a wrong Prism Thread." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MESSAGE_ERROR_SUBMIT_RESPONSE_TO_INVALID_THREAD);
    }

    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    m_gateKeeper.lock ();

    m_wakeupCaller.lock ();

    UI32 opertionCode = pPrismMessage->getOperationCode ();

    if (WAVE_OBJECT_MANAGER_TIMER_EXPIRED == opertionCode) // For Timer Expiration Message Responses
    {
        m_timerExpirationResponses.insertAtTheBack (pPrismMessage); // for Responses to all the other Framework messages like initialize, enable etc.,
    }
    else if (opertionCode >= (UINT_MAX - 1001))
    {
        m_frameworkMessageResponses.insertAtTheBack (pPrismMessage);
    }
    else
    {
        if (WAVE_MESSAGE_PRIORITY_HIGH == messagePriority)
        {
            m_highPriorityMessageResponses.insertAtTheBack (pPrismMessage);
        }
        else if (WAVE_MESSAGE_PRIORITY_NORMAL == messagePriority)
        {
            m_messageResponses.insertAtTheBack (pPrismMessage);
        }
        else
        {
            cout << "PrismThread::submitReplyMessage : Submitting a reply message with unknown priority (" << messagePriority << ")." << endl;
            prismAssert (false, __FILE__, __LINE__);
            status = WAVE_MESSAGE_ERROR_RESPONSE_UNKNOWN_PRIORITY;
        }
    }

    m_wakeupCondition.resume ();

    m_wakeupCaller.unlock ();

    m_gateKeeper.unlock ();

    return (status);
}

WaveMessageStatus PrismThread::submitEvent (PrismEvent *pPrismEvent)
{
    prismAssert (NULL != pPrismEvent, __FILE__, __LINE__);

#if 0
    // Check if the message has been submitted to a wrong thread.  This done by comparing this thread serviceid
    // to the message serviceid.
    // there is a n exception to this.  If the this thread serviceid is WAVE_SERVICE_INTER_LOCATION_MESSAGE_TRANSPORT, then
    // the thread will receive messages destined for other services since the thread has to transport to remote locations.

    // Events are defined by the Sender (broadcasting) Service where as messages are defined by the recipient service.  So, we need to
    // compare the event receiver service id to the current thread service id.

    if (((InterLocationMessageTransportObjectManager::getPrismServiceId ()) != m_prismServiceId) && (m_prismServiceId != (pPrismEvent->getReceiverServiceId ())))
    {
        cerr << "PrismThread::submitEvent : Internal Error : Submitted message to a wrong Prism Thread." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_EVENT_ERROR_SUBMIT_TO_INVALID_THREAD);
    }
#endif

    // Before proceeding check if the Service has been enabled to accept messages other services.
    // We accept a few messages even before the service is enabled.  We accept messages like initialize
    // and enable messages.

    PrismServiceId      eventSourceServiceId  = pPrismEvent->getServiceCode ();
    UI32                eventOperationCode    = pPrismEvent->getOperationCode ();
    LocationId          eventSourceLocationId = pPrismEvent->getSenderLocationId ();
    WaveObjectManager *pWaveObjectManager   = getWaveObjectManagerForEventOperationCodeForListening (eventSourceLocationId, eventSourceServiceId, eventOperationCode);

    // by the time we reach here we must not encounter a NULL WaveObjectManager.

    if (NULL == pWaveObjectManager)
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismThread::submitEvent : There is no ObjectManager registered to rceive this event operation code (") + eventOperationCode + ").");
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_EVENT_ERROR_NO_OM_TO_ACCEPT_MESSAGE);
    }
    else
    {
        if (false == (pWaveObjectManager->getIsEnabled ()))
        {
            if (true == (WaveObjectManager::isEventAllowedBeforeEnabling (eventOperationCode)))
            {
                // We cannot use trace method here.  It will lead to an infinite recursion because the trace service
                // might not have been enabled.  Also the trace statement causes invoking subMitMessage.

                // trace (TRACE_LEVEL_DEBUG, string ("PrismThread::submitMessage : Allowing the operation (") + eventOperationCode + ") before enabling the service.");
            }
            else
            {
                trace (TRACE_LEVEL_DEBUG, "PrismThread::submitEvent : The Service (" + FrameworkToolKit::getServiceNameById (pWaveObjectManager->getServiceId ()) + ") is not enabled yet.  Try later.");
                return (WAVE_MESSAGE_ERROR_SERVICE_NOT_ENABLED);
            }
        }
    }

    WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;

    m_gateKeeper.lock ();

    m_wakeupCaller.lock ();

    PrismMessage *pPrismMessage = dynamic_cast<PrismMessage *> (pPrismEvent);

    m_events.insertAtTheBack (pPrismMessage);

    m_wakeupCondition.resume ();

    m_wakeupCaller.unlock ();

    m_gateKeeper.unlock ();

    return (status);
}

PrismMessage *PrismThread::getNextMessageToProcess ()
{
    if (false == (m_frameworkResumeMessages.isEmpty ()))
    {
        // This queue shall not be affected by the holdall call and hence we do not need to emply any mutex locking though these are messages (not responses).
        // Replies for these messages will be submitted as normal framework messages and hence there io no corresponding reponses queue.

        return (m_frameworkResumeMessages.removeAndGetFromFront ());
    }

    if (false == (m_frameworkMessageResponses.isEmpty ()))
    {
        return (m_frameworkMessageResponses.removeAndGetFromFront ());
    }

    if (false == (m_frameworkMessages.isEmpty ()))
    {
        if (WAVE_MUTEX_SUCCESS == (m_frameworkMessagesMutex.tryLock ()))
        {
            PrismMessage *pPrismMessage = m_frameworkMessages.removeAndGetFromFront ();

            m_frameworkMessagesMutex.unlock ();

            return (pPrismMessage);
        }
    }

    if (false == (m_timerExpirationResponses.isEmpty ()))
    {
        return (m_timerExpirationResponses.removeAndGetFromFront ());
    }

    if (false == (m_timerExpirations.isEmpty ()))
    {
        return (m_timerExpirations.removeAndGetFromFront ());
    }

    if (false == (m_highPriorityMessageResponses.isEmpty ()))
    {
        return (m_highPriorityMessageResponses.removeAndGetFromFront ());
    }

    if (false == (m_highPriorityMessages.isEmpty ()))
    {
        if (WAVE_MUTEX_SUCCESS == (m_highPriorityMessagesMutex.tryLock ()))
        {
            PrismMessage *pPrismMessage = m_highPriorityMessages.removeAndGetFromFront ();

            m_highPriorityMessagesMutex.unlock ();

            return (pPrismMessage);
        }
    }

    if (false == (m_messageResponses.isEmpty ()))
    {
        return (m_messageResponses.removeAndGetFromFront ());
    }

    if (false == (m_events.isEmpty ()))
    {
        if (WAVE_MUTEX_SUCCESS == (m_eventsMutex.tryLock ()))
        {
            PrismMessage *pPrismMessage = m_events.removeAndGetFromFront ();

            m_eventsMutex.unlock ();

            return (pPrismMessage);
        }
    }

    if (false == (m_messages.isEmpty ()))
    {
        if (WAVE_MUTEX_SUCCESS == (m_messagesMutex.tryLock ()))
        {
            PrismMessage *pPrismMessage = m_messages.removeAndGetFromFront ();

            m_messagesMutex.unlock ();

            return (pPrismMessage);
        }
    }

    return (NULL);
}

PrismThread *PrismThread::getPrismThreadForServiceId (PrismServiceId id)
{
    return (s_pPrismServiceMap->getPrismThreadForServiceId (id));
}

vector<UI32> PrismThread::getCpuAffinityVectorForServiceId (PrismServiceId id)
{
    PrismThread  *pPrismThread       = getPrismThreadForServiceId (id);
    vector<UI32>  cpuAffinityVector;

    if (NULL != pPrismThread)
    {
        cpuAffinityVector = pPrismThread->getCpuAffinityVector ();
    }

    return (cpuAffinityVector);
}

string PrismThread::getPrismServiceNameForServiceId (const PrismServiceId &id)
{
    return (s_pPrismServiceMap->getPrismServiceNameForServiceId (id));
}

PrismServiceId PrismThread::getPrismServiceIdForServiceName (const string &serviceName)
{
    return (s_pPrismServiceMap->getPrismServiceIdForServiceName (serviceName));
}

void PrismThread::addWaveObjectManager (WaveObjectManager *pWaveObjectManager)
{
    m_pWaveObjectManagers.push_back (pWaveObjectManager);
}

WaveObjectManager *PrismThread::getWaveObjectManagerForOperationCode (UI32 operationCode)
{
    vector<WaveObjectManager *>::iterator  element             = m_pWaveObjectManagers.begin ();
    vector<WaveObjectManager *>::iterator  limitingElement     = m_pWaveObjectManagers.end (); // last element + 1
    WaveObjectManager                     *pWaveObjectManager = NULL;

    while (element < limitingElement)
    {
        pWaveObjectManager = *element;

        if (pWaveObjectManager->isOperationCodeSupported (operationCode))
        {
            return (pWaveObjectManager);
        }

        element++;
    }

    return (NULL);
}

WaveObjectManager *PrismThread::getWaveObjectManagerForEventOperationCode (UI32 eventOperationCode)
{
    vector<WaveObjectManager *>::iterator  element             = m_pWaveObjectManagers.begin ();
    vector<WaveObjectManager *>::iterator  limitingElement     = m_pWaveObjectManagers.end (); // last element + 1
    WaveObjectManager                     *pWaveObjectManager = NULL;

    while (element < limitingElement)
    {
        pWaveObjectManager = *element;

        if (pWaveObjectManager->isEventOperationCodeSupported (eventOperationCode))
        {
            return (pWaveObjectManager);
        }

        element++;
    }

    return (NULL);
}

WaveObjectManager *PrismThread::getWaveObjectManagerForEventOperationCodeForListening (const LocationId &eventSourceLocationId, const PrismServiceId &eventSourceServiceId, const UI32 &eventOperationCode)
{
    vector<WaveObjectManager *>::iterator  element             = m_pWaveObjectManagers.begin ();
    vector<WaveObjectManager *>::iterator  limitingElement     = m_pWaveObjectManagers.end (); // last element + 1
    WaveObjectManager                     *pWaveObjectManager = NULL;

    while (element < limitingElement)
    {
        pWaveObjectManager = *element;

        if (pWaveObjectManager->isEventOperationCodeSupportedForListening (eventSourceLocationId, eventSourceServiceId, eventOperationCode))
        {
            return (pWaveObjectManager);
        }

        element++;
    }

    return (NULL);
}

WaveObjectManager *PrismThread::getWaveObjectManagerForPrismMessageId  (UI32 prismMessageId)
{
    vector<WaveObjectManager *>::iterator  element            = m_pWaveObjectManagers.begin ();
    vector<WaveObjectManager *>::iterator  limitingElement    = m_pWaveObjectManagers.end (); // last element + 1
    WaveObjectManager                     *pWaveObjectManager  = NULL;

    while (element < limitingElement)
    {
        pWaveObjectManager = *element;

        if (pWaveObjectManager->isAKnownMessage (prismMessageId))
        {
            return (pWaveObjectManager);
        }

        element++;
    }

    return (NULL);
}

WaveObjectManager *PrismThread::getWaveObjectManagerForManagedClass (const string &managedClass)
{
    vector<WaveObjectManager *>::iterator  element             = m_pWaveObjectManagers.begin ();
    vector<WaveObjectManager *>::iterator  limitingElement     = m_pWaveObjectManagers.end (); // last element + 1
    WaveObjectManager                     *pWaveObjectManager = NULL;

    while (element < limitingElement)
    {
        pWaveObjectManager = *element;

        if (pWaveObjectManager->isManagedClassSupported (managedClass))
        {
            return (pWaveObjectManager);
        }

        element++;
    }

    return (NULL);
}

PrismServiceId PrismThread::getPrismServiceId () const
{
    return (m_prismServiceId);
}

bool PrismThread::hasWaveObjectManagers ()
{
    if (0 < m_pWaveObjectManagers.size ())
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

WaveThreadStatus PrismThread::joinAllThreads ()
{
    return (s_pPrismServiceMap->joinAllThreads ());
}

void PrismThread::initializeHoldCounts ()
{
    m_messagesHoldCount             = 0;
    m_highPriorityMessagesHoldCount = 0;
    m_eventsHoldCount               = 0;
    m_frameworkMessagesHoldCount    = 0;
}

void PrismThread::holdMessages ()
{
    if (0 == m_messagesHoldCount)
    {
        m_messagesMutex.lock ();
    }

    m_messagesHoldCount++;
}

void PrismThread::holdHighPriorityMessages ()
{
    if (0 == m_highPriorityMessagesHoldCount)
    {
        m_highPriorityMessagesMutex.lock ();
    }

    m_highPriorityMessagesHoldCount++;
}

void PrismThread::holdEvents ()
{
    if (0 == m_eventsHoldCount)
    {
        m_eventsMutex.lock ();
    }

    m_eventsHoldCount++;
}

void PrismThread::holdFrameworkMessages ()
{
    if (0 == m_frameworkMessagesHoldCount)
    {
        m_frameworkMessagesMutex.lock ();
    }

    m_frameworkMessagesHoldCount++;
}

void PrismThread::holdAll ()
{
    holdMessages ();
    holdHighPriorityMessages ();
    holdEvents ();
    holdFrameworkMessages ();
}

void PrismThread::unholdMessages ()
{
    if (0 == m_messagesHoldCount)
    {
        trace (TRACE_LEVEL_FATAL, "PrismThread::unholdMessages: trying to unhold without hold.");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_messagesHoldCount--;

    if (0 == m_messagesHoldCount)
    {
        m_messagesMutex.unlock ();
    }
}

void PrismThread::unholdHighPriorityMessages ()
{
    if (0 == m_highPriorityMessagesHoldCount)
    {
        trace (TRACE_LEVEL_FATAL, "PrismThread::unholdHighPriorityMessages: trying to unhold without hold.");
        prismAssert (false, __FILE__, __LINE__);
    }
    m_highPriorityMessagesHoldCount--;

    if (0 == m_highPriorityMessagesHoldCount)
    {
        m_highPriorityMessagesMutex.unlock ();
    }
}

void PrismThread::unholdEvents ()
{
    if (0 == m_eventsHoldCount)
    {
        trace (TRACE_LEVEL_FATAL, "PrismThread::unholdEvents: trying to unhold without hold.");
        prismAssert (false, __FILE__, __LINE__);
    }
    m_eventsHoldCount--;

    if (0 == m_eventsHoldCount)
    {
        m_eventsMutex.unlock ();
    }
}

void PrismThread::unholdFrameworkMessages ()
{
    if (0 == m_frameworkMessagesHoldCount)
    {
        trace (TRACE_LEVEL_FATAL, "PrismThread::unholdFrameworkMessages: trying to unhold without hold.");
        prismAssert (false, __FILE__, __LINE__);
    }
    m_frameworkMessagesHoldCount--;

    if (0 == m_frameworkMessagesHoldCount)
    {
        m_frameworkMessagesMutex.unlock ();
    }
}

void PrismThread::unholdAll ()
{
    unholdMessages ();
    unholdHighPriorityMessages ();
    unholdEvents ();
    unholdFrameworkMessages ();
}

PrismThread *PrismThread::getPrismThreadForMessageRemoteTransport ()
{
    //return (getPrismThreadForServiceId (InterLocationMessageTransportObjectManager::getPrismServiceId ()));
    return (InterLocationMessageTransportObjectManager::getPrismThread ());
}

PrismThread *PrismThread::getPrismThreadForMessageHaPeerTransport ()
{
    return (HaPeerMessageTransportObjectManager::getPrismThread ());
}

UI32 PrismThread::getNumberOfPendingTimerExpirationMessages ()
{
    // The message queues are internally protected by locks.  We need not lock them explicitly here.

    return (m_timerExpirations.getSize ());
}

UI32 PrismThread::getNumberOfPendingNormalMessages ()
{
    // The message queues are internally protected by locks.  We need not lock them explicitly here.

    return (m_messages.getSize ());
}

UI32 PrismThread::getNumberOfPendingHighPriorityMessages ()
{
    // The message queues are internally protected by locks.  We need not lock them explicitly here.

    return (m_highPriorityMessages.getSize ());
}

void PrismThread::getListOfServiceIds (vector<PrismServiceId> &serviceIds)
{
    if (NULL != s_pPrismServiceMap)
    {
        s_pPrismServiceMap->getListOfServiceIds (serviceIds);
    }
}

void PrismThread::emptyIncomingMessageQueuesForDisable (vector<PrismMessage *> &incomingMessages)
{
    PrismMessage *pPrismMessage = NULL;

    m_gateKeeper.lock ();

    while (NULL != (pPrismMessage = m_timerExpirations.removeAndGetFromFront ()))
    {
        incomingMessages.push_back (pPrismMessage);
    }

    while (NULL != (pPrismMessage = m_highPriorityMessages.removeAndGetFromFront ()))
    {
        incomingMessages.push_back (pPrismMessage);
    }

    while (NULL != (pPrismMessage = m_messages.removeAndGetFromFront ()))
    {
        incomingMessages.push_back (pPrismMessage);
    }

    while (NULL != (pPrismMessage = m_events.removeAndGetFromFront ()))
    {
        incomingMessages.push_back (pPrismMessage);
    }

    m_gateKeeper.unlock ();
}

PrismThreadId PrismThread::getSelf ()
{
    return (pthread_self ());
}

vector<UI32> PrismThread::getCpuAffinityVector () const
{
    return (m_cpuAffinityVector);
}

void PrismThread::setCpuAffinity (const vector<UI32> &cpuAffinityVector)
{
    m_cpuAffinityVector = cpuAffinityVector;

    UI32         numberOfCpusInCpuAffinityVector = m_cpuAffinityVector.size ();
    UI32         i                               = 0;
    UI32         numberOfCpus                    = FrameworkToolKit::getNumberOfCpus ();
    string       cpuAffinityString;
    vector<UI32> effectiveCpuAffinityVector;
    cpu_set_t    cpuSet;
    UI32         numberOfCpusInSet               = sizeof (cpuSet);

    if (0 != numberOfCpusInCpuAffinityVector)
    {
        CPU_ZERO (&cpuSet);

        for (i = 0; i < numberOfCpusInCpuAffinityVector; i++)
        {
            prismAssert (m_cpuAffinityVector[i] < numberOfCpus, __FILE__, __LINE__);

            CPU_SET (m_cpuAffinityVector[i], &cpuSet);

            cpuAffinityString = cpuAffinityString + m_cpuAffinityVector[i] + string (":");
        }

        if (0 < (m_pWaveObjectManagers.size ()))
        {
            trace (TRACE_LEVEL_DEBUG, "PrismThread::setCpuAffinity : Enabling CPU Affinity for Service : " + m_pWaveObjectManagers[0]->getName () + ", Affinity : " + cpuAffinityString);
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "PrismThread::setCpuAffinity : Enabling CPU Affinity for this thread.");
        }

        int setAffinityStatus = sched_setaffinity (syscall (SYS_gettid), sizeof (cpuSet), &cpuSet);

        if (0 < (m_pWaveObjectManagers.size ()))
        {
            if (0 == setAffinityStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "SUCCESSFULLY set CPU Affinity for Service : " + m_pWaveObjectManagers[0]->getName ());
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "Failed to set CPU Affinity for Service : " + m_pWaveObjectManagers[0]->getName ());
            }
        }
        else
        {
            if (0 == setAffinityStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "SUCCESSFULLY set CPU Affinity for this thread.");
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "Failed to set CPU Affinity for this thread.");
            }
        }
    }

    int getAffinityStatus = sched_getaffinity (syscall (SYS_gettid), sizeof (cpuSet), &cpuSet);

    if (0 < (m_pWaveObjectManagers.size ()))
    {
        if (0 == getAffinityStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "SUCCESSFULLY get CPU Affinity for Service : " + m_pWaveObjectManagers[0]->getName ());

            for (i = 0; i < numberOfCpusInSet; i++)
            {
                if (CPU_ISSET (i, &cpuSet))
                {
                    effectiveCpuAffinityVector.push_back (i);
                }
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "Failed to get CPU Affinity for Service : " + m_pWaveObjectManagers[0]->getName ());
        }
    }
    else
    {
        if (0 == getAffinityStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "SUCCESSFULLY get CPU Affinity for this thread.");

            for (i = 0; i < numberOfCpusInSet; i++)
            {
                if (CPU_ISSET (i, &cpuSet))
                {
                    effectiveCpuAffinityVector.push_back (i);
                }
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "Failed to get CPU Affinity for this thread.");
        }
    }

    m_cpuAffinityVector = effectiveCpuAffinityVector;
}

WaveObjectManager *PrismThread::getWaveObjectManagerForCurrentThread ()
{
    WaveObjectManager *pWaveObjectManager = NULL;

    m_prismThreadIdToWaveObjectManagerMapMutex.lock ();

    map<PrismThreadId, WaveObjectManager *>::iterator element    = m_prismThreadIdToWaveObjectManagerMap.find (pthread_self ());
    map<PrismThreadId, WaveObjectManager *>::iterator endElement = m_prismThreadIdToWaveObjectManagerMap.end ();

    if (endElement != element)
    {
        pWaveObjectManager = element->second;
    }
    else
    {
        pWaveObjectManager = ReservedWaveLocalObjectManager::getInstance ();
    }

    m_prismThreadIdToWaveObjectManagerMapMutex.unlock ();

    return (pWaveObjectManager);
}

PrismServiceId PrismThread::getPrismServiceIdForCurrentThread ()
{
    WaveObjectManager *pWaveObjectManager = NULL;
    PrismServiceId     prismServiceId;

    m_prismThreadIdToWaveObjectManagerMapMutex.lock ();

    map<PrismThreadId, WaveObjectManager *>::iterator element    = m_prismThreadIdToWaveObjectManagerMap.find (pthread_self ());
    map<PrismThreadId, WaveObjectManager *>::iterator endElement = m_prismThreadIdToWaveObjectManagerMap.end ();

    if (endElement != element)
    {
        pWaveObjectManager = element->second;
    }
    else
    {
        pWaveObjectManager = ReservedWaveLocalObjectManager::getInstance ();
    }

    prismAssert (NULL != pWaveObjectManager, __FILE__, __LINE__);

    prismServiceId = pWaveObjectManager->getServiceId ();

    m_prismThreadIdToWaveObjectManagerMapMutex.unlock ();

    return (prismServiceId);
}

void PrismThread::requestForThreadTermination ()
{
    m_terminateThread = true;
}

}
