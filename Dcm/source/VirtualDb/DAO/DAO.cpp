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
 * DAO.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "DAO.h"
#include "VirtualDb/sqlite/sq.h"
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "VirtualDb/DAO/DAOInclude.h"
#include "discover/vsphere/Debug.h"

using namespace std;

namespace DcmNs {

    __thread sqlite3 * DAO::sqlite_r = NULL;
    __thread char DAO::dbfile_r[DB_FILE_SIZE] = "/tmp/dc.db";


    DAO::DAO() {
        int ret = 0;
        if (!sqlite_r) {
            if ((ret = SQ_opendb1 (dbfile_r, &sqlite_r, 32768, 10, 1)) == -1) {
                perror(dbfile_r);
                sqlite_r = NULL;
            }
        }
        memset(&items[0], 0, 50*sizeof(char*));
        index = NO_INDEX_IS_SET;
    }

    DAO::DAO(string db_file) {
        int ret = 0;
    	VIC_PLUG_LOG(VIC_TRACE_DEBUG,
    			"DAO(%s)",db_file.c_str());

        if (!db_file.empty()) {
            memset(dbfile_r, 0, DB_FILE_SIZE);
            strncpy(dbfile_r, db_file.c_str(), DB_FILE_SIZE);
        }

        if (!sqlite_r) {
            if ((ret = SQ_opendb1 (dbfile_r, &sqlite_r, 32768, 10, 1)) == -1) {
                perror(dbfile_r);
                sqlite_r = NULL;
            }
        }

        memset(&items[0], 0, 50*sizeof(char*));
        index = NO_INDEX_IS_SET;
    }

    DAO::~DAO() {
        for (int i = 0; i < 50; i++) {
            if (items[i]){
                free(items[i]);
                items[i] = NULL;
            }
        }
        if (sqlite_r)
        {
          SQ_closedb1(&sqlite_r);
        }
        resetQuery();
    }

    int DAO::insertRow () {
        if (sqlite_r) {
        	VIC_PLUG_LOG(VIC_TRACE_DEBUG,
        			"insertRow %s:%s:%s", table.c_str(),cols.c_str(), vals.c_str());

            return SQ_setRow1 (sqlite_r, table.c_str(), cols.c_str(), vals.c_str());
        }
        return -1;
    }

    int DAO::isDBInitiallized() {
        return (sqlite_r) ? 1: -1;
    }

    int DAO::deleteRow () {
        if (sqlite_r) {
        	VIC_PLUG_LOG(VIC_TRACE_DEBUG,
        			"deleteRow %s:%s", table.c_str(),where.c_str());
        
            return SQ_deleteRow1 (sqlite_r, table.c_str(), where.c_str());
        }
        return -1;
    }


    int DAO::deleteAllEntries() {
        if (sqlite_r)
            return SQ_deleteAllEntries(sqlite_r, table.c_str());
        return -1;
    }

    int DAO::updateRow () {
        	VIC_PLUG_LOG(VIC_TRACE_DEBUG,
        			"UpdateRow %s:%s:%s", table.c_str(),where.c_str(),
                    set.c_str());
        if (sqlite_r) {
            return SQ_updateRow1 (sqlite_r, table.c_str(), set.c_str(), where.c_str());
        }else {
        	return -1;
        }
    }

    int DAO::fetchRow () {
        if (sqlite_r) {
        	VIC_PLUG_LOG(VIC_TRACE_DEBUG,
        			"fetchRow %s:%s:%s", table.c_str(),cols.c_str(), where.c_str());
            return SQ_getRow1 (sqlite_r, table.c_str(), cols.c_str(),
            		(where.size()==0)?NULL:where.c_str(),
            		(order.size()==0)?NULL:order.c_str(),
            		items, no_of_col, index,
                    (groupby.size()==0)?NULL:groupby.c_str());

        }
        return 0;
    }

    string DAO::NumberToString ( int Number ) {
        stringstream ss;
        ss << Number;
        return ss.str();
    }

    void DAO::resetStmt() {
		resetQuery();
    }

    int DAO::closeDb() {
        if (sqlite_r)
        {
          SQ_closedb1(&sqlite_r);
        }
		resetQuery();
        return 1;
    }

    DAO* DaoFactory::getDao(daoType_t type, string db_file) {
        switch (type) {
            case SPECIALNIC_DAO:
                return new SpecialNicDAO(db_file);
            case DVPG_DAO:
                return new DvpgDAO(db_file);
            case PG_DAO:
                return new PgDAO(db_file);
            case PGHOSTVS_DAO:
                return new PgHostVsDAO(db_file);
            case VS_DAO:
                return new VsDAO(db_file);
            case VNIC_DAO:
                return new VnicDAO(db_file);
            case SPECIALPG_DAO:
                return new SpecialPgDAO(db_file);
			case VMNIC_DAO:
				return new VmnicDAO(db_file);
			case VM_DAO:
				return new VmDAO(db_file);
			case HOST_DAO:
				return new HostDAO(db_file);
            default:
                return NULL;
        }
    }


} /* namespace DcmNs */
