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

#include "Regression/PrismTestObjectManager.h"
#include "Regression/RegressionTestObjectManager.h"

namespace WaveNs
{

PrismTestObjectManager::PrismTestObjectManager (const string &prismTestObjectManagerName, const UI32 &stackSize, const vector<UI32> *pCpuAffinityVector)
    : WaveLocalObjectManager (prismTestObjectManagerName, stackSize, pCpuAffinityVector)
{
    addOperationMap (WAVE_OBJECT_MANAGER_TEST,                   reinterpret_cast<PrismMessageHandler> (&PrismTestObjectManager::testRequestHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION, reinterpret_cast<PrismMessageHandler> (&PrismTestObjectManager::prepareForRegressionRequestHandler));
    addOperationMap (WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION2, reinterpret_cast<PrismMessageHandler> (&PrismTestObjectManager::prepareForRegressionRequestHandler2));

    RegressionTestObjectManager::addToRegressionShell (getServiceId (), true);
}

void PrismTestObjectManager::prepareForRegressionRequestHandler (RegressionPrepareMessage *pMessage)
{
    trace (TRACE_LEVEL_INFO, "PrismTestObjectManager::prepareForRegressionRequestHandler : Entering ...");

    m_regressionInput = pMessage->getInputStrings ();
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void PrismTestObjectManager::prepareForRegressionRequestHandler2 (RegressionPrepareMessage2 *pMessage)
{
    trace (TRACE_LEVEL_INFO, "PrismTestObjectManager::prepareForRegressionRequestHandler2 : Entering ...");

    UI32             argIndex;
    vector<string>   inputParameters;

    inputParameters = pMessage->getInputStrings();
    for (argIndex = 0; argIndex < (UI32) inputParameters.size() ; )
    {
        if ('-' == inputParameters[argIndex].at(0))
        {
            inputParameters[argIndex].erase( inputParameters[argIndex].begin() );
            m_testParameters[ inputParameters[argIndex] ] = inputParameters[argIndex + 1];
            argIndex += 2;
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "PrismTestObjectManager::prepareForRegressionRequestHandler2 : Skipping input '" + inputParameters[argIndex] + "'.  Expecting Key prefixed with '-'.");
            argIndex++;
        }
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

PrismMessage *PrismTestObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case WAVE_OBJECT_MANAGER_TEST :
            pPrismMessage = new RegressionTestMessage;
            break;

        case WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION :
            pPrismMessage = new RegressionPrepareMessage;
            break;

        case WAVE_OBJECT_MANAGER_PREPARE_FOR_REGRESSION2 :
            pPrismMessage = new RegressionPrepareMessage2;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

string PrismTestObjectManager::getTestParameterValue (const string &inputStringKey)
{
    map<string,string>::iterator iter;
    iter = m_testParameters.find(inputStringKey);
    string returnString;

    if (m_testParameters.end() != iter)
    {
        returnString = iter->second;
    }
    else
    {   
        returnString = "";
    }
    
    return (returnString);
}

}
