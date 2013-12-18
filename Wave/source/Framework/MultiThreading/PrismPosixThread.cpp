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
 ***************************************************************************/
#include "Framework/MultiThreading/PrismPosixThread.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include <signal.h>
#include <errno.h>

using namespace std;

namespace WaveNs
{

#ifdef __ZVM__
static UI32 s_prismPosixThreadStackSize = PTHREAD_STACK_MIN * 4 * 32;
#else
static UI32 s_prismPosixThreadStackSize = PTHREAD_STACK_MIN * 16;
#endif

PrismPosixThread::PrismPosixThread (const UI32 &stackSize)
{
    m_pThreadId      = (pthread_t)0;
    m_pThreadContext = NULL;

    m_stackSize = stackSize;

    // Reset the stackSize accordingly

    if (0 == m_stackSize)
    {
        m_stackSize = s_prismPosixThreadStackSize; // The default Stack Size.
    }
}

PrismPosixThread::PrismPosixThread (const PrismPosixThread &prismPosixThread)
{
    WaveNs::trace (TRACE_LEVEL_FATAL, "PrismPosixThread::PrismPosixThread : Copy constructing a PrismPosixThread does not make sense and hence not allowed.");
    WaveNs::prismAssert (false, __FILE__, __LINE__);
}

PrismPosixThread::~PrismPosixThread ()
{
    PrismFrameworkObjectManager::removePrismThreadId (m_pThreadId);
}

PrismPosixThread &PrismPosixThread::operator = (const PrismPosixThread &prismPosixThread)
{
    WaveNs::trace (TRACE_LEVEL_FATAL, "PrismPosixThread::operator = : Assigning to a PrismPosixThread does not make sense and hence not allowed.");
    WaveNs::prismAssert (false, __FILE__, __LINE__);

    return (*this);
}

WaveThreadStatus PrismPosixThread::run ()
{
    static int signalMaskIndicator = 0;
           int status              = 0;

    // The following code massks the SIGALRM and SIGPIPE signals for the entire process.
    // This happens jut before the first ptheread gets created in the system and it only happens
    // once.  We make sure that this by using the signalMaskIndicator statuc variable.
    // On linux the oly way to make the sigwait to reliably work is to block the corresponding
    // sgnals for the entire process before any thread is crated in the system and them make a
    // particular thread wait for the signal using the sigwait call.  Refer to the "Threads and Signal handling"
    // section of the GNU libc manual located at the following URL:
    // http://www.gnu.org/software/libc/manual/html_mono/libc.html#Threads%20and%20Signal%20Handling
    // This works on GNU/Linux as well as Net BSD systems.

    prismInstallSigIntHandler ();

    if (0 == signalMaskIndicator)
    {
        prismBlockSignals ();

        signalMaskIndicator = 1;

    }

    // Create the thread in a detached state so that we do not encounter the atrificial limit on the number of threads created during entire life of the process.

    pthread_attr_t pthreadAttribute;

    status = pthread_attr_init (&pthreadAttribute);

    if (0 != status)
    {
        cerr << "Pthread attributes could not be initialized : Status : " << status << endl;
        return (WAVE_THREAD_COULD_NOT_CREATE);
    }

    status = pthread_attr_setdetachstate (&pthreadAttribute, PTHREAD_CREATE_DETACHED);

    if (0 != status)
    {
        cerr << "Pthread attribute for detach state could not be set : Status : " << status << endl;
        return (WAVE_THREAD_COULD_NOT_CREATE);
    }

    status = pthread_attr_setstacksize (&pthreadAttribute, m_stackSize);

    if (0 != status)
    {
        cerr << "Pthread attribute for set stack size could not be set : Status : " << status << endl;
        return (WAVE_THREAD_COULD_NOT_CREATE);
    }

    status = pthread_create (&m_pThreadId, &pthreadAttribute, reinterpret_cast<PosixThreadStartMethodType> (PrismPosixThread::pthreadStartMethod), reinterpret_cast<void **> (this));

    if (0 != status)
    {
        cerr << "pthread could not be created : errno : " << errno << ", Status : " << status << endl;
        return (WAVE_THREAD_COULD_NOT_CREATE);
    }
    else
    {
        PrismFrameworkObjectManager::addPrismThreadId (m_pThreadId);
        return (WAVE_THREAD_SUCCESS);
    }
}

WaveThreadStatus PrismPosixThread::joinCallingThread ()
{
    void *pContext = NULL;

    int status = pthread_join (m_pThreadId, &pContext);

    if (0 != status)
    {
        return (WAVE_THREAD_ERROR);
    }
    else
    {
        return (WAVE_THREAD_SUCCESS);
    }
}

PrismPosixThreadContext *PrismPosixThread::createThreadContext ()
{
    return (new PrismPosixThreadContext ());
}

const PrismPosixThreadContext *PrismPosixThread::getThreadContext()
{
    return (m_pThreadContext);
}

PrismThreadId PrismPosixThread::getId () const
{
    return (m_pThreadId);
}

PrismPosixThread *PrismPosixThread::pthreadStartMethod (PrismPosixThread *pPrismPoixThread)
{
    pPrismPoixThread->start ();
    // We will see the following output only if user has ovewritten the start and did not make calls to pthread_exit in the new definition of start.
    //cout << "Finished Executing the User defined behavior (user did not call pthread_exit)." << endl;
    return (pPrismPoixThread);
}

WaveThreadStatus PrismPosixThread::start ()
{
    cout << "Thread Started." << endl;
    cout << "We are not doing anything useful here." << endl;
    cout << "Ending the thread now." << endl;
    stopCallingThread ();
    // We should never see the following statement in the output as the above line ends the execution of the thread.
    cout << "Thread Ended." << endl;
    return (WAVE_THREAD_SUCCESS);
}

void PrismPosixThread::stopCallingThread ()
{
    pthread_exit (reinterpret_cast<void *> (this));
}

}
