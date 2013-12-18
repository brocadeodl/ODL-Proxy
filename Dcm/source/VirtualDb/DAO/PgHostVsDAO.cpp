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
 * PgHostVsDAO.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "PgHostVsDAO.h"
#include "VirtualDb/sqlite/sq.h"

using namespace std;

namespace DcmNs {

    PgHostVsDAO::PgHostVsDAO() {
        table = "pgs_hosts_vss";
        order = "pg_id, host_id";
    }

    PgHostVsDAO::PgHostVsDAO(string db_file) : DAO(db_file) {
        table = "pgs_hosts_vss";
        order = "pg_id, host_id";
    }

    PgHostVsDAO::~PgHostVsDAO() {
    }

    int PgHostVsDAO::insertPgHostVs(PgHostVsDomain p) {
        cols = "'pg_id', 'host_id', 'host_nn', 'vs', 'vlan' ,'dc_id'";
        vals = "'" + p.getPgId() + "','" + p.getHostId() + "','" + p.getHostNN() + "','" + p.getVs() + "','" + p.getVlan() + "','" + p.getDcId() +"'";
        int status = insertRow ();
        return status;
    }

    int PgHostVsDAO::deletePgHostVsOnId(PgHostVsDomain v) {
        //TODO: delete all the dependencies in other tables
        where = "host_id = '" + v.getHostId() + "'";

    	if (v.getDcId().size() != 0)
    		where += " and dc_id = '" + v.getDcId() + "'";
    	if (v.getPgId().size() != 0)
    		where += " and pg_id = '" + v.getPgId() + "'";

        int status = deleteRow();
        return status;
    }

    int PgHostVsDAO::deletePgHostVsOnVs(string host_id, string vs) {
        //TODO: delete all the dependencies in other tables
        where = "host_id = '" + host_id + "' AND vs = '" + vs + "'";
        int status = deleteRow();
        return status;
    }

    PgHostVsDomain PgHostVsDAO::findPg(std::string dc_id, std::string host_id, std::string vs_id, std::string pg_id) {
        PgHostVsDomain pghostvs;
        cols = "'pg_id', 'host_id', 'vs', 'vlan' ,'dc_id'";

        where.clear();
        if (dc_id != ""){
            where = "dc_id ='" + dc_id+ "'";
        }
        if (where !="") 
            where += "and ";

        if (host_id != ""){
            where += "host_id ='" + host_id + "'";
        }
        if (where !="") 
            where += "and ";

        if (vs_id != ""){
            where += "vs ='" + vs_id + "'";
        }
        if (where !="") 
            where += "and ";
        if (pg_id != ""){
            where += "pg_id ='" + pg_id + "'";
        }
        no_of_col = 5;
        int more_rows = fetchRow();

        if (more_rows <1)
            return pghostvs;
        while (more_rows > 0 ) {
            pghostvs.setPgId(string(items[0]));
            pghostvs.setHostId(string(items[1]));
            pghostvs.setVs(string(items[2]));
            pghostvs.setVlan(string(items[3]));
            pghostvs.setDcId(string(items[4]));

            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return pghostvs;

}

    vector<PgHostVsDomain> PgHostVsDAO::findPgHostVsOnVs(string host_id, string vs) {
        std::vector<PgHostVsDomain> pghostvs_vector;
        table = "pgs_hosts_vss";
        cols = "pg_id, vlan";
        where = "host_id = '" + host_id + "' AND vs = '" + vs + "'";
        no_of_col = 2;
        int more_rows = fetchRow();
        while (more_rows > 0) {
            PgHostVsDomain phv;
            phv.setPgId(string(items[0]));
            phv.setVlan(string(items[1]));
            phv.setHostId(host_id);
            phv.setVs(vs);
            pghostvs_vector.push_back(phv);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return pghostvs_vector;
    }

    int PgHostVsDAO::findCountOnPg(string pg_id) {
        cols = "count(*)";
        where = "pg_id = '" + pg_id +"'";
        no_of_col = 1;
        int more_rows =  fetchRow();
        int row_count = 0;
        while (more_rows > 0) {
            row_count = atoi(items[0]);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return row_count;
    }

    int PgHostVsDAO::updatePgHostVs(PgHostVsDomain p) {
        set = "'vlan' = '" + p.getVlan() + "'" ;
        where = "host_id = '" + p.getHostId() + "'";
        where += "and dc_id = '" + p.getDcId() + "'";
        where += "and vs = '" + p.getVs() + "'";
    	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
    			"update  %s:%s:%s", table.c_str(),set.c_str(), where.c_str());
        int status = updateRow();
        return status;

    }

    vector<Domain*> PgHostVsDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> pghostvs_vector;
        table = "pgs_hosts_vss AS phv JOIN networks n ON phv.pg_id = n.id"; 
        cols = "phv.pg_id, phv.vlan, phv.host_id, phv.vs, n.name, phv.dc_id";
        where.clear();
        if (mor != "")
            where = "phv.host_id = '" + mor + "'";
        no_of_col = 6;
        int more_rows = fetchRow();
        while (more_rows > 0) {
            Domain* pghostvs = new PgHostVsDomain();
            pghostvs->setPgId(string(items[0]));
            pghostvs->setVlan(string(items[1]));
            pghostvs->setHostId(string(items[2]));
            pghostvs->setVs(string(items[3]));
            pghostvs->setPgNN(string(items[4]));
            pghostvs->setDcId(string(items[5]));
            pghostvs_vector.push_back(pghostvs);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return pghostvs_vector;
    }

    vector<Domain*> PgHostVsDAO::findAllBy(string host_id, string dc_id) {
        std::vector<Domain*> pghostvs_vector;
        table = "pgs_hosts_vss AS phv JOIN networks n ON phv.pg_id = n.id";
        cols = "phv.pg_id, phv.vlan, phv.host_id, phv.vs, n.name, phv.dc_id";
        where.clear();
		if (host_id != ""){
	        where = "phv.host_id ='" + host_id+ "'";
		}
		if (where !="")
			where += "and ";

		if (dc_id != ""){
	        where += "phv.dc_id='" + dc_id+ "'";
		}
        no_of_col = 6;
    	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
    			"findAllBy %s:%s:%s", table.c_str(),cols.c_str(), where.c_str());

        no_of_col = 7;
        int more_rows = fetchRow();
    	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
    			"findAllBy rows %d", more_rows);

        while (more_rows > 0) {
            Domain* pghostvs = new PgHostVsDomain();
        	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
        			"PGID %s", items[0]);
            pghostvs->setPgId(string(items[0]));
            pghostvs->setVlan(string(items[1]));
            pghostvs->setHostId(string(items[2]));
            pghostvs->setVs(string(items[3]));
            pghostvs->setPgNN(string(items[4]));
            pghostvs->setDcId(string(items[5]));
            pghostvs_vector.push_back(pghostvs);
//            pghostvs->printDomain();
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return pghostvs_vector;
    }
} /* namespace DcmNs */
