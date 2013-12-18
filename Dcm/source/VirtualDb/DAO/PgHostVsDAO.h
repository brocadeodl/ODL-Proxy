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
 * PgHostVsDAO.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef PGHOSTVSDAO_H_
#define PGHOSTVSDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/PgHostVsDomain.h"
#include <string> 

namespace DcmNs {

    class PgHostVsDAO : public DAO {
        public:
            PgHostVsDAO();
            PgHostVsDAO(std::string);
            virtual ~PgHostVsDAO();
            int insertPgHostVs(PgHostVsDomain);
            int deletePgHostVsOnId(PgHostVsDomain);
            int deletePgHostVsOnVs(std::string, std::string);
            std::vector<PgHostVsDomain> findPgHostVsOnVs(std::string, std::string);
            int updatePgHostVs(PgHostVsDomain);
            PgHostVsDomain findPg(std::string dc_id, std::string host_id, std::string vs_id, std::string pg_id);

            std::vector<Domain*> retrieveInfoForCorrelation(std::string mor = "");
            int findCountOnPg(std::string);
            std::vector<Domain*> findAllBy(std::string host_id= "", std::string dc_id= "");

            /* 
            TODO: method to return a collection of Datacneters
            */
    };

} /* namespace DcmNs */
#endif /* PGHOSTVSDAO_H_ */
