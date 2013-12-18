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
 * DataCenter.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include <string>
#include "DataCenterDAO.h"
#include "VirtualDb/sqlite/sq.h"
#include "discover/vsphere/Debug.h"

using namespace std;

namespace DcmNs {

    DataCenterDAO::DataCenterDAO() {
        table = "datacenters";
    }
    
    DataCenterDAO::DataCenterDAO(string db_file) : DAO(db_file) {
        table = "datacenters";
    }

    DataCenterDAO::~DataCenterDAO() {
    }

    int DataCenterDAO::insertDatacenter(DataCenterDomain dc) {
        cols = "'id', 'name'";
        vals = "'" + dc.getId() + "','" + dc.getName() + "'";
        int status = insertRow ();
        return status;
    }

    int DataCenterDAO::deleteDatacenter(DataCenterDomain dc) {
        //TODO: delete all the dependencies in other tables
        where = "id = '" + dc.getId() + "'";
        int status = deleteRow();
        return status;
    }

    DataCenterDomain DataCenterDAO::findDatacenter(DataCenterDomain dc) {
        return dc;
    }

    int DataCenterDAO::updateDatacenter(DataCenterDomain dc) {
        set = "'name' = '" + dc.getName() + "'";
        where = "id = '" + dc.getId() + "'";
    	Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"updating Datacenter %s:%s", set.c_str(), where.c_str());
        int status = updateRow();
        return status;
//        return 1;
    }

} /* namespace DcmNs */
