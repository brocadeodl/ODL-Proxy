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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : mmohan                                                       *
 **************************************************************************/

#ifndef DISCOVERWORKERS_H
#define DISCOVERWORKERS_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "discover/User/discoverTimers.h"
#include "discover/User/discoverDcmDef.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "discover/Framework/discoverManagedObjects.h"

#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "VcsFabric/Local/VcsNodeDomainIdMessages.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/NodeReadyForAllCommandsEvent.h"

using namespace WaveNs;

namespace DcmNs
{

    class DiscoverObjectManager;
    class DiscoverDiscoverMessage;
    class DiscoverStatusMessage;
	class DiscoverStartDiscoverTimer;

    class DiscoverWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

			void DiscoverDiscoverMessageHandler (DiscoverDiscoverMessage *pDiscoverDiscoverMessage);
			
            ResourceId doDiscoverStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId doCheckDiscoverStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId doDistributionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId doFcNodeAddDistributionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId doDbNewDistributionStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId doFreshDiscovery (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId deleteDiscoverAssets (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void discoverDiscoverSequencerSucceededStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void discoverDiscoverSequencerFailedStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId startDiscoverTimerStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId stopDiscoverTimerStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void discoverTimerHandlerCallback (TimerHandle timerHandle, void * pContext);
			void showStatusVector ();
			void sendMessageToLocalPluginStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			void sendMessageToLocalPluginCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
			ResourceId updateDiscoverStatus (DiscoverDiscoverMessage *msg, discoverStages_t dStage, 
																	time_t sTime, time_t eTime);
			ResourceId updateDbDiscoverStatus (DiscoverDiscoverMessage *msg, discoverStages_t dStage, 
																	time_t sTime, time_t eTime, int exitStatus);
			ResourceId updateDbNewDiscoverStatus (DiscoverDiscoverMessage *msg, discoverStages_t dStage, 
																	time_t sTime, time_t eTime, int exitStatus);
			VcenterManagedObject* getVcenterConfigFromMo ();
			ResourceId getReturnStatusForExitStatus (ResourceId status, int exitStatus);
			void DiscoverStatusMessageHandler (DiscoverStatusMessage *pDiscoverStatusMessage);
            
			ResourceId getDiscoverStatusStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
			ResourceId getVirtualIpAddress (string &vIPAddress);
			void extractScriptNames (PrismLinearSequencerContext *pPrismLinearSequencerContext, vector<string> &words);
			DiscoverStartDiscoverTimer	*pDiscoverStartDiscoverTimer;
	        ResourceId markDiscoverStatusStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            ResourceId setDiscoverStateBeginStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            DiscoverManagedObject* getDiscoverManagedObject (string vcenterName);
            ResourceId deleteDiscoverManagedObject (PrismLinearSequencerContext *pPrismLinearSequencerContext);
            void resetDiscoverFailureCounters();
            void incrDiscoverFailureCount(discoverExitStatus_t failureType);
            void generateDiscoverFailureRaslog(PrismLinearSequencerContext *pPrismLinearSequencerContext,
                                                discoverExitStatus_t failureType);
            void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void nodeReadyForAllCommands (const NodeReadyForAllCommandsEvent* &pEvent);

        protected:
        public:
            DiscoverWorker (DiscoverObjectManager *pDiscoverObjectManager);
            virtual    ~DiscoverWorker ();

// Now the data members

        private:
        protected:
        public:
            UI32    m_discInvalidCredCounter;
    };

}
#endif
// DISCOVERWORKERS_H
