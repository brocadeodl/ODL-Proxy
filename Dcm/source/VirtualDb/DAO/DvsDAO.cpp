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
 * Dvs.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "DvsDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

    DvsDAO::DvsDAO() {
        table = "dvss";
    }

    DvsDAO::DvsDAO(string db_file) : DAO(db_file) {
        table = "dvss";
    }

    DvsDAO::~DvsDAO() {
    }

    int DvsDAO::insertDvs(DvsDomain d) {
        cols = "'id', 'name', 'pgroup_id', 'pgroup_nn', 'dc_id'";
        vals = "'" + d.getId() + "','" + d.getName() + "','" + d.getPgroupId() + "','" + d.getPgroupNN() + "','" +  d.getDcId() + "'";
        int status = insertRow ();
        return status;
    }

    int DvsDAO::deleteDvs(DvsDomain d) {
        //TODO: delete all the dependencies in other tables
        where = "id = '" + d.getId() + "'";
        int status = deleteRow();
        return status;
    }

    DvsDomain DvsDAO::findDvs(DvsDomain d) {
        DvsDomain result;
        cols = "id, name, pgroup_id, pgroup_nn, dc_id";
        where = "id = '" + d.getId() + "'";
        no_of_col = 5;
        int more_rows = fetchRow();
        while (more_rows > 0 ) {

            result.setId(string(items[0]));
            result.setName(string(items[1]));
            result.setPgroupId(string(items[2]));
            result.setPgroupNN(string(items[3]));
            result.setDcId(string(items[4]));
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return result;
    }

    int DvsDAO::updateDvs(DvsDomain d) {
        set = "'name' = '" + d.getName() +  "'" ;
        where = "id = '" + d.getId() + "'";
        int status = updateRow();
        return status;
    }

} /* namespace DcmNs */
