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
#include "DVPGViewDAO.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

DVPGViewDAO::DVPGViewDAO() {
    table = (char *) "dvportgroups";
}

DVPGViewDAO::DVPGViewDAO(string db_file) :
	CLIDAO(db_file) {
    table = (char *) "dvportgroups";
}

DVPGViewDAO::~DVPGViewDAO() {
}

vector<std::string> DVPGViewDAO::getTitles() {
	std::vector<std::string> res_vector;
	res_vector.push_back(DVPG_STR);
	res_vector.push_back(DC_STR);
	res_vector.push_back(DVS_STR);
	res_vector.push_back(VLAN_STR);
	res_vector.push_back(QOS_STR);
	return res_vector;

}

vector<std::string> DVPGViewDAO::getNetConfTitles() {
    std::vector<std::string> res_vector;
    res_vector.push_back("name");
    res_vector.push_back("dc_id");
	res_vector.push_back("dvs_nn");
	res_vector.push_back("vlan");
	res_vector.push_back("qos");
    return res_vector;

}

vector<int> DVPGViewDAO::getColSizes() {
	std::vector<int> res_vector;
	res_vector.push_back(BIG_NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NAME_SIZE);
	res_vector.push_back(NUM_SIZE);
	return res_vector;
}

void DVPGViewDAO::loadColumns() {
	cols = (char *) "name,dc_id,dvs_nn,vlan,qos";
}

int DVPGViewDAO::getColumnCount() {
	return 5;
}

} /* namespace DcmNs */
