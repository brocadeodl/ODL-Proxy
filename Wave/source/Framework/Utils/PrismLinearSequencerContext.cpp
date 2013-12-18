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

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

using namespace std;

namespace WaveNs
{

PrismLinearSequencerContext::PrismLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
{
    m_pPrismMessage                              = pPrismMessage;
    m_pPrismAsynchronousContext                  = NULL;
    m_pPrismElement                              = pPrismElement;
    m_pSteps                                     = NULL;
    m_numberOfSteps                              = numberOfSteps;
    m_currentStep                                = 0;
    m_numberOfCallbacksBeforeAdvancingToNextStep = 0;
    m_isHoldAllRequested                         = false;
    m_isTransactionStartedByMe                   = false;
    m_numberOfFailures                           = 0;
    m_pQueryResults                              = NULL;
    m_isADelayedCommitTransaction                = false;
    m_completionStatus                           = WAVE_MESSAGE_ERROR;

    m_returnValue                                = -1;
    m_returnValueR                               = -1;
    m_clockId                                    = 0;

    if (3 > m_numberOfSteps)
    {
        cerr << "PrismLinearSequencerContext::PrismLinearSequencerContext : There should be atleast three steps to run a Prism Linear Sequencer. " << numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pSteps = new PrismLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismLinearSequencerContext::PrismLinearSequencerContext : Could not allocate memory for steps." << endl;
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

PrismLinearSequencerContext::PrismLinearSequencerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
{
    m_pPrismMessage                              = NULL;
    m_pPrismAsynchronousContext                  = pPrismAsynchronousContext;
    m_pPrismElement                              = pPrismElement;
    m_pSteps                                     = NULL;
    m_numberOfSteps                              = numberOfSteps;
    m_currentStep                                = 0;
    m_numberOfCallbacksBeforeAdvancingToNextStep = 0;
    m_isHoldAllRequested                         = false;
    m_isTransactionStartedByMe                   = false;
    m_numberOfFailures                           = 0;
    m_pQueryResults                              = NULL;
    m_isADelayedCommitTransaction                = false;
    m_completionStatus                           = WAVE_MESSAGE_ERROR;

    m_returnValue                                = -1;
    m_returnValueR                               = -1;
    m_clockId                                    = 0;

    if (3 > m_numberOfSteps)
    {
        cerr << "PrismLinearSequencerContext::PrismLinearSequencerContext : There should be atleast three steps to run a Prism Linear Sequencer. " << numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pSteps = new PrismLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismLinearSequencerContext::PrismLinearSequencerContext : Could not allocate memory for steps." << endl;
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

PrismLinearSequencerContext::PrismLinearSequencerContext (const PrismLinearSequencerContext &prismLinearSequencerContext)
{
    m_pPrismMessage                              = prismLinearSequencerContext.m_pPrismMessage;
    m_pPrismAsynchronousContext                  = prismLinearSequencerContext.m_pPrismAsynchronousContext;
    m_pPrismElement                              = prismLinearSequencerContext.m_pPrismElement;
    m_pSteps                                     = prismLinearSequencerContext.m_pSteps;
    m_numberOfSteps                              = prismLinearSequencerContext.m_numberOfSteps;
    m_currentStep                                = prismLinearSequencerContext.m_currentStep;
    m_numberOfCallbacksBeforeAdvancingToNextStep = prismLinearSequencerContext.m_numberOfCallbacksBeforeAdvancingToNextStep;
    m_isHoldAllRequested                         = prismLinearSequencerContext.m_isHoldAllRequested;
    m_isTransactionStartedByMe                   = prismLinearSequencerContext.m_isTransactionStartedByMe;
    m_numberOfFailures                           = prismLinearSequencerContext.m_numberOfFailures;
    m_pQueryResults                              = prismLinearSequencerContext.m_pQueryResults;
    m_isADelayedCommitTransaction                = prismLinearSequencerContext.m_isADelayedCommitTransaction;
    m_completionStatus                           = prismLinearSequencerContext.m_completionStatus;

    m_returnValue                                = -1;
    m_returnValueR                               = -1;
    m_clockId                                    = 0;


    if (3 > m_numberOfSteps)
    {
        cerr << "PrismLinearSequencerContext::PrismLinearSequencerContext : There should be atleast three steps to run a Prism Linear Sequencer. " << m_numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    m_pSteps = new PrismLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismLinearSequencerContext::PrismLinearSequencerContext : Could not allocate memory for steps." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    UI32 i = 0;

    for (i = 0; i < m_numberOfSteps; i++)
    {
        m_pSteps[i] = prismLinearSequencerContext.m_pSteps[i];
    }

    return;
}

PrismLinearSequencerContext::~PrismLinearSequencerContext ()
{
    if (NULL != m_pSteps)
    {
        delete[] m_pSteps;
    }

    if (NULL != m_pQueryResults)
    {
        UI32 numberOfQueryResults = m_pQueryResults->size ();
        UI32 i                    = 0;

        for (i = 0; i < numberOfQueryResults; i++)
        {
            delete ((*m_pQueryResults)[i]);
        }

        m_pQueryResults->clear ();
        delete m_pQueryResults;
    }

    garbageCollect ();
}

PrismLinearSequencerContext &PrismLinearSequencerContext::operator = (const PrismLinearSequencerContext &prismLinearSequencerContext)
{
    m_pPrismMessage                              = prismLinearSequencerContext.m_pPrismMessage;
    m_pPrismAsynchronousContext                  = prismLinearSequencerContext.m_pPrismAsynchronousContext;
    m_pPrismElement                              = prismLinearSequencerContext.m_pPrismElement;
    m_pSteps                                     = prismLinearSequencerContext.m_pSteps;
    m_numberOfSteps                              = prismLinearSequencerContext.m_numberOfSteps;
    m_currentStep                                = prismLinearSequencerContext.m_currentStep;
    m_numberOfCallbacksBeforeAdvancingToNextStep = prismLinearSequencerContext.m_numberOfCallbacksBeforeAdvancingToNextStep;
    m_isHoldAllRequested                         = prismLinearSequencerContext.m_isHoldAllRequested;
    m_isTransactionStartedByMe                   = prismLinearSequencerContext.m_isTransactionStartedByMe;
    m_numberOfFailures                           = prismLinearSequencerContext.m_numberOfFailures;
    m_isADelayedCommitTransaction                = prismLinearSequencerContext.m_isADelayedCommitTransaction;
    m_pQueryResults                              = prismLinearSequencerContext.m_pQueryResults;

    m_returnValue                                = -1;
    m_returnValueR                               = -1;
    m_clockId                                    = 0;


    if (3 > m_numberOfSteps)
    {
        cerr << "PrismLinearSequencerContext::operator = : There should be atleast three steps to run a Prism Linear Sequencer. " << m_numberOfSteps << " Steps were specified." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (*this);
    }

    m_pSteps = new PrismLinearSequencerStep[m_numberOfSteps];

    if (NULL == m_pSteps)
    {
        cerr << "PrismLinearSequencerContext::operator = : Could not allocate memory for steps." << endl;
        prismAssert (false, __FILE__, __LINE__);
        return (*this);
    }

    UI32 i = 0;

    for (i = 0; i < m_numberOfSteps; i++)
    {
        m_pSteps[i] = prismLinearSequencerContext.m_pSteps[i];
    }

    return (*this);
}

void PrismLinearSequencerContext::advanceCurrentStep ()
{
    m_currentStep++;
}

void PrismLinearSequencerContext::executeCurrentStep ()
{
    if (m_currentStep < (m_numberOfSteps - 2))
    {
        if ((0 != m_clockId) && (NULL != m_pPrismMessage) && (NULL != m_pPrismElement))
        {
            m_operationCode = m_pPrismMessage->getOperationCode ();
            m_returnValue   = clock_gettime (m_clockId,      &m_ts1);
            m_returnValueR  = clock_gettime (CLOCK_REALTIME, &m_tsr1);
        }
    }

    (m_pPrismElement->*(m_pSteps[m_currentStep])) (this);
}

ResourceId PrismLinearSequencerContext::getCompletionStatus ()
{
    return (m_completionStatus);
}

void PrismLinearSequencerContext::start ()
{
    SI32 returnValue = pthread_getcpuclockid (pthread_self (), &m_clockId);

    if (0 != returnValue)
    {
        m_clockId = 0;

        cerr << "Could not get Clock ID for this thread." << endl;
    }

    m_returnValue  = -1;
    m_returnValueR = -1;

    executeCurrentStep ();
}

/**
 * Note, make sure that a call to this function is followed by a return.
 * There should be absolutely no call to "executeNextStep" after calling this function.
 */
void PrismLinearSequencerContext::executeSuccessStep ()
{
    if (m_currentStep >= (m_numberOfSteps - 2))
    {
        cerr << "PrismLinearSequencerContext::jumpToSuccessStep : Invalid state for this operation. Step (" << m_currentStep << " / " << m_numberOfSteps << ")" << endl;
        executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }

    // Move step pointer to success step, set status to SUCCSSS, and then just execute the step.
    m_currentStep = m_numberOfSteps - 2;
    m_completionStatus = WAVE_MESSAGE_SUCCESS;

    executeCurrentStep ();
    return;
}

void PrismLinearSequencerContext::executeNextStep (const ResourceId &currentStepStatus)
{
    UI32     numberOfSeconds     = 0;
    SI32     numberOfNanoSeconds = 0;

    if (0 != m_numberOfCallbacksBeforeAdvancingToNextStep)
    {
        return;
    }

    if (m_currentStep < (m_numberOfSteps - 2))
    {
        if (0 == m_returnValue)
        {
            m_returnValue = clock_gettime (m_clockId, &m_ts2);

            if (0 == m_returnValue)
            {
                numberOfSeconds     = m_ts2.tv_sec  - m_ts1.tv_sec;
                numberOfNanoSeconds = m_ts2.tv_nsec - m_ts1.tv_nsec;

                m_pPrismElement->updateTimeConsumedInThisThread (m_operationCode, m_currentStep, numberOfSeconds, numberOfNanoSeconds);
            }
        }

        if (0 == m_returnValueR)
        {
            m_returnValueR = clock_gettime (CLOCK_REALTIME, &m_tsr2);

            if (0 == m_returnValueR)
            {
                numberOfSeconds     = m_tsr2.tv_sec  - m_tsr1.tv_sec;
                numberOfNanoSeconds = m_tsr2.tv_nsec - m_tsr1.tv_nsec;

                m_pPrismElement->updateRealTimeConsumedInThisThread (m_operationCode, m_currentStep, numberOfSeconds, numberOfNanoSeconds);
            }
        }
    }

    m_completionStatus = currentStepStatus;

    advanceCurrentStep ();

    if ((m_numberOfSteps - 2) < m_currentStep)
    {
        cerr << "PrismLinearSequencerContext::executeNextStep : Trying to execute beyond the end of the sequencer. (" << m_currentStep << " / " << m_numberOfSteps << ")" << endl;
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    // FIXME : sagar : Handle intermediate step failures.

    if (WAVE_MESSAGE_SUCCESS == m_completionStatus)
    {
        executeCurrentStep ();
        return;
    }
    else
    {
        m_currentStep = m_numberOfSteps - 1;
        executeCurrentStep ();
        return;
    }
}

PrismMessage *PrismLinearSequencerContext::getPPrismMessage ()
{
    return (m_pPrismMessage);
}

void PrismLinearSequencerContext::setPPrismMessage (PrismMessage *pPrismMessage)
{
    m_pPrismMessage = pPrismMessage;
}

PrismAsynchronousContext *PrismLinearSequencerContext::getPPrismAsynchronousContext ()
{
    return (m_pPrismAsynchronousContext);
}

PrismLinearSequencerContext &PrismLinearSequencerContext::operator ++ ()
{
    ++m_numberOfCallbacksBeforeAdvancingToNextStep;

    return (*this);
}

PrismLinearSequencerContext &PrismLinearSequencerContext::operator -- ()
{
    --m_numberOfCallbacksBeforeAdvancingToNextStep;

    return (*this);
}

UI32 PrismLinearSequencerContext::getNumberOfCallbacksBeforeAdvancingToNextStep ()
{
    return (m_numberOfCallbacksBeforeAdvancingToNextStep);
}

void PrismLinearSequencerContext::holdAll ()
{
    prismAssert (NULL != m_pPrismElement, __FILE__, __LINE__);

    m_pPrismElement->holdAll ();
    m_isHoldAllRequested = true;
}

void PrismLinearSequencerContext::unholdAll ()
{
    prismAssert (NULL != m_pPrismElement, __FILE__, __LINE__);

    m_pPrismElement->unholdAll ();
    m_isHoldAllRequested = false;
}

bool PrismLinearSequencerContext::getIsHoldAllRequested ()
{
    return (m_isHoldAllRequested);
}

bool PrismLinearSequencerContext::getIsTransactionStartedByMe () const
{
    return (m_isTransactionStartedByMe);
}

void PrismLinearSequencerContext::setIsTransactionStartedByMe (const bool &isTransactionStartedByMe)
{
    m_isTransactionStartedByMe = isTransactionStartedByMe;
}

UI32 PrismLinearSequencerContext::getNumberOfFailures () const
{
    return (m_numberOfFailures);
}

void PrismLinearSequencerContext::setNumberOfFailures (const UI32 &numberOfFailures)
{
    m_numberOfFailures = numberOfFailures;
}

void PrismLinearSequencerContext::incrementNumberOfFailures ()
{
    m_numberOfFailures++;
}

void PrismLinearSequencerContext::incrementNumberOfFailures (const UI32 &numberOfFailures)
{
    m_numberOfFailures += numberOfFailures;
}

vector<WaveManagedObject *> *PrismLinearSequencerContext::getPQueryResults () const
{
    return (m_pQueryResults);
}

void PrismLinearSequencerContext::setPQueryResults (vector<WaveManagedObject *> *pQueryResults)
{
    m_pQueryResults = pQueryResults;
}

void PrismLinearSequencerContext::addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &managedObjectsForGarbageCollection)
{
    m_managedObjectsForGarbageCollection.insert (m_managedObjectsForGarbageCollection.end (), managedObjectsForGarbageCollection.begin (), managedObjectsForGarbageCollection.end ());
}

void PrismLinearSequencerContext::addManagedObjectForGarbageCollection (WaveManagedObject *pWaveManagedObjectForGarbageCollection)
{
    prismAssert (NULL != pWaveManagedObjectForGarbageCollection, __FILE__, __LINE__);

    m_managedObjectsForGarbageCollection.push_back (pWaveManagedObjectForGarbageCollection);
}

void PrismLinearSequencerContext::garbageCollect ()
{
    UI32 numberOfManagedObjectsForGarbageCollection = m_managedObjectsForGarbageCollection.size ();
    UI32 i                                          = 0;

    for (i = 0; i < numberOfManagedObjectsForGarbageCollection; i++)
    {
        prismAssert (NULL != m_managedObjectsForGarbageCollection[i], __FILE__, __LINE__);

        delete (m_managedObjectsForGarbageCollection[i]);
    }
}

bool PrismLinearSequencerContext::getIsADelayedCommitTransaction () const
{
    return (m_isADelayedCommitTransaction);
}

void PrismLinearSequencerContext::setIsADelayedCommitTransaction (const bool &isADelayedCommitTransaction)
{
    m_isADelayedCommitTransaction = isADelayedCommitTransaction;
}

}
