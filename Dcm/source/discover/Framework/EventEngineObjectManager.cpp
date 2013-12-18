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

#include "discover/Framework/EventEngineObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "discover/Framework/EventEngineThread.h"
#include "discover/Framework/EventEngineTypes.h"
#include "discover/Framework/EventEngineMessages.h"
#include "ClientInterface/vCenter/vCenterClientParseUtility.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include <netinet/ether.h>

namespace DcmNs {

    EventEngineObjectManager::EventEngineObjectManager() :
            WaveObjectManager(getClassName()) {
        trace(TRACE_LEVEL_DEBUG, "EventEngine Global ::Initiated");
        dispatchThread = new EventEngineDispatchThread(this);
        phpThread = new EventEngineThread();
        addOperationMap(
                EVENTENGINE_START_STOP,
                reinterpret_cast<PrismMessageHandler>(&EventEngineObjectManager::eventEngineStartStopHandler));
        addOperationMap(
                EVENTENGINE_VCENTER_EVENTS_NOTIFY,
                reinterpret_cast<PrismMessageHandler>(&EventEngineObjectManager::handleMessageFromVCenter));
    }

    EventEngineObjectManager::~EventEngineObjectManager() {

        //stop the Events Queue , which would stop the dispatch thread
        if (dispatchThread->running) {
            eventsQueue.stopQueue();
            dispatchThread->joinCallingThread();
        }
        if (phpThread->running) {
            phpThread->stopActivity();
            phpThread->joinCallingThread();
        }
        //reclaim memory for dispatch thread.
        delete (dispatchThread);
        delete (phpThread);
    }
    void EventEngineObjectManager::shutdown(
            WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases) {
        trace(TRACE_LEVEL_INFO,
                "EventEngineObjectManager::shutdown : Entering ...");
        if (dispatchThread->running) {
            eventsQueue.stopQueue();
            dispatchThread->joinCallingThread();
        }
        if (phpThread->running) {
            phpThread->stopActivity();
            phpThread->joinCallingThread();
        }
        pWaveAsynchronousContextForShutDownPhases->setCompletionStatus(
                WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForShutDownPhases->callback();
    }

    EventEngineObjectManager *EventEngineObjectManager::getInstance() {
        static EventEngineObjectManager *pEventObjectObjectManager =
                new EventEngineObjectManager();
        WaveNs::prismAssert(NULL != pEventObjectObjectManager, __FILE__,
                __LINE__);
        return (pEventObjectObjectManager);
    }

    PrismServiceId EventEngineObjectManager::getPrismServiceId() {
        return ((getInstance())->getServiceId());
    }

    PrismMessage *EventEngineObjectManager::createMessageInstance(
            const UI32 & operationCode) {
        PrismMessage *pPrismMessage = NULL;
        switch (operationCode) {
            //Message for Starting/Stopping Events.php
            case EVENTENGINE_START_STOP:
                pPrismMessage = new EventEngineMessage();
                break;
                //Message used by DcmClient to send events to this plugin
            case EVENTENGINE_VCENTER_EVENTS_NOTIFY:
                pPrismMessage = new MigrationEventsMessage();
                break;
            default:
                pPrismMessage = NULL;
        }
        return (pPrismMessage);
    }

    WaveManagedObject *EventEngineObjectManager::createManagedObjectInstance(
            const string & managedClassName) {
        return NULL;
    }

    void EventEngineObjectManager::populateVMDetails(vnm_vm_dtls_t *vm_dtls,
            uint32_t nu_vm_ips, uint32_t nu_vm_macs,
            vector<vector<string> > vm_mac_array) {

        printf("num_of_mac_ip_maps %d\n",nu_vm_ips);
        printf("num_of_vm_macs %d\n",nu_vm_macs);
        vm_dtls->num_of_mac_ip_maps = nu_vm_ips;
        vm_dtls->num_of_vm_macs = nu_vm_macs;

        vnm_mac_ip_map_t *vnm_mac_ip_map_array =
                vnm_get_mig_notif_vm_mac_ip_map_array(vm_dtls);
        uint32_t j = 0;

        struct ether_addr *vm_dtls_mac_array = vnm_get_mig_notif_vm_mac_array(
                vm_dtls);
        uint32_t i = 0;
        uint32_t ip_cnt =0;
        for (i = 0; i < nu_vm_macs; i++) {
            struct ether_addr ether;
            vector<string> macList = vm_mac_array[i];
            //0th element contains MAC and the remaining IP for that MAc
            int len = macList[0].length();
            if (len < VNM_ETHER_ADDR_MAX_LEN) {
                struct ether_addr * s = (struct ether_addr *) ether_aton(
                        vm_mac_array[i][0].c_str());

                memcpy(&ether, s, sizeof(struct ether_addr));
            }

            vm_dtls_mac_array[i] = ether;

            for (j = 1; j < macList.size(); j++) {
                struct vnm_ip_addr_t vmIP;
                convert_string_to_ip(&vmIP, macList[j].c_str());
                vnm_mac_ip_map_array[ip_cnt].vm_ip_addr = vmIP;
                vnm_mac_ip_map_array[ip_cnt++].vm_mac = ether;
            }
        }
    }

    void EventEngineObjectManager::populateESXDetails(vnm_esx_dtls_t *esx_dtls,
            uint32_t num_of_kernel_ip_addrs, vnm_esx_role_t esx_role,
            char *console_ip, vector<vector<string> > kern_ip_array) {
        esx_dtls->num_of_kernel_ip_addrs = num_of_kernel_ip_addrs;
        esx_dtls->esx_role = esx_role;
        struct vnm_ip_addr_t conIP;

        convert_string_to_ip(&conIP, console_ip);
        esx_dtls->esx_ip_addr = conIP;
        uint32_t i = 0;
        for (i = 0; i < num_of_kernel_ip_addrs; i++) {
            struct vnm_ip_addr_t kernelIP;
            if(kern_ip_array[i].size()==2){
            convert_string_to_ip(&kernelIP, kern_ip_array[i][0].c_str());
            esx_dtls->esx_kernel_ip_addrs[i].ip = kernelIP;
            esx_dtls->esx_kernel_ip_addrs[i].nic_type = atoi(
                    kern_ip_array[i][1].c_str());
            }else{
                tracePrintf(TRACE_LEVEL_INFO, "EventEngine : Kernel IP not populated"
                                );
            }
        }
    }

    uint32_t calculateNumberofIP(vector<vector<string> > vmmacaddrList) {
        uint32_t num_of_ips = 0;
        //printf("size %ld",vmmacaddrList.size());
        for (uint32_t i = 0; i < vmmacaddrList.size(); i++) {
            num_of_ips += vmmacaddrList.at(i).size() - 1;
           // printf("list size %ld",vmmacaddrList.at(i).size());
        }
        //printf(" num_of_ips %u\n",num_of_ips);
        return num_of_ips;
    }

    vnm_resp_t *EventEngineObjectManager::getVNMResp(
            MigrationEventsMessage *message) {
        int nu_vm_ip = calculateNumberofIP(message->vmmacaddrList);
        int nu_vm_mac = message->vmmacaddrList.size();
        int nu_src_kernel_ip = message->srckernelIPList.size();
        int nu_dest_kernle_ip = message->destKernelIPList.size();
        vnm_resp_t *resp = allocate_memory_for_notif_plds(1, nu_vm_ip,
                nu_vm_mac, nu_src_kernel_ip, nu_dest_kernle_ip);
        resp->rsp_type = VNM_NOTIF_MIG_DTLS;
        resp->msg.num_plds = 1;
        resp->msg.payload.mig_notif_pld[0].cookie.mig_task_id =
                message->eventChainID;
        //TODO- Length check
        int len = message->vmMOR.length();
        if (len < VNM_VM_ID_MAX_LEN) {
            strcpy(resp->msg.payload.mig_notif_pld[0].cookie.vm_id,
                    message->vmMOR.c_str());
        }

        struct vnm_ip_addr_t vcenterIP;
        if (convert_string_to_ip(&vcenterIP, message->vCenterIP.c_str()) == 0) {
            resp->msg.payload.mig_notif_pld[0].cookie.vcenter_ip = vcenterIP;
        }

        resp->msg.payload.mig_notif_pld[0].ret_code = VNM_ERR_SUCCESS;

        switch (message->eventType) {
            case 0:
                resp->msg.payload.mig_notif_pld[0].vm_migration_status =
                        VNM_VM_MIGRATION_INPROGRESS;
                break;
            case 1:
                resp->msg.payload.mig_notif_pld[0].vm_migration_status =
                        VNM_VM_MIGRATION_INPROGRESS;
                break;
            case 2:
                resp->msg.payload.mig_notif_pld[0].vm_migration_status =
                        VNM_VM_MIGRATION_COMPLETED;
                break;
            case 3:
                resp->msg.payload.mig_notif_pld[0].vm_migration_status =
                        VNM_VM_MIGRATION_FAILED;
                break;
        }

        vnm_mig_notif_pld_t *req_pld = vnm_get_first_notif_pld(resp);
        vnm_vm_dtls_t *req_vm_dtls = vnm_get_mig_notif_vm_dtls(req_pld);
        populateVMDetails(req_vm_dtls, nu_vm_ip, nu_vm_mac,
                message->vmmacaddrList);
        vnm_esx_dtls_t *req_src_esx_dtls = vnm_get_mig_notif_pld_lcl_esx_dtls(
                req_pld);
        populateESXDetails(req_src_esx_dtls, nu_src_kernel_ip, VNM_ESX_SOURCE,
                (char*) (((message->srcConsoleIP.c_str()))),
                message->srckernelIPList);
        vnm_esx_dtls_t *req_dst_esx_dtls = vnm_get_mig_notif_pld_rmt_esx_dtls(
                req_pld);
        populateESXDetails(req_dst_esx_dtls, nu_dest_kernle_ip,
                VNM_ESX_DESTINATION,
                (char*) (((message->destConsoleIP.c_str()))),
                message->destKernelIPList);
        return resp;
    }

    void EventEngineObjectManager::handleMessageFromVCenter(
            MigrationEventsMessage *message) {
        tracePrintf(TRACE_LEVEL_DEBUG, "Recieved Event with event ID\n",
                message->eventID);
        vnm_resp_t *resp = getVNMResp(message);
        eventsQueue.pushAndNotify(resp);
        int status = WAVE_MESSAGE_SUCCESS;
        message->setCompletionStatus(status);
        reply(message);
    }

    void EventEngineObjectManager::fillReturnCodeForPowerStatus(
            int returnStatus, vnm_resp_t *& vnm_resp) {
        //Only error response are logged at INFO level
        switch (returnStatus) {
            case POWER_STATUS_POWERED_OFF:
                vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status =
                        VNM_VM_POWERED_OFF;
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code = VNM_ERR_SUCCESS;
                break;
            case POWER_STATUS_POWERED_ON:
                vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status =
                        VMM_VM_POWERED_ON;
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code = VNM_ERR_SUCCESS;
                break;
            case POWER_STATUS_SUSPENDED:
                vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status =
                        VNM_VM_SUSPENDED;
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code = VNM_ERR_SUCCESS;
                break;

            case POWER_STATUS_INVALID_CONNECTION:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code =
                        VNM_ERR_VMOTION_MANAGER_INVALID_CONNECTION;
                trace(TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - Invalid Connection");
                break;
            case POWER_STATUS_INVALID_CREDENTIALS:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code =
                        VNM_ERR_VMOTION_MANAGER_INVALID_CREDENTIALS;
                trace(TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - Invalid Credentials");
                break;
            case POWER_STATUS_INVALID_VERSION:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code =
                        VNM_ERR_VMOTION_MANAGER_INVALID_VERSION;
                trace(TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - Invalid Version");
                break;
            case POWER_STATUS_NO_SERVICE:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code =
                        VNM_ERR_VMOTION_MANAGER_NO_SERVICE;
                trace(TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - Invalid Service");
                break;
            case POWER_STATUS_INVALID_VM:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code =
                        VNM_ERR_VMOTION_MANAGER_INVALID_VM;
                trace(TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - Invalid VM");
                break;

            case POWER_STATUS_VCENTER_NOT_CONFIGURED_ACTIVATED:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code =
                        VNM_ERR_VMOTION_MANAGER_NOT_CONFIGURED_ACTIVATED;
                trace(
                        TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - vCenter not Configured/Activated");
                break;
            default:
                vnm_resp->msg.payload.vm_pwd_pld[0].ret_code = VNM_ERR_UNKNOWN;
                trace(TRACE_LEVEL_INFO,
                        "Event Engine:Power Status - Unknowm Error");
                break;
        }
    }

    void EventEngineObjectManager::eventEngineStartStopHandler(
            EventEngineMessage *eventEngineMessage) {
        if (eventEngineMessage->getCmdcode() == EVENT_ENGINE_START) {
            trace(TRACE_LEVEL_INFO,
                    "Global EvenEngine: Starting Event Dispatch thread");
            eventsQueue.reStart();

            if (dispatchThread->running == false) {
                trace(
                        TRACE_LEVEL_INFO,
                        "Global EvenEngine: Event Dispatch Thread reports being started");
                dispatchThread->run();
            }
            trace(TRACE_LEVEL_INFO,
                    "Global EvenEngine: Starting Event PHP thread");
            string password = vCenterCrypto::decryptPassword(
                    eventEngineMessage->getPassword());
            phpThread->setVcenter(eventEngineMessage->getVcenter());
            phpThread->setUsername(eventEngineMessage->getUsername());
            phpThread->setPassword(password);
            phpThread->setUrl(eventEngineMessage->getUrl());

            if (phpThread->running == false) {
                trace(TRACE_LEVEL_INFO,
                        "Global EvenEngine: PHP thread being started");
                phpThread->run();
            };
        }
        if (eventEngineMessage->getCmdcode() == EVENT_ENGINE_STOP) {
            trace(TRACE_LEVEL_INFO,
                    "Global EvenEngine: Stopping Event Dispatch thread");
            eventsQueue.stopQueue();

            trace(TRACE_LEVEL_INFO,
                    "Global EvenEngine: Stopping Event PHP thread");
            phpThread->stopActivity();

        }
        if (eventEngineMessage->getCmdcode() == 3) {
            trace(TRACE_LEVEL_DEBUG,
                    "EventEngineObjectManager :: Power Status Query");
            eventEngineMessage->setCmdcode(20);
            UI32 size;

            vnm_req_t * req = (vnm_req_t *) eventEngineMessage->findBuffer(
                    POWER_PLD_REQ_TAG, size);

            tracePrintf(TRACE_LEVEL_INFO, "Request Cookie %d\n",
                    req->payload.cookie[0].vm_id);
            VcenterManagedObject* pVcenterManagedObject =
                    getVcenterConfigFromMo();

            int returnStatus = -1;
            if (pVcenterManagedObject == NULL
                    || pVcenterManagedObject->m_activate == false) {
                returnStatus = POWER_STATUS_VCENTER_NOT_CONFIGURED_ACTIVATED;
            }
            else {
                string password = vCenterCrypto::decryptPassword(
                        pVcenterManagedObject->getPassword());
                returnStatus = fetchVMPowerStatus(
                        pVcenterManagedObject->getIpaddress(),
                        pVcenterManagedObject->getUsername(), password,
                        req->payload.cookie[0].vm_id);
            }

            vnm_resp_t* vnm_resp = allocate_memory_for_power_plds(1, 1, 1);

            vnm_resp->rsp_type = VNM_RSP_VM_POWER_STATUS;
            vnm_resp->msg.num_plds = 1;

            strcpy(vnm_resp->msg.payload.vm_pwd_pld[0].cookie.vm_id,
                    req->payload.cookie[0].vm_id);
            vnm_resp->msg.payload.vm_pwd_pld[0].cookie.vcenter_ip =
                    req->payload.cookie[0].vcenter_ip;

            fillReturnCodeForPowerStatus(returnStatus, vnm_resp);

            eventEngineMessage->addBuffer(POWER_PLD_RESP_TAG,
                    sizeof(vnm_resp_t), vnm_resp, false);

            int status = WAVE_MESSAGE_SUCCESS;
            eventEngineMessage->setCompletionStatus(status);

            reply(eventEngineMessage);

        }

    }

    VcenterManagedObject*
    EventEngineObjectManager::getVcenterConfigFromMo() {
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
                VcenterManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously(
                &syncQueryCtxt);
        VcenterManagedObject* pMO = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size();
            if (0 == numberOfResults) {
            }
            else if (1 == numberOfResults) {
                pMO = dynamic_cast<VcenterManagedObject*>((*pResults)[0]);
            }
            else {
                /* Multiple Vcenter is not supported hence release the
                 *  memory and return NULL
                 */
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
            }
        }

        return pMO;
    }

    int EventEngineObjectManager::fetchVMPowerStatus(string vcenter,
            string user, string pass, string vm_mor) {
        int returnStatus = -1;

        int ret;
        int pid = fork();

        if (pid == 0) {
            if ((ret = chdir(PHPDIR_PATH)) < 0) {
                trace(TRACE_LEVEL_INFO,
                        string("Failed chdir to: ") + string(PHPDIR_PATH));

            }
            else if ((ret = execl(INTERPRETER, INTERPRETER, VM_POWER_STATUS_PHP,
                    vcenter.c_str(), user.c_str(), pass.c_str(), vm_mor.c_str(),
                    NULL)) < 0) {
                tracePrintf(
                        TRACE_LEVEL_INFO,
                        "EventsEngineWorker::VM Power Status PHP execution failed");
            }
            //child Process Exit,if it reaches here.
            exit(-1);

        }
        else {
            int childstatus = 0;
            int tpid = wait4(pid, &childstatus, 0, NULL);
            if (tpid == pid) {
                int wexitStatus = WEXITSTATUS(childstatus);
                if ((wexitStatus <= 7) && (wexitStatus >= 0)) {
                    tracePrintf(
                            TRACE_LEVEL_DEBUG,
                            "EventsEngineWorker::VM Power Status PHP Succeded %d",
                            WEXITSTATUS(childstatus));
                    returnStatus = wexitStatus;
                }
                else {
                    tracePrintf(TRACE_LEVEL_DEBUG,

                    "EventsEngineWorker::VM Power Status PHP Failed Status %d",
                            WEXITSTATUS(childstatus));
                }
            }

        }

        return returnStatus;
    }

    EventEngineDispatchThread::EventEngineDispatchThread(
            EventEngineObjectManager *objMan) :running(false),
            objManager(objMan) {
    }

    EventEngineDispatchThread::~EventEngineDispatchThread() {
    }

    WaveThreadStatus EventEngineDispatchThread::start() {
        trace(TRACE_LEVEL_INFO, string("EventEngineDispatchThread::start"));
        running = true;
        try {
            while (TRUE) {

                EventEngineLocalMessage *dmsg = new EventEngineLocalMessage();

                trace(
                        TRACE_LEVEL_DEBUG,
                        string(
                                "EventEngineDispatchThread::Pop Event from the Queue"));
                vnm_resp_t *resp = objManager->eventsQueue.waitAndPop();

                //buffer gets de-allocated by framework.
                dmsg->addBuffer(NOTIFICATION_PLD_TAG, resp->size, resp, false);

                //Use sendToWavecluster
                WaveSendToClusterContext *send_ctx_p;
                bool flag = true;
                send_ctx_p =
                        new WaveSendToClusterContext(
                                objManager,
                                reinterpret_cast<PrismAsynchronousCallback>(&EventEngineObjectManager::EventEngineObjectManagerSendToClusterCallback),
                                NULL);
                //new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&AAAGlobalObjectManager::configReplayCompletedCallback), NULL);

                send_ctx_p->setPPrismMessageForPhase1(dmsg);
                send_ctx_p->setPartialSuccessFlag(flag);
                send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(flag);
                objManager->sendToWaveCluster(send_ctx_p);
            }
        }
        catch (int i) {
            trace(TRACE_LEVEL_INFO, string("EventEngineDispatchThread::Stop"));
            running = false;
            this->stopCallingThread();

        }

        return (WAVE_THREAD_SUCCESS);
    }
    void EventEngineObjectManager::EventEngineObjectManagerSendToClusterCallback(
            WaveSendToClusterContext *ctx_p) {
        //PrismLinearSequencerContext *seq_ctx_p;
        LocationId locationid;
        ClusterGetFirstFailureLocation(ctx_p, locationid);

        vector<LocationId> locationIdFailedNodes;
        ResourceId status = WAVE_MESSAGE_ERROR;

        //seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());

        locationIdFailedNodes = ctx_p->getLocationsToSendToForPhase1();
        for (UI32 i = 0; i < locationIdFailedNodes.size(); i++) {
            status = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locationIdFailedNodes[i]);
            if (status != WAVE_MESSAGE_SUCCESS) {
                trace(
                        TRACE_LEVEL_INFO,
                        string("getCompletionStatus: ") + status
                                + (" for location: ")
                                + locationIdFailedNodes[i]);
            }
        }

        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p;
        //seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
        return;
    }

}

