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
 * Dvpg.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "DvpgDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <vector>
#include <stdlib.h>

using namespace std;

namespace DcmNs {

    DvpgDAO::DvpgDAO() {
        table = "dvportgroups";
        order = "id";
    }

    DvpgDAO::DvpgDAO(string db_file) : DAO(db_file) {
        table = "dvportgroups";
        order = "id";
    }

    DvpgDAO::~DvpgDAO() {
    }

    int DvpgDAO::insertDvpg(DvpgDomain d) {
        cols = "'id', 'name', 'vlan', 'qos', 'dvs_id', 'dvs_nn', 'dc_id', 'nrpk','dvpgtype'";
        vals = "'" + d.getId() + "','" + d.getName() + "','" + d.getVlan() + "','" + d.getQos() + "','" + d.getDvsId() + "',' "+ d.getDvsNN() + "','" + d.getDcId() + "','" + d.getNetworkResourcePoolKey()+ "','" + d.getDVPGType() + "'";
        int status = insertRow ();
        return status;
    }

    int DvpgDAO::deleteDvpg(DvpgDomain d) {
        //TODO: delete all the dependencies in other tables
        where = "id = '" + d.getId() + "'";
        int status = deleteRow();
        return status;
    }

    DvpgDomain DvpgDAO::findDvpg(DvpgDomain d) {
        return findBy(d.getId());
    }

    int DvpgDAO::updateDvpgName(DvpgDomain d) {
        set = "'name' = '" + d.getName() + "'" ;
        where = "id = '" + d.getId() + "'";
        int status = updateRow();
        return status;
    }

    int DvpgDAO::updateDvsName(DvpgDomain d) {
        set = "'dvs_nn' = '" + d.getDvsNN() + "'" ;
        where = "id = '" + d.getId() + "'";
        int status = updateRow();
        return status;
    }

    int DvpgDAO::updateDvpgVlan(DvpgDomain d) {
        set = "'vlan' = '" + d.getVlan() + "'" ;
        where = "id = '" + d.getId() + "'";
        int status = updateRow();
        return status;
    }

    int DvpgDAO::updateDvpgQos(DvpgDomain d) {
        string qos =d.getQos();
        if (qos == "0")
            qos="";

        set = "'qos' = '" + qos + "'" ;
        where = "id = '" + d.getId() + "'";
        int status = updateRow();
        return status;
    }

    int DvpgDAO::updateDvpgPortProfile(DvpgDomain d) {
        set = "'port_prof' = '" + d.getPortProfile() + "'" ;
        where = "id = '" + d.getId() + "'";
        int status = updateRow();
        return status;
    }

    DvpgDomain DvpgDAO::findBy(string id) {
        DvpgDomain result;
        cols = "id, name, vlan, qos, dvs_id, dvs_nn, dc_id,dvpgtype";
        where = "id = '" + id + "'";
        no_of_col = 8;
        int more_rows = fetchRow();
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "fetch row returned %d ", more_rows);
        if (more_rows < 1) {
            return result;
        }

        while (more_rows > 0 ) {

            if (id.compare(string(items[0])) == 0) {
                result.setId(string(items[0]));
                result.setName(string(items[1]));
                result.setVlan(string(items[2]));
                result.setQos(string(items[3]));
                result.setDvsId(string(items[4]));
                result.setDvsNN(string(items[5]));
                result.setDcId(string(items[6]));
                result.setDVPGType(string(items[7]));
                VIC_PLUG_LOG(VIC_TRACE_DEBUG," Returning DVPG ID:%s, Name: %s, Vlan %s, QOS %s",result.getId().c_str(),
                        result.getName().c_str(),
                        result.getVlan().c_str(),
                        result.getQos().c_str());

                for (int i = 0; i < no_of_col; i++) {
                    free(items[i]);
                    items[i] = NULL;
                }
            }
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }

            more_rows = fetchRow();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"More rows %d",more_rows);
        }
        DAO::resetStmt();   
        return result;

    }


    vector<Domain *> DvpgDAO::findAllBy(string dvs_id, string dc_id) {
        std::vector<Domain*> dvpg_vector;
        cols = "id, name, vlan, qos, dvs_id, dvs_nn, dc_id,dvpgtype,nrpk";
        no_of_col = 9;

        where.clear();
        if (dvs_id != "")
            where = "dvs_id = '" + dvs_id + "'";
        if (where !="")
            where += "and ";

        if (dc_id != ""){
            where += "dc_id = '" + dc_id+ "'";
        }

        int more_rows = fetchRow();
        while (more_rows > 0 ) {

            DvpgDomain* result =  new DvpgDomain();
            result->setId(string(items[0]));
            result->setName(string(items[1]));
            result->setVlan(string(items[2]));
            result->setQos(string(items[3]));
            result->setDvsId(string(items[4]));
            result->setDvsNN(string(items[5]));
            result->setDcId(string(items[6]));
            result->setDVPGType(string(items[7]));
            result->setNetworkResourcePoolKey(string(items[8]));
            dvpg_vector.push_back(result);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return dvpg_vector;

    }

    vector<Domain*> DvpgDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> dvpg_vector;
        cols = "id, name, vlan, qos, dc_id,dvpgtype";
        where.clear();
        if (mor != "")
            where = "id = '" + mor + "'";
        no_of_col = 6;
        int more_rows = fetchRow();
        while (more_rows > 0 ) {

            Domain* dvpg =  new DvpgDomain();
            dvpg->setId(string(items[0]));
            dvpg->setName(string(items[1]));
            dvpg->setVlan(string(items[2]));
            dvpg->setQos(string(items[3]));
            dvpg->setDcId(string(items[4]));
            dvpg->setDVPGType(string(items[5]));
            dvpg_vector.push_back(dvpg);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return dvpg_vector;
    }
    vector<DvpgDomain> DvpgDAO::retrieveDvpgInfoForDiscovery() {
        std::vector<DvpgDomain> dvpg_vector;
        cols = "id, name, dvs_id, nrpk,dvpgtype";
        where.clear();
        no_of_col = 5;
        int more_rows = fetchRow();
        while (more_rows > 0 ) {

            DvpgDomain dvpg;
            dvpg.setId(string(items[0]));
            dvpg.setName(string(items[1]));
            dvpg.setDvsId(string(items[2]));
            dvpg.setNetworkResourcePoolKey(items[3]);
            dvpg.setDVPGType(items[4]);
            dvpg_vector.push_back(dvpg);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return dvpg_vector;
    }


} /* namespace DcmNs */
