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
 * Vs.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "VsDAO.h"
#include "VirtualDb/sqlite/sq.h"

using namespace std;

namespace DcmNs {

    VsDAO::VsDAO() {
        table = "vss";
        order = "host_id, name";
    }

    VsDAO::VsDAO(string db_file) : DAO(db_file) {
        table = "vss";
        order = "host_id, name";
    }

    VsDAO::~VsDAO() {
    }

    int VsDAO::insertVs(VsDomain v) {
        cols = "'name', 'host_id', 'host' ,'dc_id'";
        vals = "'" + v.getName() + "','" + v.getHostId() + "','" + v.getHost() + "','" + v.getDcId() +"'";
        int status = insertRow ();
        return status;
    }

    int VsDAO::deleteVs(VsDomain v) {
        //TODO: delete all the dependencies in other tables
        where = "host_id = '" + v.getHostId() + "'";

    	if (v.getDcId().size() != 0)
    		where += " and dc_id = '" + v.getDcId() + "'";
    	if (v.getName().size() != 0)
    		where += " and name = '" + v.getName() + "'";

        int status = deleteRow();
        return status;
    }

    VsDomain VsDAO::findVs(VsDomain v) {
        return v;
    }

    int VsDAO::updateVs(VsDomain v) {
        int status = 1;
        return status;
    }

    vector<Domain*> VsDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> vs_vector;
        cols = "name, host_id ,dc_id";
        where.clear();
        if (mor != "") 
            where = "host_id = '" + mor + "'";
        no_of_col = 3;
        int more_rows = fetchRow();
        while (more_rows > 0) {

            Domain* vs = new VsDomain();
            vs->setName(string(items[0]));
            vs->setHostId(string(items[1]));
            vs->setDcId(string(items[2]));
            vs_vector.push_back(vs);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vs_vector;
    }


} /* namespace DcmNs */
