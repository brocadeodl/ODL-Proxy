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
 * DiscoveryCorrelator.cpp
 *
 *  Created on: May 24, 2012
 *      Author: nfarhi
 */

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include "DiscoveryCorrelator.h"
#include "VirtualDb/DAO/DvpgDAO.h"
#include "VirtualDb/DAO/PgDAO.h"
#include "VirtualDb/DAO/VnicDAO.h"
#include "VirtualDb/DAO/SpecialNicDAO.h"
#include "VirtualDb/Domain/DvpgDomain.h"

#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "DcmCore/DcmToolKit.h"
#include "discover/utils/URLEncode.h"
#include "discover/vsphere/Debug.h"
#include "correlator/VcenterUtils.h"
#include "discover/utils/FileUtils.h"

#define PROFILE_NAME(vcenter_Name,datacenter,port_group)  "auto_" + vcenter_Name + "_" + URLEncode::urlencode(datacenter) + "_" + URLEncode::urlencode(port_group);

#define PROFILE_NAME_INTERMEDIATE(datacenter,port_group) URLEncode::urlencode(datacenter) + "_" +  URLEncode::urlencode(port_group);

using namespace std;

namespace DcmNs {

    DiscoveryCorrelator::DiscoveryCorrelator() {
    }

    DiscoveryCorrelator::DiscoveryCorrelator(string vcenter, string old_db_file,
            string new_db_file, bool _isNodeRejoin) {
        old_dbfile_r = old_db_file;
        new_dbfile_r = new_db_file;
		vcenter_r = vcenter;
        isNodeRejoin = _isNodeRejoin;
        switchAdapter = new SwitchAdapter(vcenter);
    }

    DiscoveryCorrelator::~DiscoveryCorrelator() {
        if(switchAdapter!=NULL){
            delete switchAdapter;
        }
    }

    void DiscoveryCorrelator::addListofProfiles(vector<Domain*> v) {
        //Domain can be PgDomain, DvpgDomain, and SpecialPgDomain
        vector<Domain*>::iterator it;
        for (it = v.begin(); it < v.end(); it++) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Id = %s, DC= %s, Name = %s, vlan = %s, QOS = %s",
                    (*it)->getId().c_str(), (*it)->getDcId().c_str(),
                    (*it)->getPgNN().c_str(), (*it)->getVlan().c_str(),
                    (*it)->getQos().c_str());
            if(isUpLink((*it))) continue;
            string vlanStr = VcenterUtils::convertToVlanUI32Range(
                        (*it)->getVlan());

            int status = switchAdapter->createProfileAndSubProfiles((*it)->getDcId(),
                        (*it)->getPgNN(), vlanStr, (*it)->getQos(),isNodeRejoin);

			if(status == WAVE_MESSAGE_SUCCESS)
				populatePortProfileInVnicAndSpecialNicTable ((*it)->getDcId(), (*it)->getPgNN());
        }
    }

    UI32 DiscoveryCorrelator::populatePortProfileInVnicAndSpecialNicTable (string dc_id, string pgName)
    {

	    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","AJ: DiscoveryCorrelator::populatePortProfileInVnicAndSpecialNicTable");
		VIC_PLUG_LOG(VIC_TRACE_INFO,"vCenter is %s",vcenter_r.c_str());
        string vcenterDB = FileUtils::getDBNewLocation(vcenter_r);
        VnicDAO dao(vcenterDB);

        string portProfile = PROFILE_NAME(vcenter_r,dc_id,pgName);
        VnicDomain vnic;
        vnic.setPgNN(pgName);
        vnic.setDcId(dc_id);
        vnic.setPortProfile(portProfile);

        dao.updateVnicPortProfileOnPgNN(vnic);
        DAO::closeDb();

        SpecialNicDAO dao2(vcenterDB);
        SpecialNicDomain spnic;

        spnic.setPgNN(pgName);
        spnic.setDcId(dc_id);
        spnic.setPortProfile(portProfile);

        dao2.updateSpecialNicPortProfileOnPgNN(spnic);
        DAO::closeDb();

        return 1;
    }

    void DiscoveryCorrelator::removeListofProfiles(vector<Domain*> v) {
        //Domain can be PgDomain, DvpgDomain, and SpecialPgDomain
        vector<Domain*>::iterator it;
        for (it = v.begin(); it < v.end(); it++) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Id = %s, DC= %s, Name = %s, vlan = %s",
                    (*it)->getId().c_str(), (*it)->getDcId().c_str(),
                    (*it)->getPgNN().c_str(), (*it)->getVlan().c_str());
            if(isUpLink((*it))) continue;
            switchAdapter->deActivatePortProfile((*it)->getDcId(), (*it)->getPgNN(),isNodeRejoin);
            switchAdapter->deletePortProfile((*it)->getDcId(), (*it)->getPgNN(),isNodeRejoin);
        }
    }

    void DiscoveryCorrelator::updateListofProfiles(vector<Domain*> v) {
        //Domain can be PgDomain and DvpgDomain
        vector<Domain*>::iterator it;
        for (it = v.begin(); it < v.end(); it++) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Id = %s, DC= %s, Name = %s, vlan = %s",
                    (*it)->getId().c_str(), (*it)->getDcId().c_str(),
                    (*it)->getPgNN().c_str(), (*it)->getVlan().c_str());
            if(isUpLink((*it))) continue;
            if (!((*it)->getQos()).empty())
                switchAdapter->updateQoSProfile((*it)->getDcId(),(*it)->getPgNN(),(*it)->getQos());
            string vlan_str = VcenterUtils::convertToVlanUI32Range((*it)->getVlan());
            switchAdapter->updateVlanProfile((*it)->getDcId(), (*it)->getPgNN(),
                        vlan_str);
        }
    }

    //ValClientSynchronousConnection *pValClientSynchronousConnection;
    void DiscoveryCorrelator::removeAllProfiles(daoType_t type) {
        //Domain can be PgDomain, DvpgDomain, and SpecialNic
        vector<Domain*> vector_old = getSource(type, old_dbfile_r);
        removeListofProfiles(vector_old);
        releaseSource(vector_old);
    }

    int DiscoveryCorrelator::deleteVCenter(UI32 vccount) {
        //Port Group
        removeAllProfiles(PG_DAO);

        //Distributed Post Group
        removeAllProfiles(DVPG_DAO);

        //SpecialNic Port Group
        removeAllProfiles(SPECIALPG_DAO);
		
		if(!(vccount>1))
        	switchAdapter->configCdpProtocol(false);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Completed Deletion of Port-Profiles");
        return 0;
    }

    int DiscoveryCorrelator::run(int type) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Correlator Run");
        switchAdapter->configCdpProtocol(true);
        if (type == FULL_DISCOVERY) {
            findPgUpdates();
            findDvpgUpdates();
            findSpecialPgUpdates();
            findVnicMacAssociationUpdates();
            findSpecialNicMacAssociationUpdates();
        }
        else {
            findPgUpdates();
            findSpecialNicMacAssociationUpdates();
        }
        return 0;
    }

    void DiscoveryCorrelator::findPgUpdates() {
        vector<Domain*> old_v, new_v, deleted, added, updated;
        findAddDeleteUpdates(PG_DAO, old_v, new_v, deleted, added, updated, true);
        configurePortProfiles(deleted, added, updated);
        //printDomainChanges(added, deleted, updated);
        releaseSource(old_v);
        releaseSource(new_v);
    }

    void DiscoveryCorrelator::findDvpgUpdates() {
        vector<Domain*> old_v, new_v, deleted, added, updated;
        findAddDeleteUpdates(DVPG_DAO, old_v, new_v, deleted, added, updated, true);
        configurePortProfiles(deleted, added, updated);
        //printDomainChanges(added, deleted, updated);
        releaseSource(old_v);
        releaseSource(new_v);
    }

    void DiscoveryCorrelator::findSpecialPgUpdates() {
        vector<Domain*> old_v, new_v, deleted, added, updated;
        findAddDeleteUpdates(SPECIALPG_DAO, old_v, new_v, deleted, added, updated, false);
        configurePortProfiles(deleted, added, updated);
        //printDomainChanges(added, deleted, updated);
        releaseSource(old_v);
        releaseSource(new_v);
    }

    void DiscoveryCorrelator::findVnicMacAssociationUpdates() {
        vector<Domain*> old_v, new_v, deleted, added, updated;
        findAddDeleteUpdates(VNIC_DAO, old_v, new_v, deleted, added, updated, true);
        configureMacAssociations(deleted, added, updated);
        //printDomainChanges(added, deleted, updated);
        releaseSource(old_v);
        releaseSource(new_v);
    }

    void DiscoveryCorrelator::findSpecialNicMacAssociationUpdates() {
        vector<Domain*> old_v, new_v, deleted, added, updated;
        findAddDeleteUpdates(SPECIALNIC_DAO, old_v, new_v, deleted, added, updated, true);
        configureMacAssociations(deleted, added, updated);
        //printDomainChanges(added, deleted, updated);
        releaseSource(old_v);
        releaseSource(new_v);
    }

    void DiscoveryCorrelator::findAddDeleteUpdates(daoType_t type, vector<Domain*>& vector_old, vector<Domain*>& vector_new, vector<Domain*>& deleted, vector<Domain*>&  added, vector<Domain*>& updated, bool update) {
        // retrieve all table_old keys into a set: A
        vector_old = getSource(type, old_dbfile_r); 
        DAO::closeDb();
        // retrieve all table_new keys into a set: B
        vector_new = getSource(type, new_dbfile_r);
        DAO::closeDb();

        // we don't need to sort the vectors b/c they are already sorted          
        // select queries on db have the order by clause

        // find A - B on id: deleted Domains
        deleted = diff(vector_old, vector_new);

        // find B - A on id: added Domains
        added = diff(vector_new, vector_old);

        // find A intersection B: C                 
        // find B intersection A: D
        // and then check for Domain field changes                 
        if (update)
            findUpdatedConfig(vector_old, vector_new, added, deleted, updated);
    }

    bool DiscoveryCorrelator::isUpLink(Domain *dom){

       if(dom->getDVPGType().compare("uplink")==0){

           return true;
       }
       return false;
    }

    void DiscoveryCorrelator::configurePortProfiles(vector<Domain*> deleted_vector, vector<Domain*>  added_vector, vector<Domain*> updated_vector){
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s %ld", "Delete Portprofile:",deleted_vector.size());
        removeListofProfiles(deleted_vector);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s %ld", "Create vlans Portprofile:",updated_vector.size());
        configureVlans(added_vector, updated_vector);

        VIC_PLUG_LOG(VIC_TRACE_INFO, "%s %ld", "Add Portprofiles:",added_vector.size());
        addListofProfiles(added_vector);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s %ld", "Change vlan/Qos for portprofiles:",updated_vector.size());
        updateListofProfiles(updated_vector);
    }

    void DiscoveryCorrelator::configureVlans (vector<Domain*>  added_vector, vector<Domain*> updated_vector) {
        vector<Domain*>::iterator it;
        std::string vlanScope = "";

        for (it = added_vector.begin(); it < added_vector.end();it++) {
            if(isUpLink((*it))) continue;
                vlanScope = vlanScope + "," + (*it)->getVlan();

        }

        for (it = updated_vector.begin();it < updated_vector.end(); it++) {
            if(isUpLink((*it))) continue;
                vlanScope = vlanScope + "," + (*it)->getVlan();

        }

        VIC_PLUG_LOG(VIC_TRACE_INFO,"Vlan scope1 str = %s",vlanScope.c_str());
        std::string vlanScopeStr = VcenterUtils::convertToVlanUI32Range(vlanScope);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"Vlan scope2 str = %s",vlanScopeStr.c_str());
        switchAdapter->createVlans(vlanScopeStr);
    }

    multimap<string, string> DiscoveryCorrelator::createMapOfProfileToMac(vector<Domain*> v) {
        multimap<string, string> portProfileMacMap;
        vector<Domain*>::iterator it;
        string mac;
        string dc_id_pgName;
        for (it = v.begin(); it != v.end(); it++) {
            if((*it)->getPgNN().empty()) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "PG NN is empty, using dvpg_id %s", (*it)->getDvpgId().c_str());
                string vcenterDB = FileUtils::getDBNewLocation(vcenter_r);
                // get pgname from dvpg_id
                DvpgDAO dao(vcenterDB);
                DvpgDomain d1 = dao.findBy((*it)->getDvpgId());
                (*it)->setPgNN(d1.getName());
                DAO::closeDb();

                SpecialNicDAO dao2(vcenterDB);
                SpecialNicDomain spnic;

                spnic.setPgNN(d1.getName());
                spnic.setDcId((*it)->getDcId());
                spnic.setHostId((*it)->getHostId());
                spnic.setDevice((*it)->getDevice());
                spnic.setMac((*it)->getMac());

                dao2.updateSpecialNic(spnic);
                DAO::closeDb();

            }
            mac = URLEncode::mac_conver_format((*it)->getMac());
            dc_id_pgName =
                PROFILE_NAME_INTERMEDIATE((*it)->getDcId(),(*it)->getPgNN());
            portProfileMacMap.insert(
                    pair<string, string>(dc_id_pgName, mac));
        }
        return portProfileMacMap;
    }

    void DiscoveryCorrelator::addOrDeleteListOfMacAssociations(vector<Domain*> v, bool add) {
        multimap<string, string> portProfileMacMap = createMapOfProfileToMac(v);
        multimap<string, string>::iterator it1;
        multimap<string, string>::iterator it2;
        pair<multimap<string, string>::iterator
                ,multimap<string, string>::iterator> retIt;
        vector<string> macVector;
        /* loop through unique port profiles in multimap create mac vector for
         * each and call ValClientSynchronousConnection API through switchAdapter
         */
        for (it1 = portProfileMacMap.begin(); it1 != portProfileMacMap.end();
                it1 = portProfileMacMap.upper_bound(it1->first)) {
            retIt = portProfileMacMap.equal_range(it1->first);
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Creating mac vector for: %s", it1->first.c_str());
            // push all the mac into the vector
            for (it2 = retIt.first; it2 != retIt.second; ++it2) {
                macVector.push_back((*it2).second);
            }
            // bulk request for each pp with vector of macs
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Creating mac vector for: %s with %d macs",
                    it1->first.c_str(), macVector.size());
            if (add) 
                switchAdapter->associateMacsToPortProfile(it1->first, macVector);
            else
                switchAdapter->deAssociateMacsFromPortProfile(it1->first,
                    macVector);
                
            macVector.clear();
        }
    }

    void DiscoveryCorrelator::configureMacAssociations(vector<Domain*> deleted_vector, vector<Domain*> added_vector, vector<Domain*> updated_vector){
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "Delete vnics::");
        addOrDeleteListOfMacAssociations(deleted_vector, false);

        VIC_PLUG_LOG(VIC_TRACE_INFO, "%s", "Add Nics::");
        addOrDeleteListOfMacAssociations(added_vector, true);
    }

} /* namespace DcmNs */
