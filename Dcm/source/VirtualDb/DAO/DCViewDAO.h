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


#ifndef DCVIEWDAO_H_
#define DCVIEWDAO_H_

#include "DAO.h"
#include <string>
#include <vector>
#include "CLIDAO.h"
namespace DcmNs {

    class DCViewDAO : public CLIDAO {
        public:
            DCViewDAO();
            DCViewDAO(std::string);
            virtual ~DCViewDAO();
            std::vector<std::vector<std::string> > findAll(std::string dc_id, std::string name, int index);
            std::vector<std::string> 	getTitles();
			std::vector<std::string>    getNetConfTitles();
            std::vector<int> 		getColSizes();
            void loadColumns();
            int getColumnCount();
    };

} /* namespace DcmNs */
#endif /* DCVIEWDAO_H_ */
