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



#include <string>
#include "DCViewDAO.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

DCViewDAO::DCViewDAO() {
    table = "dc_view";
}

DCViewDAO::DCViewDAO(string db_file) :
	CLIDAO(db_file) {
    table = "dc_view";
}

DCViewDAO::~DCViewDAO() {
}

vector<std::string> DCViewDAO::getTitles() {
	std::vector<std::string> res_vector;

	res_vector.push_back(ID_STR);
	res_vector.push_back(NAME_STR);
	res_vector.push_back(NUM_HOSTS_STR);
	res_vector.push_back(NUM_VMS_STR);
	res_vector.push_back(NUM_PGS_STR);
	res_vector.push_back(NUM_DVPGS_STR);
	res_vector.push_back(NUM_DVS_STR);
	res_vector.push_back(NUM_VSS_STR);
	return res_vector;

}

vector<std::string> DCViewDAO::getNetConfTitles() {
    std::vector<std::string> res_vector;
    res_vector.push_back("id");
    res_vector.push_back("name");
    return res_vector;

}

vector<int> DCViewDAO::getColSizes() {
	std::vector<int> res_vector;
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NUM_SIZE);
	res_vector.push_back(NUM_SIZE);
	res_vector.push_back(NUM_SIZE);
	res_vector.push_back(NUM_SIZE);
	res_vector.push_back(NUM_SIZE);
	res_vector.push_back(NUM_SIZE);
	return res_vector;
}

//select datacenters.id as id, datacenters.name as name, count(distinct hosts.id) as hcount, count(distinct vms.id) as vcount, count(distinct dvportgroups.id) as dvcount, count(distinct dvss.id) as dvsscount, count(distinct vss.name) as vsscount, count(distinct networks.name) as pgcount

void DCViewDAO::loadColumns() {
	cols = "id,name,hcount,vcount,pgcount,dvcount,dvsscount,vsscount";
}

int DCViewDAO::getColumnCount() {
	return 8;
}


vector<vector<string> > DCViewDAO::findAll(string dc_id, string name, int index1) {
	std::vector<std::vector<std::string> > res_vector;
	std::vector<std::vector<std::string> > dc_vector;

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count Datacenters");

    table = "datacenters";
	cols = "id,name";
	
	if (name.size() != 0)
		where = " id = '" + name + "'";
	cout << "findAll " << where << endl;
		
	no_of_col = 2;
	index = index1;
	int more_rows = fetchRow();
	int rcount=0;
	
    while (more_rows > 0 ) {
		std::vector<std::string> row_vector;
		for (int i = 0; i < no_of_col; i++) {
			row_vector.push_back(string(items[i]));
		}
		dc_vector.push_back(row_vector);
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
		rcount++;
	}

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count hosts");
    map<string, string> hcount_map;
    table = "hosts";
	cols = "dc_id,count(*)";
	if (name.size() != 0)
		where = " dc_id = '" + name + "'";	
	no_of_col = 2;
    groupby = "dc_id";
	more_rows = fetchRow();
	while (more_rows) {
		hcount_map.insert(pair<string, string>(items[0], items[1]));
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count vms");
    map<string, string> vcount_map;
    table = "vms";
	cols = "dc_id,count(*)";
	if (name.size() != 0)
		where = " dc_id = '" + name + "'";	
	
	no_of_col = 2;
    groupby = "dc_id";
	more_rows = fetchRow();
	while (more_rows) {
		vcount_map.insert(pair<string, string>(items[0], items[1]));
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count networks");
    map<string, string> ncount_map;
    table = "networks";
	cols = "dc_id,count(*)";
	if (name.size() != 0)
		where = " dc_id = '" + name + "'";	

	no_of_col = 2;
    groupby = "dc_id";
	more_rows = fetchRow();
	while (more_rows) {
		ncount_map.insert(pair<string, string>(items[0], items[1]));
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count dvpgs");
    map<string, string> dvpgcount_map;
    table = "dvportgroups";
	cols = "dc_id,count(*)";
	if (name.size() != 0)
		where = " dc_id = '" + name + "'";	

	no_of_col = 2;
    groupby = "dc_id";
	more_rows = fetchRow();
	while (more_rows) {
		dvpgcount_map.insert(pair<string, string>(items[0], items[1]));
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count dvss");
    map<string, string> dvsscount_map;
    table = "dvss";
	cols = "dc_id,count(*)";
	if (name.size() != 0)
		where = " dc_id = '" + name + "'";	
	
	no_of_col = 2;
    groupby = "dc_id";
	more_rows = fetchRow();
	while (more_rows) {
		dvsscount_map.insert(pair<string, string>(items[0], items[1]));
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}

    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Count vss");
    map<string, string> vsscount_map;
    table = "vss";
	cols = "dc_id,count(*)";
	if (name.size() != 0)
		where = " dc_id = '" + name + "'";	
	
	no_of_col = 2;
    groupby = "dc_id";
	more_rows = fetchRow();
	while (more_rows) {
		vsscount_map.insert(pair<string, string>(items[0], items[1]));
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}


	for (int i = 0; i < rcount; i++) {
		std::vector<std::string> row_vector;
		std::vector<std::string> dc_strs = dc_vector.at(i);
		row_vector.push_back(dc_strs.at(0)); // dc_id
		row_vector.push_back(dc_strs.at(1)); // dc_name
		row_vector.push_back((hcount_map.count(dc_strs.at(0))>0)?hcount_map.find(dc_strs.at(0))->second.c_str():"0"); // count(host)
		row_vector.push_back((vcount_map.count(dc_strs.at(0))>0)?vcount_map.find(dc_strs.at(0))->second.c_str():"0"); // count(vms)
		row_vector.push_back((ncount_map.count(dc_strs.at(0))>0)?ncount_map.find(dc_strs.at(0))->second.c_str():"0"); // count(networks)
		row_vector.push_back((dvpgcount_map.count(dc_strs.at(0))>0)?dvpgcount_map.find(dc_strs.at(0))->second.c_str():"0"); // count(dvpgs)
		row_vector.push_back((dvsscount_map.count(dc_strs.at(0))>0)?dvsscount_map.find(dc_strs.at(0))->second.c_str():"0"); // count(dvss)
		row_vector.push_back((vsscount_map.count(dc_strs.at(0))>0)?vsscount_map.find(dc_strs.at(0))->second.c_str():"0"); // count(vss)
		res_vector.push_back(row_vector);
	}


	return res_vector;
}

} /* namespace DcmNs */
