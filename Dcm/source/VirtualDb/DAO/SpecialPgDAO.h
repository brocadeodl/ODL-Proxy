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
 * SpecialPgDAO.h
 *
 *  Created on: Sep 1, 2012
 *      Author: nfarhi
 */

#ifndef SPECIALPGDAO_H_
#define SPECIALPGDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/SpecialPgDomain.h"
#include <string>
#include <vector> 

namespace DcmNs {

    class SpecialPgDAO : public DAO {
        public:
            SpecialPgDAO();
            SpecialPgDAO(std::string);
            virtual ~SpecialPgDAO();
            int insertSpecialPg(SpecialPgDomain);
            int deleteSpecialPg(SpecialPgDomain);
            int existSpecialPg(SpecialPgDomain);
            int updateSpecialPgPortProfile(SpecialPgDomain);
            int updateSpecialPgStatus(SpecialPgDomain);
            int updateSpecialPgVS(SpecialPgDomain s);
            int updateSpecialPgVLan(SpecialPgDomain s);
            vector<Domain*> retrieveInfoForCorrelation(string mor = "");
            SpecialPgDomain findPg(string pgNN,string dc_id,string host_id);
            int findCountOnPgName(string pg_nn);
    };

} /* namespace DcmNs */
#endif /* SPECIALPGDAO_H_ */
