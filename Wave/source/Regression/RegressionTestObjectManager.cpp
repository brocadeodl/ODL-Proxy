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

#include "Regression/RegressionTestObjectManager.h"
#include "Regression/RegressionTestMessage.h"
#include "Regression/RegressionServiceMessages.h"
#include "Regression/RegressionPrepareMessage2.h"
#include "Regression/RegressionTestTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Core/Test/FrameworkCoreTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager.h"
#include "Framework/Messaging/Local/Test/FrameworkLocalMessagingTestObjectManager2.h"
#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager.h"
#include "Framework/Messaging/Remote/Test/FrameworkRemoteMessagingTestObjectManager2.h"
#include "Framework/Trace/Test/TraceTestObjectManager.h"
#include "Cluster/Test/ClusterTestObjectManager.h"
#include "Framework/Timer/Test/TimerTestObjectManager.h"

namespace WaveNs
{

UI32 messageCount = 0;

RegressionTestObjectManager::RegressionTestObjectManager ()
    : WaveLocalObjectManager (getServiceName ())
{
    addOperationMap (REGRESSION_GET_TEST_SERVICE_ENTRIES,   reinterpret_cast<PrismMessageHandler> (&RegressionTestObjectManager::getTestServiceEntriesMessageHandler));
    addOperationMap (REGRESSION_SET_TEST_SERVICE_STATE,     reinterpret_cast<PrismMessageHandler> (&RegressionTestObjectManager::setTestServiceStateMessageHandler));
    addOperationMap (REGRESSION_START_REGRESSION,           reinterpret_cast<PrismMessageHandler> (&RegressionTestObjectManager::startRegressionMessageHandler));
    addOperationMap (REGRESSION_PREPARE_TEST_FOR_A_SERVICE, reinterpret_cast<PrismMessageHandler> (&RegressionTestObjectManager::prepareAServiceTestMessageHandler));
    addOperationMap (REGRESSION_RUN_TEST_FOR_A_SERVICE,     reinterpret_cast<PrismMessageHandler> (&RegressionTestObjectManager::runAServiceTestMessageHandler));
}

RegressionTestObjectManager *RegressionTestObjectManager::getInstance ()
{
    static RegressionTestObjectManager *pRegressionTestObjectManager = new RegressionTestObjectManager ();

    WaveNs::prismAssert (NULL != pRegressionTestObjectManager, __FILE__, __LINE__);

    return (pRegressionTestObjectManager);
}

PrismServiceId RegressionTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string RegressionTestObjectManager::getServiceName ()
{
    return ("Regression");
}

PrismMessage *RegressionTestObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case REGRESSION_GET_TEST_SERVICE_ENTRIES :
            pPrismMessage = new RegressionTestObjectManagerGetTestServiceEntriesMessage;
            break;

        case REGRESSION_SET_TEST_SERVICE_STATE :
            pPrismMessage = new RegressionTestObjectManagerSetTestServiceStateMessage;
            break;

        case REGRESSION_START_REGRESSION :
            pPrismMessage = new RegressionTestObjectManagerStartRegressionMessage;
            break;

        case REGRESSION_PREPARE_TEST_FOR_A_SERVICE :
            pPrismMessage = new RegressionTestObjectManagerPrepareTestForAServiceMessage;
            break;

        case REGRESSION_RUN_TEST_FOR_A_SERVICE :
            pPrismMessage = new RegressionTestObjectManagerRunTestForAServiceMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void RegressionTestObjectManager::getTestServiceEntriesMessageHandler (RegressionTestObjectManagerGetTestServiceEntriesMessage *pMessage)
{
    UI32 numberOfTestServices  = m_testServiceEntries.size ();
    UI32 i;

    for (i = 0; i < numberOfTestServices; i++)
    {
        pMessage->addTestServiceEntry (m_testServiceEntries[i].getTestServiceId (), m_testServiceEntries[i].getIsTestEnabled ());
    }

    pMessage->setNumberOfTestServiceEntries (numberOfTestServices);

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pMessage);
}

void RegressionTestObjectManager::setTestServiceStateMessageHandler (RegressionTestObjectManagerSetTestServiceStateMessage *pMessage)
{
    UI32         numberOfTestServices = m_testServiceEntries.size ();
    PrismServiceId testServiceId        = pMessage->getTestServiceId ();
    bool         isTestServiceEnabled = pMessage->getIsTestEnabled ();
    UI32         i;
    bool         foundTestService     = false;

    for (i = 0; i < numberOfTestServices; i++)
    {
        PrismServiceId tempTestServiceId = m_testServiceEntries[i].getTestServiceId ();

        if (testServiceId == tempTestServiceId)
        {
            m_testServiceEntries[i].setIsTestEnabled (isTestServiceEnabled);

            foundTestService = true;
            break;
        }
    }

    if (false == foundTestService)
    {
        pMessage->setCompletionStatus (REGRESSION_TEST_ERROR_INVALID_TEST_SERVICE_ID);
    }
    else
    {
        pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }

    reply (pMessage);
}

void RegressionTestObjectManager::startRegressionMessageHandler (RegressionTestObjectManagerStartRegressionMessage *pMessage)
{
    UI32 numberOfTestServices         = m_testServiceEntries.size ();
    UI32 numberOfTimesToRunRegression = pMessage->getNumberOfTimesToRunRegression ();
    UI32 i, j;

    for (j = 0; j < numberOfTimesToRunRegression; j++)
    {
        trace (TRACE_LEVEL_INFO, "");
        trace (TRACE_LEVEL_INFO, "");
        trace (TRACE_LEVEL_INFO, string ("Running Regression Iteration : ") + (j + 1));
        trace (TRACE_LEVEL_INFO, "");
        trace (TRACE_LEVEL_INFO, "");

        for (i = 0; i < numberOfTestServices; i++)
        {
            PrismServiceId testServiceId = m_testServiceEntries[i].getTestServiceId ();
            bool         isTestEnabled = m_testServiceEntries[i].getIsTestEnabled ();

            if (true == isTestEnabled)
            {
                RegressionTestMessage message (testServiceId);
                WaveMessageStatus      status;

                status = sendSynchronously (&message);

                if (WAVE_MESSAGE_SUCCESS != status)
                {
                    trace (TRACE_LEVEL_ERROR, "");
                    trace (TRACE_LEVEL_ERROR, string ("Test FAILED    for Service (") + FrameworkToolKit::getServiceNameById (testServiceId) + ").");
                    trace (TRACE_LEVEL_ERROR, "");
                }
                else
                {
                    ResourceId completionStatus = message.getCompletionStatus ();

                    if (WAVE_MESSAGE_SUCCESS != completionStatus)
                    {
                        trace (TRACE_LEVEL_ERROR, "");
                        trace (TRACE_LEVEL_ERROR, string ("Test FAILED    for Service (") + FrameworkToolKit::getServiceNameById (testServiceId) + ").");
                        trace (TRACE_LEVEL_ERROR, "");
                    }
                    else
                    {
                        trace (TRACE_LEVEL_SUCCESS, "");
                        trace (TRACE_LEVEL_SUCCESS, string ("Test SUCCEEDED for Service (") + FrameworkToolKit::getServiceNameById (testServiceId) + ").");
                        trace (TRACE_LEVEL_SUCCESS, "");
                    }
                }
            }
            else
            {
                trace (TRACE_LEVEL_WARN, "");
                trace (TRACE_LEVEL_WARN, string ("Test DISABLED for Service (") + FrameworkToolKit::getServiceNameById (testServiceId) + ").");
                trace (TRACE_LEVEL_WARN, "");
            }
        }
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void RegressionTestObjectManager::prepareAServiceTestMessageHandler (RegressionTestObjectManagerPrepareTestForAServiceMessage *pMessage)
{
    PrismServiceId serviceCode                 = pMessage->getServiceCode ();
    ResourceId     completionStatus            = WAVE_MESSAGE_SUCCESS;
    vector<string> inputStrings;
    UI32           argIndex;

    RegressionPrepareMessage2 message (serviceCode);
    WaveMessageStatus     status;

    inputStrings = pMessage->getInputStrings();    

    for (argIndex = 0; argIndex < (UI32) inputStrings.size(); argIndex++)
    {
        message.addInputString(inputStrings[argIndex]);
    }  

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "");
        trace (TRACE_LEVEL_ERROR, string ("Prepare FAILED    for Service (") + FrameworkToolKit::getServiceNameById (serviceCode) + ").");
        trace (TRACE_LEVEL_ERROR, "");

        completionStatus = status;
    }
    else
    {
        completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, "");
            trace (TRACE_LEVEL_ERROR, string ("Prepare FAILED    for Service (") + FrameworkToolKit::getServiceNameById (serviceCode) + ").");
            trace (TRACE_LEVEL_ERROR, "");
        }
        else
        {  
            trace (TRACE_LEVEL_SUCCESS, "");
            trace (TRACE_LEVEL_SUCCESS, string ("Prepare SUCCEEDED for Service (") + FrameworkToolKit::getServiceNameById (serviceCode) + ").");
            trace (TRACE_LEVEL_SUCCESS, "");
        }
    }

    pMessage->setCompletionStatus (completionStatus);
    reply (pMessage);
}

void RegressionTestObjectManager::runAServiceTestMessageHandler (RegressionTestObjectManagerRunTestForAServiceMessage *pMessage)
{
    PrismServiceId serviceCode                 = pMessage->getServiceCode ();
    UI32         numberOfTimesToRunServiceTest = pMessage->getNumberOfTimesToRunServiceTest ();
    UI32         i                             = 0;
    ResourceId   completionStatus              = WAVE_MESSAGE_SUCCESS;

    for (i = 0; i < numberOfTimesToRunServiceTest; i++)
    {
        trace (TRACE_LEVEL_INFO, "");
        trace (TRACE_LEVEL_INFO, "");
        trace (TRACE_LEVEL_INFO, string ("Running Service Test Iteration : ") + (i + 1));
        cout << "Running Service Test Iteration : " <<  (i + 1) << endl;
        trace (TRACE_LEVEL_INFO, "");
        trace (TRACE_LEVEL_INFO, "");

        RegressionTestMessage message (serviceCode);
        WaveMessageStatus     status;

        status = sendSynchronously (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "");
            trace (TRACE_LEVEL_ERROR, string ("Test FAILED    for Service (") + FrameworkToolKit::getServiceNameById (serviceCode) + ").");
            trace (TRACE_LEVEL_ERROR, "");

            completionStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, "");
                trace (TRACE_LEVEL_ERROR, string ("Test FAILED    for Service (") + FrameworkToolKit::getServiceNameById (serviceCode) + ").");
                trace (TRACE_LEVEL_ERROR, "");
            }
            else
            {
                trace (TRACE_LEVEL_SUCCESS, "");
                trace (TRACE_LEVEL_SUCCESS, string ("Test SUCCEEDED for Service (") + FrameworkToolKit::getServiceNameById (serviceCode) + ").");
                trace (TRACE_LEVEL_SUCCESS, "");
            }
        }
    }

    pMessage->setCompletionStatus (completionStatus);
    reply (pMessage);
}

void RegressionTestObjectManager::addToRegressionShell (const PrismServiceId &prismServiceId, const bool &isEnabledByDefault)
{
    (getInstance ())->m_testServiceEntries.push_back (RegressionTestEntry (prismServiceId, isEnabledByDefault));
}

}
