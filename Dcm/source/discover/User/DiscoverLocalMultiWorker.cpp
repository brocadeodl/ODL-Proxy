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
 * DiscoverLocalMultiWorker.cpp
 *
 *  Created on: Jun 6, 2012
 *      Author: rmadapur
 */

#include "discover/User/DiscoverLocalMultiWorker.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/Framework/discoverLocalObjectManager.h"

#include "discover/Framework/discoverLocalMessages.h"
#include "discover/Framework/discoverLocalTypes.h"
#include "correlator/DiscoveryCorrelator.h"

#include "correlator/VcenterDiff.h"
#include "discover/utils/StopWatch.h"
#include "discover/utils/FileUtils.h"
#include "DcmResourceIdEnums.h"
#include "discover/User/discoverDcmDef.h"
#include "discover/utils/FileUtils.h"
#include "discover/events/vCenterEventsMessage.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/vsphere/Debug.h"
#include "discover/events/EventMessageHandler.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "vCenter/Framework/vCenterManagedObjects.h"
#include "VirtualDb/DAO/DAO.h"
#include "VirtualDb/Domain/Domain.h"
#include "VirtualDb/DAO/VmnicDAO.h"


namespace DcmNs {

    DiscoverLocalMultiWorker::DiscoverLocalMultiWorker(
            DiscoverLocalObjectManager *pDiscoverLocalObjectManager) :
        WaveWorker(pDiscoverLocalObjectManager) {
            addOperationMap(
                    DISCOVERLOCALDISCOVER,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::DiscoverLocalDiscoverMessageHandler));
            addOperationMap(
                    LOCAL_DATACENTER_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::DatacenterEventMessageHandler));
            addOperationMap(
                    LOCAL_VSWITCH_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::VswitchEventMessageHandler));
            addOperationMap(
                    LOCAL_VMNIC_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::VmnicEventMessageHandler));
            addOperationMap(
                    LOCAL_PGHOSTVS_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::PgHostVsEventMessageHandler));
            addOperationMap(
                    LOCAL_VM_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::VMEventMessageHandler));
            addOperationMap(
                    LOCAL_DVS_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::DVSEventMessageHandler));
            addOperationMap(
                    LOCAL_DVPG_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::DVPGEventMessageHandler));
            addOperationMap(
                    LOCAL_HOST_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::HostEventMessageHandler));
            addOperationMap(
                    LOCAL_VNIC_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::VnicEventMessageHandler));
            addOperationMap(
                    LOCAL_SPG_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::SpgEventMessageHandler));
            addOperationMap(
                    LOCAL_SNIC_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::SnicEventMessageHandler));
            //        addOperationMap(
            //                LOCAL_SIMPLE_EVENT,
            //                reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::SimpleMessageHandler));
            addOperationMap(
                    LOCAL_DP_EVENT,
                    reinterpret_cast<PrismMessageHandler>(&DiscoverLocalMultiWorker::DPEventMessageHandler));


            //Ensure /tmp/Vcenter is created if not already avaiable
            if (!FileUtils::fileexists(VCENTER_DIR)) {
                FileUtils::mkDir(VCENTER_DIR);
            }
            if (!FileUtils::fileexists(MC_VCENTER_DIR)) {
                FileUtils::mkDir(MC_VCENTER_DIR);
            }
        }

    DiscoverLocalMultiWorker::~DiscoverLocalMultiWorker() {

    }
    PrismMessage *DiscoverLocalMultiWorker::createMessageInstance(
            const UI32 &operationCode) {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode) {

            case DISCOVERLOCALDISCOVER:
                pPrismMessage = new DiscoverLocalDiscoverMessage();
                break;
            case LOCAL_DATACENTER_EVENT:
                pPrismMessage = new LocalDatacenterEventMessage();
                break;
            case LOCAL_VSWITCH_EVENT:
                pPrismMessage = new LocalVswitchEventMessage();
                break;
            case LOCAL_PGHOSTVS_EVENT:
                pPrismMessage = new LocalPgHostVsEventMessage();
                break;
            case LOCAL_VM_EVENT:
                pPrismMessage = new LocalVMEventMessage();
                break;
            case LOCAL_DVS_EVENT:
                pPrismMessage = new LocalDVSEventMessage();
                break;
            case LOCAL_DVPG_EVENT:
                pPrismMessage = new LocalDVPGEventMessage();
                break;
            case LOCAL_HOST_EVENT:
                pPrismMessage = new LocalHostEventMessage();
                break;
            case LOCAL_VNIC_EVENT:
                pPrismMessage = new LocalVNICEventMessage();
                break;
            case LOCAL_VMNIC_EVENT:
                pPrismMessage = new LocalVmnicEventMessage();
                break;
            case LOCAL_SPG_EVENT:
                pPrismMessage = new LocalSPGEventMessage();
                break;
            case LOCAL_SNIC_EVENT:
                pPrismMessage = new LocalSNICEventMessage();
                break;
            case LOCAL_DP_EVENT:
                pPrismMessage = new LocalVNMDPMessage();
                break;
            default:
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject *DiscoverLocalMultiWorker::createManagedObjectInstance(
            const string &managedClassName) {
        VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s", "create ");
        return NULL;
    }

    void DiscoverLocalMultiWorker::DatacenterEventMessageHandler(
            LocalDatacenterEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->id.c_str(),
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleDataCenterEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::VswitchEventMessageHandler(
            LocalVswitchEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleVswitchEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);
    }

    void DiscoverLocalMultiWorker::VmnicEventMessageHandler(
            LocalVmnicEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleVmnicEvent(pEventMessage);
        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);
    }

    void DiscoverLocalMultiWorker::PgHostVsEventMessageHandler(
            LocalPgHostVsEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode);

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handlePgHostVsEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);
    }

    void DiscoverLocalMultiWorker::VMEventMessageHandler(
            LocalVMEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->id.c_str(),
                pEventMessage->name.c_str());


        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleVMEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::DVSEventMessageHandler(
            LocalDVSEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->id.c_str(),
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleDVSEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::DVPGEventMessageHandler(
            LocalDVPGEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->id.c_str(),
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleDVPGEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::HostEventMessageHandler(
            LocalHostEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->id.c_str(),
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleHostEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::VnicEventMessageHandler(
            LocalVNICEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d %s ",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode,
                pEventMessage->name.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleVnicEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::SpgEventMessageHandler(
            LocalSPGEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode);

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleSpgEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::SnicEventMessageHandler(
            LocalSNICEventMessage *pEventMessage) {
        pEventMessage->copyDomain();

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the vcenter event %s %d",
                pEventMessage->m_vcenter.c_str(),
                pEventMessage->m_opcode);

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleSnicEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }

    void DiscoverLocalMultiWorker::DPEventMessageHandler(
            LocalVNMDPMessage *pEventMessage) {

        VIC_PLUG_LOG(TRACE_LEVEL_INFO, "Local received the CDP LLDP event %s ",
                pEventMessage->m_vcenter.c_str());

        int status = WAVE_MESSAGE_SUCCESS;
        EventMessageHandler::handleDPEvent(pEventMessage);

        pEventMessage->setCompletionStatus(status);
        reply(pEventMessage);

    }


    void DiscoverLocalMultiWorker::DiscoverLocalDiscoverMessageHandler(
            DiscoverLocalDiscoverMessage *pDiscoverLocalDiscoverMessage) {
        trace(TRACE_LEVEL_INFO, "Received Message: ");
        bool isPrimaryLocation = FrameworkToolKit::isPrimaryLocation();
        bool isNodeRejoin = false;

        UI32 locationRole = FrameworkToolKit::getThisLocationRole();

        //IN single Node cluster(MC) locationRole is returned as LOCATION_STAND_ALONE;
        isPrimaryLocation= isPrimaryLocation||(locationRole==LOCATION_STAND_ALONE);

        string vcenter = pDiscoverLocalDiscoverMessage->getVcenter();
        tracePrintf(TRACE_LEVEL_INFO, "Location Role %0x: \n",locationRole);
		if (pDiscoverLocalDiscoverMessage->getCmdcode() == DISC_CMD_NODE_ADD_VCENTER_NAMES) 
        {
            VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s", "Received Message: Vcenter Names in node Add ");
            std::set<string> vcenterSetInDir;
            string dir = FileUtils::getDBDirectoryLocation();
            VIC_PLUG_LOG(TRACE_LEVEL_INFO,"vCenter Directory Location : %s",  dir.c_str());
            //Fetch list of vcenters as on this nodes filesystem.
            FileUtils::fetchvCenterNamesFromFileSystem(dir,vcenterSetInDir);
            //list of vcenters in the cluster, to which this node is getting added
            vector<string> vcentersInTheCluster =  pDiscoverLocalDiscoverMessage->m_datalist;

            std::set<string> ::iterator vcenterListInDirItr;
            for (vcenterListInDirItr = vcenterSetInDir.begin(); vcenterListInDirItr != vcenterSetInDir.end(); vcenterListInDirItr++) {

                string vcenterName = (*vcenterListInDirItr).c_str();

                if (std::find(vcentersInTheCluster.begin(), vcentersInTheCluster.end(), vcenterName) != vcentersInTheCluster.end())
                {
                    VIC_PLUG_LOG(VIC_TRACE_INFO, "Found vCenter during pre-nodeAdd %s \n", vcenterName.c_str());
                    // This is the case where we need ro run correlation only in
                    // LC(MC) mode
                }else{
                    //Delete those vCenters that are not going to part of the cluster
                    VIC_PLUG_LOG(VIC_TRACE_INFO, "Not found vCenter during pre-nodeAdd %s \n",
                                               vcenterName.c_str());
                    deleteVCenter(vcenterName);
                    FileUtils::deleteVcenterPersistentDBFiles(vcenterName);
                    FileUtils::deleteVcenterTemporaryDBFiles(vcenterName);
                }

            }

        }

        if ( (pDiscoverLocalDiscoverMessage->getCmdcode() == DISC_CMD_START_CORRELATION_ON_NODE_REJOIN ) && (FileUtils::isvCenterInManagementCluster()) && (isPrimaryLocation ==false))
            {
                 VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s", "Received Message: DISC_CMD_START_CORRELATION_ON_NODE_REJOIN");
                 isNodeRejoin = true;
                 VIC_PLUG_LOG(TRACE_LEVEL_INFO,"%s %s","Running NODE REJOIN RECON for vcenter:",vcenter.c_str());
                 dbInitialize(vcenter);
                 correlate(vcenter,isNodeRejoin); //true will trigger in rejoin mode
                 dbSync(vcenter);
            }

        if (pDiscoverLocalDiscoverMessage->getCmdcode()
                == DISC_CMD_START_CORELATION) {
            //Non primary nodes(MGMT cluster) ignore creation of START_CORRELATION.
            if ((FileUtils::isvCenterInManagementCluster())
                    && (isPrimaryLocation ==false)) {
                trace(
                        TRACE_LEVEL_INFO,
                        "No co-relation on secondary nodes in distribution Mode: ");

            }
            else {

                dbInitialize(vcenter);
                correlate(vcenter);
                dbSync(vcenter);
            }

        }
        if (pDiscoverLocalDiscoverMessage->getCmdcode()
                == DISC_CMD_START_RECONCILE) {
            //Non primary nodes(MGMT cluster) ignore creation of START_CORRELATION.
            if ((FileUtils::isvCenterInManagementCluster())
                    && (isPrimaryLocation ==false)) {
                trace(
                        TRACE_LEVEL_INFO,
                        "No co-relation on secondary nodes in distribution Mode: ");

            }
            else {

                dbInitialize(vcenter);
                //false tells reconcile to use db.new instead of dc.db
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Running reconcile for %s \n", vcenter.c_str());
                VcenterDiff::reconcile(vcenter,false);
                dbSync(vcenter);
            }
        }

        if (pDiscoverLocalDiscoverMessage->getCmdcode()
                == DISC_CMD_CORELATION_DELETE_VCENTER) {


            //primary Node (MGMT Cluster) and all other case deleteVcenter.
            if (FileUtils::isvCenterInManagementCluster()) {
                if (isPrimaryLocation) {
                    cleanupInterfaces(vcenter);
                    deleteVCenter(vcenter);
                }
                else {
                    trace(
                            TRACE_LEVEL_INFO,
                            "No deletion of vCenter secondary nodes in distribution Mode: ");
                }
            }
            else {
                cleanupInterfaces(vcenter);
                deleteVCenter(vcenter);
            }

            dbDelete(vcenter); 

        }
        trace(TRACE_LEVEL_INFO, "Completed Processing  Message: ");
        pDiscoverLocalDiscoverMessage->setCompletionStatus(
                WAVE_MESSAGE_SUCCESS);
        reply(pDiscoverLocalDiscoverMessage);

    }

    void DiscoverLocalMultiWorker::cleanupInterfaces(string vcenter)
    {

        string new_dbfile_r = "";
        vector<Domain*> db_vmnic;
        new_dbfile_r = FileUtils::getDBLocation(vcenter);
        tracePrintf(TRACE_LEVEL_INFO, "vcenter location is %s",new_dbfile_r.c_str());
        ValClientSynchronousConnection *pValClientSynchronousConnection;
        vector<Domain*>::iterator it;
        db_vmnic = getSource(VMNIC_DAO, new_dbfile_r);
        string mor = "";
        string host = "";

        pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();

        prismAssert(NULL != pValClientSynchronousConnection, __FILE__,
                __LINE__);

        for (it = db_vmnic.begin(); it <db_vmnic.end(); it++) {
            if((*it)->getIface().compare("")!=0)
            {
                tracePrintf(TRACE_LEVEL_INFO, "Interface FOUND: = %s",(*it)->getIface().c_str());
                pValClientSynchronousConnection->vnmAppmCdpNotify(1, (*it)->getIface(),mor,host);
            }

        }

    }

    void DiscoverLocalMultiWorker::dbInitialize(string vcenter) {

        string vcenterDBOld = FileUtils::getDBLocation(vcenter);

        if (!FileUtils::fileexists(vcenterDBOld.c_str())) {
            FileUtils::copyfile(DBFILE_ORIG, vcenterDBOld.c_str());
        }

    }

    void DiscoverLocalMultiWorker::dbDelete(string vcenter) {
        string vcenterDBNew = FileUtils::getDBNewLocation(vcenter);
        string vcenterDBOld = FileUtils::getDBLocation(vcenter);

        if (FileUtils::fileexists(vcenterDBOld.c_str())) {
            FileUtils::remove(vcenterDBOld.c_str());
        }

        if (FileUtils::fileexists(vcenterDBNew.c_str())) {
            FileUtils::remove(vcenterDBNew.c_str());
        }
        //MC Mode and principal node, delete also from /tmp/vCenter
        //ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        bool isPrimaryLocation = FrameworkToolKit::isPrimaryLocation();

        UI32 locationRole = FrameworkToolKit::getThisLocationRole();

        //IN single Node cluster(MC) locationRole is returned as LOCATION_STAND_ALONE;
        isPrimaryLocation= isPrimaryLocation||(locationRole==LOCATION_STAND_ALONE);

        if ((FileUtils::isvCenterInManagementCluster())
                && (isPrimaryLocation)) {
            vcenterDBNew = DB_NEW(vcenter);
            if (FileUtils::fileexists(vcenterDBNew.c_str())) {
                FileUtils::remove(vcenterDBNew.c_str());
            }
            vcenterDBOld = DB(vcenter);
            if (FileUtils::fileexists(vcenterDBOld.c_str())) {
                FileUtils::remove(vcenterDBOld.c_str());
            }

        }

    }

    void DiscoverLocalMultiWorker::correlate(string vcenter, bool isNodeRejoin) {
        string vcenterDBNew = FileUtils::getDBNewLocation(vcenter);
        string vcenterDBOld = FileUtils::getDBLocation(vcenter);

        DiscoveryCorrelator correlator(vcenter, vcenterDBOld, vcenterDBNew, isNodeRejoin);

        correlator.run(FULL_DISCOVERY);

    }
    void DiscoverLocalMultiWorker::deleteVCenter(string vcenter) {
        string vcenterDBNew = FileUtils::getDBNewLocation(vcenter);
        string vcenterDBOld = FileUtils::getDBLocation(vcenter);

        UI32 vccount = getVcCount();

        trace(TRACE_LEVEL_INFO, string("-------------DELETE-----  ")+vccount);
        DiscoveryCorrelator correlator(vcenter, vcenterDBOld, vcenterDBNew);

        correlator.deleteVCenter(vccount);

    }

    vector<Domain*> DiscoverLocalMultiWorker::getSource(daoType_t type, string db_file, string mor) {
        DAO* dao = DaoFactory::getDao(type, db_file);
        vector<Domain*> v = dao->retrieveInfoForDeletion(mor);
        DAO::closeDb();
        delete dao;
        return v;
    }

    UI32 DiscoverLocalMultiWorker::getVcCount() {

        VIC_PLUG_LOG(TRACE_LEVEL_DEBUG, "%s", "Get Vc Count ");

        UI32 numberOfResults = 0;
        querySynchronouslyForCount(VcenterManagedObject::getClassName(), numberOfResults);

        VIC_PLUG_LOG(TRACE_LEVEL_DEBUG, "%s %d", "Get Vc Count ", numberOfResults);
        return numberOfResults;
    }

    void DiscoverLocalMultiWorker::dbSync(string vcenter) {
        string vcenterDBNew = FileUtils::getDBNewLocation(vcenter);
        string vcenterDBOld = FileUtils::getDBLocation(vcenter);

        FileUtils::copyfile(vcenterDBNew.c_str(), vcenterDBOld.c_str());

    }

} /* namespace DcmNs */

