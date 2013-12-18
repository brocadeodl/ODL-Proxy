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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVECLIENTTRANSPORTOBJECTMANAGER_H
#define WAVECLIENTTRANSPORTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveManagementServerMap.h"

namespace WaveNs
{

class ManagementInterfaceMessage;
class WaveManagementServer;

class WaveClientTransportObjectManager : public WaveLocalObjectManager
{
    private :
        class MessageMap
        {
            private :
                void lockAccess ();
                void unlockAccess ();

            protected :
            public :
                                            MessageMap                  ();
                                            ~MessageMap                  ();
                WaveMessageStatus           addMessage                  (ManagementInterfaceMessage *pManagementInterfaceMessage);
                ManagementInterfaceMessage *findMessage                 (UI32 prismMessageId);
                ManagementInterfaceMessage *removeMessage               (UI32 prismMessageId);
                void                        getPendingMessagesForServer (const UI32 &serverId, vector<ManagementInterfaceMessage *> &messagesVector);

            // Now the data members

            private :
                map<UI32, ManagementInterfaceMessage *> m_messagesMap;
                PrismMutex                              m_accessMutex;

            protected :
            public:
        };

                                    WaveClientTransportObjectManager                ();
        void                        managementInterfaceMessageHandler               (ManagementInterfaceMessage *pManagementInterfaceMessage);
        void                        managementInterfaceMessagePostToServerStep      (PrismLinearSequencerContext *pPrismLinearSequencerContext);

        void                        lockAccess                                      ();
        void                        unlockAccess                                    ();
        void                        lockAccess1                                     ();
        void                        unlockAccess1                                   ();
        void                        lockAccessForServerIdConnectionStatus           ();
        void                        unlockAccessForServerIdConnectionStatus         ();

        WaveMessageStatus           sendToBeUsedByReceiverThreads                   (ManagementInterfaceMessage *pManagementInterfaceMessage);
        void                        callbackForSendUsedByReceiverThreads            (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pManagementInterfaceMessage, void *pContext);
        ManagementInterfaceMessage *getPendingMessage                               (const UI32 &messageId);
        void                        replyToBeUsedByReceiverThreads                  (ManagementInterfaceMessage *pManagementInterfaceMessage);
        void                        replyToBeUsedByReceiverThreads                  (UI32 prismMessageId);
        void                        getPendingMessagesForServer                     (const UI32 &serverId, vector<ManagementInterfaceMessage *> &messagesVector);
        void                        replyToRemoteMessagesPendingForServer           (const UI32 &serverId, ResourceId completionStatus);

        ResourceId                  connectToNewManagementInterfaceServer           (const string &waveClientName, const string &waveClientNameWithoutExtension, const string &serverIpAddress, const SI32 &serverPort, UI32 &serverId);
        WaveManagementServer       *getServer                                       (const string &serverIpAddress, const SI32 &serverPort);
        UI32                        getServerId                                     (const string &serverIpAddress, const SI32 &serverPort);
        UI32                        getServerIdIfKnown                              (const string &serverIpAddress, const SI32 &serverPort);
        UI32                        getServerIdIfKnownForUniqueServerIdentifier     (const string &serverIpAddress, const SI32 &serverPort, const UI32 &uniqueServerIdentifierFromServerPerspective);
        void                        addServiceCache                                 (const UI32 &serverId, const vector<string> &serviceNames, const vector<UI32> &serviceIds);
        ResourceId                  postToServer                                    (ManagementInterfaceMessage *pManagementInterfaceMessage);


        PrismServiceId              getPrismServiceIdForServiceName                 (const UI32 &serverId, const string &serviceName);
        string                      getPrismServiceNameForServiceId                 (const UI32 &serverId, const PrismServiceId &serviceId);
        void                        addServerId                                     (const UI32 &serverId);
        void                        removeServerId                                  (const UI32 &serverId);
        bool                        validateIfInTheMiddleOfConnectingToServer       (const UI32 &serverId);

    protected :
    public :
        virtual                                  ~WaveClientTransportObjectManager  ();

        static  WaveClientTransportObjectManager *getInstance                       ();
        static  PrismServiceId                    getPrismServiceId                 ();
        void                                      disconnectFromServer              (const UI32 &serverId, const bool removeFromCache = true);
        void                                      disconnectFromServer              (const string &serverIpAddress, const SI32 &serverPort);
        void                                      replyToPendingMessagesForServer   (const string &serverIpAddress, const SI32 &serverPort);

        void                                      updateMessageVersionAndSerializationTypeForServer    (const string &serverIpAddress, const SI32 &serverPort, const string &version);

    // Now the Data Members

    private :
        MessageMap              m_remoteMessagesMap;
        PrismMutex              m_accessMutexForReceiverThreads;
        PrismMutex              m_accessMutexForMessagesMap;
        PrismMutex              m_accessServerIdConnectionStatus;

        set<UI32>               m_serverIdSet;                     // If there is entry in this set, it means client is in middle of connecting to server.
        WaveManagementServerMap m_serversMap;

    protected :
    public :

    friend class WaveClientReceiverThread;
    friend class WaveClientSynchronousConnection;
};

}

#endif // WAVECLIENTTRANSPORTOBJECTMANAGER_H
