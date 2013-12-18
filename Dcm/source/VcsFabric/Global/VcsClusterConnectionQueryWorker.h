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

/**************************************************************************
*   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Sudhakar Jha                                               *
***************************************************************************/

#ifndef VCSCLUSTERCONNECTIONQUERYWORKER_H
#define VCSCLUSTERCONNECTIONQUERYWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsIsIpReachableFromClusterMessage.h"
#include "VcsFabric/Global/VcsClusterConnectionToIpEnqSequencerContext.h"

using namespace WaveNs;

namespace DcmNs
{


	class VcsClusterConnectionQueryWorker: public WaveWorker
	{
		private:
			void executeVcsIsIpReachableFromClusterMessageHandler (VcsIsIpReachableFromClusterMessage* vcsIsIpReachableFromClusterMessage);
			
			void validateIpConnectionEnqRequestStep(VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext);
			void sendIpConnectionEnqRequestStep(VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext);
			void sendIpConnectionEnqRequestStepCallback(FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext);
			void validateIpConnectionEnqResponseStep(VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext);
			
			virtual PrismMessage *createMessageInstance (const UI32 &operationCode);


		protected:
		public:
			VcsClusterConnectionQueryWorker (VcsFabObjectManager* pWaveObjectManager);
			virtual	~VcsClusterConnectionQueryWorker ();



		private:

		protected:
		public:
	};
}
#endif											//VIRTUALIPGLOBALWORKER_H

