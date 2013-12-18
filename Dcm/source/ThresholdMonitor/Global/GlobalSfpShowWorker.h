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
 *   Author : Yugant Chandrakar                                            *
 **************************************************************************/

#ifndef GLOBALSFPSHOWWORKER_H
#define GLOBALSFPSHOWWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceLinearSeqContext.h"
//#include "ThresholdMonitor/Global/SfpGlobalShowMessage.h"
//#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{
	class ThresholdMonitorGlobalObjectManager;
	class SfpGlobalShowMessage;
	class GlobalSfpShowWorker : public WaveWorker
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			void SfpGlobalShowMessageHandler (SfpGlobalShowMessage *pSfpGlobalShowMessage);
			void sendShowSfpCommandToCluster (PrismLinearSequencerContext *seq_ctx_p);
			void sendShowSfpCommandToClusterCallback (WaveSendToClusterContext *ctx_p);
		protected:
		public:
			GlobalSfpShowWorker (ThresholdMonitorGlobalObjectManager *pThresholdMonitorGlobalObjectManager);
			virtual ~GlobalSfpShowWorker ();
		private:
		protected:
		public:
	};
}

#endif
