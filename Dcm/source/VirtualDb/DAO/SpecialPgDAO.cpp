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
 * SpecialPgDAO.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: nfarhi
 */

#include <string>
#include "SpecialPgDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <vector>
#include <stdlib.h>

using namespace std;

namespace DcmNs {

    SpecialPgDAO::SpecialPgDAO() {
        table = "special_pgs";
        order = "dc_id, pg_nn";
    }

    SpecialPgDAO::SpecialPgDAO(string db_file) : DAO(db_file) {
        table = "special_pgs";
        order = "dc_id, pg_nn";
    }

    SpecialPgDAO::~SpecialPgDAO() {
    }

    int SpecialPgDAO::insertSpecialPg(SpecialPgDomain s) {
        cols = "'pg_nn','host_id', 'host_nn', 'vs', 'vlan','dc_id'";
        vals = "'" +  s.getPgNN() + "','" + s.getHostId() + "','" + s.getHostNN() + "','" + s.getVs() + "','" + s.getVlan()  + "','" + s.getDcId() +"'";
        int status = insertRow ();
        return status;
    }

    int SpecialPgDAO::deleteSpecialPg(SpecialPgDomain s) {
        //TODO: delete all the dependencies in other tables
        where = "pg_nn = '" + s.getPgNN() + "' AND dc_id = '" + s.getDcId() + "'";
        int status = deleteRow();
        return status;
    }

    int SpecialPgDAO::existSpecialPg(SpecialPgDomain s) {
        cols = "count(*)";
        where.clear();
        where = "pg_nn = '" + s.getPgNN() + "' AND dc_id = '" + s.getDcId() + "'";
        no_of_col = 1;
        int more_rows = fetchRow();
        int row_count = 0;
        while (more_rows) {
            row_count = atoi(items[0]);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return row_count;
    }

    int SpecialPgDAO::updateSpecialPgPortProfile(SpecialPgDomain s) {
        set = "'port_prof' = '" + s.getPortProfile() + "'" ;
        where = "pg_nn = '" + s.getPgNN() + "' AND dc_id = '" + s.getDcId() + "'";
        int status = updateRow();
        return status;
    }
    int SpecialPgDAO::updateSpecialPgVS(SpecialPgDomain s) {
        set = "'vs' = '" + s.getVs() + "'" ;
        where = "pg_nn = '" + s.getPgNN() + "' AND dc_id = '" + s.getDcId() + "'";
        int status = updateRow();
        return status;
    }
    int SpecialPgDAO::updateSpecialPgVLan(SpecialPgDomain s) {
        set = "'vlan' = '" + s.getVlan() + "'" ;
        where = "pg_nn = '" + s.getPgNN() + "' AND dc_id = '" + s.getDcId() + "'";
        int status = updateRow();
        return status;
    }
    int SpecialPgDAO::findCountOnPgName(string pg_nn) {
        cols = "count(*)";
        where = "pg_nn = '" + pg_nn +"'";
        no_of_col = 1;
        int more_rows =  fetchRow();
        int row_count = 0;
        while (more_rows) {
            row_count = atoi(items[0]);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return row_count;
    }

    SpecialPgDomain SpecialPgDAO::findPg(string pgNN,string dc_id,string host_id) {
        SpecialPgDomain specialpg;
        cols = "pg_nn, vlan, host_id, vs, dc_id";
        where.clear();
        where = "pg_nn = '" + pgNN + "' AND dc_id = '" + dc_id + "'";
        no_of_col = 5;
        order.clear();
        int more_rows = fetchRow();
        if (more_rows <1)
            return specialpg;
        while (more_rows) {
            specialpg.setPgNN(string(items[0]));
            specialpg.setVlan(string(items[1]));
            specialpg.setHostId(string(items[2]));
            specialpg.setVs(string(items[3]));
            specialpg.setDcId(string(items[4]));
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return specialpg;
    }

    int SpecialPgDAO::updateSpecialPgStatus(SpecialPgDomain s) {
        set = "'status' = '" + NumberToString(s.getStatus()) + "'" ;
        where = "pg_nn = '" + s.getPgNN() + "' AND dc_id = '" + s.getDcId() + "'";
        int status = updateRow();
        return status;
    }

    vector<Domain*> SpecialPgDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> specialpg_vector;
        cols = "pg_nn, vlan, host_id, vs, dc_id";
        where.clear();
        no_of_col = 5;

        if (mor != "")
            where = "host_id = '" + mor + "'";

        int more_rows = fetchRow();
        if (more_rows <1)
            return specialpg_vector;
        while (more_rows) {
            Domain* specialpg = new SpecialPgDomain();
            specialpg->setPgNN(string(items[0]));
            specialpg->setVlan(string(items[1]));
            specialpg->setHostId(string(items[2]));
            specialpg->setVs(string(items[3]));
            specialpg->setDcId(string(items[4]));
            specialpg_vector.push_back(specialpg);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return specialpg_vector;
    }

} /* namespace DcmNs */
