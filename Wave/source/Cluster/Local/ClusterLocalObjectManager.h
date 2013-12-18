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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CLUSTERLOCALOBJECTMANAGER_H
#define CLUSTERLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Utils/PrismMutex.h"
#include "Cluster/Local/WaveNode.h"
#include "Cluster/Local/WaveHaNodeTypes.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/ClusterPhase3StartEvent.h"
#include "Framework/Core/ClusterPhase3CompleteEvent.h"
#include "Cluster/Local/WaveSlotLocalManagedObject.h"
#include "Cluster/Local/ClusterLocalGetObjectIdMessages.h"

namespace WaveNs
{

class WaveAsynchronousContextForBootPhases;
class WaveAsynchronousContextForShutDownPhases;
class PrismSynchronousLinearSequencerContext;
class ClusterLocalSetThisNodeIpAddressWorker;
class ClusterLocalReportPrimaryNodeChangedWorker;
class ClusterLocalReportRemovedNodeFromClusterWorker;
class ClusterLocalReportReplaceWorker;
class ClusterLocalSetHardwareSynchronizationStateWorker;
class ClusterLocalSetHaRoleWorker;
class ClusterLocalWaveSlotManagementWorker;
class ClusterLocalGetObjectIdWorker;
class CliBlockServiceIndependentMessage;
class ClusterLocalSetControllerDetailsMessage;
class ClusterLocalReportToControllerAsClientMessage;

class ClusterLocalObjectManager : public WaveLocalObjectManager
{
    private :
                           ClusterLocalObjectManager                        ();

        WaveManagedObject *createManagedObjectInstance                      (const string &managedClassName);

		//PrismMessage 	  *createMessageInstance (const UI32 &operationCode);

        void               install                                          (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void               boot                                             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        void               shutdown                                         (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        void               backendSyncUp                                    (PrismAsynchronousContext *pPrismAsynchronousContext);

        void               setThisWaveNodeObjectId                          (const ObjectId &thisWaveNodeObjectId);

        WaveNode*          getThisWaveNodeManagedObject                     ();

        void               setThisWaveHaNodeRole                            (const WaveHaNodeRole &thisWaveHaNodeRole);

        void               nodeAddedEventHandler                            (const WaveNewNodesAddedEvent* &pEvent);

        void               phase3StartEventHandler                          (const ClusterPhase3StartEvent* &pEvent );

        void               phase3CompleteEventHandler                       (const ClusterPhase3CompleteEvent* &pEvent );

        void               listenForEvents                                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

        void               setControllerDetailsMessageHandler               (ClusterLocalSetControllerDetailsMessage *pClusterLocalSetControllerDetailsMessage);
        void               clientReportingToControllerMessageHandler        (ClusterLocalReportToControllerAsClientMessage *pClusterLocalReportToControllerAsClientMessage);
        void               clusterLocalReportToControllerAsClientCallback   (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);

        void               controllerClusterFormationDampeningTimerCallback (TimerHandle &timerHandle, void *pContext);

    protected :
    public :
        virtual                           ~ClusterLocalObjectManager        ();

        static  ClusterLocalObjectManager *getInstance                      ();
        static  PrismServiceId             getPrismServiceId                ();
        static  string                     getPrismServiceName              ();

        static  bool                       getIsInstantiated                ();

        ObjectId                   		   getThisWaveNodeObjectId          ();

        WaveHaNodeRole                     getThisWaveHaNodeRole            ();
        void                               haboot                           (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                               dbInconsistencyCheck             (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
        void                               cliBlockMessageHandler           (CliBlockServiceIndependentMessage *pCliBlockServiceIndependentMessage);
        static  void                       cliBlockMessageStaticHandler     (CliBlockServiceIndependentMessage *pCliBlockServiceIndependentMessage);
        virtual PrismMessage*              createMessageInstance            (const UI32 &operationCode);
        virtual ResourceId                 blockCli                         (const ResourceId &reason, const bool &clusterWide);
        virtual ResourceId                 unblockCli                       (const ResourceId &reason, const bool &clusterWide);


    // Now the Data Members

    private :
               ObjectId                                             m_thisWaveNodeObjectId;
               PrismMutex                                           m_thisWaveNodeObjectIdMutex;

               WaveHaNodeRole                                       m_thisWaveHaNodeRole;
               PrismMutex                                           m_thisWaveHaNodeRoleMutex;
               IpVxAddress                                          m_thisWaveNodeControllerIpAddress;
               SI32                                                 m_thisWaveNodeControllerPort;
        static UI32                                                 m_thisNodeClientIndentificationNumber;
        static bool                                                 m_isInstantiated;
        static vector<IpVxAddress>                                  m_pendingIpaddressesToBeAddedToController;
        static vector<SI32>                                         m_pendingPortsToBeAddedToController;
        static TimerHandle                                          m_controllerClusterFormationDampeningTimerHandle;
               ResourceId                                           m_thisHardwareSyncState;

               ClusterLocalSetThisNodeIpAddressWorker              *m_pClusterLocalSetThisNodeIpAddressWorker;
    	       ClusterLocalReportPrimaryNodeChangedWorker          *m_pClusterLocalPrimaryNodeChangedWorker;
	           ClusterLocalReportRemovedNodeFromClusterWorker      *m_pClusterLocalReportRemovedNodeFromClusterWorker;
               ClusterLocalReportReplaceWorker                     *m_pClusterLocalReportReplaceWorker;
               ClusterLocalSetHardwareSynchronizationStateWorker   *m_pClusterLocalSetHardwareSynchronizationStateWorker;
               ClusterLocalSetHaRoleWorker                         *m_pClusterLocalSetHaRoleWorker;
		       ClusterLocalWaveSlotManagementWorker                *m_pClusterLocalWaveSlotManagementWorker;
               ClusterLocalGetObjectIdWorker                       *m_pClusterLocalGetObjectIdWorker;

    protected :
    public :

    friend  class ClusterLocalSetHardwareSynchronizationStateWorker;
	friend  class ClusterLocalWaveSlotManagementWorker;
};

}

#endif // CLUSTERLOCALOBJECTMANAGER_H

