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

#include "Framework/Trace/TraceMessages.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

static const UI32 GET_CLIENTS_INFORMATION_IDS_TAG    = 0;
static const UI32 GET_CLIENTS_INFORMATION_LEVELS_TAG = 1;

TraceMessage::TraceMessage (TraceClientId traceClientId, TraceLevel traceLevel, const string &inputString, const bool &addNewLine)
    : PrismMessage    (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_TRACE),
      m_traceClientId (traceClientId),
      m_traceLevel    (traceLevel),
      m_addNewLine    (addNewLine)
{
    addInput (inputString);
}

TraceMessage::TraceMessage ()
    : PrismMessage    (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_TRACE),
      m_traceClientId (0),
      m_traceLevel    (TRACE_LEVEL_UNKNOWN),
      m_addNewLine    (false)
{
}

TraceMessage::~TraceMessage ()
{
}

void TraceMessage::addInput (const string &inputString)
{
    addBuffer (TRACE_INPUT_BUFFER, inputString.length (), inputString.data (), false);
}

TraceClientId TraceMessage::getTraceClientId () const
{
    return (m_traceClientId);
}

TraceLevel TraceMessage::getTraceLevel () const
{
    return (static_cast<TraceLevel> (m_traceLevel));
}

bool TraceMessage::getAddNewLine () const
{
    return (m_addNewLine);
}

void TraceMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeTraceClientId (&m_traceClientId,    "traceClientId"));
     addSerializableAttribute (new AttributeTraceLevel    (&m_traceLevel,       "traceLevel"));
     addSerializableAttribute (new AttributeBool          (&m_addNewLine,       "addNewLine"));
}

TraceObjectManagerSetClientTraceLevelMessage::TraceObjectManagerSetClientTraceLevelMessage (string className, UI32 operationCode, TraceClientId traceClientId, TraceLevel traceLevel, bool isSetAll)
    : ManagementInterfaceMessage (className, operationCode),
      m_traceClientId (traceClientId),
      m_traceLevel    (traceLevel),
      m_isSetAll      (isSetAll)
{
}

TraceObjectManagerSetClientTraceLevelMessage::TraceObjectManagerSetClientTraceLevelMessage (string className, UI32 operationCode)
    : ManagementInterfaceMessage (className, operationCode),
      m_traceClientId (0),
      m_traceLevel    (TRACE_OPERATION_CODE_TRACE),
      m_isSetAll      (false)
{
}

TraceObjectManagerSetClientTraceLevelMessage::TraceObjectManagerSetClientTraceLevelMessage ()
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_SET_CLIENT_TRACE_LEVEL),
      m_traceClientId (0),
      m_traceLevel    (TRACE_OPERATION_CODE_TRACE),
      m_isSetAll      (false)
{
}

TraceObjectManagerSetClientTraceLevelMessage::TraceObjectManagerSetClientTraceLevelMessage (TraceClientId traceClientId, TraceLevel traceLevel, bool isSetAll)
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_SET_CLIENT_TRACE_LEVEL),
      m_traceClientId (traceClientId),
      m_traceLevel    (traceLevel),
      m_isSetAll      (isSetAll)
{
}

TraceObjectManagerSetClientTraceLevelMessage::~TraceObjectManagerSetClientTraceLevelMessage ()
{
}

TraceClientId TraceObjectManagerSetClientTraceLevelMessage::getTraceClientId ()
{
    return (m_traceClientId);
}

TraceLevel TraceObjectManagerSetClientTraceLevelMessage::getTraceLevel ()
{
    return (static_cast<TraceLevel> (m_traceLevel));
}

bool TraceObjectManagerSetClientTraceLevelMessage::getIsSetAll () const
{
    return (m_isSetAll);
}

void TraceObjectManagerSetClientTraceLevelMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeTraceClientId (&m_traceClientId,    "traceClientId"));
     addSerializableAttribute (new AttributeTraceLevel    (&m_traceLevel,       "traceLevel"));
     addSerializableAttribute (new AttributeBool          (&m_isSetAll,         "isSetAll"));
}

TraceObjectManagerGetClientTraceLevelMessage::TraceObjectManagerGetClientTraceLevelMessage (TraceClientId traceClientId)
    : TraceObjectManagerSetClientTraceLevelMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_GET_CLIENT_TRACE_LEVEL, traceClientId, TRACE_LEVEL_UNKNOWN)
{
}

TraceObjectManagerGetClientTraceLevelMessage::TraceObjectManagerGetClientTraceLevelMessage ()
    : TraceObjectManagerSetClientTraceLevelMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_GET_CLIENT_TRACE_LEVEL)
{
}

TraceObjectManagerGetClientTraceLevelMessage::~TraceObjectManagerGetClientTraceLevelMessage ()
{
}

void TraceObjectManagerGetClientTraceLevelMessage::setTraceLevel (TraceLevel traceLevel)
{
    m_traceLevel = traceLevel;
}

TraceObjectManagerGetClientsInformationMessage::TraceObjectManagerGetClientsInformationMessage ()
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_GET_CLIENTS_INFORMATION)
{
}

TraceObjectManagerGetClientsInformationMessage::~TraceObjectManagerGetClientsInformationMessage ()
{
}

void TraceObjectManagerGetClientsInformationMessage::addTraceClientInformation (const TraceClientId &traceClientId, TraceLevel traceLevel)
{
    m_traceClientIds.push_back (traceClientId);
    m_traceClientLevels.push_back (traceLevel);
    m_serviceNames.push_back (FrameworkToolKit::getTraceClientNameById (traceClientId));
}

UI32 TraceObjectManagerGetClientsInformationMessage::getNumberOfTraceClients ()
{
    return (m_traceClientIds.size ());
}

TraceClientId TraceObjectManagerGetClientsInformationMessage::getTraceClientIdAt (const UI32 &i)
{
    UI32 numberOfTraceClients = getNumberOfTraceClients ();

    prismAssert (i < numberOfTraceClients, __FILE__, __LINE__);

    if (i >= numberOfTraceClients)
    {
        return (0);
    }

    return (m_traceClientIds[i]);
}

TraceLevel TraceObjectManagerGetClientsInformationMessage::getTraceLevelAt (const UI32 &i)
{
    UI32 numberOfTraceClients = getNumberOfTraceClients ();

    prismAssert (i < numberOfTraceClients, __FILE__, __LINE__);

    if (i >= numberOfTraceClients)
    {
        return (TRACE_LEVEL_UNKNOWN);
    }

    return ((TraceLevel) (m_traceClientLevels[i]));
}

string TraceObjectManagerGetClientsInformationMessage::getServiceNameAt (const UI32 &i)
{
    UI32 numberOfTraceClients = getNumberOfTraceClients ();

    prismAssert (i < numberOfTraceClients, __FILE__, __LINE__);

    if (i >= numberOfTraceClients)
    {
        return (0);
    }

    return (m_serviceNames[i]);
}

void TraceObjectManagerGetClientsInformationMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeTraceClientIdVector (&m_traceClientIds,                             "traceClientIds"));
    addSerializableAttribute (new AttributeUI32Vector (reinterpret_cast<vector<UI32> *> (&m_traceClientLevels), "traceClientLevels"));
     addSerializableAttribute (new AttributeStringVector (&m_serviceNames,                                      "serviceNames"));
}

TraceObjectManagerPrintToWaveClientSessionMessage::TraceObjectManagerPrintToWaveClientSessionMessage ()
    : PrismMessage (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_PRINT_TO_WAVE_CLIENT_SESSION),
      m_clientOriginatingLocationId(0),
      m_nativeClientId (0),
      m_userClientId (0)
{
}

TraceObjectManagerPrintToWaveClientSessionMessage::TraceObjectManagerPrintToWaveClientSessionMessage (const LocationId &clientOriginatingLocationId, const UI32 &nativeClientId, const UI32 &userClientId, const string &messageToBePrinted)
    : PrismMessage                  (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_PRINT_TO_WAVE_CLIENT_SESSION),
      m_clientOriginatingLocationId (clientOriginatingLocationId),
      m_nativeClientId              (nativeClientId),
      m_userClientId                (userClientId),
      m_messageToBePrinted          (messageToBePrinted)
{
}

TraceObjectManagerPrintToWaveClientSessionMessage::TraceObjectManagerPrintToWaveClientSessionMessage (const WaveClientSessionContext &waveClientSessionContext, const string &messageToBePrinted)
    : PrismMessage                  (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_PRINT_TO_WAVE_CLIENT_SESSION),
      m_clientOriginatingLocationId (waveClientSessionContext.getWaveClientOriginatingLocationId ()),
      m_nativeClientId              (waveClientSessionContext.getWaveNativeClientId ()),
      m_userClientId                (waveClientSessionContext.getWaveUserClientId ()),
      m_messageToBePrinted          (messageToBePrinted)
{
}

TraceObjectManagerPrintToWaveClientSessionMessage::~TraceObjectManagerPrintToWaveClientSessionMessage ()
{
}

void TraceObjectManagerPrintToWaveClientSessionMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeLocationId (&m_clientOriginatingLocationId, "clientOriginatingLocationId"));
     addSerializableAttribute (new AttributeUI32       (&m_nativeClientId,              "nativeClientId"));
     addSerializableAttribute (new AttributeUI32       (&m_userClientId,                "userClientId"));
     addSerializableAttribute (new AttributeString     (&m_messageToBePrinted,          "messageToBePrinted"));
}

void TraceObjectManagerPrintToWaveClientSessionMessage::setClientOriginatingLocationId (const LocationId &clientOriginatingLocationId)
{
    m_clientOriginatingLocationId = clientOriginatingLocationId;
}

LocationId TraceObjectManagerPrintToWaveClientSessionMessage::getClientOriginatingLocationId () const
{
    return (m_clientOriginatingLocationId);
}

void TraceObjectManagerPrintToWaveClientSessionMessage::setNativeClientId (const UI32 &nativeClientId)
{
    m_nativeClientId = nativeClientId;
}

UI32 TraceObjectManagerPrintToWaveClientSessionMessage::getNativeClientId () const
{
    return (m_nativeClientId);
}

void TraceObjectManagerPrintToWaveClientSessionMessage::setUserClientId (const UI32 &userClientId)
{
    m_userClientId = userClientId;
}

UI32 TraceObjectManagerPrintToWaveClientSessionMessage::getUserClientId () const
{
    return (m_userClientId);
}

void TraceObjectManagerPrintToWaveClientSessionMessage::setMessageToBePrinted (const string &messageToBePrinted)
{
    m_messageToBePrinted = messageToBePrinted;
}

string TraceObjectManagerPrintToWaveClientSessionMessage::getMessageToBePrinted () const
{
    return (m_messageToBePrinted);
}

TraceObjectManagerPrintToAllWaveClientSessionsMessage::TraceObjectManagerPrintToAllWaveClientSessionsMessage ()
    : PrismMessage (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_CLIENT_SESSIONS),
      m_clientOriginatingLocationId (0)
{
}

TraceObjectManagerPrintToAllWaveClientSessionsMessage::TraceObjectManagerPrintToAllWaveClientSessionsMessage (const LocationId &clientOriginatingLocationId, const string &messageToBePrinted)
    : PrismMessage                  (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_CLIENT_SESSIONS),
      m_clientOriginatingLocationId (clientOriginatingLocationId),
      m_messageToBePrinted          (messageToBePrinted)
{
}

TraceObjectManagerPrintToAllWaveClientSessionsMessage::TraceObjectManagerPrintToAllWaveClientSessionsMessage (const WaveClientSessionContext &waveClientSessionContext, const string &messageToBePrinted)
    : PrismMessage                  (TraceObjectManager::getPrismServiceId (), TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_CLIENT_SESSIONS),
      m_clientOriginatingLocationId (waveClientSessionContext.getWaveClientOriginatingLocationId ()),
      m_messageToBePrinted          (messageToBePrinted)
{
}

TraceObjectManagerPrintToAllWaveClientSessionsMessage::~TraceObjectManagerPrintToAllWaveClientSessionsMessage ()
{
}

void TraceObjectManagerPrintToAllWaveClientSessionsMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeLocationId (&m_clientOriginatingLocationId, "clientOriginatingLocationId"));
     addSerializableAttribute (new AttributeString     (&m_messageToBePrinted,          "messageToBePrinted"));
}

void TraceObjectManagerPrintToAllWaveClientSessionsMessage::setClientOriginatingLocationId (const LocationId &clientOriginatingLocationId)
{
    m_clientOriginatingLocationId = clientOriginatingLocationId;
}

LocationId TraceObjectManagerPrintToAllWaveClientSessionsMessage::getClientOriginatingLocationId () const
{
    return (m_clientOriginatingLocationId);
}

void TraceObjectManagerPrintToAllWaveClientSessionsMessage::setMessageToBePrinted (const string &messageToBePrinted)
{
    m_messageToBePrinted = messageToBePrinted;
}

string TraceObjectManagerPrintToAllWaveClientSessionsMessage::getMessageToBePrinted () const
{
    return (m_messageToBePrinted);
}

PrismMutex      TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::m_synchronizingMutex;
PrismCondition  TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::m_synchronizingCondition (&TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::m_synchronizingMutex);
UI32            TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::m_outStandingMessages    = 0;
const UI32      TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::m_maxOutStandingMessages = 1024;

TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage ()
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_DEBUG_CLIENT_SESSIONS)
{
    incrementOutstandingMessages ();
}

TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage (const string &messageToBePrinted)
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_DEBUG_CLIENT_SESSIONS),
      m_messageToBePrinted       (messageToBePrinted)
{
    incrementOutstandingMessages ();
}

TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::~TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage ()
{
    decrementOutstandingMessages ();
}

void TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_messageToBePrinted, "messageToBePrinted"));
}

void TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::setMessageToBePrinted (const string &messageToBePrinted)
{
    m_messageToBePrinted = messageToBePrinted;
}

string TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::getMessageToBePrinted () const
{
    return (m_messageToBePrinted);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use the standard Mutex-Condition combination logic to throttle the sender thread
// The sender thread will wait until at least m_maxOutStandingMessages are available
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::incrementOutstandingMessages ()
{
    m_synchronizingMutex.lock ();

    if (m_outStandingMessages >= m_maxOutStandingMessages)
    {
        m_synchronizingCondition.wait ();
    }

    m_outStandingMessages++;

    m_synchronizingMutex.unlock ();
}

void TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage::decrementOutstandingMessages ()
{
    m_synchronizingMutex.lock ();

    m_outStandingMessages--;

    if (m_outStandingMessages <= m_maxOutStandingMessages)
    {
        m_synchronizingCondition.resumeAll ();
    }

    m_synchronizingMutex.unlock ();
}


TraceObjectManagerMessageHistoryDumpForAServiceMessage::TraceObjectManagerMessageHistoryDumpForAServiceMessage (PrismServiceId prismServiceId)
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_MESSAGE_HISTORY_DUMP_FOR_A_SERVICE),
    m_prismServiceId            (prismServiceId)
{
}

TraceObjectManagerMessageHistoryDumpForAServiceMessage::TraceObjectManagerMessageHistoryDumpForAServiceMessage ()
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_MESSAGE_HISTORY_DUMP_FOR_A_SERVICE),
    m_prismServiceId (0)
{
}

TraceObjectManagerMessageHistoryDumpForAServiceMessage::~TraceObjectManagerMessageHistoryDumpForAServiceMessage ()
{
}

PrismServiceId TraceObjectManagerMessageHistoryDumpForAServiceMessage::getPrismServiceId () const
{
    return m_prismServiceId;
}

vector<string> &TraceObjectManagerMessageHistoryDumpForAServiceMessage::getMessageHistoryDumpStringVector ()
{
    return m_messageHistoryDumpStringVector;
}

void TraceObjectManagerMessageHistoryDumpForAServiceMessage::setMessageHistoryDumpStringVector (vector<string> &messageHistoryDumpStringVector)
{
    m_messageHistoryDumpStringVector = messageHistoryDumpStringVector;
}

void TraceObjectManagerMessageHistoryDumpForAServiceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32         (&m_prismServiceId,                "prismServiceId"));
     addSerializableAttribute (new AttributeStringVector (&m_messageHistoryDumpStringVector,"messageHistoryDumpStringVector"));
}


TraceObjectManagerMessageHistoryConfigForAServiceMessage::TraceObjectManagerMessageHistoryConfigForAServiceMessage ()
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_MESSAGE_HISTORY_CONFIG_FOR_A_SERVICE),
    m_prismServiceId            (0),
    m_messageHistoryState       (false),
    m_messageHistoryMaxSize     (0)
{
}

TraceObjectManagerMessageHistoryConfigForAServiceMessage::TraceObjectManagerMessageHistoryConfigForAServiceMessage (PrismServiceId prismServiceId)
    : ManagementInterfaceMessage (TraceObjectManager::getClassName (), TRACE_OPERATION_CODE_MESSAGE_HISTORY_CONFIG_FOR_A_SERVICE),
    m_prismServiceId            (prismServiceId),
    m_messageHistoryState       (false),
    m_messageHistoryMaxSize     (0)
{
}

TraceObjectManagerMessageHistoryConfigForAServiceMessage::~TraceObjectManagerMessageHistoryConfigForAServiceMessage ()
{
}

PrismServiceId TraceObjectManagerMessageHistoryConfigForAServiceMessage::getPrismServiceId () const
{
    return m_prismServiceId;
}

bool TraceObjectManagerMessageHistoryConfigForAServiceMessage::getMessageHistoryState () const
{
    return m_messageHistoryState;
}

void TraceObjectManagerMessageHistoryConfigForAServiceMessage::setMessageHistoryState (bool messageHistoryState)
{
    m_messageHistoryState = messageHistoryState;
}

UI32 TraceObjectManagerMessageHistoryConfigForAServiceMessage::getMessageHistoryMaxSize () const
{
    return m_messageHistoryMaxSize;
}

void TraceObjectManagerMessageHistoryConfigForAServiceMessage::setMessageHistoryMaxSize (UI32 messageHistoryMaxSize)
{
    m_messageHistoryMaxSize = messageHistoryMaxSize;
}

void TraceObjectManagerMessageHistoryConfigForAServiceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_prismServiceId,        "prismServiceId"));
     addSerializableAttribute (new AttributeBool (&m_messageHistoryState,   "messageHistoryState"));
     addSerializableAttribute (new AttributeUI32 (&m_messageHistoryMaxSize, "messageHistoryMaxSize"));
}

}
