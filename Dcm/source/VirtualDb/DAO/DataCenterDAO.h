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
 * DataCenterDAO.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef DATACENTERDAO_H_
#define DATACENTERDAO_H_

#include "DAO.h"
#include "VirtualDb/Domain/DataCenterDomain.h" 
#include <string>

namespace DcmNs {

    class DataCenterDAO : public DAO {
        public:
            DataCenterDAO();
            DataCenterDAO(std::string);
            virtual ~DataCenterDAO();
            int insertDatacenter(DataCenterDomain);
            int deleteDatacenter(DataCenterDomain);
            DataCenterDomain findDatacenter(DataCenterDomain);
            int updateDatacenter(DataCenterDomain);
            /* 
            TODO: method to return a collection of Datacneters
            */
    };

} /* namespace DcmNs */
#endif /* DATACENTERDAO_H_ */
