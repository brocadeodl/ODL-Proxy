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

#ifndef CENTRALCLUSTERCONFIGOBJECTMANAGER_H
#define CENTRALCLUSTERCONFIGOBJECTMANAGER_H

#include "App/AppInterface.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Cluster/ClusterManagedObject.h"
#include "Cluster/ClusterDebug.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Cluster/ClusterDeleteContext.h"
#include <string>
#include <vector>

namespace WaveNs
{

class FrameworkObjectManagerDestroyClusterMessage;
class ClusterFailoverContext;
class CentralClusterConfigCreateClusterWorker;
class CentralClusterConfigDeleteClusterWorker;
class CentralClusterConfigAddNodeWorker;
class CentralClusterConfigDeleteNodeWorker;
class CentralClusterConfigGetDebugInfoWorker;
class CentralClusterConfigRejoinNodeWorker;
class CentralClusterConfigUpdateHardwareSynchronizationStateWorker;
class WaveAsynchronousContextForBootPhases;
class CliBlockWorker;

class CentralClusterConfigObjectManager : public WaveObjectManager
{
    private :
                                     CentralClusterConfigObjectManager           ();
         virtual PrismMessage       *createMessageInstance                       (const UI32 &operationCode);
                 WaveManagedObject *createManagedObjectInstance                 (const string &managedClassName);

                 void                initialize                                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                 void                install                                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                 void                boot                                        (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                 void                bootHandleSecondaryNodeUnconfigureBootStep  (ClusterBootContext *pClusterBootContext);
                 void                bootQueryNodesStep                          (ClusterBootContext *pClusterBootContext);
                 void                bootStartHeartBeatsStep                     (ClusterBootContext *pClusterBootContext);

                 void                failover                                    (FailoverAsynchronousContext *pFailoverAsynchronousContext);
                 ResourceId          failoverQueryPrismClusterStep               (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverDetermineIfPrimaryChangedStep       (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverQueryAllWaveNodeObjectsStep         (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverUpdatePrismClusterStep              (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverUpdateWaveNodeObjectsStep           (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverStartHeartBeatsIfPrimaryChangedStep (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverStartHeartBeatsIfUncontrolledPrimaryChangedStep (ClusterFailoverContext *pClusterFailoverContext);
                 ResourceId          failoverUncontrolledUpdateWaveNodeObjectsStep (ClusterFailoverContext *pClusterFailoverContext);

    protected :
    public :
        virtual                                   ~CentralClusterConfigObjectManager  ();
        static  CentralClusterConfigObjectManager *getInstance                        ();
        static  PrismServiceId                     getPrismServiceId                  ();
        static  string                             getServiceName                     ();

                void                               createClusterConfig                                  (ClusterObjectManagerCreateClusterMessage *pMessage);
                void                               deleteCluster                                        (ClusterObjectManagerDeleteClusterMessage *pMessage);

                void                               sendCenteralHeartBeat              ();
                void                               removeClusterConfig                ();
                UI32                               getManagmentOperationStatus        ();
                void                               setManagmentOperationStatus        ();
                void                               resetManagmentOperationStatus      ();

                UI32                               validateClusterCreation            (ClusterObjectManagerCreateClusterMessage *pMessage);
                UI32                               preCreateClusterUpdateConfig       (ClusterObjectManagerCreateClusterMessage *pMessage);
                UI32                               informFrameworkForClusterCreation  (ClusterObjectManagerCreateClusterMessage *pMessage);
                UI32                               postCreateClusterUpdateConfig      (ClusterObjectManagerCreateClusterMessage *pMessage, PrismCreateClusterWithNodesMessage *pCreateClusterMessage);
                void                               replyToCreateClusterMessage        (ClusterObjectManagerCreateClusterMessage *pMessage, PrismCreateClusterWithNodesMessage *pCreateClusterMessage);
                UI32                               validateDeleteCluster              (ClusterObjectManagerDeleteClusterMessage *pMessage);
                UI32                               informFrameworkForDeleteCluster    (ClusterObjectManagerDeleteClusterMessage *pMessage);
                void                               processFrameworkDeleteClusterReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerDestroyClusterMessage *pFrameworkDestroyClusterMessage, void *pContext);
                void                               processFrameworkCreateClusterReply (FrameworkStatus frameworkStatus, PrismCreateClusterWithNodesMessage *pCreateClusterMessage, void *pContext);

                void                               joinNode                           (ClusterObjectManagerJoinNodeMessage *pMessage);
                UI32                               validateJoinNode                   (ClusterObjectManagerJoinNodeMessage *pMessage);
                UI32                               informFrameworkForJoinNode         (ClusterObjectManagerJoinNodeMessage *pMessage);
                void                               processFrameworkJoinNodeReply      (FrameworkStatus frameworkStatus, FrameworkObjectManagerRejoinNodesToClusterMessage *pFrameworkJoinNodeMessage, void *pContext);
                UI32                               informFrameworkLostHeartBeat       (string ipAddress, UI32 port);
                void                               processFrameworkHeartBeatLostReply (FrameworkStatus frameworkStatus, FrameworkObjectManagerLostHeartBeatMessage *pFrameworkLostHeartBeatMessage, void *pContext);
                void                               reportLostHeartBeat                (HeartBeatLostMessage *pMessage);


        static  void                               lockClusterMutex                   ();
        static  void                               unlockClusterMutex                 ();
        static  void                               setHeartBeatInterval               (UI32 heartBeatInterval);
        static  void                               setNLostHeartBeatsToNotify         (UI32 nLostHeartBeatsToNotify);
        static  UI32                               getHeartBeatInterval               ();
        static  UI32                               getNLostHeartBeatsToNotify         ();
        static  UI32                               getNodeRole                        ();

        // Now the data members

    private :
    //Workers
        CentralClusterConfigCreateClusterWorker *m_pCentralClusterConfigCreateClusterWorker;
        CentralClusterConfigDeleteClusterWorker *m_pCentralClusterConfigDeleteClusterWorker;
        CentralClusterConfigAddNodeWorker       *m_pCentralClusterConfigAddNodeWorker;
        CentralClusterConfigDeleteNodeWorker    *m_pCentralClusterConfigDeleteNodeWorker;
        CentralClusterConfigGetDebugInfoWorker  *m_pCentralClusterConfigGetDebugInfoWorker;
        CentralClusterConfigRejoinNodeWorker    *m_pCentralClusterConfigRejoinNodeWorker;
        CentralClusterConfigUpdateHardwareSynchronizationStateWorker  *m_pCentralClusterConfigUpdateHardwareSynchronizationStateWorker;
        CliBlockWorker                          *m_pCliBlockWorker;

        ClusterDebug                             m_clusterDebug;
        UI32                                     m_managmentOperationInProgress;
        ClusterManagedObject                     m_cluster;
#if 0
               vector <memberJoinedCallBack_t>   m_memberJoinedCallBack;
//               vector <heartBeatCallBack_t>      m_heartBeatLostCallBack;
               heartBeatCallBack_t               m_heartBeatLostCallBack;
#endif
         PrismMutex                        m_mutex;

    protected :
    public :
        friend class LocalClusterConfigObjectManager;
        
};


}

#endif // CENTRALCLUSTERCONFIGOBJECTMANAGER_H
