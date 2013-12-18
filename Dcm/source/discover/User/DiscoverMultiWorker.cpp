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

/*
 * DiscoverMultiWorker.cpp
 *
 *  Created on: May 28, 2012
 *      Author: rmadapur
 */
#include "discover/User/DiscoverMultiWorker.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Utils/DceClusterUtils.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/Framework/discoverObjectManager.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverManagedObjects.h"
#include "discover/StateMachine/DiscoveryStateMachineCollection.h"
#include "discover/StateMachine/DiscoveryStateMachine.h"
#include "discover/collector/vCenterEventCollector.h"
#include "vCenter/Framework/vCenterCrypto.h"
#include "File/Local/FilePushFileMessage.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include <sstream>
#include "Lldp/Global/CdpGlobalConfSetMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs {
    DiscoverMultiWorker *DiscoverMultiWorker::instance = NULL;

    DiscoverMultiWorker::DiscoverMultiWorker(
            DiscoverObjectManager *pDiscoverObjectManager) :
        WaveWorker(pDiscoverObjectManager) {
            DiscoverManagedObject DiscoverManagedObject(pDiscoverObjectManager);
            DiscoverManagedObject.setupOrm();
            addManagedClass(DiscoverManagedObject::getClassName(), this);
            addOperationMap(
                    DISCOVERDISCOVER,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DiscoverDiscoverMessageHandler));
            addOperationMap(
                    DISCOVER_INTERNAL,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DiscoverInternalMessageHandler));

            addOperationMap(
                    DISCOVER_DISTRIBUTE,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DiscoverDistributeMessageHandler));
            addOperationMap(
                    DISCOVER_DISTRIBUTE_DB,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DiscoverDistributeDBMessageHandler));

            addOperationMap(DISCOVER_UPDATE,reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DiscoverUpdateMessageHandler));
            addOperationMap(
                    DATACENTER_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DatacenterEventMessageHandler));
            addOperationMap(
                    VSWITCH_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::VswitchEventMessageHandler));
            addOperationMap(
                    VMNIC_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::VmnicEventMessageHandler));
            addOperationMap(
                    PGHOSTVS_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::PgHostVsEventMessageHandler));
            addOperationMap(
                    VM_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::VMEventMessageHandler));
            addOperationMap(
                    DVS_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DVSEventMessageHandler));
            addOperationMap(
                    DVPG_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DVPGEventMessageHandler));
            addOperationMap(
                    HOST_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::HostEventMessageHandler));
            addOperationMap(
                    VNIC_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::VnicEventMessageHandler));
            addOperationMap(
                    SPG_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::SpgEventMessageHandler));
            addOperationMap(
                    SNIC_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::SnicEventMessageHandler));
            addOperationMap(
                    DP_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverMultiWorker::DPEventMessageHandler));
            instance = this;
            m_pSynchronizingMutex = new PrismMutex();

        }

    PrismMessage *DiscoverMultiWorker::createMessageInstance(
            const UI32 &operationCode) {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode) {

            case DISCOVERDISCOVER:
                pPrismMessage = new DiscoverDiscoverMessage();
                break;
            case DISCOVER_INTERNAL:
                pPrismMessage = new DiscoverInternalMessage();
                break;
            case DISCOVER_DISTRIBUTE:
                pPrismMessage = new DiscoverDistributeMessage();
                break;
            case DISCOVER_DISTRIBUTE_DB:
                pPrismMessage = new DistributeDBMessage();
                break;
            case DATACENTER_EVENT:
                pPrismMessage = new DatacenterEventMessage();
                break;
            case VSWITCH_EVENT:
                pPrismMessage = new VswitchEventMessage();
                break;
            case PGHOSTVS_EVENT:
                pPrismMessage = new PgHostVsEventMessage();
                break;
            case VM_EVENT:
                pPrismMessage = new VMEventMessage();
                break;
            case DVS_EVENT:
                pPrismMessage = new DVSEventMessage();
                break;
            case DVPG_EVENT:
                pPrismMessage = new DVPGEventMessage();
                break;
            case HOST_EVENT:
                pPrismMessage = new HostEventMessage();
                break;
            case VNIC_EVENT:
                pPrismMessage = new VNICEventMessage();
                break;
            case VMNIC_EVENT:
                pPrismMessage = new VmnicEventMessage();
                break;
            case SPG_EVENT:
                pPrismMessage = new SPGEventMessage();
                break;
            case SNIC_EVENT:
                pPrismMessage = new SNICEventMessage();
                break;
            case DP_EVENT:
                pPrismMessage = new VNMDPMessage();
                break;
            case DISCOVER_UPDATE:
                pPrismMessage = new DiscoverUpdateMessage();
                break;
            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }


    void DiscoverMultiWorker::DiscoverUpdateMessageHandler(DiscoverUpdateMessage *pDiscoverUpdateMessage)
    {
        if(pDiscoverUpdateMessage->getCmdcode()==DISC_CMD_DELETE_VCENTER_MO){
            this->deleteDiscoverManagedObject(pDiscoverUpdateMessage->getVcenter());

            int status = WAVE_MESSAGE_SUCCESS;
            pDiscoverUpdateMessage->setCompletionStatus(status);
            reply(pDiscoverUpdateMessage);
            return;
        }

        DiscoverManagedObject* pMO = NULL;
        string vcenterName = pDiscoverUpdateMessage->getVcenter();
        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "DiscoverUpdateMessageHandler...vcenter %s",  vcenterName.c_str());
        discoverStageMachineStates_t state = (discoverStageMachineStates_t)pDiscoverUpdateMessage->getState();
        discoverExitStatus_t exitStatus = (discoverExitStatus_t)pDiscoverUpdateMessage->getExitStatus(); 

        if ((pMO = getDiscoverManagedObject(vcenterName)) != NULL) {
            //m_pSynchronizingMutex->lock();
            startTransaction();
            VIC_PLUG_LOG(TRACE_LEVEL_DEBUG, "update Managed Object for %s", vcenterName.c_str());

            struct timeval tv;
            time_t currentTime = 0;

            /* Get current time */
            gettimeofday(&tv, NULL);
            currentTime = tv.tv_sec;

            switch (state) {

                case DISC_STATE_START:
                    pMO->setStartTS(currentTime);
                    break;

                case DISC_STATE_END:
                    if(exitStatus == DISC_EXIT_STATUS_NO_VCENTER_SERVICE)
                        pMO->setStartTS(currentTime);
                    pMO->setEndTS(currentTime);
                    pMO->setExitCode(exitStatus);
                    break;

                default:
                    break;

            }
            pMO->setState(state);
            updateWaveManagedObject(pMO);
            commitTransaction();

            //m_pSynchronizingMutex->unlock();
            delete pMO;
            int status = WAVE_MESSAGE_SUCCESS;
            pDiscoverUpdateMessage->setCompletionStatus(status);
            reply(pDiscoverUpdateMessage);
        }
    }

    WaveManagedObject *DiscoverMultiWorker::createManagedObjectInstance(
            const string &managedClassName) {
        VIC_PLUG_LOG(TRACE_LEVEL_DEBUG, "%s", "Create Managed Object Instance");
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((DiscoverManagedObject::getClassName()) == managedClassName) {
            pWaveManagedObject =
                new DiscoverManagedObject(
                        dynamic_cast<DiscoverObjectManager *>(getPWaveObjectManager()));
        }
        else {
            VIC_PLUG_LOG(
                    TRACE_LEVEL_FATAL,
                    "DiscoverWorker::createManagedObjectInstance : Unknown Managed Class Name : %s",
                    managedClassName.c_str());
        }
        return (pWaveManagedObject);
    }
    ResourceId DiscoverMultiWorker::deleteDiscoverManagedObject(
            string vcenterName) {
        DiscoverManagedObject* pMO = NULL;
        if ((pMO = getDiscoverManagedObject(vcenterName)) != NULL) {
            m_pSynchronizingMutex->lock();
            startTransaction();
            delete pMO;
            VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                    "Deleted Managed Object for %s",vcenterName.c_str());
            commitTransaction();
            m_pSynchronizingMutex->unlock();
        }
        return (WAVE_MESSAGE_SUCCESS);

    }

    UI32
        DiscoverMultiWorker::getVcCount() {
            DiscoveryStateMachineCollection * _smlColl =
                DiscoverMultiWorker::getStateMachineCollection();
            UI32 numberOfResults = _smlColl->getStateMachines().size() ;
            VIC_PLUG_LOG(TRACE_LEVEL_INFO, "getVcCount  %d", numberOfResults);
            return  numberOfResults;

        }


    DiscoverManagedObject*
        DiscoverMultiWorker::getDiscoverManagedObject(string vcenterName) {
            WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
                    DiscoverManagedObject::getClassName());
            syncQueryCtxt.addAndAttribute(new AttributeString(vcenterName, "id"));
            vector<WaveManagedObject *> *pResults = querySynchronously(
                    &syncQueryCtxt);
            DiscoverManagedObject* pMO = NULL;

            if (NULL != pResults) {
                UI32 numberOfResults = pResults->size();
                if (0 == numberOfResults) {
                    VIC_PLUG_LOG(TRACE_LEVEL_INFO, "%s", "MO doesn't exist");
                }
                else if (1 == numberOfResults) {
                    pMO = dynamic_cast<DiscoverManagedObject*>((*pResults)[0]);
                    VIC_PLUG_LOG(TRACE_LEVEL_DEBUG,
                            "Found Managed Object for %s", vcenterName.c_str());
            		pResults->clear();
            		delete pResults;
                }
                else {
                    /* Managed object inconsistent by having multiple entries for
                     * same key
                     */
                	WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                }
            }

            return pMO;
        }

    void DiscoverMultiWorker::createDiscoverManagedObject(string vcenterName) {
        DiscoverManagedObject* pMO = NULL;

        pMO = getDiscoverManagedObject(vcenterName);
        if (pMO == NULL) {

            //multiple threads can access the DB,so guarding it
            m_pSynchronizingMutex->lock();
            startTransaction();
            pMO =
                new DiscoverManagedObject(
                        dynamic_cast<DiscoverObjectManager*>(getPWaveObjectManager()));
            pMO->setId(vcenterName);
            VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                    "Created Managed Object for %s",vcenterName.c_str());

            struct timeval tv;
            time_t currentTime = 0;

            /* Get current time */
            gettimeofday(&tv, NULL);
            currentTime = tv.tv_sec;

            pMO->setStartTS(currentTime);
            pMO->setDiscStartTS(0);
            pMO->setDiscEndTS(0);
            pMO->setPpStartTS(0);
            pMO->setPpEndTS(0);
            pMO->setExitCode(0);
            pMO->setEndTS(0);
            pMO->setState(0);

            commitTransaction();
            m_pSynchronizingMutex->unlock();
        }
        //Release Memory
        delete pMO;
    }

    void DiscoverMultiWorker::updateDiscoveryStatus(string vcenterName,
            discoverStageMachineStates_t state,
            discoverExitStatus_t exitStatus) {
        DiscoverManagedObject* pMO = NULL;
        if ((pMO = getDiscoverManagedObject(vcenterName)) != NULL) {
            m_pSynchronizingMutex->lock();
            startTransaction();
            VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                    "update Managed Object for %s",vcenterName.c_str());

            struct timeval tv;
            time_t currentTime = 0;

            /* Get current time */
            gettimeofday(&tv, NULL);
            currentTime = tv.tv_sec;

            switch (state) {

                case DISC_STATE_START:
                    pMO->setStartTS(currentTime);
                    break;

                case DISC_STATE_END:
                    pMO->setEndTS(currentTime);
                    pMO->setExitCode(exitStatus);
                    break;

                default:
                    break;

            }
            pMO->setState(state);

            updateWaveManagedObject(pMO);
            commitTransaction();
            m_pSynchronizingMutex->unlock();
            delete pMO;

        }
    }
    void DiscoverMultiWorker::DiscoverInternalMessageHandler(
            DiscoverInternalMessage *pDiscoverDiscoverMessage) {

        UI32 cmdCode = pDiscoverDiscoverMessage->getCmdcode();

        switch (cmdCode) {
            case DISC_INT_TYPE_GET_STATE_MACHINES: {
                                                       DiscoveryStateMachineCollection * _smlColl =
                                                           DiscoverMultiWorker::getStateMachineCollection();
                                                       vector<vector<string> > resp;
                                                       vector<DiscoveryStateMachine *> smList =
                                                           _smlColl->getStateMachines();

                                                       vector<string> title;
                                                       title.push_back("vCenter");
                                                       title.push_back("Full Disc");
                                                       title.push_back("Basic Disc");
                                                       title.push_back("Crt Disc Type");
                                                       title.push_back("Crt Disc State");
                                                       resp.push_back(title);
                                                       vector<string> titleBanner;
                                                       titleBanner.push_back("===============");
                                                       titleBanner.push_back("===============");
                                                       titleBanner.push_back("===============");
                                                       titleBanner.push_back("===============");
                                                       titleBanner.push_back("===============");

                                                       resp.push_back(titleBanner);

                                                       for (size_t i = 0; i < smList.size(); i++) {
                                                           DiscoveryStateMachine *sm = smList.at(i);
                                                           vector<string> inner;
                                                           inner.push_back(sm->vCenterdata->vcenter);
                                                           string fullDiscovery;
                                                           if (sm->fullDiscoveryTimerHandle == 0) {
                                                               if (sm->scheduleFullDiscovery) {
                                                                   fullDiscovery = "Scheduled";
                                                               }
                                                               else {
                                                                   fullDiscovery = "Not Scheduled";
                                                               }
                                                           }
                                                           else {
                                                               fullDiscovery = "Timer Running";
                                                           }

                                                           inner.push_back(fullDiscovery);

                                                           string basicDiscovery ="Not Available";


                                                           inner.push_back(basicDiscovery);
                                                           inner.push_back(
                                                                   DiscoveryStateMachine::DISCOVERY_TYPE_STRING[sm->currentDiscoveryType]);
                                                           inner.push_back(
                                                                   DiscoveryStateMachine::DISCOVERY_STATE_STRING[sm->state->discState]);

                                                           resp.push_back(inner);
                                                       }

                                                       pDiscoverDiscoverMessage->respList = resp;
                                                       break;
                                                   }
        }

        int status = WAVE_MESSAGE_SUCCESS;
        pDiscoverDiscoverMessage->setCompletionStatus(status);
        reply(pDiscoverDiscoverMessage);

    }

    void DiscoverMultiWorker::DiscoverDistributeDBMessageHandler(
            DistributeDBMessage *pDiscoverDistributeDBMessage) {

        int  status = WAVE_MESSAGE_SUCCESS;
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Received Distributed DB Message ");

        string dbfile = pDiscoverDistributeDBMessage->getDBfile();
        string destdbfile = dbfile;

        if(pDiscoverDistributeDBMessage->getDestDBfile() != "")
            destdbfile = pDiscoverDistributeDBMessage->getDestDBfile();

        vector<LocationId> connectedLocationsVector = pDiscoverDistributeDBMessage->getLocationIds();

        //dbfile is pushed to all nodes
        FilePushFileMessage *pMessage = new FilePushFileMessage(
                dbfile,
                destdbfile,
                FrameworkToolKit::getThisLocationId(),
                connectedLocationsVector);

        pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Send FilePushMessage");
        status = sendSynchronously(pMessage);

        pDiscoverDistributeDBMessage->setCompletionStatus(status);
        reply(pDiscoverDistributeDBMessage);
        delete pMessage;



    }



    void DiscoverMultiWorker::DatacenterEventMessageHandler(
            DatacenterEventMessage *pEventMessage) {

        LocalDatacenterEventMessage *m = new LocalDatacenterEventMessage();
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ", 
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);

    }


    void DiscoverMultiWorker::DiscoverDistributeMessageHandler(
            DiscoverDistributeMessage *pDiscoverDistributeMessage) {

        DiscoverLocalDiscoverMessage *m = new DiscoverLocalDiscoverMessage();

        //Use sendToWavecluster
        WaveSendToClusterContext *send_ctx_p;
        send_ctx_p =
            new WaveSendToClusterContext(
                    this,
                    reinterpret_cast<PrismAsynchronousCallback>(&DiscoverMultiWorker::DistributerSendToClusterCallback),
                    pDiscoverDistributeMessage);

        if (pDiscoverDistributeMessage->getLocationIds().size() > 0) {
            send_ctx_p->setLocationsToSendToForPhase1(
                    pDiscoverDistributeMessage->getLocationIds());
        }

        /* Read the data from received message and populate
         * sending message
         */
        m->setVcenter(pDiscoverDistributeMessage->getVcenter());
        m->setCmdcode(pDiscoverDistributeMessage->getCmdcode());
        m->setDataList(pDiscoverDistributeMessage->getDataList());

        bool flag = true;

        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(flag);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(flag);

        /* Send the message to all nodes in cluster */
        sendToWaveCluster(send_ctx_p);



    }

    void DiscoverMultiWorker::DistributerSendToClusterCallback(
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
                string errMsg = string("getCompletionStatus: ") + status
                    + (" for location: ") + locationIdFailedNodes[i];
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", errMsg.c_str());
            }
        }



        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Recieved message from all node");

        int DISTR_status = WAVE_MESSAGE_SUCCESS;
        DiscoverDistributeMessage *pDiscoverDistributeMessage;
        pDiscoverDistributeMessage = reinterpret_cast<DiscoverDistributeMessage *>(ctx_p->getPCallerContext());
        pDiscoverDistributeMessage->setCompletionStatus(DISTR_status);
        reply(pDiscoverDistributeMessage);
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p;
        return;
    }

    void DiscoverMultiWorker::VswitchEventMessageHandler(
            VswitchEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());


        LocalVswitchEventMessage *m = new LocalVswitchEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::VmnicEventMessageHandler(
            VmnicEventMessage *pEventMessage) {         
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s. %s. %s. %s. %s. ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getDvs().c_str(),
                pEventMessage->m_domain.getDvsNN().c_str(),
                pEventMessage->m_domain.getDcId().c_str(),
                pEventMessage->m_domain.getName().c_str());
        LocalVmnicEventMessage *m = new LocalVmnicEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::PgHostVsEventMessageHandler(
            PgHostVsEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalPgHostVsEventMessage *m = new LocalPgHostVsEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);

    }

    void DiscoverMultiWorker::VMEventMessageHandler(
            VMEventMessage *pEventMessage) {
        //        LocalDatacenterEventMessage *m = new LocalDatacenterEventMessage();
        LocalVMEventMessage *m = new LocalVMEventMessage();
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());


        m->id = pEventMessage->m_domain.getId();
        m->name = pEventMessage->m_domain.getName();

        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::DVSEventMessageHandler(
            DVSEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalDVSEventMessage *m = new LocalDVSEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::DVPGEventMessageHandler(
            DVPGEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalDVPGEventMessage *m = new LocalDVPGEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::HostEventMessageHandler(
            HostEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalHostEventMessage *m = new LocalHostEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::VnicEventMessageHandler(
            VNICEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalVNICEventMessage *m = new LocalVNICEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::SpgEventMessageHandler(
            SPGEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalSPGEventMessage *m = new LocalSPGEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::SnicEventMessageHandler(
            SNICEventMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->m_domain.getId().c_str(),
                pEventMessage->m_domain.getName().c_str());

        LocalSNICEventMessage *m = new LocalSNICEventMessage();
        m->copy(pEventMessage);
        publishEventsToCluster (m, pEventMessage);
    }

    void DiscoverMultiWorker::DPEventMessageHandler(
            VNMDPMessage *pEventMessage) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                "Global received vcenter event %s ", pEventMessage->m_vcenter.c_str()); 
        LocalVNMDPMessage *m = new LocalVNMDPMessage(pEventMessage->m_cmdCode, 
                pEventMessage->m_ifName, 
                pEventMessage->m_hostMor, 
                pEventMessage->m_portId, 
                pEventMessage->m_vcenter);



        //Use sendToWavecluster
        WaveSendToClusterContext *send_ctx_p;
        send_ctx_p = new WaveSendToClusterContext(
                this,
                reinterpret_cast<PrismAsynchronousCallback>(&DiscoverMultiWorker::DistributerSendDPToClusterCallback),
                pEventMessage);
        send_ctx_p->setPPrismMessageForPhase1(m);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);

        sendToWaveCluster(send_ctx_p);
    }

    void DiscoverMultiWorker::DistributerSendDPToClusterCallback(
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
                string errMsg = string("getCompletionStatus: ") + status
                    + (" for location: ") + locationIdFailedNodes[i];
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", errMsg.c_str());
            }
        }


        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "Recieved message from all node");

        int DISTR_status = WAVE_MESSAGE_SUCCESS;
        VNMDPMessage *pDPMessage;
        pDPMessage = reinterpret_cast<VNMDPMessage*>(ctx_p->getPCallerContext());
        pDPMessage->setCompletionStatus(DISTR_status);
        reply(pDPMessage);
        delete ctx_p->getPPrismMessageForPhase1();
        delete ctx_p;
        return;
    }

    void DiscoverMultiWorker::publishEventsToCluster (
            LocalEventMessage *pEventLocalMessage,
            EventMessage      *pEventMessage) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "publishEventsToCluster  !!");

        //Use sendToWavecluster
        WaveSendToClusterContext *send_ctx_p;
        send_ctx_p =
            new WaveSendToClusterContext(
                    this,
                    reinterpret_cast<PrismAsynchronousCallback>(&DiscoverMultiWorker::DistributerEventSendToClusterCallback),
                    pEventMessage);


        bool flag = true;

        send_ctx_p->setPPrismMessageForPhase1(pEventLocalMessage);
        send_ctx_p->setPartialSuccessFlag(flag);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(flag);

        VIC_PLUG_LOG(TRACE_LEVEL_DEBUG, "%s", "calling sendToWaveCluster");
        //         Send the message to all nodes in cluster
        sendToWaveCluster(send_ctx_p);

    }

    void DiscoverMultiWorker::DistributerEventSendToClusterCallback(
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
                string errMsg = string("getCompletionStatus: ") + status
                    + (" for location: ") + locationIdFailedNodes[i];
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", errMsg.c_str());
            }
        }



        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "Recieved message from all node");

        int DISTR_status = WAVE_MESSAGE_SUCCESS;
        EventMessage *pEventMessage;
        pEventMessage = reinterpret_cast<EventMessage  *>(ctx_p->getPCallerContext());
        pEventMessage->setCompletionStatus(DISTR_status);
        reply(pEventMessage);
        delete ctx_p->getPPrismMessageForPhase1();
        //        delete ctx_p;
        return;
    }


    class NodeAddTask: public WorkerTask {
        public:

            vector<UI32> locationVector;
            NodeAddTask(vector<UI32> _locationVector) :
                locationVector(_locationVector) {

                }
            virtual void run() {

                vector<string> vcenterList;
                DiscoveryStateMachineCollection * _smlColl =
                    DiscoverMultiWorker::getStateMachineCollection();
                vector<DiscoveryStateMachine *> smList =
                    _smlColl->getStateMachines();
                for (size_t i = 0; i < smList.size(); i++) {
                    DiscoveryStateMachine *sm = smList.at(i);
                    string vcenter = sm->state->vcenterName;
                    vcenterList.push_back(vcenter);
                }

                DistributionSender *ds = DistributionSender::getInstance();
                ds->nodeAddDistributionhandler(vcenterList, locationVector);

                this->stop();
            }
            virtual void cleanup() {
                delete this;
            }

    };
    void DiscoverMultiWorker::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
    {

         //Framework can send messages event after shutdown, so moving to
         //disable virtual function
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "DiscoverMulti::Disable : Entering ...");

        DiscoveryStateMachineCollection * _smlColl =
            DiscoverMultiWorker::getStateMachineCollection();

        vector<DiscoveryStateMachine *> stateMachines = _smlColl->getStateMachines();
        for(size_t i=0;i<stateMachines.size();i++){
            DiscoveryStateMachine *sm = stateMachines[i];
            if(sm!=NULL){
                if(sm->ecol!=NULL){
                    //Stopping Event Collector
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Stopping Event Collector");
                    sm->ecol->stop();
                    sm->ecol->stopEvents();
                }
                if(sm->collector!=NULL){
                    //Only clean the state Machine, but don't delete the DB.
                    sm->cleanStateMachineOnly = true;
                    //Stopping vCenter Discovery Collector
                    sm->stopRequest = true;
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Stopping Discovery Collector");
                    sm->collector->stop();
                }else { //Indicates no collector is running, so stop
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Stopping Discovery Thread");
                    sm->cleanStateMachineOnly = true;
                    sm->trigger(DEACTIVATE, sm->vCenterdata);
                }
            }
        }


        pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForShutDownPhases->callback ();
    }
    void DiscoverMultiWorker::DiscoverDiscoverMessageHandler(
            DiscoverDiscoverMessage *pDiscoverDiscoverMessage) {

        int status = WAVE_MESSAGE_SUCCESS;
        if(WaveObjectManager::isServiceEnabled(getPWaveObjectManager()->getServiceId())==false){
           VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
               "Message Recieved and Service is disabled, so discard");
           pDiscoverDiscoverMessage->setCompletionStatus(status);
           reply(pDiscoverDiscoverMessage);
           return;
        }
        VIC_PLUG_LOG(VIC_TRACE_INFO,"m_ignore_value =%d",pDiscoverDiscoverMessage->m_ignore_value);

        UI32 msgType = pDiscoverDiscoverMessage->getMsgType();

        UI32 cmdCode = pDiscoverDiscoverMessage->getCmdcode();
        if (cmdCode == DISC_CMD_NODE_ADD) {
            NodeAddTask * nodeAddTask = new NodeAddTask(
                    pDiscoverDiscoverMessage->getLocationIds());
            DiscoverMultiWorker::getWorkerPoolInstance()->addTask(nodeAddTask);
        }else if(cmdCode ==DISC_CMD_DISCOVER_TIMER){
            DiscoveryStateMachineCollection * _smlColl = DiscoverMultiWorker::getStateMachineCollection();
            DiscoveryStateMachine *sm =  _smlColl->findStateMachine(pDiscoverDiscoverMessage->m_vcenter);
            if(sm != NULL){
                sm->trigger(TIMER_INTERVAL,sm->vCenterdata);
            }
        }

        else {

            switch (msgType) {
                case DISC_MSG_TYPE_CREATE: 
                    {
                        StateActivateMessage *stateMsg = NULL;
                        DiscoveryStateMachineCollection * _smlColl = DiscoverMultiWorker::getStateMachineCollection();
                        DiscoveryStateMachine *sm =  _smlColl->findStateMachine(pDiscoverDiscoverMessage->m_vcenter);
                        if(sm == NULL)
                        {
                            stateMsg = new StateActivateMessage(
                                    pDiscoverDiscoverMessage->m_vcenter,
                                    pDiscoverDiscoverMessage->m_url,
                                    pDiscoverDiscoverMessage->m_username,
                                    vCenterCrypto::decryptPassword(
                                        pDiscoverDiscoverMessage->getPassword()),
                                    pDiscoverDiscoverMessage->m_ignore_value,
                                    pDiscoverDiscoverMessage->m_always              
                                    );
                            sm = _smlColl->createStateMachine(stateMsg);
                        }
                        else if(sm->markedForDeletion == true)
                        {
                            status = WRC_VCENTER_COMMON_ERR_DELETE_IN_PROGRESS;
                            VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s", "markedForDeletion is TRUE: Erroring Out");
                            goto error_out;
                        }
                        else //vCenter has been Activated- just activate the state machine
                        {
                            sm->isVcenterActivated = true;
                            sm->trigger(ACTIVATE, sm->vCenterdata);
                            goto error_out;
                        }

                        createDiscoverManagedObject(
                                pDiscoverDiscoverMessage->m_vcenter);
                        if (cmdCode
                                == DISC_CMD_DISCOVER_ALL_VCENTER_ON_NODE_STATRUP) {
                            stateMsg->runReconcile = true;
                        }

                        sm->isVcenterActivated = true;
                        sm->trigger(ACTIVATE, stateMsg);

                        VIC_PLUG_LOG(TRACE_LEVEL_INFO,
                                "%s", "submitting the vcenter ...");
                        vCenterEventCollector *ecol = new vCenterEventCollector(
                                stateMsg->vcenter, stateMsg->url,
                                stateMsg->userName, stateMsg->password,sm->uuid);

                        DiscoverMultiWorker::getWorkerPoolInstance()->addTask(ecol);
                        sm->ecol = ecol;
                        configCdpProtocol(true);

                        // sleep(10);
                        // pthread_cancel(ecol->threadId);
                    }
                    break;
                case DISC_MSG_TYPE_DELETE: 
                    {
                        //Same as De-activate, followed by Clean up of vcenter
                        DiscoveryStateMachineCollection * _smlColl =
                            DiscoverMultiWorker::getStateMachineCollection();
                        DiscoveryStateMachine *sm = _smlColl->findStateMachine(
                                pDiscoverDiscoverMessage->m_vcenter);

                        if (sm == NULL) {
                            VIC_PLUG_LOG(
                                    TRACE_LEVEL_INFO,
                                    "%s %s", "No vcenter to delete ",
                                    pDiscoverDiscoverMessage->m_vcenter.c_str());
                            break;
                        }
                        else if (sm->markedForDeletion == true)
                        {
                            VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s", "sm->markedForDeletion is TRUE");
                            //status = WRC_VCENTER_COMMON_ERR_DELETE_IN_PROGRESS;
                            status = WAVE_MESSAGE_ERROR;
                            goto error_out;
                        }

                        sm->markedForDeletion = true;
                        VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s", "markedForDeletion is set TRUE");
                        if (sm->ecol != NULL) {
                            VIC_PLUG_LOG(
                                    TRACE_LEVEL_INFO,
                                    "Event Thread stopping %s",
                                    pDiscoverDiscoverMessage->m_vcenter.c_str());
                            sm->ecol->stop();
                            sm->ecol->stopEvents();
                        }
                        if (sm->collector != NULL) {
                            VIC_PLUG_LOG(
                                    TRACE_LEVEL_INFO,
                                    "Thread stopping %s", pDiscoverDiscoverMessage->m_vcenter.c_str());
                            sm->stopRequest = true;
                            sm->collector->stop();

                        } else { //Indicates no collector is running, so stop

                            sm->trigger(DEACTIVATE, sm->vCenterdata);
                        }
                        if (1 == getVcCount()) {
                            // disable cdp if no vc is configured
                            configCdpProtocol(false);
                        }

                    }

                    break;
                case DISC_MSG_TYPE_DEACTIVATE:
                    {

                        DiscoveryStateMachineCollection * _smlColl =
                            DiscoverMultiWorker::getStateMachineCollection();
                        DiscoveryStateMachine *sm = _smlColl->findStateMachine(
                                pDiscoverDiscoverMessage->m_vcenter);

                        if (sm != NULL) 
                        {
                            sm->isVcenterActivated = false;
                            VIC_PLUG_LOG(TRACE_LEVEL_INFO,"DiscoverDiscoverMessage: Setting statemachine for vcenter %s as De-Activated",pDiscoverDiscoverMessage->m_vcenter.c_str());
                        }
                    }
                    break;

                case DISC_MSG_TYPE_UPDATE: 
                    {
                        //User has indicated a fresh discovery.
                        DiscoveryStateMachineCollection * _smlColl =
                            DiscoverMultiWorker::getStateMachineCollection();
                        DiscoveryStateMachine *sm = _smlColl->findStateMachine(
                                pDiscoverDiscoverMessage->m_vcenter);
                        VIC_PLUG_LOG( TRACE_LEVEL_INFO,"DiscoverDiscoverMessage: Starting discovery %s",pDiscoverDiscoverMessage->m_vcenter.c_str());
                        if (sm != NULL) {
                            if ((sm->markedForDeletion != true ) && (sm->collector == NULL)) {
                                sm->vCenterdata->vcenter = pDiscoverDiscoverMessage->m_vcenter;
                                sm->vCenterdata->url = pDiscoverDiscoverMessage->m_url;
                                sm->vCenterdata->userName = pDiscoverDiscoverMessage->m_username;
                                sm->vCenterdata->password = vCenterCrypto::decryptPassword(pDiscoverDiscoverMessage->getPassword());
                                sm->vCenterdata->ignoreValue = pDiscoverDiscoverMessage->m_ignore_value;
                                sm->vCenterdata->always = pDiscoverDiscoverMessage->m_always;

                                sm->trigger(ACTIVATE, sm->vCenterdata);
                            } else {
                                status = WRC_VCENTER_COMMON_ERR_DELETE_IN_PROGRESS;
                            }
                        }
                    }
                    break;
            }
        }

error_out:
        pDiscoverDiscoverMessage->setCompletionStatus(status);
        reply(pDiscoverDiscoverMessage);
    }

    WorkerThreadPool* DiscoverMultiWorker::workerPool = NULL;
    DiscoveryStateMachineCollection *DiscoverMultiWorker::smColl = NULL;

    WorkerThreadPool* DiscoverMultiWorker::getWorkerPoolInstance() {

        if (workerPool == NULL) {
            workerPool = new WorkerThreadPool(20, 20, 20);
        }
        return workerPool;
    }
    DiscoveryStateMachineCollection* DiscoverMultiWorker::getStateMachineCollection() {

        if (smColl == NULL) {
            smColl = new DiscoveryStateMachineCollection();
        }
        return smColl;
    }
    DiscoverMultiWorker* DiscoverMultiWorker::getInstance() {

        return instance;
    }
    ResourceId DiscoverMultiWorker::startDiscoveryTimer(
            TimerHandle &timerHandle, UI32 timeInMilliSeconds,
            PrismTimerExpirationHandler pPrismTimerExpirationCallback,
            void *pPrismTimerExpirationContext,
            PrismElement *pPrismTimerSender) {
        return this->startTimer(timerHandle, timeInMilliSeconds,
                pPrismTimerExpirationCallback, pPrismTimerExpirationContext,
                pPrismTimerSender);
    }
    ResourceId DiscoverMultiWorker::stopDiscoveryTimer(
            TimerHandle &timerHandle) {
        ResourceId returnCode;
        returnCode = this->deleteTimer(timerHandle);
        timerHandle = 0;
        return returnCode;
    }

    void DiscoverMultiWorker::configCdpProtocol(bool cenable) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "configCdpProtocol  %d", cenable);
        ValClientSynchronousConnection *pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();
        prismAssert(NULL != pValClientSynchronousConnection, __FILE__, __LINE__);
        pValClientSynchronousConnection->configCdpProtocol (cenable);

    }

    WaveMessageStatus DiscoverMultiWorker::sendToLocalPlugins(
            PrismMessage *pPrismMessage, const LocationId &locationId) {
        return this->sendSynchronously(pPrismMessage, locationId);
    }

    DiscoverMultiWorker::~DiscoverMultiWorker() {
        //TODO - Clear the StateMachine in the collection
        delete (smColl);
        //Stop any pending threads and clean up
        delete (workerPool);

        delete (m_pSynchronizingMutex);
        ;
    }

} /* namespace DcmNs */
