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

#ifndef CLUSTERINTERFACEOBJECTMANAGER_H
#define CLUSTERINTERFACEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "App/AppInterface.h"

namespace WaveNs
{

class ClusterInterfaceObjectManager : public WaveLocalObjectManager
{
    private :
                                               ClusterInterfaceObjectManager    ();

    protected :
    public :
        virtual                               ~ClusterInterfaceObjectManager    ();
        static  ClusterInterfaceObjectManager *getInstance                      ();
        static  PrismServiceId                 getPrismServiceId                ();

                int                        createCluster                    (SI32 appId, int nSecondaryNodes, node_t secondaryNodes [], createClusterCallBack_t  createClusterResultCB, void *pContext);
                int                        addNodeToCluster                 (SI32 appId, node_t node, addNodeCallBack_t  addNodeResultCB, void *pContext);
                int                        joinNodeToCluster                (SI32 appId, node_t node, joinNodeCallBack_t  joinNodeResultCB, void *pContext);
                int                        removeNodeFromCluster            (SI32 appId, node_t node, removeNodeCallBack_t  removeNodeResultCB, void *pContext);
                int                        removeCluster                    (SI32 appId, removeClusterCallBack_t  removeClusterResultCB, void *pContext);
                void                       removeClusterCallBack            (FrameworkStatus frameworkStatus, ClusterObjectManagerDeleteClusterMessage *pMessage, void *pContext);
                void                       removeNodeCallBack               (FrameworkStatus frameworkStatus, ClusterObjectManagerDeleteNodeMessage *pMessage, void *pContext);
                void                       addNodeCallBack                  (FrameworkStatus frameworkStatus, ClusterObjectManagerAddNodeMessage *pMessage, void *pContext);
                void                       joinNodeCallBack                 (FrameworkStatus frameworkStatus, ClusterObjectManagerJoinNodeMessage *pMessage, void *pContext);
                void                       createClusterCallBack            (FrameworkStatus frameworkStatus, ClusterObjectManagerCreateClusterMessage *pMessage, void *pContext);

                int                        registerHeartBeatCallBack        (heartBeatCallBack_t    heartBeatCB);
                int                        registerMemberJoinedCallBack     (memberJoinedCallBack_t memberJoinedCB);

                int                        setHeartBeatConfig               (int heartBeatInterval, int nLostHeartBeatsToNotify);
                int                        getSwitchHaRole                  (int *nodeRole);
                int                        getSwitchHaConfig                (haNodeStatus_t *primaryNodes, int *nSecondaryNodes, haNodeStatus_t *secondaryNodes);
                int                        getLocalIpAddr                   (ipString localIpAddr);
                UI32                       getClusterNodes                  (vector <Node> *nodeNames);

    // Now the data members

    private :
    protected :
    public :
};

class ClusterInterafceData
{
    private :
    protected :
    public :
    // Now the data members

    private :
    protected :
    public :
        void                      *pContext;
        SI32                       appId;
        addNodeCallBack_t          addNodeResultCB;
        joinNodeCallBack_t         joinNodeResultCB;
        removeNodeCallBack_t       removeNodeResultCB;
        removeClusterCallBack_t    removeClusterResultCB;
        createClusterCallBack_t    createClusterResultCB;
};

}

#endif // CLUSTERINTERFACEOBJECTMANAGER_H
