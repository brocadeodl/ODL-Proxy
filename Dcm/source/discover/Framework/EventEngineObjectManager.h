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
 *   Author : Ritesh Madapurath                                                       *
 **************************************************************************/

#ifndef EVENTENGINEOBJECTMANAGER_H
#define EVENTENGINEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/MultiThreading/PrismPosixThread.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "discover/Framework/EventEngineManagementInterfaceMessages.h"
#include "discover/Framework/MigrationEventsMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "discover/Framework/EventEngineConcurrentQueue.h"
#include "discover/Framework/EventEngineThread.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "vCenter/Framework/vCenterCrypto.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#define PHPDIR_PATH     ((char *) "/fabos/webtools/htdocs/ws")
#define INTERPRETER     ((char *) "/usr/apache/php")
#define VM_POWER_STATUS_PHP ((char *) "/fabos/webtools/htdocs/ws/vm_power_status.php")
//TODO Put VNM_
#define POWER_STATUS_POWERED_OFF  0
#define POWER_STATUS_POWERED_ON 1
#define POWER_STATUS_INVALID_CONNECTION 2
#define POWER_STATUS_INVALID_CREDENTIALS 3
#define POWER_STATUS_INVALID_VERSION 4
#define POWER_STATUS_NO_SERVICE 5
#define POWER_STATUS_SUSPENDED 6
#define POWER_STATUS_INVALID_VM 7
#define POWER_STATUS_VCENTER_NOT_CONFIGURED_ACTIVATED 20


using namespace WaveNs;

#define TRUE 1
namespace DcmNs {

    class EventEngineDispatchThread;

    class EventEngineObjectManager: public WaveObjectManager {
            friend class EventEngineDispatchThread;

        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(
                    const string &managedClassName);
            int fetchVMPowerStatus(string vcenter, string user, string pass,
                    string vm_mor);
            VcenterManagedObject*
            getVcenterConfigFromMo();
            void EventEngineObjectManagerSendToClusterCallback(
                        WaveSendToClusterContext *ctx_p);

        protected:
        public:
            EventEngineObjectManager();
            virtual ~EventEngineObjectManager();
            static EventEngineObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            void eventEngineStartStopHandler(
                    EventEngineMessage *eventEngineMessage);
            void handleMessageFromVCenter(MigrationEventsMessage *message);
            void shutdown(WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
        private:
            vnm_resp_t *getVNMResp(MigrationEventsMessage *message);
            EventEngineConcurrentQueue eventsQueue;
            void populateVMDetails(vnm_vm_dtls_t *vm_dtls, uint32_t nu_vm_ips,
                    uint32_t nu_vm_macs,
                    vector< vector <string> > vm_mac_array);
            void populateESXDetails(vnm_esx_dtls_t *esx_dtls,
                    uint32_t num_of_kernel_ip_addrs, vnm_esx_role_t esx_role,
                    char *console_ip, vector<vector<string> > kern_ip_array);
            void fillReturnCodeForPowerStatus(int returnStatus,
                    vnm_resp_t *& vnm_resp);

        protected:
        public:
        private:
            EventEngineDispatchThread *dispatchThread;
            EventEngineThread *phpThread;
    };

    inline string EventEngineObjectManager::getClassName() {
        return ("VCenterGlobalEventEngine");
    }

    class EventEngineDispatchThread: public PrismPosixThread {
        private:
        protected:
        public:
            EventEngineDispatchThread(EventEngineObjectManager *objMan);
            virtual ~EventEngineDispatchThread();

            WaveThreadStatus start();

            volatile bool running;
        private:
            EventEngineObjectManager *objManager;

        protected:
        public:
    };

}

#endif  // EVENTENGINEOBJECTMANAGER_H
