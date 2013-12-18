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
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#ifndef FIRMWAREOPLOCALTHREAD_H
#define FIRMWAREOPLOCALTHREAD_H

#include "Firmware/Local/FirmwareOpActionMessage.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/MultiThreading/PrismPosixThread.h"

using namespace WaveNs;

namespace DcmNs
{
	class FirmwareOpLocalThread : protected WaveWorker, public PrismPosixThread
	{
		public:
			FirmwareOpLocalThread(WaveObjectManager *pWaveObjectManager,
				FirmwareOpActionMessage *pFirmwareOpActionMessage,
				NetconfFirmwareDownloadMessage *pNetconfMessage,
				UI32 msg_type);
			virtual ~FirmwareOpLocalThread();
			WaveThreadStatus start();

		private:
			FirmwareOpActionMessage			*m_pFirmwareOpActionMessage;
			NetconfFirmwareDownloadMessage	*m_pNetconfMessage;
			UI32							m_msg_type;
};

}
#endif
