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
#include "VSViewDAO.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

VSViewDAO::VSViewDAO() {
    table = (char *) "vss_vmnics";
}

VSViewDAO::VSViewDAO(string db_file) :
	CLIDAO(db_file) {
    table = (char *) "vss_vmnics";
}

VSViewDAO::~VSViewDAO() {
}

vector<std::string> VSViewDAO::getTitles() {
	std::vector<std::string> res_vector;
	res_vector.push_back(VS_STR);
	res_vector.push_back(DC_STR);
	res_vector.push_back(HOST_STR);
	res_vector.push_back(UPLINK_STR);
	res_vector.push_back(INTF_STR);
	return res_vector;

}

vector<std::string> VSViewDAO::getNetConfTitles() {
    std::vector<std::string> res_vector;
    res_vector.push_back("name");
    res_vector.push_back("dc_id");
	res_vector.push_back("host");
	res_vector.push_back("pnic");
	res_vector.push_back("iface");
    return res_vector;

}

vector<int> VSViewDAO::getColSizes() {
	std::vector<int> res_vector;
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(BIG_NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	return res_vector;
}

void VSViewDAO::loadColumns() {
	cols = (char *) "name,dc_id,host,pnic,iface";
}

int VSViewDAO::getColumnCount() {
	return 5;
}


} /* namespace DcmNs */
