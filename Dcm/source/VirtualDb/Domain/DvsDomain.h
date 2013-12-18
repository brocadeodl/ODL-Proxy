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
 * DataCenter.h
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#ifndef DVSDOMAIN_H_
#define DVSDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"
namespace DcmNs {

    class DvsDomain : public Domain {
		private:
		protected:
			virtual void   setupAttributesForSerialization();
        public:
            DvsDomain();
            DvsDomain(std::string, std::string, std::string, std::string, std::string);
            virtual ~DvsDomain();
            std::string getId();
            void setId(std::string);
            std::string getName();
            void setName(std::string);
            std::string getPgroupId();
            void setPgroupId(std::string);
            std::string getPgroupNN();
            void setPgroupNN(std::string);
            std::string getDcId();
            void setDcId(std::string);

        public:
            std::string id;
            std::string name;
            std::string pgroup_id;
            std::string pgroup_nn;
            std::string dc_id;
    };

} /* namespace DcmNs */
#endif /* DVSDOMAIN_H_ */