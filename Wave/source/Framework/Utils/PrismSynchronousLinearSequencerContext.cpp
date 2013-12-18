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

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

#include <pthread.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;

namespace WaveNs
{

PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
{
    m_pPrismMessage                 = pPrismMessage;
    m_pPrismAsynchronousContext     = NULL;
    m_pPrismElement                 = pPrismElement;
    m_pSteps                        = NULL;
    m_numberOfSteps                 = numberOfSteps;
    m_currentStep                   = 0;
    m_isTransactionStartedByMe      = false;
    m_isADelayedCommitTransaction   = false;
    m_completionStatus              = WAVE_MESSAGE_ERROR;

    m_clockId                   = 0;

    if (3 > m_numberOfSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext : There should be atleast three steps to run a Prism Linear Sequencer. " << numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pSteps = new PrismSynchronousLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext : Could not allocate memory for steps." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    UI32 i = 0;

    for (i = 0; i < m_numberOfSteps; i++)
    {
        m_pSteps[i] = pSteps[i];
    }

    return;
}

PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
{
    m_pPrismMessage                 = NULL;
    m_pPrismAsynchronousContext     = pPrismAsynchronousContext;
    m_pPrismElement                 = pPrismElement;
    m_pSteps                        = NULL;
    m_numberOfSteps                 = numberOfSteps;
    m_currentStep                   = 0;
    m_isTransactionStartedByMe      = false;
    m_isADelayedCommitTransaction   = false;
    m_completionStatus              = WAVE_MESSAGE_ERROR;

    m_clockId                       = 0;

    if (3 > m_numberOfSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext : There should be atleast three steps to run a Prism Linear Sequencer. " << numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pSteps = new PrismSynchronousLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext : Could not allocate memory for steps." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    UI32 i = 0;

    for (i = 0; i < m_numberOfSteps; i++)
    {
        m_pSteps[i] = pSteps[i];
    }

    return;
}

PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext (const PrismSynchronousLinearSequencerContext &prismSynchronousLinearSequencerContext)
{
    m_pPrismMessage                 = prismSynchronousLinearSequencerContext.m_pPrismMessage;
    m_pPrismAsynchronousContext     = prismSynchronousLinearSequencerContext.m_pPrismAsynchronousContext;
    m_pPrismElement                 = prismSynchronousLinearSequencerContext.m_pPrismElement;
    m_pSteps                        = prismSynchronousLinearSequencerContext.m_pSteps;
    m_numberOfSteps                 = prismSynchronousLinearSequencerContext.m_numberOfSteps;
    m_currentStep                   = prismSynchronousLinearSequencerContext.m_currentStep;
    m_isTransactionStartedByMe      = prismSynchronousLinearSequencerContext.m_isTransactionStartedByMe;
    m_isADelayedCommitTransaction   = prismSynchronousLinearSequencerContext.m_isADelayedCommitTransaction;
    m_completionStatus              = WAVE_MESSAGE_ERROR;

    m_clockId                       = 0;

    if (3 > m_numberOfSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext : There should be atleast three steps to run a Prism Linear Sequencer. " << m_numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pSteps = new PrismSynchronousLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::PrismSynchronousLinearSequencerContext : Could not allocate memory for steps." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    UI32 i = 0;

    for (i = 0; i < m_numberOfSteps; i++)
    {
        m_pSteps[i] = prismSynchronousLinearSequencerContext.m_pSteps[i];
    }

    return;
}

PrismSynchronousLinearSequencerContext::~PrismSynchronousLinearSequencerContext ()
{
    if (NULL != m_pSteps)
    {
        delete[] m_pSteps;
    }

    garbageCollect ();
}

PrismSynchronousLinearSequencerContext &PrismSynchronousLinearSequencerContext::operator = (const PrismSynchronousLinearSequencerContext &prismSynchronousLinearSequencerContext)
{
    m_pPrismMessage                 = prismSynchronousLinearSequencerContext.m_pPrismMessage;
    m_pPrismAsynchronousContext     = prismSynchronousLinearSequencerContext.m_pPrismAsynchronousContext;
    m_pPrismElement                 = prismSynchronousLinearSequencerContext.m_pPrismElement;
    m_pSteps                        = prismSynchronousLinearSequencerContext.m_pSteps;
    m_numberOfSteps                 = prismSynchronousLinearSequencerContext.m_numberOfSteps;
    m_currentStep                   = prismSynchronousLinearSequencerContext.m_currentStep;
    m_isTransactionStartedByMe      = prismSynchronousLinearSequencerContext.m_isTransactionStartedByMe;
    m_isADelayedCommitTransaction   = prismSynchronousLinearSequencerContext.m_isADelayedCommitTransaction;
    m_completionStatus              = prismSynchronousLinearSequencerContext.m_completionStatus;

    m_clockId                       = 0;

    if (3 > m_numberOfSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::operator = : There should be atleast three steps to run a Prism Linear Sequencer. " << m_numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (*this);
    }

    m_pSteps = new PrismSynchronousLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismSynchronousLinearSequencerContext::operator = : Could not allocate memory for steps." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (*this);
    }

    UI32 i = 0;

    for (i = 0; i < m_numberOfSteps; i++)
    {
        m_pSteps[i] = prismSynchronousLinearSequencerContext.m_pSteps[i];
    }

    return (*this);
}

void PrismSynchronousLinearSequencerContext::advanceCurrentStep ()
{
    m_currentStep++;
}

ResourceId PrismSynchronousLinearSequencerContext::executeCurrentStep ()
{
    struct timespec ts1                 = {0};
    struct timespec ts2                 = {0};
    struct timespec tsr1                = {0};
    struct timespec tsr2                = {0};
            SI32    returnValue         = -1;
            SI32    returnValueR        = -1;
            UI32    operationCode       = 0;
            UI32    numberOfSeconds     = 0;
            SI32    numberOfNanoSeconds = 0;
            UI32    currentStep         = m_currentStep;
            UI32    numberOfSteps       = m_numberOfSteps;

    if (currentStep < (numberOfSteps - 2))
    {
        if ((0 != m_clockId) && (NULL != m_pPrismMessage) && (NULL != m_pPrismElement))
        {
            operationCode = m_pPrismMessage->getOperationCode ();

            returnValue   = clock_gettime (m_clockId,      &ts1);
            returnValueR  = clock_gettime (CLOCK_REALTIME, &tsr1);
        }
    }


    ResourceId status = (m_pPrismElement->*(m_pSteps[currentStep])) (this);

    if (currentStep < (numberOfSteps - 2))
    {
        if (0 == returnValue)
        {
            returnValue = clock_gettime (m_clockId, &ts2);

            if (0 == returnValue)
            {
                numberOfSeconds     = ts2.tv_sec  - ts1.tv_sec;
                numberOfNanoSeconds = ts2.tv_nsec - ts1.tv_nsec;

                m_pPrismElement->updateTimeConsumedInThisThread (operationCode, currentStep, numberOfSeconds, numberOfNanoSeconds);
            }
        }

        if (0 == returnValueR)
        {
            returnValueR = clock_gettime (CLOCK_REALTIME, &tsr2);

            if (0 == returnValueR)
            {
                numberOfSeconds     = tsr2.tv_sec  - tsr1.tv_sec;
                numberOfNanoSeconds = tsr2.tv_nsec - tsr1.tv_nsec;

                m_pPrismElement->updateRealTimeConsumedInThisThread (operationCode, currentStep, numberOfSeconds, numberOfNanoSeconds);
            }
        }
    }

    return (status);
}

ResourceId PrismSynchronousLinearSequencerContext::getCompletionStatus ()
{
    return (m_completionStatus);
}

ResourceId PrismSynchronousLinearSequencerContext::execute ()
{
    UI32 i;
    UI32 effectiveNumberOfSteps = m_numberOfSteps - 2;
    SI32 returnValue            = 0;

    m_completionStatus = WAVE_MESSAGE_ERROR;

    returnValue = pthread_getcpuclockid (pthread_self (), &m_clockId);

    if (0 != returnValue)
    {
        m_clockId = 0;

        cerr << "Could not get Clock ID for this thread." << endl;
    }

    for (i = 0; i < effectiveNumberOfSteps; i++)
    {
        m_completionStatus = executeCurrentStep ();

        if (WAVE_MESSAGE_SUCCESS != m_completionStatus)
        {
            if (WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP == m_completionStatus)
            {
                // Move step pointer to the success step
                m_currentStep = effectiveNumberOfSteps;
                m_completionStatus = WAVE_MESSAGE_SUCCESS;
            }

            break;
        }
        else
        {
            advanceCurrentStep ();
        }
    }

    if (WAVE_MESSAGE_SUCCESS != m_completionStatus)
    {
        m_currentStep = m_numberOfSteps - 1;
    }

    return (executeCurrentStep ());
}

PrismMessage *PrismSynchronousLinearSequencerContext::getPPrismMessage ()
{
    return (m_pPrismMessage);
}

void PrismSynchronousLinearSequencerContext::setPPrismMessage (PrismMessage *pPrismMessage)
{
    m_pPrismMessage = pPrismMessage;
}

PrismAsynchronousContext *PrismSynchronousLinearSequencerContext::getPPrismAsynchronousContext ()
{
    return (m_pPrismAsynchronousContext);
}

bool PrismSynchronousLinearSequencerContext::getIsTransactionStartedByMe () const
{
    return (m_isTransactionStartedByMe);
}

void PrismSynchronousLinearSequencerContext::setIsTransactionStartedByMe (const bool &isTransactionStartedByMe)
{
    m_isTransactionStartedByMe = isTransactionStartedByMe;
}

void PrismSynchronousLinearSequencerContext::addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &managedObjectsForGarbageCollection)
{
    m_managedObjectsForGarbageCollection.insert (m_managedObjectsForGarbageCollection.end (), managedObjectsForGarbageCollection.begin (), managedObjectsForGarbageCollection.end ());
}

void PrismSynchronousLinearSequencerContext::addManagedObjectForGarbageCollection (WaveManagedObject *pWaveManagedObjectForGarbageCollection)
{
    prismAssert (NULL != pWaveManagedObjectForGarbageCollection, __FILE__, __LINE__);

    m_managedObjectsForGarbageCollection.push_back (pWaveManagedObjectForGarbageCollection);
}

void PrismSynchronousLinearSequencerContext::garbageCollect ()
{
    UI32 numberOfManagedObjectsForGarbageCollection = m_managedObjectsForGarbageCollection.size ();
    UI32 i                                          = 0;

    for (i = 0; i < numberOfManagedObjectsForGarbageCollection; i++)
    {
        prismAssert (NULL != m_managedObjectsForGarbageCollection[i], __FILE__, __LINE__);

        delete (m_managedObjectsForGarbageCollection[i]);
    }
}

bool PrismSynchronousLinearSequencerContext::getIsADelayedCommitTransaction () const
{
    return (m_isADelayedCommitTransaction);
}
 
void PrismSynchronousLinearSequencerContext::setIsADelayedCommitTransaction (const bool &isADelayedCommitTransaction)
{
    m_isADelayedCommitTransaction = isADelayedCommitTransaction;
}

}
