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
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef APPINTERFACEOBJECTMANAGER_H
#define APPINTERFACEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Cluster/NodeManagedObject.h"
#include "App/AppMessages.h"
#include "App/AppInterface.h"

namespace WaveNs
{


class DataInfo
{
    private :
    protected :
    public :
        DataInfo  ();
        virtual        ~DataInfo  ();

// Now the data members

    private :
    protected :
    public :
        UI32    commandLen;
        void   *pCommand;
};

class CommandInfo
{
    private :
    protected :
    public :
                        CommandInfo  ();
        virtual        ~CommandInfo  ();

// Now the data members

    private :
    protected :
    public :

        duplicateCmdResultCallBack_t  duplicateCmdResultCB;
        void                         *pContext;
        UI32                          nNodes;
        UI32                          nReplies;
        vector <DataInfo>             dataDetail;
        remoteCmdResult_t            *pNodeResults;
        UI32                          status;
        UI32                          version;
        UI32                          nCommands;

};

class AppInterfaceObjectManager : public WaveLocalObjectManager
{
    private :
                                AppInterfaceObjectManager  (string appInterfaceName, const UI32 &stackSize = 0);
        virtual PrismMessage   *createMessageInstance      (const UI32 &operationCode);
        virtual void            failover                   (FailoverAsynchronousContext *pFailoverAsynchronousContext);

    protected :
    public :
        virtual                           ~AppInterfaceObjectManager          ();
                PrismServiceId             getPrismServiceId                  ();
        static  AppInterfaceObjectManager *createInstance                     (string appInterfaceName);

                int                        registerDuplicateCmdsHandler       (duplicateCmdsHandler_t  duplicateCmdHandler);
                void                       registerValidationHandler          (clusterValidationHandler_t m_clusterValidationHandler);
                void                       registerCollectValidationHandler   (clusterCollectValidationHandler_t   m_clusterCollectValidationHandler);
                void                       registerReportValidationHandler    (clusterReportValidationHandler_t    m_clusterReportValidationHandler);
                void                       registerNodeJoinedHandler          (memberJoinedCallBack_t nodeJoinedCallBack);

                void                       registerHeartbeatLossHandler       (heartBeatCallBack_t heartBeatLossCallBack);
                int                        duplicateCmds                      (unsigned int nCommands, commandData_t commandList [], vector <Node> *nodeNames, duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext);
                int                        duplicateMultiCastCmds             (unsigned int nCommands, commandData_t commandList [], unsigned int nNodes, node_t nodeList [], duplicateCmdResultCallBack_t duplicateCmdResultCB, void *pContext);
                int                        duplicateCmdsToClusterNodes        (UI32 nCommands, commandData_t commandList [], duplicateCmdResultCallBack_t  duplicateCmdResultCB, void *pContext);
                WaveMessageStatus         sendCommands                       (CommandInfo *pCommandDetail, Node &nodeName, UI32 *expectedReply, UI32 oneWayMessage);
                void                       processRemoteCommands              (AppInterfaceObjectManagerRemoteCommandsMessage *pMessage);
                UI32                       sendRemoteCommandsReply            (void *pContext, UI32 nResults, commandData_t resultList []);
                void                       processDuplicateCmdsReply          (FrameworkStatus frameworkStatus, AppInterfaceObjectManagerRemoteCommandsMessage *pRemoteCmdMessage, void *pContext);

                void                       clusterCreateCollectValidationData (WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
                void                       clusterCreateValidate              (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
                void                       clusterCreateSendValidationResults (WaveObjectManagerSendValidationResultsMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

                void                       clusterCollectValidationReply      (void *pContext, UI32 validationStatus, unsigned int validationResultsLen, void *pValidationResultsData);
                void                       clusterValidationReply             (void *pContext, UI32 validationStatus, unsigned int validationResultDataLen, void *pValidationResultData);

                void                       processCreateClusterDone           (ClusterInterfaceObjectManagerCreateClusterDoneMessage *pMessage);
                void                       processDeleteClusterDone           (ClusterInterfaceObjectManagerDeleteClusterDoneMessage *pMessage);
                void                       processAddNodeDone                 (ClusterInterfaceObjectManagerAddNodeDoneMessage *pMessage);
                void                       processJoinNodeDone                (ClusterInterfaceObjectManagerJoinNodeDoneMessage *pMessage);
                void                       processDeleteNodeDone              (ClusterInterfaceObjectManagerDeleteNodeDoneMessage *pMessage);

    // Now the data members
    private :
    protected :
    public :
        duplicateCmdsHandler_t              m_duplicateCommandHandler;
        clusterCollectValidationHandler_t   m_clusterCollectValidationHandler;
        clusterValidationHandler_t          m_clusterValidationHandler;
        clusterReportValidationHandler_t    m_clusterReportValidationHandler;

        memberJoinedCallBack_t              m_nodeJoinedCallBack;
        heartBeatCallBack_t                 m_heartBeatLossCallBack;
        PrismMutex                          m_mutex;

        NodeValidationResult_t             *m_clusterCreationResults;
        UI32                                m_nRepliedSecondary;
};

class MessageAndContext
{
    private :
    protected :
    public :
        MessageAndContext       (PrismMessageHandler *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext, unsigned int m_appId);
        MessageAndContext       ();
       ~MessageAndContext       ();

    private :
    protected :
    public :

        PrismMessageHandler         *m_pMessage;
        PrismAsynchronousContext    *m_pPrismAsynchronousContext;
        unsigned int                 m_appId;
};

}

#endif // APPINTERFACEOBJECTMANAGER_H
