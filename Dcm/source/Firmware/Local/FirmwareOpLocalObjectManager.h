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
 *   Author : lhu                                                     *
 **************************************************************************/

#ifndef FIRMWAREOPLOCALOBJECTMANAGER_H
#define FIRMWAREOPLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosSystemEvents.h"
#include "Framework/Core/ConfigReplayCompletedEvent.h"
#include "VcsFabric/Local/VcsNodeDomainEvents.h"

using namespace WaveNs;

namespace DcmNs
{

    class FirmwareOpActionMessage;
    class NetconfFirmwareDownloadMessage;
	class FirmwareOpAutoUpgradeMessage;

    class FirmwareOpLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void FirmwareOpActionMessageHandler( FirmwareOpActionMessage *pFirmwareOpActionMessage);
            void NetconfFirmwareDownloadMessageHandler(NetconfFirmwareDownloadMessage *pMessage);
			void FirmwareOpAutoUpgradeMessageHandler(FirmwareOpAutoUpgradeMessage *pMessage);
			void listenForEvents(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void syncDumpStartEventHandler(const VcsNodeFabosSyncStartEvent* &pEvent);
			void configReplayCompletedEventHandler(const ConfigReplayCompletedEvent* &pEvent);
			void domainValidEventHandler (const VcsNodeDomainValidEvent* &pEvent);
			void dhcpAutomaticFirmwareDownload(void);
			void dadTimerHandler(TimerHandle &timerHandle);
			void dhcpDeleteTimer(void);
			void dhcpUpdateDadState(int dadStatus);
        protected:
        public:
            FirmwareOpLocalObjectManager ();
            virtual    ~FirmwareOpLocalObjectManager ();
            static FirmwareOpLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:
			TimerHandle dadTimerHandle;
			int			dadState;
        protected:
        public:
    };
}
#endif                                            //FIRMWAREOPLOCALOBJECTMANAGER_H
