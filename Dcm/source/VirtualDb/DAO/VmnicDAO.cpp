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
 * VmnicDAO.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "VmnicDAO.h"
#include "VirtualDb/sqlite/sq.h"

using namespace std;

namespace DcmNs {

    VmnicDAO::VmnicDAO() {
        table = "vmnics";
    }

    VmnicDAO::VmnicDAO(string db_file) : DAO(db_file) {
        table = "vmnics";
    }

    VmnicDAO::~VmnicDAO() {
    }

    int VmnicDAO::insertVmnic(VmnicDomain v) {
        cols = "'name', 'mac', 'host_id', 'iface', 'dvs', 'dvs_nn' ,'dc_id' ";
        vals = "'" + v.getName() + "','" + v.getMac() + "','" + v.getHostId() + "','" + v.getIface() + "','" + v.getDvs() + "','" + v.getDvsNN()+ "','" + v.getDcId() + "'" ;
        int status = insertRow ();
        return status;
    }

    int VmnicDAO::deleteVmnic(VmnicDomain v) {
        where = "host_id = '" + v.getHostId() + "'";

    	if (v.getDcId().size() != 0)
    		where += " and dc_id = '" + v.getDcId() + "'";
    	if (v.getName().size() != 0)
    		where += " and name = '" + v.getName() + "'";

        int status = deleteRow();
        return status;
    }

    VmnicDomain VmnicDAO::findVmnic(VmnicDomain v) {
        return v;
    }

    int VmnicDAO::updateVmnicIface(VmnicDomain v) {
        set = "'iface' = '" + v.getIface() + "'";
        where = "name = '" + v.getName() + "' AND host_id = '" + v.getHostId() + "'";
        int status = updateRow();
        return status;
    }

    int VmnicDAO::updateVmnicDvs(VmnicDomain v) {
        set = "'dvs' = '" + v.getDvs() + "','dvs_nn' = '" + v.getDvsNN() + "'";
        where = "name = '" + v.getName() + "' AND host_id = '" + v.getHostId() + "'";
        int status = updateRow();
        return status;
    }
   int VmnicDAO::updateVmnic(string vc, string ifName, string hostId, string name) {
   		int now = time(0);
        set = "'iface' = '" + ifName + "','createTS' = '" + now + "'";
        where = "name = '" + name + "' AND host_id = '" + hostId + "'";
        int status = updateRow();
        return status;
   }

    int VmnicDAO::removeVmnicVs(string vs, string host_id) {
        set  = "'dvs' = '', 'dvs_nn' = ''";
        where = "dvs = '" + vs + "' AND host_id = '" + host_id + "'";
        int status = updateRow();
        return status;
    }

    vector<Domain*> VmnicDAO::findAllBy(string host_id, string dc_id) {
        std::vector<Domain*> vnic_vector;
        cols = "name, mac, host_id, iface, dvs, dvs_nn ,dc_id ";

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

        no_of_col = 7;
        int more_rows = fetchRow();
    	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
    			"findAllBy rows %d", more_rows);
        while (more_rows) {
            Domain* vnic = new VmnicDomain();
            vnic->setName(string(items[0]));
            vnic->setMac(string(items[1]));
        	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
        			"MAC %s", items[1]);
            vnic->setHostId(string(items[2]));
//            vnic->setIface(string(items[3]));
            vnic->setDvs(string(items[4]));
            vnic->setDvsNN(string(items[5]));
            vnic->setDcId(string(items[6]));
            vnic_vector.push_back(vnic);
            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vnic_vector;
    }

    vector<Domain*> VmnicDAO::retrieveInfoForDeletion(string mor)
	{

	  std::vector<Domain*> vnic_vector;
	  cols = "name, iface";

	  where.clear();
	  no_of_col = 2;
	  int more_rows = fetchRow();
	  while (more_rows) {
		Domain* vmnic = new VmnicDomain();
		vmnic->setName(string(items[0]));
		vmnic->setIface(string(items[1]));
		vnic_vector.push_back(vmnic);

		more_rows = fetchRow();
	  }
	  return vnic_vector;
	}

    vector<Domain*> VmnicDAO::retrieveInfoForCorrelation(string mor) {
        std::vector<Domain*> vnic_vector;
        table = "vmnics";

        cols = "name, mac, host_id, iface, dvs, dvs_nn ,dc_id ";

//        cols = "name, mac, host_id,iface,dc_id";
		where.clear();

        if (mor != ""){
            where += "host_id = '" + mor+ "'";
        }
        Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
                "findAllBy %s:%s:%s", table.c_str(),cols.c_str(), where.c_str());


        no_of_col = 7;
        int more_rows = fetchRow();
        if (more_rows < 1)
        {
            Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"No Vmnic Found: %d",more_rows);
            return vnic_vector;
        }

        while (more_rows) {
        	Debug::VIC_PLUG_TRACE(VIC_TRACE_DEBUG,
        			"retrieve info for By %s:%s:%s:%s",
        			items[0], items[1], items[2], items[3]);
			Domain* vmnic = new VmnicDomain();

			vmnic->setName(string(items[0]));
			vmnic->setMac(string(items[1]));
			vmnic->setHostId(string(items[2]));
			vmnic->setIface(string(items[3]));
			vmnic->setDvs(string(items[4]));
			vmnic->setDvsNN(string(items[5]));
			vmnic->setDcId(string(items[6]));
			vnic_vector.push_back(vmnic);

            for (int i = 0; i < no_of_col; i++) {
                free(items[i]);
                items[i] = NULL;
            }
            more_rows = fetchRow();
        }
        return vnic_vector;
    }

} /* namespace DcmNs */
