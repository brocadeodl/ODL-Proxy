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

#ifndef TRACEMESSAGES_H
#define TRACEMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Trace/TraceTypes.h"
#include "Framework/ObjectModel/WaveClientSessionContext.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

#include <vector>

using namespace std;

namespace WaveNs
{

typedef enum
{
    TRACE_OPERATION_CODE_TRACE = 0,
    TRACE_OPERATION_CODE_SET_CLIENT_TRACE_LEVEL,
    TRACE_OPERATION_CODE_GET_CLIENT_TRACE_LEVEL,
    TRACE_OPERATION_CODE_GET_CLIENTS_INFORMATION,
    TRACE_OPERATION_CODE_PRINT_TO_WAVE_CLIENT_SESSION,
    TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_CLIENT_SESSIONS,
    TRACE_OPERATION_CODE_PRINT_TO_ALL_WAVE_DEBUG_CLIENT_SESSIONS,
    TRACE_OPERATION_CODE_MESSAGE_HISTORY_CONFIG_FOR_A_SERVICE,
    TRACE_OPERATION_CODE_MESSAGE_HISTORY_DUMP_FOR_A_SERVICE
} TraceOperationCodes;

typedef enum
{
    TRACE_INPUT_BUFFER = 0
} TraceMessageBufferTags;

class TraceMessage : public PrismMessage
{
    private :
                void addInput                        (const string &inputString);
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                       TraceMessage     (TraceClientId traceClientId, TraceLevel traceLevel, const string &inputString, const bool &addNewLine = true);
                       TraceMessage     ();
                      ~TraceMessage     ();
        TraceClientId  getTraceClientId () const;
        TraceLevel     getTraceLevel    () const;
        bool           getAddNewLine    () const;

    // Now the data members

    private :
        TraceClientId m_traceClientId;
        ResourceId    m_traceLevel;
        bool          m_addNewLine;

    protected :
    public:
};

class TraceObjectManagerSetClientTraceLevelMessage : public ManagementInterfaceMessage 
{
    private :
    protected :
        TraceObjectManagerSetClientTraceLevelMessage (string className, UI32 operationCode, TraceClientId traceClientId, TraceLevel traceLevel, bool isSetAll = false);
        TraceObjectManagerSetClientTraceLevelMessage (string className, UI32 operationCode);

    public :
                               TraceObjectManagerSetClientTraceLevelMessage ();
                               TraceObjectManagerSetClientTraceLevelMessage (TraceClientId traceClientId, TraceLevel traceLevel, bool isSetAll = false);
        virtual               ~TraceObjectManagerSetClientTraceLevelMessage ();
                TraceClientId  getTraceClientId                             ();
                TraceLevel     getTraceLevel                                ();
                bool           getIsSetAll                                  () const;
        virtual void           setupAttributesForSerialization              ();

    // Now the data members

    private :
    protected :
        TraceClientId m_traceClientId;
        ResourceId    m_traceLevel; // TraceLevel
        bool          m_isSetAll;

    public:
};

class TraceObjectManagerGetClientTraceLevelMessage : public TraceObjectManagerSetClientTraceLevelMessage
{
    private :
    protected :
    public :
                      TraceObjectManagerGetClientTraceLevelMessage (TraceClientId traceClientId);
                      TraceObjectManagerGetClientTraceLevelMessage ();
        virtual      ~TraceObjectManagerGetClientTraceLevelMessage ();
                void  setTraceLevel                                (TraceLevel traceLevel);

    // Now the data members

    private :
    protected :
    public:
};

class TraceObjectManagerGetClientsInformationMessage : public ManagementInterfaceMessage
{
    private :

                void addTraceClientInformation       (const TraceClientId &traceClientId, TraceLevel traceLevel);
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                               TraceObjectManagerGetClientsInformationMessage ();
        virtual               ~TraceObjectManagerGetClientsInformationMessage ();

                UI32           getNumberOfTraceClients                        ();
                TraceClientId  getTraceClientIdAt                             (const UI32 &i);
                TraceLevel     getTraceLevelAt                                (const UI32 &i);
                string         getServiceNameAt                               (const UI32 &i);

    // Now the data members

    private :
        vector<TraceClientId> m_traceClientIds;
        vector<UI32>          m_traceClientLevels;
        vector<string>        m_serviceNames;

    protected :
    public :

    friend class TraceObjectManager;
};

class TraceObjectManagerPrintToWaveClientSessionMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    TraceObjectManagerPrintToWaveClientSessionMessage ();
                    TraceObjectManagerPrintToWaveClientSessionMessage (const LocationId &clientOriginatingLocationId, const UI32 &nativeClientId, const UI32 &userClientId, const string &messageToBePrinted);
                    TraceObjectManagerPrintToWaveClientSessionMessage (const WaveClientSessionContext &waveClientSessionContext, const string &messageToBePrinted);
                   ~TraceObjectManagerPrintToWaveClientSessionMessage ();

        void        setClientOriginatingLocationId                    (const LocationId &clientOriginatingLocationId);
        LocationId  getClientOriginatingLocationId                    () const;
        void        setNativeClientId                                 (const UI32 &nativeClientId);
        UI32        getNativeClientId                                 () const;
        void        setUserClientId                                   (const UI32 &userClientId);
        UI32        getUserClientId                                   () const;
        void        setMessageToBePrinted                             (const string &messageToBePrinted);
        string      getMessageToBePrinted                             () const;

    // Now the data members

    private :
        LocationId m_clientOriginatingLocationId;
        UI32       m_nativeClientId;
        UI32       m_userClientId;

        string     m_messageToBePrinted;

    protected :
    public :
};

class TraceObjectManagerPrintToAllWaveClientSessionsMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    TraceObjectManagerPrintToAllWaveClientSessionsMessage ();
                    TraceObjectManagerPrintToAllWaveClientSessionsMessage (const LocationId &clientOriginatingLocationId, const string &messageToBePrinted);
                    TraceObjectManagerPrintToAllWaveClientSessionsMessage (const WaveClientSessionContext &waveClientSessionContext, const string &messageToBePrinted);
                   ~TraceObjectManagerPrintToAllWaveClientSessionsMessage ();

        void        setClientOriginatingLocationId                    (const LocationId &clientOriginatingLocationId);
        LocationId  getClientOriginatingLocationId                    () const;
        void        setMessageToBePrinted                             (const string &messageToBePrinted);
        string      getMessageToBePrinted                             () const;

    // Now the data members

    private :
        LocationId m_clientOriginatingLocationId;

        string     m_messageToBePrinted;

    protected :
    public :
};

class TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();
        void incrementOutstandingMessages    ();
        void decrementOutstandingMessages    ();

    protected :
    public :
                    TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage ();
                    TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage (const string &messageToBePrinted);
                   ~TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage ();

        void        setMessageToBePrinted                                      (const string &messageToBePrinted);
        string      getMessageToBePrinted                                      () const;

    // Now the data members

    private :
                string          m_messageToBePrinted;
        static  PrismMutex      m_synchronizingMutex;   
        static  PrismCondition  m_synchronizingCondition; 
        static  UI32            m_outStandingMessages;
        
        static const UI32       m_maxOutStandingMessages;


    protected :
    public :
};


// This message will come from client.
class TraceObjectManagerMessageHistoryDumpForAServiceMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();
    protected :
    public :
                                TraceObjectManagerMessageHistoryDumpForAServiceMessage  ();
                                TraceObjectManagerMessageHistoryDumpForAServiceMessage  (const PrismServiceId prismServiceId);
                               ~TraceObjectManagerMessageHistoryDumpForAServiceMessage  ();

        PrismServiceId          getPrismServiceId                                       () const;
        vector<string>         &getMessageHistoryDumpStringVector                       ();
        void                    setMessageHistoryDumpStringVector                       (vector<string> &messageHistoryDumpStringVector);

    // Now the data members

    private :
        PrismServiceId      m_prismServiceId;
        vector<string>      m_messageHistoryDumpStringVector;

    protected :
    public :
};

// This message will come from client.
class TraceObjectManagerMessageHistoryConfigForAServiceMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();
    protected :
    public :
                                TraceObjectManagerMessageHistoryConfigForAServiceMessage ();
                                TraceObjectManagerMessageHistoryConfigForAServiceMessage (PrismServiceId prismServiceId);
                               ~TraceObjectManagerMessageHistoryConfigForAServiceMessage ();

        PrismServiceId          getPrismServiceId                                        () const;
        bool                    getMessageHistoryState                                   () const;
        void                    setMessageHistoryState                                   (bool messageHistoryState);
        UI32                    getMessageHistoryMaxSize                                 () const;
        void                    setMessageHistoryMaxSize                                 (UI32 messageHistoryMaxSize);

    // Now the data members

    private :
        PrismServiceId      m_prismServiceId;
        bool                m_messageHistoryState;
        UI32                m_messageHistoryMaxSize;

    protected :
    public :
};

}

#endif //TRACEMESSAGES_H
