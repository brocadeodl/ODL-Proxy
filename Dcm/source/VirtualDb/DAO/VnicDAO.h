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
 * VnicDao.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef VNICDAO_H_
#define VNICDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/VnicDomain.h"
#include <string> 
#include <vector>

namespace DcmNs {

    class VnicDAO : public DAO {
        public:
            VnicDAO();
            VnicDAO(std::string);
            virtual ~VnicDAO();
            int insertVnic(VnicDomain);
            int deleteVnic(VnicDomain);
            VnicDomain findVnic(VnicDomain);
    		vector<Domain*> findAllBy(string pg_id, string dc_id);
            vector<Domain *> findAllBy(string vm_id);
            std::vector<VnicDomain> findVnicsOnPg(std::string dvpg);
            int updateVnicMac(VnicDomain);
            int updateVnicIp(VnicDomain);
            int updateVnicDvpg(VnicDomain);
            int updateVnicPortProfile(VnicDomain);
			int updateVnicPortProfileOnPgNN(VnicDomain);
            int updateVnicDvs(VnicDomain);
            int updateVnicStatus(VnicDomain);
            std::vector<Domain*> retrieveInfoForCorrelation(std::string mor = "");
    };

} /* namespace DcmNs */
#endif /* VNICDAO_H_ */
