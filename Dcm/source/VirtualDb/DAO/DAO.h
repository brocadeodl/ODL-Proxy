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
 * DAO.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef DAO_H_
#define DAO_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "VirtualDb/sqlite/sq.h"
#include "VirtualDb/Domain/Domain.h"
#include "discover/vsphere/Debug.h"
#define DB_FILE_SIZE 50

namespace DcmNs {

    class DAO {
        public:
            DAO();
            DAO(std::string);
            virtual ~DAO();

            int insertRow();
            int deleteRow();
            int deleteAllEntries();
            int updateRow();
            int fetchRow();
            static int closeDb();
            static void resetStmt();
            int isDBInitiallized();
            virtual std::vector<Domain*> retrieveInfoForCorrelation(std::string mor= "") {
                std::vector<Domain*> v;
                return v;
            }
            virtual std::vector<Domain*> findAllBy(std::string host_id= "", std::string dc_id= "") {
                std::vector<Domain*> v;
                return v;
            }
			virtual std::vector<Domain*> retrieveInfoForDeletion(std::string mor
				= "")
			{
				std::vector<Domain*> v;
				return v;
			}


        protected:

            std::string NumberToString ( int Number );

            static __thread sqlite3 *sqlite_r;
            static __thread char dbfile_r[DB_FILE_SIZE];

            std::string table;
            std::string cols;
            std::string vals;
            std::string where;
            std::string set;
            std::string order;
            std::string groupby;
            int no_of_col;
            char *items[50]; // make sure to free this memory and use it only within an object
            int index;
    };

    typedef enum daoType_ {
        DC_DAO = 0,
        DVPG_DAO,
        HOST_DAO,
        PG_DAO,
        PGHOSTVS_DAO,
        SPECIALNIC_DAO,
        SPECIALPG_DAO,
        VM_DAO,
        VMNIC_DAO,
        VNIC_DAO,
        VS_DAO
    } daoType_t;

    class DaoFactory {
        public:
            DaoFactory();
            virtual ~DaoFactory();
            static DAO* getDao(daoType_t, std::string);
    };

} /* namespace DcmNs */
#endif /* DAO_H_ */
