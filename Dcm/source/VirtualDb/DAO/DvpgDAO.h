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
 * Dvpg.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef DVPGDAO_H_
#define DVPGDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/DvpgDomain.h"
#include <string>
#include <vector> 

namespace DcmNs {

    class DvpgDAO : public DAO {
        public:
            DvpgDAO();
            DvpgDAO(std::string);
            virtual ~DvpgDAO();
            int insertDvpg(DvpgDomain);
            int deleteDvpg(DvpgDomain);
            DvpgDomain findDvpg(DvpgDomain);
            int updateDvpgName(DvpgDomain);
            int updateDvpgVlan(DvpgDomain);
            int updateDvpgQos(DvpgDomain);
            int updateDvpgPortProfile(DvpgDomain);
            int updateDvsName(DvpgDomain);
            std::vector<Domain*> retrieveInfoForCorrelation(std::string mor = "");
            std::vector<DvpgDomain> retrieveDvpgInfoForDiscovery();
            DvpgDomain findBy(string id);
            vector<Domain *> findAllBy(string dvs_id, string dc_id);

    };

} /* namespace DcmNs */
#endif /* DVPGDAO_H_ */
