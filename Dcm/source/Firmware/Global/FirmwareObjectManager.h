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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#ifndef FIRMWAREOBJECTMANAGER_H
#define FIRMWAREOBJECTMANAGER_H

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/Core/WaveNewNodesAddedEvent.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Firmware/Local/FirmwareClusterLocalMessage.h"
#include "glog/glog.h"

using namespace WaveNs;

#if 0
#define FOM_GLOG(level, fmt, ... )                                  \
            do {                                                    \
                glog(glog_handle, level, fmt ,##__VA_ARGS__);       \
            } while (0)
#endif

namespace DcmNs
{

    class FirmwareClusterMessage;
	class FirmwareClusterLocalMessage;

    class FirmwareObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance (const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance (const string &managedClassName);
			vector<WaveManagedObject *> *DoSynchronousQuery (const string className);
            void FirmwareClusterMessageHandler (FirmwareClusterMessage *pFirmwareClusterMessage);
            void FirmwareActivateMessageHandler (FirmwareClusterMessage *pFirmwareClusterMessage);
			void SanityCheckHandler(PrismLinearSequencerContext *pFirmwareContext);
			void SanityCheckCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext);

			void FirmwareInstallHandler(PrismLinearSequencerContext *pFirmwareContext);
			void FirmwareInstallCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext);
			void RebootHandler(PrismLinearSequencerContext *pFirmwareContext);
			void RebootCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext);
			void FirmwareActivateHandler(PrismLinearSequencerContext *pFirmwareContext);
			void FirmwareActivateCallback(FrameworkStatus frameworkStatus, FirmwareClusterLocalMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext);
			void prismLinearSequencerFailedStep (PrismLinearSequencerContext *pFirmwareContext);
			void failover (FailoverAsynchronousContext *pFailoverAsynchronousContext);
			void listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void nodeAddedEventHandler(const WaveNewNodesAddedEvent* &pEvent);
			void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
			void configReplayCompletedEventHandler(const WaveNewNodesAddedEvent* &pEvent);
			void cliTimerHandler(TimerHandle &timerHandle);
			void updateFirmwareDownloadStatusMO(UI32 rid_local, int status, string mesg);


        protected:
        public:
            FirmwareObjectManager ();
            virtual    ~FirmwareObjectManager ();
            static FirmwareObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            glog_handle_t *glog_handle;
            void   fgom_log(int level, const char *fmt, ...);

// Now the data members

        private:
			TimerHandle cliTimerHandle;
			int	nodeActivateRebooted;

        protected:
        public:
    };
}
#endif                                            //FIRMWAREOBJECTMANAGER_H
