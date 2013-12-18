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

#ifndef APPMESSAGES_H
#define APPMESSAGES_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/ClusterTypes.h"
#include "App/AppInterface.h"
#include <string>

namespace WaveNs
{

typedef enum
{
    DUPLICATE_CMD_INPUT_BUFFER  = 1,
    DUPLICATE_CMD_OUTPUT_BUFFER = 1000

} AppMessageBufferId;

typedef enum
{
    APP_REMOTE_COMMANDS                 = 0,
    APP_INTERFACE_CREATE_CLUSTER_DONE,
    APP_INTERFACE_DELETE_CLUSTER_DONE,
    APP_INTERFACE_ADD_NODE_DONE,
    APP_INTERFACE_JOIN_NODE_DONE,
    APP_INTERFACE_DELETE_NODE_DONE

} AppOpCodes;

class  AppInterfaceObjectManagerRemoteCommandsMessage: public PrismMessage
{
    private :
    protected :
    public :
        virtual    ~AppInterfaceObjectManagerRemoteCommandsMessage ();
                    AppInterfaceObjectManagerRemoteCommandsMessage ();
                    AppInterfaceObjectManagerRemoteCommandsMessage (PrismServiceId serviceId);

        void        setupAttributesForSerialization                ();
        void        setNodeName                                    (string nodeName);
        void        setNodePort                                    (UI32 nodePort);
        void        setNCommands                                   (UI32 nCommands);
        void        setNResults                                    (UI32 nResults);
        string      getNodeName                                    ();
        UI32        getNodePort                                    ();
        UI32        getNCommands                                   ();
        UI32        getNResults                                    ();
        void        setDuplicateCmdInputBuffer                     (UI32 bufferNum, UI32 size, void *pBuffer);
        void        getDuplicateCmdInputBuffer                     (UI32 bufferNum, UI32 &size, void *&pBuffer);
        void        transferCmdInputBufferToUser                   (UI32 bufferNum, UI32 &size, void *&pBuffer);
        void        setDuplicateCmdOuputBuffer                     (UI32 bufferNum, UI32 size, void *pBuffer);
        void        getDuplicateCmdOuputBuffer                     (UI32 bufferNum, UI32 &size, void *&pBuffer);
        void        transferOutputBufferToUser                     (UI32 bufferNum, UI32 &size, void *&pBuffer);

        void       *transferOutputBufferToUser                     (UI32 &size);
        void        setDuplicateCmdOuputBuffer                     (UI32 size, void *pBuffer);
        void        getDuplicateCmdOuputBuffer                     (UI32 &size, void *&pBuffer);
        void        transferOutputBufferToUser                     (UI32 &size, void *&pBuffer);


    // Now the data members

    private :
        string                                  m_nodeName;
        UI32                                    m_nodePort;
        UI32                                    m_nCommands;
        UI32                                    m_nResults;

    protected :
    public :
};

class ClusterInterfaceObjectManagerCreateClusterDoneMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                ~ClusterInterfaceObjectManagerCreateClusterDoneMessage    ();
                                ClusterInterfaceObjectManagerCreateClusterDoneMessage    ();
                                ClusterInterfaceObjectManagerCreateClusterDoneMessage    (PrismServiceId serviceId);
                                //ClusterInterfaceObjectManagerCreateClusterDoneMessage    (string primaryNodeName, UI32 nSecondaryNodes, string secondariesNodeNames []);
        void                    setupAttributesForSerialization             ();
        void                    setPrimaryNodeName                          (const string &primaryNodeName);
        void                    setNSecondaryNodes                          (const UI32 nSecondaryNodes);
        void                    setSecondaryNodeNameAndPort                 (const string &nodeName, const SI32 &nodePort);
        void                    setNodeStaus                                (string nodeName, UI32 nodePort, UI32 Status);
        void                    setCreateClusterResultCallBack              (createClusterCallBack_t createClusterResultCb);
        void                    setContext                                  (void *pContext);
        void                    setOperationStatus                          (UI32 clusterCreationStatus);
        string                  getPrimaryNodeName                          ();
        UI32                    getNSecondaryNodes                          ();
        string                  getSecondaryNodeName                        (const UI32 index);
        SI32                    getSecondaryNodePort                        (const UI32 index);
        haNodeStatus_t         *getNodeStatusAddr                           (UI32 index);
        string                  getStatusNodeName                           (UI32 index);
        SI32                    getStatusNodePort                           (UI32 index);
        UI32                    getStatusNodeStatus                         (UI32 index);
        createClusterCallBack_t getCreateClusterResultCallBack              ();
        void                   *getContext                                  ();
        UI32                    getOperationStatus                          ();

    // Now the data members

    private :
        string                          m_primaryNodeName;
        UI32                            m_nSecondaryNodes;
        vector <string>                 m_secondaryNodeNames;
        vector <SI32>                   m_secondaryNodePorts;
        vector <string>                 m_statusNodeNames;
        vector <SI32>                   m_statusNodePorts;
        vector <UI32>                   m_statusNodeStatus;

        UI32                            m_operationStatus;
        createClusterCallBack_t         m_createClusterResultCb;
        void                           *m_pContext;


    protected :
    public :
};

class ClusterInterfaceObjectManagerDeleteClusterDoneMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual                ~ClusterInterfaceObjectManagerDeleteClusterDoneMessage   ();
                                ClusterInterfaceObjectManagerDeleteClusterDoneMessage   ();
                                ClusterInterfaceObjectManagerDeleteClusterDoneMessage   (PrismServiceId serviceId);
        void                    setupAttributesForSerialization                         ();
        void                    setRemoveClusterResultCallBack                          (removeClusterCallBack_t removeClusterResultCb);
        removeClusterCallBack_t getRemoveClusterResultCallBack                          ();
        void                    setContext                                              (void *pContext);
        void                   *getContext                                              ();
        void                    setOperationStatus                                      (UI32 operationStatus);
        UI32                    getOperationStatus                                      ();
    // Now the data members

    private :
        UI32                       m_operationStatus;
        removeClusterCallBack_t    m_removeClusterResultCb;
        void                      *m_pContext;
    protected :
    public :
};

class ClusterInterfaceObjectManagerAddNodeDoneMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual            ~ClusterInterfaceObjectManagerAddNodeDoneMessage ();
                            ClusterInterfaceObjectManagerAddNodeDoneMessage ();
                            ClusterInterfaceObjectManagerAddNodeDoneMessage (PrismServiceId serviceId);
        void                setupAttributesForSerialization                 ();
        void                setNodeName                                     (const string &nodeName);
        string              getNodeName                                     ();
        void                setNodePort                                     (const SI32 &nodePort);
        SI32                getNodePort                                     ();
        void                setAddNodeResultCallBack                        (addNodeCallBack_t addNodeResultCb);
        addNodeCallBack_t   getAddNodeResultCallBack                        ();
        void                setContext                                      (void *pContext);
        void               *getContext                                      ();
        void                setOperationStatus                              (UI32 operationStatus);
        UI32                getOperationStatus                              ();

    // Now the data members

    private :
        UI32                    m_operationStatus;
        string                  m_nodeName;
        SI32                    m_nodePort;
        addNodeCallBack_t       m_addNodeResultCb;
        void                   *m_pContext;

    protected :
    public :
};



class ClusterInterfaceObjectManagerJoinNodeDoneMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual            ~ClusterInterfaceObjectManagerJoinNodeDoneMessage ();
                            ClusterInterfaceObjectManagerJoinNodeDoneMessage ();
                            ClusterInterfaceObjectManagerJoinNodeDoneMessage (PrismServiceId serviceId);
        void                setNodeName                                      (const string &nodeName);
        string              getNodeName                                      ();
        void                setNodePort                                      (const SI32 &nodePort);
        SI32                getNodePort                                      ();
        void                setJoinNodeResultCallBack                        (joinNodeCallBack_t joinNodeResultCb);
        joinNodeCallBack_t  getJoinNodeResultCallBack                        ();
        void                setContext                                       (void *pContext);
        void               *getContext                                       ();
        void                setOperationStatus                               (UI32 operationStatus);
        UI32                getOperationStatus                               ();
    // Now the data members

    private :
        UI32                    m_operationStatus;
        string                  m_nodeName;
        SI32                    m_nodePort;
        joinNodeCallBack_t      m_joinNodeResultCb;
        void                   *m_pContext;

    protected :
    public :
};

class ClusterInterfaceObjectManagerDeleteNodeDoneMessage : public PrismMessage
{
    private :
    protected :
    public :
        virtual             ~ClusterInterfaceObjectManagerDeleteNodeDoneMessage ();
                             ClusterInterfaceObjectManagerDeleteNodeDoneMessage ();
                             ClusterInterfaceObjectManagerDeleteNodeDoneMessage (PrismServiceId serviceId);
        void                 setupAttributesForSerialization                    ();
        void                 setNodeName                                        (const string &nodeName);
        string               getNodeName                                        ();
        void                 setNodePort                                        (const SI32 &nodePort);
        SI32                 getNodePort                                        ();
        void                 setRemoveNodeResultCallBack                        (removeNodeCallBack_t removeNodeResultCb);
        removeNodeCallBack_t getRemoveNodeResultCallBack                        ();
        void                 setContext                                         (void *pContext);
        void                *getContext                                         ();
        void                 setOperationStatus                                 (UI32 operationStatus);
        UI32                 getOperationStatus                                 ();

    // Now the data members

    private :
        UI32                        m_operationStatus;
        string                      m_nodeName;
        SI32                        m_nodePort;
        removeNodeCallBack_t        m_removeNodeResultCb;
        void                       *m_pContext;

    protected :
    public :
};

}

#endif // APPMESSAGES_H
