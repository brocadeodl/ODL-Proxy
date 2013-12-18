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
 * PgDAO.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef PGDAO_H_
#define PGDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/PgDomain.h" 
#include <string>
#include <vector>

namespace DcmNs {

    class PgDAO : public DAO {
        public:
            PgDAO();
            PgDAO(std::string);    
            virtual ~PgDAO();
            int insertPg(PgDomain);
            int deletePg(PgDomain);
            PgDomain findPg(std::string);
            int updatePgName(PgDomain);
            int updatePgVlan(PgDomain);
            int updatePgPortProfile(PgDomain);
            std::vector<Domain*> retrieveInfoForCorrelation(std::string mor = "");
    };

} /* namespace DcmNs */
#endif /* PGDAO_H_ */
