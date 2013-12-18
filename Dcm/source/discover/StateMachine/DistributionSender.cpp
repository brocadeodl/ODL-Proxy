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
 * DistributionSender.cpp
 *
 *  Created on: Jun 6, 2012
 *      Author: rmadapur
 */

#include "discover/StateMachine/DistributionSender.h"
#include "discover/Framework/discoverLocalMessages.h"
#include "File/Local/FileLocalTypes.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/User/DiscoverMultiWorker.h"
#include "discover/vsphere/Debug.h"
#include "discover/utils/FileUtils.h"

namespace DcmNs {

    DistributionSender* DistributionSender::instance = NULL;

    DistributionSender::DistributionSender() {
        instance = this;
        m_pSynchronizingMutex = new PrismMutex();

        m_pSynchronizingCondition = new PrismCondition(m_pSynchronizingMutex);

    }

    DistributionSender::~DistributionSender() {
        instance = NULL;
        delete (m_pSynchronizingMutex);
        delete (m_pSynchronizingCondition);
    }
    DistributionSender* DistributionSender::getInstance() {
        if (instance == NULL) {
            instance = new DistributionSender();
        }
        return instance;
    }

    ResourceId DistributionSender::publishEvent(ManagementInterfaceMessage *pMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        m_pSynchronizingMutex->lock();
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "Publishing Global event ");
        status = DiscoverMultiWorker::getInstance()->sendSynchronously(pMessage);
        m_pSynchronizingMutex->unlock();
        return status;
    }

    WaveMessageStatus DistributionSender::distribute(string vcenter,
            DISCOVERY_TYPE _discType,
            discoverVcenterCmdCode_t correlationType) {
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
        vector<LocationId> locationsVector;//Leave it empty as it will sent to all nodes
        //ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        switch (correlationType) {

            case DISC_CMD_START_CORELATION:
            case DISC_CMD_START_RECONCILE:
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Distribute DB.new to all local plugins for %s",
                        vcenter.c_str());
                m_pSynchronizingMutex->lock();
                if (distributeDiscoveredNewDBtoLocalPlugins(vcenter, _discType)
                        != WAVE_MESSAGE_SUCCESS) {
                    m_pSynchronizingMutex->unlock();
                    //TODO Return Error
                    return status;
                }
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Distribute Message for local plugins for %s",
                        vcenter.c_str());
                sendMessageToLocalPluginStep(vcenter, _discType,
                        correlationType,locationsVector);

               // m_pSynchronizingCondition->wait();

                if (FileUtils::isvCenterInManagementCluster()) {
                    //MC distribute the DB file
                    string db = FileUtils::getDBLocation(vcenter);
                    FrameworkToolKit::getConnectedLocations(locationsVector);
                    distributeDBtoLocalPlugins(db,locationsVector);
                }
                m_pSynchronizingMutex->unlock();
                break;

            case DISC_CMD_CORELATION_DELETE_VCENTER:
                m_pSynchronizingMutex->lock();
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "Delete vcenter Message for local plugins for %s",
                        vcenter.c_str());
                sendMessageToLocalPluginStep(vcenter, _discType,
                        correlationType,locationsVector);
                //m_pSynchronizingCondition->wait();
                m_pSynchronizingMutex->unlock();
                break;
            default:
                break;
        }

        return status;
    }

    WaveMessageStatus DistributionSender::distributevCenterNames(vector<string> vcenterList,
                    vector<LocationId> locationsVector){
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
                DiscoverDistributeMessage *m = new DiscoverDistributeMessage();

                m->setLocationIds(locationsVector);

                /* Read the data from received message and populate
                 * sending message
                 */
                //m->setVcenter(vcenter);
                m->setCmdcode(DISC_CMD_NODE_ADD_VCENTER_NAMES);
                m->setDataList(vcenterList);

                /* Send the message to Global Plugin */
                DiscoverMultiWorker::getInstance()->sendSynchronously(m);
                        delete m;

        return status;
    }

    WaveMessageStatus DistributionSender::nodeAddDistributionhandler(vector<string> vcenterList,
                vector<LocationId> locationsVector){
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                           "Node Add Distribution Hanlder");
        m_pSynchronizingMutex->lock();
        //Vcenter Names are sent to the adding node in the cluster
        //The adding node will use this list of vcenter names to find any old vcenter's configured
        // on the node and delete those.

            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "Distribute vcenter Names in Node Add Distribution Handler");

            distributevCenterNames(vcenterList,locationsVector);
            for(size_t i =0;i<vcenterList.size();i++){
            string vcenter = vcenterList.at(i);
            status = nodeAddDistribute(vcenter,locationsVector);
            if (status != WAVE_MESSAGE_SUCCESS) {
                            VIC_PLUG_LOG(
                                    VIC_TRACE_INFO,
                                    "Node add distribution failed for %s",
                                    vcenter.c_str());
                //continue with next vCenter,In case of failure
            }

        }

        m_pSynchronizingMutex->unlock();
        return status;
    }

    WaveMessageStatus DistributionSender::nodeAddDistribute(string vcenter,
            vector<LocationId> locationsVector) {
        //ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
        if (FileUtils::isvCenterInManagementCluster()) {
            //IN MC(LC) distribute the new DB to be correlated against
            string vcenterDB = FileUtils::getDBLocation(vcenter);
            string vcenterNewDB = FileUtils::getDBNewLocation(vcenter);

            VIC_PLUG_LOG(VIC_TRACE_INFO, "Distribute NEW DB for local plugins (nodeAdd) MC for %s",
                                vcenter.c_str());
            status = distributeDBtoLocalPlugins(vcenterDB,locationsVector,vcenterNewDB); // sending message with destination file path
            if (status != WAVE_MESSAGE_SUCCESS) {
                VIC_PLUG_LOG(
                        VIC_TRACE_INFO,
                        "Distribute DB for local plugins (nodeAdd) for %s failed",
                        vcenter.c_str());

                return status;
            }
            sendMessageToLocalPluginStep(vcenter,VNM_FULL_DISCOVERY,DISC_CMD_START_CORRELATION_ON_NODE_REJOIN,locationsVector);
        }
        else if(FileUtils::isvCenterInFabricCluster())
        {
            //FC Cluster
            string vcenterDB = FileUtils::getDBLocation(vcenter);
            string vcenterNewDB = FileUtils::getDBNewLocation(vcenter);
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Distribute DB.new to connected nodes (nodeAdd) FC for %s",
                    vcenter.c_str());

            status = distributeDBtoLocalPlugins(vcenterDB,locationsVector,vcenterNewDB); // sending message with destination file path

            if (status != WAVE_MESSAGE_SUCCESS) {
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Distribute DB for local plugins (nodeAdd) for %s failed", vcenter.c_str());
                return status;
            }
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                               "Send Reconcile message to connected nodes (nodeAdd) FC for %s",
                               vcenter.c_str());
            //Send Co-relation message to specified nodes
            sendMessageToLocalPluginStep(vcenter, VNM_FULL_DISCOVERY, DISC_CMD_START_RECONCILE,locationsVector);
            //m_pSynchronizingCondition->wait();
            return status;

        }

        return status;

    }

    WaveMessageStatus DistributionSender::distributeDBtoLocalPlugins(
            string dbfile, vector<LocationId> connectedLocationsVector, string destdbfile) {
        WaveMessageStatus status = WAVE_MESSAGE_SUCCESS;
        //Single Node , return success as we need to continue;
        if (connectedLocationsVector.empty()) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "LocationIds empty");
            return (status);
        }

     /*   //dbfile is pushed to all nodes
        FilePushFileMessage *pMessage = new FilePushFileMessage(dbfile, dbfile,
                FrameworkToolKit::getThisLocationId(),
                connectedLocationsVector);

        pMessage->setFileTransferFlag(FILE_OVERWRITE_DEST_FILE_IF_EXIST);
        status = DiscoverMultiWorker::getInstance()->sendToLocalPlugins(
                pMessage);
                */


        DistributeDBMessage *pMessage = new DistributeDBMessage();

        pMessage->setLocationIds(connectedLocationsVector);

        /* Read the data from received message and populate
         * sending message
         */
        pMessage->setDBfile(dbfile);
        pMessage->setDestDBfile("");

        if (destdbfile != "")
          pMessage->setDestDBfile(destdbfile);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "Sending Distribute DB Message to MultiWorker");
        /* Send the message to Global Plugin */
        status = DiscoverMultiWorker::getInstance()->sendSynchronously(pMessage);

        if (WAVE_MESSAGE_SUCCESS != status) {
            string errMsg = "Sending message to"
                    + PrismThread::getPrismServiceNameForServiceId(
                            pMessage->getSenderServiceCode())
                    + " service] failed, Error ["
                    + FrameworkToolKit::localize(status) + "]\n";
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s", errMsg.c_str());
            status = WAVE_MESSAGE_ERROR;
        }
        else {
            if (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus()) {
                string errMsg = "Response processing: CompletionStatus Reason "
                        + FrameworkToolKit::localize(
                                pMessage->getCompletionStatus());
                VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s", errMsg.c_str());
                status = WAVE_MESSAGE_ERROR;
            }
            else {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                        "Distributed to all nodes");
            }
        }
        delete pMessage;
        return status;
    }

    WaveMessageStatus DistributionSender::distributeDiscoveredNewDBtoLocalPlugins(
            string vcenter, DISCOVERY_TYPE _discType) {

        //ResourceId vcsMode = DcmToolKit::getThisVcsNodeClusterType();

        string vcenterOrigDB = DB_NEW(vcenter);
        string vcenterDestDB = FileUtils::getDBNewLocation(vcenter);
        if (FileUtils::isvCenterInManagementCluster()) {
            //On principal node do explicit copy to /var/tmp/vCenter
            FileUtils::copyfile(vcenterOrigDB.c_str(), vcenterDestDB.c_str());
        }


        //Distribute DB to all connected locations.
        vector<LocationId> connectedLocationsVector;
        FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
        return distributeDBtoLocalPlugins(vcenterDestDB,
                connectedLocationsVector);

    }

    void DistributionSender::sendMessageToLocalPluginStep(string vcenter,
            DISCOVERY_TYPE _discType,
            discoverVcenterCmdCode_t correlationType,vector<LocationId> locationsVector) {

        DiscoverDistributeMessage *m = new DiscoverDistributeMessage();

        m->setLocationIds(locationsVector);

        /* Read the data from received message and populate
         * sending message
         */
        m->setVcenter(vcenter);
        m->setCmdcode(correlationType);

        /* Send the message to Global Plugin */
        DiscoverMultiWorker::getInstance()->sendSynchronously(m);
		delete m;

    }


} /* namespace DcmNs */
