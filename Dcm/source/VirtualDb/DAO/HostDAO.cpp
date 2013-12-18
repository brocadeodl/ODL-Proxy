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
 * Host.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "HostDAO.h"
#include "VirtualDb/sqlite/sq.h"

using namespace std;

namespace DcmNs {

    HostDAO::HostDAO() {
        table = "hosts";
    }

    HostDAO::HostDAO(string db_file) : DAO(db_file) {
        table = "hosts";
    }

    HostDAO::~HostDAO() {
    }

    int HostDAO::insertHost(HostDomain h) {
        cols = "'id', 'name', 'dc_id'";
        vals = "'" + h.getId() + "','" + h.getName() + "','" + h.getDcId() + "'";
        int status = insertRow ();
        return status;
    }

    int HostDAO::deleteHost(HostDomain h) {
        //TODO: delete all the dependencies in other tables
        where = "id = '" + h.getId() + "'";
        int status = deleteRow();
        return status;
    }

    HostDomain HostDAO::findHost(HostDomain h) {
        return h;
    }

    int HostDAO::updateHost(HostDomain h) {
        set = "'name' = '" + h.getName() + "','dc_id' = '" + h.getDcId() + "'" ;
        where = "id = '" + h.getId() + "'";
        int status = updateRow();
        return status;
    }

    vector<Domain*> HostDAO::findAllBy(string host_id, string dc_id) {
        std::vector<Domain*> vec;
        cols = "id, name, dc_id";
        where.clear();
        if (host_id != ""){
            where = "id = '" + host_id+ "'";
        }

        if (dc_id != ""){
            if (where !="")
                where += "and ";
            where += "dc_id = '" + dc_id+ "'";
        }
        no_of_col = 3;
        int more_rows = fetchRow();
        while (more_rows > 0 ) {

            Domain* dom = new HostDomain();
            dom->setId(string(items[0]));
            dom->setName(string(items[1]));
            dom->setDcId(string(items[2]));
            vec.push_back(dom);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vec;
    }

    vector<Domain*> HostDAO::retrieveInfoForCorrelation(string mor) {
        return findAllBy(mor, "");
    }

} /* namespace DcmNs */
