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
#include "NetworkViewDAO.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

NetworkViewDAO::NetworkViewDAO() {
    table = (char *) "pgs_hosts_networks";
}

NetworkViewDAO::NetworkViewDAO(string db_file) :
	CLIDAO(db_file) {
    table = (char *) "pgs_hosts_networks";
}

NetworkViewDAO::~NetworkViewDAO() {
}

vector<std::string> NetworkViewDAO::getTitles() {
	std::vector<std::string> res_vector;
	res_vector.push_back(PG_STR);
	res_vector.push_back(DC_STR);
	res_vector.push_back(VS_STR);
	res_vector.push_back(VLAN_STR);
	res_vector.push_back(HOST_STR);
	return res_vector;

}

vector<std::string> NetworkViewDAO::getNetConfTitles() {
    std::vector<std::string> res_vector;
    res_vector.push_back("name");
    res_vector.push_back("dc_id");
	res_vector.push_back("vs");
	res_vector.push_back("vlan");
	res_vector.push_back("host_nn");
    return res_vector;

}

vector<int> NetworkViewDAO::getColSizes() {
	std::vector<int> res_vector;
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(BIG_NAME_SIZE);
	return res_vector;
}


void NetworkViewDAO::loadColumns() {
	cols = (char *) "name,dc_id,vs,vlan,host_nn";
}

int NetworkViewDAO::getColumnCount() {
	return 5;
}


} /* namespace DcmNs */
