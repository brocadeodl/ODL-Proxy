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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#ifndef VCSNODESLOTMANAGEMENTWORKER_H
#define VCSNODESLOTMANAGEMENTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Local/VcsNodeFabosSlotEvents.h"
#include "VcsFabric/Local/VcsNodeSlotManagementInterfaceMessages.h"
#include "VcsFabric/Local/VcsNodeSlotMessagingContext.h"
#include "VcsFabric/Local/VcsNodeSlotSynchronousMessagingContext.h"
#include "Cluster/Local/ClusterLocalSlotOnlineMessage.h"
#include "Cluster/Local/ClusterLocalSlotOfflineMessage.h"
#include "Cluster/Local/ClusterLocalSlotRemoveMessage.h"
#include "Cluster/Local/ClusterLocalSlotRemoveAllMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSlotManagementWorker : public WaveWorker
{
    private :
        virtual void shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        virtual void install  (WaveAsynchronousContextForBootPhases     *pWaveAsynchronousContextForBootPhases);
                void installKnownSlotsWhileJoiningCluster  (WaveAsynchronousContextForBootPhases     *pWaveAsynchronousContextForBootPhases);
                void getLinecardsFromStartupDB             ();
                int  getSlotInformationFromBackend         (UI32 totalSlots);

    protected :
    public :
                    VcsNodeSlotManagementWorker         (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager);
        virtual     ~VcsNodeSlotManagementWorker        ();
        static      VcsNodeSlotManagementWorker *getInstance ();

        string      getClassName                            ();


//        vector<VcsNodeLocalManagedObject *>     *getNodeLocalManagedObjects     ();

        PrismMessage *createMessageInstance (const UI32 &operationCode);
		void replaySlotScnQueueEvents ();

		void slotEventHandler (const VcsNodeFabosSlotOnlineEvent *&pSlotOnlineEvent);

		void slotOnlineEventHandler (const VcsNodeFabosSlotOnlineEvent *&pEvent);
		void slotCreationHandler (const VcsNodeFabosSlotOnlineEvent *&pEvent);
		void sendSlotOnlineMessageStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void broadcastSlotOnlineEvent (const VcsNodeFabosSlotOnlineEvent *&pEvent);

		void slotOfflineEventHandler (const VcsNodeFabosSlotOfflineEvent *&pSlotOfflineEvent);
        void slotDeleteEventHandler (const VcsNodeFabosSlotOfflineEvent *&pSlotOfflineEvent);
		void sendSlotOfflineMessageStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void broadcastSlotOfflineEventStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void resetLinecardReadyBitStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);

        void slotFailoverToPluginsCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void sendSlotFailoverToPluginsStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);

		void removeSlotViaSendToWaveClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
		void removeSlotViaSendToWaveCluster (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void slotRemoveMessageHandler (VcsNodeSlotRemoveMessage *pSlotRemoveMessage);

		void sendSlotRemoveMessageStep (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void clearLinecardMessageHandler (VcsNodeClearLinecardMessage *pVcsNodeclearLinecardMessage);

		void getSlotObjects (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
		void validateRemoveSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void createOrUpdateSlotManagedObject (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void createCompositionAssociationIfNewSlotMO (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void createAssociationWithWaveSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void updateSlotManagedObject (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void removeSlotCompositionAssociation (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);
        void removeSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);

		void slotConfigMessageHandler (VcsNodeSlotConfigMessage *pVcsNodeSlotConfigMessage);
		void configSlotViaSendToWaveClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
		void configSlotViaSendToWaveCluster (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);

        void slotSetLinecardModeMessageHandler (VcsNodeSlotSetLinecardModeMessage *pVcsNodeSlotSetLinecardModeMessage);
		void validateConfigSlot (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);

        UI32 getLinecardTypeFromHwId (const UI32 hwId);

        void clearAllLinecardHandler ();
        ResourceId sendSlotRemoveAllMessageStep (VcsNodeSlotSynchronousMessagingContext *pVcsNodeSlotSynchronousMessagingContext);
        ResourceId removeAllSlots (VcsNodeSlotSynchronousMessagingContext *pVcsNodeSlotSynchronousMessagingContext);
		void slotStateQueryMessageHandler (VcsNodeSlotStateQueryMessage *pSlotStateQueryMessage);

		void createSlotZero (VcsNodeSlotMessagingContext *pVcsNodeSlotMessagingContext);

        void createDummySlotMessageHandler (VcsNodeCreateDummySlotMessage *pVcsNodeCreateDummySlotMessage);
        void backendSyncUp (PrismAsynchronousContext *pPrismAsynchronousContext);

    // Now the Data Members

        friend class VcsFabricLocalObjectManager;

    protected :
    public :

};

}

#endif // VCSNODESLOTMANAGEMENTWORKER_H
