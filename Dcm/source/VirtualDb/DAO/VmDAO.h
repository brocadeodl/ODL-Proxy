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
 * Vm.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef VMDAO_H_
#define VMDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/VmDomain.h" 
#include <string>

namespace DcmNs {

    class VmDAO : public DAO {
        public:
            VmDAO();
            VmDAO(std::string);
            virtual ~VmDAO();
            int insertVm(VmDomain);
            int deleteVm(VmDomain);
            VmDomain findVm(VmDomain);
            int updateVmName(VmDomain);
            int updateVmHost(VmDomain);
			vector<Domain*> findAll();
            vector<Domain*> findAllBy(string host_id, string dc_id);

            std::vector<Domain*> retrieveInfoForCorrelation(std::string mor = "");
            /* 
            TODO: method to return a collection of Datacneters
            */
    };

} /* namespace DcmNs */
#endif /* VMDAO_H_ */
