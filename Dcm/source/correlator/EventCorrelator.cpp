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
 * EventCorrelator.cpp
 *
 *  Created on: August 29, 2012
 *      Author: nfarhi
 */


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "correlator/EventCorrelator.h"
#include "discover/Framework/discoverTypes.h"
#include "discover/StateMachine/DistributionSender.h"
#include "discover/events/vCenterEventsMessage.h"

namespace DcmNs {
    EventCorrelator::EventCorrelator() {}
    EventCorrelator::EventCorrelator(string dbfile, Host* host_, string dc_id, string vc) {
        dbfile_r = dbfile;
        host = host_;
        dc_mor = dc_id;
        vcenter = vc;
    }

    EventCorrelator::EventCorrelator(string dbfile, string dc_id, string vc) {
        dbfile_r = dbfile;
        dc_mor = dc_id;
        vcenter = vc;
        host = NULL;
    }

    void EventCorrelator::correlateVMReconf(Host* host_, class VM* vm_) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", __FUNCTION__);
        host = host_;
        vm = vm_;

        vector<Domain*> vector_old, vector_new, deleted, added, updated;
        // retrieve info from DB
        string vm_mor = vm->id;
        string host_mor = host->id;        
        string host_name = getHostName();
        vector<Asset*> assets;
        assets = Asset::convertToVectorAsset<VNics*>(vm->vnicsList);
        Asset::printAssetVector(assets);             
        vector_new = getSource(assets, host_mor, host_name, dc_mor);

        vector_old = getSource(VNIC_DAO, dbfile_r, vm_mor);

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%d",
                "------vector_new -----------  ",vector_new.size());

        for (size_t s = 0; s < vector_new.size(); s++) {
            Domain * vnic1 = vector_new.at(s);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "---vmid(%s):mac(%s):dvpg(%s):name(%s) ", vnic1->getVmId().c_str(),vnic1->getMac().c_str(), vnic1->getDvpgNN().c_str(), vnic1->getName().c_str());                 
        }

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s:%d",
                "------vector_old -----------  ",vector_old.size());

        for (size_t s = 0; s < vector_old.size(); s++) {
            Domain * vnic1 = vector_old.at(s);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "---vmid(%s):mac(%s):dvpg(%s):name(%s) ", vnic1->getVmId().c_str(),vnic1->getMac().c_str(), vnic1->getDvpgNN().c_str(), vnic1->getName().c_str());                 
        }



        // retrieve info from vcenter
        deleted = diff(vector_old, vector_new);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "------deleted vector ----------- %d ", deleted.size());



        added = diff(vector_new, vector_old);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "------added vector ----------- %d ", added.size());

        findUpdatedConfig(vector_old, vector_new, added, deleted, updated);
        //          VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
        //                  "------updated vector ----------- %d ", updated.size());

        //printDomainChanges(vector_new, vector_old, updated);

        sendDbConfigUpdate(deleted, VNM_DELETE);
        sendDbConfigUpdate(added, VNM_CREATE);
        sendDbConfigUpdate(updated, VNM_UPDATE);

        releaseSource(vector_old);
        releaseSource(vector_new);
        //        releaseSource(deleted);
        //      releaseSource(added);
        //    releaseSource(updated);

    }

    vector<Asset *>  EventCorrelator::extractSpecialPortGroups(){
        vector<Asset*> result;
        vector<Network *>::iterator itrnetworkList;

        vector<string> networkNameList;
        for (itrnetworkList = host->nwList.begin();
                itrnetworkList < host->nwList.end(); itrnetworkList++) {
            networkNameList.push_back((*itrnetworkList)->name);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network Name %s\n",
                    (*itrnetworkList)->name.c_str());

        }

        vector<PortGroup *>::iterator itportGroupList;
        for (itportGroupList = host->portGroupList.begin();
                itportGroupList < host->portGroupList.end();
                itportGroupList++) {
            VIC_PLUG_LOG( VIC_TRACE_DEBUG, "%s",
                    "************************************************\n");
            PortGroup *portGroup = static_cast<PortGroup *>(*itportGroupList);

            if (std::find(networkNameList.begin(), networkNameList.end(),
                        portGroup->name) != networkNameList.end()) {
                //found
            }
            else {
                if(portGroup->name.compare("")!=0){
                    portGroup->type_internal = SP_PORT_GROUP;

                    //Indicates its a Sepcial PG
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network id %s\n",
                            portGroup->id.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network name %s\n",
                            portGroup->name.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network vlan %s\n",
                            portGroup->vlan.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network vs_nn %s\n",
                            portGroup->vs_nn.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Network vs_id %s\n",
                            portGroup->vs_id.c_str());
                    result.push_back(portGroup);
                }
            }

        }
        return result;

    }

    void EventCorrelator::findSendTaskUpdates(correlateOperation_t op, correlateObject_t obj) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", __FUNCTION__ );
        vector<Asset*> assets;
        daoType_t dao;
        switch (obj) {
            case VSWITCH:
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",  "vswitch");
                assets = Asset::convertToVectorAsset<VSwitch*>(host->vswitchList);
                dao = VS_DAO;
                break;
            case VMKERNEL:
            case CONSOLEVNIC:
                assets = Asset::convertToVectorAsset<VNics*>(host->vnicsList);
                dao = SPECIALNIC_DAO;
                break;
            case VMNIC:
                assets = Asset::convertToVectorAsset<VMNics*>(host->vmnicsList);
                dao = VMNIC_DAO;
                break;
            case PORTGROUP:
                assets = Asset::convertToVectorAsset<Network*>(host->nwList);
                dao = PGHOSTVS_DAO;
                break;
            default:
                assets = Asset::convertToVectorAsset<VSwitch*>(host->vswitchList);
                dao = VS_DAO;
        }

        switch (op) {
            case EVENT_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "delete");
                findAssetUpdates(assets, op, dao); 

                if(obj == PORTGROUP){
                    //Special PortGroups are special type of port-groups
                    assets =extractSpecialPortGroups();
                    dao = SPECIALPG_DAO;
                    findAssetUpdates(assets, op, dao);
                }

                break;
            case EVENT_INSERT:
            case EVENT_UPDATE:
                //vswitches
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "findAssetUpdates VSS");
                findAssetUpdates(assets, op, dao);

                //special port groups
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "findAssetUpdates Special Port Groups");

                assets =extractSpecialPortGroups();
                dao = SPECIALPG_DAO;
                findAssetUpdates(assets, op, dao);

                //special nics
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "findAssetUpdates SPECIALNIC_DAO");
                assets = Asset::convertToVectorAsset<VNics*>(host->vnicsList);
                dao = SPECIALNIC_DAO;
                findAssetUpdates(assets, op, dao);
                //portgroups
                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "findAssetUpdates PGHOSTVS_DAO");
                assets = Asset::convertToVectorAsset<Network*>(host->nwList);
                dao = PGHOSTVS_DAO;
                findAssetUpdates(assets, op, dao);

                VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "findAssetUpdates VMNIC_DAO");
                assets = Asset::convertToVectorAsset<VMNics*>(host->vmnicsList);
                dao = VMNIC_DAO;
                findAssetUpdates(assets, op, dao);
                break;
            default:
                //error
                break;
        }
    }


    void EventCorrelator::findAssetUpdates(vector <Asset *> assets, correlateOperation_t op, daoType_t dao) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","find asset updates");
        vector<Domain*> old_v, new_v, added, deleted, updated;
        // find added, deleted, updated Domains
        //Asset::printAssetVector(assets);

        findAddDeleteUpdates(dao, assets, op, old_v, new_v, deleted, added, updated);
        //printDomainChanges(added, deleted, updated);
        // send event messages to local plugins

        // find dependent Domains on added Domains. e.g. vmnics on a added vswitch
        vector<Domain*> dependentUpdate;
        vector<Domain*>::iterator it = added.begin();
        while (it != added.end()) {
            (*it)->findAssetDependentUpdates(host, dependentUpdate);
            it++;
        }
        if (deleted.size() >0)
            VIC_PLUG_LOG(VIC_TRACE_INFO,"DELETED %d",deleted.size());
        sendDbConfigUpdate(deleted, VNM_DELETE);
        if (added.size() >0)
            VIC_PLUG_LOG(VIC_TRACE_INFO,"CREATE %d",added.size());
        sendDbConfigUpdate(added, VNM_CREATE);
        if (updated.size() >0)
            VIC_PLUG_LOG(VIC_TRACE_INFO,"UPDATED %d",updated.size());
        sendDbConfigUpdate(updated, VNM_UPDATE);
        if (dependentUpdate.size() >0)
            VIC_PLUG_LOG(VIC_TRACE_INFO,"dependentUpdate %d",dependentUpdate.size());
        sendDbConfigUpdate(dependentUpdate, VNM_UPDATE);
        releaseSource(old_v);
        releaseSource(new_v);
        releaseSource(dependentUpdate);
    }

    // finds added, deleted, updated domain by comparing existing db and the info from vcenter
    void EventCorrelator::findAddDeleteUpdates(daoType_t type, vector<Asset*> assets, correlateOperation_t op, vector<Domain*>& vector_old, vector<Domain*>& vector_new, vector<Domain*>& deleted, vector<Domain*>&  added, vector<Domain*>& updated) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","adddeleteupdates");

        string host_mor = "";
        string host_name = getHostName();
        if(host != NULL) {
            host_mor = host->id;
            //host_name = host->hostName;
        }
        // retrieve info from DB
        vector_old = getSource(type, dbfile_r, host_mor);
        // retrieve info from vcenter
        vector_new = getSource(assets, host_mor, host_name, dc_mor);

        if (op == EVENT_DELETE) {
            // find A - B on id: deleted Domains
            deleted = diff(vector_old, vector_new);
        } else if (op == EVENT_INSERT ) {
            // find B - A on id: added Domains
            added = diff(vector_new, vector_old);
        } else if (op == EVENT_UPDATE) {
            // find A intersection B: C
            // find B intersection A: D
            // and then check for Domain field changes
            findUpdatedConfig(vector_old, vector_new, added, deleted, updated);
        }
    }

    // send and event message to local plugin
    void EventCorrelator::sendDbConfigUpdate(vector<Domain*> v, UI32 op) {
        vector<Domain*>::iterator it = v.begin();
        DistributionSender *ds = DistributionSender::getInstance();
        while (it != v.end()) {
            EventMessage* pMessage; 
            (*it)->convertToEventMessage(&pMessage);
            pMessage->m_opcode = op;
            pMessage->m_vcenter = vcenter;
            ds->publishEvent(pMessage);
            it++;
            delete pMessage;
        }
    }

    string EventCorrelator::getHostName () {

        string host_name = "";
        if(host == NULL) {
            return host_name;
        }
        host_name = host->hostName;
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Host ID%s \n",
                host->id.c_str());
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Host  HostName %s\n",
                host->hostName.c_str());
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Host  DomainName %s\n",
                host->domainName.c_str());

        host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);
        return host_name;

    }
} /* namespace DcmNs */
