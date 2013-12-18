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

#ifndef PGDOMAIN_H_
#define PGDOMAIN_H_

#include <iostream>
#include <string>
#include "VirtualDb/Domain/Domain.h"

namespace DcmNs {

    class PgDomain : public Domain {
        public:
            PgDomain();
            PgDomain(std::string, std::string, std::string, std::string);
            virtual ~PgDomain();
            std::string getId();
            void setId(std::string);
            std::string getName();
            void setName(std::string);
            std::string getVlan();
            void setVlan(std::string);
            std::string getPortProfile();
            void setPortProfile(std::string);
            std::string getDcId();
            void setDcId(std::string);
            int getStatus();
            void setStatus(int);
            void isConfigUpdate(Domain*, bool&, bool&, bool&);
            std::string getPgNN();
            void printDomain();

        protected:
            std::string id;
            std::string name;
            std::string vlan;
            std::string port_prof;
            std::string dc_id;
            int status;
    };


} /* namespace DcmNs */
#endif /* PGDOMAIN_H_ */
