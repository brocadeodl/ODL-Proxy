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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Trace/TraceMessages.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Core/Wave.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerMessages.h"

#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <dirent.h>


using namespace std;

namespace WaveNs
{

static string         s_traceFileName;
static bool           s_isSysLogRequired = true;
static TraceClientMap s_traceClientMap;

PrismMutex                               TraceObjectManager::m_mutexForTracing;
bool                                     TraceObjectManager::m_isFirstTime                               = true;
fstream                                  TraceObjectManager::traceFile;

WaveUserClientPrintFunction              TraceObjectManager::m_pWaveUserClientPrintFunction              = NULL;
WaveUserClientPrintToAllSessionsFunction TraceObjectManager::m_pWaveUserClientPrintToAllSessionsFunction = NULL;
map<UI32, UI32>                          TraceObjectManager::m_nativeClientDebugSessions;
map<UI32, UI32>                          TraceObjectManager::m_userClientDebugSessions;
PrismMutex                               TraceObjectManager::m_clientDebugSessionsMutex;

UI32                                     TraceObjectManager::m_maxTraceFileSize = 2 * 1024 * 1024; // 2MB
UI32                                     TraceObjectManager::m_maxTraceFilesToKeep = 4; // keep 4 .tgz files + 1 .trc file
UI32                                     TraceObjectManager::m_currentTraceFileSize = 0;

void TraceObjectManager::setIsSysLogRequired (bool isSysLogRequired)
{
    s_isSysLogRequired = isSysLogRequired;
}

bool TraceObjectManager::getIsSysLogRequired ()
{
    return (s_isSysLogRequired);
}

TraceObjectManager::TraceObjectManager ()
    : WaveLocalObjectManagerForUserSpecificTasks (getClassName ())
{
    // The TraceObjectManager is enabled by default.  It is required for doing any kind of tracing.  That is why
    // it is enabled by default.  This service will receive enable message as usual.  Any other things that
    // need to be done during enable other than marking the service enabled can be done at that time.

    setIsEnabled (true);

    addOperationMap (TRACE_OPERATION_CODE_TRACE,                                   reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::traceMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_SET_CLIENT_TRACE_LEVEL,                  reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::setClientTraceLevelMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_GET_CLIENT_TRACE_LEVEL,                  reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::getClientTraceLevelMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_GET_CLIENTS_INFORMATION,                 reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::getClientsInformationMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_PRINT_TO_WAVE_CLIENT_SESSION,            reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::printToWaveClientSessionMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_CLIENT_SESSIONS,       reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::printToAllWaveClientSessionsMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_DEBUG_CLIENT_SESSIONS, reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::printToAllWaveDebugClientSessionsMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_MESSAGE_HISTORY_DUMP_FOR_A_SERVICE,      reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::messageHistoryDumpForAServiceMessageHandler));
    addOperationMap (TRACE_OPERATION_CODE_MESSAGE_HISTORY_CONFIG_FOR_A_SERVICE,    reinterpret_cast<PrismMessageHandler> (&TraceObjectManager::messageHistoryConfigForAServiceMessageHandler));

    // restrictMessageHistoryLogging                (bool messageHistoryLogInsideSend, bool messageHistoryLogInsideReply, bool messageHistoryLogInsideHandleMessage);
    restrictMessageHistoryLogging                (false, false, false);
    /*
     * zeroize is required for non-Notive service and native services. This initializatin cannot be done globally
     * hence individual OM's should register themselves if they want to implement zeroize.
    FrameworkSequenceGenerator &frameworkSequenceGenerator = PrismFrameworkObjectManager::getCurrentFrameworkSequenceGenerator ();
    frameworkSequenceGenerator.addPrismServiceIdToZeroize(getServiceId());
     */

}

string TraceObjectManager::getClassName ()
{
    return ("Trace");
}

TraceObjectManager *TraceObjectManager::getInstance ()
{
    static TraceObjectManager *pTraceObjectManager = NULL;

    if (NULL == pTraceObjectManager)
    {
        pTraceObjectManager = new TraceObjectManager ();
        WaveNs::prismAssert (NULL != pTraceObjectManager, __FILE__, __LINE__);
    }

    return (pTraceObjectManager);
}

TraceObjectManager::~TraceObjectManager ()
{
}

PrismServiceId TraceObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *TraceObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case TRACE_OPERATION_CODE_TRACE :
            pPrismMessage = new TraceMessage;
            break;

        case TRACE_OPERATION_CODE_SET_CLIENT_TRACE_LEVEL :
            pPrismMessage = new TraceObjectManagerSetClientTraceLevelMessage;
            break;

        case TRACE_OPERATION_CODE_GET_CLIENT_TRACE_LEVEL :
            pPrismMessage = new TraceObjectManagerGetClientTraceLevelMessage;
            break;

        case TRACE_OPERATION_CODE_GET_CLIENTS_INFORMATION :
            pPrismMessage = new TraceObjectManagerGetClientsInformationMessage;
            break;

        case TRACE_OPERATION_CODE_PRINT_TO_WAVE_CLIENT_SESSION :
            pPrismMessage = new TraceObjectManagerPrintToWaveClientSessionMessage;
            break;

        case TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_CLIENT_SESSIONS :
            pPrismMessage = new TraceObjectManagerPrintToAllWaveClientSessionsMessage;
            break;

        case TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_DEBUG_CLIENT_SESSIONS :
            pPrismMessage = new TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage;
            break;

        case TRACE_OPERATION_CODE_MESSAGE_HISTORY_DUMP_FOR_A_SERVICE :
            pPrismMessage = new TraceObjectManagerMessageHistoryDumpForAServiceMessage;
            break;

        case TRACE_OPERATION_CODE_MESSAGE_HISTORY_CONFIG_FOR_A_SERVICE :
            pPrismMessage = new TraceObjectManagerMessageHistoryConfigForAServiceMessage;
            break;

        default :
            pPrismMessage = NULL;

            break;
    }

    return (pPrismMessage);
}

TraceClientId TraceObjectManager::addClient (const TraceLevel &traceLevel, const string &traceClientName)
{
    return (s_traceClientMap.addClient (traceLevel, traceClientName));
}

void TraceObjectManager::removeClient (const string &traceClientName)
{
    return (s_traceClientMap.removeClient (traceClientName));
}


void TraceObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    //cout << "Trace File : " << s_traceFileName << endl;

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

bool TraceObjectManager::isTracingRequired (const TraceLevel &traceLevel, const TraceClientId &traceClientId)
{
    TraceLevel currentTraceLevel = s_traceClientMap.getClientLevel (traceClientId);

    if (traceLevel >= currentTraceLevel)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void TraceObjectManager::traceDirectly (const TraceClientId &traceClientId, const TraceLevel &requestedTraceLevel, const string &stringToTrace, const bool &addNewLine, const bool &suppressPrefix)
{
    const char       *pTempString       = stringToTrace.c_str ();
          TraceLevel  currentTraceLevel = s_traceClientMap.getClientLevel (traceClientId);
          string      traceStr;

    m_mutexForTracing.lock ();

    if (true == m_isFirstTime)
    {
        m_isFirstTime = false;

        #if 0
        ifstream traceFileToread (s_traceFileName.c_str ());

        if (false == (traceFileToread.is_open ()))
        {
            ofstream traceFileToWrite (s_traceFileName.c_str ());

            traceFileToWrite.close ();
        }
        else
        {
            traceFileToread.seekg (0, ios::end);

            ofstream traceFileToTruncate (s_traceFileName.c_str (), ios::trunc | ios::out);

            traceFileToTruncate.close ();
        }
        #endif

        traceFile.open (s_traceFileName.c_str (), ios::out | ios::app);
        //cout << "TraceObjectManager::traceDirectly : Opening trace file " << s_traceFileName << " as m_isFirstTime is true" << endl << "\r";

        if (true == (traceFile.is_open ()))
        {
            traceFile.seekg (0, ios::end);
            m_currentTraceFileSize = traceFile.tellp ();
            //cout << "TraceObjectManager::traceDirectly : For tracefile " << s_traceFileName << " previous size is " << m_currentTraceFileSize << endl << "\r";
        }
        else
        {
            //cout << "TraceObjectManager::traceDirectly : Failed to open tracefile " << s_traceFileName << endl << "\r";
        }
    }

    // The following comparison between the enums is OK. We can do this by the definition of the trace levels.
    // The trace levels are ordered in a particular way so that we can take advantage of the order.

    if (requestedTraceLevel >= currentTraceLevel)
    {
        prismSetConsoleTextColor (requestedTraceLevel);

        if (false == suppressPrefix)
        {
            traceStr = getTraceTagForLevel (requestedTraceLevel);

            cout      << traceStr;
            traceFile << traceStr;
            m_currentTraceFileSize += traceStr.size();

            // Commenting out all printToAllDebugSession calls to suppress
            // traces from being printed to terminal monitor.
            // Function is commented out and not deleted for future reference.

            // For Hercules, dumping logs on cli is enabled again using "terminal monitor".
            // Hence, uncommenting the call to printToAllDebugSessions at required places.

            // Due to TR358707 during Hercules alpha, re-commenting out printToAllDebugSessions
            // at required places.
            //printToAllDebugSessions (traceStr);

            time_t currentTime;
            char   currentTimeString[64];

            currentTime = time (NULL);
            // use the tread safe version
            ctime_r (&currentTime, currentTimeString);
            //Remove the new line character at the end
            currentTimeString[(strlen (currentTimeString)) - 1] = '\0';

            cout      << currentTimeString << " : ";
            traceFile << currentTimeString << " : ";
            m_currentTraceFileSize += strlen(currentTimeString) + strlen(" : ");
            //printToAllDebugSessions (string (currentTimeString) + " : ");
        }

        // Wite to the console

        cout << pTempString;

        // Write to the trace file

        if (true == (traceFile.is_open ()))
        {
            traceFile.seekg (0, ios::end);
            traceFile << pTempString;
            m_currentTraceFileSize += strlen(pTempString);

             if (true == addNewLine)
             {
                traceFile << endl;
                m_currentTraceFileSize += 1;
             }

             rotateTraceFile();
        }

        // Wrtie to all debug sessions.

        //printToAllDebugSessions (pTempString);

        //if (true == addNewLine)
        //{
        //    printToAllDebugSessions ("\n");
        //}

        // Write to the syslog if required

        if (true == (getIsSysLogRequired ()))
        {
            prismSysLog (pTempString);
        }

        prismResetConsoleTextColor ();

        if (true == addNewLine)
        {
            cout << endl;
            cout << "\r";
        }
    }

    m_mutexForTracing.unlock ();
}

void TraceObjectManager::traceDirectly (const TraceClientId &traceClientId, const TraceLevel &requestedTraceLevel, const bool &addNewLine, const bool &suppressPrefix, const char * const pFormat, va_list &variableArguments)
{
    char       pTempString[1024];
    TraceLevel currentTraceLevel   = s_traceClientMap.getClientLevel (traceClientId);
    string     traceStr;

    m_mutexForTracing.lock ();

    if (true == m_isFirstTime)
    {
        m_isFirstTime = false;

        #if 0
        ifstream traceFileToread (s_traceFileName.c_str ());

        if (false == (traceFileToread.is_open ()))
        {
            ofstream traceFileToWrite (s_traceFileName.c_str ());

            traceFileToWrite.close ();
        }
        else
        {
            traceFileToread.close ();

            ofstream traceFileToTruncate (s_traceFileName.c_str (), ios::trunc | ios::out);

            traceFileToTruncate.close ();
        }
        #endif

        traceFile.open (s_traceFileName.c_str (), ios::out | ios::app);
        //cout << "TraceObjectManager::traceDirectly : Opening trace file " << s_traceFileName << " as m_isFirstTime is true" << endl << "\r";

        if (true == (traceFile.is_open ()))
        {
            m_currentTraceFileSize = traceFile.tellp ();
            //cout << "TraceObjectManager::traceDirectly : For tracefile " << s_traceFileName << " previous size is " << m_currentTraceFileSize << endl << "\r";
        }
        else
        {
            cout << "TraceObjectManager::traceDirectly : Failed to open tracefile " << s_traceFileName << endl << "\r";
        }
    }

    SI32 numberOfChars;

    numberOfChars = vsnprintf (pTempString, 1024, pFormat, variableArguments);

    if (0 > numberOfChars)
    {
    	// TODO : Sagar : Print a warning.
    }

    // The following comparison between the enums is OK. We can do this by the definition of the trace levels.
    // The trace levels are ordered in a particular way so that we can take advantage of the order.

    if (requestedTraceLevel >= currentTraceLevel)
    {
        prismSetConsoleTextColor (requestedTraceLevel);

        if (false == suppressPrefix)
        {
            traceStr = getTraceTagForLevel (requestedTraceLevel);

            cout      << traceStr;
            traceFile << traceStr;
            m_currentTraceFileSize += traceStr.size();
            //printToAllDebugSessions (traceStr);

            time_t currentTime;
            char   currentTimeString[64];

            currentTime = time (NULL);
            // use the tread safe version
            ctime_r (&currentTime, currentTimeString);
            //Remove the new line character at the end
            currentTimeString[(strlen (currentTimeString)) - 1] = '\0';

            cout      << currentTimeString << " : ";
            traceFile << currentTimeString << " : ";
            m_currentTraceFileSize += strlen(currentTimeString) + strlen(" : ");
            //printToAllDebugSessions (string (currentTimeString) + " : ");
        }

        // Wite to the console

        cout << pTempString;

        // Write to the trace file

        if (true == (traceFile.is_open ()))
        {
            traceFile << pTempString;
            m_currentTraceFileSize += strlen(pTempString);

             if (true == addNewLine)
             {
                traceFile << endl;
                m_currentTraceFileSize += 1;
             }

             rotateTraceFile();
        }

        // Write to all debug sessions

        //printToAllDebugSessions (pTempString);

        //if (true == addNewLine)
        //{
        //    printToAllDebugSessions ("\n");
        //}

        // Write to the syslog if required

        if (true == (getIsSysLogRequired ()))
        {
            prismSysLog (pTempString);
        }

        prismResetConsoleTextColor ();

        if (true == addNewLine)
        {
            cout << endl;
            cout << "\r";
        }
    }

    m_mutexForTracing.unlock ();
}

void TraceObjectManager::traceDirectly (const TraceClientId &traceClientId, const TraceLevel &requestedTraceLevel, const char * const pFormat, va_list &variableArguments)
{
    char       pTempString[1024];
    TraceLevel currentTraceLevel   = s_traceClientMap.getClientLevel (traceClientId);
    string     traceStr;

    m_mutexForTracing.lock ();

    if (true == m_isFirstTime)
    {
        m_isFirstTime = false;

        ifstream traceFileToread (s_traceFileName.c_str ());

        #if 0
        if (false == (traceFileToread.is_open ()))
        {
            ofstream traceFileToWrite (s_traceFileName.c_str ());

            traceFileToWrite.close ();
        }
        else
        {
            traceFileToread.close ();

            ofstream traceFileToTruncate (s_traceFileName.c_str (), ios::trunc | ios::out);

            traceFileToTruncate.close ();
        }
        #endif

        traceFile.open (s_traceFileName.c_str (), ios::out | ios::app);
        //cout << "TraceObjectManager::traceDirectly : Opening trace file " << s_traceFileName << " as m_isFirstTime is true" << endl << "\r";

        if (true == (traceFile.is_open ()))
        {
            traceFile.seekg (0, ios::end);
            m_currentTraceFileSize = traceFile.tellp ();
            //cout << "TraceObjectManager::traceDirectly : For tracefile " << s_traceFileName << " previous size is " << m_currentTraceFileSize << endl << "\r";
        }
        else
        {
            cout << "TraceObjectManager::traceDirectly : Failed to open tracefile " << s_traceFileName << endl << "\r";
        }
    }

    SI32 numberOfChars;

    numberOfChars = vsnprintf (pTempString, 1024, pFormat, variableArguments);

    if (0 > numberOfChars)
    {
    	// TODO : Sagar : Print a warning.
    }

    // The following comparison between the enums is OK. We can do this by the definition of the trace levels.
    // The trace levels are ordered in a particular way so that we can take advantage of the order.

    if (requestedTraceLevel >= currentTraceLevel)
    {
        prismSetConsoleTextColor (requestedTraceLevel);

        traceStr = getTraceTagForLevel (requestedTraceLevel);

        cout      << traceStr;
        traceFile << traceStr;
        m_currentTraceFileSize += traceStr.size();
        //printToAllDebugSessions (traceStr);

        time_t currentTime;
        char   currentTimeString[64];

        currentTime = time (NULL);
        // use the tread safe version
        ctime_r (&currentTime, currentTimeString);
        //Remove the new line character at the end
        currentTimeString[(strlen (currentTimeString)) - 1] = '\0';

        cout      << currentTimeString << " : ";
        traceFile << currentTimeString << " : ";
        m_currentTraceFileSize += strlen(currentTimeString) + strlen(" : ");
        //printToAllDebugSessions (string (currentTimeString) + " : ");

        // Wite to the console

        cout << pTempString;

        // Write to the trace file

        if (true == (traceFile.is_open ()))
        {
            traceFile << pTempString;

            traceFile << endl;
            m_currentTraceFileSize += strlen(pTempString) + 1;

            rotateTraceFile();
        }

        // Write to all debug sessions.

        //printToAllDebugSessions (string (pTempString) + "\n");

        // Write to the syslog if required

        if (true == (getIsSysLogRequired ()))
        {
            prismSysLog (pTempString);
        }

        prismResetConsoleTextColor ();

        cout << endl;
        cout << "\r";
    }

    m_mutexForTracing.unlock ();
}

void TraceObjectManager::traceMessageHandler (TraceMessage *pTraceMessage)
{
                 UI32        bufferSize          = 0;
           const char       *pTempString         = NULL;
                 TraceLevel  requestedTraceLevel = pTraceMessage->getTraceLevel ();
                 TraceLevel  currentTraceLevel   = s_traceClientMap.getClientLevel (pTraceMessage->getTraceClientId ());
                 bool        addNewLine          = pTraceMessage->getAddNewLine ();
                 string      traceStr;

    if (true == m_isFirstTime)
    {
        m_isFirstTime = false;

        ifstream traceFileToread (s_traceFileName.c_str ());

        #if 0
        if (false == (traceFileToread.is_open ()))
        {
            ofstream traceFileToWrite (s_traceFileName.c_str ());

            traceFileToWrite.close ();
        }
        else
        {
            traceFileToread.close ();

            ofstream traceFileToTruncate (s_traceFileName.c_str (), ios::trunc | ios::out);

            traceFileToTruncate.close ();
        }
        #endif

        traceFile.open (s_traceFileName.c_str (), ios::out | ios::app);
        //cout << "TraceObjectManager::traceMessageHandler : Opening trace file " << s_traceFileName << " as m_isFirstTime is true" << endl << "\r";

        if (true == (traceFile.is_open ()))
        {
            m_currentTraceFileSize = traceFile.tellp ();
            //cout << "TraceObjectManager::traceMessageHandler : For tracefile " << s_traceFileName << " previous size is " << m_currentTraceFileSize << endl << "\r";
        }
        else
        {
            cout << "TraceObjectManager::traceMessageHandler : Failed to open tracefile " << s_traceFileName << endl << "\r";
        }
    }

    // The following comparison between the enums is OK. We can do this by the definition of the trace levels.
    // The trace levels are ordered in a particular way so that we can take advantage of the order.

    if (requestedTraceLevel >= currentTraceLevel)
    {
        prismSetConsoleTextColor (requestedTraceLevel);
        traceStr = getTraceTagForLevel (requestedTraceLevel);

        cout << traceStr;
        traceFile << traceStr;
        m_currentTraceFileSize += traceStr.size();

        pTempString = (const char *) (pTraceMessage->findBuffer (TRACE_INPUT_BUFFER, bufferSize));

        char *pTempString2 = new char[bufferSize + 1];

        strncpy (pTempString2, pTempString, bufferSize);
        pTempString2[bufferSize] = '\0';

        // Wite to the console

        printf ("%s", pTempString2);

        // Write to the trace file

        if (true == (traceFile.is_open ()))
        {
            traceFile.seekg (0, ios::end);
            traceFile << pTempString2;
            m_currentTraceFileSize += strlen(pTempString2);

             if (true == addNewLine)
             {
                traceFile << endl;
                m_currentTraceFileSize += 1;
             }

             rotateTraceFile();
        }

        // Write to the syslog if required

        if (true == (getIsSysLogRequired ()))
        {
            prismSysLog (pTempString2);
        }

        delete[] pTempString2;

        prismResetConsoleTextColor ();

        if (true == addNewLine)
        {
            cout << endl;
        }
    }

    pTraceMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pTraceMessage);
}

void TraceObjectManager::setTraceFileName (const string &traceFileName)
{
    //cout << "TraceObjectManager::setTraceFileName : Now Trace File is set to : " << traceFileName << endl << "\r";
    s_traceFileName = traceFileName;
}

ResourceId TraceObjectManager::setClientTraceLevelDirectly (const TraceClientId &traceClientId, const TraceLevel &traceLevel, bool isSetAll)
{
    bool status     = false;

    if (true == isSetAll)
    {
        vector<TraceClientId>   traceClientIdsVector;
        vector<TraceLevel>      traceLevelsVector;

        s_traceClientMap.getClientsAndLevels (traceClientIdsVector, traceLevelsVector);
        UI32        nServices   = traceClientIdsVector.size ();
        UI32        nSuccess    = 0;
        for (UI32 i = 0; i < nServices; i++)
        {
            status = s_traceClientMap.setClientLevel (traceClientIdsVector[i], traceLevel);
            if (true == status)
            {
                nSuccess++;
            }
        }
        return((nSuccess == nServices) ? (WAVE_MESSAGE_SUCCESS) : (WAVE_MESSAGE_ERROR));
    }
    else
    {
        status = s_traceClientMap.setClientLevel (traceClientId, traceLevel);

        if (true == status)
        {
            return (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            return (WAVE_MESSAGE_ERROR);
        }
    }
}

void TraceObjectManager::setClientTraceLevelMessageHandler (TraceObjectManagerSetClientTraceLevelMessage *pTraceObjectManagerSetClientTraceLevelMessage)
{
    TraceClientId traceClientId = pTraceObjectManagerSetClientTraceLevelMessage->getTraceClientId ();
    TraceLevel    traceLevel    = pTraceObjectManagerSetClientTraceLevelMessage->getTraceLevel    ();
    bool          isSetAll      = pTraceObjectManagerSetClientTraceLevelMessage->getIsSetAll      ();
    ResourceId    status        = WAVE_MESSAGE_ERROR;

    status = setClientTraceLevelDirectly (traceClientId, traceLevel, isSetAll);

    pTraceObjectManagerSetClientTraceLevelMessage->setCompletionStatus (status);
    reply (pTraceObjectManagerSetClientTraceLevelMessage);
}

TraceLevel TraceObjectManager::getClientTraceLevelDirectly (const TraceClientId &traceClientId)
{
    return (s_traceClientMap.getClientLevel (traceClientId));
}

void TraceObjectManager::getClientTraceLevelMessageHandler (TraceObjectManagerGetClientTraceLevelMessage *pTraceObjectManagerGetClientTraceLevelMessage)
{
    TraceClientId traceClientId    = pTraceObjectManagerGetClientTraceLevelMessage->getTraceClientId ();
    TraceLevel    clientTraceLevel = s_traceClientMap.getClientLevel (traceClientId);

    pTraceObjectManagerGetClientTraceLevelMessage->setTraceLevel (clientTraceLevel);
    pTraceObjectManagerGetClientTraceLevelMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pTraceObjectManagerGetClientTraceLevelMessage);
}

void TraceObjectManager::getClientsInformationDirectly (vector<TraceClientId> &traceClientIdsVector, vector<TraceLevel> &traceLevelsVector)
{
    s_traceClientMap.getClientsAndLevels (traceClientIdsVector, traceLevelsVector);
}

void TraceObjectManager::getClientsInformationMessageHandler (TraceObjectManagerGetClientsInformationMessage *pTraceObjectManagerGetClientsInformationMessage)
{
    vector<TraceClientId>  traceClientIdsVector;
    vector<TraceLevel>     traceLevelsVector;
    UI32                   numberOfTraceClients;
    UI32                   i;

    s_traceClientMap.getClientsAndLevels (traceClientIdsVector, traceLevelsVector);

    prismAssert ((traceClientIdsVector.size ()) == (traceLevelsVector.size ()), __FILE__, __LINE__);

    numberOfTraceClients = traceClientIdsVector.size ();

    for (i = 0; i < numberOfTraceClients; i++)
    {
        pTraceObjectManagerGetClientsInformationMessage->addTraceClientInformation (traceClientIdsVector[i], traceLevelsVector[i]);
    }

    pTraceObjectManagerGetClientsInformationMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    reply (pTraceObjectManagerGetClientsInformationMessage);
    return;
}

string TraceObjectManager::getTraceClientName (const TraceClientId &traceClientId)
{
    return (s_traceClientMap.getTraceClientName (traceClientId));
}

void TraceObjectManager::setPWaveUserClientPrintFunction (WaveUserClientPrintFunction waveUserClientPrintFunction)
{
    m_pWaveUserClientPrintFunction = waveUserClientPrintFunction;
}

void TraceObjectManager::setPWaveUserClientPrintToAllSessionsFunction (WaveUserClientPrintToAllSessionsFunction waveUserClientPrintToAllSessionsFunction)
{
    m_pWaveUserClientPrintToAllSessionsFunction = waveUserClientPrintToAllSessionsFunction;
}

void TraceObjectManager::printToWaveClientSessionMessageHandler (TraceObjectManagerPrintToWaveClientSessionMessage *pTraceObjectManagerPrintToWaveClientSessionMessage)
{
    const LocationId waveClientOriginatingLocationId = pTraceObjectManagerPrintToWaveClientSessionMessage->getClientOriginatingLocationId ();
    const UI32       waveNativeClientId              = pTraceObjectManagerPrintToWaveClientSessionMessage->getNativeClientId              ();
    const UI32       waveUserClientId                = pTraceObjectManagerPrintToWaveClientSessionMessage->getUserClientId                ();
    const string     messageToBePrinted              = pTraceObjectManagerPrintToWaveClientSessionMessage->getMessageToBePrinted          ();
    const LocationId thisLocationId                  = FrameworkToolKit::getThisLocationId                                                ();

    prismAssert (waveClientOriginatingLocationId == thisLocationId, __FILE__, __LINE__);

    if (0 != waveNativeClientId)
    {
    }
    else if (0 != waveUserClientId)
    {
        if (NULL != m_pWaveUserClientPrintFunction)
        {
            (*m_pWaveUserClientPrintFunction) (waveUserClientId, messageToBePrinted);
        }
    }

    pTraceObjectManagerPrintToWaveClientSessionMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

    reply (pTraceObjectManagerPrintToWaveClientSessionMessage);
    return;
}

void TraceObjectManager::printToAllWaveClientSessionsMessageHandler (TraceObjectManagerPrintToAllWaveClientSessionsMessage *pTraceObjectManagerPrintToAllWaveClientSessionsMessage)
{
    const LocationId waveClientOriginatingLocationId = pTraceObjectManagerPrintToAllWaveClientSessionsMessage->getClientOriginatingLocationId ();
    const string     messageToBePrinted              = pTraceObjectManagerPrintToAllWaveClientSessionsMessage->getMessageToBePrinted          ();
    const LocationId thisLocationId                  = FrameworkToolKit::getThisLocationId                                                    ();

    prismAssert (waveClientOriginatingLocationId == thisLocationId, __FILE__, __LINE__);

    // Run through a loop for native clinets.
    // TODO : Sagar

    // Next, Call the user client API.

    if (NULL != m_pWaveUserClientPrintToAllSessionsFunction)
    {
        (*m_pWaveUserClientPrintToAllSessionsFunction) (messageToBePrinted);
    }

    pTraceObjectManagerPrintToAllWaveClientSessionsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pTraceObjectManagerPrintToAllWaveClientSessionsMessage);

    return;
}

bool TraceObjectManager::isAKnownNativeClientDebugSession (const UI32 &natieClientId)
{
    map<UI32, UI32>::iterator element     = m_nativeClientDebugSessions.find (natieClientId);
    map<UI32, UI32>::iterator endElement  = m_nativeClientDebugSessions.end  ();

    if (element != endElement)
    {
        return (true);
    }
    else
    {
        return (false);
    }

}

bool TraceObjectManager::isAKnownUserClientDebugSession (const UI32 &userClientId)
{
    map<UI32, UI32>::iterator element1    = m_userClientDebugSessions.find (userClientId);
    map<UI32, UI32>::iterator endElement1 = m_userClientDebugSessions.end  ();

    if (element1 != endElement1)
    {
        return (true);
    }
    else
    {
        return (false);
    }

}

void TraceObjectManager::addNativeClientDebugSession (const UI32 &nativeClientId)
{
    m_clientDebugSessionsMutex.lock ();

    bool isAnExistingDebugSession = isAKnownNativeClientDebugSession (nativeClientId);

    if (false == isAnExistingDebugSession)
    {
        m_nativeClientDebugSessions[nativeClientId] = nativeClientId;
    }

    m_clientDebugSessionsMutex.unlock ();
}

void TraceObjectManager::removeNativeClientDebugSession (const UI32 &nativeClientId)
{
    m_clientDebugSessionsMutex.lock ();

    bool isAnExistingDebugSession = isAKnownNativeClientDebugSession (nativeClientId);

    if (true == isAnExistingDebugSession)
    {
        m_nativeClientDebugSessions.erase (nativeClientId);
    }

    m_clientDebugSessionsMutex.unlock ();
}

void TraceObjectManager::addUserClientDebugSession (const UI32 &userClientId)
{
    ResourceId  clientNotificationStatus    = WAVE_MESSAGE_SUCCESS;

    m_clientDebugSessionsMutex.lock ();     // [

    bool isAnExistingDebugSession = isAKnownUserClientDebugSession (userClientId);

    if (false == isAnExistingDebugSession)
    {
        m_userClientDebugSessions[userClientId] = userClientId;
    }

    // if (1 == m_userClientDebugSessions.size ())  // Always send this message, so that newly connected clients get it.
    {
        WaveUpdateClientStatusMessage *pWaveUpdateClientStatusMessage = new WaveUpdateClientStatusMessage (true);
        WaveNs::prismAssert (NULL != pWaveUpdateClientStatusMessage, __FILE__, __LINE__);

        clientNotificationStatus = WaveObjectManagerToolKit::sendOneWayToAllWaveClients (pWaveUpdateClientStatusMessage);
    }

    m_clientDebugSessionsMutex.unlock ();   // ]

    if (false == isAnExistingDebugSession)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("TraceObjectManager::addUserClientDebugSession : added session id [") + userClientId + string ("]. Client notofication status [") + FrameworkToolKit::localizeToSourceCodeEnum (clientNotificationStatus) + string ("]"));
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("TraceObjectManager::addUserClientDebugSession : session id [") + userClientId + string ("] already exists. Client notofication status [") + FrameworkToolKit::localizeToSourceCodeEnum (clientNotificationStatus) + string ("]"));
    }
}

void TraceObjectManager::removeUserClientDebugSession (const UI32 &userClientId)
{
    bool        clientsNotified             = false;
    ResourceId  clientNotificationStatus    = WAVE_MESSAGE_SUCCESS;

    m_clientDebugSessionsMutex.lock ();     // [

    bool isAnExistingDebugSession = isAKnownUserClientDebugSession (userClientId);

    if (true == isAnExistingDebugSession)
    {
        m_userClientDebugSessions.erase (userClientId);

        if (true == m_userClientDebugSessions.empty ())
        {
            WaveUpdateClientStatusMessage *pWaveUpdateClientStatusMessage = new WaveUpdateClientStatusMessage (false);
            WaveNs::prismAssert (NULL != pWaveUpdateClientStatusMessage, __FILE__, __LINE__);

            clientNotificationStatus = WaveObjectManagerToolKit::sendOneWayToAllWaveClients (pWaveUpdateClientStatusMessage);

            clientsNotified = true;
        }
    }

    m_clientDebugSessionsMutex.unlock ();   // ]

    if (true == clientsNotified)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("TraceObjectManager::removeUserClientDebugSession : removed last session id [") + userClientId + string ("]. Client notofication status [") + FrameworkToolKit::localizeToSourceCodeEnum (clientNotificationStatus) + string ("]"));
    }
    else
    {
        if (true == isAnExistingDebugSession)
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("TraceObjectManager::removeUserClientDebugSession : removed session id [") + userClientId + string ("]"));
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_DEBUG, string ("TraceObjectManager::removeUserClientDebugSession : unknown session id [") + userClientId + string ("]"));
        }
    }
}

UI32 TraceObjectManager::getUserClientDebugSessionSize()
{
    UI32 size = 0;
    m_clientDebugSessionsMutex.lock ();
    size = m_userClientDebugSessions.size();
    m_clientDebugSessionsMutex.unlock ();

    return (size);
}

void TraceObjectManager::printToAllDebugSessions (const string &messageToBePrinted)
{
    m_clientDebugSessionsMutex.lock ();

    //map<UI32, UI32>::iterator element     = m_nativeClientDebugSessions.begin ();
    //map<UI32, UI32>::iterator endElement  = m_nativeClientDebugSessions.end   ();
    map<UI32, UI32>::iterator element1    = m_userClientDebugSessions.begin   ();
    map<UI32, UI32>::iterator endElement1 = m_userClientDebugSessions.end     ();

    // First run through all native client debug sessions.

    // This code seems a dead code as it is not doing anything effective. Hence commenting it out.
    // Similarly, the variables element and endElement are not being used anywhere else hence
    // commented them too.
    //while (element != endElement)
    //{
        //element++;
    //}

    // Next, run through all user client debug sessions.

    if (NULL != m_pWaveUserClientPrintFunction)
    {
        while (element1 != endElement1)
        {
            (*m_pWaveUserClientPrintFunction) (element1->first, messageToBePrinted);

            element1++;
        }
    }

    m_clientDebugSessionsMutex.unlock ();

}

void TraceObjectManager::printToAllWaveDebugClientSessionsMessageHandler (TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage *pTraceObjectManagerPrintToAllWaveDebugClientSessionsMessage)
{
    const string messageToBePrinted = pTraceObjectManagerPrintToAllWaveDebugClientSessionsMessage->getMessageToBePrinted ();

    printToAllDebugSessions (messageToBePrinted);

    pTraceObjectManagerPrintToAllWaveDebugClientSessionsMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pTraceObjectManagerPrintToAllWaveDebugClientSessionsMessage);

    return;
}

void TraceObjectManager::rotateTraceFile(bool noSizeCheck)
{
    int             i = 0, j = 0;
    string          tmpFileName;
    string          tmpFileName2;
    string          command;
    string          dirname = "/";
    string          basename;
    ifstream        ifile;
    vector<string>  output;
    vector<string>  pathVec;

    if ( m_currentTraceFileSize >= m_maxTraceFileSize || noSizeCheck)
    {
        //cout << "TraceObjectManager::rotateTraceFile : File " << s_traceFileName << " size reached/crossed max. Need to rotate" << endl << "\r";

        tokenize(s_traceFileName, pathVec, '/');
        //cout << "TraceObjectManager::rotateTraceFile : Retrieving dirname & basename from path. pathVec size=>" << pathVec.size() << endl << "\r";

        basename = pathVec[pathVec.size()-1];
        if (pathVec.size() > 1)
        {
            for (i=0; (UI32)i<pathVec.size()-1; i++)
            {
                dirname += pathVec[i] + "/";
            }
        }
        //cout << "TraceObjectManager::rotateTraceFile : Dirname is " << dirname << endl << "\r";
        //cout << "TraceObjectManager::rotateTraceFile : Basename is " << basename << endl << "\r";

        // close the current file as we'll rotate it now
        traceFile.close();

        // if current size crosses the max size, we'll rotate the old files
        for (i = (m_maxTraceFilesToKeep - 1); i >= 1; i--)
        {
            // create a filename with numbered suffix & zip suffix
            tmpFileName.clear();
            tmpFileName = dirname + basename + "." + i + ".gz";

            // check if file to rotate exists
            //cout << "TraceObjectManager::rotateTraceFile : Checking if file " << tmpFileName << " exists" << endl << "\r";
            ifile.open(tmpFileName.c_str(), ios::in);
            if ( ifile.is_open() == false )
            {
                // file does not exist
                //cout << "TraceObjectManager::rotateTraceFile : File " << tmpFileName << " does not exist. Continue to check next one" << endl << "\r";

                // check for next lower numbered file
                continue;
            }

            // rename to next higher number
            tmpFileName.clear();
            tmpFileName = dirname + basename + "." + i + ".gz";

            j = i + 1;
            tmpFileName2.clear();
            tmpFileName2 = dirname + basename + "." + j + ".gz";

            command.clear();
            command = "/bin/mv " + tmpFileName + " " + tmpFileName2;
            FrameworkToolKit::systemCommandOutput (command, output);
        }

        // Move the current file to .1
        command.clear();
        command = "/bin/mv " + dirname + basename + " " + dirname + basename + ".1";
        FrameworkToolKit::systemCommandOutput (command, output);

        // zip .1 file above
        command.clear();
        command = "/bin/gzip " + dirname + basename + ".1";
        FrameworkToolKit::systemCommandOutput (command, output);

        // create a new file
        m_isFirstTime = true;
    }

    return;

}


void TraceObjectManager::messageHistoryConfigForAServiceMessageHandler (TraceObjectManagerMessageHistoryConfigForAServiceMessage *pMessageHistoryConfigForAServiceMessage)
{
    PrismServiceId  prismServiceId          = pMessageHistoryConfigForAServiceMessage->getPrismServiceId ();
    bool            messageHistoryState     = pMessageHistoryConfigForAServiceMessage->getMessageHistoryState ();
    UI32            messageHistoryMaxSize   = pMessageHistoryConfigForAServiceMessage->getMessageHistoryMaxSize ();
    ResourceId      status                  = WAVE_MESSAGE_ERROR;
    ResourceId      completionStatus        = WAVE_MESSAGE_ERROR;
    string          messageString           = "";

    PrismServiceId  thisServiceId           = getServiceId ();

    LocationId      thisLocationId          = FrameworkToolKit::getThisLocationId ();
    LocationId      primaryLocationId       = FrameworkToolKit::getClusterPrimaryLocationId ();

    if (thisServiceId == prismServiceId)
    {
        // This message is for local service
        updateMessageHistoryConfig (messageHistoryState, messageHistoryMaxSize);
        completionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else if ((false == FrameworkToolKit::isALocalService (prismServiceId)) && (thisLocationId != primaryLocationId))
    {
        // It is a global service, and this location is not primary location.
        completionStatus = WAVE_MESSAGE_ERROR;
        messageString   += "Message History config for a global service should be done at primary node.";
    }
    else if (true == WaveNs::WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))
    {
        // These services dont do normal messaging
        completionStatus = WAVE_MESSAGE_ERROR;
        messageString += "Not a supported service.";
    }
    else
    {
        WaveObjectManagerMessageHistoryConfigMessage waveObjectManagerMessageHistoryConfigMessage (prismServiceId);
        waveObjectManagerMessageHistoryConfigMessage.setMessageHistoryState (messageHistoryState);
        waveObjectManagerMessageHistoryConfigMessage.setMessageHistoryMaxSize (messageHistoryMaxSize);

        status = sendSynchronously (&waveObjectManagerMessageHistoryConfigMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = waveObjectManagerMessageHistoryConfigMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                completionStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // service returned error in completion status.
                messageString += "Service returned failure status.";
            }
        }
        else
        {
            // could not send message to service.
            messageString += "Could not send message to service.";
        }
    }
    pMessageHistoryConfigForAServiceMessage->setMessageString (messageString);
    pMessageHistoryConfigForAServiceMessage->setCompletionStatus(completionStatus);
    reply (pMessageHistoryConfigForAServiceMessage);
}

void TraceObjectManager::messageHistoryDumpForAServiceMessageHandler (TraceObjectManagerMessageHistoryDumpForAServiceMessage *pMessageHistoryDumpForAServiceMessage)
{
    PrismServiceId  prismServiceId          = pMessageHistoryDumpForAServiceMessage->getPrismServiceId ();
    ResourceId      status                  = WAVE_MESSAGE_ERROR;
    ResourceId      completionStatus        = WAVE_MESSAGE_ERROR;
    string          messageString           = "";

    PrismServiceId  thisServiceId           = getServiceId ();

    LocationId      thisLocationId          = FrameworkToolKit::getThisLocationId ();
    LocationId      primaryLocationId       = FrameworkToolKit::getClusterPrimaryLocationId ();

    if (thisServiceId == prismServiceId)
    {
        // This message is for local service
        vector<string>  messageHistoryDumpStringVector;
        getMessageHistoryDumpStringVector (messageHistoryDumpStringVector);
        pMessageHistoryDumpForAServiceMessage->setMessageHistoryDumpStringVector (messageHistoryDumpStringVector);
        completionStatus = WAVE_MESSAGE_SUCCESS;
    }
    else if ((false == FrameworkToolKit::isALocalService (prismServiceId)) && (thisLocationId != primaryLocationId))
    {
        // It is a global service, and this location is not primary location.
        completionStatus = WAVE_MESSAGE_ERROR;
        messageString   += "Message History config for a global service should be done at primary node.";
    }
    else if (true == WaveNs::WaveLocalObjectManagerForUserSpecificTasks::isAUserSpecificService (prismServiceId))
    {
        // These services dont do normal messaging
        completionStatus = WAVE_MESSAGE_ERROR;
        messageString   += "Not a supported service.";
    }
    else
    {
        WaveObjectManagerMessageHistoryDumpMessage waveObjectManagerMessageHistoryDumpMessage (prismServiceId);

        status = sendSynchronously (&waveObjectManagerMessageHistoryDumpMessage);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            status = waveObjectManagerMessageHistoryDumpMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                pMessageHistoryDumpForAServiceMessage->setMessageHistoryDumpStringVector (waveObjectManagerMessageHistoryDumpMessage.getMessageHistoryDumpStringVector ());
                completionStatus = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                // service returned error in completion status.
                messageString += "Service returned failure status.";
            }
        }
        else
        {
            // could not send message to service.
            messageString += "Could not send message to service.";
        }
    }
    pMessageHistoryDumpForAServiceMessage->setMessageString (messageString);
    pMessageHistoryDumpForAServiceMessage->setCompletionStatus(completionStatus);
    reply (pMessageHistoryDumpForAServiceMessage);
}

void TraceObjectManager::zeroize (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContext)
{
    string secTraceFileName = "/mnt" + s_traceFileName;

    removeTraces(s_traceFileName);
    removeTraces(secTraceFileName);
    FrameworkToolKit::secureClearFile(secTraceFileName.c_str(),true);

    pWaveAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContext->callback ();

}

void TraceObjectManager::removeTraces(const string &filePath)
{
    size_t          position      = 0;
    string          traceDirName  = Wave::getTraceFileDirectory () + "/";
    string          traceFileName  ("");

    position = filePath.find(traceDirName);
    if (string::npos != position)
    {
        traceFileName = filePath.substr(position + traceDirName.length ());
    }
    if (filePath.length() != (traceDirName.length () + traceFileName.length ()))        // this will be true for /mnt filepath
    {
        position = filePath.find(traceFileName);
        traceDirName = filePath.substr (0, position);
    }
    cout << "traceFileName : " << traceFileName <<endl <<"\r";
    cout << "traceDirName: " << traceDirName <<endl <<"\r";


    // read each file in a directory and select trace files to secure delete
    DIR     *pDIR               = NULL;
    struct  dirent *entry       = NULL;
    string  filenameForMatch    = traceFileName + ".";

    if( (pDIR=opendir(traceDirName.c_str())) != NULL)
    {
        while((entry = readdir(pDIR)) != NULL)
        {
            if( 0 == strncmp ( entry->d_name, filenameForMatch.c_str(), filenameForMatch.length()))
            {
                string fullFilePath = traceDirName + entry->d_name;
                FrameworkToolKit::secureClearFile(fullFilePath.c_str(),true);
            }
        }
        closedir(pDIR);
    }
    else
    {
        cout << "TraceObjectManager::removeTraces : failed to open Trace file directory";
    }
}

string TraceObjectManager::getTraceTagForLevel (const TraceLevel& requestedTraceLevel) {
    string traceStr;
    switch (requestedTraceLevel) {
    case TRACE_LEVEL_UNKNOWN:
        traceStr = "UNKNOWN: ";
        break;
    case TRACE_LEVEL_DEVEL:
        traceStr = "DEVEL  : ";
        break;
    case TRACE_LEVEL_DEBUG:
        traceStr = "DEBUG  : ";
        break;
    case TRACE_LEVEL_INFO:
        traceStr = "INFO   : ";
        break;
    case TRACE_LEVEL_PERF_START:
        traceStr = "PERF_S : ";
        break;
    case TRACE_LEVEL_PERF_END:
        traceStr = "PERF_E : ";
        break;
    case TRACE_LEVEL_SUCCESS:
        traceStr = "SUCCESS: ";
        break;
    case TRACE_LEVEL_WARN:
        traceStr = "WARN   : ";
        break;
    case TRACE_LEVEL_ERROR:
        traceStr = "ERROR  : ";
        break;
    case TRACE_LEVEL_FATAL:
        traceStr = "FATAL  : ";
        break;
    }

    return traceStr;
}

}

