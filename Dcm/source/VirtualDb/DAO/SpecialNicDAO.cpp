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
 * SpecialNicDAO.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "SpecialNicDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <vector>
#include <stdlib.h>

using namespace std;

namespace DcmNs {

    SpecialNicDAO::SpecialNicDAO() {
        table = "special_nics";
        order = "host_id, device";
    }

    SpecialNicDAO::SpecialNicDAO(string db_file) : DAO(db_file) {
        table = "special_nics";
        order = "host_id, device";
    }

    SpecialNicDAO::~SpecialNicDAO() {
    }

    int SpecialNicDAO::insertSpecialNic(SpecialNicDomain s) {
        cols = "'device', 'mac', 'ip', 'pg_nn', 'host_id', 'vs', 'vnic_type','dc_id', 'dvpg_id'";
        vals = "'" + s.getDevice() + "','" + s.getMac() + "','" + s.getIp() + "','" + s.getPgNN() + "','" + s.getHostId() + "','" + s.getVs() + "','" + s.getVnicType() + "','" + s.getDcId() +"','"+s.getDvpgId()+"'";
printf("-- insert special nic --");
        int status = insertRow ();
        return status;
    }

    int SpecialNicDAO::deleteSpecialNic(SpecialNicDomain v) {
        //TODO: delete all the dependencies in other tables
        where = "host_id = '" + v.getHostId() + "'";

    	if (v.getDcId().size() != 0)
    		where += " and dc_id = '" + v.getDcId() + "'";
    	if (v.getDevice().size() != 0)
    		where += " and device = '" + v.getDevice() + "'";
        int status = deleteRow();
        return status;
    }

    SpecialNicDomain SpecialNicDAO::findSpecialNic(SpecialNicDomain s) {
        return s;
    }

    int SpecialNicDAO::findCountOnPg(string pg_name, string dc_id) {
        cols = "count(*)";
        where = "pg_nn = '" + pg_name +"' AND dc_id = '" + dc_id + "'";
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

    int SpecialNicDAO::updateSpecialNic(SpecialNicDomain s) {
        set = "'pg_nn' = '" + s.getPgNN() + "'" ;
        where = "device = '" + s.getDevice() + "' AND mac ='"+s.getMac()+"' AND host_id = '" + s.getHostId() + "'";
        int status = updateRow();
        return status;
    }

    int SpecialNicDAO::updateSpecialNicPortProfile(SpecialNicDomain s) {
        set = "'port_prof' = '" + s.getPortProfile() + "'" ;
        where = "device = '" + s.getDevice() + "' AND host_id = '" + s.getHostId() + "'";
        int status = updateRow();
        return status;
    }

    int SpecialNicDAO::updateSpecialNicPortProfileOnPgNN(SpecialNicDomain p) {
        set = "'port_prof' = '" + p.getPortProfile() +  "'" ;
        where = "dc_id = '" + p.getDcId() + "' AND pg_nn = '" + p.getPgNN() + "'";
        int status = updateRow();
        return status;
    }

    int SpecialNicDAO::updateSpecialNicStatus(SpecialNicDomain s) {
        set = "'status' = '" + NumberToString(s.getStatus()) + "'" ;
        where = "device = '" + s.getDevice() + "' AND host_id = '" + s.getHostId() + "'";
        int status = updateRow();
        return status;
    }
    
    vector<Domain*> SpecialNicDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> specialnic_vector;
        cols = "host_id, device, mac, pg_nn, dc_id, dvpg_id";
        where.clear();
        if (mor != "")
            where = "host_id = '" + mor + "'";
        no_of_col = 6;
        int more_rows = fetchRow();
        while (more_rows > 0) {
            Domain* specialnic = new SpecialNicDomain();
            specialnic->setHostId(string(items[0]));
            specialnic->setDevice(string(items[1]));
            specialnic->setMac(string(items[2]));
            specialnic->setPgNN(string(items[3]));
            specialnic->setDcId(string(items[4]));
            specialnic->setDvpgId(string(items[5]));
            specialnic_vector.push_back(specialnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return specialnic_vector;
    }

    vector<Domain*> SpecialNicDAO::findAllBy(string host_id, string dc_id) {
        std::vector<Domain*> specialnic_vector;
        cols = "host_id, device, mac, pg_nn, dc_id, dvpg_id";
        where.clear();
		if (host_id != ""){
	        where = "host_id = '" + host_id+ "'";
		}
		if (where !="")
			where += "and ";

		if (dc_id != ""){
	        where += "dc_id = '" + dc_id+ "'";
		}
        no_of_col = 6;
        int more_rows = fetchRow();
        while (more_rows > 0) {
            Domain* specialnic = new SpecialNicDomain();
            specialnic->setHostId(string(items[0]));
            specialnic->setDevice(string(items[1]));
            specialnic->setMac(string(items[2]));
            specialnic->setPgNN(string(items[3]));
            specialnic->setDcId(string(items[4]));
            specialnic->setDvpgId(string(items[5]));
            specialnic_vector.push_back(specialnic);
//            specialnic->printDomain();
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return specialnic_vector;
    }

} /* namespace DcmNs */
