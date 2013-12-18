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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/Migration/WaveSystemManagementGetPossibleCompletionsWorker.h"
#include "SystemManagement/WaveSystemManagementTypes.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "SystemManagement/CommandLineInterface/Client/CommandLineInterfaceEntry.h"
#include "SystemManagement/Migration/WaveSystemManagementGetPossibleCompletionsMessage.h"

namespace WaveNs
{

WaveSystemManagementGetPossibleCompletionsWorker::WaveSystemManagementGetPossibleCompletionsWorker (WaveObjectManager *pWaveObjectManager)
    : WaveWorker (pWaveObjectManager), 
    m_pTopLevelCommandLineinterfaceEntry                (NULL),
    m_pCommandLineInterfaceEntryForShowRunningConfig    (NULL),
    m_pCommandLineInterfaceEntryForShow                 (NULL),
    m_pYangUserInterface                                (NULL),
    m_isInitializationDone                              (false)

{
    addOperationMap (WAVE_SYSTEM_MANAGEMENT_GET_POSSIBLE_COMPLETIONS_MESSAGE, reinterpret_cast<PrismMessageHandler> (&WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler));
}

WaveSystemManagementGetPossibleCompletionsWorker::~WaveSystemManagementGetPossibleCompletionsWorker ()
{
}

PrismMessage *WaveSystemManagementGetPossibleCompletionsWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case WAVE_SYSTEM_MANAGEMENT_GET_POSSIBLE_COMPLETIONS_MESSAGE:
            pPrismMessage = new WaveSystemManagementGetPossibleCompletionsMessage ();
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("WaveSystemManagementGetPossibleCompletionsMessage::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}
                
void WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler (WaveSystemManagementGetPossibleCompletionsMessage *pWaveSystemManagementGetPossibleCompletionsMessage)
{
    trace (TRACE_LEVEL_DEBUG, "WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler : Entered ");

    if (false == m_isInitializationDone)
    {
        initializeCommandLineInterfaces ();
    }

    string commandName = pWaveSystemManagementGetPossibleCompletionsMessage->getCommandName ();
    string token = pWaveSystemManagementGetPossibleCompletionsMessage->getToken ();
    vector<string> keyVector;
    
    trace (TRACE_LEVEL_DEBUG, string ("WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler Command Name is ###########   ") + commandName + string ("   ##################"));

    vector<string>              possibleCompletions;
    CommandLineInterfaceEntry  *pTokenCliEntry;
    vector<YangElement *>       tempYangElements;
    
    if (commandName == "")
    {
        m_pCommandLineInterfaceEntryForShowRunningConfig->getValidCommands (possibleCompletions, keyVector , token) ;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string ("WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler : command name after  removing  :  ") + commandName);
        pTokenCliEntry = m_pCommandLineInterfaceEntryForShowRunningConfig->getCommandLineInterfaceEntry (commandName , keyVector);
        if (NULL != pTokenCliEntry)
        { 
            pTokenCliEntry->getValidCommands (possibleCompletions, keyVector, token);
        }
    }

    if ("" != commandName)
    {
        commandName += " ";
    }

    for (UI32 i = 0; i < possibleCompletions.size (); i++)
    {
        trace (TRACE_LEVEL_DEBUG, string ("WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler : Children :  ") + possibleCompletions[i]);

        tempYangElements    = m_pYangUserInterface->getYangElementsByTargetNodeName (commandName + possibleCompletions[i]);

        if (tempYangElements.size () > 0)
        {
            trace (TRACE_LEVEL_DEBUG, string ("WaveSystemManagementGetPossibleCompletionsWorker::getPossibleCompletionsMessageHandler : Child Info : ") + tempYangElements[0]->getInfo ());
            pWaveSystemManagementGetPossibleCompletionsMessage->addCompletionNameAndInfoOneByOne (possibleCompletions[i], tempYangElements[0]->getInfo ());
        }
        else
        {
            pWaveSystemManagementGetPossibleCompletionsMessage->addCompletionNameAndInfoOneByOne (possibleCompletions[i], ""); 
        }

        tempYangElements.clear ();
    }

    pWaveSystemManagementGetPossibleCompletionsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pWaveSystemManagementGetPossibleCompletionsMessage);
}
                
void WaveSystemManagementGetPossibleCompletionsWorker::initializeCommandLineInterfaces ()
{
    trace (TRACE_LEVEL_DEBUG, string ("WaveSystemManagementGetPossibleCompletionsWorker::initializeCommandLineInterfaces3 : Entering ... "));

    m_pYangUserInterface  = WaveSystemManagementObjectManager::getYangUserInterface ();    

    vector<string> commandLinesForShowRunningConfig;

    m_pYangUserInterface->getAllCliTargetNodeNamesForData (commandLinesForShowRunningConfig);

    m_pTopLevelCommandLineinterfaceEntry                = new CommandLineInterfaceEntry ();
    m_pCommandLineInterfaceEntryForShow                 = new CommandLineInterfaceEntry ("show");
    m_pCommandLineInterfaceEntryForShowRunningConfig    = new CommandLineInterfaceEntry ("running-config");

    m_pCommandLineInterfaceEntryForShow->addChildEntryIfDoesNotExist    (m_pCommandLineInterfaceEntryForShowRunningConfig);
    m_pTopLevelCommandLineinterfaceEntry->addChildEntryIfDoesNotExist   (m_pCommandLineInterfaceEntryForShow);
   
    m_pCommandLineInterfaceEntryForShow->setCliTargetNodeName              ("");
    m_pCommandLineInterfaceEntryForShowRunningConfig->setCliTargetNodeName ("");


    m_pCommandLineInterfaceEntryForShowRunningConfig->addCommandLines (commandLinesForShowRunningConfig);
    
    m_pTopLevelCommandLineinterfaceEntry->sortValidCommands ();

    m_isInitializationDone = true;
}

}

