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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James chen                                                   *
 **************************************************************************/

#ifndef RASLOCALSSTHREAD_H
#define RASLOCALSSTHREAD_H

#include "RAS/Local/RASSupportMessage.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/MultiThreading/PrismPosixThread.h"
#include "RAS/Local/RASLocalMessageContext.h"

using namespace WaveNs;

namespace DcmNs
{

	class RASLocalSSThread : protected WaveWorker, public PrismPosixThread
	{
		public:
			RASLocalSSThread(WaveObjectManager *pWaveObjectManager,
				RASSupportMessage *pRASSupportMessage);
			virtual ~RASLocalSSThread();
			WaveThreadStatus start();

		private:
			RASSupportMessage *m_pRASSupportMessage;
			ResourceId	SSCLI(RASLocalMessagingContext *pRASLocalMessagingContext);
			string syncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext);
			void asyncDataOutputHandler(RASLocalMessagingContext *pRASLocalMessagingContext);
	};

}
#endif
