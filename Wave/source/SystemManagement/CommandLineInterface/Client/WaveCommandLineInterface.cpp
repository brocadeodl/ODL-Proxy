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

#include <iostream>
#include <fstream>
#include "SystemManagement/CommandLineInterface/Client/WaveCommandLineInterface.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Types/Types.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "SystemManagement/CommandLineInterface/Server/CommandLineInterfaceMessage.h"
#include "SystemManagement/SystemManagementToolKit.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "Modeling/YANG/Parser/YinParser.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "SystemManagement/WaveSystemManagementObjectManager.h"

using namespace std;

namespace WaveNs
{

WaveCommandLineInterface *WaveCommandLineInterface::m_pWaveCommandLineInterface = NULL;

WaveCommandLineInterface::WaveCommandLineInterface (const string &serverIpAddress, const SI32 &serverPort)
    : m_serverIpAddress                      (serverIpAddress),
      m_serverPort                           (serverPort),
      m_connectionStatus                     (WAVE_CLI_CONNECTION_NOT_ATTEMPTED),
      m_pClientStreamingSocket               (NULL),
      m_pYangUserInterface                   (NULL),
      m_pCurrentModeCommandLineInterfacEntry (NULL)
{
    m_pWaveCommandLineInterface = this;

    m_pCurrentModeCommandLineInterfacEntry = &m_commandLineinterfaceEntries;

    initializePrismSocketLayer ();

    string                ipAddress = FrameworkToolKit::getAPublicIpAddressForThisMachine ();
    FrameworkSequenceType type      = FRAMEWORK_SEQUENCE_TYPE0;
    SI32                  port      = FrameworkToolKit::getPrismDefaultTcpPort ();

    PrismFrameworkObjectManager::setIpAddressForThisLocation (ipAddress);

    PrismFrameworkObjectManager::setFrameworkSequenceType (type);
    PrismFrameworkObjectManager::configureAsLocation (port);

    FrameworkToolKit::setManagementInterfaceRole (WAVE_MGMT_INTF_ROLE_CLI);
    FrameworkToolKit::initialize ();
    TraceObjectManager::setIsSysLogRequired (false);
    FrameworkToolKit::setIsRunningAsADaemon (false);
    PrismFrameworkObjectManager::getInstance ();

    installSigIntHandlers ();

    m_thisMachineIpAddress = ipAddress;
    m_thisMachineHostName  = FrameworkToolKit::getHostNameForThisMachine ();

    connectToServer ();

    sendHelloSecretToServer ();

    CommandLineInterfaceMessage cliMessgae (WAVE_CLI_OPERATION_REQUEST, WAVE_CLI_CLIENT_OPERATION_GET_YANG_USER_INTERFACE);

    ResourceId processingStatus = WAVE_CLI_OPERATION_ERROR;

    processingStatus = processCommand (cliMessgae);

    if (WAVE_CLI_OPERATION_SUCCESS != processingStatus)
    {
        tracePrintf (TRACE_LEVEL_ERROR, false, true, "Could not load user interface from server.");
        exit (-1);
    }
    else
    {
        displayTopLevelPrompt ();
        fflush (stdout);

        vector<string>  output       = cliMessgae.getOutput              ();
        YangElement    *pYangElement = YinParser::parseYangUserInterface (output[0]);

        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        m_pYangUserInterface = dynamic_cast<YangUserInterface *> (pYangElement);

        prismAssert (NULL != m_pYangUserInterface, __FILE__, __LINE__);

        m_pYangUserInterface->computeFirstDataElementInHierarchy    ();
        m_pYangUserInterface->computeIsConfiguration                ();
        m_pYangUserInterface->computeCliTargetNodeNames             ();
        m_pYangUserInterface->computeWillBeShownInConfigurationTree ();
        m_pYangUserInterface->computeCliTargetNodeNameMapForData    ();

        initializeCommandLineEntries ();

        acceptCommands ();
    }
}

WaveCommandLineInterface::~WaveCommandLineInterface ()
{
    if (NULL != m_pClientStreamingSocket)
    {
        delete m_pClientStreamingSocket;
    }
}

void WaveCommandLineInterface::connectToServer ()
{
    if (NULL == m_pClientStreamingSocket)
    {
        m_pClientStreamingSocket = new ClientStreamingSocket (m_serverIpAddress, m_serverPort, 1);

        if (false == (m_pClientStreamingSocket->getIsConnected ()))
        {
            delete m_pClientStreamingSocket;
            m_pClientStreamingSocket = NULL;
            m_connectionStatus = WAVE_CLI_CONNECTION_NOT_ESTABLISHED;
        }
        else
        {
            m_connectionStatus = WAVE_CLI_CONNECTION_ESTABLISHED;
        }
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveCommandLineInterface::reconnectToServer ()
{
    if (NULL != m_pClientStreamingSocket)
    {
        delete m_pClientStreamingSocket;
        m_pClientStreamingSocket = NULL;
        m_connectionStatus = WAVE_CLI_CONNECTION_NOT_ESTABLISHED;

        connectToServer ();
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void WaveCommandLineInterface::sendHelloSecretToServer ()
{
    if (WAVE_CLI_CONNECTION_ESTABLISHED == m_connectionStatus)
    {
        (*m_pClientStreamingSocket) << (SystemManagementToolKit::getHelloSecret ());
    }
}

void WaveCommandLineInterface::post (CommandLineInterfaceMessage &commandLineInterfaceMessage)
{
    if (WAVE_CLI_CONNECTION_ESTABLISHED == m_connectionStatus)
    {
        (*m_pClientStreamingSocket) << &commandLineInterfaceMessage;
    }
}

bool WaveCommandLineInterface::receive (CommandLineInterfaceMessage &commandLineInterfaceMessage)
{
    bool isSuccessful = false;
    UI32 messageSize  = 0;

    if (WAVE_CLI_CONNECTION_ESTABLISHED == m_connectionStatus)
    {
        isSuccessful = (*m_pClientStreamingSocket) >> messageSize;

        if (true == isSuccessful)
        {
            FixedSizeBuffer messageBuffer (messageSize);

            isSuccessful = (*m_pClientStreamingSocket) >> messageBuffer;

            if (true == isSuccessful)
            {
                string messageString;

                messageBuffer.toString (messageString);

                commandLineInterfaceMessage.loadFromSerializedData2 (messageString);
            }
        }
    }

    return (isSuccessful);
}

ResourceId WaveCommandLineInterface::processCommand (CommandLineInterfaceMessage &commandLineInterfaceMessage)
{
    bool       receivingStatus  = false;
    ResourceId processingStatus = WAVE_CLI_OPERATION_ERROR;
    string     ttyName          = FrameworkToolKit::getCurrentTtyName ();

    commandLineInterfaceMessage.setTtyName (ttyName);

    post (commandLineInterfaceMessage);

    receivingStatus =  receive (commandLineInterfaceMessage);

    if (true == receivingStatus)
    {
        processingStatus = commandLineInterfaceMessage.getCommandLineInterfaceOperationStatus ();
    }

    return (processingStatus);
}

void WaveCommandLineInterface::displayTopLevelPrompt ()
{
    printf ("%s# ", m_thisMachineHostName.c_str ());
}

void WaveCommandLineInterface::initializeCommandLineEntries ()
{
    CommandLineInterfaceEntry *pCommandLineInterfaceEntryForshow              = new CommandLineInterfaceEntry ("show");
    CommandLineInterfaceEntry *pCommandLineInterfaceEntryForshowRunningConfig = new CommandLineInterfaceEntry ("running-config");
    CommandLineInterfaceEntry *pCommandLineInterfaceEntryForConfigure         = new CommandLineInterfaceEntry ("configure");
    CommandLineInterfaceEntry *pCommandLineInterfaceEntryForConfigureTerminal = new CommandLineInterfaceEntry ("terminal");

    pCommandLineInterfaceEntryForshow->addChildEntryIfDoesNotExist      (pCommandLineInterfaceEntryForshowRunningConfig);
    pCommandLineInterfaceEntryForConfigure->addChildEntryIfDoesNotExist (pCommandLineInterfaceEntryForConfigureTerminal);

    m_commandLineinterfaceEntries.addChildEntryIfDoesNotExist (pCommandLineInterfaceEntryForshow);
    m_commandLineinterfaceEntries.addChildEntryIfDoesNotExist (pCommandLineInterfaceEntryForConfigure);

    pCommandLineInterfaceEntryForshow->setCliTargetNodeName              ("");
    pCommandLineInterfaceEntryForshowRunningConfig->setCliTargetNodeName ("");
    pCommandLineInterfaceEntryForConfigure->setCliTargetNodeName         ("");
    pCommandLineInterfaceEntryForConfigureTerminal->setCliTargetNodeName ("");

    vector<string> commandLinesForShowRunningConfig;

    m_pYangUserInterface->getAllCliTargetNodeNamesForData (commandLinesForShowRunningConfig);

    pCommandLineInterfaceEntryForshowRunningConfig->addCommandLines (commandLinesForShowRunningConfig);

    m_commandLineinterfaceEntries.sortValidCommands ();
}

CommandLineInterfaceEntry *WaveCommandLineInterface::getCommandLineInterfaceEntryForShowRunningConfiguration (const string &commandLine)
{
//    string fullCommandLineForShowRunningConfiguration = "show running-configuration " + commandLine;

    vector<string> keyVector;
    return (m_commandLineinterfaceEntries.getCommandLineInterfaceEntry (commandLine, keyVector));
}

void WaveCommandLineInterface::sigIntHandler (int signal)
{
    system ("stty cooked echo");

    cout << endl;
    //cout << "Program received signal SIGINT, Exiting." << endl;

    exit (0);
}

void WaveCommandLineInterface::sigSegvHandler (int signal)
{
    system ("stty cooked echo");

    cout << endl;
    //cout << "Program received signal SIGSEGV, Exiting." << endl;

    prismAssert (0, __FILE__, __LINE__);

    exit (0);
}

void WaveCommandLineInterface::installSigIntHandlers () const
{
    struct sigaction signalAction;

    sigemptyset (&(signalAction.sa_mask));
    signalAction.sa_flags = 0;
    signalAction.sa_handler = sigIntHandler;

    sigaction (SIGINT, &signalAction, NULL);

    sigemptyset (&(signalAction.sa_mask));
    signalAction.sa_flags = 0;
    signalAction.sa_handler = sigSegvHandler;

    sigaction (SIGSEGV, &signalAction, NULL);
}


void WaveCommandLineInterface::acceptCommands ()
{
    system("stty raw -echo isig");

    string commandFromPreviousLevel;
    string command;

    m_pCurrentModeCommandLineInterfacEntry = &m_commandLineinterfaceEntries;

    while (true)
    {
        commandFromPreviousLevel.clear ();
        command.clear ();

        m_commandLineinterfaceEntries.acceptcommand (m_thisMachineHostName, commandFromPreviousLevel, command, this);

        StringUtils::stripAllOccurancesAtTheEnd (commandFromPreviousLevel, ' ');

        m_pCurrentModeCommandLineInterfacEntry->addToCommandHistory (commandFromPreviousLevel);
        m_pCurrentModeCommandLineInterfacEntry->resetCommandHistoryCurrentIndex ();
        m_pCurrentModeCommandLineInterfacEntry->resetCurrentlyTypingCommand ();

        if (0 == (commandFromPreviousLevel.find ("show running-config")))
        {
            CommandLineInterfaceMessage cliMessage    (WAVE_CLI_OPERATION_REQUEST, WAVE_CLI_CLIENT_OPERATION_SHOW_RUNNING_CONFIGURATION);
            string                      prunedCommand = commandFromPreviousLevel;

            StringUtils::replaceAllInstancesOfInputStringWith (prunedCommand, "show running-config ", "");

            cliMessage.addToInput (prunedCommand);

            ResourceId processingStatus = WAVE_CLI_OPERATION_ERROR;

            processingStatus = processCommand (cliMessage);

            if (WAVE_CLI_OPERATION_ERROR == processingStatus)
            {
            	// TODO
            }
        }
        else if (0 == (commandFromPreviousLevel.find ("writezoning")))
        {
            StringUtils::replaceAllInstancesOfInputStringWith (commandFromPreviousLevel, "writezoning ", "");
            
            vector<string> tokens; 
            
            CommandLineInterfaceMessage cliMessage (WAVE_CLI_OPERATION_REQUEST, WAVE_CLI_CLIENT_OPERATION_WRITE_REQUEST);

            StringUtils::tokenize (commandFromPreviousLevel, tokens, '"'); 


            if (tokens.size () > 2 && tokens.size () < 1)
            {
                cout << "Input arguments are wrong" << endl;
            }
            else if (tokens.size () == 1)
            {
                cliMessage.addToInput ("none");
                cliMessage.addToInput (tokens[0]);
            }
            else if (tokens.size () == 2)
            {
                cliMessage.addToInput (tokens[0]);
                cliMessage.addToInput (tokens[1]);
            }

            ResourceId processingStatus = WAVE_CLI_OPERATION_ERROR;

            processingStatus = processCommand (cliMessage);

            if (WAVE_CLI_OPERATION_ERROR == processingStatus)
            {
                cout << "            Error in processing command"; 
            }
        
        }
        else if (0 == (commandFromPreviousLevel.find ("replay file")))
        {
            // Get the file name
            StringUtils::replaceAllInstancesOfInputStringWith (commandFromPreviousLevel, "replay file ", "");

            // Read the commands from file
            string line;
            string policy;
            UI32 lineCounter = 0;
            ifstream myfile (commandFromPreviousLevel.c_str ());
    
            system ("date");
    
            if (myfile.is_open())
            {   
                while ( myfile.good())
                {
                    lineCounter++;
                    getline (myfile,line);
   
                    StringUtils::removeAllInstancesOfInputChar (line, '\r');
    
                    if (line == "exit")
                    {
                        //cout << "Exit" << endl;
                        lineCounter = 0;
                        continue;
                    }
                    else if (true == line.empty ())
                    {
                        cout << "Empty Line" << endl;
                        continue;
                    }

                    if (lineCounter == 1)
                    {
                        policy = line;
                    }
                    // Send the line to the Server 
                    CommandLineInterfaceMessage cliMessage (WAVE_CLI_OPERATION_REQUEST, WAVE_CLI_CLIENT_OPERATION_WRITE_REQUEST);

                    if (lineCounter == 1)
                    {
                        cliMessage.addToInput ("none");
                    }
                    else 
                    {
                        cliMessage.addToInput (policy);
                    }
                    
                    //line = " " + line;

                    cliMessage.addToInput (line);

                    ResourceId processingStatus = WAVE_CLI_OPERATION_ERROR;

                    processingStatus = processCommand (cliMessage);

                    if (WAVE_CLI_OPERATION_ERROR == processingStatus)
                    {
            	        // TODO
                    }

                    // Print the line 
                    //cout << line << endl;
                }
            
                myfile.close();
            }
            else
            {   
                cout << "Unable to open file";
            }   

            system ("date");
        }
    }

    system("stty cooked echo");
}

WaveCommandLineInterface *WaveCommandLineInterface::getInstance ()
{
    return (m_pWaveCommandLineInterface);
}

vector<YangElement *> WaveCommandLineInterface::getYangElementsByTargetNodeName (const string &targetNodeName)
{
    WaveCommandLineInterface *pWaveCommandLineInterface = getInstance ();

    /* If pWaveCommandLineInterface is NOT NULL then we are coming from the WyserCli path
     * If pWaveCommandLineInterface is NULL then we are coming from alternate Cli path and 
     * we will depend on the Yang User Interface in the WaveSystemManagementObjectManager */

    if (NULL != pWaveCommandLineInterface)
    {
        return (pWaveCommandLineInterface->m_pYangUserInterface->getYangElementsByTargetNodeName (targetNodeName));
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveCommandLineInterface::getYangElementsByTargetNodeName : pWaveCommandLineInterface is NULL so getting YUI from WSMObjectManager");
        YangUserInterface *pYangUserInterface =  WaveSystemManagementObjectManager::getYangUserInterface ();
        return (pYangUserInterface->getYangElementsByTargetNodeName  (targetNodeName));
    }
}

CommandLineInterfaceEntry *WaveCommandLineInterface::getPCurrentModecommandLineInterfaceEntry ()
{
    return (m_pCurrentModeCommandLineInterfacEntry);
}

vector<string> &WaveCommandLineInterface::getCommandHistory ()
{
    return (m_pCurrentModeCommandLineInterfacEntry->getCommandHistory ());
}

UI32 &WaveCommandLineInterface::getCommandHistoryCurrentIndex ()
{
    return (m_pCurrentModeCommandLineInterfacEntry->getCommandHistoryCurrentIndex ());
}

string &WaveCommandLineInterface::getCurrentlyTypingCommand ()
{
    return (m_pCurrentModeCommandLineInterfacEntry->getCurrentlyTypingCommand ());
}

CommandLineInterfaceEntry *WaveCommandLineInterface::getPCurrentModeCommandLineInterfaceEntry ()
{
    WaveCommandLineInterface *pWaveCommandLineInterface = getInstance ();

    prismAssert (NULL != pWaveCommandLineInterface, __FILE__, __LINE__);

    return (pWaveCommandLineInterface->m_pCurrentModeCommandLineInterfacEntry);
}

}
