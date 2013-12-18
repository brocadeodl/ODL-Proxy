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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Persistence/Test/PersistenceTestObjectManager.h"
#include "Framework/Utils/AssertUtils.h"
#include "Regression/RegressionTestObjectManager.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/Test/FrameworkTestabilityMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Regression/RegressionTestMessage.h"

namespace WaveNs
{

PersistenceTestObjectManager::PersistenceTestObjectManager ()
    : PrismTestObjectManager ("Persistence Test")
{
}

PersistenceTestObjectManager::~PersistenceTestObjectManager ()
{
}

PersistenceTestObjectManager * PersistenceTestObjectManager::getInstance ()
{
    static PersistenceTestObjectManager *pPersistenceTestObjectManager = new PersistenceTestObjectManager ();

    WaveNs::prismAssert (NULL != pPersistenceTestObjectManager, __FILE__, __LINE__);

    return (pPersistenceTestObjectManager);
}

PrismServiceId PersistenceTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

void PersistenceTestObjectManager::testRequestHandler (RegressionTestMessage *pRegressionTestMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceTestObjectManager::simpleTransactionForObjectCreationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceTestObjectManager::nestedTransactionForObjectCreationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&PersistenceTestObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pRegressionTestMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();
}

void PersistenceTestObjectManager::simpleTransactionForObjectCreationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_DEVEL, "PersistenceTestObjectManager::simpleTransactionForObjectCreationStep : Entering ...");

    FrameworkTestabilityMessage8 message, pageQueryMessage;
    UI32                         numberOfObjectsToBeCreated = 1000;

    //To do:- Make the page size configurable
    UI32                         resultSizeOfPagedQuery = 23;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfObjectsToBeCreated = atoi (m_regressionInput[0].c_str ());
    }

    message.setNumberOfObjectsToBeCreated (numberOfObjectsToBeCreated);

    ResourceId status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "PersistenceTestObjectManager::simpleTransactionForObjectCreationStep : Failed to send message to test Persistence.  Status : " + FrameworkToolKit::localize (status));

        pPrismLinearSequencerContext->executeNextStep (status);
        return;
    }
    else
    {
        status = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PersistenceTestObjectManager::simpleTransactionForObjectCreationStep : Persistence Test failed.  Completion Status : " + FrameworkToolKit::localize (status));

            pPrismLinearSequencerContext->executeNextStep (status);
            return;
        }
    }


//Paged query testing
    if (0 != (m_regressionInput.size ()))
    {
        numberOfObjectsToBeCreated = atoi (m_regressionInput[0].c_str ());
    }

    pageQueryMessage.setNumberOfObjectsToBeCreated (numberOfObjectsToBeCreated);
    pageQueryMessage.setPageSize(resultSizeOfPagedQuery);		
	

    status = sendSynchronously (&pageQueryMessage);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "PersistenceTestObjectManager::simpleTransactionForObjectCreationStep : Failed to send paged message to test Persistence.  Status : " + FrameworkToolKit::localize (status));
    }
    else
    {
        status = pageQueryMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "PersistenceTestObjectManager::simpleTransactionForObjectCreationStep : Persistence paged Test failed.  Completion Status : " + FrameworkToolKit::localize (status));
        }
    }


    pPrismLinearSequencerContext->executeNextStep (status);
}

void PersistenceTestObjectManager::nestedTransactionForObjectCreationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Starting Simple Nested Transaction Test. More nesting ");

    UI32               numberOfMessagesToSend = 1000;
    UI32               i                      = 0;
    WaveMessageStatus status                 = WAVE_MESSAGE_ERROR;

    if (0 != (m_regressionInput.size ()))
    {
        numberOfMessagesToSend = atoi (m_regressionInput[0].c_str ());
    }

    trace (TRACE_LEVEL_INFO, string ("    Sending ") + numberOfMessagesToSend + " messages.");

    for (i = 0; i < numberOfMessagesToSend; i++)
    {
        //trace (TRACE_LEVEL_INFO, "PersistenceTestObjectManager::nestedTransactionForObjectCreationStep : New ");

        FrameworkTestabilityMessage9 *pMessage = new FrameworkTestabilityMessage9 ();

        status = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, string ("PersistenceTestObjectManager::nestedTransactionForObjectCreationStep : Sending a message to [") + PrismThread::getPrismServiceNameForServiceId (pMessage->getSenderServiceCode ()) + " service] failed.");
            delete pMessage;
        }
    }

    pPrismLinearSequencerContext->executeNextStep (status);
}

}
