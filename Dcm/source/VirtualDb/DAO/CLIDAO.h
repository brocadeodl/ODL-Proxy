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


#ifndef CLIDAO_H_
#define CLIDAO_H_

#include "DAO.h"
#include <string>
#include <vector>
#include "CLIDefines.h"
#include "discover/vsphere/Debug.h"

namespace DcmNs {

    class CLIDAO : public DAO {
        public:
            CLIDAO();
            CLIDAO(std::string);
            virtual ~CLIDAO();
            virtual std::vector<std::vector<std::string> > findAll(std::string dc_id, std::string name, int index);
            virtual std::vector<std::string> 	getTitles()=0;
			virtual std::vector<std::string>    getNetConfTitles()=0;
            virtual std::vector<int> 		getColSizes()=0;
            virtual void loadColumns()=0;
            virtual int getColumnCount()=0;
    };

} /* namespace DcmNs */
#endif /* CLIDAO_H_ */
