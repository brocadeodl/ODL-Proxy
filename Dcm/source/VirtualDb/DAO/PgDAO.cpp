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
 * PgDAO.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "PgDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "VirtualDb/Domain/PgDomain.h" 

using namespace std;

namespace DcmNs {

    PgDAO::PgDAO() {
        table = "networks";
        order = "id";
    }

    PgDAO::PgDAO(string db_file) : DAO(db_file) {
        table = "networks";
        order = "id";
    }

    PgDAO::~PgDAO() {
    }

    int PgDAO::insertPg(PgDomain p) {
        cols = "'id', 'name', 'vlan', 'dc_id'";
        vals = "'" + p.getId() + "','" + p.getName() + "','" + p.getVlan() + "','" + p.getDcId() + "'";
        int status = insertRow ();
        return status;
    }

    int PgDAO::deletePg(PgDomain p) {
        //TODO: delete all the dependencies in other tables
        where = "id = '" + p.getId() + "'";
        int status = deleteRow();
        return status;
    }

    PgDomain PgDAO::findPg(string id_) {
        PgDomain pg;
        cols = "id, name, vlan ,dc_id";
        where.clear();
        where = "id = '" + id_ + "'";
        no_of_col = 4;
        order.clear();
        int more_rows = fetchRow();

        if (more_rows <1)
            return pg;
        while (more_rows > 0 ) {

            pg.setId(string(items[0]));
            pg.setName(string(items[1]));
            pg.setVlan(string(items[2]));
            pg.setDcId(string(items[3]));
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return pg;
    }

    int PgDAO::updatePgName(PgDomain p) {
        set = "'name' = '" + p.getName() + "'" ;
        where = "id = '" + p.getId() + "'";
        int status = updateRow();
        return status;
    }

    int PgDAO::updatePgVlan(PgDomain p) {
        set = "'vlan' = '" + p.getVlan() + "'" ;
        where = "id = '" + p.getId() + "'";
        int status = updateRow();
        return status;
    }

    int PgDAO::updatePgPortProfile(PgDomain p) {
        set = "'port_prof' = '" + p.getPortProfile() + "'" ;
        where = "id = '" + p.getId() + "'";
        int status = updateRow();
        return status;
    }

    vector<Domain*> PgDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> pg_vector;
        cols = "id, name, vlan ,dc_id";
        where.clear();
        no_of_col = 4;
        int more_rows = fetchRow();
        while (more_rows > 0 ) {

            Domain* pg = new PgDomain();
            pg->setId(string(items[0]));
            pg->setName(string(items[1]));
            pg->setVlan(string(items[2]));
            pg->setDcId(string(items[3]));
            pg_vector.push_back(pg);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return pg_vector;
    }

} /* namespace DcmNs */
