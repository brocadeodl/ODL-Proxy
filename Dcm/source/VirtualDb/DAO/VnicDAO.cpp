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
 * Vnic.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include <vector>
#include "VnicDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <stdlib.h>
#include "discover/vsphere/Debug.h"
using namespace std;

namespace DcmNs {

    VnicDAO::VnicDAO() {
        table = "vnics";
        order = "vm_id, name";
    }

    VnicDAO::~VnicDAO() {
    }

    VnicDAO::VnicDAO(string db_file) : DAO(db_file) {
        table = "vnics";
        order = "vm_id, name";
    }

    int VnicDAO::insertVnic(VnicDomain p) {
        cols = "'name', 'mac', 'ip', 'dvpg_id', 'dvpg_nn', 'vm_id', 'dvs' ,'dc_id' ";
        vals = "'" + p.getName() + "','" + p.getMac() + "','" + p.getIp() + "','" + p.getDvpgId() + "','" + p.getDvpgNN() + "','" + p.getVmId() + "','" + p.getDvs() +"','" + p.getDcId() + "'";
        int status = insertRow ();
        return status;
    }

    int VnicDAO::deleteVnic(VnicDomain p) {
        //TODO: delete all the dependencies in other tables
        //        where =  "name = '" + p.getName() + "' and vm_id = '" + p.getVmId() + "' and mac = '" + p.getMac() + "'";
        where = "mac = '" + p.getMac() + "'";
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,
                "deleteVnic  %s", where.c_str());
        int status = deleteRow();
        return status;
    }

    VnicDomain VnicDAO::findVnic(VnicDomain p) {
        return p;
    }

    vector<VnicDomain> VnicDAO::findVnicsOnPg(string dvpg) {
        std::vector<VnicDomain> vnic_vector;
        cols = "name, mac, vm_id";
        where = "dvpg_id = '" + dvpg + "'";
        no_of_col =  3;
        int more_rows = fetchRow();
        while (more_rows) {
            VnicDomain vnic;
            vnic.setName(string(items[0]));
            vnic.setMac(string(items[1]));
            vnic.setVmId(string(items[2]));
            vnic_vector.push_back(vnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow(); 
        }
        return vnic_vector;
    }

    int VnicDAO::updateVnicMac(VnicDomain p) {
        set = "'mac' = '" + p.getMac() +  "'" ;
        where = "name = '" + p.getName() + "' AND vm_id = '" + p.getVmId() + "'";
        int status = updateRow();
        return status;
    }

    int VnicDAO::updateVnicPortProfileOnPgNN(VnicDomain p) {
        set = "'port_prof' = '" + p.getPortProfile() +  "'" ;
        where = "dc_id = '" + p.getDcId() + "' AND dvpg_nn = '" + p.getDvpgNN() + "'";
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "updateVnic where=  %s", where.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "updateVnic set= %s",set.c_str()); 
        int status = updateRow();
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "updateRow return status = %d",status);
        return status;
    }

    int VnicDAO::updateVnicIp(VnicDomain p) {
        set = "'ip' = '" + p.getIp() +  "'" ;
        where = "name = '" + p.getName() + "' AND vm_id = '" + p.getVmId() + "'";
        int status = updateRow();
        return status;
    }

    int VnicDAO::updateVnicDvpg(VnicDomain p) {
        set = "'dvpg_id' = '" + p.getDvpgId() +  "','dvpg_nn' = '" + p.getDvpgNN() + "'" ;
        where = "name = '" + p.getName() + "' AND vm_id = '" + p.getVmId() + "'";
        int status = updateRow();
        return status;
    }

    int VnicDAO::updateVnicPortProfile(VnicDomain p) {
        set = "'port_prof' = '" + p.getPortProfile() +  "'" ;
        where = "name = '" + p.getName() + "' AND vm_id = '" + p.getVmId() + "'";
        int status = updateRow();
        return status;
    }

    int VnicDAO::updateVnicDvs(VnicDomain p) {
        set = "'dvs' = '" + p.getDvs() +  "'" ;
        where = "name = '" + p.getName() + "' AND vm_id = '" + p.getVmId() + "'";
        int status = updateRow();
        return status;
    }

    int VnicDAO::updateVnicStatus(VnicDomain p) {
        set = "'status' = '" + NumberToString(p.getStatus()) +  "'" ;
        where = "name = '" + p.getName() + "' AND vm_id = '" + p.getVmId() + "'";
        int status = updateRow();
        return status;
    }

    vector<Domain*> VnicDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> vnic_vector;
        cols = "vm_id, name, mac, dvpg_id, dvpg_nn ,dc_id";
        where.clear();
        if (mor != ""){
            where = "vm_id = '" + mor + "'";
        }
        no_of_col = 6;
        int more_rows = fetchRow();
        if (more_rows < 1)
        {
            Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"No Vnic Found: %d",more_rows);
            return vnic_vector;
        }
        while (more_rows) {
            Domain* vnic = new VnicDomain();
            vnic->setVmId(string(items[0]));
            vnic->setName(string(items[1]));
            vnic->setMac(string(items[2]));
            vnic->setDvpgId(string(items[3]));
            vnic->setDvpgNN(string(items[4]));
            vnic->setDcId(string(items[5]));
            vnic_vector.push_back(vnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vnic_vector;
    }

    vector<Domain*> VnicDAO::findAllBy(string pg_id, string dc_id) {
        std::vector<Domain*> vec;
        cols = "vm_id, name, mac, dvpg_id, dvpg_nn ,dc_id";
        where.clear();
        if (pg_id != ""){
            where = "dvpg_id = '" + pg_id+ "'";
        }
        if (where !="")
            where += "and ";

        if (dc_id != ""){
            where += "dc_id = '" + dc_id+ "'";
        }
        no_of_col = 6;
        int more_rows = fetchRow();
        while (more_rows) {
            Domain* vnic = new VnicDomain();
            vnic->setVmId(string(items[0]));
            vnic->setName(string(items[1]));
            vnic->setMac(string(items[2]));
            vnic->setDvpgId(string(items[3]));
            vnic->setDvpgNN(string(items[4]));
            vnic->setDcId(string(items[5]));
            vec.push_back(vnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vec;
    }
    vector<Domain*> VnicDAO::findAllBy(string vm_id) {
        std::vector<Domain*> vec;
        cols = "vm_id, name, mac, dvpg_id, dvpg_nn ,dc_id";
        where.clear();
        if (vm_id != ""){
            where = "vm_id = '" + vm_id+ "'";
        }
        no_of_col = 6;
        int more_rows = fetchRow();
        while (more_rows) {
            Domain* vnic = new VnicDomain();
            vnic->setVmId(string(items[0]));
            vnic->setName(string(items[1]));
            vnic->setMac(string(items[2]));
            vnic->setDvpgId(string(items[3]));
            vnic->setDvpgNN(string(items[4]));
            vnic->setDcId(string(items[5]));
            vec.push_back(vnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vec;
    }

} /* namespace DcmNs */
