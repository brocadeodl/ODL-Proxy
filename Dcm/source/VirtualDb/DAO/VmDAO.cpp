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
 * Vm.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "VmDAO.h"
#include "VirtualDb/sqlite/sq.h"

using namespace std;

namespace DcmNs {

    VmDAO::VmDAO() {
        table = "vms";
    }

    VmDAO::VmDAO(string db_file) : DAO(db_file) {
        table = "vms";
    }

    VmDAO::~VmDAO() {
    }

    int VmDAO::insertVm(VmDomain v) {
        cols = "'id', 'name', 'host_id', 'host_nn', 'dc_id'";
        vals = "'" + v.getId() + "','" + v.getName() + "','" + v.getHostId() + "','" + v.getHostNN() + "','" + v.getDcId() + "'";
        int status = insertRow ();
        return status;
    }

    int VmDAO::deleteVm(VmDomain v) {
        //TODO: delete all the dependencies in other tables
        where = "host_id = '" + v.getHostId() + "'";

        if (v.getDcId().size() != 0)
            where += " and dc_id = '" + v.getDcId() + "'";
        if (v.getId().size() != 0)
            where += " and id = '" + v.getId() + "'";

        int status = deleteRow();
        return status;
    }

    VmDomain VmDAO::findVm(VmDomain v) {
        return v;
    }

    int VmDAO::updateVmName(VmDomain v) {
        set = "name = '" + v.getName() + "'";
        where = "id = '" + v.getId() + "'";
        int status = updateRow();
        return status;
    }

    int VmDAO::updateVmHost(VmDomain v) {
        set ="name = '"+v.getName()+ "',host_id = '" + v.getHostId() + "',host_nn = '" + v.getHostNN() + "'";
        where = "id = '" + v.getId() + "'";
        int status = updateRow();
        return status;
    }

    vector<Domain*> VmDAO::findAll() {
        std::vector<Domain*> vec;
        cols = "id, name, host_id, host_nn, dc_id";

        no_of_col = 5;
        int more_rows = fetchRow();
        while (more_rows > 0) {

            Domain* vnic = new VmDomain();
            vnic->setId(string(items[0]));
            vnic->setName(string(items[1]));
            vnic->setHostId(string(items[2]));
            vnic->setHostNN(string(items[3]));
            vnic->setDcId(string(items[4]));
            vec.push_back(vnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vec;
    }

    vector<Domain*> VmDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> vec;
        table = "vms";

        cols = "id, name, host_id, host_nn, dc_id";
        where.clear();
        no_of_col = 5;
        int more_rows = fetchRow();
        Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
                "more rows VmDAO %d",more_rows);
        if (more_rows < 1)
        {
            Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,"No VMs Found: %d",more_rows);
            return vec;
        }
        while (more_rows > 0) {

            Domain* vnic = new VmDomain();
            vnic->setId(string(items[0]));
            vnic->setName(string(items[1]));
            vnic->setHostId(string(items[2]));
            vnic->setHostNN(string(items[3]));
            vnic->setDcId(string(items[4]));
            vec.push_back(vnic);
            vnic->printDomain();
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vec;
    }

    vector<Domain*> VmDAO::findAllBy(string host_id, string dc_id) {
        std::vector<Domain*> vec;
        table = "vms";

        cols = "id, name, host_id, host_nn, dc_id";
        where.clear();
        if (host_id != ""){
            where = "host_id = '" + host_id+ "'";
        }
        if (where !="")
            where += "and ";

        if (dc_id != ""){
            where += "dc_id = '" + dc_id+ "'";
        }
        Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
                "findAllBy %s:%s:%s", table.c_str(),cols.c_str(), where.c_str());

        no_of_col = 5;
        int more_rows = fetchRow();
        Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
                "more rows VmDAO %d",more_rows);
        if (more_rows < 1)
        {
            Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,"No VMs Found: %d",more_rows);
            return vec;
        }
        while (more_rows > 0) {

            Domain* vnic = new VmDomain();
            vnic->setId(string(items[0]));
            vnic->setName(string(items[1]));
            vnic->setHostId(string(items[2]));
            vnic->setHostNN(string(items[3]));
            vnic->setDcId(string(items[4]));
            vec.push_back(vnic);
            vnic->printDomain();
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vec;

    }


} /* namespace DcmNs */
