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
#include "VMPolicyViewDAO.h"
#include <stdlib.h>

using namespace std;

namespace DcmNs {

    VMPolicyViewDAO::VMPolicyViewDAO() {
        table = (char *) "vms_vnics_vmkernel";
    }

    VMPolicyViewDAO::VMPolicyViewDAO(string db_file) :
        CLIDAO(db_file) {
            table = (char *) "vms_vnics_vmkernel";
        }

    VMPolicyViewDAO::~VMPolicyViewDAO() {
    }

    vector<std::string> VMPolicyViewDAO::getTitles() {
        std::vector<std::string> res_vector;
        res_vector.push_back(ASSOC_MAC_STR);
        res_vector.push_back(DC_STR);
        res_vector.push_back(VM_STR);
        res_vector.push_back(DVPG1_STR);
        res_vector.push_back(TYPE_STR);
        res_vector.push_back(PP_STR);
        return res_vector;

    }

    vector<std::string> VMPolicyViewDAO::getNetConfTitles() {
        std::vector<std::string> res_vector;
        res_vector.push_back("mac");
        res_vector.push_back("dc_id");
        res_vector.push_back("name");
        res_vector.push_back("dvpg_nn");
        res_vector.push_back("type");
        res_vector.push_back("port_prof");
        return res_vector;

    }

    vector<int> VMPolicyViewDAO::getColSizes() {
        std::vector<int> res_vector;
        res_vector.push_back(NAME_SIZE);
        res_vector.push_back(NAME_SIZE);
        res_vector.push_back(NAME_SIZE);
        res_vector.push_back(NAME_SIZE);
        res_vector.push_back(NAME_SIZE);
        res_vector.push_back(NAME_SIZE);
        return res_vector;
    }

    void VMPolicyViewDAO::loadColumns() {
        cols = (char *) "mac,dc_id,name,dvpg_nn,type,port_prof";
    }

    int VMPolicyViewDAO::getColumnCount() {
        return 6;
    }

    vector<vector<string> > VMPolicyViewDAO::findAll(string dc_id, string name, int index1) {
        std::vector<std::vector<std::string> > res_vector;
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"Find ALL in VMPolicyView-DAO");
        loadColumns();

        if (dc_id.size() != 0)
            where = "dc_id = '" + dc_id + "'";
        if (dc_id.size() != 0 && name.size() != 0)
            where += " and ";
        if (name.size() != 0)
            where += " mac = '" + name + "'";
        cout << "findAll " << where << endl;

        no_of_col = getColumnCount();
        index = index1;
        int more_rows = fetchRow();
        while (more_rows > 0) {

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
