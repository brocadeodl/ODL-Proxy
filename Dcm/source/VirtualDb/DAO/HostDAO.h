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
 * Host.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef HOSTDAO_H_
#define HOSTDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/HostDomain.h"
#include <string> 

namespace DcmNs {

    class HostDAO : public DAO {
        public:
            HostDAO();
            HostDAO(std::string);
            virtual ~HostDAO();
            int insertHost(HostDomain);
            int deleteHost(HostDomain);
            HostDomain findHost(HostDomain);
            int updateHost(HostDomain);
            std::vector<Domain*> findAllBy(std::string host_id= "", std::string dc_id= "");
            std::vector<Domain*> retrieveInfoForCorrelation(std::string mor = "");

            /* 
            TODO: method to return a collection of Datacneters
            */
    };

} /* namespace DcmNs */
#endif /* HOSTDAO_H_ */
