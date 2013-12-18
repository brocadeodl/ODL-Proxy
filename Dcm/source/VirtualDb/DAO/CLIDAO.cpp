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
#include "CLIDAO.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

CLIDAO::CLIDAO() {
}

CLIDAO::~CLIDAO() {
}

CLIDAO::CLIDAO(string db_file) :
	DAO(db_file) {
}

vector<vector<string> > CLIDAO::findAll(string dc_id, string name, int index1) {
	std::vector<std::vector<std::string> > res_vector;
    Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Find ALL in CLI-DAO");
	loadColumns();
//    cols = (char *) "name,dc_id,vs,vlan,host_nn";

	if (dc_id.size() != 0)
		where = "dc_id = '" + dc_id + "'";
	if (dc_id.size() != 0 && name.size() != 0)
		where += " and ";
	if (name.size() != 0)
		where += " name = '" + name + "'";
	cout << "findAll " << where << endl;

	no_of_col = getColumnCount();
	index = index1;
	Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Find ALL in CLI-DAO index is %d",index);
	int more_rows = fetchRow();
	while (more_rows > 0 ) {
		std::vector<std::string> row_vector;
		for (int i = 0; i < no_of_col; i++) {
			row_vector.push_back(string(items[i]));
		}
		res_vector.push_back(row_vector);
		for (int i = 0; i < no_of_col; i++) {
			free(items[i]);
			items[i] = NULL;
		}
		more_rows = fetchRow();
	}
	return res_vector;
}
} /* namespace DcmNs */
