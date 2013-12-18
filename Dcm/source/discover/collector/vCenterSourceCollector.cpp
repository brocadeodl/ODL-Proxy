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
 * vCenterSourceCollector.cpp
 *
 *  Created on: Apr 24, 2012
 *      Author: rmadapur
 */

#include "discover/collector/vCenterSourceCollector.h"
#include "VirtualDb/Domain/DomainInclude.h"
#include "VirtualDb/DAO/DAOInclude.h"
#include "discover/vsphere/Debug.h"
#include "discover/StateMachine/DiscoveryStateMachineCollection.h"
#include "discover/StateMachine/DiscoveryStateMachine.h"
#include "discover/utils/StopWatch.h"
#include "discover/utils/FileUtils.h"
#include "discover/utils/URLEncode.h"
#include <curl/curl.h>
#include "discover/Framework/discoverManagementInterfaceMessages.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "vCenter/Framework/vCenterCrypto.h"

#include <raslog/raslog.h>
#include <raslog/raslog_vcenter.h>

#ifdef _X86_COMPILE_
#define DBFILE_ORIG     ((const char *) "/vobs/projects/springboard/fabos/src/vcs/dcm/source/VirtualDb/dc.db")
#else
#define DBFILE_ORIG     ((const char *) "/fabos/webtools/htdocs/ws/config/dc.db")
#endif
#define EVALUATE_DISCOVERY_STOP(sm,vcenter) if(sm->stopRequest){ \
    VIC_PLUG_LOG(VIC_TRACE_INFO, "Stop Request recieved for %s",\
            vcenter.c_str());\
    discoveryInProgress = false;\
    this->stop();\
    return;\
}

#define LOGIN_PHASE 1
#define PER_CALL_VM_SIZE 1000
namespace DcmNs {

    int vCenterInvalidResponseCount = 0;

    vCenterSourceCollector::vCenterSourceCollector( string _vcenter, string  _url, string _user, string _password,UI32 _ignoreValue, bool   _always,string _sm_uuid, DISCOVERY_TYPE _discType, DISCOVERY_STATE
            _nextSuccesState) : vcenter(_vcenter), url(_url), user(_user), password(_password),ignoreValue(_ignoreValue), always(_always),sm_uuid(_sm_uuid), discType(_discType), nextSuccesState(_nextSuccesState) {

    }

    vCenterSourceCollector::~vCenterSourceCollector() {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "Destructor for %s ", vcenter.c_str());
        this->vmDataCenterMap.clear();
        this->hostDataCenterMap.clear();
        this->networkDataCenterMap.clear();
        this->dvsDataCenterMap.clear();
        this->dvpgDataCenterMap.clear();
        this->dvpgidTodvpgNameMap.clear();
        this->dvsidTodvsNameMap.clear();
        this->nrpToQosMap.clear();
        this->spnicVector.clear();
        DiscoveryStateMachineCollection * _smlColl =
            DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);

        if ( (sm == NULL) || (sm->uuid != sm_uuid))
        {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s","sm is NULL - collector is already NULL or StateMachine IDs do not match");
            return;
        } else if(sm != NULL)
        {
            sm->collector = NULL;
        }
    }

    UI32 sendUpdateDiscoveryStatus(string vcenter, UI32 state, UI32 exitStatus)

    {
        DiscoverUpdateMessage *dmsg = new DiscoverUpdateMessage();
        int ret = WAVE_MESSAGE_SUCCESS;

        dmsg->setVcenter(vcenter);
        dmsg->setState(UI32(state));
        dmsg->setCmdcode(DISC_CMD_UPDATE_DISCOVER_STATUS);
        dmsg->setExitStatus(UI32(exitStatus));

        ret = WaveObjectManagerToolKit :: sendSynchronously(dmsg);
        delete dmsg;
        return ret;
    }

    void vCenterSourceCollector::cleanup() {
        if(stopTask){
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Cleaning up Discovery collector for %s ", vcenter.c_str());
            DiscoveryStateMachineCollection * _smlColl =
                DiscoverMultiWorker::getStateMachineCollection();
            DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);
            if(sm!=NULL){
                sm->trigger(SHUTDOWN, sm->vCenterdata);
            }
        }
        delete(this);

    }

    void vCenterSourceCollector::updatevCenterExitStatus(string _vcenter, int phase, int code) {

        discoverExitStatus_t exitStatus = DISC_EXIT_STATUS_SUCCESS;

        int ret;
        switch (code) {
            case VIC_WSCLIENT_CURL_CURLE_COULDNT_CONNECT:
                exitStatus = DISC_EXIT_STATUS_NO_VCENTER_SERVICE;
                break;
            case VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR:
                if (phase == LOGIN_PHASE)
                    exitStatus = DISC_EXIT_STATUS_BAD_CREDENTIAL;
                break;
            default:
                exitStatus = DISC_EXIT_STATUS_NO_VCENTER_SERVICE;
                break;
        }
        if (exitStatus != DISC_EXIT_STATUS_SUCCESS) {

            ret = sendUpdateDiscoveryStatus(_vcenter,DISC_STATE_END,exitStatus);
			if(ret == WAVE_MESSAGE_SUCCESS)
                raslog_ext(VCENTER_RASLOG_PARAMS, VC_1103,_vcenter.c_str(),url.c_str(),disc_exit_status2str(exitStatus));
        }
    }

    void cleanBasicDiscoveryDB() {

        HostDAO hostDao;
        VmnicDAO vmnicDao;
        VnicDAO vnicDao;
        VmDAO vmDao;
        VsDAO vsDao;
        PgDAO pgDao;
        PgHostVsDAO pgHostVsDAO;
        SpecialNicDAO specialNicDAO;
        SpecialPgDAO specialPgDAO;

        hostDao.deleteAllEntries();
        vmnicDao.deleteAllEntries();
        vnicDao.deleteAllEntries();
        vmDao.deleteAllEntries();
        vsDao.deleteAllEntries();
        pgDao.deleteAllEntries();
        pgHostVsDAO.deleteAllEntries();
        specialNicDAO.deleteAllEntries();
        specialPgDAO.deleteAllEntries();
        DAO::closeDb();

    }

    bool vCenterSourceCollector::ignoreDeleteAllResponse(string vcenterName )
    {
        bool isdeleteAll = false;
        if(isDeleteAllResponse(vcenterName,PG_DAO) == true)
            isdeleteAll = true;
        else if(isDeleteAllResponse(vcenterName,DVPG_DAO) == true)
            isdeleteAll = true;
        else if(isDeleteAllResponse(vcenterName,VM_DAO) == true)
            isdeleteAll = true; 

        VIC_PLUG_LOG(VIC_TRACE_DEBUG," ignoreValue = %d", ignoreValue);

        if (always == true)
        {
            vCenterInvalidResponseCount = 0;
            return false;
        }

        if((isdeleteAll) && ( vCenterInvalidResponseCount < (int)ignoreValue && ignoreValue != 0  ))
        {
            vCenterInvalidResponseCount++;
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Ignoring DELETE-ALL response %d", vCenterInvalidResponseCount); //Stop discovery and update status
            return true;
        }
        vCenterInvalidResponseCount = 0;

        return false;

    }

    vector<Domain*> vCenterSourceCollector::getSource(daoType_t type, string db_file, string mor) {
        DAO* dao = DaoFactory::getDao(type, db_file);
        vector<Domain*> v = dao->retrieveInfoForCorrelation(mor);
        DAO::closeDb();
        delete dao;
        return v;
    }


    void vCenterSourceCollector::releaseSource(vector<Domain*> v) {
        for (unsigned int i = 0; i < v.size(); i++ )
            delete v[i];
    }

    bool vCenterSourceCollector::isDeleteAllResponse(string vcenter_name, daoType_t type)
    {

        string vcenterDBNew = "";
        string vcenterDBOld = "";
        VIC_PLUG_LOG(VIC_TRACE_INFO," isDeleteAllResponse FOR = %s",vcenter_name.c_str());

        vcenterDBNew = FileUtils::getDBNewLocation(vcenter_name);
        vcenterDBOld = FileUtils::getDBLocation(vcenter_name);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"vcenterDBNew =%s",vcenterDBNew.c_str());
        VIC_PLUG_LOG(VIC_TRACE_INFO,"vcenterDBOld =%s",vcenterDBOld.c_str());

        vector<Domain*> v_new = getSource(type,vcenterDBNew);
        VIC_PLUG_LOG(VIC_TRACE_INFO," v_new.size() = %d",v_new.size());
        DAO::closeDb();
        vector<Domain*> v_old = getSource(type, vcenterDBOld);
        VIC_PLUG_LOG(VIC_TRACE_INFO," v_old.size() = %d",v_old.size());
        DAO::closeDb();

        if( v_old.size() != 0 && v_new.size() == 0)
        {
            //vCenterInvalidResponseCount++;
            VIC_PLUG_LOG(VIC_TRACE_INFO,"DELETE-ALL RESPONSE %d", vCenterInvalidResponseCount); //Stop discovery and update status
            releaseSource(v_old);
            releaseSource(v_new);
            return true;
        }
        else
        {
            releaseSource(v_old);
            releaseSource(v_new);
            return false;
        }

    }

    void vCenterSourceCollector::run() {
        DiscoveryStateMachineCollection * _smlColl =
            DiscoverMultiWorker::getStateMachineCollection();
        DiscoveryStateMachine *sm = _smlColl->findStateMachine(vcenter);
        if(sm==NULL){
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed Starting Discovery for %s:Could not find StateMachine",vcenter.c_str());
            return;
        }
        StopWatch discSw;
        sm->updateSuccess(DISC_IN_PRG);

        EVALUATE_DISCOVERY_STOP(sm, vcenter);

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery for %s , discoveryType = %s",
                vcenter.c_str(),
                DiscoveryStateMachine::DISCOVERY_TYPE_STRING[discType]);

        raslog_ext(VCENTER_RASLOG_PARAMS, VC_1100,
                DiscoveryStateMachine::DISCOVERY_TYPE_STRING[discType],
                vcenter.c_str(), url.c_str());
        discSw.start();
        ServiceInstance *serviceInstance = new ServiceInstance(url);
        auto_ptr<ServiceInstance> pautoServiceInstance(serviceInstance);
        int code = pautoServiceInstance->login(user, password);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            updatevCenterExitStatus(vcenter, LOGIN_PHASE, code);
            VIC_PLUG_LOG(VIC_TRACE_INFO, "Failed to execute Login Error Code: %d", code);
            goto DISC_ERROR;

        }

        intitializeEnvironMent();

        EVALUATE_DISCOVERY_STOP(sm, vcenter);
        discoveryInProgress = true;
        code  = retrieveDataCenter(serviceInstance);
        if (code != VIC_WSCLIENT_SUCCESS) {
            updatevCenterExitStatus(vcenter, LOGIN_PHASE, code);
            goto DISC_ERROR;
        }

        { //Full Discovery
            //Don't change the order
            EVALUATE_DISCOVERY_STOP(sm, vcenter);
            code  = retrieveHost(serviceInstance);
            if (code != VIC_WSCLIENT_SUCCESS) {
                updatevCenterExitStatus(vcenter, LOGIN_PHASE, code);
                goto DISC_ERROR;
            }
            EVALUATE_DISCOVERY_STOP(sm, vcenter);
            code  = retrieveDVPGS(serviceInstance);
            if (code != VIC_WSCLIENT_SUCCESS) {
                updatevCenterExitStatus(vcenter, LOGIN_PHASE, code);
                goto DISC_ERROR;
            }
            EVALUATE_DISCOVERY_STOP(sm, vcenter);
            code = retrieveDVS(serviceInstance);
            if (code != VIC_WSCLIENT_SUCCESS) {
                updatevCenterExitStatus(vcenter, LOGIN_PHASE, code);
                goto DISC_ERROR;
            }
            EVALUATE_DISCOVERY_STOP(sm, vcenter);
            code = retrieveVMS(serviceInstance);
            if (code != VIC_WSCLIENT_SUCCESS) {
                updatevCenterExitStatus(vcenter, LOGIN_PHASE, code);
                goto DISC_ERROR;
            }
            EVALUATE_DISCOVERY_STOP(sm, vcenter);
            populateDVPGwithDVSName();
            EVALUATE_DISCOVERY_STOP(sm, vcenter);
            populateSPNICwithDVPGName();
        }

        pautoServiceInstance->logout();
        discoveryInProgress = false;
        discSw.stop();

        //On Success
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Completed Discovery for vCenter %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(discSw.getElapsedTime()));

        if(ignoreDeleteAllResponse(vcenter))
        {

            sendUpdateDiscoveryStatus(sm->vCenterdata->vcenter,DISC_STATE_END,DISC_EXIT_STATUS_INVALID);
            sm->updateSuccess(DISC_DIST_IN_PRG);
            sm->trigger(DISTRIBUTION_COMPLETED, sm->vCenterdata);

            DAO::closeDb();
            return;
        }

        vCenterInvalidResponseCount = 0; //reset and let discovery proceed
        VIC_PLUG_LOG(VIC_TRACE_INFO,"PROCEEDING WITH DISCOVER %d",vCenterInvalidResponseCount);

        sendUpdateDiscoveryStatus(sm->vCenterdata->vcenter,DISC_STATE_DISCOVER_END,DISC_EXIT_STATUS_SUCCESS);

        EVALUATE_DISCOVERY_STOP(sm, vcenter);

        sm->updateSuccess(DISC_IN_PRG);
        sm->trigger(ASSET_COLLECTION_COMPLETED, sm->vCenterdata);
        DAO::closeDb();
        return;

DISC_ERROR:

        discoveryInProgress = false;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Discovery failed for vCenter %s. Re-schedule",
                vcenter.c_str());
        DAO::closeDb();

        //Call trriger to indicated Distribution is completed
        //so that Timer gets restarted
        sm->updateSuccess(DISC_IDLE);

        //On failure
        //TODO - re-schedule Discover
        //Re-schedule discovery as per timer
        return;

    }
    int vCenterSourceCollector::intitializeEnvironMent() {
        //Copy the empty DB, To Start Discovery
        string vcenterDB = DB_NEW(vcenter);

        if (!FileUtils::fileexists(VCENTER_DIR)) {
            FileUtils::mkDir(VCENTER_DIR);
        }

        //Full Discovery
        {
            FileUtils::copyfile(DBFILE_ORIG, vcenterDB.c_str());
        }

        return VIC_WSCLIENT_SUCCESS;
    }

    void vCenterSourceCollector::checkAndPrintError(int vcRes, Fault *fault) {
        if (vcRes == VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "Fault Error String %s",
                    fault->faultstring.c_str());
        }
    }

    int vCenterSourceCollector::retrieveHost(ServiceInstance *serviceInstance) {

        AssetBuilder *builder = new HostBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pautoBuilder(builder);


        AssetBuilder *networkBuilder = new NetworkBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pNetworkAutoBuilder(networkBuilder);


        HostDAO hostDao;
        VmnicDAO vmnicDao;
        VnicDAO vnicDao;
        VmDAO vmDao;
        VsDAO vsDao;
        PgDAO pgDao;
        PgHostVsDAO pgHostVsDAO;
        SpecialNicDAO specialNicDAO;
        SpecialPgDAO specialPgDAO;

        StopWatch hostsw;
        vector<Asset *> assetvector;
        vector<Asset *> newtworkAssetvector;
        map<string,Network *> networkNameNetworkIdMap;
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of Hosts for %s", vcenter.c_str());
        hostsw.start();
        int code = pautoBuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve Host Details for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of Networks for %s", vcenter.c_str());
        code = pNetworkAutoBuilder->getAsset(newtworkAssetvector);

        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve Network Details for %s",
                    vcenter.c_str());
            return code;

        }
        //Populate the Network(DC_ID+network-name map to network-id)
        for (size_t i = 0; i < newtworkAssetvector.size(); i++) {
            //            Network *network = (Network *) newtworkAssetvector.at(i);
            Network *network = static_cast<Network *>( newtworkAssetvector.at(i));

            string dc_id = "";
            map<string,string>::iterator itnetworkDataCenterMap;
            itnetworkDataCenterMap = this->networkDataCenterMap.find(network->id);
            if(itnetworkDataCenterMap!=networkDataCenterMap.end()){
                dc_id = (*itnetworkDataCenterMap).second;
            }else{
                VIC_PLUG_LOG(VIC_TRACE_ERROR,
                        "Failed to retrieve DataCenter Map %s for Network  for %s",
                        network->id.c_str(),vcenter.c_str());
            }

            string key = dc_id + "_" + network->name;
            networkNameNetworkIdMap[key] = network;

        }



        vector<Asset *>::iterator itAssetvector;
        StopWatch hostdbsw;
        hostdbsw.start();
        //Iterate the hosts
        for (itAssetvector = assetvector.begin();
                itAssetvector < assetvector.end(); itAssetvector++) {

            Host *host = static_cast<Host *>(*itAssetvector);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Host ID%s ",
                    host->id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Host  HostName %s",
                    host->hostName.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Host  DomainName %s",
                    host->domainName.c_str());

            /**** DB INSERT ****/
            //DB : (*id = id,name= hostName+domainName)
            string host_name;
            if(host->domainName.compare("")==0){ //domain is empty
                host_name = URLEncode::dbencode(host->hostName);
            }else{
                host_name = URLEncode::dbencode(host->hostName  +"." + host->domainName);
            }

            string dc_id = "";
            map<string, string>::iterator ithostDataCenterMap;
            ithostDataCenterMap = this->hostDataCenterMap.find(
                    host->id);
            if (ithostDataCenterMap != hostDataCenterMap.end()) {
                dc_id = (*ithostDataCenterMap).second;
            }
            else {
                VIC_PLUG_LOG(
                        VIC_TRACE_ERROR,
                        "Failed to retrieve DataCenter Map %s for Host  for %s",
                        host->id.c_str(), vcenter.c_str());
            }
            HostDomain hostDb(host->id, host_name, dc_id); //dc should be present
            hostDao.insertHost(hostDb);
            /**** DB INSERT END****/

            //prepare a temporary map of vmNics (key to object)
            map<string, VMNics *> vmNicsMap;
            vector<VMNics *>::iterator itvmnicsList;
            for (itvmnicsList = host->vmnicsList.begin();
                    itvmnicsList < host->vmnicsList.end(); itvmnicsList++) {
                VMNics *vmnics = static_cast<VMNics *>(*itvmnicsList);
                vmNicsMap[vmnics->key] = vmnics;
            }

            //Iterate through vswitches
            vector<VSwitch *>::iterator itvswitchList;
            for (itvswitchList = host->vswitchList.begin();
                    itvswitchList < host->vswitchList.end(); itvswitchList++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
                VSwitch *vswitch = static_cast<VSwitch *>(*itvswitchList);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VSwitch key %s",
                        vswitch->key.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VSwitch name %s",
                        vswitch->name.c_str());

                /**** DB INSERT ****/
                //DB(*id="vs-" + host-mor + key, name = name,host = host_name, host_id = host_mor)
                //Instead of id schema talks about name
                VsDomain vs(vswitch->name.c_str(), host->id.c_str(),
                        host_name,dc_id); // host should be present
                vsDao.insertVs(vs);
                /**** DB INSERT END ****/

                vector<string>::iterator itpnicList;
                for (itpnicList = vswitch->pnicList.begin();
                        itpnicList < vswitch->pnicList.end(); itpnicList++) {
                    string pnic = ((*itpnicList));
                    map<string, VMNics *>::iterator itvmNicsMap =
                        vmNicsMap.find(pnic);
                    VMNics *vmnics = (*itvmNicsMap).second;
                    vmnics->dvs = vswitch->key;
                    vmnics->dvs_nn = vswitch->name;
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                            "VSwitch pnic key %s", (*itpnicList).c_str());

                }
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
            }

            //Iterate through vmnics
            for (itvmnicsList = host->vmnicsList.begin();
                    itvmnicsList < host->vmnicsList.end(); itvmnicsList++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
                VMNics *vmnics = static_cast<VMNics *>(*itvmnicsList);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VMNics key %s",
                        vmnics->key.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VMNics device %s",
                        vmnics->device.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VMNics mac %s",
                        vmnics->mac.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VMNics dvs %s",
                        vmnics->dvs.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VMNics dvs_nn %s",
                        vmnics->dvs_nn.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");

                /**** DB INSERT ****/
                //DB:(*id="vmnic"+ host-mor + key, name = device, mac = mac, host_id = host_mor
                //,dvs=dvs,dvs_nn=dvs_nn);
                // in case of a vswitch both dvs id and name are the same
                VmnicDomain vmnic(vmnics->device, vmnics->mac, host->id,
                        vmnics->dvs_nn, vmnics->dvs_nn,dc_id); // host should be present
                vmnicDao.insertVmnic(vmnic);
                /**** DB INSERT END****/
            }

            //Iterate through PortGroups/network
            vector<PortGroup *>::iterator itportGroupList;
            for (itportGroupList = host->portGroupList.begin();
                    itportGroupList < host->portGroupList.end(); itportGroupList++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
                PortGroup *portGroup = static_cast<PortGroup *>(*itportGroupList);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network id %s",
                        portGroup->id.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network name %s",
                        portGroup->name.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network vlan %s",
                        portGroup->vlan.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network vs_nn %s",
                        portGroup->vs_nn.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network vs_id %s",
                        portGroup->vs_id.c_str());




                string key = dc_id + "_" + portGroup->name;

                map<string, Network *>::iterator itnetworkNameNetworkIdMap =
                    networkNameNetworkIdMap.find(key);
                if (itnetworkNameNetworkIdMap
                        != networkNameNetworkIdMap.end()) {

                    Network *network = (*itnetworkNameNetworkIdMap).second;

                    network->vlan = portGroup->vlan;

                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network id %s  %s %s",
                            network->id.c_str(),network->vlan.c_str(),network->name.c_str());

                    PgHostVsDomain pgHostVsDomain(
                            network->id, host->id,
                            host_name, portGroup->vs_nn, portGroup->vlan,dc_id);
                    pgHostVsDAO.insertPgHostVs(pgHostVsDomain);
                }else{
                    //Special PG

                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Special PG %s  %s %s",
                            portGroup->name.c_str(),portGroup->vlan.c_str(),portGroup->vs_nn.c_str());
                    SpecialPgDomain specialPg( portGroup->name, dc_id,host->id,
                            host_name, portGroup->vs_nn, portGroup->vlan);
                    specialPgDAO.insertSpecialPg(specialPg);
                }

                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");

            }





            //Iterate through VMList
            vector<VirtualMachine *>::iterator itvmList;
            for (itvmList = host->vmList.begin(); itvmList < host->vmList.end();
                    itvmList++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
                VirtualMachine *vm = static_cast<VirtualMachine *>(*itvmList);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM id %s",
                        vm->id.c_str());

                /**** DB INSERT ****/
                //DB(*id=id,host_id=host_mor,host_nn=host_name)
                //vm.name to filled during VM Discovery
                VmDomain vmDb(vm->id, "", host->id, host_name, dc_id);
                //dc and host should be present
                // VM name - Being updated when VMs are discovered

                vmDao.insertVm(vmDb);
                /**** DB INSERT ****/

                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
            }
            //Iterate through vnics
            vector<VNics *>::iterator itvnicsList;
            for (itvnicsList = host->vnicsList.begin();
                    itvnicsList < host->vnicsList.end(); itvnicsList++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************");
                VNics *vnics = static_cast<VNics *>(*itvnicsList);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics key %s",
                        vnics->key.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics ipaddress %s",
                        vnics->ipaddress.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics mac %s",
                        vnics->mac.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics dvpg_nn %s",
                        vnics->dvpg_nn.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics dvpg_id %s",
                        vnics->dvpg_id.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics vm_id %s",
                        vnics->vm_id.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s",
                        "************************************************");

                /**** DB INSERT ****/
                //DB(*id="vnic-" + mac , ip = ipaddress, mac = mac, vm_id = vm_id,dvpg_id= dvpg_id,dvpg_nn= dvpg_nn)
                if (vnics->vm_id.compare("VMKernel") == 0) {
                    //SpecialPgDomain specialPg(vnics->dvpg_nn, dc_id);
                    //specialPgDAO.insertSpecialPg(specialPg);
                    SpecialNicDomain specialNic(vnics->key,
                            vnics->mac, vnics->ipaddress, 
                            URLEncode::dbencode(vnics->dvpg_nn),
                            host->id, "", vnics->vm_id, dc_id, vnics->dvpg_id);

                    if (vnics->dvpg_nn == "" && vnics->dvpg_id !="") {
                        spnicVector.push_back(specialNic);
                    }
                    specialNicDAO.insertSpecialNic(specialNic);
                }


                /**** DB INSERT ****/

            }
        }
        networkNameNetworkIdMap.clear();

        for (size_t i = 0; i < newtworkAssetvector.size(); i++) {
            //Network *network = (Network *) newtworkAssetvector.at(i);
            Network *network = static_cast<Network *>( newtworkAssetvector.at(i));


            //Some networks, don't have host mappings.(and hence no vlans)
            if((network->type.compare("Network")==0)&&(network->vlan.compare("")!=0)){

                string network_dc_id = "";
                map<string, string>::iterator itnetworkDataCenterMap;
                itnetworkDataCenterMap = this->networkDataCenterMap.find(network->id);
                if (itnetworkDataCenterMap != networkDataCenterMap.end()) {
                    network_dc_id = (*itnetworkDataCenterMap).second;
                }
                else {
                    VIC_PLUG_LOG(
                            VIC_TRACE_ERROR,
                            "Failed to retrieve DataCenter Map %s for network  for %s",
                            network->id.c_str(), vcenter.c_str());
                }

                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network id %s %s %s",
                        network->id.c_str(),network->vlan.c_str(),network->name.c_str());
                PgDomain pgdDomain(network->id,
                        URLEncode::dbencode(network->name), network->vlan,
                        network_dc_id);

                pgDao.insertPg(pgdDomain);
            }

        }

        ListUtil<Asset>::clearPointersFromList(assetvector);
        ListUtil<Asset>::clearPointersFromList(newtworkAssetvector);
        hostsw.stop();
        hostdbsw.stop();
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Writing Hosts to DB for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(hostdbsw.getElapsedTime()));
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Completed Discovery of Hosts for %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(hostsw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;
    }

    int vCenterSourceCollector::retrieveDataCenter(
            ServiceInstance *serviceInstance) {

        DataCenterBuilder *builder = new DataCenterBuilder(serviceInstance);

        StopWatch dcsw;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of DataCenter for %s", vcenter.c_str());
        dcsw.start();

        string vcenterDB = DB_NEW(vcenter);
        // if (discType == VNM_BASIC_DISCOVERY) {
        //    vcenterDB = DB_MINI_NEW(vcenter);
        // }
        DataCenterDAO dcDao(vcenterDB);

        //Fetch DataCenter
        auto_ptr<DataCenterBuilder> pAutobuilder(builder);

        vector<Asset *> assetvector;

        int code = pAutobuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve DatCenterDetails for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        code = pAutobuilder->getDataCenterHostMap(assetvector,
                hostDataCenterMap);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve hostDataCenterMap for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        map<string, string>::iterator ithostDataCenterMap;
        for (ithostDataCenterMap = hostDataCenterMap.begin();
                ithostDataCenterMap != hostDataCenterMap.end();
                ithostDataCenterMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " HOST ID %s = DataCenter ID %s",
                    (*ithostDataCenterMap).first.c_str(),
                    (*ithostDataCenterMap).second.c_str());
        }
        code = pAutobuilder->getDataCenterVMMap(assetvector, vmDataCenterMap);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve vmDataCenterMap for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        map<string, string>::iterator itvmDataCenterMap;
        for (itvmDataCenterMap = vmDataCenterMap.begin();
                itvmDataCenterMap != vmDataCenterMap.end();
                itvmDataCenterMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " VM ID %s = DataCenter ID %s",
                    (*itvmDataCenterMap).first.c_str(),
                    (*itvmDataCenterMap).second.c_str());
        }

        code = pAutobuilder->getDataCenterNetworkMap(assetvector,
                networkDataCenterMap);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve networkDataCenterMap for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        map<string, string>::iterator itnetworkDataCenterMap;
        for (itnetworkDataCenterMap = networkDataCenterMap.begin();
                itnetworkDataCenterMap != networkDataCenterMap.end();
                itnetworkDataCenterMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " Network ID %s = DataCenter ID %s",
                    (*itnetworkDataCenterMap).first.c_str(),
                    (*itnetworkDataCenterMap).second.c_str());
        }
        code = pAutobuilder->getDataCenterDVSMap(assetvector, dvsDataCenterMap);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve dvsDataCenterMap for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        map<string, string>::iterator itdvsDataCenterMap;
        for (itdvsDataCenterMap = dvsDataCenterMap.begin();
                itdvsDataCenterMap != dvsDataCenterMap.end();
                itdvsDataCenterMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " DVS ID %s = DataCenter ID %s",
                    (*itdvsDataCenterMap).first.c_str(),
                    (*itdvsDataCenterMap).second.c_str());
        }
        code = pAutobuilder->getDataCenterDVPGMap(assetvector,
                dvpgDataCenterMap);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve dvpgDataCenterMap for %s",
                    vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        map<string, string>::iterator itdvpgDataCenterMap;
        for (itdvpgDataCenterMap = dvpgDataCenterMap.begin();
                itdvpgDataCenterMap != dvpgDataCenterMap.end();
                itdvpgDataCenterMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " DVPG ID %s = DataCenter ID %s",
                    (*itdvpgDataCenterMap).first.c_str(),
                    (*itdvpgDataCenterMap).second.c_str());
        }

        StopWatch dcdbsw;
        dcdbsw.start();
        for (size_t i = 0; i < assetvector.size(); i++) {
            DataCenter *dataCenter = (DataCenter *) assetvector.at(i);

            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Data Center ID %s ",
                    dataCenter->id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Data Center Name %s",
                    dataCenter->name.c_str());

            /**** DB INSERT ****/
            DataCenterDomain dc(dataCenter->id, URLEncode::dbencode(dataCenter->name));
            dcDao.insertDatacenter(dc);
            /**** DB INSERT END****/
        }

        //cleanup

        ListUtil<Asset>::clearPointersFromList(assetvector);
        dcsw.stop();
        dcdbsw.stop();
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Writing DataCenters to DB for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(dcdbsw.getElapsedTime()));
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Completed Discovery of DataCenters for %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(dcsw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;

    }

    int vCenterSourceCollector::retrieveDVS(ServiceInstance *serviceInstance) {

        AssetBuilder *builder = new DVSBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pAutobuilder(builder);
        vector<Asset *> assetvector;

        StopWatch sw;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of DVS for %s", vcenter.c_str());
        sw.start();
        int code = pAutobuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve DVS for %s", vcenter.c_str());
            DAO::closeDb();
            return code;

        }

        StopWatch dbsw;
        dbsw.start();
        DvsDAO dvsDao;
        VmnicDAO vmnicDao;


        for (size_t i = 0; i < assetvector.size(); i++) {
            DVS *dvs = (DVS *) assetvector.at(i);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvs ID %s ",
                    dvs->id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvs  Name %s",
                    dvs->name.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvs  prgroup_nn %s",
                    dvs->prgroup_nn.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvs  prgroup_id %s",
                    dvs->prgroup_id.c_str());


            nrpToQosMap.insert(dvs->nrp_qosList.begin(),
                    dvs->nrp_qosList.end());

            this->dvsidTodvsNameMap[dvs->id] = dvs->name;
            string dvpg_nn = dvs->prgroup_nn;

            map<string, string>::iterator itdvpgidTodvpgNameMap =
                this->dvpgidTodvpgNameMap.find(dvs->prgroup_id);
            if (itdvpgidTodvpgNameMap != dvpgidTodvpgNameMap.end()) {

                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvpg nn %s",
                        (*itdvpgidTodvpgNameMap).second.c_str());
                dvpg_nn = (*itdvpgidTodvpgNameMap).second;
            }

            /**** DB INSERT ****/
            //DB(*id=id,name=name,prgroup_id = prgroup_id)
            //Fetch the prgroup_nn from DVPortGroup using prgroup_id
            string dvs_dc_id = "";
            map<string, string>::iterator itdvsDataCenterMap;
            itdvsDataCenterMap = this->dvsDataCenterMap.find(
                    dvs->id);
            if (itdvsDataCenterMap != dvsDataCenterMap.end()) {
                dvs_dc_id = (*itdvsDataCenterMap).second;
            }
            else {
                VIC_PLUG_LOG(
                        VIC_TRACE_ERROR,
                        "Failed to retrieve DataCenter Map %s for dvs  for %s",
                        dvs->id.c_str(), vcenter.c_str());
            }
            DvsDomain dvsDb(dvs->id, URLEncode::dbencode(dvs->name), dvs->prgroup_id, URLEncode::dbencode(dvpg_nn),
                    dvs_dc_id); // dc should be present

            dvsDao.insertDvs(dvsDb);
            /**** DB INSERT END****/
            vector<string>::iterator ithost_mors;
            for (ithost_mors = dvs->host_mors.begin();
                    ithost_mors != dvs->host_mors.end(); ithost_mors++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVS HOST ID %s",
                        (*ithost_mors).c_str());
            }

            multimap<string, string>::iterator ithost_pnics;
            for (ithost_pnics = dvs->host_pnicList.begin();
                    ithost_pnics != dvs->host_pnicList.end(); ithost_pnics++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "DV HOST ID %s = DVS PNIC ID %s",
                        (*ithost_pnics).first.c_str(),
                        (*ithost_pnics).second.c_str());
                VmnicDomain vmnic;
                vmnic.setName((*ithost_pnics).second);
                vmnic.setHostId((*ithost_pnics).first);
                vmnic.setDvs(dvs->id);
                vmnic.setDvsNN(URLEncode::dbencode(dvs->name));
                vmnicDao.updateVmnicDvs(vmnic);

            }

        }
        ListUtil<Asset>::clearPointersFromList(assetvector);
        sw.stop();
        dbsw.stop();
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Writing DVS to DB for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(dbsw.getElapsedTime()));
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Completed Discovery of DVS for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(sw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;
    }
    int vCenterSourceCollector::retrieveDVPGS(
            ServiceInstance *serviceInstance) {

        DVPGBuilder *builder = new DVPGBuilder(serviceInstance);
        auto_ptr<DVPGBuilder> pAutobuilder(builder);
        vector<Asset *> assetvector;
        StopWatch sw;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of DVPGS for %s", vcenter.c_str());
        sw.start();

        DvpgDAO dvpgDao;

        int code = pAutobuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {

            checkAndPrintError(code, serviceInstance->getLastFault());
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve DVPGS for %s", vcenter.c_str());
            DAO::closeDb();
            return code;

        }
        StopWatch dbsw;
        dbsw.start();

        for (size_t i = 0; i < assetvector.size(); i++) {
            DVPG *dvpg = (DVPG *) assetvector.at(i);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG ID %s ",
                    dvpg->id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG  Name %s",
                    dvpg->name.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG  vlan %s",
                    dvpg->vlan.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG  dvs_id %s",
                    dvpg->dvs_id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG  uplinktype %s",
                    dvpg->uplinktype.c_str());

            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    "Data Center Network Resource Pool Key %s",
                    dvpg->networkResourcePoolKey.c_str());

            this->dvpgidTodvpgNameMap[dvpg->id] = dvpg->name;

            string qos = "";
            /**** DB INSERT ****/
            //DB(*id=id,name = name, vlan = vlan.dvs_id=dvs_id, uplinktype=uplink_type)
            //dvs_nn to be fetched from DVS using dvs_id

            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG  Name %s",
                    URLEncode::dbencode(dvpg->name).c_str());
            string dvpg_dc_id = "";
            map<string, string>::iterator itdvpgDataCenterMap;
            itdvpgDataCenterMap = this->dvpgDataCenterMap.find(dvpg->id);
            if (itdvpgDataCenterMap != dvpgDataCenterMap.end()) {
                dvpg_dc_id = (*itdvpgDataCenterMap).second;
            }
            else {
                VIC_PLUG_LOG(
                        VIC_TRACE_ERROR,
                        "Failed to retrieve DataCenter Map %s for dvpg  for %s",
                        dvpg->id.c_str(), vcenter.c_str());
            }
            DvpgDomain dvpgDb(dvpg->id, URLEncode::dbencode(dvpg->name), dvpg->vlan, qos,
                    dvpg->dvs_id, "",
                    dvpg_dc_id,
                    dvpg->networkResourcePoolKey,dvpg->uplinktype); // dc and dvs shouldbe present
            dvpgDao.insertDvpg(dvpgDb);

            // dvs_nn - TBD -AJ - need API to query DVS name based on DVS Id
            /**** DB INSERT END****/

        }
        ListUtil<Asset>::clearPointersFromList(assetvector);
        sw.stop();
        dbsw.stop();
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Number of DVPG Maps %d",
                dvpgidTodvpgNameMap.size());

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Writing DVPGS to DB for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(dbsw.getElapsedTime()));
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Completed Discovery of DVPGS for %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(sw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;
    }
    int vCenterSourceCollector::retrieveVMS(ServiceInstance *serviceInstance) {

        VMBuilder *builder = new VMBuilder(serviceInstance);
        auto_ptr<VMBuilder> pAutobuilder(builder);
        VmDAO vmDao;
        VnicDAO vnicDao;

        StopWatch sw;
        StopWatch dbsw;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of VMS for %s", vcenter.c_str());
        sw.start();
        dbsw.start();

        /* int code = pAutobuilder->getAsset(assetvector);
           if (code != VIC_WSCLIENT_SUCCESS) {

           checkAndPrintError(code, serviceInstance->getLastFault());
           VIC_PLUG_LOG(VIC_TRACE_INFO,
           "Failed to retrieve VmDetails for %s", vcenter.c_str());
           DAO::closeDb();
           return code;

           }*/

        //        map<string, string>::iterator itvmDataCenterMap;
        //        for (itvmDataCenterMap = vmDataCenterMap.begin();
        //                itvmDataCenterMap != vmDataCenterMap.end();
        //                itvmDataCenterMap++) {
        //
        //            Asset *asset = NULL;
        //            builder->getAsset((*itvmDataCenterMap).first.c_str(), &asset);
        //            if (asset == NULL)
        //                continue;
        //            VM *vm = (VM *) asset;

        size_t num =0;
        size_t count =0;
        int code = WAVE_MESSAGE_ERROR;
        vector<vector <string> > listMorList;
        vector <string> morList;
        size_t vmsize = vmDataCenterMap.size();

        map<string, string>::iterator itvmDataCenterMap1;
        for (itvmDataCenterMap1 = vmDataCenterMap.begin();itvmDataCenterMap1 != vmDataCenterMap.end();itvmDataCenterMap1++)
        {
            printf(" VM ID %s = DataCenter ID %s\n", (*itvmDataCenterMap1).first.c_str(), (*itvmDataCenterMap1).second.c_str());
            morList.push_back((*itvmDataCenterMap1).first);
            num++;
            count++;
            if (num == PER_CALL_VM_SIZE || count == vmsize)
            {
                num = 0;
                listMorList.push_back(morList);
                morList.clear();
            }
        }

        for(size_t t = 0; t < listMorList.size(); t++)
        {
            vector<Asset *> assetvector;
            vector<string> inner = listMorList.at(t);

            if(inner.size() != 0)
                code = builder->getAsset(inner,assetvector);
            else
                continue;
            if (code != VIC_WSCLIENT_SUCCESS) {

                checkAndPrintError(code, serviceInstance->getLastFault());
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Failed to retrieve Host Details for %s", vcenter.c_str());
                DAO::closeDb();
                return code;
            }        
            for (size_t i = 0; i < assetvector.size(); i++) {
                VM *vm = (VM *) assetvector.at(i);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM ID %s ",
                        vm->id.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM  Name %s",
                        vm->name.c_str());
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM  isTemplate %d",
                        vm->isTemplate);
                map<string, string>::iterator itvmDataCenterMap;
                itvmDataCenterMap = this->vmDataCenterMap.find(vm->id);
                if (itvmDataCenterMap == vmDataCenterMap.end()) {
                    //TODO Fix issues with VM's created under Folders
                    continue;
                }
                string dc_id = (*itvmDataCenterMap).second.c_str();

                /**** DB INSERT ****/
                //Inserting the VM name here
                VmDomain vmDb; //(vm->id.c_str()
                vmDb.setId(vm->id.c_str());
                vmDb.setName(URLEncode::dbencode(vm->name).c_str());
                vmDao.updateVmName(vmDb);

                /**** DB INSERT END****/

                //Delete row from VM Table if isTemplate
                //(DB:update(id=id,name= name);
                // Need APIs to delete a row from VM table
                //Fetch the host-mor fromVM table
                //Iterate through vnics
                vector<VNics *>::iterator itvnicsList;
                for (itvnicsList = vm->vnicsList.begin();
                        itvnicsList < vm->vnicsList.end(); itvnicsList++) {
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                            "************************************************");
                    VNics *vnics = static_cast<VNics *>(*itvnicsList);
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics NAME %s",
                            vnics->name.c_str());
                    string ip = "";
                    map<string, string>::iterator itipMacMap = vm->ipMacMap.find(
                            vnics->mac);
                    if (itipMacMap != vm->ipMacMap.end()) {
                        ip = (*itipMacMap).second;

                    }

                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics ipaddress %s",
                            ip.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics mac %s",
                            vnics->mac.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics dvpg_nn %s",
                            vnics->dvpg_nn.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics dvpg_id %s",
                            vnics->dvpg_id.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics vm_id %s",
                            vnics->vm_id.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                            "************************************************");

                    string dvpg_nn = vnics->dvpg_nn;

                    map<string, string>::iterator itdvpgidTodvpgNameMap =
                        this->dvpgidTodvpgNameMap.find(vnics->dvpg_id);
                    if (itdvpgidTodvpgNameMap != dvpgidTodvpgNameMap.end()) {

                        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvpg nn %s",
                                (*itdvpgidTodvpgNameMap).second.c_str());
                        dvpg_nn = (*itdvpgidTodvpgNameMap).second;
                    }

                    VnicDomain vnic(vnics->name, vnics->mac, ip,
                            vnics->dvpg_id, URLEncode::dbencode(dvpg_nn),
                            vnics->vm_id, "dvs-1", dc_id);

                    vnicDao.insertVnic(vnic);

                }

            }
            ListUtil<Asset>::clearPointersFromList(assetvector);
            inner.clear();
        }
        sw.stop();
        dbsw.stop();
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Writing VM to DB for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(dbsw.getElapsedTime()));
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Completed Discovery of VM for %s in " TIME_FORMAT " seconds", vcenter.c_str(),
                TIME_PARAM(sw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;
    }

    int vCenterSourceCollector::fetchDVPGForBasicDsicovery() {
        DvpgDAO dvpgDao;
        StopWatch sw;
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Fetching DVPG (Basic Discovery) for %s", vcenter.c_str());
        sw.start();
        vector<DvpgDomain> dvPGVector = dvpgDao.retrieveDvpgInfoForDiscovery();
        for (size_t i = 0; i < dvPGVector.size(); i++) {
            DvpgDomain dvpg = dvPGVector.at(i);

            dvpgidTodvpgNameMap[dvpg.getId()] = dvpg.getName();

        }
        sw.stop();

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "population of DVPG with DVS name %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(sw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;

    }

    int vCenterSourceCollector::populateDVPGwithDVSName() {
        DvpgDAO dvpgDao;

        StopWatch sw;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting population of DVPG with DVS name for %s",
                vcenter.c_str());
        sw.start();
        vector<DvpgDomain> dvPGVector = dvpgDao.retrieveDvpgInfoForDiscovery();
        for (size_t i = 0; i < dvPGVector.size(); i++) {
            DvpgDomain dvpg = dvPGVector.at(i);

            string dvs_nn = "unknown";

            map<string, string>::iterator itdvsidTodvpgNameMap =
                this->dvsidTodvsNameMap.find(dvpg.getDvsId());
            if (itdvsidTodvpgNameMap != dvsidTodvsNameMap.end()) {

                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvs nn %s",
                        (*itdvsidTodvpgNameMap).second.c_str());
                dvs_nn = (*itdvsidTodvpgNameMap).second;
                dvpg.setDvsNN(URLEncode::dbencode(dvs_nn));
            }

            map<string, string>::iterator itnrpToQosMap = nrpToQosMap.find(
                    dvpg.getNetworkResourcePoolKey());
            if (itnrpToQosMap != nrpToQosMap.end()) {
                string val = (*itnrpToQosMap).second;
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, " DVPG %s , QOs  %s ",
                        dvpg.getName().c_str(), val.c_str());
                dvpg.setQos(val);
            }

            dvpgDao.updateDvsName(dvpg);
            dvpgDao.updateDvpgQos(dvpg);

        }
        sw.stop();

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "population of DVPG with DVS name %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(sw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;

    }

    int vCenterSourceCollector::populateSPNICwithDVPGName() {

        StopWatch sw;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting population of SPNIC with DVS name for %s",
                vcenter.c_str());
        sw.start();

        SpecialNicDAO dao;
        for (size_t i = 0; i < spnicVector.size(); i++) {
            SpecialNicDomain dom = spnicVector.at(i);
            VIC_PLUG_LOG(VIC_TRACE_INFO, " Mac %s ", dom.getMac().c_str());
            VIC_PLUG_LOG(VIC_TRACE_INFO, "PG Name %s", dom.getPgNN().c_str());
            VIC_PLUG_LOG(VIC_TRACE_INFO, "DVPG ID %s", dom.getDvpgId().c_str());


            string dvpg_nn = "";

            map<string, string>::iterator itdvpgidTodvpgNameMap =
                this->dvpgidTodvpgNameMap.find(dom.getDvpgId());
            if (itdvpgidTodvpgNameMap != dvpgidTodvpgNameMap.end()) {

                VIC_PLUG_LOG(VIC_TRACE_INFO, "dvpg nn %s",
                        (*itdvpgidTodvpgNameMap).second.c_str());
                dvpg_nn = (*itdvpgidTodvpgNameMap).second;
            }

            dom.setPgNN(dvpg_nn);
            dao.updateSpecialNic(dom);
        }

        sw.stop();

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "population of SPNIC with DVPG name %s in " TIME_FORMAT " seconds",
                vcenter.c_str(), TIME_PARAM(sw.getElapsedTime()));
        DAO::closeDb();
        return VIC_WSCLIENT_SUCCESS;

    }

    } /* namespace DcmNs */
